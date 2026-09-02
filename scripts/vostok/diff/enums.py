#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.diff.enums - diff the TARGET enum set (the original game, ground truth) against
our BASE enum set, and maintain docs/binary_matching/enum_queue.md as a PERSISTENT
work queue of every enum we still owe.

Two sides, two very different shapes:

  TARGET  binaries/structure/target/headers - pdb-parser's normalized dump. Since
          the all-enums type-stream-walk bump the builder emits EVERY enum it sees
          (~1150 unique names), as either a standalone file under an `enums/` dir
          or a nested `enum cls::foo` inside the owning class header. Bodies are
          one-enumerator-per-line with explicit hex values:

              namespace vostok {
              namespace render {
              enum light_type
              {
                  light_type_point  = 0x0,
                  ...
              };
              } } // namespaces

          The qualified name comes from the open `namespace X {` lines plus the
          name on the `enum ...` line (which itself may carry `cls::` for a
          class-nested enum). This tree is gitignored/regenerable; regenerate it
          with `python3 -m vostok.build.generate_structure target` before trusting a
          diff (this script can do it for you with --regen-target).

  BASE    sources/vostok/**/*.h - hand-written C++. Enums are lexically nested in
          `namespace`/`class`/`struct` blocks, may be single-line, may omit
          values (auto-increment) or use expressions (`1 << 0`, `u32(-1)`). The
          fully-qualified name is built from the lexical scope stack, so we run a
          brace-tracking scanner over each file.

Defect classes reported (scoped to engine namespaces survarium:: / vostok::, with
vostok::render:: split into a DEFERRED bucket - render is matched last):

  MISSING        - enum defined in TARGET, absent from BASE.
  FIELD-MISMATCH - same qualified enum in both, but the enumerator set or values
                   differ (missing / extra / renamed members, or value diffs).

EXPORT-COMPLETENESS (--check-export): the structure builder (pdb-parser) is the
only tool that reads this BigMSF PDB (llvm-pdbutil chokes on it), so it is also
the type-stream oracle. We regenerate a throwaway target dump and confirm the
in-repo tree carries every enum the builder extracts; any builder-level NAME drop
(pdb-parser's "enum X seen with differing enumerator counts; keeping first"
collision dedup) is surfaced separately as a builder gap, distinct from base gaps.

QUEUE SEMANTICS (also documented at the top of enum_queue.md):
  * One row per UNHANDLED enum (MISSING or FIELD-MISMATCH).
  * An enum is REMOVED from the queue once handled (base matches target).
  * BLOCKED rows STAY (persistent) with their cause - we iterate until empty.
  * --write-queue reconciles the file: it re-derives the live defect set, drops
    rows now handled, and PRESERVES the human-authored Status (BLOCKED) + cause +
    "intended base location" columns of rows that are still defective.

Usage:
  python3 -m vostok diff enums                      # print the diff (in-scope)
  python3 -m vostok diff enums --include-render      # include the deferred render bucket
  python3 -m vostok diff enums --check-export        # + export-completeness check
  python3 -m vostok diff enums --write-queue         # reconcile/refresh enum_queue.md
  python3 -m vostok diff enums --regen-target        # regen target dump first, then diff
  python3 -m vostok diff enums --target DIR --base-src DIR
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

from vostok.core import paths
from vostok.core.paths import ENGINE as BASE_SOURCES
from vostok.core.paths import ENUM_QUEUE as QUEUE_FILE
from vostok.core.paths import TARGET_HEADERS as STRUCTURE_TARGET

# Engine namespaces we own and match (third-party lands under other namespaces,
# emitted into the `others/` subtree - out of scope).
ENGINE_PREFIXES = ("survarium::", "vostok::")
# Render is matched last; split it into a deferred bucket rather than the queue.
RENDER_PREFIX = "vostok::render::"

# ---------------------------------------------------------------------------
# TARGET parsing (pdb-parser normalized dump)
# ---------------------------------------------------------------------------

# `namespace X {` opener(s). A file may chain several on one line:
#   namespace compression { namespace ppmd {
_NS_OPEN_RE = re.compile(r"namespace\s+([A-Za-z_]\w*)\s*\{")
# `enum NAME` opening a definition body (NAME may be class-qualified: cls::foo).
# A trailing `;` (forward decl) or anything after the name excludes it.
_ENUM_OPEN_RE = re.compile(r"^enum\s+([A-Za-z_][\w:]*)\s*$")
_ENUMERATOR_RE = re.compile(
    r"^\s*([A-Za-z_]\w*)\s*=\s*(-?0[xX][0-9a-fA-F]+|-?\d+)\s*,?\s*$"
)


