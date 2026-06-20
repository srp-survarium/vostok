#!/usr/bin/env python3
"""
build_gfx_lib_direct.py - Compile + lib one (or all) sibling GFx libs without ninja.

Generalizes build_libgfx_direct.py: ninja.exe under Wine deadlocks after ~70-80 cl
spawns, so the in-graph build of the big GFx libs never finishes. Each individual
`wine cmd /c cl @rsp` is reliable, so this driver runs them one at a time (skipping
TUs whose .obj already exists), then `lib`s the result, using the EXACT flags
vcproj2ninja generated (binaries/ninja/rsp/<lib>_cl_0.rsp + <lib>_lib.rsp) so the
bytes match the in-graph build.

Run inside `nix develop`. Idempotent: re-run to resume.

    python3 scripts/build_gfx_lib_direct.py              # all 8 (small first)
    python3 scripts/build_gfx_lib_direct.py libgfx_as3   # one
"""
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gfx_libs as G
from gfx_mspdbsrv import kill_mspdbsrv, wine_cl

VOSTOK = G.VOSTOK_DIR
RSP_DIR = VOSTOK / "binaries/ninja/rsp"
# small -> large so a config bug surfaces cheaply.
DEFAULT_ORDER = ["libgfxexpat", "libgfxsound_fmod", "libgfx_zlib", "libgfx_libpng",
                 "libgfx_libjpeg", "libgfx_as2", "libgfx_air", "libgfx_as3"]


def wine_lib(cmdline, cwd):
    """`lib` has no obj-completion sentinel, so just reap mspdbsrv after it."""
    env = dict(os.environ)
    env.setdefault("WINEDEBUG", "fixme-all,err-kerberos")
    r = subprocess.run(["wine", "cmd", "/c", cmdline], cwd=str(cwd), env=env,
                       capture_output=True, text=True)
    kill_mspdbsrv()
    return r


def build_one(name):
    proj_dir = VOSTOK / "sources/vostok" / name / "sources"
    obj_dir = VOSTOK / "binaries/Win32/intermediates/Master Gold" / name
    cl_rsp = RSP_DIR / f"{name}_cl_0.rsp"
    lib_rsp = RSP_DIR / f"{name}_lib.rsp"
    out_lib = VOSTOK / "binaries.prebuilt/Win32/libraries/shipping" / f"{name}.lib"
    if not cl_rsp.is_file():
        print(f"[{name}] SKIP - no {cl_rsp.name} (regen_ninja first?)")
        return 2

    text = cl_rsp.read_text()
    lines = text.splitlines()
    flags_line = lines[0]
    tus = [ln.strip().strip('"') for ln in lines[1:] if ln.strip().startswith('"')]
    print(f"[{name}] {len(tus)} TUs")

    per_tu_rsp = RSP_DIR / f"{name}_cl_onetu.rsp"
    per_tu_arg = "@Z:" + str(per_tu_rsp).replace("/", "\\")

    built = skipped = failed = 0
    fails = []
    for i, tu in enumerate(tus, 1):
        obj = obj_dir / (Path(tu.replace("\\", "/")).stem + ".obj")
        if obj.is_file() and obj.stat().st_size > 0:
            skipped += 1
            continue
        per_tu_rsp.write_text(f'{flags_line}\n"{tu}"\n')
        r = wine_cl(f"cl {per_tu_arg} /nologo /errorReport:prompt",
                    cwd=proj_dir, obj_path=obj)
        if obj.is_file() and obj.stat().st_size > 0:
            built += 1
            print(f"[{name}] [{i}/{len(tus)}] OK   {obj.name}")
        else:
            failed += 1
            fails.append(obj.name)
            errs = "\n".join(ln for ln in (r.stdout + r.stderr).splitlines()
                             if "error" in ln.lower() or "fatal" in ln.lower())
            print(f"[{name}] [{i}/{len(tus)}] FAIL {obj.name}\n{errs[:800]}")

    print(f"[{name}] built={built} skipped={skipped} failed={failed}")
    if fails:
        print(f"[{name}] FAILED:", ", ".join(fails[:20]))
        return 1

    print(f"[{name}] Linking {name}.lib ...")
    lib_arg = "@Z:" + str(lib_rsp).replace("/", "\\")
    r = wine_lib(f"lib {lib_arg} /NOLOGO", cwd=proj_dir)
    if out_lib.is_file():
        print(f"[{name}] {name}.lib: {out_lib.stat().st_size} bytes")
        return 0
    print(f"[{name}] LIB FAILED:\n" + (r.stdout + r.stderr)[:1200])
    return 1


def main():
    names = sys.argv[1:] or DEFAULT_ORDER
    rc = 0
    for n in names:
        r = build_one(n)
        if r == 1:
            print(f"\n*** {n} failed; stopping. ***")
            return 1
        rc = rc or r
    return rc


if __name__ == "__main__":
    sys.exit(main())
