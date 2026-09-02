# SPDX-License-Identifier: GPL-3.0-or-later

"""`python3 -m vostok <command> ...`"""

from vostok.cli import main
from vostok.core import log as _log

raise SystemExit(_log.run("vostok", main))
