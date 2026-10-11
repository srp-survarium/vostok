# SPDX-License-Identifier: GPL-3.0-or-later

"""Section-contribution layout and COMDAT copy selection, retail beside ours.

    python3 -m vostok tool link-layout runs     [--section N] [--limit N]
    python3 -m vostok tool link-layout owners   [--module M] [--all] [--pass1 LINK_VERBOSE_LOG]

`runs` prints each PDB's section contributions as consecutive runs of one
archive (or direct object), split into COMDAT and non-COMDAT code, so the
link layout can be read side by side.

`owners` lists COMDAT functions whose code the two linkers took from different
compilands. Under /LTCG every TU that references an inline or template function
carries an IL copy, and the linker keeps the copy of the first module it loads
in pass 1. When the candidates were compiled with different optimisation
settings (game_core /Od against game /Ox, say) the kept body differs. Given a
`/VERBOSE` log of our final link (`--pass1`), a row whose retail owner loads
before ours is `missing-reference`: the retail owner's TU references the
function and ours does not. `load-order` means ours loads first, `roster` that
one owner is absent from a PDB. Rows are joined with the ledger; non-exact rows
come first.

The reader decodes only the MSF directory, the DBI module list, the section
contribution substream and the section headers, which llvm-pdbutil cannot open
for these PDBs (too many directory blocks).
"""

from __future__ import annotations

import argparse
import bisect
import csv
import re
import sqlite3
import struct
from dataclasses import dataclass
from pathlib import Path

from vostok.core import log as _log
from vostok.core.paths import BASE_PDB, MATCH_STATE, NINJA_DIR, PDB_DIR, survarium_bin

IMAGE_SCN_CNT_CODE = 0x20
IMAGE_SCN_LNK_COMDAT = 0x1000


@dataclass(frozen=True)
class Module:
    index: int
    name: str
    archive: str

    @property
    def key(self) -> str:
        """Path-independent identity: archive basename | object basename."""
        lib = _basename(self.archive).lower() if self.archive.lower().endswith(".lib") else "<obj>"
        return f"{lib}|{_basename(self.name).lower()}"


@dataclass(frozen=True)
class Contribution:
    section: int
    offset: int
    size: int
    characteristics: int
    module: int

    @property
    def comdat(self) -> bool:
        return bool(self.characteristics & IMAGE_SCN_LNK_COMDAT)

    @property
    def code(self) -> bool:
        return bool(self.characteristics & IMAGE_SCN_CNT_CODE)


@dataclass(frozen=True)
class Layout:
    modules: list[Module]
    contributions: list[Contribution]
    section_va: list[int]          # 1-based section index -> virtual address

    def rva(self, c: Contribution) -> int:
        return self.section_va[c.section] + c.offset


def _basename(path: str) -> str:
    return path.replace("\\", "/").rsplit("/", 1)[-1]


