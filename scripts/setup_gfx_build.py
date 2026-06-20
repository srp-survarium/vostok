#!/usr/bin/env python3
"""
setup_gfx_build.py - Stage the build-tree pieces the 8 sibling GFx libs need.

setup_libgfx_build.py already stages sources/scaleform_build/ = the FULL external
SDK (Src + Include + 3rdParty) overlaid with the repo's survarium patches, and
preamble-prepends libgfx's 212 TUs. This script adds, for the remaining 8 libs:

  * cpp libs (as2/as3/air/expat/sound_fmod): a per-lib pch.h (copy of libgfx's
    engine-memory preamble) under sources/vostok/<lib>/sources/, and a
    `#include "pch.h"` prepend on each of their build-tree TUs (idempotent).
  * c libs (zlib/libpng/libjpeg): NOTHING - plain C, no preamble, no pch. Their
    .c already live in scaleform_build/3rdParty/ from the libgfx stage.

Run setup_libgfx_build.py (--sdk ...) FIRST so scaleform_build/ exists.

    python3 scripts/setup_gfx_build.py
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gfx_libs as G

VOSTOK = G.VOSTOK_DIR
BUILD_SDK = VOSTOK / "sources" / "scaleform_build"
LIBGFX_PCH = VOSTOK / "sources" / "vostok" / "libgfx" / "sources" / "pch.h"
PREAMBLE = '#include "pch.h" // libgfx_preamble (engine memory env force-include)\n'


def main():
    if not BUILD_SDK.is_dir():
        raise SystemExit(f"{BUILD_SDK} missing - run setup_libgfx_build.py --sdk ... first")
    pch_template = LIBGFX_PCH.read_text()

    for name, cfg in G.LIBS.items():
        proj_dir = VOSTOK / "sources" / "vostok" / name / "sources"
        proj_dir.mkdir(parents=True, exist_ok=True)
        if cfg["kind"] != "cpp":
            continue  # C libs: no preamble, no pch

        # Per-lib pch.h: byte-identical to libgfx's engine-memory preamble, only the
        # include-guard macro is renamed so two libs can't double-guard if mixed.
        guard = f"VOSTOK_{name.upper()}_PCH_H_INCLUDED"
        pch = pch_template.replace("VOSTOK_LIBGFX_PCH_H_INCLUDED", guard)
        (proj_dir / "pch.h").write_text(pch)

        prepended = 0
        for rel in G.tus(name):
            tu = BUILD_SDK / rel
            if not tu.is_file():
                raise SystemExit(f"TU missing in build tree: {rel}")
            text = tu.read_text(errors="surrogateescape")
            if "libgfx_preamble" not in text.split("\n", 1)[0]:
                tu.write_text(PREAMBLE + text, errors="surrogateescape")
                prepended += 1
        print(f"{name}: pch.h staged, {prepended}/{len(G.tus(name))} TUs preamble-prepended")

    print("C libs (zlib/libpng/libjpeg): no preamble needed (plain C).")


if __name__ == "__main__":
    main()
