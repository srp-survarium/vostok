# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.shaders - the shipped shader blobs, and the proof we can rebuild them.

resources.db ships every shader as `shaders/sm_4_0/<name>.{vs,ps,gs}/<perm>`,
where <perm> is the define vector (all underscores = no define set). Each blob
is a dependency table followed by a raw DXBC container:

    u32 dep_count
    dep_count x { char name[260]; u32 unix_mtime }   # include closure; entry 0
                                                     # is the shader itself,
                                                     # under resources.sources/
    DXBC ...                                         # to end of file

Build 802 cannot compile HLSL at runtime - shader_binary_source_cook's
recompile arm is compiled out of Master Gold - so these blobs are the only
shader truth the client accepts, and authoring a shader means producing a blob
it would have produced itself.

The blobs name their own recipe. The RDEF chunk's creator string is
"Microsoft (R) HLSL Shader Compiler 9.29.952.3111" (June 2010 DirectX SDK,
D3DCompiler_43), and its flags word is 0x8108: PACK_MATRIX_ROW_MAJOR (0x8) |
NO_PRESHADER (0x100, fxc adds it for 4_0 targets on its own) |
OPTIMIZATION_LEVEL3 (0x8000). Note the engine's own shader_compiler source
passes only ROW_MAJOR - the shipped blobs were built at /O3 regardless, so the
blobs outrank the tool source here.