def _join_qual(ns_stack: list[str], name: str) -> str:
    """Build the qualified enum name, collapsing a redundant leading namespace
    duplicate. The pdb-parser dump sometimes re-qualifies the enum on its own
    line, e.g. `enum survarium::action_type` INSIDE `namespace survarium {`,
    which would otherwise yield `survarium::survarium::action_type`. If the name
    already starts with (a suffix of) the namespace stack, drop the overlap so
    both forms collapse to the canonical FQN."""
    # The dump sometimes emits an already-fully-qualified enum name inside an
    # unrelated namespace wrapper (e.g. `enum vostok::input::enum_keyboard` under
    # `namespace vostok { namespace ui {`). When the name itself is rooted at a
    # known top-level namespace, it IS the FQN - the wrapper is just the header it
    # landed in.
    if name.startswith(("vostok::", "survarium::")):
        return name
    if not ns_stack:
        return name
    name_parts = name.split("::")
    # find the largest overlap: ns_stack tail == name head (collapses the
    # `enum survarium::foo` re-qualification inside `namespace survarium {`).
    for k in range(min(len(ns_stack), len(name_parts)), 0, -1):
        if ns_stack[-k:] == name_parts[:k]:
            return "::".join(ns_stack + name_parts[k:])
    return "::".join(ns_stack + name_parts)


def parse_target_enums(headers_dir: Path) -> dict[str, list[tuple[str, int]]]:
    """{qualified_name: [(enumerator, value)]} for every enum body in the dump.

    The dump duplicates a given enum across many headers (full body where the
    type is defined, empty `{ }` name-only refs elsewhere); keep the
    MOST-POPULATED body per qualified name.
    """
    enums: dict[str, list[tuple[str, int]]] = {}

    for path in sorted(headers_dir.rglob("*.h")):
        try:
            lines = path.read_text(errors="replace").splitlines()
        except OSError:
            continue

        ns_stack: list[str] = []
        brace_depth = 0  # net braces seen on namespace-only lines
        i, n = 0, len(lines)
        while i < n:
            line = lines[i]
            m = _ENUM_OPEN_RE.match(line)
            if m and i + 1 < n and lines[i + 1].strip() == "{":
                name = m.group(1)
                qual = _join_qual(ns_stack, name)
                members: list[tuple[str, int]] = []
                j = i + 2
                while j < n and lines[j].strip() not in ("};", "}"):
                    em = _ENUMERATOR_RE.match(lines[j])
                    if em:
                        members.append((em.group(1), _parse_int(em.group(2))))
                    j += 1
                if qual not in enums or len(members) > len(enums[qual]):
                    enums[qual] = members
                i = j + 1
                continue
            # Track namespace nesting (only `namespace X { ... }` blocks matter for
            # the qualified name; class/struct bodies in the dump carry their own
            # `cls::` prefix on the enum line, so we don't track them here).
            opens = _NS_OPEN_RE.findall(line)
            for ns in opens:
                ns_stack.append(ns)
                brace_depth += 1
            # Closing braces that pop a namespace we pushed.
            if not opens:
                closes = line.count("}")
                while closes > 0 and brace_depth > 0 and ns_stack:
                    ns_stack.pop()
                    brace_depth -= 1
                    closes -= 1
            i += 1

    return enums


# ---------------------------------------------------------------------------
# BASE parsing (real C++ sources, lexical scope tracking)
# ---------------------------------------------------------------------------

_TOKEN_RE = re.compile(
    r"""
      //[^\n]*                      # line comment
    | /\*.*?\*/                     # block comment
    | \"(?:\\.|[^"\\])*\"           # string literal
    | '(?:\\.|[^'\\])*'             # char literal
    | \#[^\n]*                      # preprocessor line (kept crude on purpose)
    | \benum\b
    | \bnamespace\b
    | \b(?:class|struct)\b
    | 0[xX][0-9a-fA-F]+|\d+         # integer literal (keep digits together)
    | [A-Za-z_]\w*
    | ::                            # scope operator (class-nested enum names)
    | <<|>>                         # shift ops (enum flag bodies use `1 << n`)
    | [{}();<>=,:]
    | .
    """,
    re.VERBOSE | re.DOTALL,
)


