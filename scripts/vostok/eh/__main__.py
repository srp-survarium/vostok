# SPDX-License-Identifier: GPL-3.0-or-later

"""`python3 -m vostok.eh ...`"""

from vostok.core import log as _log
from vostok.eh import main

raise SystemExit(_log.run("vostok.eh", main))
