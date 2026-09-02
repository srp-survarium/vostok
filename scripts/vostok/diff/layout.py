#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Header/type-layout diff: the class-layout analog of an objdiff function diff.

Compares the TARGET class/struct layouts (the PDB-generated stub headers under
``binaries/structure/target/headers``) against our BASE headers (``sources/
vostok/**/*.h``) and reports four defect classes per type:

  (1) SIZE MISMATCH  - base STATIC_SIZE_ASSERT size != target size (the
      ``queries_result``-was-8-bytes-too-large bug class). Highest priority:
      a wrong size caps EVERY consumer that walks the type. Also flags a type
      that the target asserts a size for but base never STATIC_SIZE_ASSERTs.
  (2) MISSING FIELDS - a data member in the target layout (name + offset) that
      the base struct/class does not declare.
  (3) MISSING METHODS - a method declared on the target class that the base
      header does not declare (matched by name + arity, since the spelling of
      argument types differs between the two formats - e.g. a ``query_callback``
      typedef vs the expanded ``boost::function<...>``). These frequently equal
      the TARGET_ONLY functions a function-level investigation is mapping.
  (4) MISSING TYPES  - a target class/struct in vostok::/survarium:: that has NO
      corresponding base type at all (no base header declares the FQN).

Match key is the fully-qualified type name (FQN); base header file paths do not
mirror the target's namespace directory layout, so we key on the name the parser
reconstructs from each side's ``namespace`` / ``class`` / ``struct`` nesting.

Scope: only ``vostok::`` and ``survarium::`` types are considered (third-party
template instantiations parked in ``headers/others`` live in boost/stlport/...
namespaces and are skipped). The RENDER module is excluded entirely - the
``vostok/render`` target subtree and any ``vostok::render::*`` FQN are dropped
(rendering is not matched yet, so a lot of layout churn is expected there).

Usage:
    python3 -m vostok diff layout                 # full ranked report, all
                                                   #   non-render modules
    python3 -m vostok diff layout --module resources
                                                   # one target module (the
                                                   #   first namespace segment
                                                   #   under vostok/ or the
                                                   #   'survarium' bucket)
    python3 -m vostok diff layout --type vostok::resources::queries_result
                                                   # one fully-qualified type
    python3 -m vostok diff layout --json out.json # machine-readable dump
    python3 -m vostok diff layout --only size     # one defect class:
                                                   #   size|fields|methods|types

Run from the repo root (or anywhere - paths are resolved relative to this file).
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

from vostok.core.paths import ENGINE as BASE_HEADERS
from vostok.core.paths import REPO as REPO_ROOT
from vostok.core.paths import TARGET_HEADERS
from vostok.core import log as _log

# Namespaces we care about; everything else (boost, stlport, std, ...) is skipped.
WANTED_ROOTS = ("vostok", "survarium")

# ----------------------------------------------------------------------------
# Parsing
# ----------------------------------------------------------------------------


@dataclass
class TypeInfo:
    fqn: str
    kind: str  # 'class' or 'struct'
    size: int | None = None  # primary (first-seen) STATIC_SIZE_ASSERT size
    sizes: set[int] = field(default_factory=set)  # ALL asserted sizes for the FQN
    methods: dict[str, set[int]] = field(default_factory=dict)  # name -> {arities}
    fields: dict[str, int | None] = field(default_factory=dict)  # name -> offset
    field_order: list[str] = field(default_factory=list)
    source: str = ""  # originating header path (for reporting)


# A class/struct opener head: optional leading template<>/API macro, then the
# keyword and the (possibly templated) name. The opening brace may be on the
# same line or a later line (base side often puts ': public ...' then '{' on the
# next line), so we do NOT anchor on '{' here - the scanner finds it.
_OPEN_RE = re.compile(
    r"(?:^|\s)(?:template\s*<[^>]*>\s*)?(class|struct)\s+"
    r"(?:[A-Z_][A-Z0-9_]*_API\s+)?"  # e.g. VOSTOK_CORE_API
    r"([A-Za-z_]\w*(?:\s*<[^>]*>)?)"  # name, optional <...> template args
    r"\s*(?!::|\w)"  # name must NOT be followed by '::' or more identifier
    r"(?:;|$|:(?!:)|\{)"  # forward-decl(;), eol, base-list(: not ::) or brace
)

# STATIC_SIZE_ASSERT(type, 0xNN) - type may carry template args / spaces.
_SIZE_RE = re.compile(r"STATIC_SIZE_ASSERT\s*\(\s*(.+?)\s*,\s*(0[xX][0-9A-Fa-f]+|\d+)\s*\)")

