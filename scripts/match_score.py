#!/usr/bin/env python3
"""Shim: scripts/match_score.py -> vostok.ledger.readme.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.ledger readme`.
"""

from vostok.ledger.readme import main

if __name__ == "__main__":
    main()
