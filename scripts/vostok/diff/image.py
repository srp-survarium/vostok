# SPDX-License-Identifier: GPL-3.0-or-later

"""Whole-image comparison of our linked survarium.exe with retail, region by region.

    python3 -m vostok diff image                    # counts against config/image_diff.tsv
    python3 -m vostok diff image --detail rich      # one region's findings (repeatable)
    python3 -m vostok diff image --update           # bank lower counts (never raises one)

While functions still differ in size, every later address differs too, so a
byte-for-byte file comparison says nothing about the link. The regions compare
like with like instead:

  headers        DOS and PE header bytes, PE headers aligned at each image's own
                 signature; layout fields (sizes, entry point, directories,
                 section positions), the timestamp and the checksum are left out
  rich           |count difference| summed over the Rich header @comp.ids
  imports        import and delay-import tables read semantically: DLL order,
                 thunk names in IAT order, hints
  sections       section-table rows whose name, order or characteristics differ
  rsrc           resource bytes after rebasing data RVAs (all of retail's when
                 ours has no .rsrc)
  code-exact     retail code bytes our image reproduces: functions paired by PDB
                 name whose bytes agree once every relocated field and rel32
                 branch is translated through the PDB symbols of each side
  code-differing paired function bytes that still differ (the ledger's work;
                 informational)
  code-misplaced exact bytes outside the longest run that keeps retail order
  code-order     places where our code order steps backwards in retail order
  code-unplaced  our function bytes with no retail name
  code-absent    retail function bytes with no counterpart in our image
  data-*         the data lane's linked-image audit (binaries/gen/data_image_report.json)
  order-*        pairwise inversions of compiland order per section kind, from
                 `vostok tool link-layout`
  vftables       retail vftables (??_7) our image does not define

header-layout (layout and clock header bytes) and code-untranslated (bytes that
agree but reach a field one side cannot name) are informational as well.
Every count is "lower is closer to retail" except code-exact. config/image_diff.tsv
banks each one as a ceiling (code-exact as a floor); a run fails when a count moves
past its banked value. `--update` only tightens. Nothing here runs in `vostok build`.
"""

from __future__ import annotations

import argparse
import bisect
import json
import re
import sqlite3
import struct
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from difflib import SequenceMatcher
from pathlib import Path

from vostok.core import log as _log
from vostok.core.paths import BASE_EXE, BASE_PDB, CONFIG, DATA_REPORT, PDB_DIR, survarium_bin
from vostok.tool import link_layout as ll

CEILING = CONFIG / "image_diff.tsv"
#: Regions where a larger count is better.
FLOORS = ("code-exact",)
#: Printed but never banked: they rise with progress elsewhere (an absent
#: function that gets paired is "differing" before it is exact).
INFORMATIONAL = ("header-layout", "code-differing", "code-untranslated")
_EXEC = 0x20000000


# ------------------------------------------------------------------------ PE --

@dataclass
class Pe:
    data: bytes
    base: int
    header: int
    sections: list[tuple[str, int, int, int, int, int]]   # name, rva, vsize, raw, rawsize, flags

    @classmethod
    def read(cls, data: bytes) -> "Pe":
        pe = struct.unpack_from("<I", data, 0x3C)[0]
        count = struct.unpack_from("<H", data, pe + 6)[0]
        optional = struct.unpack_from("<H", data, pe + 20)[0]
        base = struct.unpack_from("<I", data, pe + 52)[0]
        rows = []
        for i in range(count):
            at = pe + 24 + optional + 40 * i
            name = data[at:at + 8].rstrip(b"\0").decode("latin-1")
            vsize, rva, rawsize, raw = struct.unpack_from("<IIII", data, at + 8)
            rows.append((name, rva, vsize, raw, rawsize, struct.unpack_from("<I", data, at + 36)[0]))
        return cls(data, base, pe, rows)

    def directory(self, index: int) -> tuple[int, int]:
        return struct.unpack_from("<II", self.data, self.header + 120 + 8 * index)

    def offset(self, rva: int) -> int | None:
        for _n, start, vsize, raw, rawsize, _f in self.sections:
            if start <= rva < start + max(vsize, rawsize):
                return raw + rva - start if rva - start < rawsize else None
        return rva if rva < 0x400 else None

    def read_at(self, rva: int, size: int) -> bytes | None:
        at = self.offset(rva)
        return None if at is None else self.data[at:at + size]

    def cstr(self, rva: int) -> str:
        raw = self.read_at(rva, 512) or b""
        return raw.split(b"\0", 1)[0].decode("latin-1")

    def section(self, name: str):
        return next((s for s in self.sections if s[0] == name), None)

    def fixups(self) -> list[int]:
        """RVAs of every HIGHLOW base relocation."""
        rva, size = self.directory(5)
        blob = self.read_at(rva, size) or b""
        out, pos = [], 0
        while pos + 8 <= len(blob):
            page, block = struct.unpack_from("<II", blob, pos)
            if block < 8:
                break
            for k in range(8, block, 2):
                entry = struct.unpack_from("<H", blob, pos + k)[0]
                if entry >> 12 == 3:
                    out.append(page + (entry & 0xFFF))
            pos += block
        return sorted(out)