# A field with an explicit offset comment (target format):
#   /* 0x0008 */  boost::function<...>  m_callback;
# We capture the offset and the trailing identifier (the member name). Skip
# base-subobject markers /* 0x.. */ /* base */ (no real member name).
_FIELD_OFFSET_RE = re.compile(
    r"/\*\s*0[xX]([0-9A-Fa-f]+)\s*\*/\s*(.+?)\s*;\s*$"
)

# Strip a trailing array subscript from a member name: m_buffer[46] -> m_buffer.
_ARRAY_RE = re.compile(r"\[[^\]]*\]\s*$")


def _strip_block_comments(text: str) -> str:
    """Remove /* ... */ comments that are NOT offset markers, and // comments.

    Offset markers (/* 0x.. */) are preserved so the field regex can read them;
    everything else (doc comments, the /* base */ markers, /* no source */) is
    dropped so it never masquerades as a declaration. Applied line-wise on the
    base side only - the target's field offsets must survive.
    """
    # Drop // line comments.
    text = re.sub(r"//[^\n]*", "", text)
    # Drop /* ... */ that do not start with an 0x offset.
    text = re.sub(r"/\*(?!\s*0[xX])[^*]*(?:\*(?!/)[^*]*)*\*/", "", text)
    # Drop preprocessor directive lines (#if / #pragma / #endif / ...). These
    # ride alongside members (e.g. the warning(disable:4200) around a flexible
    # array) and would otherwise inject stray '(' that masks the real field.
    text = re.sub(r"(?m)^\s*#.*$", "", text)
    return text


def _arity(arglist: str) -> int:
    """Count comma-separated top-level args in a parenthesized arg list body."""
    s = arglist.strip()
    if s == "" or s == "void":
        return 0
    depth = 0
    count = 1
    for ch in s:
        if ch in "<([{":
            depth += 1
        elif ch in ">)]}":
            depth -= 1
        elif ch == "," and depth == 0:
            count += 1
    return count


# A method DECLARATION head: the identifier (or ctor/dtor/operator) immediately
# before the argument list, then '( args )' [const] then ';' or '{' (skeleton
# form after inline bodies are stripped). The name must be a *simple* identifier
# (no '::') - a qualified name is a call/definition from another scope.
_METHOD_RE = re.compile(
    r"(?:^|[\s*&>])"  # boundary before the name (return type / access)
    r"(~?[A-Za-z_]\w*"  # simple name, ctor/dtor ...
    r"|operator\s*(?:\(\s*\)|\[\s*\]|[^\s(\[]+))"  # ... or an operator (incl () [])
    r"\s*\(([^;{}]*?)\)\s*(?:const)?\s*(?:=\s*0\s*)?[;{]"
)

# Keywords / control words that can sit before '(' but are not methods. The
# fundamental type names guard against an 'operator char()' conversion operator
# being mis-captured as a method literally named 'char' (the 'operator' lands in
# the return-type position); such conversions are rare and parse symmetrically.
_NOT_METHOD = {
    "if", "for", "while", "switch", "return", "sizeof", "do", "else",
    "typedef", "static_cast", "reinterpret_cast", "const_cast", "dynamic_cast",
    "char", "short", "int", "long", "float", "double", "bool", "void",
    "unsigned", "signed", "u8", "u16", "u32", "u64", "s8", "s16", "s32", "s64",
}


