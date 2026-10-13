"""Inject analyzer findings into C sources as comments (the repair feedback).

Refactor of v1 ``generate_commented_weak_code.py``. Consumes report files in
the standard ``file:line:col:severity:message`` format from BOTH channels:

- static:  .gcc.txt, .clang.txt, .cppcheck.txt, .flawfinder.txt
- dynamic: .asan.txt, .fuzz.txt, .test.txt

and inserts ``/* Possible weaknesses found: ... */`` blocks above flagged
lines. The feedback mode selects which suffixes are consumed. Modes ending
in ``+history`` (v3) also prepend a ``/* Repair history */``
block naming findings that moved to another function or were replaced by a
call with the same CWE since the previous round (see ``finding_track``).

File naming: basenames are ``problem-{id}.c`` (single sample) or
``problem-{id}-s{k}.c`` (multi-sample). Reports share the basename, e.g.
``problem-5-s2.gcc.txt``.
"""
from __future__ import annotations

import os
import re
from pathlib import Path
from typing import Iterable

from . import finding_track, flawfinder_norm

from tqdm import tqdm

STATIC_SUFFIXES = [".gcc.txt", ".clang.txt", ".cppcheck.txt", ".flawfinder.txt"]
DYNAMIC_SUFFIXES = [".asan.txt", ".fuzz.txt", ".test.txt"]

CHANNEL_SUFFIXES = {
    "none": [],
    "static": STATIC_SUFFIXES,
    "dynamic": DYNAMIC_SUFFIXES,
    "static+dynamic": STATIC_SUFFIXES + DYNAMIC_SUFFIXES,
    "static+history": STATIC_SUFFIXES,
    "static+dynamic+history": STATIC_SUFFIXES + DYNAMIC_SUFFIXES,
}

_PROBLEM_RE = re.compile(r"^problem-(\d+)(-s\d+)?$")


def generate_commented_weak_code(
    input_dir: str,
    output_dir: str,
    feedback: str = "static",
    task_ids: Iterable[int] | None = None,
    resume: bool = False,
    history_dir: str | None = None,
) -> int:
    """Write commented (annotated) sources for all flagged files.

    ``history_dir`` holds the previous round's programs and reports; it is
    used only by ``+history`` feedback modes.

    Returns the number of annotated files written.
    """
    suffixes = CHANNEL_SUFFIXES.get(feedback, STATIC_SUFFIXES)
    with_history = feedback.endswith("+history") and history_dir is not None
    findings = get_problems_weaknesses(input_dir, suffixes, task_ids)
    create_output_folders(output_dir)
    written = 0
    for basename, errors in tqdm(findings.items(), "Generating comments on weak code: "):
        src = os.path.join(input_dir, basename + ".c")
        dst = os.path.join(output_dir, basename + ".c")
        if not errors or not os.path.isfile(src):
            continue
        if resume and os.path.isfile(dst):
            written += 1
            continue
        with open(src) as inputfile:
            with open(dst, "w") as outputfile:
                try:
                    commented = add_comments_to_weak_source(
                        input=inputfile.read(), errors=errors
                    )
                    if with_history:
                        commented = history_header(finding_track.history_notes(
                            Path(history_dir), Path(input_dir), basename, suffixes
                        )) + commented
                    outputfile.write(commented)
                    written += 1
                except Exception as ex:  # noqa: BLE001
                    print(f"{basename}: {ex}")
    return written


def create_output_folders(path: str) -> None:
    os.makedirs(path, exist_ok=True)


