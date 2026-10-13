"""Track analyzer findings across repair rounds (v3).

A finding is keyed by (tool:rule, enclosing function, normalized source line,
occurrence index); line numbers alone are useless because every repair
shifts them. The offline relocation analysis (``src/analysis/relocation``)
and the ``+history`` feedback mode (``comment_inject``) share these keys, so
what the model is told and what the paper measures are the same events.
"""
from __future__ import annotations

import re
from collections import Counter, defaultdict
from pathlib import Path

STD = re.compile(r"^(.*?):(\d+):(\d+):\s*([a-zA-Z ]+?):(.*)$")
FF_CWE = re.compile(r"\((CWE-[0-9][^)]*)\)")
IDENT_CALL = re.compile(r"([A-Za-z_]\w*)\s*\(")
KEYWORDS = {"if", "for", "while", "switch", "return", "do", "else", "sizeof",
            "__attribute__"}
TOOL_NAMES = {"gcc": "GCC", "clang": "Clang", "cppcheck": "Cppcheck",
              "flawfinder": "Flawfinder", "asan": "Sanitizer", "fuzz": "Fuzzing"}


def rule_of(tool: str, sev: str, msg: str) -> str | None:
    msg = msg.strip()
    if sev == "note":
        return None
    if tool in ("gcc", "clang") and sev in ("error", "fatal error"):
        return "COMPILE"
    if tool in ("gcc", "clang", "cppcheck"):
        pat = r"\[(-W[\w.=-]+)\]" if tool == "gcc" else r"\[([\w.]+)\]\s*$"
        m = re.search(pat, msg)
        return f"{tool}:" + (m.group(1) if m else re.sub(r"'[^']*'", "X", msg)[:50])
    if tool == "flawfinder":
        m = re.match(r"Flawfinder (\w+)", msg)
        return "flawfinder:" + (m.group(1) if m else msg[:40])
    if tool in ("asan", "fuzz"):
        m = re.search(r"(ASan|UBSan): ([A-Za-z -]+)", msg)
        if m:
            return f"{tool}:{m.group(1)}:{m.group(2).strip()[:40]}"
        if "Fuzzing found" in msg:
            return None  # crash signal without a source location
        return f"{tool}:" + msg[:40]
    return None


def parse_reports(d: Path, base: str, suffixes: list[str]):
    """(rule, line) findings for ``base`` in round dir ``d``, plus the CWE that
    Flawfinder attaches to each of its rules."""
    found, cwe_of = [], {}
    for suf in suffixes:
        p = d / f"{base}{suf}"
        if not p.is_file():
            continue
        tool = suf.split(".")[1]
        for raw in p.read_text(errors="replace").splitlines():
            m = STD.match(raw.strip())
            if not m or m.group(1).rsplit("/", 1)[-1] != f"{base}.c":
                continue
            line = int(m.group(2))
            rule = rule_of(tool, m.group(4).strip(), m.group(5))
            if line <= 0 or not rule:
                continue
            found.append((rule, line))
            if tool == "flawfinder":
                c = FF_CWE.search(m.group(5))
                if c:
                    cwe_of[rule] = c.group(1)
    return found, cwe_of


def _clean(s: str, state: dict) -> str:
    """Source line without comments, string/char contents or preprocessor."""
    if state["block"]:
        if "*/" not in s:
            return ""
        s = s.split("*/", 1)[1]
        state["block"] = False
    s = re.sub(r"/\*.*?\*/", "", s)
    if "/*" in s:
        s = s.split("/*", 1)[0]
        state["block"] = True
    s = re.sub(r"//.*$", "", s)
    s = re.sub(r'"(\\.|[^"\\])*"', '""', s)
    s = re.sub(r"'(\\.|[^'\\])*'", "''", s)
    return "" if s.lstrip().startswith("#") else s