def _strip_inline_bodies(body: str) -> str:
    """Remove the contents of balanced '{...}' blocks (inline method bodies,
    nested enum/struct bodies, member-init blocks) so only declaration skeletons
    remain. Each removed block collapses to a single ';' so the preceding decl
    still terminates cleanly. The OUTERMOST class braces are not present in the
    body string we receive (it starts after the class '{'), so depth starts at 0
    and every '{' here is an inner body to strip."""
    out: list[str] = []
    depth = 0
    for ch in body:
        if ch == "{":
            if depth == 0:
                out.append(";")  # terminate the decl whose body we are dropping
            depth += 1
        elif ch == "}":
            if depth > 0:
                depth -= 1
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def _parse_methods_and_fields(body: str, ti: TypeInfo, is_target: bool) -> None:
    """Extract methods (name+arity) and fields from one class body string."""
    # --- target fields: trust the explicit /* 0xNN */ offset markers ---
    if is_target:
        for line in body.splitlines():
            stripped = line.strip()
            m = _FIELD_OFFSET_RE.search(stripped)
            if not m:
                continue
            decl = m.group(2).strip()
            if not decl:  # /* base */ marker (comment-stripped to empty) etc.
                continue
            # A field decl never ends in a parameter list: 'name ( ... )'.
            if re.search(r"\)\s*$", decl) and "(" in decl.split("=")[0]:
                # could be a function-typed member only if the '(' is inside the
                # TYPE (e.g. function<void(..)>); reject true method skeletons.
                if re.search(r"\b[A-Za-z_]\w*\s*\([^)]*\)\s*$", decl):
                    continue
            name = _member_name_from_decl(decl)
            if name:
                ti.fields.setdefault(name, int(m.group(1), 16))
                if name not in ti.field_order:
                    ti.field_order.append(name)

    # --- methods: scan declaration skeletons (inline bodies stripped) ---
    clean = _strip_block_comments(body)
    skeleton = _strip_inline_bodies(clean)
    flat = re.sub(r"\s+", " ", skeleton)

    for m in _METHOD_RE.finditer(flat):
        name = m.group(1).strip()
        if name in _NOT_METHOD or not name:
            continue
        if name.startswith("operator"):
            name = re.sub(r"\s+", "", name)
        ti.methods.setdefault(name, set()).add(_arity(m.group(2)))

    # --- base fields: member decls ending in ';' that are not methods ---
    if not is_target:
        for raw in skeleton.split(";"):
            s = raw.strip()
            # Drop preprocessor lines (#if/#pragma/#endif) that ride along.
            s = re.sub(r"^(?:\s*#[^\n]*\n?)+", "", s, flags=re.MULTILINE)
            s = " ".join(s.split())
            # Strip a leading access specifier ('public:' / 'private:' / ...).
            s = re.sub(r"^(?:public|private|protected)\s*:\s*", "", s)
            if not s or "(" in s or "{" in s or "}" in s:
                continue
            low = s.split()
            if not low or low[0] in (
                "typedef", "using", "friend", "enum", "class", "struct",
                "public", "private", "protected", "template", "return",
                "static_assert", "mutable",
            ):
                continue
            if ":" in s and "::" not in s:  # leftover access label
                continue
            name = _member_name_from_decl(s)
            if name and name not in ti.fields:
                ti.fields[name] = None
                ti.field_order.append(name)


def _member_name_from_decl(decl: str) -> str | None:
    """Pull the member identifier out of a declaration like 'u32 m_size' or
    'query_result m_queries[0]' or 'boost::function<...> m_callback'."""
    decl = decl.strip().rstrip("=").strip()
    decl = _ARRAY_RE.sub("", decl).strip()
    # Drop an initializer ' = ...'.
    decl = decl.split("=", 1)[0].strip()
    # The member name is the last identifier-looking token, sans leading '*'/'&'.
    m = re.search(r"([A-Za-z_]\w*)\s*$", decl)
    if not m:
        return None
    name = m.group(1)
    # Reject obvious type keywords that slipped through as 'names'.
    if name in ("const", "void", "volatile", "mutable", "static", "inline"):
        return None
    # Must be preceded by something (a type) - a lone token is not a member.
    if decl.strip() == name:
        return None
    return name


_NS_OPEN_RE = re.compile(r"\bnamespace\s+(\w+)\s*\{")


def _strip_strings(line: str) -> str:
    """Blank out string/char literals so braces inside them never miscount."""
    line = re.sub(r'"(?:\\.|[^"\\])*"', '""', line)
    line = re.sub(r"'(?:\\.|[^'\\])*'", "''", line)
    return line


