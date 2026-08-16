#!/usr/bin/env python3
"""Shim: scripts/find_breakpoints.py -> vostok.tool.breakpoints.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.tool.breakpoints`.
"""

from vostok.tool.breakpoints import main

if __name__ == "__main__":
    main()
