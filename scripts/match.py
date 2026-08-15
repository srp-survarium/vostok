#!/usr/bin/env python3
"""Shim: scripts/match.py -> vostok.ledger.cli.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.ledger <verb>`.
"""

from vostok.ledger.cli import main

if __name__ == "__main__":
    main()
