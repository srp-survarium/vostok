#!/usr/bin/env python3
"""Shim: scripts/generate_delink.py -> vostok.build.generate_delink.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.generate_delink`.
"""

from vostok.build.generate_delink import main

if __name__ == "__main__":
    main()
