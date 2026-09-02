# SPDX-License-Identifier: GPL-3.0-or-later
r"""vostok.core.wine - the three spellings of a native path on the Wine side.

`drive_path` is what a command line gets (`Z:\a\b`, case kept); `pdb_path` is
what MSVC under Wine records in a PDB (`z:\a\b`, lowercased); `winepath_w` asks
Wine itself, for paths that may live outside the Z: mapping.
"""
import subprocess
from pathlib import Path


def drive_path(p: Path) -> str:
    """/a/b -> Z:\\a\\b - the drive-rooted form cl/ninja are handed under Wine."""
    return "Z:" + str(p).replace("/", "\\")


def pdb_path(p: Path) -> str:
    r"""Render a native absolute path as MSVC-under-Wine records it in a PDB:
    on the Z: drive (Wine maps ``/`` -> ``Z:``), lowercased, ``\``-separated.
    e.g. /home/u/Proj/vostok/sources -> z:\home\u\proj\vostok\sources
    """
    return "z:" + str(p).replace("/", "\\").lower()


def winepath_w(p: Path) -> str:
    return subprocess.check_output(["winepath", "-w", str(p)], text=True).strip()
