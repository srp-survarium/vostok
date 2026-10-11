# SPDX-License-Identifier: GPL-3.0-or-later

"""`python3 -m vostok.evidence FUNCTION ...`"""

from vostok.core import log as _log
from vostok.evidence import main

raise SystemExit(_log.run("vostok.evidence", main))
