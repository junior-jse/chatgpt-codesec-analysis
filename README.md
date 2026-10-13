# Tracking analyzer findings across LLM repair rounds

This repository accompanies a paper under double-blind review. It contains the code, the data and the outputs needed to reproduce the paper's analysis of how analyzer findings disappear during iterative LLM repair of C programs (fixed, moved to another function, replaced by a call with the same CWE, or returned in a later round).

## Contents

| Path | Content |
|---|---|
| `src/` | Repair pipeline (`src/pipeline/`) and analyses (`src/analysis/`) |
| `config/` | Models, tools and experiment settings |
| `data/collected_code_6/` | Repair trajectories of seven LLMs on 974 C tasks: the static-feedback and full-gate cells, rounds 0 to 5 (programs and analyzer reports) |
| `data/bct.py` | Task descriptions |
| `results/<model>/` | Manifests of the cells above (outcome of every program) |
| `results/finding_tracking/` | Outputs of the paper's analysis |
| `docs/reproduction.md` | Full manual; section 11 covers this study |

Other folders, when present, hold earlier versions of the pipeline and are not needed for this study.

## Setup

Python 3.10 or later:

```sh
python3 -m venv .venv
.venv/bin/pip install -r requirements.txt
```

The analyses below need no API keys and no analyzers.

## Reproducing the results

```sh
.venv/bin/python -m src.analysis.relocation               # event counts per model and cell
.venv/bin/python -m src.analysis.relocation_sample        # 150 sampled events
.venv/bin/python -m src.analysis.relocation_context       # full programs of the sampled events
.venv/bin/python -m src.analysis.relocation_judge_report  # judge labels and human check
```

Each command rewrites its files in `results/finding_tracking/`; the regenerated files are identical to the ones provided.

| Output | Paper content |
|---|---|
| `relocation.md`, `relocation.json` | Prevalence of each event per model and cell; programs accepted after each event; where re-added findings land; events per analyzer |
| `relocation_events.csv` | Every cross-function event |
| `relocation_sample.csv`, `relocation_sample_context.json` | The validation sample |
| `relocation_judge.json`, `relocation_judge.md` | LLM-judge labels and agreement |
| `relocation_human_check.md`, `relocation_human_check.csv` | Human check of the judge labels |

## New repair cells

Section 11 of `docs/reproduction.md` explains how to run the repair arms (repair-only reasoning, repair-history feedback) and how to check a cell end to end without calling any model (`src/pipeline/replay_check.py`). Running new cells needs API keys and the analyzers listed in `config/tools.yaml`; the commercial Cppcheck Premium binary is not included.

## License

MIT (see `LICENSE`).