def _evaluate(expr: str, prev: int | None, syms: dict[str, int]) -> int | None:
    """Best-effort enumerator value. Handles plain ints/hex, `1 << 0`, simple
    arithmetic, `u32(-1)`/`(int)(-1)` style casts, and references to prior
    enumerators in the same enum (`t_embedded_types = t_table_indexed`) via
    `syms`. Returns None if it cannot reduce to an int (caller leaves it
    unknown)."""
    expr = expr.strip().rstrip(",")
    if not expr:
        return prev + 1 if prev is not None else 0
    # unwrap numeric casts ANYWHERE in the expression: `u32(1) << 5`, `u32(-1)`,
    # `(int)0x4`. Repeatedly replace `ident(<numeric>)` with `(<numeric>)` until
    # no cast remains. Only single, comma-free numeric/identifier args qualify, so
    # real function calls are left intact (and the expr then fails the numeric
    # gate below).
    cast_re = re.compile(
        r"\b[A-Za-z_]\w*\s*\(\s*(-?(?:0[xX][0-9a-fA-F]+|\d+))\s*\)"
    )
    for _ in range(8):
        new = cast_re.sub(r"(\1)", expr)
        if new == expr:
            break
        expr = new
    # substitute references to prior enumerators in the same body
    def _sub(m: re.Match) -> str:
        return str(syms[m.group(0)]) if m.group(0) in syms else m.group(0)

    expr = re.sub(r"[A-Za-z_]\w*", _sub, expr)
    # only evaluate when the expression is now purely numeric/operators.
    if re.fullmatch(r"[0-9a-fA-FxX()\s+\-*/<>|&~]+", expr):
        try:
            # python `<<`/`|`/etc. match C semantics for the small constants used
            return int(eval(expr, {"__builtins__": {}}, {}))  # noqa: S307
        except (ValueError, SyntaxError, ZeroDivisionError, TypeError):
            return None
    return None


def _parse_enum_body(body: str) -> list[tuple[str, int | None]]:
    """Parse `A = 1, B, C = 0x4` -> [(A,1),(B,2),(C,4)] with auto-increment."""
    members: list[tuple[str, int | None]] = []
    prev: int | None = None
    # split on commas at brace/paren depth 0
    depth = 0
    cur = ""
    parts: list[str] = []
    for ch in body:
        if ch in "({[":
            depth += 1
        elif ch in ")}]":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += ch
    if cur.strip():
        parts.append(cur)
    syms: dict[str, int] = {}  # prior enumerators in THIS body, for references
    for part in parts:
        part = part.strip()
        if not part:
            continue
        if "=" in part:
            nm, _, val = part.partition("=")
            nm = nm.strip()
            v = _evaluate(val, prev, syms)
        else:
            nm = part.strip()
            v = prev + 1 if prev is not None else 0
        if not re.fullmatch(r"[A-Za-z_]\w*", nm):
            continue
        members.append((nm, v))
        prev = v
        if v is not None:
            syms[nm] = v
    return members


def parse_base_enums(sources_dir: Path) -> dict[str, list[tuple[str, int | None]]]:
    """{qualified_name: [(enumerator, value|None)]} for every enum DEFINITION in
    the base sources, qualified by its lexical namespace/class/struct scope."""
    enums: dict[str, list[tuple[str, int | None]]] = {}

    for path in sorted(sources_dir.rglob("*.h")):
        try:
            text = path.read_text(errors="replace")
        except OSError:
            continue
        _scan_file(text, enums)
    return enums


