#!/usr/bin/env python3
"""Shim: scripts/generate_rich.py -> vostok.build.generate_rich.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.generate_rich`.
"""

from vostok.build.generate_rich import main

if __name__ == "__main__":
    main()