def _split_classes(text: str, fqn_prefix_stack: list[str]) -> list[tuple[TypeInfo, str]]:
    """Walk a header tracking namespace + brace depth, returning (TypeInfo, body)
    for every class/struct DEFINITION (forward decls 'class X;' are skipped).

    The scanner keeps a stack of scopes. A 'namespace N' scope pushes N onto the
    FQN prefix; a 'class/struct C' scope pushes C and starts capturing the body.
    Each scope records the brace depth it was opened at so we pop it on the
    matching '}'. The opening '{' of a class may be on a later line than the
    keyword (base side: ': public Base' then '{'), so a pending opener waits for
    its brace.
    """
    results: list[tuple[TypeInfo, str]] = []
    # scope entries: dict(kind=ns|class, name, depth_at_open, frame=<for class>)
    scopes: list[dict] = []
    for ns in fqn_prefix_stack:
        scopes.append({"kind": "ns", "name": ns, "depth": -1})

    depth = 0
    pending: dict | None = None  # an opener seen, waiting for its '{'
    lines = text.splitlines()
    i, n = 0, len(lines)

    def ns_prefix() -> list[str]:
        return [s["name"] for s in scopes if s["kind"] in ("ns", "class")]

    while i < n:
        raw = lines[i]
        # Strip comments from the SCANNING copy only (the stored body keeps the
        # raw line incl. /* 0xNN */ offset markers). Without this, a closing
        # '}; // struct foo' comment would match the class opener regex and set a
        # stale 'pending', stealing the next real class's '{' (lost damage_model).
        line = _strip_strings(raw)
        line = re.sub(r"//.*$", "", line)
        line = re.sub(r"/\*.*?\*/", "", line)
        col = 0
        # Walk tokens we care about left-to-right on this line so a brace and a
        # namespace/class on the same line are ordered correctly.
        while col < len(line):
            # If we are waiting for a class opener's brace, the next '{' opens it.
            if pending is not None:
                br = line.find("{", col)
                if br == -1:
                    break
                depth += 1
                frame = {"ti": pending["ti"], "body": [], "open_depth": depth}
                scopes.append(
                    {"kind": "class", "name": pending["ti"].fqn.split("::")[-1],
                     "depth": depth, "frame": frame}
                )
                pending = None
                col = br + 1
                continue

            # Look for the nearest interesting token from col.
            m_ns = _NS_OPEN_RE.search(line, col)
            m_open = _OPEN_RE.search(line, col)
            nb = line.find("{", col)
            nc = line.find("}", col)

            cands = [p for p in (
                (m_ns.start() if m_ns else -1, "ns"),
                (m_open.start() if m_open else -1, "cls"),
                (nb, "{"),
                (nc, "}"),
            ) if p[0] != -1]
            if not cands:
                break
            cands.sort()
            pos, kind = cands[0]

            if kind == "ns":
                scopes.append({"kind": "ns", "name": m_ns.group(1), "depth": depth + 1})
                depth += 1
                col = m_ns.end()
            elif kind == "cls":
                # Skip forward declarations: 'class X;' (next non-space is ';').
                tail = line[m_open.end() - 1:]
                if tail.lstrip().startswith(";"):
                    col = m_open.end()
                    continue
                ckind = m_open.group(1)
                cname = re.sub(r"\s+", "", m_open.group(2))
                fqn = "::".join(ns_prefix() + [cname]) if ns_prefix() else cname
                ti = TypeInfo(fqn=fqn, kind=ckind)
                pending = {"ti": ti}
                col = m_open.end()
                # The opener regex may have consumed the '{'; if so, open now.
                if m_open.group(0).rstrip().endswith("{"):
                    depth += 1
                    frame = {"ti": ti, "body": [], "open_depth": depth}
                    scopes.append({"kind": "class", "name": cname, "depth": depth, "frame": frame})
                    pending = None
            elif kind == "{":
                depth += 1
                col = pos + 1
            else:  # '}'
                depth -= 1
                col = pos + 1
                # Pop any scope opened at the depth we just left.
                while scopes and scopes[-1].get("depth", -1) == depth + 1:
                    sc = scopes.pop()
                    if sc["kind"] == "class":
                        _finish_class(sc["frame"], results)

        # Append the whole raw line to every active class frame's body.
        for sc in scopes:
            if sc["kind"] == "class":
                sc["frame"]["body"].append(raw)
        i += 1

    # Close any classes left open at EOF (defensive).
    while scopes:
        sc = scopes.pop()
        if sc["kind"] == "class":
            _finish_class(sc["frame"], results)
    return results


def _finish_class(frame: dict, results: list) -> None:
    ti: TypeInfo = frame["ti"]
    body = "\n".join(frame["body"])
    # Drop everything up to (and including) the class's own opening '{' so that
    # brace doesn't fool the inline-body stripper. The closing '}; // class' line
    # is never captured (the scanner pops the scope before appending it), so no
    # tail trim is needed - and trimming by rfind('}') would wrongly cut at the
    # last inline-body brace when the final member is a plain field.
    first = body.find("{")
    if first != -1:
        body = body[first + 1:]
    results.append((ti, body))


def parse_header(path: Path, is_target: bool) -> list[TypeInfo]:
    try:
        text = path.read_text(errors="replace")
    except OSError:
        return []

    # Collect STATIC_SIZE_ASSERTs keyed by normalized bare type name. A name may
    # be asserted more than once (the delinker emits foo.h / foo_1.h / foo_2.h
    # for distinct instantiations of the same FQN, each with its own size), so
    # keep the full set per name.
    sizes: dict[str, set[int]] = {}
    for m in _SIZE_RE.finditer(text):
        tname = re.sub(r"\s+", "", m.group(1))
        bare = tname.split("::")[-1]
        val = int(m.group(2), 16) if m.group(2).lower().startswith("0x") else int(m.group(2))
        sizes.setdefault(bare, set()).add(val)

    out: list[TypeInfo] = []
    for ti, body in _split_classes(text, []):
        bare = ti.fqn.split("::")[-1]
        if bare in sizes:
            ti.sizes = set(sizes[bare])
            ti.size = next(iter(sizes[bare]))
        _parse_methods_and_fields(body, ti, is_target)
        ti.source = str(path.relative_to(REPO_ROOT))
        out.append(ti)
    return out