`roundtrip` recompiles a shader from resources/sources/shaders/ with exactly
that compiler (fxc.exe under Wine, the native D3DCompiler_43.dll beside it -
both from the flake's dxsdk-shader-compiler package) and byte-compares the
DXBC against the shipped blob. The recovered tree is original dev source (it
arrived with the repo's initial source drop, comments and dead code intact),
but it is ONE snapshot and the blobs were compiled from another, so identity
doubles as per-file authentication. First sweep over the plain permutations:
33 identical (that file is the ship revision), 47 DIFF (revision drift -
clouds.ps in the tree is a 692-byte stub, the shipped compile is 5,280).

Scope: only all-underscore permutations are compiled today. The permutation
string is a concatenation of per-define value strings in registry order
(shaders/masks maps effect names to the define names each stage consumes);
decoding that mapping is the next step, and until then a digit-bearing
permutation is reported as `skip`.
"""

from __future__ import annotations

import os
import struct
import subprocess
import tempfile
from pathlib import Path

from vostok.core import paths

SM_DIR = "sm_4_0"
PROFILES = {".ps": "ps_4_0", ".vs": "vs_4_0", ".gs": "gs_4_0"}


def resources_root(override: str | None = None) -> Path:
    """The unpacked resources tree (blobs live under shaders/sm_4_0/)."""
    root = override or os.environ.get("VOSTOK_RESOURCES_UNPACKED")
    if not root:
        raise SystemExit("no --resources and VOSTOK_RESOURCES_UNPACKED unset "
                         "(enter `nix develop .#with-resources`)")
    return Path(root)


def fxc_dir(override: str | None = None) -> Path:
    """Directory holding fxc.exe + native D3DCompiler_43.dll."""
    d = override or os.environ.get("DXSDK_SHADER_COMPILER")
    if not d:
        raise SystemExit("no --fxc-dir and DXSDK_SHADER_COMPILER unset "
                         "(re-enter the dev shell to pick up the flake export)")
    return Path(d)


def parse_blob(path: Path) -> tuple[list[tuple[str, int]], bytes]:
    """Split a shipped blob into its dependency table and DXBC payload."""
    data = path.read_bytes()
    (count,) = struct.unpack_from("<I", data, 0)
    deps: list[tuple[str, int]] = []
    off = 4
    for _ in range(count):
        raw = data[off:off + 260]
        name = raw.split(b"\x00", 1)[0].decode("ascii")
        (mtime,) = struct.unpack_from("<I", data, off + 260)
        deps.append((name, mtime))
        off += 264
    dxbc = data[off:]
    if dxbc[:4] != b"DXBC":
        raise ValueError(f"{path}: expected DXBC at +{off}, got {dxbc[:4]!r}")
    return deps, dxbc


def shipped(resources: Path) -> dict[str, list[str]]:
    """Shader name -> its shipped permutation strings, from the resources tree."""
    base = resources / "shaders" / SM_DIR
    out: dict[str, list[str]] = {}
    for entry in sorted(base.iterdir()):
        if entry.is_dir():
            out[entry.name] = sorted(p.name for p in entry.iterdir())
    return out


def compile_shader(fxc: Path, source: Path, out: Path,
                   defines: dict[str, str] | None = None) -> None:
    """Era-exact compile: fxc 9.29.952.3111 under Wine, /Zpr /O3, entry main.

    Wine maps / as the Z: drive, so unix paths are passed Windows-spelled.
    Quoted #includes resolve relative to the including file, which is how the
    original tool ran too (every source names its neighbours directly).
    """
    profile = PROFILES[source.suffix]
    zsrc = "Z:" + str(source.resolve()).replace("/", "\\")
    zout = "Z:" + str(out.resolve()).replace("/", "\\")
    cmd = ["wine", str(fxc / "fxc.exe"), "/nologo", "/Zpr", "/O3",
           "/T", profile, "/E", "main"]
    for name, value in (defines or {}).items():
        cmd += ["/D", f"{name}={value}"]
    cmd += ["/Fo", zout, zsrc]
    env = dict(os.environ)
    env.setdefault("WINEDEBUG", "-all")
    env.setdefault("WINEPREFIX", str(paths.WINEPREFIX))
    proc = subprocess.run(cmd, env=env, capture_output=True, text=True)
    if proc.returncode != 0 or not out.exists():
        raise RuntimeError(f"fxc failed on {source.name}:\n{proc.stderr.strip()}")


def disasm_one(fxc: Path, blob: Path, out: Path) -> None:
    """Era-exact disassembly: fxc /dumpbin /Fc on a shipped blob's DXBC.

    DXBC keeps its reflection chunks, so the listing opens with named cbuffer
    layouts, resource bindings and signatures - ship truth with symbols. This
    is the reference for reconstructing HLSL the roundtrip can then prove.
    """
    _, dxbc = parse_blob(blob)
    out.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="fxc-disasm-") as tmp:
        obj = Path(tmp) / "in.dxbc"
        obj.write_bytes(dxbc)
        zobj = "Z:" + str(obj).replace("/", "\\")
        zout = "Z:" + str(out.resolve()).replace("/", "\\")
        cmd = ["wine", str(fxc / "fxc.exe"), "/nologo", "/dumpbin",
               "/Fc", zout, zobj]
        env = dict(os.environ)
        env.setdefault("WINEDEBUG", "-all")
        env.setdefault("WINEPREFIX", str(paths.WINEPREFIX))
        proc = subprocess.run(cmd, env=env, capture_output=True, text=True)
        if proc.returncode != 0 or not out.exists():
            raise RuntimeError(
                f"fxc /dumpbin failed on {blob}:\n{proc.stderr.strip()}")


def roundtrip_one(fxc: Path, resources: Path, name: str, perm: str,
                  defines: dict[str, str] | None = None) -> str:
    """'ok' | 'DIFF <detail>' | 'skip <reason>' for one shipped permutation.

    `defines` is the parsed /D set for this permutation (empty dict for the
    plain permutation); None means the permutation string could not be
    solved against the registry.
    """
    if defines is None:
        return "skip (permutation string unsolved against the registry)"
    source = paths.SHADER_SOURCES / SM_DIR / name
    if not source.exists():
        return "skip (no recovered source)"
    blob = resources / "shaders" / SM_DIR / name / perm
    _, want = parse_blob(blob)
    # Unmasked globals never reach the permutation string but did reach the
    # compile; the permutation's own values win where both exist.
    from vostok.shaders.registry import GLOBAL_DEFAULTS
    merged = dict(GLOBAL_DEFAULTS)
    merged.update(defines)
    with tempfile.TemporaryDirectory(prefix="fxc-roundtrip-") as tmp:
        out = Path(tmp) / "out.dxbc"
        try:
            compile_shader(fxc, source, out, merged)
        except RuntimeError as e:
            return f"DIFF (compile failed: {e})"
        got = out.read_bytes()
    if got == want:
        return "ok"
    if len(got) != len(want):
        return f"DIFF (size {len(got)} != shipped {len(want)})"
    first = next(i for i in range(len(got)) if got[i] != want[i])
    return f"DIFF (first mismatch at +{first} of {len(want)})"
