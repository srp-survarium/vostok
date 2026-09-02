# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.derive.inventory - one side's functions, as the campaign sees them.

The rich index says what the linker emitted; report.json says which translation
unit objdiff compared it in; the override tables say who OWNS it. This joins the
three into one record per function, and it is the shape the committed ledger's
roster columns (unit / module / size / frameless) are written from.

`unit` is the TU a matcher would open. It is deliberately the record's own file
when report.json compared the symbol there, so a header-inlined method keeps its
header instead of being filed under whichever .cpp happened to win the fold.
"""

from dataclasses import dataclass

from vostok.derive.index import body_statements, is_framed
from vostok.derive.modules import logical_module


@dataclass(frozen=True)
class Function:
    mangled: str
    demangled: str
    unit: str | None      # report.json TU (or the owning header), None when unknown
    file: str             # source file the rich index attributes the body to
    module: str           # logical owner, resolved through the override tables
    rva: int
    line: int | None      # first statement line, None for line-less compiler bodies
    size: int
    n_stmts: int          # REAL statements: the frame braces are not source
    frameless: bool       # no push ebp/mov ebp,esp: an LTCG-customized leaf


def unit_of(rec, units):
    """The TU to file a record under: its own file when report.json compared it
    there, else the first unit that did, else the file the PDB attributes it to.

    That last fallback is not a guess. report.json is keyed by MANGLED name, and
    the retail PDB carries no mangled name for a dynamic initializer - it stores
    the display form, ``survarium::`dynamic initializer for 's_cc''``, where the
    report says `??__Es_cc@survarium@@YAXXZ`. The lookup misses on spelling
    alone, and the record knew its own file all along. Returning None there
    filed 1,765 real functions under no unit at all - 799 of them dynamic
    initializers - so `--unit` could not see them.
    """
    if units and rec["file"] in units:
        return rec["file"]
    return units[0] if units else rec.get("file")


def describe(mangled, rec, artifacts):
    units = artifacts.units_by_mangled.get(mangled)
    statements = rec["statements"]
    return Function(
        mangled=mangled,
        demangled=artifacts.demangled.get(mangled) or rec["name"],
        unit=unit_of(rec, units),
        file=rec["file"],
        module=logical_module(
            mangled, rec, units, artifacts.dynamic_owners, artifacts.module_overrides
        ),
        rva=rec["rva"],
        # `line` stays over the FULL record: the opening frame brace is where
        # the function starts in source, which is where a matcher wants to land.
        line=min((s["line"] for s in statements if s.get("line")), default=None),
        size=rec["size"],
        n_stmts=len(body_statements(rec)),
        frameless=not is_framed(rec),
    )


def functions(side, artifacts):
    """{mangled: Function} for one side of the rich index, RVA-ordered."""
    described = (describe(mangled, rec, artifacts) for mangled, rec in side.items())
    return {fn.mangled: fn for fn in sorted(described, key=lambda fn: fn.rva)}
