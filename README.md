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

| Version | What it studies | Code and data | How to run |
|---|---|---|---|
| v1 (ISE 2026) | Vulnerabilities in GPT-4-generated C code, repaired with feedback from GCC's static analyzer | `v1/` | [v1/README.md](v1/README.md) |
| v2 | Seven LLMs, a multi-channel analysis gate (static analyzers, tests, sanitizers, fuzzing) and iterative repair | `src/`, `config/`, `data/`, `results/` | [docs/reproduction.md](docs/reproduction.md), sections 1 to 10 |
| v3 | How findings disappear during repair, with LLM judges for moved code, repair-only reasoning and repair-history feedback | v2 code plus the v3 modules; `results/finding_tracking/` | [docs/reproduction.md](docs/reproduction.md), section 11 |

The design of the v2 pipeline is in [docs/architecture.md](docs/architecture.md).

## Quick start

```bash
python3 -m pip install -r requirements.txt
python -m src.analysis.relocation   # v3 analysis of the collected data; no keys or analyzers
```

Running new cells needs API keys and the analysis toolchain (see [docs/reproduction.md](docs/reproduction.md)).
