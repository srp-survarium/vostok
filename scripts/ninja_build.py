#!/usr/bin/env python3
"""Shim: scripts/ninja_build.py -> vostok.build.ninja.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.ninja`.
"""

from vostok.build.ninja import main

if __name__ == "__main__":
    main()