# ------------------------------------------------------------------- headers --

def _layout_fields(pe: Pe) -> set[int]:
    """Offsets (from the PE signature) of fields that follow layout or build clock."""
    opt = 24
    fields = [(8, 4),                       # TimeDateStamp
              (opt + 4, 12), (opt + 16, 12), (opt + 56, 4), (opt + 64, 4)]   # sizes, entry, bases, image size, checksum
    for index in (1, 2, 5, 6, 12, 13):      # import, resource, reloc, debug, IAT, delay-import
        fields.append((opt + 96 + 8 * index, 8))
    optional = struct.unpack_from("<H", pe.data, pe.header + 20)[0]
    for index in range(len(pe.sections)):
        fields.append((opt + optional + 40 * index + 8, 16))
    return {offset + i for offset, size in fields for i in range(size)}


def header_difference(want: Pe, got: Pe) -> tuple[int, int, list[str]]:
    """(differing header bytes, differing layout/clock bytes, findings)."""
    first = min(raw for _n, _r, _v, raw, size, _f in want.sections if size)
    rows = []
    differing = sum(1 for i in range(0x80) if i not in range(0x3C, 0x40)
                    and want.data[i] != got.data[i])
    layout = _layout_fields(want)
    moved = 0
    for k in range(first - max(want.header, got.header)):
        if want.data[want.header + k] == got.data[got.header + k]:
            continue
        if k in layout:
            moved += 1
        else:
            differing += 1
            rows.append(f"PE+{k:#x}: retail {want.data[want.header + k]:02x}, ours {got.data[got.header + k]:02x}")
    return differing, moved, rows


def identity_rows(want: Pe, got: Pe) -> list[str]:
    """The headline fields of each image, side by side."""
    def fields(pe: Pe) -> dict[str, str]:
        h = pe.header
        out = {
            "machine": f"{struct.unpack_from('<H', pe.data, h + 4)[0]:#06x}",
            "timestamp": f"{struct.unpack_from('<I', pe.data, h + 8)[0]:#010x}",
            "characteristics": f"{struct.unpack_from('<H', pe.data, h + 22)[0]:#06x}",
            "linker": "{}.{}".format(*pe.data[h + 26:h + 28]),
            "entry": f"{struct.unpack_from('<I', pe.data, h + 40)[0]:#x}",
            "image base": f"{pe.base:#x}",
            "image size": f"{struct.unpack_from('<I', pe.data, h + 80)[0]:#x}",
            "subsystem": str(struct.unpack_from("<H", pe.data, h + 92)[0]),
            "codeview pdb": codeview_path(pe),
        }
        for name, _r, vsize, _raw, _s, _f in pe.sections:
            out[f"{name} vsize"] = f"{vsize:#x}"
        return out
    a, b = fields(want), fields(got)
    return [f"{k:16} retail {a.get(k, '-'):>24}  ours {b.get(k, '-'):>24}{'' if a.get(k) == b.get(k) else '  *'}"
            for k in dict.fromkeys([*a, *b])]


def codeview_path(pe: Pe) -> str:
    rva, size = pe.directory(6)
    for k in range(0, size, 28):
        entry = pe.read_at(rva + k, 28)
        if entry and struct.unpack_from("<I", entry, 12)[0] == 2:
            raw = pe.data[struct.unpack_from("<I", entry, 24)[0]:][:300]
            if raw[:4] == b"RSDS":
                return raw[24:].split(b"\0", 1)[0].decode("latin-1")
    return "-"


