# Finding Tracking Manual (v3)

This manual explains what version 3 of the experiment does, how to run it, and how to check that it works before paying for API calls. Version 3 builds on the v2 pipeline (`reproduction.md`) and reuses its collected data.

## Contents

1. [What v3 does](#1-what-v3-does)
2. [What changed in the code](#2-what-changed-in-the-code)
3. [Offline analysis](#3-offline-analysis)
4. [LLM judges and the human check](#4-llm-judges-and-the-human-check)
5. [Checking a new cell without spending](#5-checking-a-new-cell-without-spending)
6. [Running the new repair arms](#6-running-the-new-repair-arms)
7. [Analyzing the new arms](#7-analyzing-the-new-arms)
8. [Generated reports](#8-generated-reports)
9. [Notes](#9-notes)

## 1. What v3 does

In v2, a program counts as repaired when the analysis gate stops reporting findings. A finding can stop being reported because the problem was fixed, but also because the model rewrote the flagged line, moved the code to another function, replaced a call with another one that has the same CWE, or because a finding went away and came back later.

In v3 we follow each finding from one repair round to the next and count how it left. A finding is identified by its rule, the function it is in, the text of its line, and its position among equal findings, so it survives the line shifts of every repair. We call the events:

- **relocation**: the rule leaves one function and appears in another;
- **new function**: the same, but the function did not exist before (a new helper);
- **substitution**: a Flawfinder call is replaced by a call with the same CWE;
- **reappearance**: a finding that had gone comes back.

Two LLM judges then read a sample of relocation events and decide whether the moved code is a real weakness, and two people check part of their labels.

v3 also adds two repair arms that need new runs: repair with reasoning only during repair (arm B), and repair feedback that names the findings that moved (arm C).

## 2. What changed in the code

| File | What it does |
|---|---|
| `src/pipeline/finding_track.py` | Identifies findings across rounds and writes the repair-history notes |
| `src/pipeline/comment_inject.py` | Feedback modes `static+history` and `static+dynamic+history`: the same findings and stopping rule as `static` and `static+dynamic`, plus a `/* Repair history */` block at the top of the program |
| `src/pipeline/run_experiment.py` | Option `--gen-source-model`, to repair the initial programs of another model id |
| `src/pipeline/replay_check.py` | Runs a cell end to end without calling the model (Section 5) |
| `config/models.yaml` | Ids ending in `-repairthink`: the same settings as `-thinking`, in their own folder |
| `src/analysis/relocation.py` | Counts the events per model and cell |
| `src/analysis/relocation_sample.py` | Draws 150 events, one per program, balanced across models |
| `src/analysis/relocation_context.py` | Gathers the full programs and analyzer messages of each sampled event |
| `src/analysis/relocation_judge_report.py` | Summarizes the judge labels and compares them with the human labels |

The v2 cells are not changed, and the `static` and `static+dynamic` modes produce the same feedback as before.

## 3. Offline analysis

These commands use only the collected v2 data. They need no API keys and no analyzers.

---

**Event counts**, for the seven models, the static and full-gate cells, rounds 0 to 5:

```sh
.venv/bin/python -m src.analysis.relocation
```

---

**Sample for validation** (150 events) and its full context:

```sh
.venv/bin/python -m src.analysis.relocation_sample --n 150 --seed 2027
.venv/bin/python -m src.analysis.relocation_context
```

---

**Judge report and human sheet:**

```sh
.venv/bin/python -m src.analysis.relocation_judge_report
```

---

`relocation.py` also accepts `--tasks subset`, `--samples N`, `--models ...`, `--slug ...` and `--name ...`, which Section 7 uses.

## 4. LLM judges and the human check

Each sampled event was read by two judges working independently, with the same instructions and without knowing which model wrote the code. When they disagreed, a third judge decided. The judges were Claude Opus 5.5 agents, and their labels are in `results/finding_tracking/relocation_judge.json`.

For each event the judges answer three questions with yes, no or unclear:

1. Is the flagged code before the move a real weakness in this program? We assume the usual C contracts (strings end in `\0`, sizes passed by callers are right) unless the program breaks them.
2. Is the flagged code after the move a real weakness?
3. Is it the same operation on the same data, only moved?

The label follows from the answers: **UNCLEAR** if any answer is unclear; **SAME** if the code after the move is a weakness and it is the same operation; **BENIGN** if neither is a weakness; **FIX+NEW** otherwise.

Result: the two judges gave the same label to all 150 events. 149 are BENIGN and 1 is FIX+NEW (a fuzzing finding); none is SAME. Because both judges are the same model, this agreement is an upper bound.

**Human check.** `relocation_human_check.md` has 30 random events with their full programs. Two people label them in `relocation_human_check.csv`, columns `label_a` and `label_b`, without opening `relocation_judge.md` first. Then we run the judge report again, and it adds the agreement between the people and between the people and the judges.

## 5. Checking a new cell without spending

`replay_check` runs a cell from start to end in a temporary folder, but instead of calling the model it reuses the repairs already recorded in a v2 cell. It checks the import of the initial programs, the analysis, the feedback, and the manifest. Nothing is written to the repository.

---

**Arm C** (history feedback):

```sh
.venv/bin/python -m src.pipeline.replay_check --model openai-gpt56-sol --feedback static+history --tasks 544,607,747,794 --replay openai-gpt56-sol/feedback-static-r0-t1.0-p1.0
```

Expected: rounds 2 and 3 show programs with a history block, for example `Flawfinder strlen was removed from longest_common_subsequence() but now appears in main()`.

---

**Arm B** (reasoning only during repair):

```sh
.venv/bin/python -m src.pipeline.replay_check --model openai-gpt56-sol-repairthink --gen-source-model openai-gpt56-sol --feedback static --tasks 544,607,747,794 --replay openai-gpt56-sol/feedback-static-r0-t1.0-p1.0
```

Expected: the cell is `openai-gpt56-sol-repairthink/feedback-static-r1-t1.0-p1.0`, the 4 initial programs are imported from the reasoning-off cell, and the model is never called.

---

Analyzers missing on the machine are skipped with a warning, so the outcomes can differ from the recorded cell. We ran both checks on 2026-10-13 without errors.

## 6. Running the new repair arms

Both arms repair the same initial programs as the v2 cell `feedback-static-r0-t1.0-p1.0` (reasoning off), on the 200-task subset, with static feedback and the 5-round limit. The initial programs and their reports are copied, not generated again, so any difference comes from the repair. The keys, tools and proxy are the same as in `reproduction.md`. Before each arm, a quick test with one task (`--tasks 2 --samples 1`) costs cents.

| Arm | Question | What changes |
|---|---|---|
| A (baseline) | already collected | nothing to run |
| B | Does reasoning help when it is on only during repair? | reasoning on during repair |
| C | Does naming the findings that moved change how the model repairs? | `+history` feedback |

**Status of arm C:** the judges found that almost all relocations move code that was already safe (Section 4). The note of arm C tells the model that moving the code does not fix the weakness, which is wrong for those cases, so arm C stays on hold until its message is reviewed. Arm B is ready.

---

**Arm B:**

```sh
for m in deepseek-v4-pro openai-gpt56-sol kimi-k3 glm-5.1; do
  .venv/bin/python -m src.pipeline.run_experiment --model "$m-repairthink" --feedback static --tasks subset --samples 3 --gen-source feedback-static-r0-t1.0-p1.0 --gen-source-model "$m" --gen-source-reports
done
```

---

**Arm C** (after its message is reviewed):

```sh
for m in deepseek-v4-pro openai-gpt56-sol kimi-k3; do
  .venv/bin/python -m src.pipeline.run_experiment --model "$m" --feedback static+history --tasks subset --samples 3 --gen-source feedback-static-r0-t1.0-p1.0 --gen-source-reports
done
```

---

Where the data lands:

| Arm | Cell folder | Manifest |
|---|---|---|
| B | `data/collected_code_6/<model>-repairthink/feedback-static-r1-t1.0-p1.0/` | `results/<model>-repairthink/feedback-static-r1-t1.0-p1.0.manifest.json` |
| C | `data/collected_code_6/<model>/feedback-static+history-r0-t1.0-p1.0/` | `results/<model>/feedback-static+history-r0-t1.0-p1.0.manifest.json` |

Estimated cost, with worst-case input prices from the v2 usage records:

| Plan | Arm B | Arm C |
|---|---|---|
| `--samples 1` | about US$42 (up to 93) | about US$20 (up to 40) |
| `--samples 3` | about US$121 (up to 267) | about US$57 (up to 115) |

Without GPT-5.6 Sol, arm B costs about US$23 (`--samples 1`) or US$66 (`--samples 3`). Each model takes about 2 to 5 hours per arm; models from different providers can run at the same time.

## 7. Analyzing the new arms

We compare each arm with the baseline on the same tasks:

```sh
.venv/bin/python -m src.analysis.relocation --tasks subset --models deepseek-v4-pro openai-gpt56-sol kimi-k3 glm-5.1 --slug feedback-static-r0-t1.0-p1.0 --name arm_a
.venv/bin/python -m src.analysis.relocation --tasks subset --models deepseek-v4-pro-repairthink openai-gpt56-sol-repairthink kimi-k3-repairthink glm-5.1-repairthink --slug feedback-static-r1-t1.0-p1.0 --name arm_b
.venv/bin/python -m src.analysis.relocation --tasks subset --models deepseek-v4-pro openai-gpt56-sol kimi-k3 --slug feedback-static+history-r0-t1.0-p1.0 --name arm_c
```

If the arms ran with `--samples 1`, we add `--samples 1` to the three commands so the baseline covers the same programs. Paired tests use the v2 statistics module (`src/analysis/stats.py`).

## 8. Generated reports

All v3 outputs are in `results/finding_tracking/`:

| File | Content |
|---|---|
| `relocation.md`, `relocation.json` | Events per model and cell, programs accepted after each event, where re-added findings land, cross-function events per tool |
| `relocation_events.csv` | Every cross-function event |
| `relocation_sample.md`, `relocation_sample.csv` | The 150 sampled events (the two functions) |
| `relocation_sample_context.json` | Full programs and analyzer messages of the sampled events |
| `relocation_judge.json`, `relocation_judge.md` | Judge labels and their summary |
| `relocation_human_check.md`, `relocation_human_check.csv` | The 30 events for the human check and the label sheet |

## 9. Notes

- The v2 configuration limits also apply here: GCC runs with `-w`, which hides its `-fanalyzer` warnings, and Clang ran without Z3. Most findings, and 97% of the cross-function events, come from Flawfinder, which matches text patterns.
- In every mode, the order of messages inside one feedback comment can change between runs; the messages themselves are the same. This comes from v2.
- `replay_check` uses whatever analyzers this machine has, so it checks that the pipeline works, not that the numbers match.
