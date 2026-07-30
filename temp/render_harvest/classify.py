#!/usr/bin/env python3
"""First-pass mechanical classification of temp/render_legacy files.

For every legacy file, find its counterpart(s) in the rebuilt render tree
(sources/vostok/render) by basename, then by defined-symbol content, and
emit a TSV manifest with one row per legacy file:

    legacy_path  first_pass_class  new_tree_counterparts  symbol_hits  notes

First-pass classes (refined by hand/agents later):
    DX9              - under dx9/; target exe is DX11-only (0 dx9 units in match.db)
    BASENAME         - same-stem file exists in the new tree
    SYMBOL           - >=1 defined class/struct symbol found in the new tree
    DEAD             - defines classes, NONE of which appear in ANY target symbol
                       (unused retail code, e.g. the blender_* shader system)
    UNMAPPED         - nothing found; renamed, data-only, or reconstruction candidate
The target_evidence column counts this file's defined classes that appear
somewhere in the shipped exe's demangled symbols (any module).
Run from the repo root:  python3 temp/render_harvest/classify.py
"""
import os
import re
import sqlite3
import sys
from collections import defaultdict

LEGACY = "temp/render_legacy"
NEW = "sources/vostok/render"
OUT = "temp/render_harvest/manifest.tsv"

DEF_RE = re.compile(
    r"^\s*(?:template\s*<[^>]*>\s*)?(?:class|struct|enum)\s+"
    r"(?:VOSTOK_\w+_API\s+)?(\w+)\b(?!\s*;)",
    re.M,
)
METHOD_RE = re.compile(r"^\s*(?:[\w:<>,~*&\s]+?)?\b(\w+)\s*::\s*~?(\w+)\s*\(", re.M)


def read(path):
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            return f.read()
    except OSError:
        return ""


def stem(name):
    for ext in (".cpp", ".h", ".hpp", ".inl"):
        if name.endswith(ext):
            return name[: -len(ext)]
    return name


def target_tokens():
    """All word tokens appearing in any target function's demangled symbol."""
    db = sqlite3.connect("docs/binary_matching/match.db")
    toks = set()
    for (sym,) in db.execute(
        "SELECT s.demangled FROM target_functions tf JOIN symbols s ON tf.sym = s.id"
    ):
        if sym:
            toks.update(re.findall(r"\w+", sym))
    return toks


def main():
    ttoks = target_tokens()
    # --- index the new tree ---------------------------------------------
    new_by_stem = defaultdict(list)   # stem -> [paths]
    new_defs = defaultdict(set)       # class/struct/enum name -> {paths}
    new_stub_counts = {}              # path -> STATE[STUB] count
    for root, _dirs, files in os.walk(NEW):
        for fn in files:
            p = os.path.join(root, fn)
            new_by_stem[stem(fn)].append(p)
            text = read(p)
            new_stub_counts[p] = text.count("STATE[STUB]")
            for m in DEF_RE.finditer(text):
                new_defs[m.group(1)].add(p)

    # --- classify each legacy file ---------------------------------------
    rows = []
    counts = defaultdict(int)
    for root, _dirs, files in os.walk(LEGACY):
        for fn in sorted(files):
            p = os.path.join(root, fn)
            rel = os.path.relpath(p, LEGACY)
            if rel == "triage_log.md":
                continue
            text = read(p)

            # symbols this legacy file defines / implements
            syms = set(m.group(1) for m in DEF_RE.finditer(text))
            if fn.endswith(".cpp"):
                syms |= set(m.group(1) for m in METHOD_RE.finditer(text))
            syms.discard("vostok")

            base_hits = new_by_stem.get(stem(fn), [])
            sym_hits = defaultdict(int)  # new path -> #syms found there
            for s in syms:
                for np in new_defs.get(s, ()):
                    sym_hits[np] += 1
            top_sym = sorted(sym_hits.items(), key=lambda kv: -kv[1])[:3]

            evidenced = sorted(s for s in syms if s in ttoks)
            if rel.startswith("dx9" + os.sep):
                cls = "DX9"
            elif base_hits:
                cls = "BASENAME"
            elif top_sym:
                cls = "SYMBOL"
            elif syms and not evidenced:
                cls = "DEAD"
            else:
                cls = "UNMAPPED"
            counts[cls] += 1

            counterparts = base_hits or [t[0] for t in top_sym]
            stubs = sum(new_stub_counts.get(c, 0) for c in counterparts)
            rows.append(
                "\t".join(
                    [
                        rel,
                        cls,
                        ";".join(counterparts) or "-",
                        ";".join(f"{os.path.basename(t[0])}:{t[1]}" for t in top_sym) or "-",
                        f"target_evidence={len(evidenced)}/{len(syms)}"
                        + (":" + ",".join(evidenced[:5]) if evidenced else ""),
                        f"stubs_in_counterpart={stubs}",
                    ]
                )
            )

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        f.write("# legacy_path\tclass\tcounterparts\tsymbol_hits\ttarget_evidence\tnotes\n")
        f.write("\n".join(rows) + "\n")

    total = sum(counts.values())
    print(f"{total} legacy files classified -> {OUT}")
    for k in ("BASENAME", "SYMBOL", "DEAD", "UNMAPPED", "DX9"):
        print(f"  {k:10s} {counts[k]}")


if __name__ == "__main__":
    sys.exit(main())
