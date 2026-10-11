# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.evidence.structure - the statement-partition verdict, in-process.

A port of vostok-pdb's `--view structure-diff` (tools/vostok-pdb/src/
rich_structure_diff.rs), so `vostok evidence` reads the verdict from the two
records it already loaded instead of starting the Rust tool, which re-opens
both databases. Same projection, same alignment, same tags:

  1. `structure_rows` turns a statement list into BODY rows: the synthetic
     first/last frame braces are skipped and each source-line gap collapses
     to one empty marker (None), trimmed at both ends.
  2. `diff_structure` LCS-aligns the two streams keyed on statement SIZE
     (deletes win ties, as in the Rust backtrack), then pairs a delete run with
     the insert run that follows it into `changed` rows.
  3. `findings` tags every diverging aligned row: SIZE and/or OFFSET for a
     statement on both sides, TRGT_ONLY / BASE_ONLY for a one-sided one.

`geometry_breaks` is the actionable reading of the relative-line check: where
the paired records' line distance from the opening brace CHANGES, retail had
more (or fewer) source lines before that statement. One break per edit, rather
than one divergent line for every record below it.
"""

from __future__ import annotations


def structure_rows(statements: list[dict]) -> list[dict | None]:
    """BODY rows of one side: statement dicts, None for a collapsed gap."""
    if len(statements) <= 2:
        return []
    rows: list[dict | None] = []
    next_line = statements[0]["line"]
    for s in statements[1:-1]:
        if s["line"] > next_line:
            rows.append(None)
        next_line = s["line"] + 1
        rows.append(s)
    if statements[-1]["line"] > next_line:
        rows.append(None)
    while rows and rows[0] is None:
        rows.pop(0)
    while rows and rows[-1] is None:
        rows.pop()
    return rows


def _key(row):
    return "E" if row is None else f"S{row['size']}"


def diff_structure(base_rows, target_rows) -> list[tuple[str, dict | None, dict | None]]:
    """Aligned rows `(kind, base, target)`; kind is equal / changed / only_base /
    only_target, or the empty_* variants for collapsed gaps."""
    b = [_key(r) for r in base_rows]
    t = [_key(r) for r in target_rows]
    n, m = len(b), len(t)
    dp = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        row, below = dp[i], dp[i + 1]
        for j in range(m - 1, -1, -1):
            row[j] = below[j + 1] + 1 if b[i] == t[j] else max(below[j], row[j + 1])
    ops = []
    i = j = 0
    while i < n and j < m:
        if b[i] == t[j]:
            ops.append(("eq", i, j))
            i, j = i + 1, j + 1
        elif dp[i + 1][j] >= dp[i][j + 1]:
            ops.append(("del", i, None))
            i += 1
        else:
            ops.append(("ins", None, j))
            j += 1
    ops += [("del", k, None) for k in range(i, n)]
    ops += [("ins", None, k) for k in range(j, m)]

    def one_side(row, side):
        if row is None:
            return (f"empty_only_{side}", None, None)
        return (f"only_{side}", row, None) if side == "base" else (f"only_{side}", None, row)

    out = []
    k = 0
    while k < len(ops):
        op, bi, tj = ops[k]
        if op == "eq":
            br, tr = base_rows[bi], target_rows[tj]
            out.append(("empty_equal", None, None) if br is None and tr is None
                       else ("equal", br, tr))
            k += 1
            continue
        if op == "ins":
            out.append(one_side(target_rows[tj], "target"))
            k += 1
            continue
        dels = []
        while k < len(ops) and ops[k][0] == "del":
            dels.append(base_rows[ops[k][1]])
            k += 1
        inss = []
        while k < len(ops) and ops[k][0] == "ins":
            inss.append(target_rows[ops[k][2]])
            k += 1
        ins_stmts = [r for r in inss if r is not None]
        pairs = min(sum(r is not None for r in dels), len(ins_stmts))
        paired = 0
        for br in dels:
            if br is None:
                out.append(("empty_only_base", None, None))
            elif paired < pairs:
                out.append(("changed", br, ins_stmts[paired]))
                paired += 1
            else:
                out.append(one_side(br, "base"))
        consumed = 0
        for tr in inss:
            if tr is None:
                out.append(("empty_only_target", None, None))
            elif consumed < pairs:
                consumed += 1
            else:
                out.append(one_side(tr, "target"))
    return out


def findings(aligned) -> list[dict]:
    """One dict per diverging statement, in alignment order.

    `tags` holds SIZE / OFFSET / TRGT_ONLY / BASE_ONLY; `delta` is base minus
    target bytes (positive: ours is larger and must shrink)."""
    out = []
    for kind, b, t in aligned:
        if kind in ("equal", "changed"):
            tags = []
            if b["size"] != t["size"]:
                tags.append("SIZE")
            if b["off"] != t["off"]:
                tags.append("OFFSET")
            if tags:
                out.append({"tags": tags, "base": b, "target": t,
                            "delta": b["size"] - t["size"],
                            "shift": b["off"] - t["off"]})
        elif kind == "only_target":
            out.append({"tags": ["TRGT_ONLY"], "base": None, "target": t,
                        "delta": -t["size"], "shift": 0})
        elif kind == "only_base":
            out.append({"tags": ["BASE_ONLY"], "base": b, "target": None,
                        "delta": b["size"], "shift": 0})
    return out


def compare(base: dict, target: dict) -> dict:
    """The structure verdict for one pair of records (each with `statements`
    and `size`), with the frame braces' own size deltas split out."""
    base_rows = structure_rows(base["statements"])
    target_rows = structure_rows(target["statements"])
    aligned = diff_structure(base_rows, target_rows)
    rows = findings(aligned)
    counts = {tag: sum(tag in r["tags"] for r in rows)
              for tag in ("TRGT_ONLY", "BASE_ONLY", "SIZE", "OFFSET")}
    t_n = sum(r is not None for r in target_rows)
    b_n = sum(r is not None for r in base_rows)
    if not t_n and not b_n:
        verdict = "NO BODY"
    elif not rows:
        verdict = "MATCH" if base["size"] == target["size"] else "BODY MATCH"
    elif counts["TRGT_ONLY"] or counts["BASE_ONLY"]:
        verdict = "DIFFERS"
    elif counts["SIZE"]:
        verdict = "SIZE"
    else:
        verdict = "OFFSET"
    frame = {}
    bs, ts = base["statements"], target["statements"]
    if len(bs) >= 2 and len(ts) >= 2:
        for name, bi in (("open", 0), ("close", -1)):
            delta = bs[bi]["size"] - ts[bi]["size"]
            if delta:
                frame[name] = delta
    return {"verdict": verdict, "rows": rows, "aligned": aligned, "counts": counts,
            "statements": {"target": t_n, "base": b_n},
            "total_delta": base["size"] - target["size"], "frame": frame}


