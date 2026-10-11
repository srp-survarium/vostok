# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.evidence.render - the verdict line, then only what needs action.

Everything that matches is left out: matched statements, aligned blocks,
calls both sides make, shifted-only offsets. `--full` restores the detail
(offset-only rows, every divergent geometry line, every differing block, the
folded call names, callers, the untruncated note).
"""

from __future__ import annotations

import textwrap

from vostok.evidence.gather import text

WIDTH = 72        # source text in the default view
LIST = 6          # rows per list in the default view


def _signed(n: int, unit: str = "B") -> str:
    return f"{n:+d}{unit}"


def _pct(value) -> str:
    return "-" if value is None else f"{value:.2f}"


def _score(report) -> str:
    s = report.get("score") or {}
    if not s:
        return "not in the ledger"
    if s["cur"] is None:
        part = f"not scored this build, max {_pct(s['max'])}"
    elif s["exact"] and s["cur"] >= 99.995:
        part = "100%"
    elif s["exact"]:
        part = f"cur {_pct(s['cur'])}, max 100 (held)"
    else:
        part = f"cur {_pct(s['cur'])}"
        if s["held"]:
            part += f", max {_pct(s['max'])} (held)"
    if s["headroom"]:
        part += f", hist {_pct(s['hist'])} (headroom)"
    if s.get("status") in ("parked", "blocked"):
        part += f" [{s['status']}]"
    return part


def _structure_part(st) -> str:
    c, v = st["counts"], st["verdict"]
    if v == "NO BODY":
        return "NO BODY STATEMENTS"
    if v == "DIFFERS":
        only = [f"{c[t]} {t}" for t in ("TRGT_ONLY", "BASE_ONLY") if c[t]]
        size = f" (+{c['SIZE']} SIZE)" if c["SIZE"] else ""
        return "STRUCTURE DIFFERS: " + ", ".join(only) + size
    part = "STRUCTURE MATCH"
    if c["SIZE"]:
        part += f", {c['SIZE']} SIZE residual{'s' if c['SIZE'] != 1 else ''}"
    for name, label in (("open", "prologue"), ("close", "epilogue")):
        if st["frame"].get(name):
            part += f", {label} {_signed(st['frame'][name])}"
    if v == "BODY MATCH" and not st["frame"]:
        part += f", total size {_signed(st['total_delta'])}"
    return part


def verdict(report) -> str:
    if report.get("error"):
        return report["error"]
    if report["target"] is None:
        return "BASE_ONLY: ours only, retail has no such function"
    if report["base"] is None:
        return f"TARGET_ONLY: nothing compiled yet; {_score(report)}"
    parts = []
    st, geo, flow = report.get("structure"), report.get("geometry"), report.get("flow")
    if st:
        parts.append(_structure_part(st))
    if geo and not geo["match"]:
        n = len(geo["breaks"])
        parts.append(f"geometry off ({n} break{'s' if n != 1 else ''})" if n
                     else f"geometry off ({geo['divergent']} lines)")
    if flow:
        if flow["order_only"]:
            parts.append("flow ORDER-ONLY")
        elif not flow["same"]:
            parts.append("flow DIFFERS")
    head = ", ".join(parts)
    if report.get("rc") == 0 and (report.get("score") or {}).get("exact"):
        checked = [name for name in ("structure", "geometry", "flow") if report.get(name)]
        return "MATCH 100%: " + (", ".join(checked) + " identical" if checked else "ledger max 100")
    return f"{head}; {_score(report)}" if head else _score(report)


def _structure(st, full) -> list[str]:
    diverging = [r for r in st["rows"] if r["tags"]]
    rows = st["rows"] if full else [r for r in diverging if r["tags"] != ["OFFSET"]]
    shifted = len(diverging) - len(rows)
    if not rows and not st["frame"] and not full:
        return []
    out = [f"statements  retail {st['statements']['target']} / ours {st['statements']['base']}"
           + (f"; total {_signed(st['total_delta'])}" if st["total_delta"] else "")]
    for name, label in (("open", "{ prologue"), ("close", "} epilogue")):
        if st["frame"].get(name):
            out.append(f"  {'FRAME':9s} {_signed(st['frame'][name]):>5s}  {label}")
    width = None if full else WIDTH
    for r in rows:
        if not r["tags"]:
            out.append(f"  {'=':9s} {r['our_size']:>4d}B  L{r['line']}  {text(r['source'], width)}")
            continue
        tag = r["tags"][0]
        if tag == "TRGT_ONLY":
            where = (f"after L{r['after']['line']} {text(r['after']['source'], 40)}"
                     if r["after"] else "at the top")
            out.append(f"  {tag:9s} {r['retail_size']:>4d}B  retail +{r['retail_rel']} "
                       f"has a statement ours lacks, {where}")
            continue
        if tag == "BASE_ONLY":
            out.append(f"  {tag:9s} {r['our_size']:>4d}B  L{r['line']}  "
                       f"{text(r['source'], width)}")
            continue
        label = "SIZE" if "SIZE" in r["tags"] else "OFFSET"
        amount = r["delta"] if label == "SIZE" else r["shift"]
        detail = (f"   (ours {r['our_size']}, retail {r['retail_size']})"
                  if label == "SIZE" else "")
        out.append(f"  {label:9s} {_signed(amount):>5s}  L{r['line']}  "
                   f"{text(r['source'], width)}{detail}")
    if shifted and not full:
        out.append(f"  ({shifted} later statement{'s' if shifted != 1 else ''} only shifted "
                   "by the deltas above; --full lists them)")
    return out


def _geometry(geo, full) -> list[str]:
    if geo["match"]:
        return []
    out = []
    if geo["breaks"]:
        out.append("geometry  lines retail has more (+) or fewer (-) before the statement"
                   + (f"; {geo['gaps']} known gap(s) applied" if geo["gaps"] else ""))
        shown = geo["breaks"] if full else geo["breaks"][:LIST]
        for b in shown:
            out.append(f"  {b['lines']:+3d}  L{b['line']}  "
                       f"{text(b['source'], None if full else WIDTH)}")
        if len(geo["breaks"]) > len(shown):
            out.append(f"  ... {len(geo['breaks']) - len(shown)} more (--full)")
    if full or not geo["breaks"]:
        out.append(f"geometry  {geo['divergent']} divergent line(s) of {geo['records']} records")
        shown = geo["lines"] if full else geo["lines"][:LIST]
        for line in shown:
            sizes = "/".join("+".join(f"{s}B" for s in side) or "." for side in
                             (line["retail"], line["ours"]))
            where = f"L{line['line']}" if line["line"] else "-"
            out.append(f"  {line['rel']:+4d} {line['flag']:12s} {sizes:14s} {where}  "
                       f"{text(line['source'], None if full else WIDTH)}")
        if len(geo["lines"]) > len(shown):
            out.append(f"  ... {len(geo['lines']) - len(shown)} more (--full)")
    return out


def _flow(flow, full) -> list[str]:
    real = [d for d in flow["differing"] if full or not d["calls_only"]]
    if flow["same"] and not real:
        return []
    out = []
    n_t = flow["blocks"]["retail"]
    if flow["starved"]:
        out.append("flow      ! a switch/tail jmp left most blocks unreachable; "
                   "the verdict covers only the kept prefix")
    if flow["order_only"]:
        moved = ", ".join(f"ours B{k}->retail B{j}" for k, j in flow["moved"][:LIST])
        out.append(f"flow      ORDER-ONLY: same blocks and edges, block layout differs ({moved})")
    elif not flow["same"]:
        sk = flow["skeleton"]
        if sk:
            out.append(f"flow      DIFFERS at B{sk['block']}: ours [{sk['base']}] vs retail "
                       f"[{sk['target']}]" + (f"  ; {text(sk['source'], 50)}" if sk["source"] else ""))
        elif flow["extra"]:
            side, n, shared = flow["extra"]
            side = "ours" if side == "base" else "retail"
            out.append(f"flow      DIFFERS: same skeleton for {shared} blocks, {side} has "
                       f"{n} extra block(s)")
        else:
            out.append("flow      DIFFERS: same branch kinds, block targets shift "
                       "(an inserted or moved block)")
    if not (flow["same"] or full):
        return out
    # A block whose length differs gained or lost instructions (an inlined
    # call, a temporary); one of the same length differs only in operands.
    grown = [d for d in real if d["ours"] is None or d["retail"] is None
             or d["our_insns"] != d["retail_insns"]]
    operands = [d for d in real if d not in grown]
    if flow["same"]:
        out.append(f"flow      SAME; {len(grown)} of {n_t} blocks change length" if grown else
                   f"flow      SAME; {len(operands)} block(s) differ only in operands "
                   "(registers, offsets, constants)" if operands else
                   "flow      SAME; blocks differ only in callee names (ICF folds)")
    shown = real if full else grown[:LIST]
    for d in shown:
        if d["ours"] is None or d["retail"] is None:
            side = "retail" if d["ours"] is None else "ours"
            index = d["retail"] if d["ours"] is None else d["ours"]
            what = f"{side}-only block B{index}"
        elif d["calls_only"]:
            what = f"B{d['retail']:<3d} callee names only (ICF fold?)"
        elif d["our_insns"] != d["retail_insns"]:
            what = f"B{d['retail']:<3d} {d['our_insns']} insns vs retail {d['retail_insns']}"
        else:
            what = f"B{d['retail']:<3d} operands differ"
        out.append(f"  {what}" + (f"  ; {text(d['source'], WIDTH)}" if d["source"] else ""))
        if full:
            out += [f"        {line}" for line in d.get("diff", [])]
    if full and shown:
        out.append("  (block diffs: - ours, + retail)")
    if not full:
        if len(grown) > len(shown):
            out.append(f"  ... {len(grown) - len(shown)} more (--full)")
        if operands and grown:
            out.append(f"  {len(operands)} more block(s) differ only in operands "
                       "(registers, offsets, constants)")
        hidden = len(flow["differing"]) - len(real)
        if hidden and real:
            out.append(f"  {hidden} block(s) differ only in callee names (ICF folds)")
    return out


def _calls(calls, callers, full) -> list[str]:
    out = []
    for name, n in calls["missing"]:
        out.append(f"  retail calls {name}{f' x{n}' if n > 1 else ''} - ours does not "
                   "(we inline or drop it)")
    for name, n in calls["extra"]:
        out.append(f"  ours calls   {name}{f' x{n}' if n > 1 else ''} - retail does not "
                   "(retail inlined it)")
    if full:
        for retail, ours in calls["renamed"]:
            out.append(f"  renamed      retail {retail}  /  ours {ours}")
        if callers is not None:
            more = len(callers) > 40
            out.append(f"  retail callers: {len(callers[:40])}{'+' if more else ''}")
            for c in callers[:40]:
                folded = f" (+{c['aliases'] - 1} ICF)" if c["aliases"] > 1 else ""
                out.append(f"    rva={c['rva']:#x}  {text(c['name'], 110)}{folded}")
    if not out:
        return []
    return [f"calls     retail {calls['retail']} / ours {calls['ours']}"] + out


def _ledger(led, full) -> list[str]:
    if not led:
        return []
    if not full and not led.get("note") and not led.get("tries") \
            and led.get("status") not in ("parked", "blocked"):
        return []
    head = f"ledger    {led['status']}"
    if led.get("tries"):
        head += f", tried {led['tries']}x"
    if full:
        head += (f", cls {led['cls'] or '-'}, cur {_pct(led['cur'])} max {_pct(led['max'])} "
                 f"hist {_pct(led['hist'])}, {led['module']}")
    out = [head]
    note = led.get("note") or ""
    if note:
        lines = textwrap.wrap(note, 96)
        if not full and len(lines) > 3:
            lines = lines[:3]
            lines[-1] = lines[-1][:92] + " ..."
        out += [f"  {line}" for line in lines]
    return out


def render(report, full: bool = False) -> str:
    out = [verdict(report)]
    if report.get("candidates") is not None:
        for c in report["candidates"]:
            out.append(f"  {c['side']:6s} rva={c['rva']:#x}  {c['mangled']}")
            out.append(f"         {c['name']}")
        if report.get("more"):
            out.append(f"  ... and {report['more']} more")
        return "\n".join(out) + "\n"
    t, b = report["target"], report["base"]
    rec = t or b
    if rec:
        out.append(rec["name"])
        where = [f"  {rec['file']}"]
        if t:
            where.append(f"retail rva={t['rva']:#x} {t['size']}B")
        if b:
            where.append(f"ours rva={b['rva']:#x} {b['size']}B")
        if t and t.get("aliases", 1) > 1:
            where.append(f"({t['aliases']} ICF aliases)")
        out.append("  ".join(where))
    stale = report.get("stale")
    if stale:
        out.append(f"! evidence predates the source: {stale['file']}:{stale['line']} was "
                   f"`{text(stale['recorded'], 40)}`, now `{text(stale['now'], 40)}` - "
                   "rebuild before trusting line numbers")
    views = {"structure": _structure, "geometry": _geometry, "flow": _flow,
             "calls": lambda calls, full: _calls(calls, report.get("callers"), full),
             "ledger": _ledger}
    for name, view in views.items():
        lines = view(report[name], full) if report.get(name) is not None else []
        if lines:
            out.append("")
            out += lines
    if full and report.get("seconds"):
        out.append("")
        out.append("seconds   " + "  ".join(f"{k} {v:.2f}" for k, v in report["seconds"].items()))
    return "\n".join(out) + "\n"
