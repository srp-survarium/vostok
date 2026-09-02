#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Roll up the committed ledger into README.md's human-readable score block.

Reads ``config/match_state.tsv`` - the committed record - for
every number. ``vostok build`` re-derives that ledger from ``report.json``
immediately before this runs, so the block and the ledger always describe the
same build, and there is exactly one roster behind every figure. report.json is
consulted for one thing only, the NAMES of the units it compared (the `Units`
column): its function roster counts every COMDAT objdiff sees (~25,372) rather
than the ledger's target-function roster (~12,932), and mixing the two is how a
wrong number once reached the README. When either input cannot answer, this
RAISES (``ScoreDataUnavailable``) instead of substituting; ``vostok build``
warns and leaves the previous block alone.

Per module the block shows, over every target function (paired plus
inlined/folded target-only, which count as 0%-matched weight):
  - functions exact / exact-max : byte-perfect now (``cur``), and byte-perfect
    proven for the source body checked out now (``max``, scoped to ``hash``).
  - fuzzy / fuzzy-max          : the same two rulers, byte-weighted.

``--max-code`` is the same data on the byte ruler instead of the function ruler.

Usage:
    python3 -m vostok ledger readme                   # print to stdout
    python3 -m vostok ledger readme --write-readme    # refresh README.md block
    python3 -m vostok ledger readme --max-code        # code matched vs max
    python3 -m vostok ledger readme --max-code --module game_core