def func_map(src_lines: list[str]) -> list[str]:
    """Enclosing top-level function of each line (header lines included)."""
    state = {"block": False}
    clean = [_clean(s, state) for s in src_lines]
    names = ["<global>"] * len(src_lines)
    depth, start, buf, cur = 0, 0, "", "<global>"
    for i, s in enumerate(clean):
        if depth > 0:
            names[i] = cur
            depth = max(0, depth + s.count("{") - s.count("}"))
            continue
        if not buf.strip():
            start = i
        buf += " " + s
        if "{" in s:
            pre = buf.split("{", 1)[0]
            cands = [m.group(1) for m in IDENT_CALL.finditer(pre)
                     if m.group(1) not in KEYWORDS]
            is_fn = bool(cands) and ")" in pre and "=" not in pre.split("(", 1)[0]
            cur = cands[0] if is_fn else "<global>"
            for j in range(start, i + 1):
                names[j] = cur
            depth = max(0, s.count("{") - s.count("}"))
            buf = ""
        elif s.rstrip().endswith((";", "}")):
            buf = ""
    return names


def located(found, src: str):
    """Shift-robust keys (rule, function, normalized line text, occurrence)."""
    lines = src.splitlines()
    fmap = func_map(lines)
    occ, keys = Counter(), []
    for rule, ln in sorted(found, key=lambda x: (x[1], x[0])):
        if 0 < ln <= len(lines):
            text = re.sub(r"\s+", "", re.sub(r"//.*$", "", re.sub(r"/\*.*?\*/", "", lines[ln - 1])))
            fn = fmap[ln - 1]
        else:
            text, fn = f"#L{ln}", "<?>"
        occ[(rule, fn, text)] += 1
        keys.append((rule, fn, text, occ[(rule, fn, text)]))
    return keys


def _label(rule: str) -> str:
    tool, _, name = rule.partition(":")
    return f"{TOOL_NAMES.get(tool, tool)} {name}"


def history_notes(prev_dir: Path, cur_dir: Path, base: str,
                  suffixes: list[str]) -> list[str]:
    """Repair-history notes for ``base`` between the previous round's program
    (``prev_dir``) and the current one (``cur_dir``): findings that moved to
    another function, and Flawfinder calls replaced by a call with the same
    CWE. Empty when either program or its reports are missing."""
    prev_src, cur_src = prev_dir / f"{base}.c", cur_dir / f"{base}.c"
    if not prev_src.is_file() or not cur_src.is_file():
        return []
    snapshots = []
    for d, src in ((prev_dir, prev_src), (cur_dir, cur_src)):
        found, cwe_of = parse_reports(d, base, suffixes)
        keys = Counter(k for k in located(found, src.read_text(errors="replace"))
                       if k[0] != "COMPILE")
        snapshots.append((keys, cwe_of))
    (prev, prev_cwe), (cur, cur_cwe) = snapshots
    common = prev & cur
    rem_by, add_by = defaultdict(set), defaultdict(set)
    for k in prev - common:
        rem_by[k[0]].add(k[1])
    for k in cur - common:
        add_by[k[0]].add(k[1])
    notes = []
    for rule in sorted(set(rem_by) & set(add_by)):
        moved_to = add_by[rule] - rem_by[rule] - {"<global>"}
        moved_from = rem_by[rule] - {"<global>"}
        if moved_to and moved_from:
            notes.append(
                f"{_label(rule)} was removed from {', '.join(sorted(moved_from))}() "
                f"but now appears in {', '.join(sorted(moved_to))}(). Moving the "
                "flagged code does not fix the weakness; fix its cause.")
    for a in sorted(set(rem_by) - set(add_by)):
        for b in sorted(set(add_by) - set(rem_by)):
            cwe = prev_cwe.get(a)
            if cwe and cwe == cur_cwe.get(b):
                notes.append(
                    f"{_label(a)} was replaced by {b.partition(':')[2]}, which "
                    f"has the same weakness ({cwe}). Replacing the call does "
                    "not fix it; make the operation safe.")
    return notes