def rich_entries(data: bytes) -> Counter:
    end = data.find(b"Rich", 0, 0x400)
    if end < 0:
        return Counter()
    key = struct.unpack_from("<I", data, end + 4)[0]
    start = data.find(struct.pack("<I", 0x536E6144 ^ key), 0, end)
    out = Counter()
    for at in range(start + 16, end, 8):
        comp, count = struct.unpack_from("<II", data, at)
        out[comp ^ key] = count ^ key
    return out


def rich_difference(want: bytes, got: bytes) -> tuple[int, list[str]]:
    a, b = rich_entries(want), rich_entries(got)
    keys = sorted(set(a) | set(b))
    rows = [f"prodid {c >> 16:#04x} build {c & 0xFFFF}: retail {a[c]}, ours {b[c]}"
            for c in keys if a[c] != b[c]]
    return sum(abs(a[c] - b[c]) for c in keys), rows


# ------------------------------------------------------------------- imports --

def import_table(pe: Pe) -> list[tuple[str, list[tuple[object, int]]]]:
    rva, _size = pe.directory(1)
    out = []
    while rva:
        lookup, _stamp, _chain, name, iat = struct.unpack("<5I", pe.read_at(rva, 20))
        if not name:
            break
        out.append((pe.cstr(name), _thunks(pe, lookup or iat, 0)))
        rva += 20
    return out


def delay_import_table(pe: Pe) -> list[tuple[str, list[tuple[object, int]]]]:
    rva, _size = pe.directory(13)
    out = []
    while rva:
        row = pe.read_at(rva, 32)
        attrs, name, _hmod, _iat, names = struct.unpack_from("<5I", row)
        if not name:
            break
        bias = 0 if attrs & 1 else pe.base
        out.append((pe.cstr(name - bias), _thunks(pe, names - bias, bias)))
        rva += 32
    return out


def _thunks(pe: Pe, table: int, bias: int) -> list[tuple[object, int]]:
    out = []
    while True:
        entry = struct.unpack("<I", pe.read_at(table, 4))[0]
        if not entry:
            return out
        if entry & 0x80000000:
            out.append((entry & 0xFFFF, -1))
        else:
            hint = struct.unpack("<H", pe.read_at(entry - bias, 2))[0]
            out.append((pe.cstr(entry - bias + 2), hint))
        table += 4


def import_difference(want: Pe, got: Pe) -> tuple[int, list[str]]:
    """One finding per DLL out of place and per thunk missing, extra or moved
    (aligned by name, so one absent import does not shift every later slot),
    and per differing hint."""
    rows = []
    for label, read in (("import", import_table), ("delay", delay_import_table)):
        a, b = read(want), read(got)
        for op, i1, i2, j1, j2 in SequenceMatcher(None, [d.lower() for d, _t in a],
                                                  [d.lower() for d, _t in b], autojunk=False).get_opcodes():
            if op != "equal":
                rows.append(f"{label} DLLs {op}: retail {[d for d, _t in a[i1:i2]]}, ours {[d for d, _t in b[j1:j2]]}")
        ours = {d.lower(): t for d, t in b}
        for dll, thunks in a:
            other = ours.get(dll.lower())
            if other is None:
                continue
            x, y = [t[0] for t in thunks], [t[0] for t in other]
            for op, i1, i2, j1, j2 in SequenceMatcher(None, x, y, autojunk=False).get_opcodes():
                if op == "equal":
                    for k in range(i2 - i1):
                        if thunks[i1 + k][1] != other[j1 + k][1]:
                            rows.append(f"{label} {dll} {x[i1 + k]}: hint retail {thunks[i1 + k][1]}, ours {other[j1 + k][1]}")
                    continue
                for name in x[i1:i2]:
                    rows.append(f"{label} {dll}: retail-only {name}" if name not in y else
                                f"{label} {dll}: {name} moved")
                for name in y[j1:j2]:
                    if name not in x:
                        rows.append(f"{label} {dll}: ours-only {name}")
    return len(rows), rows


def section_difference(want: Pe, got: Pe) -> tuple[int, list[str]]:
    rows = []
    for index in range(max(len(want.sections), len(got.sections))):
        a = want.sections[index] if index < len(want.sections) else None
        b = got.sections[index] if index < len(got.sections) else None
        if a is None or b is None or a[0] != b[0] or a[5] != b[5]:
            rows.append(f"section #{index}: retail {a and (a[0], hex(a[5]))}, ours {b and (b[0], hex(b[5]))}")
    return len(rows), rows