def _scan_file(text: str, enums: dict[str, list[tuple[str, int | None]]]) -> None:
    # scope stack of (name_or_None, kind) where kind in {ns, cls, blk, enum}.
    # Anonymous/local scopes push (None, blk) so braces stay balanced.
    scope: list[tuple[str | None, str]] = []
    toks = [
        m.group(0)
        for m in _TOKEN_RE.finditer(text)
        if not m.group(0).startswith(("//", "/*", "#"))
    ]
    i, n = 0, len(toks)
    pending: str | None = None       # 'ns' / 'cls' awaiting a name then '{'
    pending_name: str | None = None  # captured name (first ident after the kw)
    name_locked = False              # stop capturing once name is fixed (`:` base list)
    while i < n:
        t = toks[i]
        if t == "namespace":
            pending, pending_name, name_locked = "ns", None, False
        elif t in ("class", "struct"):
            pending, pending_name, name_locked = "cls", None, False
        elif t == "enum":
            # Read the (optional) name up to `{` or `;`.
            name, j = _read_enum_name(toks, i + 1)
            if j < n and toks[j] == "{":
                body, k = _read_braced(toks, j)
                quals = [s for s, _ in scope if s is not None]
                if name:
                    qual = "::".join(quals + [name])
                    enums.setdefault(qual, _parse_enum_body(body))
                i = k
                continue
            # forward decl or unnamed-without-body: skip to ; or {
            i = j
            continue
        elif re.fullmatch(r"[A-Za-z_]\w*", t):
            # First identifier after class/struct/namespace IS the name; later
            # identifiers (a base-class list, attributes) must not overwrite it.
            if pending in ("ns", "cls") and not name_locked:
                pending_name = t
                if pending == "cls":
                    name_locked = True  # `class foo : public bar {` -> name is foo
        elif t == ":":
            # base-clause start (or access specifier) - freeze the class name.
            name_locked = True
        elif t == "{":
            if pending == "ns":
                scope.append((pending_name, "ns"))
            elif pending == "cls":
                scope.append((pending_name, "cls"))
            else:
                scope.append((None, "blk"))
            pending = pending_name = None
            name_locked = False
        elif t == "}":
            if scope:
                scope.pop()
            pending = pending_name = None
            name_locked = False
        elif t == ";":
            # ends a pending class/namespace intro that had no body
            # (a forward decl `class foo;` or a variable `class foo bar;`).
            pending = pending_name = None
            name_locked = False
        i += 1


def _read_enum_name(toks: list[str], i: int) -> tuple[str | None, int]:
    """From just after `enum`, read an optional (possibly `cls::foo`) name; stop
    at `{` or `;`. Returns (name|None, index_of_stop)."""
    parts: list[str] = []
    n = len(toks)
    while i < n and toks[i] not in ("{", ";"):
        tk = toks[i]
        # `enum class foo` / `enum struct foo` - skip the class/struct keyword
        if tk in ("class", "struct"):
            i += 1
            continue
        # `:` base-type spec (enum foo : int) - stop name accumulation at it
        if tk == ":":
            # consume to the brace/semicolon
            while i < n and toks[i] not in ("{", ";"):
                i += 1
            break
        if re.fullmatch(r"[A-Za-z_]\w*|::", tk):
            parts.append(tk)
        i += 1
    name = "".join(parts) if parts else None
    return name, i


def _read_braced(toks: list[str], i: int) -> tuple[str, int]:
    """toks[i] == '{' - return (inner_text, index_after_matching_'}')."""
    assert toks[i] == "{"
    depth = 0
    out: list[str] = []
    n = len(toks)
    j = i
    while j < n:
        tk = toks[j]
        if tk == "{":
            depth += 1
            if depth > 1:
                out.append(tk)
        elif tk == "}":
            depth -= 1
            if depth == 0:
                return " ".join(out), j + 1
            out.append(tk)
        else:
            out.append(tk)
        j += 1
    return " ".join(out), j


# ---------------------------------------------------------------------------
# Diff
# ---------------------------------------------------------------------------

def _parse_int(text: str) -> int:
    text = text.strip()
    neg = text.startswith("-")
    if neg:
        text = text[1:]
    base = 16 if text.lower().startswith("0x") else 10
    v = int(text, base)
    return -v if neg else v


def _fmt(v: int | None) -> str:
    if v is None:
        return "?"
    return f"-0x{-v:x}" if v < 0 else f"0x{v:x}"


def _values_equal(base_value: int | None, target_value: int) -> bool:
    """Compare MSVC enum values by their 32-bit representation.

    The source scanner evaluates ``1 << 31`` as positive Python 0x80000000,
    while the PDB renders the same MSVC enum constant as -0x80000000.
    """
    return (
        base_value is not None
        and (base_value & 0xFFFFFFFF) == (target_value & 0xFFFFFFFF)
    )


