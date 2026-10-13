"""Full context of each sampled relocation event, for judges (RQ2).

Motivation: deciding whether a moved finding is still a weakness needs the
whole program (callers, inputs, buffer sizes), not just the two functions in
relocation_sample.md. For each event in relocation_sample.csv this writes the
task text, the complete program before and after the transition with line
numbers, and every report line of the event's rule in both rounds. The model
name is left out so judges label blind.

Usage:
    python -m src.analysis.relocation_context

Output: results/finding_tracking/relocation_sample_context.json
"""
from __future__ import annotations

import csv
import json

from src.analysis.relocation import COLL, OUT, round_dir
from src.pipeline.finding_track import STD, rule_of

SUFFIX = {"gcc": ".gcc.txt", "clang": ".clang.txt", "cppcheck": ".cppcheck.txt",
          "flawfinder": ".flawfinder.txt", "asan": ".asan.txt", "fuzz": ".fuzz.txt"}


def numbered(path) -> str:
    if not path.is_file():
        return "(file missing)"
    return "\n".join(f"{i:4d}  {s}" for i, s in
                     enumerate(path.read_text(errors="replace").splitlines(), 1))


def rule_messages(d, base: str, rule: str) -> list[str]:
    """Report lines of ``rule`` for ``base`` in round dir ``d``."""
    tool = rule.split(":")[0]
    p = d / f"{base}{SUFFIX[tool]}"
    if not p.is_file():
        return []
    out = []
    for raw in p.read_text(errors="replace").splitlines():
        m = STD.match(raw.strip())
        if m and rule_of(tool, m.group(4).strip(), m.group(5)) == rule:
            out.append(f"line {m.group(2)}: {m.group(5).strip()}")
    return out


def main() -> None:
    from src.pipeline.run_experiment import _load_tasks
    task_text = {t["id"]: t["text"] for t in _load_tasks()}
    with open(OUT / "relocation_sample.csv") as f:
        rows = list(csv.DictReader(f))
    with open(OUT / "relocation_events.csv") as f:
        events = {(e["model"], e["slug"], e["program"], e["round"], e["rule"]): e
                  for e in csv.DictReader(f)}
    out = []
    for r in rows:
        e = events[(r["model"], r["slug"], r["program"], r["round"], r["rule"])]
        cell = COLL / r["model"] / r["slug"]
        rnd = int(r["round"])
        before, after = round_dir(cell, rnd - 1), round_dir(cell, rnd)
        base = r["program"]
        out.append({
            "id": r["id"], "rule": r["rule"], "round": rnd,
            "task": task_text.get(int(base.split("-")[1]), ""),
            "left_functions": e["from_fns"].split("|"),
            "entered_functions": e["to_fns"].split("|"),
            "new_function": e["new_fn"] == "True",
            "before_reports": rule_messages(before, base, r["rule"]),
            "after_reports": rule_messages(after, base, r["rule"]),
            "before_source": numbered(before / f"{base}.c"),
            "after_source": numbered(after / f"{base}.c"),
        })
    (OUT / "relocation_sample_context.json").write_text(json.dumps(out, indent=1) + "\n")
    print(f"{len(out)} events -> {OUT / 'relocation_sample_context.json'}")


if __name__ == "__main__":
    main()
