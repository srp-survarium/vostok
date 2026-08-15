"""vostok.sema.disasm - pdb_fetch renders the assembly, this parses it.

Both sides come out of the same delinker and the same disassembler, so almost
no instruction-spelling normalization is needed - which is what lets everything
downstream treat a textual difference as a real byte difference.
"""

from __future__ import annotations

import os
import re
import shutil
import subprocess

from vostok.core.paths import REPO as VOSTOK

from vostok.sema import die
from vostok.sema.index import _index_path


RE_STMT = re.compile(r"^\[0x([0-9a-fA-F]+)\]:(?:\s*(.*))?$")


RE_LABEL = re.compile(r"^\.(\d+):\s*$")


RE_INSN = re.compile(r"^0x([0-9a-fA-F]+):\s+(\S.*?)\s*$")


def _pdb_fetch():
    exe = os.environ.get("PDB_FETCH") or shutil.which("pdb_fetch")
    if not exe:
        die("pdb_fetch not on PATH - run inside `nix develop`")
    return exe


def disasm(side, rva):
    """One side's rendered disassembly (offsets, `.N` labels, symbolized calls,
    and - base side only - the source statement headers)."""
    cmd = [_pdb_fetch(), f"--{side}-index", str(_index_path(side)),
           "--rva", hex(rva), "--view", side]
    res = subprocess.run(cmd, cwd=str(VOSTOK), capture_output=True, text=True)
    if res.returncode != 0:
        die(f"pdb_fetch failed: {res.stderr.strip()}")
    if "no function matched" in res.stdout:
        die(f"pdb_fetch found no {side} function at {rva:#x}")
    return res.stdout


def parse(text):
    """-> (insns, labels, stmts): [(off, text)], {'.1': off}, {off: source}."""
    insns, labels, stmts, pending = [], {}, {}, []
    for line in text.splitlines():
        line = line.rstrip()
        m = RE_LABEL.match(line)
        if m:
            pending.append("." + m.group(1))
            continue
        m = RE_INSN.match(line)
        if m:
            off = int(m.group(1), 16)
            for lab in pending:
                labels[lab] = off
            pending = []
            insns.append((off, re.sub(r"\s+", " ", m.group(2)).strip()))
            continue
        m = RE_STMT.match(line)
        if m:
            stmts[int(m.group(1), 16)] = (m.group(2) or "").strip()
    return insns, labels, stmts
