#!/usr/bin/env python3
"""Shim: scripts/generate_structure.py -> vostok.build.generate_structure.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.generate_structure`.
"""

from vostok.build.generate_structure import main

if __name__ == "__main__":
    main()
