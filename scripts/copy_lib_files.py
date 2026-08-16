#!/usr/bin/env python3
"""Shim: scripts/copy_lib_files.py -> vostok.tool.libs.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.tool.libs`.
"""

from vostok.tool.libs import main

if __name__ == "__main__":
    main()
