"""vostok.derive.diff - what moved between two committed revisions.

Reads a match.db out of git at each end of a spec and diffs the two function
states: improved, regressed, appeared, vanished. Older schemas are tolerated
(missing history/attempts tables degrade to NULL) so a spec can reach back
across schema changes.
"""

import json
import os
import sqlite3
import sys

from vostok.core.paths import MATCH_DB as DB_PATH
from vostok.core.paths import REPO as VOSTOK
from vostok.derive.names import fn_from_mangled, shorten_fn
from vostok.derive.query import emit


def _db_blob_at(rev):
    """(path, cleanup_path) for a readable match.db at git `rev`. Empty rev = the
    on-disk current DB (cleanup None). Else extract the committed blob to a tempfile."""
    if not rev:
        return str(DB_PATH), None
    import subprocess
    import tempfile

    rel = "docs/binary_matching/match.db"
    fd, tmp = tempfile.mkstemp(prefix="match_db_", suffix=".db")
    os.close(fd)
    with open(tmp, "wb") as fh:
        r = subprocess.run(
            ["git", "-C", str(VOSTOK), "show", f"{rev}:{rel}"],
            stdout=fh, stderr=subprocess.PIPE,
        )
    if r.returncode != 0:
        os.unlink(tmp)
        sys.exit(
            f"[match_db] cannot read {rel} at '{rev}': "
            f"{r.stderr.decode(errors='replace').strip()}\n"
            "[match_db] match.db is no longer committed (binaries/match.db is a\n"
            "[match_db] gitignored cache), so this command only works against a\n"
            "[match_db] revision old enough to still carry the blob. For the\n"
            "[match_db] current campaign use the committed text ledger instead:\n"
            "[match_db]   git diff <rev> -- docs/binary_matching/match_state.tsv"
        )
    return tmp, tmp


def _fn_state(db_path, module):
    """{mangled: row(dem, pct, cls, loc, best, tries)} over a match.db's target fns.
    Tolerates older schemas that lack the history/attempts tables (best=NULL,tries=0)."""
    con = sqlite3.connect(f"file:{db_path}?mode=ro", uri=True)
    con.row_factory = sqlite3.Row
    have = {r[0] for r in con.execute(
        "SELECT name FROM sqlite_master WHERE type='table'")}
    best = "h.best_fuzzy_pct" if "history" in have else "NULL"
    tries = "coalesce(a.n, 0)" if "attempts" in have else "0"
    hist = "LEFT JOIN history h ON h.mangled = s.mangled" if "history" in have else ""
    att = "LEFT JOIN attempts a ON a.mangled = s.mangled" if "attempts" in have else ""
    where = "WHERE t.module = ?" if module else ""
    rows = con.execute(
        f"""SELECT s.mangled AS m, s.demangled AS dem, p.fuzzy_pct AS pct,
                   p.struct_class AS cls, {best} AS best, {tries} AS tries,
                   coalesce(fl.path, un.name, '(no unit)') AS loc
            FROM symbols s
            JOIN target_functions t ON t.sym = s.id
            LEFT JOIN units un ON un.id = t.unit
            LEFT JOIN files fl ON fl.id = t.file
            LEFT JOIN pairs p ON p.sym = s.id
            {hist}
            {att}
            {where}""",
        ([module] if module else []),
    ).fetchall()
    con.close()
    return {r["m"]: r for r in rows}


