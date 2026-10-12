"""Experiment orchestrator for the FSE 2027 extension.

Replaces ``run_full_experiment.py`` (kept untouched for v1 reproduction).

Per cell: generate -> sanitize -> analyze(measure) -> comment_inject(feedback)
-> heal loop {heal -> sanitize -> analyze -> comment_inject} until no findings
remain or max_rounds reached.

Foldering: data/collected_code_6/{model}/{cell_slug}/heal_N/{to_be_healed,healed}/
cell_slug = feedback-{fb}-r{reasoning}-t{temp}-p{top_p}

Semantics:
- ``measure``   = which detection channels are RUN (the paper's numbers).
  RQ1 uses "static"; ablation cells use "static+dynamic".
- ``feedback``  = which findings are injected as comments for repair
  ("none" | "static" | "dynamic" | "static+dynamic"). A ``+history`` suffix
  (SAC 2027 extension) also tells the model which findings moved or were
  replaced since the previous round; detection and stopping are unchanged.
"""
from __future__ import annotations

import argparse
import csv
import json
import re
import shutil
import sys
from dataclasses import asdict, dataclass, field
from pathlib import Path

import yaml
from tqdm import tqdm

from src.config import max_healing_rounds
from src.llm.registry import load_registry
from src.pipeline import comment_inject as ci
from src.pipeline import generate as gen
from src.pipeline import static as st

REPO_ROOT = Path(__file__).resolve().parents[2]


@dataclass
class ExperimentCell:
    model: str
    feedback: str = "static"  # none | static | dynamic | static+dynamic
    measure: str = "static+dynamic"  # static | dynamic | static+dynamic
    tasks: list[int] = field(default_factory=lambda: list(range(1, 975)))
    temp: float = 1.0
    top_p: float = 1.0
    # Healing-round budget comes from config/experiment.yaml -> experiment.max_rounds.
    max_rounds: int = field(default_factory=max_healing_rounds)
    samples: int = 1
    fuzz: bool = False  # AFL++ channel (stdin/argv consumers only; costly)
    root: str = "data/collected_code_6"
    gen_source: str | None = None  # sibling cell slug to import round-0 from
    gen_source_reports: bool = False  # also import static reports + .o
    # Registry id whose cell holds gen_source (default: this model). Lets a
    # reasoning-on alias repair the programs of its reasoning-off sibling.
    gen_source_model: str | None = None


def cell_slug(cell: ExperimentCell, cfg) -> str:
    r = 1 if cfg.reasoning else 0
    return f"feedback-{cell.feedback}-r{r}-t{cell.temp}-p{cell.top_p}"


def _load_tasks() -> list[dict]:
    """Load BCT problems (id, text) from bct.py."""
    import ast

    tree = ast.parse((REPO_ROOT / "data" / "bct.py").read_text())
    for node in tree.body:
        if isinstance(node, ast.Assign):
            for target in node.targets:
                if isinstance(target, ast.Name) and target.id == "problems":
                    return ast.literal_eval(node.value)
    raise RuntimeError("problems list not found in bct.py")


def _run_analyze(source_dir: Path, measure: str, model_id: str, fuzz: bool = False) -> None:
    if any(s in ci.STATIC_SUFFIXES for s in ci.CHANNEL_SUFFIXES.get(measure, [])):
        tools = ["gcc", "clang", "cppcheck", "flawfinder"]
        st.analyze(str(source_dir), tools=tools, compile_gate=True)
    if "dynamic" in measure:
        _run_dynamic(source_dir, model_id, fuzz=fuzz)


def _run_dynamic(source_dir: Path, model_id: str, fuzz: bool = False) -> None:
    """Run the dynamic channel (sanitizer oracle + optional AFL++ fuzzing)."""
    try:
        from src.dyn.runner import analyze_dynamic
    except ImportError:
        print("[skip] src.dyn.runner not available; skipping dynamic analysis")
        return
    counts = analyze_dynamic(str(source_dir), fuzz=fuzz)
    print(f"[{model_id}] dynamic analysis: {counts}")


# Strict allowlist: raw generations and sanitized code ONLY. Analyzer reports
# (.{gcc,clang,cppcheck,flawfinder}.txt, dynamic reports, .o objects) are never
# imported — the importing cell must run its own detection gate.
_ROUND0_RE = re.compile(r"^problem-(\d+)(?:-s(\d+))?\.(c|txt)$")
# Static-analyzer reports + compile-gate objects for the code above. Copying
# them (opt-in) lets the importing cell skip re-running static analysis on
# identical code; the parsers only use the line numbers, so the absolute
# source paths recorded inside the reports are irrelevant.
_ROUND0_REPORT_RE = re.compile(
    r"^problem-(\d+)(?:-s(\d+))?\.(?:gcc|clang|cppcheck|flawfinder)\.txt$")
_ROUND0_OBJ_RE = re.compile(r"^problem-(\d+)(?:-s(\d+))?\.o$")