def rsrc_difference(want: Pe, got: Pe) -> int:
    a, b = want.section(".rsrc"), got.section(".rsrc")
    if a is None:
        return 0 if b is None else b[4]
    if b is None:
        return a[4]

    def rebased(pe: Pe, sec) -> bytes:
        blob = pe.data[sec[3]:sec[3] + sec[4]]
        out = bytearray(blob)

        def walk(at: int, depth: int) -> None:
            named, ids = struct.unpack_from("<HH", blob, at + 12)
            for i in range(named + ids):
                child = struct.unpack_from("<I", blob, at + 20 + 8 * i)[0]
                if child & 0x80000000 and depth < 3:
                    walk(child & 0x7FFFFFFF, depth + 1)
                elif not child & 0x80000000:
                    struct.pack_into("<I", out, child, struct.unpack_from("<I", blob, child)[0] - sec[1])
        try:
            walk(0, 0)
        except struct.error:
            pass
        return bytes(out)
    x, y = rebased(want, a), rebased(got, b)
    return sum(1 for i in range(min(len(x), len(y))) if x[i] != y[i]) + abs(len(x) - len(y))


# ---------------------------------------------------------------- identities --

_ORDINAL = re.compile(r"`\d+'")
_ANONYMOUS = re.compile(r"\?A0x[0-9a-f]{8}")


def normal_name(name: str) -> str:
    """A name with the parts each compile numbers on its own blanked: local-scope
    ordinals (`` `47' ``) and anonymous-namespace hashes (``?A0x0ab9c976``)."""
    return _ANONYMOUS.sub("?A", _ORDINAL.sub("`#'", name))


class Names:
    """RVA -> names of one image, from its PDB publics, data and procedures."""

    def __init__(self, symbols: list[ll.Symbol]):
        at: dict[int, set[str]] = defaultdict(set)
        sizes: dict[int, int] = {}
        for s in symbols:
            at[s.rva].add(normal_name(s.name))
            if s.size:
                sizes[s.rva] = max(sizes.get(s.rva, 0), s.size)
        self.starts = sorted(at)
        self.names = [at[r] for r in self.starts]
        self.sizes = sizes

    def tokens(self, rva: int) -> set[tuple[str, int]]:
        """{(name, offset)} for the nearest named start at or below `rva`."""
        i = bisect.bisect_right(self.starts, rva) - 1
        if i < 0:
            return set()
        start = self.starts[i]
        size = self.sizes.get(start)
        if rva - start > (size if size is not None else 0x10000):   # one-past-the-end counts
            return set()
        return {(n, rva - start) for n in self.names[i]}


@dataclass
class Function:
    rva: int
    size: int
    names: set[str]                       # normalized names, for display
    keys: set[tuple[str, str]] = field(default_factory=set)   # pairing keys


def functions(symbols: list[ll.Symbol], text: tuple[int, int], layout: ll.Layout) -> dict[int, Function]:
    """Procedure spans inside .text, merged by address (ICF twins share one).

    A name defined at one address pairs on its own; a name several compilands
    define at different addresses (statics, anonymous initializers) pairs only
    together with its compiland."""
    out: dict[int, Function] = {}
    for s in symbols:
        if s.size and text[0] <= s.rva < text[1]:
            f = out.setdefault(s.rva, Function(s.rva, s.size, set()))
            f.size = max(f.size, s.size)
    where: dict[str, set[int]] = defaultdict(set)
    scoped: dict[tuple[str, str], set[int]] = defaultdict(set)
    for s in symbols:
        if s.rva in out:
            name = normal_name(s.name)
            where[name].add(s.rva)
            if s.module is not None:
                scoped[(layout.modules[s.module].key, name)].add(s.rva)
    for s in symbols:
        if s.rva not in out:
            continue
        name = normal_name(s.name)
        f = out[s.rva]
        f.names.add(name)
        if len(where[name]) == 1:
            f.keys.add(("", name))
        elif s.module is not None:
            key = (layout.modules[s.module].key, name)
            if len(scoped[key]) == 1:        # overloads share a name inside one compiland
                f.keys.add(key)
    return out