class _Msf:
    """MSF 7.00 stream reader."""

    MAGIC = b"Microsoft C/C++ MSF 7.00\r\n\x1aDS\0\0\0"

    def __init__(self, data: bytes):
        if not data.startswith(self.MAGIC):
            raise ValueError("not an MSF 7.00 PDB")
        self.data = data
        self.block, _fpm, _count, dir_bytes, _unknown, map_addr = struct.unpack_from("<6I", data, 32)
        nblocks = -(-dir_bytes // self.block)
        dir_blocks = struct.unpack_from(f"<{nblocks}I", data, map_addr * self.block)
        directory = b"".join(self._read(b) for b in dir_blocks)[:dir_bytes]
        count = struct.unpack_from("<I", directory, 0)[0]
        sizes = struct.unpack_from(f"<{count}I", directory, 4)
        pos = 4 + 4 * count
        self.streams: list[tuple[int, tuple[int, ...]]] = []
        for size in sizes:
            size = 0 if size == 0xFFFFFFFF else size
            n = -(-size // self.block)
            self.streams.append((size, struct.unpack_from(f"<{n}I", directory, pos)))
            pos += 4 * n

    def _read(self, block: int) -> bytes:
        return self.data[block * self.block:(block + 1) * self.block]

    def stream(self, index: int) -> bytes:
        size, blocks = self.streams[index]
        return b"".join(self._read(b) for b in blocks)[:size]


def _cstr(data: bytes, pos: int) -> tuple[str, int]:
    end = data.index(b"\0", pos)
    return data[pos:end].decode("latin-1"), end + 1


def parse_dbi(dbi: bytes, section_headers: bytes) -> Layout:
    """Decode the DBI module list and section contributions."""
    (mod_size, sc_size, map_size, src_size, tsm_size, _mfc, dbg_size, ec_size) = \
        struct.unpack_from("<iiiiiIii", dbi, 24)
    modules = []
    pos, end = 64, 64 + mod_size
    while pos < end:
        name, nxt = _cstr(dbi, pos + 64)
        archive, nxt = _cstr(dbi, nxt)
        modules.append(Module(len(modules), name, archive))
        pos = (nxt + 3) & ~3
    version = struct.unpack_from("<I", dbi, end)[0]
    entry = 32 if version == 0xEFFE0000 + 20140516 else 28
    contributions = []
    pos = end + 4
    while pos + entry <= end + sc_size:
        section, _pad, offset, size, chars, module = struct.unpack_from("<HHiiIH", dbi, pos)
        contributions.append(Contribution(section, offset, size, chars, module))
        pos += entry
    section_va = [0]
    for k in range(len(section_headers) // 40):
        section_va.append(struct.unpack_from("<I", section_headers, k * 40 + 12)[0])
    return Layout(modules, contributions, section_va)


def read_layout(pdb: Path) -> Layout:
    msf = _Msf(pdb.read_bytes())
    dbi = msf.stream(3)
    mod_size, sc_size, map_size, src_size, tsm_size, _mfc, dbg_size, ec_size = \
        struct.unpack_from("<iiiiiIii", dbi, 24)
    dbg = 64 + mod_size + sc_size + map_size + src_size + tsm_size + ec_size
    section_header_stream = struct.unpack_from("<H", dbi, dbg + 10)[0]
    return parse_dbi(dbi, msf.stream(section_header_stream))


def runs(layout: Layout, section: int) -> list[tuple[str, bool, int, int, int]]:
    """Consecutive (archive-or-object, comdat, count, bytes, first offset) runs."""
    out: list[list] = []
    for c in sorted((c for c in layout.contributions if c.section == section), key=lambda c: c.offset):
        lib = layout.modules[c.module].key.split("|")[0]
        if out and out[-1][0] == lib and out[-1][1] == c.comdat:
            out[-1][2] += 1
            out[-1][3] += c.size
        else:
            out.append([lib, c.comdat, 1, c.size, c.offset])
    return [tuple(r) for r in out]


def code_owners(layout: Layout, functions: list[tuple[str, int]]) -> dict[str, tuple[str, bool]]:
    """mangled -> (owning module key, comdat) for functions with one placement."""
    code = sorted((c for c in layout.contributions if c.code), key=layout.rva)
    starts = [layout.rva(c) for c in code]
    seen: dict[str, tuple[str, bool] | None] = {}
    for mangled, rva in functions:
        i = bisect.bisect_right(starts, rva) - 1
        if i < 0 or rva >= starts[i] + code[i].size:
            continue
        owner = (layout.modules[code[i].module].key, code[i].comdat)
        seen[mangled] = owner if mangled not in seen else None
    return {m: o for m, o in seen.items() if o is not None}


def pass1_order(log_text: str) -> dict[str, int]:
    """Module key -> first `Loaded archive(member)` position in a /VERBOSE link log."""
    order: dict[str, int] = {}
    for m in re.finditer(r"Loaded ([^\s(]+\.lib)\(([^)]+)\)", log_text):
        key = f"{_basename(m.group(1)).lower()}|{_basename(m.group(2)).lower()}"
        order.setdefault(key, len(order))
    return order


def optimization_classes(ninja_dir: Path) -> dict[str, str]:
    """archive basename -> Od / O2 / partial (no /O umbrella), from the cl response files."""
    out = {}
    rsp = ninja_dir / "rsp"
    for lib_rsp in rsp.glob("*_lib.rsp"):
        m = re.search(r'/OUT:"?([^"\s]+\.lib)', lib_rsp.read_text(errors="replace"))
        cl = rsp / lib_rsp.name.replace("_lib.rsp", "_cl_0.rsp")
        if not m or not cl.is_file():
            continue
        flags = set(cl.read_text(errors="replace").split())
        cls = "Od" if "/Od" in flags else "O2" if flags & {"/O2", "/Ox", "/O1"} else "partial"
        out[_basename(m.group(1)).lower()] = cls
    return out


def _functions(db: Path) -> list[tuple[str, int]]:
    return list(sqlite3.connect(db).execute("select mangled, rva from functions"))


def _ledger() -> dict[str, dict[str, str]]:
    with open(MATCH_STATE, newline="") as f:
        rows = csv.DictReader((line for line in f if not line.startswith("#")), delimiter="\t")
        return {r["mangled"]: r for r in rows}


def _class(classes: dict[str, str], key: str) -> str:
    lib = key.split("|")[0]
    return classes.get(lib, "O2" if lib == "<obj>" else "vendor")


def owner_rows(target: Layout, base: Layout, target_fns, base_fns, classes, ledger, pass1=None):
    t_own, b_own = code_owners(target, target_fns), code_owners(base, base_fns)
    t_keys = {m.key for m in target.modules}
    b_keys = {m.key for m in base.modules}
    rows = []
    for mangled, (tk, comdat) in t_own.items():
        if not comdat or mangled not in b_own or b_own[mangled][0] == tk:
            continue
        bk = b_own[mangled][0]
        if not {tk, bk} <= t_keys or not {tk, bk} <= b_keys:
            verdict = "roster"
        elif pass1 and tk in pass1 and bk in pass1:
            verdict = "missing-reference" if pass1[tk] < pass1[bk] else "load-order"
        else:
            verdict = "owner-differs"
        led = ledger.get(mangled, {})
        rows.append({
            "mangled": mangled, "cur": led.get("cur", ""), "max": led.get("max", ""),
            "status": led.get("status", ""), "target_owner": tk, "target_class": _class(classes, tk),
            "base_owner": bk, "base_class": _class(classes, bk), "verdict": verdict,
        })
    rows.sort(key=lambda r: (r["cur"] == "100", r["target_class"] == r["base_class"], r["mangled"]))
    return rows


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(prog="vostok tool link-layout", description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("runs", help="archive runs per section, retail beside ours")
    r.add_argument("--section", type=int, default=1, help="1=.text 2=.rdata 3=.data")
    r.add_argument("--limit", type=int, default=80)
    o = sub.add_parser("owners", help="COMDATs taken from a different compiland")
    o.add_argument("--module", default="", help="substring of either owner key")
    o.add_argument("--all", action="store_true", help="include exact rows")
    o.add_argument("--pass1", type=Path, help="/VERBOSE log of our final link")
    args = ap.parse_args(argv)

    target = read_layout(survarium_bin() / "survarium.pdb")
    base = read_layout(BASE_PDB)
    if args.cmd == "runs":
        for side, layout in (("retail", target), ("ours", base)):
            print(f"== {side}")
            for lib, comdat, n, size, off in runs(layout, args.section)[:args.limit]:
                print(f"  {off:#9x} {'comdat' if comdat else 'plain ':6} {n:6d} {size:9d}  {lib}")
        return 0
    rows = owner_rows(target, base, _functions(PDB_DIR / "target" / "evidence.sqlite"),
                      _functions(PDB_DIR / "base" / "evidence.sqlite"),
                      optimization_classes(NINJA_DIR), _ledger(),
                      pass1_order(args.pass1.read_text(errors="replace")) if args.pass1 else None)
    print("cur\tmax\tstatus\tverdict\ttarget_owner(class)\tbase_owner(class)\tmangled")
    for row in rows:
        if not args.all and row["cur"] == "100":
            continue
        if args.module and args.module not in row["target_owner"] + row["base_owner"]:
            continue
        print(f"{row['cur']}\t{row['max']}\t{row['status']}\t{row['verdict']}\t"
              f"{row['target_owner']}({row['target_class']})\t{row['base_owner']}({row['base_class']})\t"
              f"{row['mangled']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.link_layout", main))
