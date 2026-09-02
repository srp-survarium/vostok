# SPDX-License-Identifier: GPL-3.0-or-later

from vostok.core import log as _log
from vostok.data.cli import main

raise SystemExit(_log.run("vostok.data", main))
