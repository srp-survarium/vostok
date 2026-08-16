#!/usr/bin/env python3
"""Shim: scripts/test_match_db.py -> vostok.tests.test_match_db.

The tests moved into the package; this path stays so existing docs and habits
keep working. Equivalent: `python3 -m unittest vostok.tests.test_match_db`.
"""

import unittest

from vostok.tests.test_match_db import *  # noqa: F401,F403

if __name__ == "__main__":
    unittest.main()
