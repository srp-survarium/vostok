# SPDX-License-Identifier: GPL-3.0-or-later

"""`python3 -m vostok.ledger <verb> ...`"""

import sys

from vostok.core import log as _log
from vostok.ledger import main

sys.argv[0] = "vostok.ledger"
raise SystemExit(_log.run("vostok.ledger", main))
