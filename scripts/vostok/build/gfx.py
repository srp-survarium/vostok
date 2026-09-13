#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.build.gfx - build the shipped Scaleform GFx static-lib suite from the
pinned recovered 4.2.21 SDK source, with the EXACT cl flags the shipped PDB records.

The shipped survarium.exe statically links a GFx lib suite Survarium built from
their 4.2.21 SDK with the same VS2008 / Msvc90 toolchain we match. Crucially the
PDB build records (`pdb_build_info --grep libgfx --full`) prove those libs were
built **non-/GL, /Ox, from SDK source** - a finished machine-code
prebuilt staged to C:\\survarium\\binaries.prebuilt\\ (like boost/openssl), NOT
in the engine whole-program LTCG. So:

  * NO /GL  -> objects are machine code, `lib` just archives them cheaply (no
    2GB-contiguous-block OOM under Wine, no as3 4-way split, one archive per lib).
  * NO survarium patches (no HeapMH g_mt_allocator routing - the engine feeds GFx
    an allocator at runtime via Scaleform::SysAlloc / GFx::System::Init, see
    engine_scaleform_initialize.cpp), NO engine pch.h prepend (the fake-HANDLE
    macro problem that motivated the pch came from survarium's extensions.h, which
    a standalone build never includes), NO /Od, NO MASTER_GOLD,
    NO VOSTOK_STATIC_LIBRARIES, NO /arch.

This driver authors a per-lib `cl` response file straight from the PDB recipe and
compiles each TU directly under Wine (`wine cmd /c cl @rsp`), then `lib`s the
objects - NO vcproj2ninja / sln / ninja involvement at all. ninja.exe under Wine
deadlocks after ~70-80 cl spawns, and the in-graph /GL libs were the whole
problem; a direct machine-code build sidesteps both.

Flags (verified per-lib via pdb_build_info; libgfx/as2/as3 share one config):
  C++ libs (libgfx, libgfx_as2, libgfx_as3):
    -Ox -Ob2 -Ot -Zp8 -Z7 -MT -W4 -GS- -Gy -GR- -GF -WX -Zl -FD -MP
    -errorreport:none -TP  + the -w44### warning enables
    defines: WIN32 _WINDOWS SF_BUILD_STATICLIB NDEBUG SF_BUILD_SHIPPING _MBCS
             _VC80_UPGRADE=0x0710
  C libs (libgfx_zlib, libgfx_libpng, libgfx_libjpeg, libgfxexpat, pcre):
    plain Release C static libs (the shipped PDB carries no debug cmd for them -
    they are not engine-matchable; they only need to resolve the symbols the C++
    archives pull). /O2 /MT, -TC, with the SDK projects' release defines.

System includes (VC / WinSDK / DXSDK) come from Wine's %INCLUDE% (set in the Wine
registry by vostok.tool.toolchain), so the rsp lists only the GFx-specific -I dirs;
the PDB's explicit -X + system -I list is equivalent (same headers found).

Output: binaries.prebuilt/Win32/libraries/shipping/<name>.lib - exactly where the
exe's `#pragma comment(lib,"<name>.lib")` resolves it. The exe links these as
plain prebuilts (un-wired from the sln). The same suite ships prebuilt inside
the vostok-libs release, which vostok.tool.libs stages here on FIRST toolchain
setup (it overwrites; local rebuilds after that are never restaged over, but
cut a fresh vostok-libs release when the suite changes so new clones link the
current libs).

Run inside `nix develop`. Idempotent: re-run to resume (skips objs already built).

    python3 -m vostok.build.gfx            # all 8 (small first)
    python3 -m vostok.build.gfx libgfx     # one
