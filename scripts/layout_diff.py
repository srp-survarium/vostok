#!/usr/bin/env python3
"""Shim: scripts/layout_diff.py -> vostok.diff.layout.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.diff.layout`.
"""

import sys

from vostok.diff.layout import main

if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
