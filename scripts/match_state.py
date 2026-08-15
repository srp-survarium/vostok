#!/usr/bin/env python3
"""Shim: scripts/match_state.py -> vostok.ledger.store.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.ledger.store <verb>`.
"""

import sys

from vostok.ledger.store import main

if __name__ == "__main__":
    sys.exit(main(sys.argv))
