"""`python3 -m vostok.shaders <verb>` - shipped-shader coverage and roundtrip.

    coverage                 shipped names vs recovered sources, with counts
    dump NAME[/PERM]         one blob: dependency table, mtimes, DXBC size
    disasm [NAME...]         era-exact fxc /dumpbin listings of shipped blobs
                             into binaries/shaders/disasm/ (one representative
                             permutation per name; --all-perms for every one)
    roundtrip [NAME...]      recompile + byte-compare; no names = every shipped
                             all-underscore permutation with a recovered source

roundtrip exits nonzero on any DIFF, so it can stand as an assertion.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from vostok import shaders
from vostok.core import paths
from vostok.shaders import registry


def _coverage(args: argparse.Namespace) -> int:
    resources = shaders.resources_root(args.resources)
    ship = shaders.shipped(resources)
    src_dir = paths.SHADER_SOURCES / shaders.SM_DIR
    have = {p.name for p in src_dir.iterdir()
            if p.suffix in shaders.PROFILES}
    missing = sorted(set(ship) - have)
    extra = sorted(have - set(ship))
    plain = sorted(n for n, perms in ship.items()
                   if any(set(p) == {"_"} for p in perms))
    print(f"shipped shader names : {len(ship)}")
    print(f"recovered sources    : {len(have)} "
          f"({len(set(ship) & have)} of the shipped names)")
    print(f"missing sources      : {len(missing)}")
    print(f"source-only files    : {len(extra)}")
    print(f"plain permutations   : {len(plain)} names shipped with no define set")
    if args.missing:
        for name in missing:
            print(f"  missing {name}")
    return 0


def _dump(args: argparse.Namespace) -> int:
    resources = shaders.resources_root(args.resources)
    name, _, perm = args.name.partition("/")
    base = resources / "shaders" / shaders.SM_DIR / name
    blobs = [base / perm] if perm else sorted(base.iterdir())
    for blob in blobs:
        deps, dxbc = shaders.parse_blob(blob)
        print(f"{name}/{blob.name}")
        print(f"  dxbc {len(dxbc)} bytes, {len(deps)} dependencies:")
        for dep, mtime in deps:
            print(f"    {dep}  mtime={mtime}")
    return 0


def _disasm(args: argparse.Namespace) -> int:
    resources = shaders.resources_root(args.resources)
    fxc = shaders.fxc_dir(args.fxc_dir)
    out_root = Path(args.out) if args.out else paths.SHADER_DISASM
    ship = shaders.shipped(resources)
    names = args.names or sorted(ship)
    done = failed = 0
    for name in names:
        if name not in ship:
            print(f"FAIL {name} (not shipped)")
            failed += 1
            continue
        perms = ship[name]
        if not args.all_perms:
            # One representative: the plain permutation when shipped, else the
            # first - every permutation is the same source, differently defined.
            perms = [next((p for p in perms if set(p) == {"_"}), perms[0])]
        for perm in perms:
            blob = resources / "shaders" / shaders.SM_DIR / name / perm
            out = out_root / shaders.SM_DIR / name / f"{perm}.asm"
            try:
                shaders.disasm_one(fxc, blob, out)
                done += 1
            except RuntimeError as e:
                print(f"FAIL {name}/{perm}: {e}")
                failed += 1
    print(f"disasm: {done} listings under {out_root}, {failed} failed")
    return 1 if failed else 0


def _roundtrip(args: argparse.Namespace) -> int:
    resources = shaders.resources_root(args.resources)
    fxc = shaders.fxc_dir(args.fxc_dir)
    ship = shaders.shipped(resources)
    names = args.names or sorted(ship)
    counts = {"ok": 0, "DIFF": 0, "skip": 0}
    for name in names:
        if name not in ship:
            print(f"DIFF {name} (not shipped)")
            counts["DIFF"] += 1
            continue
        mapping = registry.parse_permutations(ship[name])
        for perm in ship[name]:
            defines = None if mapping is None else mapping.get(perm)
            verdict = shaders.roundtrip_one(fxc, resources, name, perm, defines)
            counts[verdict.split()[0]] += 1
            if verdict != "ok" and (verdict.startswith("DIFF") or args.verbose):
                print(f"{verdict.split()[0]} {name} {verdict[len(verdict.split()[0]) + 1:]}")
            elif verdict == "ok":
                print(f"ok   {name}")
    print(f"\nroundtrip: {counts['ok']} identical, {counts['DIFF']} differ, "
          f"{counts['skip']} skipped")
    return 1 if counts["DIFF"] else 0


def main() -> int:
    parser = argparse.ArgumentParser(prog="vostok.shaders",
                                     description=__doc__.split("\n")[0])
    parser.add_argument("--resources", help="unpacked resources tree "
                        "(default: $VOSTOK_RESOURCES_UNPACKED)")
    sub = parser.add_subparsers(dest="verb", required=True)

    p = sub.add_parser("coverage", help="shipped names vs recovered sources")
    p.add_argument("--missing", action="store_true",
                   help="list every shipped name with no recovered source")
    p.set_defaults(func=_coverage)

    p = sub.add_parser("dump", help="one blob's dependency table")
    p.add_argument("name", help="shader name, optionally name/permutation")
    p.set_defaults(func=_dump)

    p = sub.add_parser("disasm", help="disassemble shipped blobs to asm")
    p.add_argument("names", nargs="*",
                   help="shader names (default: all shipped)")
    p.add_argument("--all-perms", action="store_true",
                   help="every permutation, not one representative per name")
    p.add_argument("--out", help=f"output tree (default: {paths.SHADER_DISASM})")
    p.add_argument("--fxc-dir", help="dir with fxc.exe + D3DCompiler_43.dll "
                   "(default: $DXSDK_SHADER_COMPILER)")
    p.set_defaults(func=_disasm)

    p = sub.add_parser("roundtrip", help="recompile and byte-compare")
    p.add_argument("names", nargs="*",
                   help="shader names (default: all shipped)")
    p.add_argument("--fxc-dir", help="dir with fxc.exe + D3DCompiler_43.dll "
                   "(default: $DXSDK_SHADER_COMPILER)")
    p.add_argument("--verbose", action="store_true",
                   help="print skips, not only oks and DIFFs")
    p.set_defaults(func=_roundtrip)

    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