def _branch_sites(code: bytes, boundaries: list[int]) -> list[int]:
    """Offsets of rel32 fields of call/jmp/jcc instructions starting at `boundaries`."""
    out = []
    for off in boundaries:
        op = code[off:off + 2]
        if op[:1] in (b"\xe8", b"\xe9"):
            out.append(off + 1)
        elif len(op) == 2 and op[0] == 0x0F and 0x80 <= op[1] <= 0x8F:
            out.append(off + 2)
    return out


def instruction_starts(db: Path) -> dict[int, list[int]]:
    """Function RVA -> instruction offsets, from an evidence database."""
    out = {}
    q = "select f.rva, p.instructions_json from functions f join function_payloads p on p.function_id=f.id"
    for rva, ins in sqlite3.connect(db).execute(q):
        out.setdefault(rva, [i["off"] for i in json.loads(ins)])
    return out


@dataclass
class CodeReport:
    counts: dict[str, int] = field(default_factory=dict)
    details: dict[str, list[str]] = field(default_factory=lambda: defaultdict(list))


def compare_code(want: Pe, got: Pe, want_syms, got_syms, want_starts: dict[int, list[int]],
                 got_starts: dict[int, list[int]], want_layout: ll.Layout,
                 got_layout: ll.Layout) -> CodeReport:
    report = CodeReport()
    wtext, gtext = want.section(".text"), got.section(".text")
    wf = functions(want_syms, (wtext[1], wtext[1] + wtext[2]), want_layout)
    gf = functions(got_syms, (gtext[1], gtext[1] + gtext[2]), got_layout)
    wnames, gnames = Names(want_syms), Names(got_syms)
    wfix, gfix = set(want.fixups()), set(got.fixups())
    by_key: dict[tuple[str, str], int] = {}
    for f in gf.values():
        for k in f.keys:
            by_key.setdefault(k, f.rva)
    counts = Counter()
    pairs = []
    matched_got: set[int] = set()
    for f in sorted(wf.values(), key=lambda f: f.rva):
        mates = {by_key[k] for k in f.keys if k in by_key}
        if not mates:
            counts["code-absent"] += f.size
            report.details["code-absent"].append(f"{f.rva:#x} {min(f.names)} ({f.size} B)")
            continue
        matched_got |= mates
        rank = {"exact": 0, "untranslated": 1, "differing": 2}
        verdict, g = min(((_same_code(f, gf[m], want, got, wfix, gfix, wnames, gnames,
                                      want_starts.get(f.rva) or got_starts.get(m)), gf[m])
                          for m in sorted(mates)), key=lambda vg: (rank[vg[0]], vg[1].size != f.size, vg[1].rva))
        pairs.append((g.rva, f.rva, f, verdict))
        if verdict == "exact":
            counts["code-exact"] += f.size
        elif verdict == "untranslated":
            counts["code-untranslated"] += f.size
            report.details["code-untranslated"].append(f"{f.rva:#x} {min(f.names)}")
        else:
            counts["code-differing"] += f.size
    for g in gf.values():
        if g.rva not in matched_got:
            counts["code-unplaced"] += g.size
            report.details["code-unplaced"].append(f"{g.rva:#x} {min(g.names)} ({g.size} B)")
    pairs.sort(key=lambda p: p[0])
    for (_ga, ra, fa, _va), (_gb, rb, fb, _vb) in zip(pairs, pairs[1:]):
        if rb < ra:
            counts["code-order"] += 1
            report.details["code-order"].append(f"{rb:#x} {min(fb.names)} follows {ra:#x} {min(fa.names)}")
    keep = _longest_increasing([p[1] for p in pairs])
    for index, (_g, rva, f, verdict) in enumerate(pairs):
        if verdict == "exact" and index not in keep:
            counts["code-misplaced"] += f.size
            report.details["code-misplaced"].append(f"{rva:#x} {min(f.names)} ({f.size} B)")
    for name in ("code-exact", "code-differing", "code-untranslated", "code-misplaced",
                 "code-order", "code-unplaced", "code-absent"):
        report.counts[name] = counts[name]
    return report


