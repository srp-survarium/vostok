# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok - the tooling that drives the Vostok Engine binary-matching campaign.

One direction of data flow:

    sources/ --build--> binaries/Win32/*.{exe,pdb}
                    --delink--> binaries/objdiff/{base,target} + report.json
                            --derive--> config/match_state.tsv
                                    --render--> README.md score block

Nothing is cached in between. The derivation writes the committed record
directly, so there is no second copy that can disagree with it.

Layers (imports point strictly downward):

    core/     repo paths, tracked-table I/O, symbol-name normalization. Knows
              formats and locations, never what a match means.
    derive/   report.json + the rich indexes -> the ledger's roster (the only
              place pairing, structure classification and MAX policy live).
    ledger/   the committed campaign record: query, mutate, and render it.
    sema/     read-only control-flow views over one base<->target function pair.
    build/    driving the toolchain: ninja under Wine, delink, structure, rich.
    diff/     target-vs-base source-shape reports (layouts, enums, TU order).
    tool/     one module per external tool or one-off operational helper.

`scripts/` is the package root and the dev shell puts it on PYTHONPATH, so
every module runs as `python3 -m vostok.<pkg> ...` from anywhere in the tree.
`python3 -m vostok` prints the umbrella map: `vostok build`, `vostok ledger`,
`vostok derive`, `vostok sema`, `vostok diff`, `vostok tool`.

Layers. Imports point downward only, `core` knows nothing above it:

    core/     paths, tsv, symbols, log, wine, fmt - semantically blind helpers
    derive/ ledger/ data/ sema/ diff/ shaders/ build/ tool/   one lane each
    cli       the umbrella dispatch (python3 -m vostok)

Known cross-lane imports, kept because they are one-way facts, not layering:
build reads derive/ledger/data to refresh them after a build; tool drives
build; data/sema read the ledger store and each other's pairing. The cycles
ledger<->derive and ledger<->data are debt: the store should not need to know
who fills it.
"""
