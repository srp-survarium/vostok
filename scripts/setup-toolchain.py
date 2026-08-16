#!/usr/bin/env python3
"""Shim: scripts/setup-toolchain.py -> vostok.tool.toolchain.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.tool.toolchain`.
"""

from vostok.tool.toolchain import main

if __name__ == "__main__":
    main()
