#!/usr/bin/env python3
"""One-off: archive libgfx_as3 in N contiguous pieces.

Wine's wow64 address space can't hand `lib.exe` a contiguous ~2 GB block, which a
single 272-obj /GL archive needs (real Windows does this fine; it's a Wine
limit). The needed block scales with the input, so we split into N smaller
archives (libgfx_as3.lib, _b, _c, ...) each well under the wall, retrying on the
occasional ASLR-fragmentation miss. The exe linker merges all object sets at
LTCG time, so the final code is byte-identical to one archive; the exe pulls the
extra libs via added `#pragma comment(lib,"libgfx_as3_<x>.lib")`.
"""
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from gfx_mspdbsrv import kill_mspdbsrv  # noqa: E402

REPO = Path("/home/sheep/Projects/surv/vostok_4")
RSP = REPO / "binaries/ninja/rsp/libgfx_as3_lib.rsp"
PROJ = REPO / "sources/vostok/libgfx_as3/sources"
SHIP = REPO / "binaries.prebuilt/Win32/libraries/shipping"
N = int(sys.argv[1]) if len(sys.argv) > 1 else 4
RETRIES = 4

lines = RSP.read_text().splitlines()
out_line, objs = lines[0], [ln for ln in lines[1:] if ln.strip()]
# contiguous chunks
chunks = [objs[i * len(objs) // N:(i + 1) * len(objs) // N] for i in range(N)]
suffixes = [""] + [f"_{chr(ord('b') + i - 1)}" for i in range(1, N)]
print(f"as3: {len(objs)} objs -> {N} pieces of {[len(c) for c in chunks]}")


def run_lib(rsp):
    arg = "@Z:" + str(rsp).replace("/", "\\")
    env = dict(os.environ)
    env.setdefault("WINEDEBUG", "fixme-all,err-kerberos")
    for attempt in range(1, RETRIES + 1):
        r = subprocess.run(["wine", "cmd", "/c", f"lib {arg} /NOLOGO"],
                           cwd=str(PROJ), env=env, capture_output=True, text=True)
        kill_mspdbsrv()
        if r.returncode == 0:
            print(f"[{rsp.name}] OK (attempt {attempt})")
            return 0
        print(f"[{rsp.name}] rc={r.returncode} attempt {attempt}/{RETRIES}")
    print((r.stdout + r.stderr)[-400:])
    return r.returncode


rc = 0
libs = []
for sfx, chunk in zip(suffixes, chunks):
    lib = f"libgfx_as3{sfx}.lib"
    libs.append(lib)
    rsp = REPO / "binaries/ninja/rsp" / f"libgfx_as3_lib{sfx or '_a'}.rsp"
    rsp.write_text(out_line.replace("libgfx_as3.lib", lib) + "\n"
                   + "\n".join(chunk) + "\n")
    rc = run_lib(rsp) or rc

print("=== results ===")
for lib in libs:
    f = SHIP / lib
    sz = f"{f.stat().st_size/1e6:.0f} MB" if f.exists() else "MISSING"
    print(f"  {lib}: {sz}")
print("EXTRA_LIBS=" + " ".join(libs[1:]))  # the _b/_c/_d to wire into the exe link
sys.exit(rc)
