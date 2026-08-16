#!/usr/bin/env python3
"""Shim: scripts/tu_order.py -> vostok.diff.tu_order.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.diff.tu_order`.
"""

from vostok.diff.tu_order import main

if __name__ == "__main__":
    main()