def diff_enums(base, target, include_render):
    """Return (missing, mismatched, render_missing, render_mismatched).

    *_mismatched entries are (qualified_name, [diff_line, ...])."""
    missing, mismatched = [], []
    render_missing, render_mismatched = [], []

    for name in sorted(target):
        if not name.startswith(ENGINE_PREFIXES):
            continue
        is_render = name.startswith(RENDER_PREFIX)
        if name not in base:
            (render_missing if is_render else missing).append(name)
            continue
        b = base[name]
        t = target[name]
        b_map = dict(b)
        t_map = dict(t)
        if [m for m, _ in t] == [m for m, _ in b] and all(
            _values_equal(b_map[m], v) for m, v in t
        ):
            continue
        diff_lines = _member_diff(name, b, t, b_map, t_map)
        if diff_lines:
            (render_mismatched if is_render else mismatched).append((name, diff_lines))

    return missing, mismatched, render_missing, render_mismatched


def _member_diff(name, b, t, b_map, t_map):
    order = list(dict.fromkeys([m for m, _ in t] + [m for m, _ in b]))
    lines = []
    for em in order:
        in_b, in_t = em in b_map, em in t_map
        if in_b and in_t:
            if not _values_equal(b_map[em], t_map[em]):
                lines.append(f"{em}: base={_fmt(b_map[em])} target={_fmt(t_map[em])}")
        elif in_t:
            lines.append(f"{em}: base=<absent> target={_fmt(t_map[em])}")
        else:
            lines.append(f"{em}: base={_fmt(b_map[em])} target=<absent>")
    return lines


def _intended_base_location(name: str) -> str:
    """Guess the base header a missing enum should live in, from the target dump's
    file naming convention (an `enums/<snake>.h` under the namespace dir)."""
    parts = name.split("::")
    leaf = parts[-1]
    if parts[0] == "survarium":
        return f"sources/vostok/game_core/{leaf}.h (or owning class header)"
    if parts[0] == "vostok" and len(parts) >= 3:
        module = parts[1]
        return f"sources/vostok/{module}/... ({leaf} - enum file or owning class)"
    return "sources/vostok/... (locate owning class/header)"


# ---------------------------------------------------------------------------
# Export-completeness (pdb-parser is the type-stream oracle)
# ---------------------------------------------------------------------------

def check_export(target_dir: Path) -> tuple[list[str], list[str]]:
    """Re-extract a throwaway target dump and compare its qualified-enum set to
    the in-repo tree's. Returns (oracle_only, collision_warnings):

      oracle_only - enums the builder extracts that are MISSING from the in-repo
                    tree (stale/regenerable tree gap; empty if freshly regen'd).
      collisions  - pdb-parser "differing enumerator counts; keeping first"
                    warnings (the only builder-level NAME drop).
    """
    pdb_parser = os.environ.get("PDB_PARSER", "pdb_parser")
    survarium_bin = Path(os.environ.get("SURVARIUM_BIN", ""))
    pdb = survarium_bin / "survarium.pdb"
    if not pdb.is_file():
        print(
            "  (skipping export check: SURVARIUM_BIN/survarium.pdb not found - "
            "run inside `nix develop`)",
            file=sys.stderr,
        )
        return [], []

    with tempfile.TemporaryDirectory(prefix="enum_export_") as tmp:
        proc = subprocess.run(
            [
                pdb_parser,
                "--output-path", tmp,
                "--pdb-path", str(pdb),
                "--engine-path", "c:/survarium/sources",
            ],
            stderr=subprocess.PIPE,
            text=True,
            check=False,
        )
        collisions = [
            ln.strip()
            for ln in (proc.stderr or "").splitlines()
            if "differing enumerator counts" in ln
        ]
        oracle = set(parse_target_enums(Path(tmp)))

    inrepo = set(parse_target_enums(target_dir))
    oracle_only = sorted(oracle - inrepo)
    return oracle_only, collisions


# ---------------------------------------------------------------------------
# Persistent queue file
# ---------------------------------------------------------------------------

_QUEUE_HEADER = """# Enum work queue (target vs base)

Generated/reconciled by `python3 -m vostok diff enums --write-queue`.

This is a PERSISTENT queue of every engine enum (`survarium::` / `vostok::`,
**excluding** `vostok::render::` - render is matched last; see the deferred
bucket at the bottom) whose definition in our base sources does NOT yet match the
original game (the TARGET pdb-parser dump under
`binaries/structure/target/headers`).

## Semantics

* One row per UNHANDLED enum.
* `MISSING` - defined in target, absent from base sources.
* `FIELD-MISMATCH` - present in both, but the enumerator set or values differ.
* An enum is **removed** from this queue once handled (base matches target):
  re-run `--write-queue` and the row drops out.
* `BLOCKED` rows **stay** (persistent) with their cause - we iterate until the
  file is empty. When you add a real block (a missing type/header, a cross-module
  dependency), set Status to `BLOCKED:<cause>` and `--write-queue` preserves it.
* The reconciler preserves the human-authored `Status` (BLOCKED) and the
  `Intended base location` of any row that is still defective; it only drops rows
  that are now handled and appends newly-discovered defects.

Re-derive the live set anytime:

    python3 -m vostok diff enums                 # human-readable diff
    python3 -m vostok diff enums --write-queue   # reconcile this file

"""

