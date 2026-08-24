#!/usr/bin/env python3
"""
vostok.build.gfx - build the shipped Scaleform GFx static-lib suite from the
PRISTINE 4.2.22 SDK source, with the EXACT cl flags the shipped PDB records.

The shipped survarium.exe statically links a GFx lib suite Survarium built from
their 4.2.22 SDK with the same VS2008 / Msvc90 toolchain we match. Crucially the
PDB build records (`pdb_build_info --grep libgfx --full`) prove those libs were
built **non-/GL, /Ox, from PRISTINE SDK source** - a finished machine-code
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
plain prebuilts (un-wired from the sln); vostok.tool.libs skips the foreign 4.0.15
distribution libs so a setup pass never clobbers them.

Run inside `nix develop`. Idempotent: re-run to resume (skips objs already built).

    python3 -m vostok.build.gfx            # all 8 (small first)
    python3 -m vostok.build.gfx libgfx     # one
"""
import os
import subprocess
import sys
from pathlib import Path

from vostok.build.gfx_mspdbsrv import kill_mspdbsrv, wine_cl
from vostok.core.paths import GFX_BUILD_TREE, PREBUILT, SCALEFORM_SDK, WIN32_DIR
from vostok.core.paths import REPO as VOSTOK_DIR
from vostok.core.paths import GFX_TU_LISTS

SDK = SCALEFORM_SDK.resolve()
SHIP = PREBUILT / "Win32/libraries/shipping"
OBJ_ROOT = WIN32_DIR / "intermediates/gfx"

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

# The lib TUs compile from GFX_BUILD_TREE: the pristine SDK hardlinked
# file-by-file with the repo's reconstructed 4.2.21 files copied over it.
# That makes EVERY include style see the reconstructions - bare
# neighbor-includes ("Render_HAL.h") resolve inside the tree, which an -I
# overlay can never shadow. materialize_tree() below keeps it current.
#
# Overlay roots, applied in order (later wins):
#   sources/scaleform/{Src,Include}  - the vendored reconstruction tree
#                                      (engine-side TUs compile these same
#                                      files, so both sides agree)
#   sources/scaleform/sdk-overlay/   - lib-only shapes; GFxConfig.h maps to
#                                      Include/, the rest map under Src/
OVERLAY_ROOT = VOSTOK_DIR / "sources/scaleform"
OVERLAY_LIB_ONLY = OVERLAY_ROOT / "sdk-overlay"
OVERLAY_SKIP = {
    # engine-pch macro armor (parenthesized CRT calls, trimmed Realloc) - an
    # engine-side workaround, not a 4.2.21 truth; the lib compiles the
    # pristine SDK file.
    "Src/Kernel/HeapMH/HeapMH_SysAllocMalloc.h",
}


def wine_path(p: Path) -> str:
    return "Z:" + str(p).replace("/", "\\")


def _overlay_files():
    """Yield (rel_path_in_tree, source_file) for every reconstruction file."""
    for sub in ("Src", "Include"):
        root = OVERLAY_ROOT / sub
        if not root.is_dir():
            continue
        for f in root.rglob("*"):
            if not f.is_file():
                continue
            rel = str(Path(sub) / f.relative_to(root))
            if rel in OVERLAY_SKIP:
                continue
            yield rel, f
    if OVERLAY_LIB_ONLY.is_dir():
        for f in OVERLAY_LIB_ONLY.rglob("*"):
            if not f.is_file():
                continue
            r = f.relative_to(OVERLAY_LIB_ONLY)
            # map into the SDK layout: Include/ if the SDK has it there,
            # else under Src/
            rel = f"Include/{r}" if (SDK / "Include" / r).is_file() else f"Src/{r}"
            yield rel, f