# ----------------------------------------------------------------------------
# Collection
# ----------------------------------------------------------------------------


def _is_render(fqn: str, path_str: str) -> bool:
    if "::render::" in fqn or fqn.startswith("vostok::render::") or fqn == "vostok::render":
        return True
    # Target render subtree.
    if "/render/" in path_str.replace("\\", "/"):
        return True
    return False


def _wanted(fqn: str) -> bool:
    root = fqn.split("::", 1)[0]
    return root in WANTED_ROOTS


def collect_target() -> dict[str, TypeInfo]:
    types: dict[str, TypeInfo] = {}
    for root in ("vostok", "survarium"):
        base = TARGET_HEADERS / root
        if not base.is_dir():
            continue
        for path in base.rglob("*.h"):
            if "/render/" in str(path).replace("\\", "/"):
                continue
            for ti in parse_header(path, is_target=True):
                if not _wanted(ti.fqn) or _is_render(ti.fqn, ti.source):
                    continue
                # Prefer the first full definition seen; merge methods/fields.
                if ti.fqn in types:
                    _merge(types[ti.fqn], ti)
                else:
                    types[ti.fqn] = ti
    return types


def collect_base() -> dict[str, TypeInfo]:
    types: dict[str, TypeInfo] = {}
    for path in BASE_HEADERS.rglob("*.h"):
        if "/render/" in str(path).replace("\\", "/"):
            continue
        for ti in parse_header(path, is_target=False):
            if not _wanted(ti.fqn) or _is_render(ti.fqn, ti.source):
                continue
            if ti.fqn in types:
                _merge(types[ti.fqn], ti)
            else:
                types[ti.fqn] = ti
    return types


def _merge(dst: TypeInfo, src: TypeInfo) -> None:
    if dst.size is None and src.size is not None:
        dst.size = src.size
    dst.sizes |= src.sizes
    for name, arities in src.methods.items():
        dst.methods.setdefault(name, set()).update(arities)
    for name, off in src.fields.items():
        if name not in dst.fields or (dst.fields[name] is None and off is not None):
            dst.fields[name] = off
            if name not in dst.field_order:
                dst.field_order.append(name)


# ----------------------------------------------------------------------------
# Diffing
# ----------------------------------------------------------------------------


def module_of(fqn: str) -> str:
    parts = fqn.split("::")
    if parts[0] == "survarium":
        return "survarium"
    if parts[0] == "vostok" and len(parts) >= 2:
        return parts[1]
    return parts[0]


@dataclass
class Defects:
    fqn: str
    module: str
    base_size: int | None
    target_size: int | None
    size_delta: int | None
    missing_fields: list[tuple[str, int | None]]
    missing_methods: list[tuple[str, int]]
    base_missing_type: bool
    base_no_size_assert: bool
    is_template: bool = False  # FQN has <...> - generic base, not name-matchable
    name_elsewhere: bool = False  # bare name exists in base under another ns
    has_members: bool = False  # target type declares fields/methods (a real def)
    target_field_count: int = 0
    base_field_count: int = 0
    base_fields_unreliable: bool = False  # base parsed 0 fields but has methods
    # (anonymous union/struct or macro-defined layout the line parser can't see)


def _nearest_size(base_size: int | None, target_sizes: set[int]) -> int | None:
    """Pick the target size closest to base_size (when an FQN has several variant
    sizes from foo.h/foo_1.h/...). Returns None if no target size known."""
    if not target_sizes:
        return None
    if base_size is None:
        return next(iter(target_sizes))
    return min(target_sizes, key=lambda s: abs(s - base_size))


