#!/usr/bin/env python3
"""Shim: scripts/create-toolchain-release.py -> vostok.tool.toolchain_release.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.tool.toolchain_release`.
"""

from vostok.tool.toolchain_release import main

if __name__ == "__main__":
    main()
