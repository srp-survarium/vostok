#!/usr/bin/env python3
"""Shim: scripts/compare_enums.py -> vostok.diff.enums_compare.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.diff.enums_compare`.
"""

import sys

from vostok.diff.enums_compare import main

if __name__ == "__main__":
    sys.exit(main())
