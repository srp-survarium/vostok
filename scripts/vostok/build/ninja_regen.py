#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Regenerate the ninja build graph into binaries/ninja, writing only files
whose content actually changed.

vcproj2ninja output is deterministic (vcproj2ninja#6), so a per-file byte
compare is exact: a no-op regen writes nothing and bumps no mtimes (rsp files
are implicit inputs of their edges - a gratuitous rewrite would dirty every TU
in the module), while a real change (source added/excluded, flags, a new
#include discovered by the header scan) rewrites just the affected files and
ninja rebuilds exactly those edges. `vostok build` runs this before every build.

The tool clears its output dir on each run - writing in place would bump every
mtime - so it emits into a temp dir, whose path lands inside the generated
text (rsp @-references and rsp implicit-input lines). Those are rewritten to
the binaries/ninja form before comparing.

Usage:
  python3 -m vostok.build.ninja_regen            # regen + merge (minimal rebuild)
  python3 -m vostok.build.ninja_regen --dry-run  # report the delta, write nothing
  python3 -m vostok.build.ninja_regen --compdb   # also force the clangd inputs
  python3 -m vostok.build.ninja_regen --configuration release

`--configuration` picks the solution configuration. `gold` (the default) is the
matching build and the only one that owns binaries/ninja, the retail-source-root
and LTCG-order corrections, and the clangd inputs; `release`/`debug` generate
plain graphs into their own dirs (binaries/ninja-{release,debug}).
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

from vostok.core import paths
from vostok.core.paths import REPO as VOSTOK_DIR
from vostok.core.paths import SLN as SLN_PATH
from vostok.core.wine import drive_path
from vostok.core.log import logger
from vostok.core import log as _log

# clangd inputs at the repo root (gitignored, so absent on fresh clones and
# worktrees). They only depend on flags and file sets, never on #includes.
COMPDB_FILES = ("compile_commands.json", "clangd-vfs.yaml")

# Retail section contributions preserve this LTCG output order.  The solution
# dependency walk does not: vcproj2ninja expands nested projects before later
# executable dependencies, changing COMDAT retention and PDB ownership.
# Apply the correction only when the complete shipping suite is present, so
# library and focused-module response files remain untouched.
RETAIL_LINK_LIBRARY_ORDER = (
    "vostok_engine_pc_dx11-static-gold.lib",
    "vostok_engine-static-gold.lib",
    "vostok_render_facade-static-gold.lib",
    "vostok_collision-static-gold.lib",
    "vostok_fs-static-gold.lib",
    "vostok_vfs-static-gold.lib",
    "vostok_opcode-static-gold.lib",
    "vostok_physics-static-gold.lib",
    "vostok_bullet-static-gold.lib",
    "vostok_core-static-gold.lib",
    "vostok_network-static-gold.lib",
    "vostok_animation-static-gold.lib",
    "vostok_logging-static-gold.lib",
    "vostok_debug-static-gold.lib",
    "vostok_render_engine_pc_dx11-static-gold.lib",
    "vostok_game-static-gold.lib",
    "vostok_scaleform-static-gold.lib",
    "vostok_game_core-static-gold.lib",
    "vostok_ui-static-gold.lib",
    "vostok_network_core-static-gold.lib",
    "vostok_ai_navigation-static-gold.lib",
    "vostok_libfoundation-static.lib",
    "vostok_particle-static-gold.lib",
    "vostok_ai-static-gold.lib",
    "vostok_input-static-gold.lib",
    "vostok_render_core_pc_dx11-static-gold.lib",
    "vostok_sound-static-gold.lib",
    "vostok_vorbisfile-static-gold.lib",
    "vostok_vorbis-static-gold.lib",
    "vostok_ogg-static-gold.lib",
)

# The Win32 Master Gold sound objects are ordinary /Od objects, not /GL CIL.
# Their retained DBI module sequence follows archive-member order.  This is the
# complete shared sound sequence from the retail PDB; members absent from the
# retail executable keep their generated response-file slots.
RETAIL_ARCHIVE_MEMBER_ORDERS = {
    "sound_lib.rsp": (
        "sound_world.obj",
        "sound_world_orders.obj",
        "world.obj",
        "world_user.obj",
        "sound_collection.obj",
        "sound_collection_cook.obj",
        "single_sound.obj",
        "single_sound_cook.obj",
        "composite_sound.obj",
        "composite_sound_cook.obj",
        "sound_emitter.obj",
        "sound_receiver.obj",
        "sound_propagator.obj",
        "sound_voice.obj",
        "sound_voice_callbacks.obj",
        "sound_instance_proxy.obj",
        "sound_instance_proxy_internal.obj",
        "sound_producer.obj",
        "ogg_utils.obj",
        "ogg_sound.obj",
        "ogg_sound_cook.obj",
        "ogg_file_contents.obj",
        "ogg_file_contents_cook.obj",
        "sound_rms.obj",
        "sound_rms_cook.obj",
        "ogg_source_cook.obj",
        "sound_instance_proxy_order.obj",
        "sound_object_commands.obj",
        "voice_bridge.obj",
        "voice_factory.obj",
        "sound_buffer.obj",
        "sound_buffer_factory.obj",
        "encoded_sound_interface.obj",
        "encoded_sound_with_qualities.obj",
        "ogg_encoded_sound_interface.obj",
        "ogg_encoded_sound_interface_cook.obj",
        "encoded_sound_with_qualities_cook.obj",
        "wav_encoded_sound_interface.obj",
        "wav_encoded_sound_interface_cook.obj",
        "sound_spl.obj",
        "sound_spl_cook.obj",
        "panning_lut.obj",
        "panning_lut_cook.obj",
        "sound_scene.obj",
        "sound_scene_environment.obj",
        "sound_scene_propagators.obj",
        "sound_scene_proxies.obj",
        "sound_scene_cook.obj",
        "debug_statistic.obj",
        "sound_debug_stats.obj",
        "search_service.obj",
        "sound_environment.obj",
        "sound_environment_cook.obj",
        "sound_entry_point.obj",
    ),
}


log = logger("regen-ninja")


def _normalize_link_rsp_paths(
    text: str,
    solution_dir: Path = SLN_PATH.parent,
    repo_dir: Path = VOSTOK_DIR,
) -> str:
    """Collapse vcproj output paths that cross above the solution directory.

    Wine's cmd does not resolve the mixed ``sources\\../`` spelling reliably:
    link can create an import library through it and then fail to reopen the
    adjacent .exp file.  Restrict this correction to link response files so
    compile command lines and their PDB identity remain untouched.
    """
    source = "Z:" + str(solution_dir)
    root = "Z:" + str(repo_dir) + "/"
    return text.replace(source + r"\../", root).replace(source + r"\/../", root)


def _normalize_compile_rsp_source_root(
    text: str,
    repo_dir: Path = VOSTOK_DIR,
) -> str:
    """Compile through retail's C:\\survarium\\sources include root.

    Only include switches move.  Object, PCH, and PDB outputs deliberately stay
    on the worktree's Z: path; rewriting the source-root substring globally
    would redirect ``sources\\../binaries`` outside the build tree.
    """
    local = "Z:" + str(repo_dir / "sources")
    retail = paths.RETAIL_INCLUDE_SOURCE_PREFIX.replace("\\", "/")
    return text.replace(f'/I "{local}', f'/I "{retail}')


def _normalize_compile_working_source_root(
    text: str,
    repo_dir: Path = VOSTOK_DIR,
) -> str:
    """Run ``cl`` from retail's source drive without moving other tools.

    Relative source and quoted-include paths are compiler input and therefore
    affect ``__FILE__``.  Library response files, however, contain object paths
    relative to the real checkout, so the ``lib`` and ``link`` rules must keep
    using ``proj_dir`` on Z:.
    """
    local = "Z:" + str(repo_dir / "sources")
    retail = paths.RETAIL_SOURCE_PREFIX.replace("\\", "/")
    marker = f"proj_dir = {local}"
    if marker not in text:
        return text

    lines = text.splitlines(keepends=True)
    project_dir = next(
        line.removeprefix("proj_dir = ").rstrip("\r\n")
        for line in lines
        if line.startswith("proj_dir = ")
    )
    if not project_dir.startswith(local):
        return text
    compile_dir = retail + project_dir[len(local):]

    for index, line in enumerate(lines):
        if line.startswith("proj_dir = "):
            ending = "\r\n" if line.endswith("\r\n") else "\n" if line.endswith("\n") else ""
            lines.insert(index + 1, f"compile_dir = {compile_dir}{ending}")
            break
    command = '  command = cmd /c cd "$proj_dir" && cl $flags'
    lines = [
        line.replace(command, '  command = cmd /c cd /d "$compile_dir" && cl $flags')
        for line in lines
    ]
    return "".join(lines)


def _normalize_link_rsp_library_order(text: str) -> str:
    """Reproduce the retail LTCG library order in the final executable link."""
    wanted = {name.casefold() for name in RETAIL_LINK_LIBRARY_ORDER}
    lines = text.splitlines(keepends=True)

    for index, line in enumerate(lines):
        words = line.split()
        names = [word.rsplit("/", 1)[-1].casefold() for word in words]
        if not wanted.issubset(names):
            continue

        positions = [i for i, name in enumerate(names) if name in wanted]
        if len(positions) != len(RETAIL_LINK_LIBRARY_ORDER):
            raise ValueError("shipping link response contains duplicate libraries")

        by_name = {
            name: word for name, word in zip(names, words, strict=True)
            if name in wanted
        }
        remaining = [
            word for name, word in zip(names, words, strict=True)
            if name not in wanted
        ]
        first = positions[0]
        ordered = [by_name[name.casefold()] for name in RETAIL_LINK_LIBRARY_ORDER]
        ending = "\n" if line.endswith("\n") else ""
        lines[index] = " ".join(remaining[:first] + ordered + remaining[first:]) + ending
        break

    return "".join(lines)


def _normalize_archive_member_order(text: str, order: tuple[str, ...]) -> str:
    """Reorder one evidenced member subset without guessing absent members."""
    wanted = {name.casefold() for name in order}
    lines = text.splitlines(keepends=True)
    positions: list[int] = []
    by_name: dict[str, str] = {}

    for index, line in enumerate(lines):
        token = line.strip().strip('"').replace("\\", "/")
        name = token.rsplit("/", 1)[-1].casefold()
        if name not in wanted:
            continue
        positions.append(index)
        if name in by_name:
            raise ValueError(f"archive response contains duplicate member {name}")
        by_name[name] = line

    if not wanted.issubset(by_name):
        return text
    for position, name in zip(positions, order, strict=True):
        lines[position] = by_name[name.casefold()]
    return "".join(lines)


def gen_fresh(
    out_dir: Path,
    target: str = "ninja",
    configuration: str = paths.GOLD_CONFIGURATION,
) -> None:
    exe = os.environ.get("VCPROJ2NINJA_EXE")
    if not exe:
        sys.exit("[regen-ninja] VCPROJ2NINJA_EXE not set - run from `nix develop`")
    if not SLN_PATH.is_file():
        sys.exit(f"[regen-ninja] solution not found: {SLN_PATH}")
    out_dir.mkdir(parents=True, exist_ok=True)
    # vcproj2ninja sometimes exits non-zero under wine even on success; trust the
    # produced output over the return code (same as vostok.tool.toolchain).
    subprocess.run(
        ["wine", exe, "--wine", "--target", target, "--sln-path", str(SLN_PATH),
         "--configuration-platform", configuration,
         "--output-dir", str(out_dir),
         "--project-name", "survarium - PC - DirectX 11"],
        check=False, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    probe = "build.ninja" if target == "ninja" else "compile_commands.json"
    if not (out_dir / probe).is_file():
        sys.exit(f"[regen-ninja] vcproj2ninja did not produce {probe}")


_LTO_FLAG = {
    "_cl_": re.compile(r" /GL(?=\s|$)"),
    "_lib": re.compile(r" /LTCG(?=\s|$)"),
    "_link": re.compile(r" /LTCG(?=\s|$)"),
}


def _strip_lto(name: str, text: str) -> str:
    """Drop whole-program optimization from one response file.

    vcproj2ninja renders `WholeProgramOptimization="1"` as `/GL` on every
    compile and `/LTCG` on the library and link steps. Removing both turns the
    configuration into an ordinary (per-TU) optimized build. The flag and its
    leading space go together, and the match ends on whitespace or end-of-file,
    so `/GLOBAL`-like flags are untouched and a trailing `/LTCG` (the lib rsps
    end on it, no newline) is caught too."""
    if "_cl_" in name:
        return _LTO_FLAG["_cl_"].sub("", text)
    if name.endswith("_lib.rsp"):
        return _LTO_FLAG["_lib"].sub("", text)
    if name.endswith("_link.rsp"):
        return _LTO_FLAG["_link"].sub("", text)
    return text


def regenerate(
    dry_run: bool = False,
    compdb: bool = False,
    configuration: str = paths.GOLD_CONFIGURATION,
    lto: bool = True,
) -> list[str]:
    """Regenerate and merge; return the relative paths that changed.

    The clangd inputs (COMPDB_FILES) are include-invariant, so they are only
    regenerated when something they DO depend on may have moved: a generated
    file appeared or went stale (TU/module added or removed), or they are
    missing entirely (fresh clone/worktree). Pass compdb=True to force them
    (e.g. after a flags-only .vcproj edit, which this trigger can't see).

    Everything below the retail-reproduction line - the C:\\survarium source
    root, the LTCG library order, the clangd inputs - belongs to the matching
    build alone. A non-gold configuration gets vcproj2ninja's plain output in
    its own graph dir (paths.ninja_dir), so building Release or Debug can never
    move the graph the ledger is measured against."""
    gold = configuration == paths.GOLD_CONFIGURATION
    if gold and not lto:
        sys.exit("[regen-ninja] the gold configuration is always LTO (retail is LTCG)")
    build_dir = paths.ninja_dir(configuration, lto=lto)
    changed: list[str] = []
    tu_set_changed = False
    with tempfile.TemporaryDirectory(prefix="ninja_regen_") as tmp:
        tmp_dir = Path(tmp)
        gen_fresh(tmp_dir, configuration=configuration)

        # The temp path appears in two spellings: raw in `flags = @...` lines,
        # ninja-escaped (`:` -> `$:`) in the rsp implicit-input dep lines.
        raw_t, raw_b = drive_path(tmp_dir), drive_path(build_dir)
        esc_t, esc_b = raw_t.replace(":", "$:"), raw_b.replace(":", "$:")

        fresh = sorted(p for p in tmp_dir.rglob("*") if p.is_file())
        for fp in fresh:
            rel = fp.relative_to(tmp_dir)
            text = fp.read_text().replace(raw_t, raw_b).replace(esc_t, esc_b)
            if fp.name.endswith("_link.rsp"):
                # A Wine `cmd` defect, not a retail detail: every configuration
                # needs it.
                text = _normalize_link_rsp_paths(text)
                if gold:
                    text = _normalize_link_rsp_library_order(text)
            elif gold and fp.name in RETAIL_ARCHIVE_MEMBER_ORDERS:
                text = _normalize_archive_member_order(
                    text, RETAIL_ARCHIVE_MEMBER_ORDERS[fp.name]
                )
            elif gold and fp.suffix == ".rsp" and "_cl_" in fp.name:
                text = _normalize_compile_rsp_source_root(text)
            elif gold and fp.suffix == ".ninja":
                text = _normalize_compile_working_source_root(text)
            if not lto and fp.suffix == ".rsp":
                text = _strip_lto(fp.name, text)
            dst = build_dir / rel
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
        # only: the graph dir also holds ninja state (.ninja_log) we must not
        # touch.
        fresh_set = {str(p.relative_to(tmp_dir)) for p in fresh}
        for p in sorted(build_dir.rglob("*")):
            if not p.is_file() or p.suffix not in (".ninja", ".rsp"):
                continue
            rel = str(p.relative_to(build_dir))
            if rel not in fresh_set:
                log(f"STALE (delete manually): {rel}")
                tu_set_changed = True

    verb = "would change" if dry_run else "changed"
    log(f"{verb}: {len(changed)} file(s)" if changed
        else "nothing to do - ninja graph already matches the .vcprojs/sources.")
    for rel in changed:
        log(f"  {rel}")

    if not gold:
        return changed
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
    ap.add_argument("--configuration", default="gold",
                    choices=sorted(paths.CONFIGURATIONS),
                    help="solution configuration to generate (default: gold, "
                         "the matching build)")
    ap.add_argument("--no-lto", dest="lto", action="store_false",
                    help="strip /GL and /LTCG (whole-program optimization) from "
                         "a non-gold graph, into its own -nolto dir")
    args = ap.parse_args()
    regenerate(dry_run=args.dry_run, compdb=args.compdb,
               configuration=paths.configuration(args.configuration),
               lto=args.lto)


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.build.ninja_regen", main))
