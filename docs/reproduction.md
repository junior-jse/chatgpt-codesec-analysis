# Experiment Reproduction Manual

This manual explains how to run the paper's experiment, in which large language models (LLMs) write and repair C programs, and how to reproduce its results from the collected data.

Sections 1 to 10 cover the v2 cells. Section 11 covers v3, which reuses the same setup.

## Contents

1. [What the experiment does](#1-what-the-experiment-does)
2. [Environment and tools](#2-environment-and-tools)
3. [Configuring the tools](#3-configuring-the-tools)
4. [Access keys](#4-access-keys)
5. [Running](#5-running)
6. [Execution options](#6-execution-options)
7. [Resuming an interrupted run](#7-resuming-an-interrupted-run)
8. [Generated reports](#8-generated-reports)
9. [Reproducing the paper's results](#9-reproducing-the-papers-results)
10. [Notes](#10-notes)
11. [Version 3: finding tracking](#11-version-3-finding-tracking)

## 1. What the experiment does

We asked seven LLMs to write C programs for 974 tasks. The pipeline then analyzes each program with static tools and, depending on the configuration, also with dynamic tools. When the tools find problems, the pipeline itself inserts these findings as comments into the code and sends it back to the model, asking for a fix. Only the findings from the channels selected for feedback go into the comments.

This cycle repeats until no findings remain or until the round limit is reached. The limit is set by `max_rounds` in `config/experiment.yaml`, and the limit used in each run is recorded in its manifest (Section 8).

We call each combination of a model and a configuration a cell.

## 2. Environment and tools

We ran the experiment on macOS with these versions:

- Python 3, with the packages in `requirements.txt`
- GNU Compiler Collection (GCC) 15.2.0, installed with Homebrew as `gcc-15`
- Apple clang 21.0.0, used for static analysis and for builds with the AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan) sanitizers
- Cppcheck Premium 23.6.0
- Flawfinder 2.0.19
- AFL++ 5.02c, a fuzzer derived from American Fuzzy Lop (AFL), used only for fuzzing

To install the Python packages, we used:

```sh
python3 -m venv .venv
.venv/bin/pip install -r requirements.txt
```

### 2.1 About Cppcheck Premium

We used Cppcheck Premium in `bughunting` mode, which is only available in the paid version and performs a broader analysis than the default mode. Without it, the pipeline still works: it skips cppcheck, or it runs the free version if we point the path to it and remove the `--premium=bughunting` flag from `config/tools.yaml`.

## 3. Configuring the tools

Tool paths and flags are set in `config/tools.yaml`. In the repository, the paths are absolute and point to the machine where we ran the experiment. Before running, we replace these fields with local paths:

- `tools.gcc.path`
- `tools.clang.path`
- `tools.cppcheck.path`
- `tools.flawfinder.path`
- `dynamic.afl_bin`, `dynamic.afl_cc`, and `dynamic.afl_lib`

The flags should stay as they are, to keep the same configuration as the paper.

If a tool is not found, the pipeline prints a warning and continues without it:

```
[warn] gcc not found at /usr/local/bin/gcc-15; skipping
```

Two details:

- Sanitizer builds use the `clang` found on the PATH.
- Besides the analysis, gcc runs the compilation check, which produces the `.o` file. Without gcc, this check is not run either.

## 4. Access keys

The models are accessed through an application programming interface (API). Each model reads its key from an environment variable defined in `config/models.yaml`. The id used with `--model` is shown in parentheses:

- `OPENAI_API_KEY`: GPT-5.6 Sol (`openai-gpt56-sol`)
- `ANTHROPIC_API_KEY`: Claude Fable 5 (`claude-fable-5`)
- `GEMINI_API_KEY`: Gemini 3.1 Pro (`gemini-3-pro`)
- `DASHSCOPE_API_KEY`, all through Alibaba's DashScope platform:
  - DeepSeek V4 Pro (`deepseek-v4-pro`)
  - Qwen Max (`qwen-max`)
  - Kimi K3 (`kimi-k3`)
  - GLM-5.1 (`glm-5.1`)

The reasoning variants, whose ids end in `-thinking`, use the same key as the base model. We only need the keys for the models we run. Without the key, the pipeline stops with the message `API key missing: set env var ...`.

## 5. Running

Every run goes through a single command, `python -m src.pipeline.run_experiment`, and each run executes one cell.

---

**Quick test**, with one task:

```sh
.venv/bin/python -m src.pipeline.run_experiment --model qwen-max --feedback static --tasks 2
```

---

**Main cell**, with all 974 tasks, three samples per task, and only the static channel:

```sh
.venv/bin/python -m src.pipeline.run_experiment --model qwen-max --feedback static --tasks all --samples 3
```

---

**Cell with the static and dynamic channels.** It reuses the initial code from the previous cell, so it does not pay for generation again:

```sh
.venv/bin/python -m src.pipeline.run_experiment --model qwen-max --feedback static+dynamic --tasks all --samples 3 --fuzz --gen-source feedback-static-r0-t1.0-p1.0 --gen-source-reports
```

---

**Reasoning cells.** We use the model id ending in `-thinking` (for example, `glm-5.1-thinking`), with `--tasks subset` and `--samples 3`.

---

Each cell name summarizes its configuration. For example, `feedback-static+dynamic-r0-t1.0-p1.0` means static and dynamic feedback, no reasoning (`r0`), temperature 1.0, and top_p 1.0. The exact parameters of each cell in the paper are in `results/<model>/<cell>.manifest.json`.

## 6. Execution options

These options let us run only part of the experiment or change its configuration:

- `--tasks 1-30` or `--tasks 5,8,13`: runs only these tasks.
- `--tasks subset`: runs the first 200 tasks of the ranking in `results/vulnerable_subset.csv`. The size comes from `subset_size` in `config/experiment.yaml`.
- `--feedback`: sets what the model receives as comments (`none`, `static`, `dynamic`, or `static+dynamic`).
- `--measure`: sets the tools that measure the code. If omitted, it follows `--feedback`; with `none` or `dynamic`, it uses both channels.
- `--max-rounds N`: round limit. The default comes from `config/experiment.yaml`.
- `--samples N`: number of samples per task.
- `--temp` and `--top-p`: the model's sampling parameters.
- `--fuzz`: turns on AFL++, only for programs that read from standard input or from arguments.
- `--gen-source <cell>`: reuses the initial generation of another cell of the same model.
- `--gen-source-reports`: also reuses the static reports of that cell.

Environment variables for the dynamic channel:

- `DYN_SANITIZERS`: sanitizers to use. The default is `address,undefined`.
- `DYN_ARCHIVE=0`: does not keep the builds or the fuzzer sessions.
- `DYN_ARCHIVE_ROOT`: folder where these files are kept. The default is `artifacts/` in the repository.

## 7. Resuming an interrupted run

If a run is interrupted, we just run the same command again. The pipeline skips what already exists and continues where it stopped.

## 8. Generated reports

The data of each cell is stored in `data/collected_code_6/<model>/<cell>/`, split by round:

- `heal_0/`: the initial generation and its reports
- `heal_<n>/to_be_healed/`: the code with the findings inserted as comments, exactly as it was sent to the model in round `n`
- `heal_<n>/healed/`: the code fixed by the model and its reports

For each program, for example `problem-5.c`, the pipeline produces, depending on the measured channels:

- `problem-5.txt`: the model's original answer
- `problem-5.c`: the code extracted from that answer
- `problem-5.gcc.txt`, `problem-5.clang.txt`, `problem-5.cppcheck.txt`, and `problem-5.flawfinder.txt`: the static tool reports
- `problem-5.test.txt`: the test case results
- `problem-5.asan.txt`: the sanitizer findings
- `problem-5.fuzz.txt`: the fuzzing findings, when `--fuzz` is on
- `problem-5.o`: the result of the compilation check, when gcc is installed

With more than one sample, files get the suffix `-s2`, `-s3`, and so on.

The clang analyzer also writes one `.plist` file per program in the folder where the command was run. The pipeline does not use these files.

For each cell, the pipeline also produces:

- `results/<model>/<cell>.manifest.json`: the cell summary, with the parameters and the outcome of each program
- `results/<model>/<cell>.usage.jsonl`: the token and time log of each API call

## 9. Reproducing the paper's results

The analyses do not call the models. They read the collected data in `data/collected_code_6/` and `results/` and write their output to `results/`, overwriting existing files. All of them run with the same command format, replacing `<name>` with the analysis name:

```sh
.venv/bin/python -m src.analysis.<name>
```

Paper numbers, with the research question (RQ) of each analysis:

- overall numbers of the results section: `final_numbers`
- RQ1: `consistency_tests`
- RQ2: `rq2_3channel --model <id>` and `rq2_paired --model <id>`
- RQ3: `thinking_paired_all` and `rq3_paired`
- RQ4: `rq4_sampling`
- RQ5: `rq5_cost_benefit`, `rq5_cost_dynamics`, and `rq5_usd_costs`

Figures:

- Venn diagram of the channels: `problem_channel_venn`, then `make_venn_figure`
- reasoning token histograms: `thinking_token_histogram`
- Kaplan-Meier curves: `rq5_cost_dynamics`

## 10. Notes

- API calls have a cost, so we recommend starting with the quick test.
- LLMs do not always produce the same answer, even with the same parameters. A new run therefore produces code different from the code we collected. To check the paper's numbers, we use the collected data (Section 9).
- The test harnesses, which run the test cases of each task, are already in `data/harnesses/<id>/spec.json`. The `mbpp/mbpp.jsonl` file, from the Mostly Basic Python Problems (MBPP) dataset, is only needed to rebuild them.

## 11. Version 3: finding tracking

### 11.1 What it does

In v2, a program counts as repaired when the gate stops reporting findings. A finding can stop being reported because the problem was fixed, but also because the model rewrote the line, moved the code to another function, or replaced a call with another one that has the same CWE, or because it went away and came back. Version 3 follows each finding from one round to the next and counts how it left. A finding is identified by its rule, its function, the text of its line and its position among equal findings, so it survives the line shifts of every repair.

It also adds two repair arms that need new runs, both on the same initial programs as the v2 cell `feedback-static-r0-t1.0-p1.0`:

| Arm | Question | What changes |
|---|---|---|
| B | Does reasoning help when it is on only during repair? | model id ending in `-repairthink` (reasoning on) repairs the reasoning-off programs |
| C | Does telling the model which findings moved change how it repairs? | feedback `static+history`: the same findings and stopping rule, plus a `/* Repair history */` block saying what moved |

Arm B is the priority. Arm C is optional.

### 11.2 Offline analysis

These commands use only the collected v2 data, with no keys and no analyzers:

```sh
.venv/bin/python -m src.analysis.relocation
.venv/bin/python -m src.analysis.relocation_sample --n 150 --seed 2027
.venv/bin/python -m src.analysis.relocation_context
.venv/bin/python -m src.analysis.relocation_judge_report
```

They count the events per model and cell, draw 150 events (one per program, balanced across models), gather the full programs of those events, and summarize the judge labels.

### 11.3 LLM judges and the human check

Two judges (Claude Opus 5.5 agents) read each sampled event independently, without knowing which model wrote the code; a third decides when they disagree. They answer whether the flagged code before and after the move is a real weakness in that program (assuming the usual C contracts) and whether it is the same operation, only moved. The label follows from the answers: SAME (the weakness moved and persists), BENIGN (neither is a weakness), FIX+NEW (the old weakness is gone), UNCLEAR.

Result: 149 of the 150 events are BENIGN, 1 is FIX+NEW, none is SAME, and the two judges agreed on all of them. Because both judges are the same model, that agreement is an upper bound. Two people label the 30 random events in `results/finding_tracking/relocation_human_check.md` (sheet: `relocation_human_check.csv`) before reading the judge report; running the report again adds the agreement.

### 11.4 Checking a cell without spending

`replay_check` runs a cell from start to end in a temporary folder, but reuses the repairs recorded in a v2 cell instead of calling the model. It checks the import of the initial programs, the analysis, the feedback and the manifest, and writes nothing to the repository.

```sh
.venv/bin/python -m src.pipeline.replay_check --model openai-gpt56-sol-repairthink --gen-source-model openai-gpt56-sol --feedback static --tasks 544,607,747,794 --replay openai-gpt56-sol/feedback-static-r0-t1.0-p1.0
.venv/bin/python -m src.pipeline.replay_check --model openai-gpt56-sol --feedback static+history --tasks 544,607,747,794 --replay openai-gpt56-sol/feedback-static-r0-t1.0-p1.0
```

The first must report the cell `openai-gpt56-sol-repairthink/feedback-static-r1-t1.0-p1.0` with 4 imported programs; the second must show history blocks in rounds 2 and 3. Missing analyzers are skipped with a warning, so outcomes can differ from the recorded cell.

### 11.5 Running the new arms

A quick test with one task (`--tasks 2 --samples 1`) costs cents. Then:

**Arm B:**

```sh
for m in deepseek-v4-pro glm-5.1 kimi-k3 openai-gpt56-sol; do
  .venv/bin/python -m src.pipeline.run_experiment --model "$m-repairthink" --feedback static --tasks subset --samples 3 --gen-source feedback-static-r0-t1.0-p1.0 --gen-source-model "$m" --gen-source-reports
done
```

**Arm C (optional):**

```sh
for m in deepseek-v4-pro openai-gpt56-sol kimi-k3; do
  .venv/bin/python -m src.pipeline.run_experiment --model "$m" --feedback static+history --tasks subset --samples 3 --gen-source feedback-static-r0-t1.0-p1.0 --gen-source-reports
done
```

The initial programs and their reports are copied, not generated again. Arm B lands in `data/collected_code_6/<model>-repairthink/feedback-static-r1-t1.0-p1.0/` and `results/<model>-repairthink/`; arm C in `data/collected_code_6/<model>/feedback-static+history-r0-t1.0-p1.0/` and `results/<model>/`.

Estimated cost with worst-case input prices from the v2 usage records:

| Plan | Arm B | Arm C |
|---|---|---|
| `--samples 1` | about US$42 (up to 93) | about US$20 (up to 40) |
| `--samples 3` | about US$121 (up to 267) | about US$57 (up to 115) |

Without GPT-5.6 Sol, arm B costs about US$23 or US$66. Each model takes about 2 to 5 hours per arm, and models from different providers can run at the same time.

### 11.6 Analyzing the arms

```sh
.venv/bin/python -m src.analysis.relocation --tasks subset --models deepseek-v4-pro glm-5.1 kimi-k3 openai-gpt56-sol --slug feedback-static-r0-t1.0-p1.0 --name arm_a
.venv/bin/python -m src.analysis.relocation --tasks subset --models deepseek-v4-pro-repairthink glm-5.1-repairthink kimi-k3-repairthink openai-gpt56-sol-repairthink --slug feedback-static-r1-t1.0-p1.0 --name arm_b
```

With `--samples 1` runs, we add `--samples 1` so the baseline covers the same programs.

### 11.7 Reports

All v3 outputs are in `results/finding_tracking/`: `relocation.md` and `.json` (events per model and cell), `relocation_events.csv` (every cross-function event), `relocation_sample.*` (the 150 events), `relocation_sample_context.json` (their full programs), `relocation_judge.*` (judge labels and summary) and `relocation_human_check.*` (the human check).

The v2 limits also apply: GCC runs with `-w`, which hides its `-fanalyzer` warnings, and Clang ran without Z3, so most findings, and 97% of the cross-function events, come from Flawfinder, which matches text patterns.