def materialize_tree():
    """(Re)build GFX_BUILD_TREE: hardlink the pristine SDK, copy overlays over.

    Idempotent and cheap: SDK files are hardlinks (created once); an overlay
    file is re-copied only when its content is newer than the tree's. A file
    whose overlay was DELETED is re-linked back to the SDK (detected by inode:
    a tree file that is neither the SDK's inode nor overlay-fresh is stale).
    """
    from shutil import copy2
    tree = GFX_BUILD_TREE
    overlays = dict(_overlay_files())
    linked = copied = 0
    for sub in ("Src", "Include", "3rdParty"):
        src_root = SDK / sub
        for f in src_root.rglob("*"):
            if not f.is_file():
                continue
            rel = str(Path(sub) / f.relative_to(src_root))
            dst = tree / rel
            if rel in overlays:
                continue  # overlay pass handles it
            if dst.is_file():
                if dst.stat().st_ino == f.stat().st_ino:
                    continue
                dst.unlink()  # was an overlay copy; overlay is gone now
            dst.parent.mkdir(parents=True, exist_ok=True)
            os.link(f, dst)
            linked += 1
    for rel, src in overlays.items():
        dst = tree / rel
        if dst.is_file():
            st_d, st_s = dst.stat(), src.stat()
            sdk_f = SDK / rel
            is_link = sdk_f.is_file() and st_d.st_ino == sdk_f.stat().st_ino
            if not is_link and st_d.st_mtime >= st_s.st_mtime \
                    and st_d.st_size == st_s.st_size:
                continue
            dst.unlink()
        dst.parent.mkdir(parents=True, exist_ok=True)
        copy2(src, dst)
        copied += 1
    if linked or copied:
        print(f"[tree] {tree.name}: +{linked} sdk links, {copied} overlay copies")


def lib_config(name):
    if name in CPP_LIBS:
        return CPP_FLAGS, CPP_DEFINES, CPP_INCLUDES
    cfg = C_LIBS[name]
    return C_FLAGS, cfg["defines"], cfg["includes"]


def tus(name):
    f = GFX_TU_LISTS / f"{name}_tus.txt"
    return [ln.strip() for ln in f.read_text().splitlines() if ln.strip()]


def build_one(name):
    flags, defines, includes = lib_config(name)
    obj_dir = OBJ_ROOT / name
    obj_dir.mkdir(parents=True, exist_ok=True)
    SHIP.mkdir(parents=True, exist_ok=True)
    out_lib = SHIP / f"{name}.lib"

    inc_args = " ".join(f'-I"{wine_path(GFX_BUILD_TREE / d)}"' for d in includes)
    def_args = " ".join(f"-D{d}" for d in defines)
    fd_arg = f'-Fd"{wine_path(obj_dir)}\\vc90.pdb"'
    base = f"{flags} {inc_args} {def_args} {fd_arg}"

    tu_list = tus(name)
    print(f"[{name}] {len(tu_list)} TUs -> {out_lib.name}")
    rsp = obj_dir / "cl_onetu.rsp"
    rsp_arg = "@" + wine_path(rsp)

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
        fo = f'-Fo"{wine_path(obj)}"'
        rsp.write_text(f'{base} {fo}\n"{wine_path(src)}"\n')
        r = wine_cl(f"cl {rsp_arg}", cwd=obj_dir, obj_path=obj)
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
    lib_rsp.write_text(f'-out:"{wine_path(out_lib)}"\n'
                       + "\n".join(f'"{wine_path(o)}"' for o in objs) + "\n")
    env = dict(os.environ)
    env.setdefault("WINEDEBUG", "fixme-all,err-kerberos")
    r = subprocess.run(["wine", "cmd", "/c", f"lib -nologo @{wine_path(lib_rsp)}"],
                       cwd=str(obj_dir), env=env, capture_output=True, text=True)
    kill_mspdbsrv()
    if out_lib.is_file() and out_lib.stat().st_size > 0:
        print(f"[{name}] {out_lib.name}: {out_lib.stat().st_size/1e6:.1f} MB")
        return 0
    print(f"[{name}] LIB FAILED:\n{(r.stdout + r.stderr)[:1500]}")
    return 1


def main():
    if not SDK.is_dir():
        raise SystemExit(f"pristine SDK not found: {SDK} (set SCALEFORM_SDK)")
    materialize_tree()
    names = sys.argv[1:] or DEFAULT_ORDER
    for n in names:
        if build_one(n) == 1:
            print(f"\n*** {n} failed; stopping. ***")
            return 1
    print("\nAll libs built.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
