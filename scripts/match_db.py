#!/usr/bin/env python3
"""Shim: scripts/match_db.py -> vostok.derive.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.derive <verb>`.
"""

from vostok.derive.cli import main

if __name__ == "__main__":
    main()
