#!/usr/bin/env python3
"""Regenerate the ninja build graph and MERGE it into binaries/ninja, applying
only the real structural delta - so flipping a .vcproj `ExcludedFromBuild` (or any
single source add/remove) rebuilds just that one module, not the whole engine.

Why a merge and not a plain regen: vcproj2ninja is **non-deterministic**:
  - every `.ninja` embeds the `--output-dir` in its `flags = @<dir>\\rsp\\..` lines
    (the build edges themselves are otherwise deterministic);
  - a module's sources are split across `<module>_cl_0/1/2.rsp` batch files and the
    partition is shuffled run-to-run (same total set, different batches), which
    also reshuffles the obj lists in `<module>.ninja`.
A naive regen therefore rewrites nearly every file and ninja cascade-rebuilds the
whole engine (an edge whose obj-output list changed is re-run).

So we work at **module granularity**: regenerate into a temp dir, then for each
module compare the UNION of its sources (across all its `_cl_*.rsp`). If the union
is unchanged, the module is left byte-identical (not rebuilt) - the partition
shuffle is ignored. Only a module whose source set actually changed (e.g. a
`.cpp` un-excluded) gets its `.ninja` + rsps rewritten, so just that module (and
the final relink that consumes its `.lib`) rebuilds. Non-module top-level files
(`build.ninja`, the link project) are exact-compared after output-dir
normalization.

Usage:
  python3 scripts/regen_ninja.py            # regen + merge (minimal rebuild)
  python3 scripts/regen_ninja.py --dry-run  # report the delta, write nothing
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPT_DIR.parent
BUILD_DIR  = VOSTOK_DIR / "binaries" / "ninja"
SLN_PATH   = VOSTOK_DIR / "sources" / "vostok v2.0.sln"

# rsp/<module>_cl_<n>.rsp  or  rsp/<module>_lib.rsp  ->  <module>
_RSP_MODULE = re.compile(r"^(?P<m>.+?)_(?:cl_\d+|lib)\.rsp$")


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
    # Capture its chatter instead of discarding it, so a REAL failure (no
    # build.ninja produced) can show what went wrong.
    proc = subprocess.run(
        ["wine", exe, "--wine", "--sln-path", str(SLN_PATH),
         "--configuration-platform", "Master Gold|Win32",
         "--output-dir", str(out_dir),
         "--project-name", "survarium - PC - DirectX 11"],
        check=False, capture_output=True, text=True, errors="replace",
    )
    if not (out_dir / "build.ninja").is_file():
        tail = "\n".join((proc.stdout + proc.stderr).splitlines()[-20:])
        if tail:
            print(tail, file=sys.stderr)
        sys.exit("[regen-ninja] vcproj2ninja did not produce build.ninja")


def normalize(text: str, tmp_dir: Path) -> str:
    """Rewrite the temp output-dir back to binaries/ninja so the only remaining
    differences are real ones (a source added/removed)."""
    return text.replace(wine_path(tmp_dir), wine_path(BUILD_DIR))


def rsp_sources(path: Path) -> set:
    """Body lines (sources/objs) of an rsp, ignoring the leading flags line."""
    if not path.is_file():
        return set()
    lines = path.read_text().splitlines()
    return set(lines[1:])


def module_sources(rsp_dir: Path, module: str) -> set:
    """Union of sources across a module's compile rsps (`<module>_cl_*.rsp`)."""
    out = set()
    for rsp in rsp_dir.glob(f"{module}_cl_*.rsp"):
        out |= rsp_sources(rsp)
    return out


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dry-run", action="store_true",
                    help="report the delta, write nothing")
    args = ap.parse_args()

    with tempfile.TemporaryDirectory(prefix="ninja_regen_") as tmp:
        tmp_dir = Path(tmp)
        log(f"regenerating into {tmp_dir} ...")
        gen_fresh(tmp_dir)
        fresh_rsp = tmp_dir / "rsp"

        modules = sorted(p.stem for p in tmp_dir.glob("*.ninja")
                         if p.name != "build.ninja")

        changed_mods, new_mods, top_changed, unchanged_mods = [], [], [], 0

        def write(rel: Path, text: str) -> None:
            if args.dry_run:
                return
            dst = BUILD_DIR / rel
            dst.parent.mkdir(parents=True, exist_ok=True)
            dst.write_text(text)

        # --- per-module: compare the source UNION, ignore partition shuffle ---
        for mod in modules:
            mod_ninja = tmp_dir / f"{mod}.ninja"
            fresh_src = module_sources(fresh_rsp, mod)
            exist_src = module_sources(BUILD_DIR / "rsp", mod)
            existed = (BUILD_DIR / f"{mod}.ninja").is_file()

            if existed and fresh_src == exist_src:
                unchanged_mods += 1
                continue

            (new_mods if not existed else changed_mods).append(
                (mod, len(fresh_src - exist_src), len(exist_src - fresh_src)))
            # rewrite the module's .ninja + every one of its rsps from fresh
            write(Path(f"{mod}.ninja"), normalize(mod_ninja.read_text(), tmp_dir))
            for rsp in fresh_rsp.glob(f"{mod}_*.rsp"):
                write(Path("rsp") / rsp.name,
                      normalize(rsp.read_text(), tmp_dir))

        # --- non-module top-level files (build.ninja, etc.): exact compare ---
        for fp in sorted(tmp_dir.glob("*.ninja")):
            if fp.stem in modules:
                continue
            rel = fp.relative_to(tmp_dir)
            fresh_text = normalize(fp.read_text(), tmp_dir)
            dst = BUILD_DIR / rel
            if dst.is_file() and dst.read_text() == fresh_text:
                continue
            top_changed.append(str(rel))
            write(rel, fresh_text)

        verb = "would change" if args.dry_run else "changed"
        log(f"{verb}: {len(new_mods)} new module(s), {len(changed_mods)} module(s) "
            f"updated, {len(top_changed)} top-level file(s); "
            f"{unchanged_mods} modules unchanged")
        for m, a, r in new_mods:
            log(f"  NEW    {m}  (+{a} sources)")
        for m, a, r in changed_mods:
            log(f"  MODULE {m}  (+{a} -{r} sources)")
        for f in top_changed:
            log(f"  TOP    {f}")
        if not changed_mods and not new_mods and not top_changed:
            log("nothing to do - ninja graph already matches the .vcprojs.")
        if args.dry_run:
            log("dry-run: nothing written.")


if __name__ == "__main__":
    main()
