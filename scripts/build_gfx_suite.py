#!/usr/bin/env python3
"""Shim: scripts/build_gfx_suite.py -> vostok.build.gfx.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.gfx`.
"""

import sys

from vostok.build.gfx import main

if __name__ == "__main__":
    sys.exit(main())
