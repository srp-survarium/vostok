"""vostok - the umbrella CLI over the matching toolchain.

    vostok build [ninja args...]   THE canonical build: ninja under Wine, then
                                   delink + structure + rich, then the derive
                                   and ledger refresh and the README block
    vostok ledger <verb>           the committed campaign record: report / list
                                   / queue / tried / park / open, plus `readme`
                                   and `mismatch-queue`
    vostok derive refresh          re-derive the ledger from an already-built
                                   report; the one verb, since the derivation
                                   now writes the record rather than a cache
    vostok sema <view>             read-only control-flow views over one
                                   base<->target function pair
    vostok diff <view>             target-vs-base source shape: layout / order
                                   / tu-order / enums
    vostok tool <name>             one external tool: clangd / toolchain / libs

Every module is also a direct entry (`python3 -m vostok.sema xref ...`), which
is the only spelling for the ones this umbrella does not name: the build steps
(`vostok.build.ninja`, `.ninja_regen`, `.generate_{delink,structure,rich}`,
`.gfx`), `vostok.ledger.store` and `vostok.core.symbols`.
"""

from __future__ import annotations

import importlib
import sys

DIFF = {"layout": "vostok.diff.layout", "order": "vostok.diff.order",
        "tu-order": "vostok.diff.tu_order", "enums": "vostok.diff.enums",
        "enums-compare": "vostok.diff.enums_compare"}
TOOLS = {"clangd": "vostok.tool.clangd", "toolchain": "vostok.tool.toolchain",
         "toolchain-release": "vostok.tool.toolchain_release",
         "libs": "vostok.tool.libs", "sizes": "vostok.tool.sizes",
         "breakpoints": "vostok.tool.breakpoints"}


def _run(module: str, argv: list[str], prog: str) -> int:
    sys.argv = [prog, *argv]
    return importlib.import_module(module).main() or 0


def main(argv: list[str] | None = None) -> int:
    argv = list(sys.argv[1:] if argv is None else argv)
    if not argv or argv[0] in ("-h", "--help", "help"):
        # `python3 -m vostok` with no verb is documented (CLAUDE.md, README) as
        # "a map of the whole surface", so it is a successful informational run,
        # not a usage error - same output and same rc as `--help`.
        print(__doc__.strip())
        return 0
    cmd, rest = argv[0], argv[1:]
    if cmd == "build":
        return _run("vostok.build.rebuild", rest, "vostok build")
    if cmd in ("ledger", "derive", "sema"):
        mod = {"ledger": "vostok.ledger", "derive": "vostok.derive.cli",
               "sema": "vostok.sema"}[cmd]
        sys.argv = [f"vostok {cmd}", *rest]
        return importlib.import_module(mod).main() or 0
    for name, table in (("diff", DIFF), ("tool", TOOLS)):
        if cmd != name:
            continue
        if not rest or rest[0] not in table:
            print(f"vostok {name}: pick one of {', '.join(sorted(table))}",
                  file=sys.stderr)
            return 2
        return _run(table[rest[0]], rest[1:], f"vostok {name} {rest[0]}")
    print(f"vostok: unknown command {cmd!r} (see `vostok --help`)",
          file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
