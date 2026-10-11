# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.evidence.gather - every section's facts for one record pair, as data.

Each section reuses the module that owns the question: the ledger row from
`ledger.store`, the statement partition from `evidence.structure` (the
structure-diff port), the relative-line overlay from `tool.geometry`, the
control-flow verdict from `sema.cfg` / `sema.blocks`, and the call sites from
`sema.xref`. Nothing here prints; `evidence.render` decides what is worth a line.
"""

from __future__ import annotations

import difflib
import re
import time
from collections import Counter

from vostok.core.paths import SOURCES
from vostok.evidence import structure
from vostok.ledger.store import EXACT
from vostok.sema.blocks import flow_verdict
from vostok.sema.cfg import cfg, starved
from vostok.sema.disasm import listing, parse
from vostok.sema.xref import _callee_rows, _qualified_name
from vostok.tool import geometry

SECTIONS = ("ledger", "structure", "geometry", "flow", "calls")
ALIASES = {"blocks": "flow", "sema": "flow", "xref": "calls", "xrefs": "calls",
           "callees": "calls", "geo": "geometry", "statements": "structure"}


def text(source: str | None, width: int | None = None) -> str:
    """One statement's source on one line: whitespace runs collapsed."""
    flat = re.sub(r"\s+", " ", source or "").strip()
    if width and len(flat) > width:
        return flat[: width - 3] + "..."
    return flat


def ledger(row: dict | None) -> dict | None:
    if row is None:
        return None
    keep = ("status", "cls", "cur", "max", "hist", "tries", "module", "unit", "note", "flags")
    return {key: row.get(key) for key in keep}


def stale_source(base: dict) -> dict | None:
    """The first of our statements whose recorded source text no longer sits on
    its recorded line: the evidence predates a source edit, so its line numbers
    (and every geometry reading) are about a file that no longer exists."""
    lines_by_file = {}
    for s in base["statements"]:
        source = s.get("source")
        if source is None or source in ("{", "}"):
            continue
        file = s.get("file", base["file"])
        if file not in lines_by_file:
            path = SOURCES / file
            lines_by_file[file] = (path.read_text(errors="replace").splitlines()
                                   if path.is_file() else None)
        lines = lines_by_file[file]
        if lines is None:
            continue
        now = lines[s["line"] - 1] if 0 < s["line"] <= len(lines) else ""
        if now.rstrip() != source.rstrip():
            return {"file": file, "line": s["line"], "recorded": source, "now": now}
    return None


def fill_sources(base: dict) -> None:
    """Give each of our statements without recorded text (vostok-pdb leaves
    some lines unread) the text on its line now. Called only when the source
    is not stale, so the line still holds the statement."""
    files = {}
    for s in base["statements"]:
        if s.get("source") is not None:
            continue
        file = s.get("file", base["file"])
        if file not in files:
            path = SOURCES / file
            files[file] = (path.read_text(errors="replace").splitlines()
                           if path.is_file() else [])
        if 0 < s["line"] <= len(files[file]):
            s["source"] = files[file][s["line"] - 1]


def structure_section(base: dict, target: dict) -> dict:
    """The verdict plus one row per aligned statement; a matched statement has
    no tags (the default view drops it, --full and --json keep it)."""
    result = structure.compare(base, target)
    t_first = next((r["line"] for r in structure.structure_rows(target["statements"])
                    if r is not None), 0)
    found = {(id(r["base"]), id(r["target"])): r for r in result["rows"]}
    rows, after = [], None
    for kind, b, t in result["aligned"]:
        if kind.startswith("empty"):
            continue
        hit = found.get((id(b), id(t)), {"tags": [], "delta": 0, "shift": 0})
        rows.append({
            "tags": hit["tags"], "delta": hit["delta"], "shift": hit["shift"],
            "line": b["line"] if b is not None else None,
            "file": (b or t).get("file"),
            "retail_rel": t["line"] - t_first if t is not None else None,
            "retail_size": t["size"] if t is not None else None,
            "our_size": b["size"] if b is not None else None,
            "source": text(b.get("source")) if b is not None else "",
            "after": ({"line": after["line"], "source": text(after.get("source"))}
                      if b is None and after is not None else None),
        })
        if b is not None:
            after = b
    return {key: result[key] for key in ("verdict", "counts", "statements", "total_delta",
                                         "frame")} | {"rows": rows,
                                                      "_aligned": result["aligned"]}


def geometry_section(base: dict, target: dict, aligned) -> dict:
    view = geometry.overlay(target, base)
    frame_of = {ours: frame for frame, ours in view["ours_at"].items()}
    breaks = structure.geometry_breaks(base, target, aligned, frame_of.get)
    lines = []
    statements = {(s["line"] - view["anchor"]): s for s in base["statements"]
                  if s.get("file", base["file"]) == base["file"]}
    for rel, flag in view["divergent"]:
        row = next((r for r in view["rows"] if r[0] == rel), None)
        ours = view["ours_at"].get(rel)
        stmt = statements.get(ours) if ours is not None else None
        lines.append({"rel": rel, "flag": flag,
                      "retail": row[1] if row else [], "ours": row[2] if row else [],
                      "line": geometry.our_line(view, rel),
                      "source": text(stmt.get("source")) if stmt else ""})
    return {"match": not view["divergent"], "records": view["records"],
            "divergent": len(view["divergent"]), "gaps": len(view["gaps"]),
            "byte_lines": view["bytes"],
            "breaks": [{"lines": b["lines"], "line": b["base"]["line"],
                        "retail_rel": b["retail_rel"], "our_rel": b["our_rel"],
                        "source": text(b["base"].get("source"))} for b in breaks],
            "lines": lines,
            "other_files": view["other"]}


_CALL = re.compile(r"^(call\s+)(\S.*)$")


