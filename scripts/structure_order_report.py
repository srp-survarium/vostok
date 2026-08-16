#!/usr/bin/env python3
"""Shim: scripts/structure_order_report.py -> vostok.diff.order.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.diff.order`.
"""

from vostok.diff.order import main

if __name__ == "__main__":
    main()
