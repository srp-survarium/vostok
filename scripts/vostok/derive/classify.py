# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.derive.classify - the structure verdict for one paired function.

Bytes are objdiff's business. This asks the other question: do the two sides
have the same SOURCE STATEMENTS? Four answers, and the difference between them
decides whether a residual is steerable work at all:

    MATCH     statements identical
    SIZE      same count and geometry, some statement's bytes differ
    QUANTITY  the statement COUNTS differ - real missing/extra source
    SPLIT     equal counts, but alignment left rows unpairable

Counted over BODY statements only. The frame braces are codegen, not source:
including them let a prologue that merely differs in size register as a
statement difference, which is the opposite of what this asks.
"""

import difflib

from vostok.derive.index import body_statements


def stmt_seq(rec):
    """Primary alignment alphabet: statement sizes."""
    return [s["size"] for s in body_statements(rec)]


def normalized_stmt_lines(rec):
    """Return function-relative PDB line numbers when every row has one.

    Absolute lines legitimately move as files are reconstructed. Exact relative
    line geometry, however, is strong evidence that equal-count rows correspond
    in source order even when repeated statement sizes confuse SequenceMatcher.
    """
    lines = [s.get("line") for s in body_statements(rec)]
    if not lines or any(not isinstance(line, int) for line in lines):
        return None
    first = lines[0]
    return [line - first for line in lines]


def classify(t_rec, b_rec):
    """Approximate structure classification for queue ranking (the authoritative
    per-function verdict stays `pdb_fetch --view structure-diff`)."""
    t_seq, b_seq = stmt_seq(t_rec), stmt_seq(b_rec)
    t_n, b_n = len(t_seq), len(b_seq)
    if t_seq == b_seq:
        return "MATCH", t_n, b_n, 0, 0, 0

    # Prefer exact PDB line geometry over a size-only fuzzy alignment. This is
    # deliberately an exact shortcut: differing relative lines still fall back
    # to the established size alignment below.
    t_lines = normalized_stmt_lines(t_rec)
    if (
        t_n == b_n
        and t_lines is not None
        and t_lines == normalized_stmt_lines(b_rec)
    ):
        n_size = sum(t_size != b_size for t_size, b_size in zip(t_seq, b_seq))
        return "SIZE", t_n, b_n, n_size, 0, 0

    sm = difflib.SequenceMatcher(a=t_seq, b=b_seq, autojunk=False)
    n_size = n_trgt_only = n_base_only = 0
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "replace":
            paired = min(i2 - i1, j2 - j1)
            n_size += paired
            n_trgt_only += (i2 - i1) - paired
            n_base_only += (j2 - j1) - paired
        elif op == "delete":
            n_trgt_only += i2 - i1
        elif op == "insert":
            n_base_only += j2 - j1

    if t_n != b_n:
        cls = "QUANTITY"
    elif n_trgt_only or n_base_only:
        cls = "SPLIT"  # equal counts, but alignment left unpairable rows
    else:
        cls = "SIZE"
    return cls, t_n, b_n, n_size, n_trgt_only, n_base_only
