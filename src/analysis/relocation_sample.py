"""Stratified sample of relocation events for manual validation (RQ2).

Motivation: relocation events come from a heuristic key (rule, enclosing
function, normalized line text), so before reporting them as moved defects
two annotators must check whether the moved code still carries the weakness.
This script draws a reproducible sample from results/sac2027/relocation_events.csv
(one event per program, quotas balanced across models) and writes, for each
event, the source function the rule left and the function it entered.

Labels (one per annotator):
  SAME      the flagged weakness moved: the new location is unsafe in the
            same way the old one was
  FIX+NEW   the old weakness was fixed and an unrelated flagged call appeared
  BENIGN    both locations are safe; the analyzer flags a pattern, not a defect
  UNCLEAR   cannot be decided from the two functions

Usage:
    python -m src.analysis.relocation_sample [--n 150] [--seed 2027]

Output: results/sac2027/relocation_sample.md (code for reading),
        results/sac2027/relocation_sample.csv (label sheet)
"""
from __future__ import annotations

import argparse
import csv
import random
from collections import defaultdict

from src.analysis.relocation import COLL, OUT, round_dir
from src.pipeline.finding_track import func_map


def function_text(path, names: set[str], limit: int = 40) -> str:
    """Source lines of the given top-level functions, at most ``limit`` each."""
    if not path.is_file():
        return "(file missing)"
    lines = path.read_text(errors="replace").splitlines()
    fmap = func_map(lines)
    out = []
    for name in sorted(names):
        body = [f"{i + 1:4d}  {s}" for i, s in enumerate(lines) if fmap[i] == name]
        if len(body) > limit:
            body = body[:limit] + [f"      ... ({len(body) - limit} more lines)"]
        out += body or [f"(function {name} not found)"]
    return "\n".join(out)


def allocate(groups: dict[str, list], n: int) -> dict[str, int]:
    """Equal quotas per group, capped by group size; leftovers go round-robin."""
    quota = {g: 0 for g in groups}
    left = n
    while left > 0:
        open_groups = [g for g in sorted(groups) if quota[g] < len(groups[g])]
        if not open_groups:
            break
        for g in open_groups:
            if left == 0:
                break
            quota[g] += 1
            left -= 1
    return quota


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--n", type=int, default=150)
    ap.add_argument("--seed", type=int, default=2027)
    a = ap.parse_args()
    with open(OUT / "relocation_events.csv") as f:
        events = list(csv.DictReader(f))
    # One event per program (first in trajectory order), grouped by model.
    seen, by_model = set(), defaultdict(list)
    for e in events:
        key = (e["model"], e["slug"], e["program"])
        if key not in seen:
            seen.add(key)
            by_model[e["model"]].append(e)
    rng = random.Random(a.seed)
    quota = allocate(by_model, a.n)
    sample = []
    for model in sorted(by_model):
        sample += rng.sample(by_model[model], quota[model])
    rng.shuffle(sample)  # annotators should not see model blocks

    md = ["# Relocation events for manual validation", "",
          f"{len(sample)} events, seed {a.seed}, one per program. Label each "
          "event in relocation_sample.csv (SAME, FIX+NEW, BENIGN, UNCLEAR); "
          "see src/analysis/relocation_sample.py for the definitions.", ""]
    rows = []
    for i, e in enumerate(sample, 1):
        r = int(e["round"])
        cell = COLL / e["model"] / e["slug"]
        before = round_dir(cell, r - 1) / f"{e['program']}.c"
        after = round_dir(cell, r) / f"{e['program']}.c"
        src_fns = set(filter(None, e["from_fns"].split("|")))
        dst_fns = set(filter(None, e["to_fns"].split("|")))
        md += [f"## E{i:03d}  `{e['rule']}`  round {r - 1} -> {r}", "",
               f"Program `{e['program']}` ({e['slug']}); left "
               f"`{', '.join(sorted(src_fns)) or '-'}`, entered "
               f"`{', '.join(sorted(dst_fns)) or '-'}`"
               + (" (new function)" if e["new_fn"] == "True" else "") + ".", "",
               f"Round {r - 1}, where the rule was removed:", "", "```c",
               function_text(before, src_fns), "```", "",
               f"Round {r}, where the rule appeared:", "", "```c",
               function_text(after, dst_fns), "```", ""]
        rows.append({"id": f"E{i:03d}", "model": e["model"], "slug": e["slug"],
                     "program": e["program"], "round": r, "rule": e["rule"],
                     "new_fn": e["new_fn"], "outcome": e["outcome"],
                     "label_a": "", "label_b": "", "notes": ""})
    (OUT / "relocation_sample.md").write_text("\n".join(md) + "\n")
    with open(OUT / "relocation_sample.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    per_model = {m: q for m, q in quota.items() if q}
    print(f"sampled {len(sample)} events: {per_model}")
    print(f"written -> {OUT / 'relocation_sample.md'}")


if __name__ == "__main__":
    main()
