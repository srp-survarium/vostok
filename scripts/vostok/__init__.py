"""vostok - the tooling that drives the Vostok Engine binary-matching campaign.

One direction of data flow:

    sources/ --build--> binaries/Win32/*.{exe,pdb}
                    --delink--> binaries/objdiff/{base,target} + report.json
                            --derive--> binaries/match.db (regenerable cache)
                                    --project--> docs/binary_matching/match_state.tsv
                                             --render--> README.md score block

Layers (imports point strictly downward):

    core/     repo paths, tracked-table I/O, symbol-name normalization. Knows
              formats and locations, never what a match means.
    derive/   report.json + the rich indexes -> the match.db roster (the only
              place pairing, structure classification and MAX policy live).
    ledger/   the committed campaign record: query, mutate, and render it.
    sema/     read-only control-flow views over one base<->target function pair.
    build/    driving the toolchain: ninja under Wine, delink, structure, rich.
    diff/     target-vs-base source-shape reports (layouts, enums, TU order).
    tool/     one module per external tool or one-off operational helper.

`scripts/` is the package root, so every module is reachable two ways: as
`python3 -m vostok.<pkg> ...` (needs scripts/ on PYTHONPATH - the dev shell
exports it) and through the thin `scripts/<old-name>.py` shims, which exist so
existing docs, muscle memory and rebuild.py keep working unchanged.
"""
