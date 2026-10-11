# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.evidence.select - one selector to one retail/ours record pair.

Indexed lookups only: exact mangled name, exact signature, retail/our RVA or
VA, then a substring over the function table (never the payloads). The base
partner is chosen the way `vostok-pdb inspect` chooses it for structure-diff -
same mangled name, else same signature, the same source file preferred - so
both tools compare the same pair. Records sharing an RVA are ICF aliases of one
function, not candidates.
"""

from __future__ import annotations

import json
import re
import sqlite3
from pathlib import Path

from vostok.derive.aliases import dyn_canon_base
from vostok.derive.index import overload_key
from vostok.ledger import store
from vostok.sema.xref import _qualified_name

FIELDS = "f.id,f.name,f.mangled,f.rva,f.image_base,f.size,f.file"
HEX = re.compile(r"0[xX][0-9a-fA-F]+\Z")


class Ambiguous(Exception):
    """A selector naming several functions; `.candidates` lists one record each."""

    def __init__(self, selector, candidates):
        super().__init__(f"'{selector}' names {len(candidates)} functions")
        self.candidates = candidates


def connect(path: Path) -> sqlite3.Connection:
    if not Path(path).is_file():
        raise FileNotFoundError(f"{path} is missing - run `python3 -m vostok build` first")
    connection = sqlite3.connect(f"file:{path}?mode=ro", uri=True)
    connection.row_factory = sqlite3.Row
    return connection


def _rows(connection, where, args):
    return [dict(row) for row in connection.execute(
        f"SELECT {FIELDS} FROM functions f WHERE {where} ORDER BY f.file,f.rva,f.id", args)]


def _fold(rows, selector=None):
    """One record per RVA: the exact mangled spelling first, else the lowest id."""
    by_rva = {}
    for row in rows:
        row.setdefault("aliases", 1)
        keep = by_rva.get(row["rva"])
        if keep is None:
            by_rva[row["rva"]] = row
            continue
        keep["aliases"] += 1
        if keep["mangled"] != selector and row["mangled"] == selector:
            row["aliases"] = keep["aliases"]
            by_rva[row["rva"]] = row
    return list(by_rva.values())


def find(connection, selector: str) -> list[dict]:
    """Every function (one per RVA) the selector can mean on one side."""
    if HEX.match(selector):
        value = int(selector, 16)
        # Indexed: the image base is one constant per database.
        bases = [row[0] for row in connection.execute("SELECT DISTINCT image_base FROM functions "
                                                       "WHERE rva=(SELECT MIN(rva) FROM functions)")]
        rvas = {value, *(value - image_base for image_base in bases)}
        rows = _rows(connection, f"f.rva IN ({','.join('?' * len(rvas))})", tuple(rvas))
        return _fold([row for row in rows
                      if row["rva"] == value or row["rva"] + row["image_base"] == value])
    for where in ("f.mangled=?", "f.name=?"):
        hits = _fold(_rows(connection, where, (selector,)), selector)
        if hits:
            return hits
    if selector.startswith("??__"):
        canon = dyn_canon_base(selector)
        if canon:
            hits = [row for row in _rows(connection, "instr(f.name,'dynamic ')>0 OR "
                                         "f.mangled LIKE '??\\_\\_%' ESCAPE '\\'", ())
                    if dyn_canon_base(row["mangled"]) == canon]
            if hits:
                return _fold(hits)
    hits = _fold(_rows(connection, "instr(lower(f.name),?)>0 OR instr(f.mangled,?)>0",
                       (selector.lower(), selector)), selector)
    if len(hits) > 1 and "(" not in selector:
        # `weapon_core::tick` names the function whose qualified name ends
        # there, not every `weapon_core::tick_*` it is a substring of.
        exact = [row for row in hits if _qualified_name(row["name"]) == selector
                 or _qualified_name(row["name"]).endswith("::" + selector)]
        hits = exact or hits
    return hits


def partner(connection, rec: dict) -> dict | None:
    """The other side's record for `rec`, chosen as vostok-pdb's structure-diff
    chooses it; None when that side has no such function."""
    rows = _rows(connection, "f.mangled=?", (rec["mangled"],)) or \
        _rows(connection, "f.name=?", (rec["name"],))
    if not rows:
        canon = dyn_canon_base(rec["mangled"])
        if canon:
            rows = [row for row in _rows(connection, "instr(f.name,'dynamic ')>0 OR "
                                         "f.mangled LIKE '??\\_\\_%' ESCAPE '\\'", ())
                    if dyn_canon_base(row["mangled"]) == canon]
    rows = [row for row in rows if row["name"] == rec["name"]] or rows
    rows = [row for row in rows if row["file"].lower() == rec["file"].lower()] or rows
    rows = _fold(rows, rec["mangled"])
    return rows[0] if rows else None


def payload(connection, rec: dict) -> dict:
    """`rec` with its statements and instructions loaded (one row read)."""
    row = connection.execute(
        "SELECT statements_json,instructions_json FROM function_payloads WHERE function_id=?",
        (rec["id"],)).fetchone()
    rec["statements"] = json.loads(row["statements_json"]) if row else []
    rec["instructions"] = json.loads(row["instructions_json"]) if row else []
    return rec


def ledger_row(rec: dict) -> dict | None:
    """The ledger row for a retail record: its overload key, its mangled name,
    or - for a dynamic initializer the two PDBs spell differently - the row
    whose `??__E`/`??__F` key has the same canonical identity."""
    keys = [overload_key(rec["mangled"], rec["name"]), rec["mangled"]]
    canon = dyn_canon_base(rec["mangled"])
    rows = store.lookup(keys, prefix="??__" if canon else "")
    for key in keys:
        if key in rows:
            return rows[key]
    if canon:
        for key, row in rows.items():
            if dyn_canon_base(key) == canon:
                return row
    return None


def resolve(selector: str, target_db: Path, base_db: Path):
    """(target record | None, base record | None, the two connections).

    Raises Ambiguous when the selector names several functions, LookupError
    when it names none. A bare hex is a retail or our RVA or VA at once, so
    both sides are asked and a disagreement is listed, never picked."""
    target_con, base_con = connect(target_db), connect(base_db)
    if HEX.match(selector):
        pairs = [(t, partner(base_con, t)) for t in find(target_con, selector)]
        for b in find(base_con, selector):
            if not any(ours and ours["rva"] == b["rva"] for _, ours in pairs):
                pairs.append((partner(target_con, b), b))
    else:
        targets = find(target_con, selector)
        if targets:
            pairs = [(t, None) for t in targets]
        else:
            pairs = [(None, b) for b in find(base_con, selector)]
    if len(pairs) > 1:
        raise Ambiguous(selector, [dict(t, side="retail") if t else dict(b, side="ours")
                                   for t, b in pairs])
    if not pairs:
        raise LookupError(f"no function matches '{selector}'")
    target, base = pairs[0]
    if base is None and target is not None:
        base = partner(base_con, target)
    elif target is None:
        target = partner(target_con, base)
    return target, base, target_con, base_con