def diff(target: dict[str, TypeInfo], base: dict[str, TypeInfo]) -> list[Defects]:
    # Index base bare names so we can tell a true "missing type" apart from a
    # forward-ref stub the target generator parked in the wrong namespace.
    base_bare: dict[str, list[str]] = {}
    for fqn in base:
        base_bare.setdefault(fqn.split("::")[-1], []).append(fqn)

    out: list[Defects] = []
    for fqn, tti in target.items():
        bti = base.get(fqn)
        missing_type = bti is None
        is_template = "<" in fqn

        # Size: compare base's size against the NEAREST target variant size, so
        # an FQN with several recorded sizes (foo.h/foo_1.h/...) is flagged only
        # when base matches NONE of them.
        base_size = bti.size if bti else None
        target_size = _nearest_size(base_size, tti.sizes) if tti.sizes else None
        size_delta = None
        no_assert = False
        if target_size is not None:
            if base_size is not None:
                size_delta = base_size - target_size
            elif not missing_type:
                no_assert = True  # target asserts a size, base never does

        # Missing fields (target field not present in base by name).
        missing_fields: list[tuple[str, int | None]] = []
        if bti is not None:
            for fname in tti.field_order:
                if fname not in bti.fields:
                    missing_fields.append((fname, tti.fields.get(fname)))

        # Missing methods (target method name+arity not present in base).
        missing_methods: list[tuple[str, int]] = []
        if bti is not None:
            for mname, arities in tti.methods.items():
                base_arities = bti.methods.get(mname)
                for ar in sorted(arities):
                    if base_arities is None or ar not in base_arities:
                        # If base has the name with *some* arity, only flag the
                        # missing overload; tolerate arity off-by-default-args.
                        if base_arities and _arity_close(ar, base_arities):
                            continue
                        missing_methods.append((mname, ar))

        name_elsewhere = (
            missing_type and not is_template
            and fqn.split("::")[-1] in base_bare
        )

        if (
            missing_type
            or (size_delta not in (None, 0))
            or no_assert
            or missing_fields
            or missing_methods
        ):
            out.append(
                Defects(
                    fqn=fqn,
                    module=module_of(fqn),
                    base_size=base_size,
                    target_size=target_size,
                    size_delta=size_delta,
                    missing_fields=missing_fields,
                    missing_methods=missing_methods,
                    base_missing_type=missing_type,
                    base_no_size_assert=no_assert,
                    is_template=is_template,
                    name_elsewhere=name_elsewhere,
                    has_members=bool(tti.fields or tti.methods),
                    target_field_count=len(tti.fields),
                    base_field_count=len(bti.fields) if bti else 0,
                    base_fields_unreliable=bool(
                        bti and not bti.fields and bti.methods and tti.fields
                    ),
                )
            )
    return out


def _arity_close(target_ar: int, base_arities: set[int]) -> bool:
    """A method name present in base with an arity within +/-1 (default args /
    parse noise) is treated as present, to suppress spurious overload noise."""
    return any(abs(target_ar - b) <= 1 for b in base_arities)


# ----------------------------------------------------------------------------
# Reporting
# ----------------------------------------------------------------------------


# Cached "consumer" index: how many distinct target header files mention each
# bare type name (in a field type, signature, include, or forward-ref). A higher
# count = the type is walked/embedded by more code, so a layout bug in it caps
# more matching. Built once from the target corpus, lazily.
_REF_COUNTS: dict[str, int] | None = None


def _build_ref_counts() -> dict[str, int]:
    counts: dict[str, int] = {}
    ident = re.compile(r"[A-Za-z_]\w*")
    for root in ("vostok", "survarium"):
        base = TARGET_HEADERS / root
        if not base.is_dir():
            continue
        for path in base.rglob("*.h"):
            if "/render/" in str(path).replace("\\", "/"):
                continue
            try:
                text = path.read_text(errors="replace")
            except OSError:
                continue
            for name in set(ident.findall(text)):
                counts[name] = counts.get(name, 0) + 1
    return counts


def _consumer_weight(fqn: str) -> int:
    """How many target header files reference this type's bare name (a proxy for
    consumer count). Used to break ties when ranking by size/field-count delta."""
    global _REF_COUNTS
    if _REF_COUNTS is None:
        _REF_COUNTS = _build_ref_counts()
    # Subtract 1 for the type's own defining header.
    return max(0, _REF_COUNTS.get(fqn.split("::")[-1], 0) - 1)