"""
import errno
import hashlib
import json
import os
import subprocess
import sys
from pathlib import Path

from vostok.build.gfx_mspdbsrv import (
    fixed_time_command,
    fixed_time_env,
    kill_mspdbsrv,
    stop_wine_session,
    wine_cl,
)
from vostok.core.paths import (
    GFX_BUILD_TREE,
    GFX_OBJECT_PREFIX,
    GFX_OBJECT_TREE,
    GFX_TREE_PREFIX,
    PREBUILT,
    SCALEFORM_SDK,
)
from vostok.core.paths import GFX_TU_LISTS
from vostok.core.wine import drive_path
from vostok.core import log as _log

SDK = SCALEFORM_SDK.resolve()
SHIP = PREBUILT / "Win32/libraries/shipping"
OBJ_ROOT = GFX_OBJECT_TREE

# The retail game link already uses this reconstructed build date. Reusing it
# for the separately-built GFx objects and archives removes host wall-clock
# bytes without inventing another timestamp contract.
GFX_BUILD_TIME = "2013-05-09 12:00:00"

# Shared C++ flags (from the libgfx/as2/as3 PDB cmd; -Fo/-Fd are per-lib).
CPP_FLAGS = (
    "-nologo -Ox -Ob2 -Ot -Zp8 -Z7 -MT -W4 -GS- -Gy -GR- -GF -WX -Zl "
    "-errorreport:none -FD -c -MP "
    "-w44264 -w44062 -w44265 -w44287 -w44289 -w44296 -w44431 -w44545 -w44546 "
    "-w44547 -w44548 -w44549 -w44623 "
    "-TP"
)
CPP_DEFINES = ("WIN32", "_WINDOWS", "SF_BUILD_STATICLIB", "NDEBUG",
               "SF_BUILD_SHIPPING", "_MBCS", "_VC80_UPGRADE=0x0710")
# GFx -I dirs (PDB order). System VC/WinSDK/DXSDK come from %INCLUDE%.
CPP_INCLUDES = [
    "Include", "Src",
    "3rdParty/zlib-1.2.7", "3rdParty/jpeg-8d", "3rdParty/libpng-1.5.13",
    "3rdParty/expat-2.1.0/lib", "3rdParty/pcre", "3rdParty/glext",
    "3rdParty/PlatformSDK", "3rdParty/cri/pc/include", "3rdParty/fmod/pc/Win32/inc",
]

# C libs: plain Release C static lib. The PDB carries no debug cmd for these and
# they are not engine-matchable; they only resolve symbols the C++ archives pull.
C_LIBS = {
    "libgfx_zlib":    {"defines": ("WIN32", "NDEBUG", "_MBCS"),
                       "includes": ["3rdParty/zlib-1.2.7"]},
    "libgfx_libpng":  {"defines": ("WIN32", "NDEBUG", "_MBCS"),
                       "includes": ["3rdParty/libpng-1.5.13", "3rdParty/zlib-1.2.7"]},
    "libgfx_libjpeg": {"defines": ("WIN32", "NDEBUG", "_LIB",
                                   "_CRT_SECURE_NO_WARNINGS", "_MBCS"),
                       "includes": ["3rdParty/jpeg-8d"]},
    "libgfxexpat":    {"defines": ("WIN32", "NDEBUG", "COMPILED_FROM_DSP"),
                       "includes": ["3rdParty/expat-2.1.0/lib"]},
    "pcre":           {"defines": ("WIN32", "NDEBUG", "HAVE_CONFIG_H",
                                   "_CRT_SECURE_NO_WARNINGS"),
                       "includes": ["3rdParty/pcre"]},
}
C_FLAGS = "-nologo -O2 -Ob2 -Ot -MT -GS- -W3 -Z7 -FD -c -MP -errorreport:none -TC"

CPP_LIBS = ("libgfx", "libgfx_as2", "libgfx_as3")
DEFAULT_ORDER = ["libgfx_zlib", "libgfx_libpng", "libgfx_libjpeg",
                 "libgfxexpat", "pcre",
                 "libgfx_as2", "libgfx_as3", "libgfx"]

def tree_path(rel) -> str:
    """A GFX_BUILD_TREE path as the compiler must see it: through the neutral
    C:\\survarium\\gfx-sdk alias, so objects record GFX_TREE_PREFIX, not the
    checkout's Z: path (vostok.tool.toolchain creates the alias)."""
    return GFX_TREE_PREFIX + "\\" + str(rel).replace("/", "\\")


