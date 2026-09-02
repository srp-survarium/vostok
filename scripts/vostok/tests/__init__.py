# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.tests - the tooling's own unit tests.

    python3 -m unittest discover -s scripts/vostok/tests

They cover the parts where a silent wrong answer would corrupt the campaign
record rather than merely crash: name normalization, alias admission, the
structure classifier, and the MAX/epoch policy.
"""
