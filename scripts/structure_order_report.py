#!/usr/bin/env python3
"""structure_order_report - per-module TARGET-vs-BASE class-structure diff.

The engine matches "structure first": a class's on-disk layout (member order,
offsets, total size) and its method-declaration list are part of that structure,
and getting them wrong is what drives downstream inline/codegen divergence. This
script rolls up, per non-render module, every place TARGET and BASE diverge on a
*class definition*.

SOURCE: the Rust pdb_parser tool (wrapped by scripts/generate_structure.py) renders
each PDB's classes as annotated C++ under binaries/structure/{base,target}/headers/
vostok. We re-use that output for BOTH PDBs and diff, per class present on both
sides:

  methods  : declaration ORDER (LCS) + per-signature divergence (different arg
             COUNT, static/const/virtual/inline attrs) + presence (a method on
             one side only)
  members  : declaration ORDER (LCS) + presence (a field MISSING on a side)
             + per-member OFFSET / TYPE mismatch
  size     : STATIC_SIZE_ASSERT total mismatch

(The COMDAT emitted-function-order signal - paired target_rva vs base_rva - was
intentionally dropped: it is noisy and far less actionable than the class diff.)

Classes are scoped to a module by the namespace head of their qualified name
(vostok::sound::x -> sound); template containers with no module namespace are not
module-owned source and are dropped from the per-module rollup.

Default run prints a human-readable, ranked report over the in-scope modules.
--regen first re-runs pdb_parser for both sides (slow; do not use while a build
runs). --json emits the structured data instead.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

VOSTOK = Path(__file__).resolve().parent.parent
STRUCTURE = VOSTOK / "binaries" / "structure"
GEN_STRUCTURE = VOSTOK / "scripts" / "generate_structure.py"

# Modules we actively match and care about here. render is matched LAST and is the
# most optimized, so its structure noise would swamp the signal; everything else
# excluded is third-party built verbatim from source (exact by construction).
IN_SCOPE = [
    "ui", "fs", "input", "logging", "vfs", "debug", "collision",
    "particle", "engine", "sound", "ai", "ai_navigation", "animation",
    "survarium", "vostok",
]


# --------------------------------------------------------------------------- LCS

def lcs_len(a: list, b: list) -> int:
    """Length of the longest common subsequence of two sequences (O(n*m))."""
    if not a or not b:
        return 0
    prev = [0] * (len(b) + 1)
    for x in a:
        cur = [0]
        for j, y in enumerate(b):
            cur.append(prev[j] + 1 if x == y else max(prev[j + 1], cur[j]))
        prev = cur
    return prev[-1]


def out_of_order(target_order: list, base_order: list) -> int:
    """Items out of place: total - LCS(target, base). 0 == identical order.

    The minimum number of items to relocate to make base order equal target
    order - a stable, symmetric structural distance.
    """
    return len(target_order) - lcs_len(target_order, base_order)


# ----------------------------------------------------------------- stub parsing

# A member-var line:   \t/* 0x0008 */\t<type...>   <name>;
MEMBER_RE = re.compile(r"/\*\s*(0x[0-9A-Fa-f]+)\s*\*/\s*(.+?)\s+([A-Za-z_]\w*)\s*;")
# A base-class region:  \t/* 0x0000 */\t/* core::noncopyable */   (no real field)
BASE_REGION_RE = re.compile(r"/\*\s*0x[0-9A-Fa-f]+\s*\*/\s*/\*.*\*/\s*$")
SIZE_RE = re.compile(r"STATIC_SIZE_ASSERT\(\s*([^,]+?)\s*,\s*(0x[0-9A-Fa-f]+|\d+)\s*\)")
NAMESPACE_RE = re.compile(r"^\s*namespace\s+([A-Za-z_]\w*)\s*\{")
ATTR_RE = re.compile(r"\b(inline|static|virtual)\b")


@dataclass
class Member:
    offset: int
    type: str
    name: str


@dataclass
class Method:
    sig: str            # name/argc - the identity used for order LCS + matching
    attrs: frozenset    # inline / static / virtual / const qualifiers
    name: str
    argc: int


@dataclass
class ClassDef:
    qual: str
    size: int | None
    members: list       # list[Member] in declaration (== source) order
    methods: list       # list[Method] in declaration order
    namespaces: tuple = ()  # enclosing namespace stack, e.g. ("vostok", "sound")


def _arg_count(args: str) -> int:
    """Top-level comma-separated arg count, ignoring commas inside <>/()."""
    args = args.strip()
    if not args or args == "void":
        return 0
    depth = 0
    n = 1
    for ch in args:
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        elif ch == "," and depth == 0:
            n += 1
    return n


def _parse_method(raw: str) -> Method | None:
    """Parse a method-declaration stub line, or None if it is not one.

    A method line ends in an inline body `{`; its arg-list `(` opens at angle-depth
    0 (NOT inside a template `<...>` - templated ctor/dtor names like
    `intrusive_list<...>(...)` would otherwise mis-capture a template-arg fragment
    as the name). We take the identifier immediately preceding that paren as the
    method name (ctor/dtor included), and the trailing `const` as an attribute.
    """
    if "{" not in raw or "(" not in raw:
        return None
    # locate the arg-list open paren at angle-depth 0
    angle = 0
    open_idx = -1
    for i, ch in enumerate(raw):
        if ch == "<":
            angle += 1
        elif ch == ">":
            angle = max(0, angle - 1)
        elif ch == "(" and angle == 0:
            open_idx = i
            break
    if open_idx < 0:
        return None
    # matching close paren (paren-balanced), then a body `{` after optional `const`
    depth = 0
    close_idx = -1
    for i in range(open_idx, len(raw)):
        if raw[i] == "(":
            depth += 1
        elif raw[i] == ")":
            depth -= 1
            if depth == 0:
                close_idx = i
                break
    if close_idx < 0:
        return None
    tail = raw[close_idx + 1:]
    if "{" not in tail:
        return None
    is_const = bool(re.match(r"\s*const\b", tail))
    # identifier just before the arg-list paren (operators keep their symbol run)
    head = raw[:open_idx].rstrip()
    nm = re.search(r"(operator\s*\S+|~?[A-Za-z_]\w*)\s*$", head)
    if not nm:
        return None
    name = re.sub(r"\s+", "", nm.group(1))
    attrs = frozenset(ATTR_RE.findall(head))
    if is_const:
        attrs |= {"const"}
    argc = _arg_count(raw[open_idx + 1:close_idx])
    return Method(f"{name}/{argc}", attrs, name, argc)


def parse_structure_header(text: str) -> ClassDef | None:
    """Pull the class (with its STATIC_SIZE_ASSERT) out of one structure stub.

    The stubs hold one class definition per header; methods/members are collected
    in file order, which is declaration == source order. The namespace stack is
    captured so we can scope the class to its owning module. Returns None for a
    body-only stub (no size assert -> not a class definition).
    """
    members: list[Member] = []
    methods: list[Method] = []
    namespaces: list[str] = []
    for raw in text.splitlines():
        sm = SIZE_RE.search(raw)
        if sm:
            return ClassDef(sm.group(1), int(sm.group(2), 0), members, methods,
                            tuple(namespaces))
        ns = NAMESPACE_RE.match(raw)
        if ns:
            namespaces.append(ns.group(1))
            continue
        # base-class region first: it is annotated like a member but is not a
        # steerable own-field (the inner class carries its own assert).
        if BASE_REGION_RE.search(raw):
            continue
        mm = MEMBER_RE.search(raw)
        if mm and raw.lstrip().startswith("/*"):
            members.append(Member(int(mm.group(1), 0), mm.group(2).strip(), mm.group(3)))
            continue
        meth = _parse_method(raw)
        if meth:
            methods.append(meth)
    return None


def _load_classes(side_dir: Path) -> dict[str, ClassDef]:
    """Map header filename -> ClassDef for one side's headers/vostok stubs."""
    hdr = side_dir / "headers" / "vostok"
    out: dict[str, ClassDef] = {}
    if not hdr.is_dir():
        return out
    for f in hdr.glob("*.h"):
        cd = parse_structure_header(f.read_text(errors="replace"))
        if cd:
            out[f.name] = cd
    return out


