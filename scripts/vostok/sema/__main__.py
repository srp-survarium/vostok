# SPDX-License-Identifier: GPL-3.0-or-later

"""`python3 -m vostok.sema <view> ...`"""

import sys

from vostok.core import log as _log
from vostok.sema import main

sys.argv[0] = "vostok.sema"
raise SystemExit(_log.run("vostok.sema", main))
