#!/usr/bin/env python3
"""Shim: scripts/structure_mismatch_queue.py -> vostok.ledger.queue.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.ledger mismatch-queue`.
"""

import sys

from vostok.ledger.queue import main

if __name__ == "__main__":
    sys.exit(main())