def object_path(rel) -> str:
    """A generated GFx object path through its machine-independent alias."""
    return GFX_OBJECT_PREFIX + "\\" + str(rel).replace("/", "\\")


def materialize_tree():
    """Link the recovered SDK through the stable compiler path, removing stale files."""
    tree = GFX_BUILD_TREE
    if not all((SDK / sub).is_dir() for sub in ("Src", "Include", "3rdParty")):
        raise RuntimeError(f"recovered SDK not found: {SDK} (enter nix develop or set SCALEFORM_SDK)")
    expected = set()
    linked = 0
    for sub in ("Src", "Include", "3rdParty"):
        for source in (SDK / sub).rglob("*"):
            if not source.is_file():
                continue
            relative = source.relative_to(SDK)
            expected.add(relative)
            destination = tree / relative
            if destination.is_file() and destination.samefile(source):
                continue
            destination.unlink(missing_ok=True)
            destination.parent.mkdir(parents=True, exist_ok=True)
            try:
                os.link(source, destination)
            except OSError as error:
                if error.errno not in {errno.EXDEV, errno.EPERM, errno.EROFS}:
                    raise
                destination.symlink_to(source)
            linked += 1
    for sub in ("Src", "Include", "3rdParty"):
        for destination in (tree / sub).rglob("*"):
            if (destination.is_file() or destination.is_symlink()) and destination.relative_to(tree) not in expected:
                destination.unlink()
    if linked:
        print(f"[tree] {tree.name}: +{linked} SDK links")


def lib_config(name):
    if name in CPP_LIBS:
        return CPP_FLAGS, CPP_DEFINES, CPP_INCLUDES
    cfg = C_LIBS[name]
    return C_FLAGS, cfg["defines"], cfg["includes"]


def tus(name):
    f = GFX_TU_LISTS / f"{name}_tus.txt"
    return [ln.strip() for ln in f.read_text().splitlines() if ln.strip()]


def build_key(name, flags, defines, includes, tu_list):
    """Key objects on the pinned SDK, compiler recipe and fixed provenance."""
    inputs = {
        "version": 3,
        "sdk": str(SDK),
        "build_time": GFX_BUILD_TIME,
        "object_prefix": GFX_OBJECT_PREFIX,
        "flags": flags,
        "defines": list(defines),
        "includes": list(includes),
        "tus": tu_list,
    }
    # An explicit development checkout is mutable; store inputs are immutable.
    if not str(SDK).startswith("/nix/store/"):
        inputs["sources"] = [
            [str(source.relative_to(SDK)), hashlib.sha256(source.read_bytes()).hexdigest()]
            for sub in ("Src", "Include", "3rdParty")
            for source in sorted((SDK / sub).rglob("*")) if source.is_file()
        ]
    payload = json.dumps(inputs, sort_keys=True, separators=(",", ":")).encode()
    return hashlib.sha256(payload).hexdigest()


