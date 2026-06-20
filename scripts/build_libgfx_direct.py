"""
build_libgfx_direct.py - Compile + lib the 212 libgfx TUs without ninja.

ninja.exe under Wine deadlocks after ~70-80 cl spawns (it sleeps with no live
child and never makes progress), so the in-graph libgfx build never finishes.
Each INDIVIDUAL `wine cl` invocation is reliable, so this driver runs them one at
a time (skipping TUs whose .obj already exists), then `lib`s the result. It uses
the EXACT flags vcproj2ninja generated (binaries/ninja/rsp/libgfx_cl_0.rsp +
libgfx_lib.rsp) so the bytes match the in-graph build.

Run inside `nix develop`. Idempotent: re-run to pick up where it stopped.
"""

import os
import subprocess
import sys
from pathlib import Path

VOSTOK = Path("/home/sheep/Projects/surv/vostok_4")
PROJ_DIR = VOSTOK / "sources/vostok/libgfx/sources"
RSP_DIR = VOSTOK / "binaries/ninja/rsp"
OBJ_DIR = VOSTOK / "binaries/Win32/intermediates/Master Gold/libgfx"
CL_RSP = RSP_DIR / "libgfx_cl_0.rsp"
LIB_RSP = RSP_DIR / "libgfx_lib.rsp"


def wine_cmd(cmdline, cwd):
    """Invoke a Windows tool the way ninja does: `wine cmd /c <cmdline>`.

    The bare `wine cl ...` form routes through start.exe, which deadlocks under
    this Wine after many spawns; `cmd /c` launches the tool directly and is what
    the working ninja graph uses.
    """
    env = dict(os.environ)
    env.setdefault("WINEDEBUG", "fixme-all,err-kerberos")
    return subprocess.run(["wine", "cmd", "/c", cmdline], cwd=str(cwd), env=env,
                          capture_output=True, text=True)


def main():
    text = CL_RSP.read_text()
    # The rsp is: <flags...> then one quoted "..\\..\\..\\scaleform_build\\...cpp"
    # per line. Split flags (first line) from the TU list (the quoted lines).
    lines = text.splitlines()
    flags_line = lines[0]
    tus = [ln.strip().strip('"') for ln in lines[1:] if ln.strip().startswith('"')]
    print(f"{len(tus)} TUs in rsp")

    # Per-TU: write a complete rsp (flags + the one quoted TU line, exactly as
    # the original rsp quotes it) and `cl @rsp`. This is the only quoting that
    # survives the `cmd /c` layer cleanly (passing the path on the command line
    # double-escapes the quotes -> C1083 "Invalid argument").
    per_tu_rsp = RSP_DIR / "libgfx_cl_onetu.rsp"
    per_tu_arg = "@Z:" + str(per_tu_rsp).replace("/", "\\")

    built = skipped = failed = 0
    fails = []
    for i, tu in enumerate(tus, 1):
        obj_name = Path(tu.replace("\\", "/")).stem + ".obj"
        obj = OBJ_DIR / obj_name
        if obj.is_file() and obj.stat().st_size > 0:
            skipped += 1
            continue
        per_tu_rsp.write_text(f'{flags_line}\n"{tu}"\n')
        r = wine_cmd(f"cl {per_tu_arg} /nologo /errorReport:prompt", cwd=PROJ_DIR)
        if obj.is_file() and obj.stat().st_size > 0:
            built += 1
            print(f"[{i}/{len(tus)}] OK   {obj_name}")
        else:
            failed += 1
            fails.append(obj_name)
            errs = "\n".join(ln for ln in (r.stdout + r.stderr).splitlines()
                             if "error" in ln.lower() or "fatal" in ln.lower())
            print(f"[{i}/{len(tus)}] FAIL {obj_name}\n{errs[:600]}")

    print(f"\nbuilt={built} skipped={skipped} failed={failed}")
    if fails:
        print("FAILED:", ", ".join(fails))
        return 1

    # All objs present -> lib them.
    print("Linking libgfx.lib ...")
    lib_arg = "@Z:" + str(LIB_RSP).replace("/", "\\")
    r = wine_cmd(f"lib {lib_arg} /NOLOGO", cwd=PROJ_DIR)
    out = VOSTOK / "binaries.prebuilt/Win32/libraries/shipping/libgfx.lib"
    if out.is_file():
        print(f"libgfx.lib: {out.stat().st_size} bytes")
        return 0
    print("LIB FAILED:\n" + (r.stdout + r.stderr)[:1000])
    return 1


if __name__ == "__main__":
    sys.exit(main())
