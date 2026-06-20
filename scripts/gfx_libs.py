"""
gfx_libs.py - Shared config for the 9-lib Scaleform GFx suite rebuild.

The shipped survarium.exe statically links a GFx 4.2.21 lib suite Survarium built
themselves from their PATCHED 4.2.22 source with the SAME toolchain we match
(VS2008 / Msvc90). The pilot rebuilt the base `libgfx.lib`; this module generalizes
that recipe to the remaining 8 libs (gen_gfx_lib_vcproj / setup_gfx_build /
build_gfx_lib_direct all import LIBS from here).

Two flavors:
  * GFx C++ libs (as2/as3/air/expat/sound_fmod): SAME config as libgfx - the GFx
    Shipping flags AND the survarium MASTER_GOLD module flags together (/Od /GL,
    SF_BUILD_STATICLIB;SF_BUILD_SHIPPING;NDEBUG;VOSTOK_STATIC_LIBRARIES;MASTER_GOLD),
    full-split, /Z7, and the engine-memory `#include "pch.h"` preamble prepended to
    each upstream TU (vcproj2ninja rejects ForcedIncludeFiles).
  * 3rd-party C libs (zlib/libpng/libjpeg): their OWN Msvc10 Release|Win32 config -
    plain C static libs, /MT, NO /GL, NO SF defines, NO MASTER_GOLD, NO preamble.
    libpng needs the zlib include dir; libjpeg defines _LIB;_CRT_SECURE_NO_WARNINGS.

Each lib gets sources/vostok/<name>/sources/<name>.vcproj, is registered in
`vostok v2.0.sln`, and game_core takes a ProjectDependencies edge on it (puts it in
the exe build cone so vcproj2ninja emits its .ninja + rsp). OutputFile is
binaries.prebuilt/Win32/libraries/shipping/<name>.lib - exactly where the exe's
`#pragma comment(lib,...)` resolves it - overwriting the foreign 4.0.15 prebuilt.
"""

from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPT_DIR.parent

# Deterministic per-lib GUIDs (libgfx's is in gen_libgfx_vcproj.py; reuse it there).
GUIDS = {
    "libgfx_as2":      "{C1A2B3D4-1111-4A18-9C56-2D8F1B3E6401}",
    "libgfx_as3":      "{C1A2B3D4-2222-4A18-9C56-2D8F1B3E6402}",
    "libgfx_air":      "{C1A2B3D4-3333-4A18-9C56-2D8F1B3E6403}",
    "libgfxexpat":     "{C1A2B3D4-4444-4A18-9C56-2D8F1B3E6404}",
    "libgfxsound_fmod":"{C1A2B3D4-5555-4A18-9C56-2D8F1B3E6405}",
    "libgfx_zlib":     "{C1A2B3D4-6666-4A18-9C56-2D8F1B3E6406}",
    "libgfx_libpng":   "{C1A2B3D4-7777-4A18-9C56-2D8F1B3E6407}",
    "libgfx_libjpeg":  "{C1A2B3D4-8888-4A18-9C56-2D8F1B3E6408}",
}

# kind: "cpp" = GFx Shipping + MASTER_GOLD config (libgfx recipe, preamble).
#       "c"   = plain 3rd-party C Release static lib (own vcxproj flags, no preamble).
LIBS = {
    "libgfx_as2":      {"kind": "cpp"},
    "libgfx_as3":      {"kind": "cpp"},
    "libgfx_air":      {"kind": "cpp"},
    "libgfxexpat":     {"kind": "cpp"},
    "libgfxsound_fmod":{"kind": "cpp"},
    # zlib: MaxSpeed(/O2) /Ob1, WIN32;NDEBUG, /MT, /GF /Gy, no RTTI.
    "libgfx_zlib":     {"kind": "c",
                        "opt": "2", "inline": "1",
                        "defines": "WIN32;NDEBUG",
                        "extra_includes": []},
    # libpng: no /O (compiler default), /Ob1, WIN32;NDEBUG, needs zlib include.
    "libgfx_libpng":   {"kind": "c",
                        "opt": None, "inline": "1",
                        "defines": "WIN32;NDEBUG",
                        "extra_includes": ["$(SolutionDir)/scaleform_build/3rdParty/zlib-1.2.7"]},
    # libjpeg: MaxSpeed(/O2) /Ob1, WIN32;NDEBUG;_LIB;_CRT_SECURE_NO_WARNINGS, /MT.
    "libgfx_libjpeg":  {"kind": "c",
                        "opt": "2", "inline": "1",
                        "defines": "WIN32;NDEBUG;_LIB;_CRT_SECURE_NO_WARNINGS",
                        "extra_includes": []},
}

# Include dirs for the cpp libs (identical to libgfx; mirror GFx_Lib.vsprops roots
# pointed at the build SDK + engine headers + stlport + 3rdParty).
CPP_INCLUDES = [
    "$(SolutionDir)/stlport",
    "$(SolutionDir)",
    "$(SolutionDir)/scaleform_build/Include",
    "$(SolutionDir)/scaleform_build/Src",
    "$(SolutionDir)/scaleform_build/3rdParty/zlib-1.2.7",
    "$(SolutionDir)/scaleform_build/3rdParty/jpeg-8d",
    "$(SolutionDir)/scaleform_build/3rdParty/libpng-1.5.13",
    "$(SolutionDir)/scaleform_build/3rdParty/expat-2.1.0/lib",
    "$(SolutionDir)/scaleform_build/3rdParty/pcre",
    "$(SolutionDir)/scaleform_build/3rdParty/glext",
    "$(SolutionDir)/scaleform_build/3rdParty/PlatformSDK",
    "$(SolutionDir)/scaleform_build/3rdParty/fmod/pc/Win32/inc",
]

# Base 3rdParty include for the C libs (the lib's own dir is the SDK 3rdParty root).
C_BASE_INCLUDES = ["$(SolutionDir)/scaleform_build/3rdParty"]


def tus(name):
    """Build-tree-relative TU list for a lib (Src/... or 3rdParty/...)."""
    return [t for t in (SCRIPT_DIR / f"{name}_tus.txt").read_text().split() if t]


def includes(name):
    cfg = LIBS[name]
    if cfg["kind"] == "cpp":
        return CPP_INCLUDES
    # C lib: its own dir + zlib (libpng) + the engine/build roots are not needed.
    base = ["$(SolutionDir)/scaleform_build/3rdParty/" + _c_subdir(name)]
    return base + cfg.get("extra_includes", [])


def _c_subdir(name):
    # the 3rdParty sub-tree the lib's .c live under (first TU's parent under 3rdParty/)
    first = tus(name)[0]            # e.g. 3rdParty/zlib-1.2.7/adler32.c
    parts = first.split("/")
    return parts[1]                # zlib-1.2.7 / libpng-1.5.13 / jpeg-8d
