#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""sema - CONTROL-FLOW views over the delinked base<->target function pair.

The layer vostok did not have. `pdb_fetch --view structure-diff` compares SOURCE
STATEMENTS and `pdb_fetch --view diff` compares INSTRUCTIONS (objdiff-core,
operand/reloc aware); neither says anything about the shape of the control-flow
graph. A branch that lands on a different basic block is, to both of those views,
either invisible or an uninterpretable pair of absolute addresses:

    ~ 0x2b: je short 0000029Eh    -> je short 000003C1h

Two numbers from two different address spaces. Nothing tells you whether that is
a real retarget (an `if` we got wrong) or the harmless consequence of an upstream
size drift. `sema blocks --diff` and `sema branches --diff` answer exactly that,
by naming every branch destination with a BLOCK INDEX derived per side, so the
two sides become comparable and a uniform displacement shift compares EQUAL while
a genuine retarget does not. Design + evidence: docs/binary_matching/sema_tools.md.

Both sides come out of the same delinker and the same disassembler
(`pdb_fetch --view target` / `--view base` over `binaries/rich/{target,base}`),
so - unlike the two-disassembler setups this idea comes from - almost no
instruction-spelling normalization is needed: only the branch operand is masked.
Every other textual difference between the two sides is a real byte difference.

Blocks that carry no control flow (an alignment pad, a spill reload, a
re-materialised zero starting a block only because it follows a `jcc`) are
CONTRACTED before anything compares the two graphs - without that, one byte of
padding on one side renames every later block and prints as a cascade of
retargeted branches. See `contract`.

    python3 -m vostok sema rva      <fn>                  # address/source/match dossier
    python3 -m vostok sema xref     <fn> --callees        # root -> direct callees
    python3 -m vostok sema xref     <fn>                  # direct callers
    python3 -m vostok sema strings  <fn>                  # referenced string literals
    python3 -m vostok sema strings  --find <text>         # reverse literal lookup
    python3 -m vostok sema blocks   <fn> --diff --lite    # THE VERDICT VIEW
    python3 -m vostok sema blocks   <fn> --diff     # same, with per-block bodies
    python3 -m vostok sema blocks   <fn> [--base]   # one side's CFG
    python3 -m vostok sema branches <fn> --diff     # read a difference branch by branch
    python3 -m vostok sema dot      <fn> [--diff]   # graphviz
    python3 -m vostok sema sweep --module render [--unit U] [--min-pct P] [--max N]

Only `blocks` takes `--lite`. `--base` selects the side for a one-sided view and
is ignored by `--diff`, which always shows both.

`blocks` aligns by CONTENT and is what a verdict comes from; `branches` pairs
POSITIONALLY, so it is for reading a difference `blocks` already established.

<fn> is a mangled name, a demangled substring, or a hex RVA/VA on either side. A
bare hex is a target/base RVA and a target/base VA at once; every reading that
hits is listed and the command stops rather than picking one.

ADDRESSES. THE RULE, and it holds across the whole toolchain: every absolute
address that gets PRINTED says which space it is in. Two are in play and they
differ by the 0x10000 image base, so an unlabelled number is a 64 KB error
waiting to happen - paste an RVA where a VA belongs and you land inside a
DIFFERENT function, with nothing to complain about it:

    rva=      what the rich indexes and the ledger STORE; pdb_fetch's `--rva`
    va=       rva + image_base (0x10000) - what IDA shows and what carcass
              comments and bug reports quote; pdb_fetch's `--va`
    +0xNN     a function-RELATIVE offset (xref call sites, block starts). Never
              an address, and never image-based.

`sema rva` prints BOTH forms of each side's address so the reader never has to
do the arithmetic; pdb_fetch labels its own headers the same way and names its
structure-diff columns `t.va`/`b.va`. Its FLAGS (`--rva`, `--va`, `--address`,
`--offset`) say which kind they take and are unchanged by any of this. A stale
`nix develop` shell can still hold a pdb_fetch that prints bare addresses - the
NUMBERS are the same either way, so a value comparison against it stays valid.

rc: 0 = answered YES, 1 = answered NO, 2 = error. WHAT the answer is about is
per verb: `blocks --diff` answers "same flow AND every aligned block identical",
so it returns 1 for nearly every function below 100% - including ones it just
printed `flow SAME` for. `branches --diff` answers "branch counts, mnemonics and
destination blocks agree". The other verbs always return 0. Take the flow
verdict from the printed `flow SAME | DIFFERS` line, never from rc.
"""

import argparse
import sys


#: what every verb accepts as <fn>. Spelled out because the one thing a reader
#: must not have to guess is which KIND of address a bare hex means.
_FN_HELP = ("mangled name, demangled substring, or a hex address; a bare hex is "
            "read as a target/base RVA AND a target/base VA (they differ by the "
            "0x10000 image base), so every reading that hits is listed instead of "
            "one being picked")


def die(msg):
    sys.stderr.write(f"sema: {msg}\n")
    sys.exit(2)


def main():
    from vostok.sema.blocks import cmd_blocks
    from vostok.sema.branches import cmd_branches
    from vostok.sema.dot import cmd_dot
    from vostok.sema.rva import cmd_rva
    from vostok.sema.strings import cmd_strings
    from vostok.sema.sweep import cmd_sweep
    from vostok.sema.xref import cmd_xref

    ap = argparse.ArgumentParser(
        description=__doc__.split("\n\n")[0],
        formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("rva", help="address, source, and match dossier")
    p.add_argument("fn", help=_FN_HELP)
    p.set_defaults(func=cmd_rva)
    p = sub.add_parser("xref", help="direct target/base caller-callee graph")
    p.add_argument("fn", help=_FN_HELP)
    p.add_argument("--callees", action="store_true", help="show outgoing calls")
    p.add_argument("--base", action="store_true", help="read the base side")
    p.add_argument("--raw", action="store_true", help="show every call site")
    p.set_defaults(func=cmd_xref)
    p = sub.add_parser("strings", help="per-function literals or reverse lookup")
    p.add_argument("fn", nargs="?", help=_FN_HELP)
    p.add_argument("--find", metavar="TEXT", help="list functions referencing a literal")
    p.add_argument("--base", action="store_true", help="read the base side")
    p.set_defaults(func=cmd_strings)
    views = (
        ("blocks", cmd_blocks, "basic-block CFG, or THE verdict view with --diff"),
        ("branches", cmd_branches, "ordered branch sequence; read a difference blocks found"),
        ("dot", cmd_dot, "the CFG as graphviz on stdout (notes go to stderr)"),
    )
    for name, fn, blurb in views:
        p = sub.add_parser(name, help=blurb)
        p.add_argument("fn", help=_FN_HELP)
        p.add_argument("--base", action="store_true",
                       help="show the base side (default: target); ignored with --diff, "
                            "which always shows both")
        p.add_argument("--diff", action="store_true", help="compare base against target")
        if name == "blocks":
            p.add_argument("--lite", action="store_true",
                           help="one line per block, no instruction bodies")
        p.set_defaults(func=fn, lite=False)
    p = sub.add_parser("sweep", help="classify a whole module/TU by flow verdict")
    p.add_argument("--module", help="module name, e.g. render (default: every module)")
    p.add_argument("--unit", help="substring of the TU path")
    p.add_argument("--min-pct", type=float, help="skip functions below this fuzzy %%")
    p.add_argument("--max", type=int, help="stop after N functions (highest fuzzy %% first)")
    p.set_defaults(func=cmd_sweep)
    args = ap.parse_args()
    sys.exit(args.func(args))


if __name__ == "__main__":
    main()
