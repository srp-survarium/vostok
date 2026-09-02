# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.tool.libs_release - cut a vostok-libs release.

`binaries.prebuilt/` is staged on first `nix develop` from the vostok-libs
package the flake pins: the third-party blobs (.lib/.dll/.pdb) the engine links,
laid out as the shipped game stored them (see vostok.tool.libs). The one part of
that package this repo BUILDS is the Scaleform GFx static-lib suite:
`vostok.build.gfx` compiles it from the pristine 4.2.22 SDK with the exact cl
recipe the shipped PDB records, into binaries.prebuilt/Win32/libraries/shipping/.

A release is the previous package with those GFx libs replaced by a fresh build,
zipped deterministically, published on the vostok-build-env GitHub release and
pinned in flake.nix - the same flow as vostok.tool.toolchain_release for the
toolchain. Run inside `nix develop` (the GFx build needs the pristine SDK:
`nix develop .#with-scaleform-sdk`, or $SCALEFORM_SDK).

    python3 -m vostok tool libs-release --tag gfx421r2             # build GFx, package, print sha256
    python3 -m vostok tool libs-release --tag gfx421r2 --skip-gfx  # package the suite already in binaries.prebuilt
    python3 -m vostok tool libs-release --tag gfx421r2 --upload    # ... and `gh release upload`
"""
import argparse
import hashlib
import shutil
import subprocess
import sys
import zipfile
from pathlib import Path

from vostok.core.paths import BINARIES
from vostok.tool.libs import GFX_SRC, LIBS_DIR

RELEASE_TAG = "v0.100b"
RELEASE_REPO = "srp-survarium/vostok-build-env"
# zip entries carry a fixed timestamp so the archive is reproducible
RELEASE_DATE = (2026, 1, 1, 0, 0, 0)


def _wine_prefix() -> str:
    """The source root the fresh objects record: GFX_BUILD_TREE as Wine spells it."""
    from vostok.core.paths import GFX_BUILD_TREE
    return "z:" + str(GFX_BUILD_TREE.resolve()).replace("/", "\\").lower()


def log(msg: str) -> None:
    print(f"[libs-release] {msg}", flush=True)


def build_gfx() -> None:
    from vostok.build import gfx
    log("building the GFx suite from source (vostok.build.gfx) ...")
    if gfx.main() != 0:
        raise SystemExit("GFx build failed - fix it or pass --skip-gfx to package what is staged")


def stage(work: Path) -> Path:
    """Copy the current package, then drop the freshly built GFx libs over it."""
    from vostok.build import gfx
    if not (LIBS_DIR / "sources").is_dir():
        raise SystemExit(f"vostok-libs package not found at {LIBS_DIR} (VOSTOK_LIBS_DIR)")
    root = work / "vostok-libs"
    if root.exists():
        shutil.rmtree(root)
    log(f"staging {LIBS_DIR} -> {root}")
    shutil.copytree(LIBS_DIR, root, symlinks=False)
    for p in root.rglob("*"):
        p.chmod(0o755 if p.is_dir() else 0o644)   # nix store copies are read-only
    ship = root / "sources" / GFX_SRC
    ship.mkdir(parents=True, exist_ok=True)
    for name in gfx.DEFAULT_ORDER:
        built = gfx.SHIP / f"{name}.lib"
        if not built.is_file():
            raise SystemExit(f"missing {built} - run the GFx build first (or drop --skip-gfx)")
        shutil.copy2(built, ship / f"{name}.lib")
        log(f"  {name}.lib: {built.stat().st_size / 1e6:.1f} MB")
    return root


def package(root: Path, output: Path) -> str:
    log(f"packaging -> {output}")
    output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(output, "w", zipfile.ZIP_DEFLATED) as z:
        for p in sorted(root.rglob("*")):
            if not p.is_file():
                continue
            info = zipfile.ZipInfo(str(p.relative_to(root.parent)), date_time=RELEASE_DATE)
            info.compress_type = zipfile.ZIP_DEFLATED
            info.external_attr = 0o644 << 16
            z.writestr(info, p.read_bytes())
    h = hashlib.sha256()
    with output.open("rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def main() -> int:
    ap = argparse.ArgumentParser(prog="vostok tool libs-release", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--tag", required=True, help="suffix for the zip name, e.g. gfx421r2")
    ap.add_argument("--skip-gfx", action="store_true", help="do not rebuild GFx; package what binaries.prebuilt holds")
    ap.add_argument("--upload", action="store_true", help=f"gh release upload to {RELEASE_REPO} {RELEASE_TAG}")
    ap.add_argument("--work", type=Path, default=BINARIES / "libs-release", help="staging directory")
    args = ap.parse_args()

    if not args.skip_gfx:
        build_gfx()
    root = stage(args.work)
    output = BINARIES / f"vostok-libs-{RELEASE_TAG}-{args.tag}.zip"
    digest = package(root, output)
    print()
    log("done")
    print(f"  Output: {output}  ({output.stat().st_size / 1e6:.0f} MB)")
    print(f"  SHA256: {digest}")
    print()
    print("Next steps:")
    print(f"  1. gh release upload {RELEASE_TAG} {output} --repo {RELEASE_REPO} --clobber")
    print("  2. In flake.nix (vostok-libs), set name/url to the new zip and")
    print(f'       sha256 = "{digest}";')
    print("  3. Re-enter `nix develop` so binaries.prebuilt/ is re-staged from the new package.")
    print("  4. In vostok/core/paths.py set GFX_RELEASE_PREFIX to the tree the suite was built from:")
    print(f'       GFX_RELEASE_PREFIX = r"{_wine_prefix()}"')
    if args.upload:
        subprocess.run(["gh", "release", "upload", RELEASE_TAG, str(output),
                        "--repo", RELEASE_REPO, "--clobber"], check=True)
        log("uploaded")
    return 0


if __name__ == "__main__":
    sys.exit(main())
