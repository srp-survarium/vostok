#!/usr/bin/env python3
"""Shim: scripts/generate_objdiff_config.py -> vostok.build.generate_objdiff_config.

The code moved into the package; this path stays so existing docs, habits and
callers keep working. Equivalent: `python3 -m vostok.build.generate_objdiff_config`.
"""

from vostok.build.generate_objdiff_config import cli

if __name__ == "__main__":
    cli()
