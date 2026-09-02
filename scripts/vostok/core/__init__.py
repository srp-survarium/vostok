# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.core - semantically blind primitives.

Rules of the layer:
  * stdlib only; never imports another vostok subpackage;
  * knows where a file lives and how it is spelled, never what it means;
  * one convention, one owner: every repo path comes from `paths`, every
    tracked tab-separated table is read through `tsv`.
"""