# A queue row is `| name | target loc | intended base loc | status | detail | blocked |`.
_ROW_RE = re.compile(r"^\|\s*`?([\w:]+)`?\s*\|")


def _read_existing_queue(path: Path) -> dict[str, dict[str, str]]:
    """Parse existing rows -> {name: {status, intended, blocked}} so a reconcile
    can preserve human-authored BLOCKED status / cause / intended location."""
    rows: dict[str, dict[str, str]] = {}
    if not path.is_file():
        return rows
    for line in path.read_text().splitlines():
        m = _ROW_RE.match(line)
        if not m:
            continue
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if len(cells) < 6:
            continue
        name = cells[0].strip("`")
        rows[name] = {
            "intended": cells[2],
            "status": cells[3],
            "blocked": cells[5],
        }
    return rows


def _target_location(name: str) -> str:
    return name.rsplit("::", 1)[-1] + ".h (enums/ or owning class header)"


def _detail_for(name, base, target):
    if name not in base:
        members = target.get(name, [])
        return f"{len(members)} enumerators to add"
    b, t = base[name], target[name]
    b_map, t_map = dict(b), dict(t)
    lines = _member_diff(name, b, t, b_map, t_map)
    return "; ".join(lines) if lines else "members differ"


def write_queue(path: Path, base, target) -> int:
    """Reconcile the queue file in place. Returns the number of live rows."""
    existing = _read_existing_queue(path)
    missing, mismatched, _, _ = diff_enums(base, target, include_render=False)
    mismatched_names = [n for n, _ in mismatched]

    live: list[tuple[str, str]] = (
        [(n, "MISSING") for n in missing]
        + [(n, "FIELD-MISMATCH") for n in mismatched_names]
    )
    live.sort()

    rows = []
    for name, klass in live:
        prev = existing.get(name, {})
        # Preserve a human-set BLOCKED status (and cause); otherwise it's the
        # plain defect class.
        prev_status = prev.get("status", "")
        if prev_status.upper().startswith("BLOCKED"):
            status = prev_status
        else:
            status = klass
        intended = prev.get("intended") or _intended_base_location(name)
        blocked = prev.get("blocked") or ("" if not status.upper().startswith("BLOCKED") else status)
        detail = _detail_for(name, base, target).replace("|", r"\|")
        rows.append(
            f"| `{name}` | {_target_location(name)} | {intended} "
            f"| {status} | {detail} | {blocked} |"
        )

    # deferred render bucket (informational, not part of the live queue): when
    # render is EXCLUDED, diff_enums collects render defects into its last two
    # return values.
    _, _, r_missing, r_mismatched = diff_enums(base, target, include_render=False)
    render_rows = sorted(set(r_missing) | set(n for n, _ in r_mismatched))

    out = [_QUEUE_HEADER]
    out.append(
        f"_Live rows: {len(rows)} "
        f"({len(missing)} MISSING + {len(mismatched_names)} FIELD-MISMATCH). "
        f"Deferred render enums: {len(render_rows)}._\n"
    )
    out.append(
        "| Enum (fully-qualified) | Target location | Intended base location "
        "| Status | Diff detail | BLOCKED? + cause |"
    )
    out.append("|---|---|---|---|---|---|")
    out.extend(rows if rows else ["| _(queue empty - all engine enums match)_ ||||||"])

    out.append("\n## Deferred: render-module enums (`vostok::render::`)\n")
    out.append(
        "Render is matched last; these are tracked but not part of the live "
        "queue above. Re-run with `--include-render` to diff them.\n"
    )
    if render_rows:
        out.append("| Enum | Status |")
        out.append("|---|---|")
        for name in render_rows:
            klass = "MISSING" if name not in base else "FIELD-MISMATCH"
            out.append(f"| `{name}` | {klass} |")
    else:
        out.append("_(no render-enum defects)_")

    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(out) + "\n")
    return len(rows)


