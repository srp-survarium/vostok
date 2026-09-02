# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.core.fmt - tiny formatting helpers shared by the reports."""


def pct(part, whole) -> float:
    return 100.0 * part / whole if whole else 0.0
