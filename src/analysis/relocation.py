"""Defect relocation across repair rounds.

Motivation: the gate re-analyzes every repaired program from scratch, so a
finding the model "fixes" at one place can come back somewhere else (pushed to
the caller, wrapped in a new helper, swapped for another API with the same
CWE). This script measures how often that happens, per model and cell.

A finding is keyed by (tool:rule, enclosing function, normalized source line,
occurrence index); line numbers alone are useless because every repair shifts
them. For each transition round r -> r+1 of a program that entered repair:

  reloc     a rule is removed at one location and added at another
  crossfn   ... added in a function where that rule was not removed
  newfn     ... added in a function that did not exist in round r
  subst     a Flawfinder rule is removed and another rule with the same CWE
            is added (e.g. strcpy -> memcpy, both CWE-120)
  memmove   Flawfinder memcpy removed while memmove use grew (Flawfinder
            does not flag memmove)
  reappear  a finding absent in round r that was present in an earlier round

Compile errors and functional-test failures are not findings here. The
function map is a brace/regex heuristic, so events must be validated by hand
before they are reported as defects; relocation_events.csv lists every
cross-function event for that sampling.

Usage:
    python -m src.analysis.relocation [--cap 5] [--slug SLUG ...]
        [--models ID ...] [--tasks subset] [--name relocation]

Output: results/sac2027/relocation.md, relocation.json,
        relocation_events.csv
"""
from __future__ import annotations

import argparse
import csv
import difflib
import json
from collections import Counter, defaultdict
from pathlib import Path

from src.pipeline.finding_track import func_map, located, parse_reports

REPO = Path(__file__).resolve().parents[2]
COLL = REPO / "data" / "collected_code_6"
RES = REPO / "results"
OUT = RES / "sac2027"  # v3 (SAC 2027) outputs; manifests stay in RES

MODELS = ["deepseek-v4-pro", "qwen-max", "glm-5.1", "kimi-k3",
          "openai-gpt56-sol", "claude-fable-5", "gemini-3-pro"]
SLUGS = ["feedback-static-r0-t1.0-p1.0", "feedback-static+dynamic-r0-t1.0-p1.0"]
STATIC = [".gcc.txt", ".clang.txt", ".cppcheck.txt", ".flawfinder.txt"]
# .test.txt is the functional oracle (pass/fail), not a located finding.
DYNAMIC = [".asan.txt", ".fuzz.txt"]
FLAGS = ["reloc", "crossfn", "newfn", "subst", "memmove", "reappear"]

INPLACE_SIM = 0.6  # added line this similar to a removed one = rewrite in place


def round_dir(cell: Path, r: int) -> Path:
    return cell / "heal_0" if r == 0 else cell / f"heal_{r}" / "healed"


def analyze_cell(model: str, slug: str, cap: int, events: list,
                 keep: set[int] | None = None) -> dict | None:
    cell = COLL / model / slug
    mf = RES / model / f"{slug}.manifest.json"
    if not mf.is_file() or not cell.is_dir():
        return None
    suffixes = STATIC + (DYNAMIC if "dynamic" in slug else [])
    outcomes = json.loads(mf.read_text())["outcomes"]
    if keep is not None:
        outcomes = {b: v for b, v in outcomes.items()
                    if int(b.split("-")[1]) in keep}
    st = Counter()
    progs = defaultdict(set)
    crossfn_by_tool = Counter()
    added_kind = Counter()
    for base, v in outcomes.items():
        if v.get("outcome") == "clean_at_start":
            continue
        progs["entered"].add(base)
        hist, prev, prev_fns, prev_cwe, prev_src = [], None, set(), {}, ""
        for r in range(cap + 1):
            srcp = round_dir(cell, r) / f"{base}.c"
            if not srcp.is_file():
                break
            src = srcp.read_text(errors="replace")
            found, cwe_of = parse_reports(round_dir(cell, r), base, suffixes)
            cur = Counter(k for k in located(found, src) if k[0] != "COMPILE")
            fns = set(func_map(src.splitlines())) - {"<global>"}
            if prev is not None:
                st["transitions"] += 1
                common = prev & cur
                removed, added = prev - common, cur - common
                rem_by, add_by = defaultdict(list), defaultdict(list)
                for k, n in removed.items():
                    rem_by[k[0]] += [k] * n
                for k, n in added.items():
                    add_by[k[0]] += [k] * n
                flags = set()
                for rule in sorted(set(rem_by) & set(add_by)):
                    flags.add("reloc")
                    for a in add_by[rule]:
                        sims = [difflib.SequenceMatcher(None, a[2], b[2]).ratio()
                                for b in rem_by[rule] if b[1] == a[1]]
                        added_kind["in_place" if sims and max(sims) >= INPLACE_SIM
                                   else "same_fn" if sims
                                   else "other_fn" if a[1] != "<global>"
                                   else "global"] += 1
                    rf = {k[1] for k in rem_by[rule]}
                    af = {k[1] for k in add_by[rule]} - {"<global>"}
                    if af - rf:
                        flags.add("crossfn")
                        crossfn_by_tool[rule.split(":")[0]] += 1
                        into_new = bool((af - rf) - prev_fns)
                        if into_new:
                            flags.add("newfn")
                        events.append({"model": model, "slug": slug,
                                       "program": base, "round": r, "rule": rule,
                                       "from_fns": "|".join(sorted(rf)),
                                       "to_fns": "|".join(sorted(af - rf)),
                                       "new_fn": into_new,
                                       "outcome": v.get("outcome", "")})
                rem_only, add_only = set(rem_by) - set(add_by), set(add_by) - set(rem_by)
                if any(prev_cwe.get(a) and prev_cwe.get(a) == cwe_of.get(b)
                       for a in rem_only for b in add_only):
                    flags.add("subst")
                if "flawfinder:memcpy" in rem_by and src.count("memmove") > prev_src.count("memmove"):
                    flags.add("memmove")
                earlier = Counter()
                for h in hist[:-1]:
                    earlier |= h
                if any(k in earlier for k in added):
                    flags.add("reappear")
                for fl in flags:
                    progs[fl].add(base)
            hist.append(cur)
            prev, prev_fns, prev_cwe, prev_src = cur, fns, cwe_of, src
    # Accepted = the gate reported no finding within the analyzed horizon, so
    # events and outcomes cover the same rounds.
    accepted = {b for b, v in outcomes.items() if v.get("outcome") == "clean"
                and 0 < (v.get("rounds_to_clean") or 0) <= cap}
    return {"model": model, "slug": slug, "cap": cap,
            "entered": len(progs["entered"]), "transitions": st["transitions"],
            "accepted": len(accepted),
            **{f: len(progs[f]) for f in FLAGS},
            **{f"{f}_accepted": len(progs[f] & accepted) for f in FLAGS},
            "added_kind": dict(added_kind),
            "crossfn_by_tool": dict(crossfn_by_tool)}


