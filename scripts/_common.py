"""Helpers shared by the scripts/ build-and-diff tools.

Everything here used to be copy-pasted per script; keep it in one place so a
behavior change (e.g. how Wine renders PDB paths) lands everywhere at once.
"""

import os
import shutil
import sys
from collections.abc import Callable
from pathlib import Path


SCRIPTS_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPTS_DIR.parent


def make_log(tag: str) -> Callable[[str], None]:
    def log(msg: str) -> None:
        print(f"[{tag}] {msg}", flush=True)
    return log


def make_die(tag: str) -> Callable[..., None]:
    def die(msg: str, *hints: str) -> None:
        print(f"[{tag}] ERROR: {msg}", file=sys.stderr)
        for h in hints:
            print(f"  {h}", file=sys.stderr)
        sys.exit(1)
    return die


def wine_pdb_path(p: Path) -> str:
    r"""Render a native absolute path the way MSVC-under-Wine records it in a PDB:
    on the Z: drive (Wine maps ``/`` -> ``Z:``), lowercased, ``\``-separated.
    e.g. /home/u/Proj/vostok/sources -> z:\home\u\proj\vostok\sources

    Not to be confused with the drive-rooted ``Z:\...`` form vcproj2ninja emits
    into the build graph (case preserved - see regen_ninja.wine_path).
    """
    return "z:" + str(p).replace("/", "\\").lower()


def nonempty_dir(p: Path) -> bool:
    return p.is_dir() and any(p.iterdir())


def survarium_bin_dir() -> Path:
    """Directory holding the original survarium.{exe,pdb} (the target side):
    $SURVARIUM_BIN inside `nix develop`, else the pinned gcroot it points at."""
    return Path(os.environ.get(
        "SURVARIUM_BIN", VOSTOK_DIR / "binaries" / "nix-store" / "survarium-game"
    ))


def begin_output_dir(out: Path) -> Path:
    """Start a crash-safe regeneration of `out`: return a fresh sibling
    `<out>.tmp` for the generator to fill. Pairs with commit_output_dir - a
    generator that dies mid-write leaves the previous `out` untouched instead
    of a half-written tree that confuses the next run."""
    tmp = out.with_name(out.name + ".tmp")
    if tmp.exists():
        shutil.rmtree(tmp)
    tmp.mkdir(parents=True)
    return tmp


def commit_output_dir(tmp: Path, out: Path) -> None:
    """Swap a finished `<out>.tmp` into place, dropping any previous `out`
    (and with it any stale files for units that no longer exist)."""
    if out.exists():
        shutil.rmtree(out)
    tmp.rename(out)
