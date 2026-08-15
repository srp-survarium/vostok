#!/usr/bin/env python3
"""Shim: scripts/sema.py -> vostok.sema.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.sema <view>`.
"""

from vostok.sema import main

if __name__ == "__main__":
    main()