def import_round0(cell: ExperimentCell, cfg, tasks: list[dict],
                  reports: bool = False) -> int:
    """Copy round-0 generations from a sibling cell instead of regenerating.

    Imports ``problem-{id}[-s{N}].{c,txt}`` for the cell's tasks, limited to
    the cell's sample count, into this cell's heal_0. Files already present
    are left untouched (idempotent). With ``reports=True`` also imports the
    static-analyzer reports and compile-gate objects (safe only when the
    source cell analyzed the exact same code — true for gen-source imports
    from a completed static cell). Returns the number of files copied.
    """
    src_model = cell.gen_source_model or cell.model
    src_dir = REPO_ROOT / cell.root / src_model / cell.gen_source / "heal_0"
    if not src_dir.is_dir():
        sys.exit(f"[gen-source] source cell not found: {src_dir}")
    dst = REPO_ROOT / cell.root / cell.model / cell_slug(cell, cfg) / "heal_0"
    dst.mkdir(parents=True, exist_ok=True)
    task_ids = {t["id"] for t in tasks}
    pats = [_ROUND0_RE]
    if reports:
        pats += [_ROUND0_REPORT_RE, _ROUND0_OBJ_RE]
    copied = 0
    for f in sorted(src_dir.iterdir()):
        m = None
        for p in pats:
            m = p.match(f.name)
            if m:
                break
        if not m:
            continue
        if int(m.group(1)) not in task_ids:
            continue
        if int(m.group(2) or 1) > cell.samples:
            continue
        target = dst / f.name
        if target.exists():
            continue
        shutil.copy2(f, target)
        copied += 1
    print(
        f"[{cell.model}] gen-source: imported {copied} round-0 files "
        f"from {src_model}/{cell.gen_source} (tasks={len(task_ids)}, samples={cell.samples}"
        f"{', incl. static reports' if reports else ''})"
    )
    return copied


def run_cell(cell: ExperimentCell, resume: bool = True) -> None:
    registry = load_registry(str(REPO_ROOT / "config" / "models.yaml"))
    cfg = registry[cell.model]
    tasks = [t for t in _load_tasks() if t["id"] in cell.tasks]
    root = REPO_ROOT / cell.root / cell.model / cell_slug(cell, cfg)
    root.mkdir(parents=True, exist_ok=True)
    slug = cell_slug(cell, cfg)

    # ---- heal_0: generation + first analysis ----
    if cell.gen_source:
        import_round0(cell, cfg, tasks, reports=cell.gen_source_reports)
    heal0 = root / "heal_0"
    heal0.mkdir(parents=True, exist_ok=True)
    for sample in range(1, cell.samples + 1):
        gen.run_batch(
            cfg, tasks, str(heal0), sample=sample,
            temperature=cell.temp, top_p=cell.top_p,
        )
    _run_analyze(heal0, cell.measure, cell.model, fuzz=cell.fuzz)

    # ---- heal loop ----
    outcome: dict[str, dict] = {}
    ever_flagged: set[str] = set()
    prev_healed = heal0
    history: Path | None = None  # program one round before prev_healed
    for round_no in range(1, cell.max_rounds + 1):
        to_be_healed = root / f"heal_{round_no}" / "to_be_healed"
        ci.generate_commented_weak_code(
            str(prev_healed), str(to_be_healed),
            feedback=cell.feedback, task_ids=set(cell.tasks), resume=resume,
            history_dir=str(history) if history else None,
        )
        flagged = sorted(p.stem for p in to_be_healed.glob("*.c"))
        if not flagged:
            print(f"[{cell.model}/{slug}] round {round_no}: no remaining findings; done")
            break
        ever_flagged.update(flagged)
        print(f"[{cell.model}/{slug}] round {round_no}: {len(flagged)} files to heal")

        healed = root / f"heal_{round_no}" / "healed"
        healed.mkdir(parents=True, exist_ok=True)
        _heal_batch(cfg, [to_be_healed / f"{b}.c" for b in flagged], healed, round_no)
        _run_analyze(healed, cell.measure, cell.model, fuzz=cell.fuzz)

        # Determine who is still flagged by pre-building next round's input.
        next_tbe = root / f"heal_{round_no + 1}" / "to_be_healed"
        ci.generate_commented_weak_code(
            str(healed), str(next_tbe),
            feedback=cell.feedback, task_ids=set(cell.tasks), resume=True,
            history_dir=str(prev_healed),
        )
        still_flagged = {p.stem for p in next_tbe.glob("*.c")}
        for base in flagged:
            if base in still_flagged:
                continue
            if (healed / f"{base}.c").exists():
                outcome[base] = {"rounds_to_clean": round_no, "outcome": "clean"}
            else:
                # Heal call failed permanently (no artifact): never count a
                # missing file as clean.
                outcome[base] = {
                    "rounds_to_clean": round_no, "outcome": "heal_failed",
                }
        history = prev_healed
        prev_healed = healed

    # Everything that never reached zero findings.
    for base in sorted(p.stem for p in heal0.glob("*.c")):
        if base in outcome:
            continue
        if base not in ever_flagged:
            # Clean at heal_0 already — never needed a heal round.
            outcome[base] = {"rounds_to_clean": 0, "outcome": "clean_at_start"}
        else:
            outcome[base] = {"rounds_to_clean": cell.max_rounds, "outcome": "not_cleaned"}

    manifest = {
        "cell": asdict(cell),
        "model_id": cfg.model_id,
        "reasoning": cfg.reasoning,
        "outcomes": outcome,
    }
    out_dir = REPO_ROOT / "results" / cell.model
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / f"{slug}.manifest.json").write_text(json.dumps(manifest, indent=2))
    print(f"[{cell.model}/{slug}] manifest written to results/{cell.model}/{slug}.manifest.json")


