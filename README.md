# Investigating vulnerable code generation from ChatGPT

This project contains the assets of three versions of the experiment (see [Versions](#versions)).

This code is published under the MIT License. Please read [it](LICENSE) and use it only if you agree with the terms.

You are welcome to mimic our experiment and check the results yourself. Use the issues as a forum to discuss about this experiment (if you need any assistance running it, or any ideas you want to discuss).

Our dataset is based on [Most Basic Python Problems](https://github.com/google-research/google-research/tree/master/mbpp) dataset, adapted for C code generation instead of python code.

I'd like to thank all supporters of this project.
 - 许海涛 (Xu Haitao), my supervisor;
 - 浙江大学 (Zhejiang University);
 - 中华人民共和国教育部 (Ministry of Education of the People's Republic of China) for the grants provided;
 - Instituto Federal do Piauí, for the financial support;
 - [Gabriel Habib Medeiros Oliveira](https://github.com/enggabrielhabib), for his contributions.

## Versions

The experiment has grown in three versions. Each keeps its own run guide.

| Version | What it studies | Where | How to run |
|---|---|---|---|
| v1 (ISE 2026) | Vulnerabilities in GPT-4-generated C code and repair guided by GCC's static analyzer | `v1/` | [v1/README.md](v1/README.md) |
| v2 (multi-channel gate) | Seven LLMs, a multi-channel analysis gate (static analyzers, tests, sanitizers, fuzzing) and iterative repair | `src/`, `config/`, `data/`, `results/` | [docs/reproduction.md](docs/reproduction.md), [docs/architecture.md](docs/architecture.md) |
| v3 (finding tracking) | How analyzer findings leave during repair (fixed, moved, replaced, returned), LLM judges for the moved code, repair-only reasoning and repair-history feedback | v2 code plus `src/pipeline/finding_track.py`, `src/pipeline/replay_check.py`, `src/analysis/relocation*.py`, `results/finding_tracking/` | [docs/finding-tracking.md](docs/finding-tracking.md) |

## Quick start

```bash
python3 -m pip install -r requirements.txt

# v3 offline analysis on the collected v2 data (no API keys or analyzers needed)
python -m src.analysis.relocation
python -m src.analysis.relocation_sample
```

Running new cells needs API keys and the analysis toolchain; see
[docs/reproduction.md](docs/reproduction.md) for v2 cells and
[docs/finding-tracking.md](docs/finding-tracking.md) for the v3 repair arms.
