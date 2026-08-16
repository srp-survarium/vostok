#!/usr/bin/env python3
"""Shim: scripts/count_sizes.py -> vostok.tool.sizes.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.tool.sizes`.
"""

from vostok.tool.sizes import main

if __name__ == "__main__":
    main()
