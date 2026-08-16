#!/usr/bin/env python3
"""Shim: scripts/regen_ninja.py -> vostok.build.ninja_regen.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.ninja_regen`.
"""

from vostok.build.ninja_regen import main

if __name__ == "__main__":
    main()