def _segment_module(parts: list[str]) -> str | None:
    """In-scope module from a `::`-split name: the segment under `vostok`, or head."""
    if not parts:
        return None
    head = parts[1] if parts[0] == "vostok" and len(parts) > 1 else parts[0]
    return head if head in IN_SCOPE else None


def _module_of(cd: ClassDef) -> str | None:
    """Owning in-scope module of a class, else None.

    The QUALIFIED class name is authoritative (survarium::game_world_object_list is
    survarium's, even though pdb_parser emits it inside a `namespace vostok` block).
    Fall back to the enclosing namespace stack, which covers names the size-assert
    strips down to a bare identifier (e.g. `game_test_suite` -> vostok). A bare
    `vostok` with no module sub-namespace is the catch-all `vostok` module.
    """
    qparts = [p for p in cd.qual.split("<")[0].strip().split("::") if p]
    by_qual = _segment_module(qparts)
    if by_qual:
        return by_qual
    by_ns = _segment_module(list(cd.namespaces))
    if by_ns:
        return by_ns
    # bare vostok (own namespace or qual) -> catch-all vostok module
    if (cd.namespaces and cd.namespaces[0] == "vostok") or (qparts and qparts[0] == "vostok"):
        return "vostok"
    return None


# --------------------------------------------------------------------- diffing

