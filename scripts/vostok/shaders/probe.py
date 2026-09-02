# SPDX-License-Identifier: GPL-3.0-or-later
"""Parallel roundtrip probe: same verdicts as `vostok.shaders roundtrip`,
but concurrent and able to report the permutation strings that DIFF.

    probe.py [--jobs N] [--perms FILE] [--list] NAME [NAME...]
"""
from __future__ import annotations

import argparse
import sys
from concurrent.futures import ThreadPoolExecutor

from vostok import shaders
from vostok.shaders import registry


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("names", nargs="+")
    ap.add_argument("--jobs", type=int, default=8)
    ap.add_argument("--perms", help="file of permutation strings to restrict to")
    ap.add_argument("--list", action="store_true",
                    help="print the permutation string of every DIFF")
    args = ap.parse_args()

    resources = shaders.resources_root(None)
    fxc = shaders.fxc_dir(None)
    ship = shaders.shipped(resources)
    only = None
    if args.perms:
        only = set(open(args.perms).read().split())

    jobs = []
    for name in args.names:
        mapping = registry.parse_permutations(ship[name])
        for perm in ship[name]:
            if only is not None and perm not in only:
                continue
            jobs.append((name, perm, None if mapping is None
                         else mapping.get(perm)))

    def run(job):
        name, perm, defines = job
        return name, perm, shaders.roundtrip_one(fxc, resources, name, perm,
                                                 defines)

    counts: dict[str, dict[str, int]] = {}
    diffs: list[tuple[str, str, str]] = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        for name, perm, verdict in pool.map(run, jobs):
            key = verdict.split()[0]
            counts.setdefault(name, {}).setdefault(key, 0)
            counts[name][key] += 1
            if key != "ok":
                diffs.append((name, perm, verdict))

    if args.list:
        for name, perm, verdict in sorted(diffs):
            print(f"DIFF {name} {perm} {verdict}")
    total_ok = total_diff = 0
    for name in args.names:
        c = counts.get(name, {})
        ok, diff = c.get("ok", 0), c.get("DIFF", 0) + c.get("skip", 0)
        total_ok += ok
        total_diff += diff
        print(f"{name:44} {ok:5} ok {diff:4} DIFF")
    print(f"TOTAL {total_ok} ok {total_diff} DIFF")
    return 0


sys.exit(main())
