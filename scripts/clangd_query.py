#!/usr/bin/env python3
"""Shim: scripts/clangd_query.py -> vostok.tool.clangd.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.tool.clangd`.
"""

from vostok.tool.clangd import main

if __name__ == "__main__":
    main()