@dataclass
class ClassDiff:
    qual: str
    header: str
    size_t: int | None
    size_b: int | None
    method_ooo: int            # method declaration-order divergence
    method_only_t: list        # methods present only in target (name/argc)
    method_only_b: list
    method_sig_changed: list   # (sig, "what changed")
    member_ooo: int            # member declaration-order divergence
    member_only_t: list        # fields present only in target ("missing in base")
    member_only_b: list
    member_changed: list       # (name, "offset/type change")

    def is_clean(self) -> bool:
        return not (
            self.size_t != self.size_b
            or self.method_ooo or self.method_only_t or self.method_only_b
            or self.method_sig_changed or self.member_ooo
            or self.member_only_t or self.member_only_b or self.member_changed
        )

    def score(self) -> int:
        """Severity ranking for the per-module sort (size + missing weigh most)."""
        return (
            (4 if self.size_t != self.size_b else 0)
            + len(self.member_only_t) + len(self.member_only_b) + len(self.member_changed)
            + self.member_ooo + self.method_ooo
            + len(self.method_only_t) + len(self.method_only_b) + len(self.method_sig_changed)
        )


def _diff_class(qual: str, header: str, t: ClassDef, b: ClassDef) -> ClassDiff:
    # --- members ---
    t_mem = {m.name: m for m in t.members}
    b_mem = {m.name: m for m in b.members}
    only_t = [m.name for m in t.members if m.name not in b_mem]
    only_b = [m.name for m in b.members if m.name not in t_mem]
    changed = []
    for name, tm in ((n, m) for n, m in t_mem.items() if n in b_mem):
        bm = b_mem[name]
        if tm.offset != bm.offset:
            changed.append((name, f"offset 0x{tm.offset:x}->0x{bm.offset:x}"))
        elif tm.type != bm.type:
            changed.append((name, f"type {tm.type!r}->{bm.type!r}"))
    # order over fields common to both sides (presence handled above)
    member_ooo = out_of_order(
        [m.name for m in t.members if m.name in b_mem],
        [m.name for m in b.members if m.name in t_mem],
    )

    # --- methods (identity = name/argc; attrs compared separately) ---
    t_meth = {m.sig: m for m in t.methods}
    b_meth = {m.sig: m for m in b.methods}
    m_only_t = [m.sig for m in t.methods if m.sig not in b_meth]
    m_only_b = [m.sig for m in b.methods if m.sig not in t_meth]
    sig_changed = []
    for sig in (s for s in t_meth if s in b_meth):
        if t_meth[sig].attrs != b_meth[sig].attrs:
            diff = t_meth[sig].attrs ^ b_meth[sig].attrs
            sig_changed.append((sig, "attrs " + ",".join(sorted(diff))))
    method_ooo = out_of_order(
        [m.sig for m in t.methods if m.sig in b_meth],
        [m.sig for m in b.methods if m.sig in t_meth],
    )

    return ClassDiff(
        qual, header, t.size, b.size,
        method_ooo, m_only_t, m_only_b, sig_changed,
        member_ooo, only_t, only_b, changed,
    )


