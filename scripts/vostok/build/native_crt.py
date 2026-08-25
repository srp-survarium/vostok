"""Native msvcr90 for the build prefix.

MSVC composes template-instance UDT display names by undecorating the mangled
name through the CRT's __unDName (c1xx.dll, c2.dll and link.exe all import it
from msvcr90.dll). Wine's builtin msvcr90 reimplements the undecorator and
renders enums bare (`flags_type<cook_base::flags_enum,...>`) where Microsoft's
renders them elaborated (`flags_type<enum cook_base::flags_enum,...>`, and
`T const ` with a trailing space) - the retail survarium.pdb style. Installing
the toolchain's redist msvcr90 and forcing it native makes our PDBs spell type
records exactly like retail's (probe-proven end to end, /GL + LTCG included).

The install must land INSIDE the prefix's fake winsxs assembly dir: cl.exe's
embedded manifest requests Microsoft.VC90.CRT via SxS, and Wine resolves that
to its own stub assembly, so app-dir or system32 copies are never consulted.

This is prefix SETUP, not a per-build step: `provision()` both installs the
DLLs and writes a persistent `msvcr90=native` DllOverride into the prefix's
Wine registry, so every later `wine` in that prefix prefers the native CRT
with no `WINEDLLOVERRIDES` on the command line. `vostok.tool.toolchain` calls
it during the wine/registry setup stages; a plain build touches none of this.
"""

import os
import shutil
import subprocess
from pathlib import Path

_ASSEMBLY_GLOB = "x86_microsoft.vc90.crt_*"
_DLLS = ("msvcr90.dll", "msvcp90.dll", "msvcm90.dll")


def _assembly_dirs(wineprefix: Path) -> list[Path]:
    winsxs = wineprefix / "drive_c" / "windows" / "winsxs"
    manifests = winsxs / "manifests"
    if not manifests.is_dir():
        return []
    return [winsxs / m.name.removesuffix(".manifest")
            for m in manifests.glob(_ASSEMBLY_GLOB + ".manifest")]


def install(wineprefix: Path) -> bool:
    """Copy the redist VC90 CRT into every vc90.crt winsxs assembly dir.

    Idempotent and cheap (copy-if-missing). Returns True if the native DLLs
    are in place afterwards.
    """
    msvc_dir = os.environ.get("MSVC_DIR")
    if not msvc_dir:
        return False
    redist = Path(msvc_dir) / "VC" / "redist" / "x86" / "Microsoft.VC90.CRT"
    dirs = _assembly_dirs(wineprefix)
    if not redist.is_dir() or not dirs:
        return False
    for d in dirs:
        d.mkdir(parents=True, exist_ok=True)
        for dll in _DLLS:
            src, dst = redist / dll, d / dll
            # wineboot pre-creates Wine STUB DLLs here; a same-name file is
            # not proof of the native one, so compare sizes, not existence.
            if src.is_file() and (not dst.is_file()
                                  or dst.stat().st_size != src.stat().st_size):
                if dst.is_file():
                    dst.chmod(0o644)
                shutil.copyfile(src, dst)
                dst.chmod(0o644)
    return installed(wineprefix)


def installed(wineprefix: Path) -> bool:
    msvc_dir = os.environ.get("MSVC_DIR")
    if not msvc_dir:
        return False
    src = Path(msvc_dir) / "VC" / "redist" / "x86" / "Microsoft.VC90.CRT" / "msvcr90.dll"
    dirs = _assembly_dirs(wineprefix)
    return (src.is_file() and bool(dirs)
            and all((d / "msvcr90.dll").is_file()
                    and (d / "msvcr90.dll").stat().st_size == src.stat().st_size
                    for d in dirs))


_DLLOVERRIDES_KEY = r"HKEY_CURRENT_USER\Software\Wine\DllOverrides"


def registry_override_set(wineprefix: Path) -> bool:
    """True if the prefix's Wine registry already prefers native msvcr90.

    Queries the LIVE registry (`wine reg query`) rather than parsing user.reg:
    `wine reg add` only flushes the on-disk hive on wineserver shutdown, so a
    disk read right after a write races. One cheap wine spawn, negligible
    against a full build.
    """
    env = dict(os.environ, WINEPREFIX=str(wineprefix), WINEDEBUG="-all")
    r = subprocess.run(
        ["wine", "reg", "query", _DLLOVERRIDES_KEY, "/v", "msvcr90"],
        check=False, capture_output=True, text=True, env=env)
    return "native" in r.stdout.lower()


def set_registry_override(wineprefix: Path) -> None:
    """Persist msvcr90=native in the prefix registry (no per-build env var).

    A registry DllOverride is read by every wine process in the prefix, so the
    compiler prefers Microsoft's __unDName for the life of the prefix without
    WINEDLLOVERRIDES on each invocation.
    """
    env = dict(os.environ, WINEPREFIX=str(wineprefix), WINEDEBUG="-all")
    subprocess.run(
        ["wine", "reg", "add", _DLLOVERRIDES_KEY,
         "/v", "msvcr90", "/t", "REG_SZ", "/d", "native", "/f"],
        check=False, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, env=env)


def provision(wineprefix: Path) -> bool:
    """Prefix setup: install the native CRT and persist the native override.

    Idempotent. Returns True once the prefix is set to build with native
    msvcr90 (DLLs in winsxs + registry override). Called from the toolchain
    setup stages; NOT from the per-build path.
    """
    ok = install(wineprefix)
    if ok and not registry_override_set(wineprefix):
        set_registry_override(wineprefix)
    return ok and registry_override_set(wineprefix)


def provisioned(wineprefix: Path) -> bool:
    """True if this prefix will build with native msvcr90 (DLLs + override)."""
    return installed(wineprefix) and registry_override_set(wineprefix)