def _longest_increasing(values: list[int]) -> set[int]:
    """Indexes of one longest strictly increasing subsequence."""
    tails, prev, tail_index = [], [-1] * len(values), []
    for i, v in enumerate(values):
        k = bisect.bisect_left(tails, v)
        if k == len(tails):
            tails.append(v)
            tail_index.append(i)
        else:
            tails[k] = v
            tail_index[k] = i
        prev[i] = tail_index[k - 1] if k else -1
    out, i = set(), tail_index[-1] if tail_index else -1
    while i >= 0:
        out.add(i)
        i = prev[i]
    return out


def _same_code(f: Function, g: Function, want: Pe, got: Pe, wfix: set[int], gfix: set[int],
               wnames: Names, gnames: Names, starts: list[int] | None) -> str:
    """exact / untranslated (bytes agree, a field has no name on one side) / differing."""
    if f.size != g.size:
        return "differing"
    a, b = want.read_at(f.rva, f.size), got.read_at(g.rva, g.size)
    if not a or not b or len(a) != f.size or len(b) != g.size:
        return "differing"
    fields = [o for o in range(f.size) if f.rva + o in wfix]
    if fields != [o for o in range(g.size) if g.rva + o in gfix]:
        return "differing"
    skip, unknown = set(), False
    for o in fields:
        skip.update(range(o, o + 4))
        x = (struct.unpack_from("<I", a, o)[0] - want.base) & 0xFFFFFFFF
        y = (struct.unpack_from("<I", b, o)[0] - got.base) & 0xFFFFFFFF
        verdict = _same_target(x, y, f, g, wnames, gnames)
        if verdict is None:
            unknown = True
        elif not verdict:
            return "differing"
    for o in _branch_sites(a, starts or []):
        if o in skip or o + 4 > f.size or b[o - 1:o] != a[o - 1:o]:
            continue
        skip.update(range(o, o + 4))
        x = (f.rva + o + 4 + struct.unpack_from("<i", a, o)[0]) & 0xFFFFFFFF
        y = (g.rva + o + 4 + struct.unpack_from("<i", b, o)[0]) & 0xFFFFFFFF
        verdict = _same_target(x, y, f, g, wnames, gnames)
        if verdict is None:
            unknown = True
        elif not verdict:
            return "differing"
    if any(a[i] != b[i] for i in range(f.size) if i not in skip):
        return "differing"
    return "untranslated" if unknown else "exact"


def _same_target(x: int, y: int, f: Function, g: Function, wnames: Names, gnames: Names) -> bool | None:
    if f.rva <= x <= f.rva + f.size or g.rva <= y <= g.rva + g.size:
        return x - f.rva == y - g.rva
    p, q = wnames.tokens(x), gnames.tokens(y)
    if not p or not q:
        return None
    return bool(p & q)


# ---------------------------------------------------------------- data, order --

def data_regions(report_path: Path = DATA_REPORT) -> dict[str, int]:
    """The data lane's linked-image audit, in image-diff terms."""
    if not report_path.is_file():
        return {}
    report = json.loads(report_path.read_text())
    counts, coverage = report["counts"], report["coverage"]
    return {
        "data-inexact": coverage["compared_bytes"] - coverage["exact_bytes"],
        "data-unclaimed": coverage["gross_bytes"] - coverage["claimed_bytes"],
        "data-absent": counts.get("TARGET_ONLY", 0),
        "data-unplaced": counts.get("BASE_ONLY", 0),
    }


def order_regions(target: ll.Layout, base: ll.Layout) -> tuple[dict[str, int], list[str]]:
    counts, rows = {}, []
    for kind, keep in ll.SECTION_KINDS.items():
        name = f"order-{kind}"
        m = ll.order_metrics(target, base, keep)
        counts[name] = m["inversions"]
        rows.append(f"{name:18} shared {m['shared']:5d}  inverted pairs {m['inversions']:8d} "
                    f"({m['inverted_percent']:.2f}%)  in order (LIS) {m['in_order_percent']:.1f}%")
    return counts, rows


def vftable_regions(want_syms, got_syms) -> tuple[int, list[str]]:
    a = {normal_name(s.name) for s in want_syms if s.name.startswith("??_7")}
    b = {normal_name(s.name) for s in got_syms if s.name.startswith("??_7")}
    missing = sorted(a - b)
    return len(missing), missing


# -------------------------------------------------------------------- driver --

@dataclass
class Report:
    counts: dict[str, int]
    details: dict[str, list[str]]


