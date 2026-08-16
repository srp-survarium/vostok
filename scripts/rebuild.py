#!/usr/bin/env python3
"""Shim: scripts/rebuild.py -> vostok.build.rebuild.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.rebuild`.
"""

from vostok.build.rebuild import main

if __name__ == "__main__":
    main()
