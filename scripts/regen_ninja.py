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
  python3 scripts/regen_ninja.py --compdb   # also force the clangd inputs
"""

import argparse
import os
import subprocess
import sys
import tempfile
from pathlib import Path


from vostok.core.paths import NINJA_DIR as BUILD_DIR
from vostok.core.paths import REPO as VOSTOK_DIR
from vostok.core.paths import SLN as SLN_PATH

# clangd inputs at the repo root (gitignored, so absent on fresh clones and
# worktrees). They only depend on flags and file sets, never on #includes.
COMPDB_FILES = ("compile_commands.json", "clangd-vfs.yaml")


def log(msg: str) -> None:
    print(f"[regen-ninja] {msg}", flush=True)


def wine_path(p: Path) -> str:
    """/a/b -> Z:\\a\\b (the drive-rooted form vcproj2ninja --wine emits)."""
    return "Z:" + str(p).replace("/", "\\")


def gen_fresh(out_dir: Path, target: str = "ninja") -> None:
    exe = os.environ.get("VCPROJ2NINJA_EXE")
    if not exe:
        sys.exit("[regen-ninja] VCPROJ2NINJA_EXE not set - run from `nix develop`")
    if not SLN_PATH.is_file():
        sys.exit(f"[regen-ninja] solution not found: {SLN_PATH}")
    out_dir.mkdir(parents=True, exist_ok=True)
    # vcproj2ninja sometimes exits non-zero under wine even on success; trust the
    # produced output over the return code (same as setup-toolchain.py).
    subprocess.run(
        ["wine", exe, "--wine", "--target", target, "--sln-path", str(SLN_PATH),
         "--configuration-platform", "Master Gold|Win32",
         "--output-dir", str(out_dir),
         "--project-name", "survarium - PC - DirectX 11"],
        check=False, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    probe = "build.ninja" if target == "ninja" else "compile_commands.json"
    if not (out_dir / probe).is_file():
        sys.exit(f"[regen-ninja] vcproj2ninja did not produce {probe}")


def regenerate(dry_run: bool = False, compdb: bool = False) -> list[str]:
    """Regenerate and merge; return the relative paths that changed.

    The clangd inputs (COMPDB_FILES) are include-invariant, so they are only
    regenerated when something they DO depend on may have moved: a generated
    file appeared or went stale (TU/module added or removed), or they are
    missing entirely (fresh clone/worktree). Pass compdb=True to force them
    (e.g. after a flags-only .vcproj edit, which this trigger can't see)."""
    changed: list[str] = []
    tu_set_changed = False
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
            if not dst.is_file():
                # A brand-new .ninja/.rsp means a module/group/TU appeared -
                # the only kind of change the compdb cares about. A content
                # change to an existing file (new #include, flag edit) is not.
                tu_set_changed = True
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
                tu_set_changed = True

    verb = "would change" if dry_run else "changed"
    log(f"{verb}: {len(changed)} file(s)" if changed
        else "nothing to do - ninja graph already matches the .vcprojs/sources.")
    for rel in changed:
        log(f"  {rel}")

    compdb_missing = not all((VOSTOK_DIR / n).is_file() for n in COMPDB_FILES)
    if compdb or tu_set_changed or compdb_missing:
        reason = ("forced" if compdb
                  else "TU set changed" if tu_set_changed else "missing")
        log(f"compdb: regenerating ({reason})")
        changed += regenerate_compdb(dry_run=dry_run)
    return changed


def regenerate_compdb(dry_run: bool = False) -> list[str]:
    """clangd inputs at the repo root: compile_commands.json + clangd-vfs.yaml
    (vcproj2ninja --target clangd), write-if-changed like the graph.

    A new #include changes NEITHER file (flags and file sets are untouched -
    include tracking lives in the ninja graph), so clangd's background index
    reindexes only TUs whose content digests moved, never the world. The
    overlay changes only when files appear/disappear, and its content is not
    part of clangd's shard validity - only the path in the command is.
    """
    changed: list[str] = []
    with tempfile.TemporaryDirectory(prefix="compdb_regen_") as tmp:
        tmp_dir = Path(tmp)
        gen_fresh(tmp_dir, target="clangd")
        for name in COMPDB_FILES:
            # the overlay path inside the commands points at the temp dir
            text = (tmp_dir / name).read_text().replace(str(tmp_dir), str(VOSTOK_DIR))
            if name == "compile_commands.json":
                # vcproj2ninja joins the -ivfsoverlay path with a Windows
                # separator even for the native clangd target, so the flag reads
                # `<repo>\clangd-vfs.yaml`. clang takes the command as a Unix path,
                # the stray backslash points at a file that doesn't exist, and the
                # whole TU fails with `error: missing_vfs_overlay_file`. Every
                # other path here is already '/'-separated; normalise just this one
                # (the JSON-escaped "\\" -> "/").
                text = text.replace("\\\\clangd-vfs.yaml", "/clangd-vfs.yaml")
            dst = VOSTOK_DIR / name
            if dst.is_file() and dst.read_text() == text:
                continue
            changed.append(name)
            if not dry_run:
                dst.write_text(text)
    if changed:
        log(f"compdb: {', '.join(changed)} updated")
    return changed


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dry-run", action="store_true",
                    help="report the delta, write nothing")
    ap.add_argument("--compdb", action="store_true",
                    help="force compile_commands.json + clangd-vfs.yaml regen "
                         "(they auto-regen when missing or the TU set changes; "
                         "force after a flags-only .vcproj edit)")
    args = ap.parse_args()
    regenerate(dry_run=args.dry_run, compdb=args.compdb)


if __name__ == "__main__":
    main()
