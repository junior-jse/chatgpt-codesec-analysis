"""Offline end-to-end check of a repair cell (no API calls).

Runs ``run_experiment.run_cell`` on a few tasks inside a temporary copy of
the repository layout, but replaces the model's repair with the repair
already recorded in an existing cell (replay). This exercises round-0 import,
static analysis, feedback injection (including ``+history``) and the
manifest without spending tokens, so a new cell can be checked before the
paid run. Analyzers missing on this machine are skipped with a warning, so
findings can differ from the recorded cell. Nothing is written to the
repository.

Usage:
    python -m src.pipeline.replay_check --model openai-gpt56-sol \\
        --feedback static+history --tasks 544,607,747,794 \\
        --replay openai-gpt56-sol/feedback-static-r0-t1.0-p1.0
"""
from __future__ import annotations

import argparse
import json
import shutil
import tempfile
from pathlib import Path

from src.pipeline import generate as gen
from src.pipeline import run_experiment as rx

REPO = rx.REPO_ROOT


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", required=True)
    ap.add_argument("--feedback", default="static+history")
    ap.add_argument("--tasks", required=True, help="comma list of task ids")
    ap.add_argument("--replay", required=True,
                    help="recorded cell to replay, as <model>/<cell slug>")
    ap.add_argument("--gen-source", default="feedback-static-r0-t1.0-p1.0")
    ap.add_argument("--gen-source-model", default=None)
    ap.add_argument("--max-rounds", type=int, default=3)
    a = ap.parse_args()

    tmp = Path(tempfile.mkdtemp(prefix="replay_check_"))
    (tmp / "data").mkdir()
    (tmp / "config").symlink_to(REPO / "config")
    (tmp / "data" / "bct.py").symlink_to(REPO / "data" / "bct.py")
    src_model = a.gen_source_model or a.model
    (tmp / "cells" / src_model).mkdir(parents=True)
    (tmp / "cells" / src_model / a.gen_source).symlink_to(
        REPO / "data" / "collected_code_6" / src_model / a.gen_source)
    replay = REPO / "data" / "collected_code_6" / a.replay

    def replay_heal(cfg, commented_source, task_id, output_dir, heal_round,
                    sample=1, force=False):
        base = f"problem-{task_id}{gen.sample_suffix(sample)}"
        recorded = replay / f"heal_{heal_round}" / "healed"
        if not (recorded / f"{base}.c").is_file():
            raise RuntimeError(f"no recorded repair for {base} round {heal_round}")
        for ext in (".c", ".txt"):
            if (recorded / f"{base}{ext}").is_file():
                shutil.copy2(recorded / f"{base}{ext}", Path(output_dir) / f"{base}{ext}")
        return Path(output_dir) / f"{base}.txt"

    def imported_only(cfg, task_text, task_id, output_dir, sample=1, *_, **__):
        raw = Path(output_dir) / f"problem-{task_id}{gen.sample_suffix(sample)}.txt"
        if raw.is_file():
            return raw  # same skip the real generate_code does
        raise RuntimeError(f"round-0 program {raw.name} missing: the check must not call the model")

    gen.heal_code = replay_heal
    gen.generate_code = imported_only
    rx.REPO_ROOT = tmp
    cell = rx.ExperimentCell(
        model=a.model, feedback=a.feedback,
        measure=a.feedback.removesuffix("+history"),
        tasks=[int(t) for t in a.tasks.split(",")], samples=1,
        max_rounds=a.max_rounds, root="cells", gen_source=a.gen_source,
        gen_source_reports=True, gen_source_model=a.gen_source_model)
    rx.run_cell(cell)

    registry = rx.load_registry(str(REPO / "config" / "models.yaml"))
    slug = rx.cell_slug(cell, registry[a.model])
    root = tmp / "cells" / a.model / slug
    print(f"\ncell: {a.model}/{slug} (temporary root {tmp})")
    for r in range(1, a.max_rounds + 1):
        tbh = root / f"heal_{r}" / "to_be_healed"
        files = sorted(tbh.glob("*.c")) if tbh.is_dir() else []
        notes = [f for f in files if f.read_text().startswith("/* Repair history")]
        print(f"round {r}: {len(files)} programs to repair, {len(notes)} with a history block")
        for f in notes[:2]:
            print("   " + f.name + ": " + f.read_text().splitlines()[1].strip())
    manifest = tmp / "results" / a.model / f"{slug}.manifest.json"
    outcomes = json.loads(manifest.read_text())["outcomes"]
    print("manifest outcomes:", {b: v["outcome"] for b, v in sorted(outcomes.items())})
    shutil.rmtree(tmp)


if __name__ == "__main__":
    main()