def pct(n: int, d: int) -> str:
    return f"{n} ({100 * n / d:.1f}%)" if d else "0"


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--cap", type=int, default=5,
                    help="last repair round analyzed (paper horizon: 5)")
    ap.add_argument("--slug", nargs="*", default=SLUGS)
    ap.add_argument("--models", nargs="*", default=MODELS)
    ap.add_argument("--tasks", default=None,
                    help="restrict to these tasks ('subset' or a comma list)")
    ap.add_argument("--name", default="relocation",
                    help="output basename under results/sac2027/")
    a = ap.parse_args()
    keep = None
    if a.tasks:
        from src.pipeline.run_experiment import _parse_tasks
        keep = set(_parse_tasks(a.tasks))
    OUT.mkdir(parents=True, exist_ok=True)
    rows, events = [], []
    for slug in a.slug:
        for model in a.models:
            row = analyze_cell(model, slug, a.cap, events, keep)
            if row:
                rows.append(row)
                print(f"{model} / {slug}: {row['entered']} programs, "
                      f"crossfn {row['crossfn']}")
    lines = ["# Defect relocation across repair rounds", "",
             f"Rounds 0..{a.cap}; programs that entered repair (not clean at "
             "start). A program is counted once per event kind. Generated by "
             "`python -m src.analysis.relocation`. Events are heuristic until "
             "validated by hand (see results/sac2027/relocation_events.csv).", "",
             "| Model | Cell | entered | reloc | crossfn | newfn | subst | memmove | reappear |",
             "|---|---|---:|---:|---:|---:|---:|---:|---:|"]
    for slug in a.slug:
        sub = [r for r in rows if r["slug"] == slug]
        for r in sub:
            lines.append(f"| {r['model']} | {slug} | {r['entered']} | "
                         + " | ".join(pct(r[f], r["entered"]) for f in FLAGS) + " |")
        tot = {k: sum(r[k] for r in sub) for k in ["entered", *FLAGS]}
        lines.append(f"| **all** | {slug} | **{tot['entered']}** | "
                     + " | ".join(pct(tot[f], tot["entered"]) for f in FLAGS) + " |")
    lines += ["", f"## Programs the gate accepted by round {a.cap}", "",
              "Share of accepted programs whose trajectory contains each event.", "",
              "| Cell | accepted | " + " | ".join(FLAGS) + " |",
              "|---|---:|" + "---:|" * len(FLAGS)]
    for slug in a.slug:
        sub = [r for r in rows if r["slug"] == slug]
        acc = sum(r["accepted"] for r in sub)
        lines.append(f"| {slug} | {acc} | " + " | ".join(
            pct(sum(r[f"{f}_accepted"] for r in sub), acc) for f in FLAGS) + " |")
    lines += ["", "## Where added instances of a relocated rule land", "",
              "| Cell | in place (sim >= 0.6) | same function | other function | global |",
              "|---|---:|---:|---:|---:|"]
    for slug in a.slug:
        k = Counter()
        for r in rows:
            if r["slug"] == slug:
                k.update(r["added_kind"])
        n = sum(k.values())
        lines.append(f"| {slug} | {pct(k['in_place'], n)} | {pct(k['same_fn'], n)} | "
                     f"{pct(k['other_fn'], n)} | {pct(k['global'], n)} |")
    lines += ["", "## Cross-function events by tool", "",
              "| Cell | " + " | ".join(t for t in ["flawfinder", "cppcheck", "clang", "gcc", "asan", "fuzz"]) + " |",
              "|---|" + "---:|" * 6]
    for slug in a.slug:
        k = Counter()
        for r in rows:
            if r["slug"] == slug:
                k.update(r["crossfn_by_tool"])
        lines.append(f"| {slug} | " + " | ".join(str(k[t]) for t in
                     ["flawfinder", "cppcheck", "clang", "gcc", "asan", "fuzz"]) + " |")
    (OUT / f"{a.name}.md").write_text("\n".join(lines) + "\n")
    (OUT / f"{a.name}.json").write_text(json.dumps(rows, indent=1) + "\n")
    with open(OUT / f"{a.name}_events.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["model", "slug", "program", "round", "rule",
                                          "from_fns", "to_fns", "new_fn", "outcome"])
        w.writeheader()
        w.writerows(events)
    print(f"written -> {OUT / (a.name + '.md')} ({len(events)} cross-function events)")


if __name__ == "__main__":
    main()