def build_one(name):
    flags, defines, includes = lib_config(name)
    obj_dir = OBJ_ROOT / name
    obj_dir.mkdir(parents=True, exist_ok=True)
    SHIP.mkdir(parents=True, exist_ok=True)
    out_lib = SHIP / f"{name}.lib"

    inc_args = " ".join(f'-I"{tree_path(d)}"' for d in includes)
    def_args = " ".join(f"-D{d}" for d in defines)
    fd_arg = f'-Fd"{object_path(Path(name) / "vc90.pdb")}"'
    base = f"{flags} {inc_args} {def_args} {fd_arg}"

    tu_list = tus(name)
    print(f"[{name}] {len(tu_list)} TUs -> {out_lib.name}")

    cache_key = build_key(name, flags, defines, includes, tu_list)
    cache_stamp = obj_dir / "build-key.sha256"
    old_key = cache_stamp.read_text().strip() if cache_stamp.is_file() else ""
    if old_key != cache_key:
        invalidated = 0
        for rel in tu_list:
            obj = obj_dir / (Path(rel).stem + ".obj")
            if obj.is_file():
                obj.unlink()
                invalidated += 1
        print(f"[{name}] source/config key changed: invalidated {invalidated} object(s)")

    rsp = obj_dir / "cl_onetu.rsp"
    rsp_arg = "@" + drive_path(rsp)

    built = skipped = failed = 0
    fails, objs = [], []
    for i, rel in enumerate(tu_list, 1):
        src = GFX_BUILD_TREE / rel
        if not src.is_file():
            print(f"[{name}] [{i}] MISSING SRC {rel}")
            failed += 1
            fails.append(rel)
            continue
        obj = obj_dir / (Path(rel).stem + ".obj")
        objs.append(obj)
        if obj.is_file() and obj.stat().st_size > 0:
            skipped += 1
            continue
        fo = f'-Fo"{object_path(Path(name) / obj.name)}"'
        rsp.write_text(f'{base} {fo}\n"{tree_path(rel)}"\n')
        r = wine_cl(
            f"cl {rsp_arg}",
            cwd=obj_dir,
            obj_path=obj,
            build_time=GFX_BUILD_TIME,
        )
        if obj.is_file() and obj.stat().st_size > 0:
            built += 1
            print(f"[{name}] [{i}/{len(tu_list)}] OK   {obj.name}")
        else:
            failed += 1
            fails.append(obj.name)
            errs = "\n".join(ln for ln in r.stdout.splitlines()
                             if "error" in ln.lower() or "fatal" in ln.lower())
            print(f"[{name}] [{i}/{len(tu_list)}] FAIL {obj.name}\n{errs[:1200]}")

    print(f"[{name}] built={built} skipped={skipped} failed={failed}")
    if fails:
        print(f"[{name}] FAILED: {', '.join(str(f) for f in fails[:20])}")
        return 1

    print(f"[{name}] archiving {out_lib.name} ...")
    out_lib.unlink(missing_ok=True)
    lib_rsp = obj_dir / "lib.rsp"
    lib_rsp.write_text(f'-out:"{drive_path(out_lib)}"\n'
                       + "\n".join(f'"{drive_path(o)}"' for o in objs) + "\n")
    env = fixed_time_env(GFX_BUILD_TIME, fake_stat=True)
    command = ["wine", "cmd", "/c", f"lib -nologo @{drive_path(lib_rsp)}"]
    r = subprocess.run(fixed_time_command(command, GFX_BUILD_TIME, freeze=True),
                       cwd=str(obj_dir), env=env, capture_output=True, text=True)
    kill_mspdbsrv()
    if out_lib.is_file() and out_lib.stat().st_size > 0:
        # Keep dependency tools on the real clock; only archive contents are fixed.
        os.utime(out_lib, None)
        cache_stamp.write_text(cache_key + "\n")
        print(f"[{name}] {out_lib.name}: {out_lib.stat().st_size/1e6:.1f} MB")
        return 0
    print(f"[{name}] LIB FAILED:\n{(r.stdout + r.stderr)[:1500]}")
    return 1


def main():
    if not SDK.is_dir():
        raise SystemExit(f"recovered SDK not found: {SDK} (set SCALEFORM_SDK)")
    stop_wine_session(GFX_BUILD_TIME)
    try:
        materialize_tree()
        names = sys.argv[1:] or DEFAULT_ORDER
        for n in names:
            if build_one(n) == 1:
                print(f"\n*** {n} failed; stopping. ***")
                return 1
        print("\nAll libs built.")
        return 0
    finally:
        stop_wine_session(GFX_BUILD_TIME)


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.build.gfx", main))
