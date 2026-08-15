#!/usr/bin/env python3
"""Shim: scripts/normalize_objdiff_symbols.py -> vostok.core.symbols.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.core.symbols`.
"""

import sys

from vostok.core.symbols import main

if __name__ == "__main__":
    sys.exit(main())