# ---------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("--target", type=Path, default=STRUCTURE_TARGET)
    ap.add_argument("--base-src", type=Path, default=BASE_SOURCES)
    ap.add_argument("--include-render", action="store_true",
                    help="include the deferred vostok::render:: bucket in the diff")
    ap.add_argument("--check-export", action="store_true",
                    help="cross-check the in-repo target tree against a fresh "
                         "pdb-parser extraction (the type-stream oracle)")
    ap.add_argument("--write-queue", action="store_true",
                    help="reconcile docs/binary_matching/enum_queue.md")
    ap.add_argument("--regen-target", action="store_true",
                    help="regenerate the target structure dump first "
                         "(python3 -m vostok.build.generate_structure target)")
    ap.add_argument("--queue-file", type=Path, default=QUEUE_FILE)
    args = ap.parse_args()

    if args.regen_target:
        subprocess.run(
            [sys.executable, "-m", "vostok.build.generate_structure", "target"],
            check=True, env=paths.child_env(),
        )

    if not args.target.is_dir():
        print(f"ERROR: target headers dir not found: {args.target}\n"
              "  run: python3 -m vostok.build.generate_structure target", file=sys.stderr)
        return 1
    if not args.base_src.is_dir():
        print(f"ERROR: base sources dir not found: {args.base_src}", file=sys.stderr)
        return 1

    target = parse_target_enums(args.target)
    base = parse_base_enums(args.base_src)

    target_engine = {n for n in target if n.startswith(ENGINE_PREFIXES)}
    target_render = {n for n in target_engine if n.startswith(RENDER_PREFIX)}
    base_engine = {n for n in base if n.startswith(ENGINE_PREFIXES)}

    missing, mismatched, r_missing, r_mismatched = diff_enums(
        base, target, args.include_render
    )

    print("enum diff - target (original game) vs base (our sources)")
    print(f"  target enums total:   {len(target)}")
    print(f"  target engine enums:  {len(target_engine)} "
          f"(of which render: {len(target_render)})")
    print(f"  base enums total:     {len(base)} (engine: {len(base_engine)})")
    print()
    print(f"  MISSING (in-scope):        {len(missing)}")
    print(f"  FIELD-MISMATCH (in-scope): {len(mismatched)}")
    print(f"  deferred render MISSING:   "
          f"{len([n for n in (r_missing if args.include_render else []) if n.startswith(RENDER_PREFIX)])}"
          if args.include_render else
          f"  deferred render (excluded): {len(target_render)}")
    print()

    print(f"=== FIELD-MISMATCH ({len(mismatched)}) ===")
    for name, lines in mismatched:
        print(f"enum {name}")
        for ln in lines:
            print(f"    {ln}")
    if not mismatched:
        print("(none)")
    print()

    print(f"=== MISSING ({len(missing)}) ===")
    for name in missing:
        print(f"enum {name}  ({len(target[name])} enumerators)")
    if not missing:
        print("(none)")

    if args.include_render:
        print(f"\n=== RENDER (deferred) MISSING ({len([n for n in r_missing if n.startswith(RENDER_PREFIX)])}) ===")
        for name in r_missing:
            if name.startswith(RENDER_PREFIX):
                print(f"enum {name}")
        print(f"=== RENDER (deferred) FIELD-MISMATCH ({len([1 for n, _ in r_mismatched if n.startswith(RENDER_PREFIX)])}) ===")
        for name, _ in r_mismatched:
            if name.startswith(RENDER_PREFIX):
                print(f"enum {name}")

    if args.check_export:
        print("\n=== EXPORT-COMPLETENESS (pdb-parser type-stream oracle) ===")
        oracle_only, collisions = check_export(args.target)
        print(f"  enums extracted by builder but MISSING from in-repo tree: "
              f"{len(oracle_only)}")
        for name in oracle_only[:50]:
            print(f"    {name}")
        if len(oracle_only) > 50:
            print(f"    ... and {len(oracle_only) - 50} more")
        print(f"  builder NAME-collision drops (keeping first): {len(collisions)}")
        for c in collisions:
            print(f"    {c}")

    if args.write_queue:
        n = write_queue(args.queue_file, base, target)
        print(f"\nwrote {args.queue_file} ({n} live rows)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