"""

from __future__ import annotations

import argparse
import datetime
import json

from vostok.core.paths import FLAKE_LOCK, MATCH_STATE, README, REPORT
from vostok.derive.modules import module_of
from vostok.ledger import store
from vostok.core.fmt import pct as _pct

START = "<!-- match-score:start -->"
END = "<!-- match-score:end -->"




def _md_table(headers: list[str], aligns: str, rows: list[list[str]]) -> list[str]:
    """Render a GitHub markdown table with pipes aligned to column widths, so the
    raw source reads cleanly in an editor (GitHub renders it identically). `aligns`
    is one char per column: 'l' left, 'r' right."""
    widths = [len(h) for h in headers]
    for r in rows:
        for i, c in enumerate(r):
            widths[i] = max(widths[i], len(c))

    def cell(text: str, i: int) -> str:
        return text.rjust(widths[i]) if aligns[i] == "r" else text.ljust(widths[i])

    def row(cells: list[str]) -> str:
        return "| " + " | ".join(cell(c, i) for i, c in enumerate(cells)) + " |"

    sep = []
    for w, a in zip(widths, aligns):
        sep.append("-" * (w - 1) + ":" if a == "r" else ":" + "-" * (w - 1))
    return [row(headers), "| " + " | ".join(sep) + " |", *(row(r) for r in rows)]


# Third-party libraries: real linked target code, but not engine modules we match -
# excluded so the README stays an engine match tracker (sushi). NOT excluded: the
# `vostok` module is the shared engine library (containers/strings/configs/math,
# report.json's "shared"), heavily matched - it stays. An exclude-set, so a NEW
# engine module auto-appears.
_NON_ENGINE = frozenset({
    "boost", "stlport", "bullet", "vorbis", "ogg", "zlib", "opcode",
    "speedtree", "wildmagic", "fastdelegate", "gfx", "_msvc_internal",
})


class ScoreDataUnavailable(RuntimeError):
    """The score block cannot be rendered from the committed record.

    Deliberately fatal rather than a fallback. report.json counts a different,
    much larger roster (every COMDAT objdiff sees, ~25,372 functions) than the
    ledger's target-function roster (~12,932), so quietly substituting it
    produces a block that is off by ~12k functions while still claiming, in its
    own first line, to come from the ledger. That exact substitution is what once
    shipped "25,372 functions" into README.md. `vostok build` catches this and
    warns, leaving the previous (true) block in place.
    """


def _proven(row: dict) -> float:
    """The peak PROVEN for the source body that is checked out now.

    `max` is scoped to `hash`, the function's own source body, and resets when
    that body changes. A banked peak carrying no hash describes a body we can no
    longer identify - that is history (`hist`), not proof, so the MAX columns do
    not credit it. This is exactly what the retired `source_maxima` table meant
    by "same effective-source epoch": every row in it carried a hash.
    """
    return (row["max"] or 0.0) if row["hash"] else 0.0


def _unit_counts() -> dict[str, int]:
    """TUs per module, from report.json's unit roster.

    Only the unit NAMES are read - never a count of functions, see the module
    docstring. A unit objdiff compared with no functions in it is not a TU of
    the campaign and does not count.
    """
    if not REPORT.is_file():
        raise ScoreDataUnavailable(
            f"no report.json at {REPORT} - the score block's unit counts come "
            "from the units objdiff compared; run `python3 -m vostok build`"
        )
    units: dict[str, int] = {}
    for unit in json.loads(REPORT.read_text())["units"]:
        if unit["functions"]:
            module = module_of(unit["name"])
            units[module] = units.get(module, 0) + 1
    return units


def module_stats() -> dict[str, dict]:
    """Per-module stats computed from the committed ledger: total target
    functions, TU count, current + source-hash-scoped exact-max counts, and
    byte-weighted current + fuzzy-max %. Third-party / catch-all modules
    (`_NON_ENGINE`) are excluded. Returns module -> dict plus an 'OVERALL'
    aggregate. Raises ScoreDataUnavailable when the ledger cannot answer."""
    if not MATCH_STATE.is_file():
        raise ScoreDataUnavailable(
            f"no ledger at {MATCH_STATE} - run `python3 -m vostok build` (or "
            "`python3 -m vostok derive refresh` if the report is already built)"
        )
    # module -> [n_funcs, exact_cur, exact_max, code, fuzzy_cur_num, fuzzy_max_num]
    agg: dict[str, list[float]] = {}

    def acc(mod: str, size: float, cur: float, maximum: float) -> None:
        # A row with no module is banked from a build whose target spelling is
        # gone; it belongs to no module's tally and carries no weight.
        if not mod or mod in _NON_ENGINE:
            return
        a = agg.setdefault(mod, [0, 0, 0, 0.0, 0.0, 0.0])
        a[0] += 1
        a[1] += 1 if cur >= _EXACT else 0
        a[2] += 1 if maximum >= _EXACT else 0
        a[3] += size
        a[4] += cur * size
        a[5] += maximum * size

    # Unpaired (inlined/folded target-only) rows carry cur = None: 0% now, full
    # size as weight, and MAX credit only from a hash-scoped peak.
    for row in store.load(str(MATCH_STATE)).values():
        cur = row["cur"] or 0.0
        acc(row["module"], row["size"] or 0, cur, max(cur, _proven(row)))
    if not agg:
        raise ScoreDataUnavailable(f"{MATCH_STATE} has no function rows")
    units = {m: n for m, n in _unit_counts().items() if m not in _NON_ENGINE}

    out: dict[str, dict] = {}
    tot = [0, 0, 0, 0.0, 0.0, 0.0]
    for mod, a in agg.items():
        out[mod] = {
            "total_funcs": int(a[0]), "units": int(units.get(mod, 0)),
            "exact_cur": int(a[1]), "exact_max": int(a[2]),
            "fuzzy_cur": a[4] / a[3] if a[3] else 0.0,
            "fuzzy_max": a[5] / a[3] if a[3] else 0.0,
        }
        tot = [x + y for x, y in zip(tot, a)]
    out["OVERALL"] = {
        "total_funcs": int(tot[0]), "units": int(sum(units.values())),
        "exact_cur": int(tot[1]), "exact_max": int(tot[2]),
        "fuzzy_cur": tot[4] / tot[3] if tot[3] else 0.0,
        "fuzzy_max": tot[5] / tot[3] if tot[3] else 0.0,
    }
    return out


def render(delinker_rev: str) -> str:
    today = datetime.date.today().isoformat()

    # Every FIGURE in the block comes from the committed ledger (sushi:
    # "calculated through our database entirely, not by using anything from
    # report"). There is NO report.json fallback on purpose - see
    # ScoreDataUnavailable; only the unit NAMES come from it.
    db = module_stats()

    ov = db["OVERALL"]
    funcs, tfuncs = ov["exact_cur"], ov["total_funcs"]
    rows = []
    for mod in sorted((m for m in db if m != "OVERALL"),
                      key=lambda k: -db[k]["total_funcs"]):
        a = db[mod]
        tot = a["total_funcs"]
        exc, exm = a["exact_cur"], a["exact_max"]
        fzc, fzm = a["fuzzy_cur"], a["fuzzy_max"]
        rows.append([
            f"`{mod}`",
            f"{a['units']}",
            f"{exc:,} / {tot:,} ({_pct(exc, tot):.1f}%)",
            f"{exm:,} / {tot:,} ({_pct(exm, tot):.1f}%)",
            f"{fzc:.1f}%",
            f"{fzm:.1f}%",
        ])
    table = _md_table(
        ["Module", "Units", "Functions exact", "Functions exact-max",
         "Fuzzy", "Fuzzy-max"],
        "lrrrrr",
        rows,
    )

    lines = [
        START,
        "## Match status",
        "",
        "_Auto-generated from `config/match_state.tsv` (the committed "
        "matching ledger) - refreshed by `vostok build` at the end of every build; "
        "do not hand-edit. Diff this block across commits to spot regressions._",
        "",
        f"**Overall: {funcs:,} / {tfuncs:,} functions exact "
        f"({_pct(funcs, tfuncs):.2f}%) &middot; "
        f"{ov['exact_max']:,} / {tfuncs:,} functions exact-max "
        f"({_pct(ov['exact_max'], tfuncs):.2f}%) &middot; "
        f"{ov['fuzzy_cur']:.2f}% fuzzy &middot; "
        f"{ov['fuzzy_max']:.2f}% fuzzy-max.**",
        "",
        "_All figures come from the ledger over every target function (paired plus "
        "inlined/folded target-only). **Functions exact** and **Fuzzy** describe the "
        "current build (`cur`). **Exact-max** and **Fuzzy-max** use `max`, the peak "
        "proven for the function's own source body (`hash`), which resets when that "
        "body changes; the all-time `hist` peak is never promoted into it, and a "
        "banked peak carrying no `hash` is not credited. Byte-weighted code view: "
        "`python3 -m vostok ledger readme --max-code`._",
        "",
        *table,
    ]
    lines += [
        "",
        f"_Updated {today} &middot; delinker `{delinker_rev}` "
        f"(folded-symbol reconciliation)._",
        END,
    ]
    return "\n".join(lines)


# a function is byte-exact when objdiff scores it at 100%; allow float slop
_EXACT = store.EXACT


def code_max(module: str | None = None) -> list[tuple]:
    """Per-module code match on BOTH rulers, each current vs source-scoped MAX.

    Code-weighted over ALL target code: each target function contributes its
    target size to the denominator, so a TRGT_ONLY/unpaired function is
    0%-matched dead weight (exactly like objdiff's total_code). Two rulers:
      - fuzzy  : partial credit - a function adds (fuzzy% * size). The "how close"
                 number (~82% for game_core; objdiff's weighted match).
      - exact  : byte-perfect only - a function adds its full size iff fuzzy==100,
                 else 0. The README's "Code matched" (~26%).
    The max columns use the ledger's `max`: it survives compiler-state churn
    within one source body and resets when that body changes.

    Returns rows of (module, total_code, fuzzy_cur, fuzzy_max, exact_cur,
    exact_max) sorted by code size, with a trailing OVERALL row.
    """
    # module -> [total, fuzzy_cur_num, fuzzy_max_num, exact_cur, exact_max]
    agg: dict[str, list[float]] = {}

    def add(mod: str | None, size: int, cur_fz: float, max_fz: float) -> None:
        if not mod:
            return
        a = agg.setdefault(mod, [0.0, 0.0, 0.0, 0.0, 0.0])
        a[0] += size
        a[1] += cur_fz * size
        a[2] += max_fz * size
        a[3] += size if cur_fz >= _EXACT else 0
        a[4] += size if max_fz >= _EXACT else 0

    for row in store.load().values():
        cur = row["cur"] or 0.0
        add(row["module"], row["size"] or 0, cur, max(cur, _proven(row)))

    if module:
        agg = {m: v for m, v in agg.items() if m == module}
    out, tot = [], [0.0, 0.0, 0.0, 0.0, 0.0]
    for mod, a in agg.items():
        t = a[0]
        out.append((mod, int(t), a[1] / t if t else 0, a[2] / t if t else 0,
                    _pct(a[3], t), _pct(a[4], t)))
        tot = [x + y for x, y in zip(tot, a)]
    out.sort(key=lambda r: -r[1])
    t = tot[0]
    out.append(("OVERALL", int(t), tot[1] / t if t else 0, tot[2] / t if t else 0,
                _pct(tot[3], t), _pct(tot[4], t)))
    return out


def render_code_max(rows: list[tuple]) -> str:
    lines = [
        "Code match  (ledger, code-weighted over ALL target code incl. "
        "TRGT_ONLY@0%; max = scoped to the function's own source body)",
        "  fuzzy = partial credit (how close) | exact = byte-perfect "
        "(README 'Code matched')",
        f"{'module':<14}{'code bytes':>12}{'fuzzy':>9}{'fuzzy-max':>11}"
        f"{'exact':>9}{'exact-max':>11}",
    ]
    for mod, tc, fz, fzm, ex, exm in rows:
        lines.append(f"{mod:<14}{tc:>12,}{fz:>8.1f}%{fzm:>10.1f}%"
                     f"{ex:>8.1f}%{exm:>10.1f}%")
    return "\n".join(lines)


def _delinker_rev() -> str:
    try:
        lock = json.loads(FLAKE_LOCK.read_text())
        rev = lock["nodes"]["vostok-delinker-src"]["locked"]["rev"]
        return rev[:7]
    except Exception:
        return "unknown"


def write_readme(block: str) -> None:
    text = README.read_text()
    if START in text and END in text:
        pre = text[: text.index(START)]
        post = text[text.index(END) + len(END):]
        README.write_text(pre + block + post)
    else:
        # First install: drop the block right after the intro paragraph (the
        # blank line before the first "## " heading).
        marker = "\n## "
        i = text.index(marker)
        README.write_text(text[:i] + "\n" + block + "\n" + text[i:])


def regen_readme() -> str:
    """Refresh README.md's score block from the committed ledger and return the
    rendered block (callable with no args). `vostok build` calls this at the end of
    every build, right after the regen that rewrote the ledger it reads, so the
    README regression tracker stays current; `--write-readme` is the same path from
    the CLI. Raises ScoreDataUnavailable rather than writing numbers off a
    different roster."""
    block = render(_delinker_rev())
    write_readme(block)
    # Keep the separate image-data block adjacent when a measured data report
    # exists. A missing shadow-lane report must not block the function block.
    try:
        from vostok.data import readme as data_readme
        data_readme.write_readme(data_readme.render())
    except (OSError, RuntimeError, ValueError):
        pass
    return block


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write-readme", action="store_true",
                    help="refresh the score block in README.md")
    ap.add_argument("--max-code", action="store_true",
                    help="print per-module 'code matched' vs 'max code matched' "
                         "(source-body-scoped, churn-immune) from the ledger")
    ap.add_argument("--module", help="restrict --max-code to one module")
    args = ap.parse_args()

    if args.max_code:
        print(render_code_max(code_max(args.module)))
        return

    if args.write_readme:
        block = regen_readme()
        print(block)
        print(f"\n[match_score] wrote score block to {README}")
    else:
        print(render(_delinker_rev()))


if __name__ == "__main__":
    main()
