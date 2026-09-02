# SPDX-License-Identifier: GPL-3.0-or-later

"""`python3 -m vostok.derive <verb> ...`"""

import sys

from vostok.core import log as _log
from vostok.derive.cli import main

sys.argv[0] = "vostok.derive"
raise SystemExit(_log.run("vostok.derive", main))
