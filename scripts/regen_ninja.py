#!/usr/bin/env python3
"""Regenerate the ninja build graph into binaries/ninja, writing only files
whose content actually changed.

vcproj2ninja output is deterministic (vcproj2ninja#6), so a per-file byte
compare is exact: a no-op regen writes nothing and bumps no mtimes (rsp files
are implicit inputs of their edges - a gratuitous rewrite would dirty every TU
in the module), while a real change (source added/excluded, flags, a new
#include discovered by the header scan) rewrites just the affected files and
ninja rebuilds exactly those edges. rebuild.py runs this before every build.

The tool clears its output dir on each run - writing in place would bump every
mtime - so it emits into a temp dir, whose path lands inside the generated
text (rsp @-references and rsp implicit-input lines). Those are rewritten to
the binaries/ninja form before comparing.

Usage:
  python3 scripts/regen_ninja.py            # regen + merge (minimal rebuild)
  python3 scripts/regen_ninja.py --dry-run  # report the delta, write nothing
"""

import argparse
import os
import subprocess
import sys
import tempfile
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPT_DIR.parent
BUILD_DIR  = VOSTOK_DIR / "binaries" / "ninja"
SLN_PATH   = VOSTOK_DIR / "sources" / "vostok v2.0.sln"


def log(msg: str) -> None:
    print(f"[regen-ninja] {msg}", flush=True)


def wine_path(p: Path) -> str:
    """/a/b -> Z:\\a\\b (the drive-rooted form vcproj2ninja --wine emits)."""
    return "Z:" + str(p).replace("/", "\\")


def gen_fresh(out_dir: Path) -> None:
    exe = os.environ.get("VCPROJ2NINJA_EXE")
    if not exe:
        sys.exit("[regen-ninja] VCPROJ2NINJA_EXE not set - run from `nix develop`")
    if not SLN_PATH.is_file():
        sys.exit(f"[regen-ninja] solution not found: {SLN_PATH}")
    out_dir.mkdir(parents=True, exist_ok=True)
    # vcproj2ninja sometimes exits non-zero under wine even on success; trust the
    # produced build.ninja over the return code (same as setup-toolchain.py).
    subprocess.run(
        ["wine", exe, "--wine", "--sln-path", str(SLN_PATH),
         "--configuration-platform", "Master Gold|Win32",
         "--output-dir", str(out_dir),
         "--project-name", "survarium - PC - DirectX 11"],
        check=False, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    if not (out_dir / "build.ninja").is_file():
        sys.exit("[regen-ninja] vcproj2ninja did not produce build.ninja")


def regenerate(dry_run: bool = False) -> list[str]:
    """Regenerate and merge; return the relative paths that changed."""
    changed: list[str] = []
    with tempfile.TemporaryDirectory(prefix="ninja_regen_") as tmp:
        tmp_dir = Path(tmp)
        gen_fresh(tmp_dir)

        # The temp path appears in two spellings: raw in `flags = @...` lines,
        # ninja-escaped (`:` -> `$:`) in the rsp implicit-input dep lines.
        raw_t, raw_b = wine_path(tmp_dir), wine_path(BUILD_DIR)
        esc_t, esc_b = raw_t.replace(":", "$:"), raw_b.replace(":", "$:")

        fresh = sorted(p for p in tmp_dir.rglob("*") if p.is_file())
        for fp in fresh:
            rel = fp.relative_to(tmp_dir)
            text = fp.read_text().replace(raw_t, raw_b).replace(esc_t, esc_b)
            dst = BUILD_DIR / rel
            if dst.is_file() and dst.read_text() == text:
                continue
            changed.append(str(rel))
            if not dry_run:
                dst.parent.mkdir(parents=True, exist_ok=True)
                dst.write_text(text)

        # Generated files no longer produced (module removed/renamed). Report
        # only: BUILD_DIR also holds ninja state (.ninja_log) we must not touch.
        fresh_set = {str(p.relative_to(tmp_dir)) for p in fresh}
        for p in sorted(BUILD_DIR.rglob("*")):
            if not p.is_file() or p.suffix not in (".ninja", ".rsp"):
                continue
            rel = str(p.relative_to(BUILD_DIR))
            if rel not in fresh_set:
                log(f"STALE (delete manually): {rel}")

    verb = "would change" if dry_run else "changed"
    log(f"{verb}: {len(changed)} file(s)" if changed
        else "nothing to do - ninja graph already matches the .vcprojs/sources.")
    for rel in changed:
        log(f"  {rel}")
    return changed


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dry-run", action="store_true",
                    help="report the delta, write nothing")
    args = ap.parse_args()
    regenerate(dry_run=args.dry_run)


if __name__ == "__main__":
    main()