def _graph(rec):
    insns, labels, stmts = parse(listing(rec))
    stats = {}
    graph = cfg(insns, labels, stmts, stats)
    stats["kept"] = len(graph)
    return graph, stats


def _masked(block):
    return [_CALL.sub(r"\1<fn>", insn) for insn in block[1]] + [block[2] or ""]


def flow_section(base: dict, target: dict) -> dict:
    b, b_stats = _graph(base)
    t, t_stats = _graph(target)
    verdict = flow_verdict(b, t)
    blocks = []
    for bi, tj in verdict["differing"]:
        entry = {"ours": bi, "retail": tj,
                 "our_insns": len(b[bi][1]) if bi is not None else None,
                 "retail_insns": len(t[tj][1]) if tj is not None else None,
                 "source": text((b[bi][3] if bi is not None else "")
                                or (t[tj][3] if tj is not None else "")),
                 "calls_only": False}
        if bi is not None and tj is not None:
            entry["calls_only"] = _masked(b[bi]) == _masked(t[tj])
            entry["kind_same"] = (b[bi][2] or "") == (t[tj][2] or "")
            entry["terms"] = {"ours": b[bi][2] or "", "retail": t[tj][2] or ""}
            entry["diff"] = [line for line in difflib.unified_diff(
                b[bi][1] + [b[bi][2] or ""], t[tj][1] + [t[tj][2] or ""], lineterm="", n=0)
                if not line.startswith(("---", "+++", "@@"))]
        blocks.append(entry)
    skeleton = verdict["skeleton"]
    return {"same": verdict["same_flow"],
            "order_only": verdict["iso"] is not None,
            "moved": verdict["moved"],
            "skeleton": (dict(skeleton, source=text(skeleton["source"])) if skeleton else None),
            "extra": verdict["extra"],
            "blocks": {"ours": len(b), "retail": len(t)},
            "differing": blocks,
            "starved": starved({"base": b_stats, "target": t_stats}),
            "contracted": {"ours": b_stats.get("contracted", 0),
                           "retail": t_stats.get("contracted", 0)}}


_REGISTER = re.compile(r"^(e?[abcd]x|e[sd]i|ebp|esp|(d?word|byte) ptr .*|\[.*\])$", re.I)


def _callees(rec):
    return ["<indirect>" if _REGISTER.match(operand) else operand
            for _, operand in _callee_rows(rec)]


def calls_section(base: dict, target: dict) -> dict:
    """Callee sequences compared in order. A call one side makes and the other
    does not is the inline-vs-call signal; a call pair whose names differ at the
    same position is usually one body folded under two names (ICF)."""
    ours, retail = _callees(base), _callees(target)
    missing, extra, renamed = Counter(), Counter(), []
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(a=ours, b=retail,
                                                      autojunk=False).get_opcodes():
        if op == "equal":
            continue
        paired = min(i2 - i1, j2 - j1) if op == "replace" else 0
        renamed += [(retail[j1 + k], ours[i1 + k]) for k in range(paired)]
        extra.update(ours[i1 + paired:i2])
        missing.update(retail[j1 + paired:j2])
    return {"ours": len(ours), "retail": len(retail),
            "missing": sorted(missing.items()), "extra": sorted(extra.items()),
            "renamed": renamed}


def callers(connection, rec: dict, limit: int = 40) -> list[dict]:
    """Direct retail callers, one per RVA (ICF aliases counted) - a scan of
    every instruction payload, so it runs only under --full."""
    needles = sorted({f'"call  {rec["mangled"]}"', f'"call  {_qualified_name(rec["name"])}"'})
    where = " OR ".join("instr(p.instructions_json,?)>0" for _ in needles)
    by_rva = {}
    for row in connection.execute(
            "SELECT f.name,f.rva,f.file FROM functions f JOIN function_payloads p "
            f"ON p.function_id=f.id WHERE {where} ORDER BY f.file,f.rva,f.id", needles):
        hit = by_rva.setdefault(row["rva"], dict(row, aliases=0))
        hit["aliases"] += 1
    return sorted(by_rva.values(), key=lambda r: (r["file"], r["rva"]))[:limit + 1]


def score(row: dict | None) -> dict:
    """cur / max / hist and the two readings of them a matcher acts on."""
    if not row:
        return {}
    cur, mx, hist = row.get("cur"), row.get("max"), row.get("hist")
    return {"cur": cur, "max": mx, "hist": hist, "status": row.get("status"),
            "exact": mx is not None and mx >= EXACT,
            "held": cur is not None and mx is not None and cur < mx - 0.01,
            "headroom": hist is not None and mx is not None and hist > mx + 0.01}


def gather(target: dict | None, base: dict | None, row: dict | None, sections,
           target_con=None, full: bool = False) -> dict:
    out = {"sections": list(sections), "seconds": {}}

    def timed(name, fn):
        started = time.monotonic()
        out[name] = fn()
        out["seconds"][name] = round(time.monotonic() - started, 3)

    if "ledger" in sections:
        timed("ledger", lambda: ledger(row))
    if target and base:
        stale = stale_source(base)
        if stale:
            out["stale"] = stale
        else:
            fill_sources(base)
        if "structure" in sections or "geometry" in sections:
            timed("structure", lambda: structure_section(base, target))
            aligned = out["structure"].pop("_aligned")
            if "geometry" in sections:
                timed("geometry", lambda: geometry_section(base, target, aligned))
            if "structure" not in sections:
                del out["structure"]
        if "flow" in sections:
            timed("flow", lambda: flow_section(base, target))
        if "calls" in sections:
            timed("calls", lambda: calls_section(base, target))
    if "calls" in sections and full and target and target_con is not None:
        timed("callers", lambda: callers(target_con, target))
    return out
