#!/usr/bin/env python3
"""Shim: scripts/test_sema.py -> vostok.tests.test_sema.

The tests moved into the package; this path stays so existing docs and habits
keep working. Equivalent: `python3 -m unittest vostok.tests.test_sema`.
"""

import unittest

from vostok.tests.test_sema import *  # noqa: F401,F403

if __name__ == "__main__":
    unittest.main()