def measure(target_exe: Path, target_pdb: Path, base_exe: Path, base_pdb: Path,
            evidence: Path = PDB_DIR) -> Report:
    want, got = Pe.read(target_exe.read_bytes()), Pe.read(base_exe.read_bytes())
    want_syms, got_syms = ll.read_symbols(target_pdb), ll.read_symbols(base_pdb)
    details: dict[str, list[str]] = defaultdict(list)
    counts: dict[str, int] = {}
    counts["headers"], counts["header-layout"], details["headers"] = header_difference(want, got)
    details["identity"] = identity_rows(want, got)
    counts["rich"], details["rich"] = rich_difference(want.data, got.data)
    counts["imports"], details["imports"] = import_difference(want, got)
    counts["sections"], details["sections"] = section_difference(want, got)
    counts["rsrc"] = rsrc_difference(want, got)
    starts_t = instruction_starts(evidence / "target" / "evidence.sqlite")
    starts_b = instruction_starts(evidence / "base" / "evidence.sqlite")
    want_layout, got_layout = ll.read_layout(target_pdb), ll.read_layout(base_pdb)
    code = compare_code(want, got, want_syms, got_syms, starts_t, starts_b, want_layout, got_layout)
    counts.update(code.counts)
    details.update(code.details)
    counts.update(data_regions())
    order, details["order"] = order_regions(want_layout, got_layout)
    counts.update(order)
    counts["vftables"], details["vftables"] = vftable_regions(want_syms, got_syms)
    return Report(counts, details)


HEADER = ("# Banked counts of `vostok diff image`. Each is a ceiling (code-exact a floor):\n"
          "# a run fails when a count moves past it. `vostok diff image --update` only\n"
          "# tightens a bank; never edit a row to admit a regression.\n"
          "region\tcount\n")


def read_banked(path: Path = CEILING) -> dict[str, int]:
    if not path.is_file():
        return {}
    rows = [line.split("\t") for line in path.read_text().splitlines()
            if line and not line.startswith("#")]
    return {r[0]: int(r[1]) for r in rows[1:]}


def worse(name: str, count: int, banked: int) -> bool:
    return count < banked if name in FLOORS else count > banked


def tightened(counts: dict[str, int], banked: dict[str, int]) -> dict[str, int]:
    """New banks: each region moves only toward retail; new regions are banked as measured."""
    out = {}
    for name, count in counts.items():
        if name in INFORMATIONAL:
            continue
        old = banked.get(name)
        out[name] = count if old is None else (max if name in FLOORS else min)(old, count)
    return out


def write_banked(counts: dict[str, int], path: Path = CEILING) -> None:
    path.write_text(HEADER + "".join(f"{k}\t{v}\n" for k, v in counts.items()))


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(prog="vostok diff image", description=__doc__.split("\n\n")[0])
    ap.add_argument("--detail", action="append", default=[], help="print one region's findings")
    ap.add_argument("--limit", type=int, default=50, help="findings per --detail region")
    ap.add_argument("--update", action="store_true", help="bank counts that moved toward retail")
    ap.add_argument("--base-exe", type=Path, default=BASE_EXE)
    ap.add_argument("--base-pdb", type=Path, default=BASE_PDB)
    args = ap.parse_args(argv)
    retail = survarium_bin()
    report = measure(retail / "survarium.exe", retail / "survarium.pdb", args.base_exe, args.base_pdb)
    banked = read_banked()
    regressions = []
    for name, count in report.counts.items():
        old = banked.get(name)
        mark = ""
        if name in INFORMATIONAL:
            mark = "  (informational)"
        elif old is None:
            mark = "  (not banked)"
        elif worse(name, count, old):
            mark = f"  (WORSE than banked {old})"
            regressions.append(name)
        elif count != old:
            mark = f"  (better than banked {old})"
        print(f"[image] {name:18} {count:12,d}{mark}")
    for name in args.detail:
        lines = report.details.get(name, [])
        for line in lines[:args.limit]:
            print(f"  {name}: {line}")
        if len(lines) > args.limit:
            print(f"  {name}: ... {len(lines) - args.limit} more")
    if args.update:
        if regressions:
            print(f"[image] not banking: worse than banked in {', '.join(regressions)}", file=sys.stderr)
            return 1
        write_banked(tightened(report.counts, banked))
        print(f"[image] banked {CEILING.name}")
        return 0
    return 1 if regressions else 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.diff.image", main))