def render_report(defects: list[Defects], target: dict[str, TypeInfo], only: str | None) -> str:
    lines: list[str] = []

    size_defs = [d for d in defects if d.size_delta not in (None, 0) or d.base_no_size_assert]
    field_defs = [d for d in defects if d.missing_fields]
    method_defs = [d for d in defects if d.missing_methods]
    type_defs = [d for d in defects if d.base_missing_type]

    if only in (None, "size"):
        lines.append("=" * 78)
        lines.append("(1) SIZE MISMATCH  -  base size != target size (caps every consumer)")
        lines.append("=" * 78)
        # Two tiers: a hard byte mismatch (both sides assert, differ) outranks a
        # base that simply lacks a STATIC_SIZE_ASSERT. Within a tier, biggest
        # |delta| first, then consumer count.
        hard = [d for d in size_defs if d.size_delta not in (None, 0)]
        soft = [d for d in size_defs if d.base_no_size_assert]
        hard.sort(
            key=lambda d: (abs(d.size_delta), _consumer_weight(d.fqn)),
            reverse=True,
        )
        # Soft tier (base has no STATIC_SIZE_ASSERT) is only actionable with a
        # corroborating signal - a differing field count between base and target
        # implies a real layout divergence. Plain "base doesn't assert this size"
        # is mostly noise (base rarely uses the macro), so it is summarised, not
        # listed.
        soft_corr = [
            d for d in soft
            if d.target_field_count != d.base_field_count and not d.base_fields_unreliable
        ]
        soft_unrel = [d for d in soft if d.base_fields_unreliable]
        soft_plain = [
            d for d in soft
            if d.target_field_count == d.base_field_count and not d.base_fields_unreliable
        ]
        # Rank by IMPACT: consumer count first (a layout bug caps every type that
        # walks it), then field-count delta (size of the divergence).
        soft_corr.sort(
            key=lambda d: (_consumer_weight(d.fqn),
                           abs(d.target_field_count - d.base_field_count)),
            reverse=True,
        )
        if not hard and not soft_corr:
            lines.append("  (none with a hard byte/field-count signal)")
        for d in hard:
            sign = "+" if d.size_delta > 0 else ""
            cons = _consumer_weight(d.fqn)
            lines.append(
                f"  {d.fqn}: base=0x{d.base_size:X} target=0x{d.target_size:X} "
                f"delta={sign}{d.size_delta} (0x{abs(d.size_delta):X})  [~{cons} consumers]"
            )
        if soft_corr:
            lines.append(
                "  --- base lacks STATIC_SIZE_ASSERT AND field count differs "
                "(likely real layout gap) ---"
            )
        for d in soft_corr:
            cons = _consumer_weight(d.fqn)
            lines.append(
                f"  {d.fqn}: target=0x{d.target_size:X} base=<no assert> "
                f"fields target={d.target_field_count} base={d.base_field_count}  "
                f"[~{cons} consumers]"
            )
        lines.append(
            f"  [{len(soft_plain)} more types: base lacks STATIC_SIZE_ASSERT but field "
            f"counts agree - add a STATIC_SIZE_ASSERT to lock them, low priority]"
        )
        lines.append(
            f"  [{len(soft_unrel)} types skipped: base layout is an anonymous "
            f"union/struct the parser can't field-count (math/buffer types)]"
        )
        lines.append("")

    if only in (None, "fields"):
        lines.append("=" * 78)
        lines.append("(2) MISSING FIELDS  -  target member absent from base (type, field @off)")
        lines.append("=" * 78)
        reliable = [d for d in field_defs if not d.base_fields_unreliable]
        unrel = [d for d in field_defs if d.base_fields_unreliable]
        reliable.sort(
            key=lambda d: (_consumer_weight(d.fqn), len(d.missing_fields)), reverse=True
        )
        if not reliable:
            lines.append("  (none)")
        for d in reliable:
            flds = ", ".join(
                f"{n}@0x{o:X}" if o is not None else f"{n}@?" for n, o in d.missing_fields
            )
            cons = _consumer_weight(d.fqn)
            lines.append(f"  {d.fqn}  [{len(d.missing_fields)} fields, ~{cons} consumers]: {flds}")
        lines.append(
            f"  [{len(unrel)} types skipped: base layout uses an anonymous "
            f"union/struct - field names not parseable, compare by hand]"
        )
        lines.append("")

    if only in (None, "methods"):
        lines.append("=" * 78)
        lines.append("(3) MISSING METHODS  -  target method absent from base (often TARGET_ONLY)")
        lines.append("=" * 78)
        # A ctor/dtor the base leaves implicit (compiler-generated) shows up as
        # "missing" but is rarely actionable, so split it out: types missing a
        # REAL named method first (the TARGET_ONLY overlap), ctor/dtor-only after.
        def _real_methods(d: Defects) -> list[tuple[str, int]]:
            cls = d.fqn.split("::")[-1]
            return [(n, a) for n, a in d.missing_methods if n != cls and n != "~" + cls]

        real = [d for d in method_defs if _real_methods(d)]
        special_only = [d for d in method_defs if not _real_methods(d)]
        real.sort(key=lambda d: (_consumer_weight(d.fqn), len(_real_methods(d))), reverse=True)
        if not real:
            lines.append("  (none with a real named method)")
        for d in real:
            rm = _real_methods(d)
            ms = ", ".join(f"{n}/{a}" for n, a in rm)
            cons = _consumer_weight(d.fqn)
            lines.append(f"  {d.fqn}  [{len(rm)} methods, ~{cons} consumers]: {ms}")
        lines.append(
            f"  [{len(special_only)} more types missing only an implicit ctor/dtor "
            f"- usually a compiler-generated special member, low priority]"
        )
        lines.append("")

    if only in (None, "types"):
        lines.append("=" * 78)
        lines.append("(4) MISSING TYPES  -  target vostok::/survarium:: type with NO base header")
        lines.append("=" * 78)
        # Confidence tiers (template instantiations are dropped from the headline:
        # base declares the generic template, so the instantiated FQN can never
        # name-match - reported only as a count):
        #   HIGH : non-template, has real members, bare name not seen in base.
        #   MED  : non-template, bare name not seen in base, but no members
        #          (likely a real but empty/forward type).
        #   LOW  : bare name exists in base under another namespace (the target
        #          generator commonly parks forward-ref stubs in the using ns).
        non_tmpl = [d for d in type_defs if not d.is_template]
        n_tmpl = sum(1 for d in type_defs if d.is_template)
        high = [d for d in non_tmpl if d.has_members and not d.name_elsewhere]
        med = [d for d in non_tmpl if not d.has_members and not d.name_elsewhere]
        low = [d for d in non_tmpl if d.name_elsewhere]
        if not non_tmpl:
            lines.append("  (none)")

        def _emit(group: list[Defects], label: str) -> None:
            if not group:
                return
            lines.append(f"  --- {label} ({len(group)}) ---")
            for d in sorted(group, key=lambda d: (d.module, d.fqn)):
                sz = f"0x{d.target_size:X}" if d.target_size is not None else "?"
                mem = "members" if d.has_members else "no members"
                lines.append(f"  {d.fqn}  (target size {sz}, {mem})")

        _emit(high, "HIGH confidence: real definition, name not in base")
        _emit(med, "MED confidence: name not in base, but no members")
        _emit(low, "LOW confidence: bare name exists in base under another namespace")
        lines.append(
            f"  [{n_tmpl} template instantiations omitted - base declares the "
            f"generic template, FQN cannot name-match]"
        )
        lines.append("")

    # Summary
    hard_sz = sum(1 for d in size_defs if d.size_delta not in (None, 0))
    soft_sz = sum(1 for d in size_defs if d.base_no_size_assert)
    real_missing = sum(1 for d in type_defs if not d.is_template and not d.name_elsewhere)
    lines.append("=" * 78)
    lines.append("SUMMARY")
    lines.append("=" * 78)
    lines.append(f"  size mismatches : {len(size_defs)}  ({hard_sz} hard byte diffs, {soft_sz} base-no-assert)")
    lines.append(f"  types w/ missing fields : {len(field_defs)}")
    lines.append(f"  types w/ missing methods: {len(method_defs)}")
    lines.append(f"  missing types : {real_missing} likely-real (+ noise: templates / wrong-ns stubs)")
    return "\n".join(lines)