def get_problems_weaknesses(
    input_dir: str,
    suffixes: list[str],
    task_ids: Iterable[int] | None = None,
) -> dict[str, dict[int, list[str]]]:
    """Return {basename: {line: [messages]}} for every .c with findings."""
    allowed = set(task_ids) if task_ids is not None else None
    result: dict[str, dict[int, list[str]]] = {}
    for filename in sorted(os.listdir(input_dir)):
        if not filename.endswith(".c"):
            continue
        basename = filename[:-2]
        m = _PROBLEM_RE.match(basename)
        if not m:
            continue
        if allowed is not None and int(m.group(1)) not in allowed:
            continue
        errors: dict[int, list[str]] = {}
        for suffix in suffixes:
            path = os.path.join(input_dir, basename + suffix)
            if os.path.isfile(path):
                with open(path) as f:
                    content = f.read()
                    msgs: list[dict] = []
                    if suffix == ".flawfinder.txt":
                        # Flawfinder reports exist in two formats: the raw
                        # tool output (pre-2026-09-19) and the normalized
                        # standard format. Parse BOTH; only one matches a
                        # given file, so there is no double counting.
                        msgs = (extract_standard_messages(content)
                                + flawfinder_norm.extract_flawfinder_raw(content))
                    else:
                        msgs = extract_standard_messages(content)
                    for msg in msgs:
                        if msg["line"] <= 0:
                            continue  # info/error lines are never comment targets
                        errors.setdefault(msg["line"], []).append(msg["message"])
        if errors:
            result[basename] = errors
    return result


def extract_standard_messages(input: str) -> list[dict]:
    """Parse ``file:line:col:severity:message`` lines (shared by all tools)."""
    output = []
    for line in input.split("\n"):
        tokens = line.split(":")
        if len(tokens) >= 5:
            try:
                output.append(
                    {"line": int(tokens[1]), "message": ":".join(tokens[4:])}
                )
            except ValueError:
                continue
    return output


# v1-compatible aliases
extract_gcc_messages = extract_standard_messages
extract_cppcheck_messages = extract_standard_messages
extract_asan_messages = extract_standard_messages
extract_fuzz_messages = extract_standard_messages
extract_test_messages = extract_standard_messages


def extract_flawfinder_messages(input: str) -> list[dict]:
    output = []
    lines = input.split("\n")
    for i in range(0, len(lines)):
        current_line = lines[i]
        tokens = current_line.split(":")
        if len(tokens) == 4:
            reason = ""
            j = i + 1
            next_line = lines[j]
            next_line_tokens = next_line.split(":")
            while len(next_line_tokens) != 4 and next_line != "":
                reason += next_line
                j = j + 1
                next_line = lines[j]
                next_line_tokens = next_line.split(":")
            output.append({"line": int(current_line.split(":")[1]), "message": reason})
    return output


def add_comments_to_weak_source(input: str, errors: dict[int, list[str]]) -> str:
    error_line_numbers = sorted(errors.keys(), reverse=True)
    first_line = "/* Possible weaknesses found:\n"
    weakness_line_start = " * "
    weakness_line_end = "\n"
    last_line_end = " */\n"
    input_lines = input.splitlines(keepends=True)
    for error_line in error_line_numbers:
        weaknesses_comment = ""
        try:
            padding = leading_spaces(input_lines[error_line - 1])
        except Exception:  # noqa: BLE001
            continue
        weaknesses_comment += padding + first_line
        for error_msg in set(errors[error_line]):
            weaknesses_comment += padding + weakness_line_start + error_msg + weakness_line_end
        weaknesses_comment += padding + last_line_end
        input_lines.insert(error_line - 1, weaknesses_comment)
    return "".join(input_lines)


def history_header(notes: list[str]) -> str:
    if not notes:
        return ""
    return ("/* Repair history (previous round):\n"
            + "".join(f" * - {n}\n" for n in notes) + " */\n")


def leading_spaces(s: str) -> str:
    match = re.search(r"^\s*", s)
    if match.end():
        return " " * match.end()
    return ""


if __name__ == "__main__":
    import sys

    feedback = sys.argv[3] if len(sys.argv) > 3 else "static"
    print(generate_commented_weak_code(sys.argv[1], sys.argv[2], feedback))