def _heal_batch(cfg, sources: list[Path], healed: Path, round_no: int) -> None:
    from concurrent.futures import ThreadPoolExecutor, as_completed

    progress = tqdm(total=len(sources), desc=f"Healing round {round_no}: ")
    with ThreadPoolExecutor(max_workers=cfg.concurrency) as pool:
        futs = {
            pool.submit(
                gen._safe, gen.heal_code, cfg,
                src.read_text(), _id_from_stem(src.stem), str(healed),
                round_no, _sample_from_stem(src.stem),
            ): src
            for src in sources
        }
        for fut in as_completed(futs):
            try:
                fut.result()
            except KeyboardInterrupt:
                raise
            except BaseException as ex:  # noqa: BLE001
                # One permanently failed heal must not kill the whole cell.
                print(f"[warn] heal failed permanently: {ex}")
            progress.update()


def _id_from_stem(stem: str) -> int:
    return int(stem.split("-")[1].split("-s")[0])


def _sample_from_stem(stem: str) -> int:
    return int(stem.split("-s")[1]) if "-s" in stem else 1


def _parse_tasks(value: str | None) -> list[int]:
    if value in (None, "all"):
        return list(range(1, 975))
    if value == "subset":
        subset_csv = REPO_ROOT / "results" / "vulnerable_subset.csv"
        if not subset_csv.exists():
            sys.exit("results/vulnerable_subset.csv missing; run subset selection first")
        with open(subset_csv) as f:
            ids = [int(r["problem_id"]) for r in csv.DictReader(f)]
        # The CSV holds the FULL ranked list (subset.py keeps every top-k cut
        # recoverable); apply the pre-registered subset_size from
        # experiment.yaml (default 200).
        cfg = yaml.safe_load(open(REPO_ROOT / "config" / "experiment.yaml"))
        k = int(cfg.get("experiment", {}).get("subset_size", 200))
        if k < len(ids):
            print(f"[subset] using top-{k} of {len(ids)} ranked problems")
            return ids[:k]
        return ids
    ids: list[int] = []
    for part in value.split(","):
        part = part.strip()
        if "-" in part:  # inclusive range, e.g. "1-30"
            lo, hi = part.split("-", 1)
            ids.extend(range(int(lo), int(hi) + 1))
        else:
            ids.append(int(part))
    return ids


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run an experiment cell")
    parser.add_argument("--model", required=True, help="registry id, e.g. deepseek-chat")
    parser.add_argument("--feedback", default="static",
                        choices=["none", "static", "dynamic", "static+dynamic",
                                 "static+history", "static+dynamic+history"])
    parser.add_argument("--measure", default=None,
                        choices=["static", "dynamic", "static+dynamic"])
    parser.add_argument("--tasks", default=None, help="comma list, 'subset', or 'all'")
    parser.add_argument("--temp", type=float, default=1.0)
    parser.add_argument("--top-p", type=float, default=1.0)
    parser.add_argument("--samples", type=int, default=1)
    parser.add_argument("--max-rounds", type=int, default=None,
                        help="healing-round budget; default: experiment.max_rounds "
                             "from config/experiment.yaml")
    parser.add_argument("--fuzz", action="store_true",
                        help="enable AFL++ fuzzing channel (stdin/argv consumers only)")
    parser.add_argument("--gen-source", default=None, metavar="CELL_SLUG",
                        help="import round-0 generations from a sibling cell "
                             "(e.g. feedback-static-r0-t1.0-p1.0) instead of "
                             "regenerating; missing samples are still generated")
    parser.add_argument("--gen-source-model", default=None, metavar="MODEL_ID",
                        help="registry id whose cell holds --gen-source (default: "
                             "--model); e.g. repair the reasoning-off programs "
                             "with a reasoning-on alias")
    parser.add_argument("--gen-source-reports", action="store_true",
                        help="also import static-analyzer reports and compile "
                             "objects from the gen-source cell (skips re-running "
                             "static analysis on identical code)")
    args = parser.parse_args()

    channels = args.feedback.removesuffix("+history")
    measure = args.measure or (
        "static+dynamic" if channels in ("none", "dynamic") else channels
    )
    cell = ExperimentCell(
        model=args.model,
        feedback=args.feedback,
        measure=measure,
        tasks=_parse_tasks(args.tasks),
        temp=args.temp,
        top_p=args.top_p,
        samples=args.samples,
        max_rounds=(args.max_rounds if args.max_rounds is not None
                    else max_healing_rounds()),
        fuzz=args.fuzz,
        gen_source=args.gen_source,
        gen_source_reports=args.gen_source_reports,
        gen_source_model=args.gen_source_model,
    )
    run_cell(cell)