# ----------------------------------------------------------------------------
# CLI
# ----------------------------------------------------------------------------


def main(argv: list[str] | None = None) -> int:
    # `vostok diff layout` calls main() with no args after rewriting sys.argv.
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--module", help="restrict to one module (first ns segment, or 'survarium')")
    ap.add_argument("--type", help="restrict to one fully-qualified type name")
    ap.add_argument("--only", choices=["size", "fields", "methods", "types"], help="one defect class")
    ap.add_argument("--json", help="write machine-readable JSON to this path")
    args = ap.parse_args(argv)

    target = collect_target()
    base = collect_base()
    defects = diff(target, base)

    if args.type:
        defects = [d for d in defects if d.fqn == args.type]
        if not defects:
            print(f"no defects for {args.type} (or type not found / matches base)", file=sys.stderr)
    elif args.module:
        defects = [d for d in defects if d.module == args.module]

    if args.json:
        payload = [
            {
                "fqn": d.fqn,
                "module": d.module,
                "base_size": d.base_size,
                "target_size": d.target_size,
                "size_delta": d.size_delta,
                "base_missing_type": d.base_missing_type,
                "base_no_size_assert": d.base_no_size_assert,
                "is_template": d.is_template,
                "name_elsewhere": d.name_elsewhere,
                "has_members": d.has_members,
                "missing_fields": [{"name": n, "offset": o} for n, o in d.missing_fields],
                "missing_methods": [{"name": n, "arity": a} for n, a in d.missing_methods],
            }
            for d in defects
        ]
        Path(args.json).write_text(json.dumps(payload, indent=2))

    print(render_report(defects, target, args.only))
    print(
        f"\n[parsed {len(target)} target types, {len(base)} base types "
        f"in vostok::/survarium:: (render excluded)]",
        file=sys.stderr,
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.diff.layout", main, sys.argv[1:]))
