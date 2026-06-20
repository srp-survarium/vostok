"""
setup_libgfx_build.py - Regenerate the gitignored libgfx build SDK overlay tree.

The shipped survarium.exe statically links a GFx 4.2.21 lib suite Survarium built
themselves from their PATCHED 4.2.22 source with the SAME toolchain we match
(VS2008 / Msvc90). `sources/vostok/libgfx/sources/libgfx.vcproj` rebuilds
`libgfx.lib` from the 212 Win32 GFx TUs so the exe links a 4.2.22-ABI lib (the
prebuilt 4.0.15 one fails the 4.2.22 D3D1x HAL link with 44 LNK2001).

The TUs are upstream Scaleform source we do not edit, so we stage a private,
gitignored build tree (`sources/scaleform_build/`) = the full external SDK
(Src + Include + 3rdParty) overlaid with the repo's survarium patches (the only
libgfx-byte-affecting one is Src/Kernel/HeapMH/HeapMH_SysAllocMalloc.h, which
routes Win32 Alloc/Free/Realloc through vostok::memory::g_mt_allocator). Each TU
gets a `#include "pch.h"` prepended (the engine-memory + GFx-windows.h-env
preamble in sources/vostok/libgfx/sources/pch.h); vcproj2ninja rejects the
ForcedIncludeFiles attribute, so the include is prepended in-tree instead of /FI.

Usage:
    python3 scripts/setup_libgfx_build.py --sdk /path/to/scaleform_sdk
"""

import argparse
import shutil
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPT_DIR.parent
REPO_SDK = VOSTOK_DIR / "sources" / "scaleform"          # curated, patched 4.2.22
BUILD_SDK = VOSTOK_DIR / "sources" / "scaleform_build"   # gitignored overlay
PREAMBLE = '#include "pch.h" // libgfx_preamble (engine memory env force-include)\n'

# The 212 Win32 libgfx TUs (Src-relative). Ground truth: the Msvc10 GFx.vcxproj
# <ClCompile> set == libgfx.txt resolved for Win32 (they agree). Kept here so the
# build tree and the vcproj stay in lockstep without re-parsing either.
TU_LIST_FILE = SCRIPT_DIR / "libgfx_tus.txt"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--sdk", type=Path, required=True,
                    help="External GFx 4.2.22 SDK root (has Src/Include/3rdParty)")
    args = ap.parse_args()
    sdk = args.sdk.resolve()
    for sub in ("Src", "Include", "3rdParty"):
        if not (sdk / sub).is_dir():
            raise SystemExit(f"external SDK missing {sub}/: {sdk}")

    tus = [line.strip() for line in TU_LIST_FILE.read_text().splitlines() if line.strip()]
    if len(tus) != 212:
        raise SystemExit(f"expected 212 TUs in {TU_LIST_FILE}, got {len(tus)}")

    if BUILD_SDK.exists():
        shutil.rmtree(BUILD_SDK)
    BUILD_SDK.mkdir(parents=True)

    # 1. Full external SDK source + headers + 3rdParty.
    for sub in ("Src", "Include", "3rdParty"):
        shutil.copytree(sdk / sub, BUILD_SDK / sub)

    # 2. Overlay every repo survarium patch that exists in both trees and differs.
    patched = 0
    for f in sorted((REPO_SDK).rglob("*")):
        if not f.is_file() or f.suffix not in (".h", ".cpp"):
            continue
        rel = f.relative_to(REPO_SDK)
        ext = sdk / rel
        dst = BUILD_SDK / rel
        if ext.is_file() and dst.is_file() and f.read_bytes() != ext.read_bytes():
            shutil.copyfile(f, dst)
            patched += 1

    # 2b. HeapPT_SysAllocMalloc.cpp is NOT in the curated subset (only its .h is),
    # so the malloc/free in SysAllocPagedMalloc::Alloc/Free hit the engine's
    # void-trap #defines. Route them through the engine allocator, mirroring the
    # HeapMH_SysAllocMalloc.h patch (this TU's SysAllocPagedMalloc is /OPT:REF-dead
    # in the shipped exe - not separately addressed - so it is build-only).
    heappt = BUILD_SDK / "Src/Kernel/HeapPT/HeapPT_SysAllocMalloc.cpp"
    if heappt.is_file():
        t = heappt.read_text(errors="surrogateescape")
        t = t.replace(
            "void* ptr = malloc(size);",
            'void* ptr = VOSTOK_MALLOC_IMPL( vostok::memory::g_mt_allocator, size, "scaleform" );')
        t = t.replace(
            "    free(ptr);\n",
            "    VOSTOK_FREE_IMPL( vostok::memory::g_mt_allocator, ptr );\n", 1)
        heappt.write_text(t, errors="surrogateescape")

    # 3. Prepend the engine-memory preamble to each of the 212 build-tree TUs.
    for rel in tus:
        tu = BUILD_SDK / rel
        if not tu.is_file():
            raise SystemExit(f"TU missing in build tree: {rel}")
        text = tu.read_text(errors="surrogateescape")
        first_line = text.split("\n", 1)[0]
        if "libgfx_preamble" not in first_line:
            tu.write_text(PREAMBLE + text, errors="surrogateescape")

    print(f"libgfx build tree: {BUILD_SDK}")
    print(f"  external SDK staged + {patched} repo patches overlaid + {len(tus)} TUs preamble-prepended")


if __name__ == "__main__":
    main()
