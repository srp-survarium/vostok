#!/usr/bin/env python3
"""Shim: scripts/enum_diff.py -> vostok.diff.enums.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.diff.enums`.
"""

import sys

from vostok.diff.enums import main

if __name__ == "__main__":
    sys.exit(main())