def cmd_diff(args):
    """Function-level diff between two committed match.db revisions (or a rev vs the
    working tree): what each target function's fuzzy_pct / struct_class did."""
    spec = args.spec
    a_rev, b_rev = spec.split("..", 1) if ".." in spec else (spec, "")
    if not a_rev:
        sys.exit("[match_db] diff needs a base rev: <hash> or <hash>..<hash>")
    a_path, a_tmp = _db_blob_at(a_rev)
    b_path, b_tmp = _db_blob_at(b_rev)
    try:
        a_state = _fn_state(a_path, args.module)
        b_state = _fn_state(b_path, args.module)
    finally:
        for t in (a_tmp, b_tmp):
            if t:
                os.unlink(t)

    eps = 0.01
    # ORDER drives both the printed summary and row grouping.
    ORDER = ["REGRESS", "LOST", "NEW", "IMPROVE", "TOUCHED", "RECLASS"]
    cats = {k: [] for k in ORDER}
    for m in set(a_state) | set(b_state):
        a, b = a_state.get(m), b_state.get(m)
        ap, bp = (a["pct"] if a else None), (b["pct"] if b else None)
        ac, bc = (a["cls"] if a else None), (b["cls"] if b else None)
        at, bt = (a["tries"] if a else 0), (b["tries"] if b else 0)
        amx, bmx = (a["best"] if a else None), (b["best"] if b else None)
        rec = {"m": m, "dem": (b or a)["dem"], "loc": (b or a)["loc"],
               "ap": ap, "bp": bp, "ac": ac, "bc": bc, "amx": amx, "bmx": bmx,
               "at": at, "bt": bt}
        if ap is None and bp is None:
            kind, srt = ("TOUCHED", bt - at) if bt > at else (None, 0)
        elif ap is None:
            kind, srt = "NEW", bp
        elif bp is None:
            kind, srt = "LOST", ap
        elif bp - ap > eps:
            kind, srt = "IMPROVE", bp - ap
        elif ap - bp > eps:
            kind, srt = "REGRESS", bp - ap
        elif ac != bc:
            kind, srt = "RECLASS", ap or 0
        elif bt > at:                       # worked but % + structure unchanged
            kind, srt = "TOUCHED", bt - at
        else:
            kind = None
        if kind:
            rec["srt"] = srt
            cats[kind].append(rec)
    for k in ORDER:                          # REGRESS most-negative first; rest desc
        cats[k].sort(key=lambda r: r["srt"], reverse=(k != "REGRESS"))

    b_label = b_rev or "WORKTREE"
    if args.json:
        def js(r):
            return {"from": r["ap"], "to": r["bp"],
                    "max_from": r["amx"], "max_to": r["bmx"],
                    "from_cls": r["ac"], "to_cls": r["bc"],
                    "tries_from": r["at"], "tries_to": r["bt"],
                    "fn": r["dem"], "file": r["loc"]}
        print(json.dumps({"a": a_rev, "b": b_label, "module": args.module,
                          **{k.lower(): [js(r) for r in cats[k]] for k in ORDER}},
                         indent=1))
        return

    def strip(loc):
        return loc[7:] if loc.startswith("vostok/") else loc

    def clsfmt(ac, bc):
        return (ac or "-") if ac == bc else f"{ac or ''}->{bc or ''}"

    def pf(x):
        return f"{x:.1f}" if x is not None else "-"

    print(f"[match_db] diff {a_rev} -> {b_label}"
          + (f"  (module {args.module})" if args.module else ""))
    print("  " + "  ".join(f"{k.lower()} {len(cats[k])}" for k in ORDER))

    DTAG = {"LOST": "gone", "NEW": "new", "TOUCHED": "~tries", "RECLASS": "~cls"}
    rows = []
    for k in ORDER:
        for r in cats[k]:
            d = (f"{r['bp'] - r['ap']:+.2f}" if k in ("REGRESS", "IMPROVE")
                 else DTAG[k])
            tries = (f"{r['at']}->{r['bt']}" if r["bt"] != r["at"] else str(r["bt"]))
            # max DROP (amx > bmx) = the fn was RE-WORKED (best reset to current); show
            # it as from->to so a re-match is unmistakable vs a non-steerable LTO drop.
            mx = (f"{pf(r['amx'])}->{pf(r['bmx'])}"
                  if r["amx"] is not None and r["bmx"] is not None and r["amx"] > r["bmx"] + 0.01
                  else pf(r["bmx"] if r["bmx"] is not None else r["amx"]))
            rows.append({
                "kind": k, "d": d, "from": pf(r["ap"]), "to": pf(r["bp"]),
                "max": mx, "cls": clsfmt(r["ac"], r["bc"]), "tries": tries,
                "file": strip(r["loc"]),
                "fn": (shorten_fn(r["dem"]) if args.verbose
                       else fn_from_mangled(r["m"], r["dem"])),
            })
    if not rows:
        print("  (no function-level differences)")
    else:
        emit(rows, False)
