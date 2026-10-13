"""Stratified sample of relocation events for validation (RQ2).

Motivation: relocation events come from a heuristic key (rule, enclosing
function, normalized line text), so before they are reported as moved
defects someone must check whether the moved code is still a weakness. This
script draws a reproducible sample from results/finding_tracking/
relocation_events.csv: one event per program, quotas balanced across models,
shuffled so the model order is not visible. relocation_context.py adds the
full programs; the LLM judges and the human check read those.

Usage:
    python -m src.analysis.relocation_sample [--n 150] [--seed 2027]

Output: results/finding_tracking/relocation_sample.csv
"""
from __future__ import annotations

import argparse
import csv
import random
from collections import defaultdict

from src.analysis.relocation import OUT


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

    rows = []
    for i, e in enumerate(sample, 1):
        rows.append({"id": f"E{i:03d}", "model": e["model"], "slug": e["slug"],
                     "program": e["program"], "round": int(e["round"]), "rule": e["rule"],
                     "new_fn": e["new_fn"], "outcome": e["outcome"]})
    with open(OUT / "relocation_sample.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    per_model = {m: q for m, q in quota.items() if q}
    print(f"sampled {len(sample)} events: {per_model}")
    print(f"written -> {OUT / 'relocation_sample.csv'}")


if __name__ == "__main__":
    main()