def _anchor(rec):
    own = [s for s in rec["statements"] if s.get("file", rec["file"]) == rec["file"]]
    return min(own, key=lambda s: s["off"])["line"] if own else None


def geometry_breaks(base: dict, target: dict, aligned, framed_rel=None) -> list[dict]:
    """Where retail's line distance from the opening brace departs from ours.

    Takes the paired statements (both frame braces included), one per line of
    ours (the lowest offset when a line carries several records, e.g. a shared
    epilogue `}`), in OUR source order, and reports each point where
    `retail_rel - our_rel` changes: `lines` more (positive) or fewer (negative)
    source lines sit above that statement in retail. `framed_rel` maps our
    relative line into retail's frame through the known gaps of
    config/geometry_ignores.tsv (None: the line sits in a gap)."""
    t_anchor, b_anchor = _anchor(target), _anchor(base)
    if t_anchor is None or b_anchor is None:
        return []
    pairs = []
    if len(base["statements"]) >= 2 and len(target["statements"]) >= 2:
        pairs.append((base["statements"][0], target["statements"][0]))
    pairs += [(b, t) for kind, b, t in aligned if kind in ("equal", "changed")]
    if len(base["statements"]) >= 3 and len(target["statements"]) >= 3:
        pairs.append((base["statements"][-1], target["statements"][-1]))
    by_line = {}
    for b, t in pairs:
        if (b.get("file", base["file"]) != base["file"]
                or t.get("file", target["file"]) != target["file"]):
            continue
        ours = b["line"] - b_anchor
        framed = framed_rel(ours) if framed_rel is not None else ours
        if framed is None:
            continue
        keep = by_line.get(ours)
        if keep is None or b["off"] < keep[0]["off"]:
            by_line[ours] = (b, t, framed)
    out, previous = [], 0
    for ours in sorted(by_line):
        b, t, framed = by_line[ours]
        gap = (t["line"] - t_anchor) - framed
        if gap != previous:
            out.append({"lines": gap - previous, "base": b, "target": t,
                        "retail_rel": t["line"] - t_anchor, "our_rel": ours})
            previous = gap
    return out