def class_diff_report() -> dict[str, list[ClassDiff]]:
    """Per in-scope module, the structural diff of each class on both sides."""
    t_cls = _load_classes(STRUCTURE / "target")
    b_cls = _load_classes(STRUCTURE / "base")
    if not t_cls or not b_cls:
        sys.exit(
            f"structure stubs missing under {STRUCTURE} - run "
            "`structure_order_report.py --regen` (inside nix develop), or "
            "scripts/generate_structure.py {base,target}"
        )
    out: dict[str, list[ClassDiff]] = {m: [] for m in IN_SCOPE}
    for header in set(t_cls) & set(b_cls):
        tc, bc = t_cls[header], b_cls[header]
        module = _module_of(tc)
        if module is None:
            continue  # template container / non-module-owned class
        d = _diff_class(tc.qual, header, tc, bc)
        if not d.is_clean():
            out[module].append(d)
    for lst in out.values():
        lst.sort(key=lambda d: d.score(), reverse=True)
    return out


# ------------------------------------------------------------------- rendering

def _print_text(report: dict[str, list[ClassDiff]], modules: list[str]) -> None:
    print("=" * 78)
    print("CLASS-DEFINITION DIFF  (pdb_parser structure stubs, TARGET vs BASE)")
    print("  methods: order + arg-count/attr/presence ; members: order/offset/size/presence")
    print("=" * 78)
    for m in modules:
        diffs = report.get(m, [])
        n_size = sum(1 for d in diffs if d.size_t != d.size_b)
        n_missing = sum(
            len(d.member_only_t) + len(d.member_only_b)
            + len(d.method_only_t) + len(d.method_only_b)
            for d in diffs
        )
        print(f"\n[{m}]  classes-differing={len(diffs)}  size-mismatches={n_size}  "
              f"missing methods/fields={n_missing}")
        for d in diffs:
            bits = []
            if d.size_t != d.size_b:
                st = f"0x{d.size_t:x}" if d.size_t is not None else "?"
                sb = f"0x{d.size_b:x}" if d.size_b is not None else "?"
                bits.append(f"SIZE {st}->{sb}")
            if d.member_ooo:
                bits.append(f"member-order {d.member_ooo}")
            if d.method_ooo:
                bits.append(f"method-order {d.method_ooo}")
            print(f"  {d.qual}   [{', '.join(bits) or 'detail below'}]")
            for fld in d.member_only_t:
                print(f"        + field {fld} (TARGET only - MISSING in base)")
            for fld in d.member_only_b:
                print(f"        - field {fld} (BASE only - not in target)")
            for name, what in d.member_changed[:4]:
                print(f"        ~ field {name}: {what}")
            for sig in d.method_only_t[:4]:
                print(f"        + method {sig} (TARGET only)")
            for sig in d.method_only_b[:4]:
                print(f"        - method {sig} (BASE only)")
            for sig, what in d.method_sig_changed[:4]:
                print(f"        ~ method {sig}: {what}")


def _to_json(report: dict[str, list[ClassDiff]], modules: list[str]) -> dict:
    return {
        m: [
            {
                "qual": d.qual, "header": d.header,
                "size_target": d.size_t, "size_base": d.size_b,
                "method_order_divergence": d.method_ooo,
                "method_only_target": d.method_only_t,
                "method_only_base": d.method_only_b,
                "method_sig_changed": d.method_sig_changed,
                "member_order_divergence": d.member_ooo,
                "field_only_target": d.member_only_t,
                "field_only_base": d.member_only_b,
                "field_changed": d.member_changed,
            }
            for d in report.get(m, [])
        ]
        for m in modules
    }


def _regen() -> None:
    py = sys.executable or "python3"
    for side in ("target", "base"):
        print(f"[regen] generating {side} structure stubs via pdb_parser ...",
              file=sys.stderr)
        subprocess.run([py, str(GEN_STRUCTURE), side], check=True, env={**os.environ})


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("--module", help="restrict the report to one in-scope module")
    ap.add_argument("--json", action="store_true", help="emit JSON instead of text")
    ap.add_argument("--regen", action="store_true",
                    help="re-run pdb_parser for both PDBs first (slow; not while building)")
    args = ap.parse_args()

    if args.regen:
        _regen()

    modules = IN_SCOPE
    if args.module:
        if args.module not in IN_SCOPE:
            sys.exit(f"--module {args.module!r} not in scope {IN_SCOPE}")
        modules = [args.module]

    report = class_diff_report()
    if args.json:
        json.dump(_to_json(report, modules), sys.stdout, indent=2)
        sys.stdout.write("\n")
    else:
        _print_text(report, modules)


if __name__ == "__main__":
    main()
