# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.tool.cleanliness - the source-cleanliness board and its down-only ratchet.

Counts source smells in the engine sources (`sources/vostok/**`, vendored
subtrees excluded) per module, so a later cleanup phase can drive each count
down and a gate can keep it from rising again. A count is a list of
candidates, not a verdict: `--list KIND` names every site so a reviewer can
judge it. Not wired into `vostok build`; `--check` is the opt-in gate.

    python3 -m vostok tool cleanliness                  # the board: module x kind
    python3 -m vostok tool cleanliness --module render  # one module's row
    python3 -m vostok tool cleanliness --list joined    # file:line per site (or `all`)
    python3 -m vostok tool cleanliness --top 5          # worst files per kind
    python3 -m vostok tool cleanliness --json           # machine-readable board
    python3 -m vostok tool cleanliness --markdown       # the board as a Markdown table
    python3 -m vostok tool cleanliness --check          # exit 1 when a count rose
    python3 -m vostok tool cleanliness --update         # bless: rewrite the floors

The kinds (comments and string/char literals are blanked first, offsets and
newlines kept, so prose and literals never count as code; the three marker
kinds read the comments instead):

  joined      two statements on one line (`a; b;`) inside a function body.
              Not counted: `for (;;)` headers (their `;` sit inside parens),
              macro definitions, declarations at namespace/class scope, a
              one-line function body in a header (`{ a = x; b = y; }` setters
              exist in retail headers), `case X: stmt; break;`, empty `;;`.
  ctl-line    a controlled statement on its condition's line:
              `if ( x ) return;`, `} else y( );`, `for ( ... ) z( );`
              (a one-line header body is admitted, as for `joined`).
  pragma      codegen-steering directives: `#pragma optimize / inline_depth /
              inline_recursion / auto_inline / check_stack / runtime_checks /
              function / float_control / fp_contract`.
  line-pin    `#line N` directives pinning statement records to retail lines.
  inline-pin  `__declspec( noinline )` and `__forceinline`.
  dead-cond   constant conditions (`if ( 0 )`, `if ( identity( false ) )`),
              empty `if` bodies, a repeated operand (`a && a`, `a == a`), and a
              condition repeated in one if / else-if chain.
  opacity     `identity( x )` around a runtime value: a member or a non-const
              local, where the wrapper silences no C4127 and only hides the
              value from the optimizer. Template parameters and const locals
              are the original idiom and are not counted.
  padding     a run of three or more blank lines inside a function body.
  volatile    the `volatile` qualifier.
  c-cast      C-style casts: `(u32)x`, `(T*)p`, `(T)this`, `(name)value`.
              `(void)x` (the discard idiom) and functional casts are not counted.
  unknown     `UNKNOWN_EXPRESSION` / `UNKNOWN_EXPRESSION_T` placeholders.
  state       `STATE[...]` carcass markers (STUB, INLINED, UNMATCHABLE, ...).
  todo        `sushi@TODO` review questions.
  prose       stale matching narration: a comment block carrying an agent tag
              (`claude@NOTE`, `codex@...`) or matching jargon (LTCG, ICF,
              objdiff, residual, structure-diff, `base inlines`, scores like
              98.57%, mangled-name dumps). One per block; a block anchoring a
              `sushi@TODO` or `STATE[...]` marker is counted there instead.

Floors live in config/cleanliness/baseline.tsv, one row per module and one
column per kind. `--check` fails (exit 1) when any cell rises above its floor,
and fails closed when a kind has no floor column at all; a module missing from
the file has floor 0. Only `--update` writes the file. Every run first
self-tests each kind against embedded positive and negative samples, so the
gate proves it can still fail before it judges the tree.

Lineage: the metric table, embedded self-test samples, cast detectors and the
`--update` bless follow homm3's cleanliness board; the offset-preserving comment
blanker and the fail-closed, never-writing gate follow gruntz's verify board.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
from collections import Counter, defaultdict
from concurrent.futures import ProcessPoolExecutor
from dataclasses import dataclass, field
from pathlib import Path

from vostok.core import log as _log
from vostok.core.paths import CLEANLINESS_BASELINE, ENGINE, REPO

EXTS = {".c", ".cpp", ".cxx", ".cc", ".h", ".hpp", ".inl"}
HEADERS = {".h", ".hpp", ".inl"}

# Third-party code kept inside the engine tree; not ours to clean.
VENDORED = (
    "core/sources/ptmalloc3/",          # Wolfram Gloger's ptmalloc3
    "core/sources/compressor_ppmd",     # Dmitry Shkarin's PPMd
    "core/sources/delayhlp.cpp",        # Microsoft's delay-load helper
    "login_server/sources/delayhlp.cpp",
    "maya_animation/sources/sisl/",     # SINTEF spline library
    "maya/sources/NvMender2003/",       # NVIDIA mesh mender
)

ROOT_MODULE = "(root)"   # headers directly under sources/vostok/


# --- blanking --------------------------------------------------------------------


@dataclass
class Source:
    """One file's views, all offset- and line-preserving. `code` has comments
    and literal contents blanked to spaces (delimiters and newlines kept);
    `live` additionally blanks `#if 0` regions; `stmt` also blanks every
    preprocessor directive; `nocomment` blanks comments only."""
    path: Path
    raw: str
    code: str = ""
    stmt: str = ""
    live: str = ""
    nocomment: str = ""
    comments: list = field(default_factory=list)   # (start, end) spans
    line_starts: list = field(default_factory=list)

    def __post_init__(self):
        self.code, self.comments = blank(self.raw)
        self.nocomment = _blank_spans(self.raw, self.comments)
        self.stmt, self.live = blank_preprocessor(self.code, self.raw)
        self.line_starts = [0] + [m.end() for m in re.finditer("\n", self.raw)]

    def line_of(self, offset: int) -> int:
        lo, hi = 0, len(self.line_starts) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if self.line_starts[mid] <= offset:
                lo = mid
            else:
                hi = mid - 1
        return lo + 1

    def line_text(self, line: int) -> str:
        start = self.line_starts[line - 1]
        end = self.raw.find("\n", start)
        return self.raw[start:end if end >= 0 else len(self.raw)]

    @property
    def is_header(self) -> bool:
        return self.path.suffix in HEADERS


def blank(text: str) -> tuple[str, list]:
    """Blank `//` and `/* */` comments and the contents of string and char
    literals to spaces, keeping newlines and literal delimiters (the
    gruntz srcscan state machine: a `//` inside a string is not a comment).
    Returns the blanked text and the comment spans."""
    out = list(text)
    spans = []
    n = len(text)
    i = 0
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            start = i
            while i < n and text[i] != "\n":
                out[i] = " "
                i += 1
            spans.append((start, i))
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            start = i
            end = text.find("*/", i + 2)
            end = n if end < 0 else end + 2
            for k in range(i, end):
                if text[k] != "\n":
                    out[k] = " "
            spans.append((start, end))
            i = end
            continue
        if c in "\"'":
            quote = c
            i += 1
            while i < n and text[i] != quote and text[i] != "\n":
                if text[i] == "\\" and i + 1 < n:
                    if text[i + 1] != "\n":
                        out[i + 1] = " "
                    out[i] = " "
                    i += 2
                    continue
                out[i] = " "
                i += 1
            i += 1
            continue
        i += 1
    return "".join(out), spans


def _blank_spans(text: str, spans) -> str:
    out = list(text)
    for start, end in spans:
        for k in range(start, end):
            if out[k] != "\n":
                out[k] = " "
    return "".join(out)


_PP_LINE = re.compile(r"^[ \t]*#")


def blank_preprocessor(code: str, raw: str) -> tuple[str, str]:
    """Blank every preprocessor directive, continuation lines included, so a
    macro body never reads as a statement. Continuations are read from the raw
    text: a backslash ending a `// comment` still continues the directive."""
    lines = code.split("\n")
    live = list(lines)
    raw_lines = raw.split("\n")
    cont = False
    dead = 0      # nesting inside an `#if 0` region: never compiled, never scanned
    for k, line in enumerate(lines):
        directive = not cont and _PP_LINE.match(line)
        was_dead = dead
        if directive:
            word = re.match(r"[ \t]*#[ \t]*(\w*)", line).group(1)
            if dead:
                if word in ("if", "ifdef", "ifndef"):
                    dead += 1
                elif word == "endif" or (dead == 1 and word in ("else", "elif")):
                    dead -= 1
            elif word == "if" and _IF_ZERO.match(line):
                dead = 1
        if dead or (directive and word == "endif" and was_dead):
            live[k] = " " * len(line)
        if cont or directive or dead:
            cont = (cont or bool(directive)) and raw_lines[k].rstrip().endswith("\\")
            lines[k] = " " * len(line)
    return "\n".join(lines), "\n".join(live)


_IF_ZERO = re.compile(r"[ \t]*#[ \t]*if[ \t]*\(?[ \t]*0[ \t]*\)?[ \t]*$")


# --- block structure ---------------------------------------------------------------

_TEMPLATE_HEAD = re.compile(r"^\s*template\s*<")
_ATTRIBUTE = re.compile(r"\[[^\[\]]*\]")     # C++/CLI `[Serializable( )]`
_BODY_TAIL = re.compile(r"(?:\)|\b(?:const|volatile|override|sealed|abstract)|\bthrow\s*\([^)]*\))\s*$")


def _strip_template_heads(head: str) -> str:
    while True:
        m = _TEMPLATE_HEAD.match(head)
        if not m:
            return head
        depth, k = 1, m.end()
        while k < len(head) and depth:
            if head[k] == "<":
                depth += 1
            elif head[k] == ">":
                depth -= 1
            k += 1
        head = head[k:]


@dataclass
class Block:
    kind: str        # "code" | "decl" | "init"
    parent: str      # kind of the enclosing block ("decl" at file scope)
    open_line: int
    close_line: int = 0


def blocks(src: Source) -> tuple[list[Block], list[int]]:
    """Classify every `{` of the statement view as a function/statement body
    ("code"), a namespace/class/enum/property scope ("decl") or an initializer
    ("init"). Returns the blocks and `marks`, where `marks[i]` is the id of
    the innermost block enclosing offset i (-1 at file scope)."""
    text = src.stmt
    out: list[Block] = []
    stack: list[int] = []
    head_start = 0
    paren = 0
    marks: list[int] = [-1] * (len(text) + 1)
    current = -1
    for i, ch in enumerate(text):
        marks[i] = current
        if ch == "(":
            paren += 1
        elif ch == ")":
            paren = max(0, paren - 1)
        elif ch == "{":
            parent = out[current].kind if current >= 0 else "decl"
            head = text[head_start:i]
            if parent == "code":
                kind = "init" if re.search(r"[=,(]\s*$", head) or paren else "code"
            elif parent == "init":
                kind = "init"
            else:
                h = _ATTRIBUTE.sub(" ", _strip_template_heads(head))
                if re.search(r"=\s*$", h) or (re.search(r"=", h) and "(" not in h):
                    kind = "init"
                elif _BODY_TAIL.search(h):
                    kind = "code"     # `f( )`, `X( ) : m( 0 )`, `g( ) const`, `get( ) override`
                else:
                    kind = "decl"     # class / struct / namespace / enum / extern "C" / property
            out.append(Block(kind, parent, src.line_of(i)))
            stack.append(len(out) - 1)
            current = len(out) - 1
            head_start = i + 1
        elif ch == "}":
            if stack:
                out[stack.pop()].close_line = src.line_of(i)
            current = stack[-1] if stack else -1
            head_start = i + 1
        elif ch == ";" and paren == 0:
            head_start = i + 1
    marks[len(text)] = current
    return out, marks


# --- kind scanners: each returns a list of offsets ---------------------------------


def _match_paren(text: str, open_at: int) -> int:
    """Offset of the `)` closing the `(` at `open_at`, or -1."""
    depth = 0
    for k in range(open_at, len(text)):
        if text[k] == "(":
            depth += 1
        elif text[k] == ")":
            depth -= 1
            if depth == 0:
                return k
    return -1


_CASE_IDIOM = re.compile(r"^\s*(?:case\b[^:]*|default)\s*:")
_TAIL_JUMP = re.compile(r"^\s*(?:break|continue|return\b[^;]*)\s*$")
_LOCAL_DECL_KEYWORD = re.compile(r"^\s*(?:typedef|using|friend|static_assert|STATIC_ASSERT)\b")


def joined_sites(src: Source, ctx) -> list[int]:
    blist, marks = ctx["blocks"]
    text = src.stmt
    out = []
    lines = text.split("\n")
    offset = 0
    paren = 0
    for line in lines:
        # depth-0 statement terminators inside a code block on this line
        terms = []
        for k, ch in enumerate(line):
            if ch == "(":
                paren += 1
            elif ch == ")":
                paren = max(0, paren - 1)
            elif ch == ";" and paren == 0:
                b = marks[offset + k]
                if b >= 0 and blist[b].kind == "code":
                    terms.append(k)
        if terms:
            pieces = []
            prev = 0
            for k in terms:
                pieces.append(line[prev:k])
                prev = k + 1
            # statements on this line: every non-empty piece a terminator ends
            # (an empty one is a stray `;;`), plus unterminated code after the last
            nonempty = [p for p in pieces if p.strip(" \t{}")]
            extra = line[prev:].strip(" \t{})\\")
            count = len(nonempty) + (1 if extra else 0)
            if count >= 2 and not _LOCAL_DECL_KEYWORD.match(pieces[0]):
                if _CASE_IDIOM.match(line) and count == 2 and len(nonempty) == 2 \
                        and _TAIL_JUMP.match(nonempty[1]):
                    pass   # `case X: stmt; break;`
                elif src.is_header and _one_line_body(blist, marks, offset, terms):
                    pass
                else:
                    out.append(offset + terms[0])
        offset += len(line) + 1
    return out


def _header_one_liner(src: Source, block: Block) -> bool:
    """A function body opened and closed on one header line: admitted shape."""
    return src.is_header and block.parent != "code" and block.open_line == block.close_line


def _one_line_body(blist, marks, offset, terms) -> bool:
    """Every terminator sits in a function body opened and closed on this line."""
    for k in terms:
        b = blist[marks[offset + k]]
        if not (b.parent != "code" and b.open_line == b.close_line):
            return False
    return True


_CTL = re.compile(r"\b(if|while|for|switch)\s*\(")
_ELSE = re.compile(r"\belse\b")


def _rest_of_line(text: str, at: int) -> str:
    end = text.find("\n", at)
    return text[at:end if end >= 0 else len(text)]


def ctl_line_sites(src: Source, ctx) -> list[int]:
    blist, marks = ctx["blocks"]
    text = src.stmt
    out = set()
    for m in _CTL.finditer(text):
        if m.group(1) == "switch":
            continue
        b = marks[m.start()]
        if b < 0 or blist[b].kind != "code" or _header_one_liner(src, blist[b]):
            continue
        close = _match_paren(text, m.end() - 1)
        if close < 0:
            continue
        rest = _rest_of_line(text, close + 1).strip()
        if not rest or rest == "{" or rest == ";":
            continue
        if m.group(1) == "while" and text[max(0, m.start() - 256):m.start()].rstrip().endswith("}"):
            continue   # do { } while ( ... );
        out.add(text.rfind("\n", 0, close) + 1)
    for m in _ELSE.finditer(text):
        b = marks[m.start()]
        if b < 0 or blist[b].kind != "code" or _header_one_liner(src, blist[b]):
            continue
        rest = _rest_of_line(text, m.end()).strip()
        if not rest or rest == "{" or re.match(r"if\s*\(", rest):
            continue
        out.add(text.rfind("\n", 0, m.start()) + 1)
    return sorted(out)


_STEER_PRAGMA = re.compile(
    r"^[ \t]*#[ \t]*pragma[ \t]+(?:optimize|inline_depth|inline_recursion|auto_inline"
    r"|check_stack|runtime_checks|function|float_control|fp_contract)\b", re.MULTILINE)
_LINE_PIN = re.compile(r"^[ \t]*#[ \t]*line\b", re.MULTILINE)
_INLINE_PIN = re.compile(r"__declspec\s*\(\s*noinline\s*\)|\b__forceinline\b")
_VOLATILE = re.compile(r"\bvolatile\b")
_UNKNOWN = re.compile(r"\bUNKNOWN_EXPRESSION(?:_T)?\b")
_STATE = re.compile(r"\bSTATE\[[A-Z_]+\]")
_TODO = re.compile(r"\bsushi@TODO\b")


def _regex(pattern, view="live"):
    def sites(src: Source, _ctx) -> list[int]:
        return [m.start() for m in pattern.finditer(getattr(src, view))]
    return sites


def unknown_sites(src: Source, _ctx) -> list[int]:
    return [m.start() for m in _UNKNOWN.finditer(src.live)
            if not re.match(r"[ \t]*#[ \t]*define\s*$",
                            src.live[src.live.rfind("\n", 0, m.start()) + 1:m.start()])]


def _comment_sites(pattern):
    def sites(src: Source, _ctx) -> list[int]:
        out = []
        for start, end in src.comments:
            out.extend(start + m.start() for m in pattern.finditer(src.raw[start:end]))
        return out
    return sites


# dead conditions
_CONST_COND = re.compile(
    r"\bif\s*\(\s*(?:!\s*)?(?:(?:::)?(?:vostok::)?identity\s*\(\s*)?"
    r"(?:0|1|true|false|NULL)\s*\)?\s*\)")
_COND_HEAD = re.compile(r"\b(else[ \t]+)?(if|while)\s*\(")


def _norm(expr: str) -> str:
    return re.sub(r"\s+", "", expr)


def _split_top(expr: str, ops=("&&", "||")) -> list[str]:
    parts, depth, cur, k = [], 0, "", 0
    while k < len(expr):
        ch = expr[k]
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if depth == 0 and expr[k:k + 2] in ops:
            parts.append(cur)
            cur = ""
            k += 2
            continue
        cur += ch
        k += 1
    parts.append(cur)
    return parts


def _strip_parens(expr: str) -> str:
    expr = expr.strip()
    while expr.startswith("(") and _match_paren(expr, 0) == len(expr) - 1:
        expr = expr[1:-1].strip()
    return expr


def _repeats(cond: str) -> bool:
    """`a && a`, `a || a` or `a == a` anywhere in the condition tree."""
    for group in (_split_top(cond, ("||",)), *(_split_top(d, ("&&",))
                                               for d in _split_top(cond, ("||",)))):
        operands = [_norm(_strip_parens(p)) for p in group]
        operands = [p for p in operands if p]
        if len(operands) != len(set(operands)):
            return True
        for p in group:
            sides = _split_top(_strip_parens(p), ("==", "!="))
            if len(sides) == 2 and _norm(sides[0]) and _norm(sides[0]) == _norm(sides[1]):
                return True
    return False


def _indent(text: str, at: int) -> int:
    start = text.rfind("\n", 0, at) + 1
    lead = re.match(r"[ \t]*", text[start:at]).group()
    return len(lead.expandtabs(4))


_EMPTY_BODY = re.compile(r"\s*(?:;|\{\s*\}(?!\s*else\b))")


def dead_cond_sites(src: Source, ctx) -> list[int]:
    blist, marks = ctx["blocks"]
    text = src.stmt
    out = set(m.start() for m in _CONST_COND.finditer(text))
    # if / else-if chains, keyed by brace depth and the indentation of the
    # chain's lines, so a brace-less nested `if` does not join its parent's chain
    chains: dict[tuple[int, int], list[str]] = {}
    depth_at = _brace_depths(text)
    for m in _COND_HEAD.finditer(text):
        b = marks[m.start()]
        if b < 0 or blist[b].kind != "code":
            continue
        close = _match_paren(text, m.end() - 1)
        if close < 0:
            continue
        cond = src.nocomment[m.end():close]   # literals intact: "a" != "b"
        if _repeats(cond):
            out.add(m.start())
        if m.group(2) != "if":
            continue
        # an empty body: `if ( x ) ;` or a bare `{ }` (not an empty branch of a
        # chain, and not one holding commented-out code)
        if _EMPTY_BODY.match(src.nocomment, close + 1) and \
                _EMPTY_BODY.match(src.raw, close + 1):
            out.add(m.start())
        key = (depth_at[m.start()], _indent(text, m.start()))
        norm = _norm(_strip_parens(cond))
        if m.group(1):
            chain = chains.setdefault(key, [])
            if norm in chain:
                out.add(m.start())
            chain.append(norm)
        else:
            chains[key] = [norm]
    return sorted(out)


def _brace_depths(text: str) -> list[int]:
    out, d = [0] * (len(text) + 1), 0
    for i, ch in enumerate(text):
        out[i] = d
        if ch == "{":
            d += 1
        elif ch == "}":
            d = max(0, d - 1)
    out[len(text)] = d
    return out


_IDENTITY = re.compile(r"(?<![.>\w])(?:::)?(?:vostok::)?(?:debug::)?identity\s*\(")
_RUNTIME_PATH = re.compile(r"^(?:this\s*->\s*)?([a-z_]\w*)(?:\s*(?:\.|->)\s*[a-z_]\w*)*$")


def opacity_sites(src: Source, ctx) -> list[int]:
    text = src.stmt
    out = []
    for m in _IDENTITY.finditer(text):
        close = _match_paren(text, m.end() - 1)
        if close < 0:
            continue
        arg = text[m.end():close].strip()
        path = _RUNTIME_PATH.match(arg)
        if not path:
            continue
        root = path.group(1)
        if root in ("true", "false"):
            continue
        if not root.startswith("m_"):
            if re.search(r"\bconst\b[^;{}()=]*\b" + re.escape(root) + r"\s*(?:=|;|\[)", src.code):
                continue   # a const local: identity( ) silences C4127, the idiom
            if re.search(r"\btemplate\s*<[^;{}]*\b" + re.escape(root) + r"\b", src.code):
                continue   # a template parameter: same idiom
        out.append(m.start())
    return out


def padding_sites(src: Source, ctx) -> list[int]:
    blist, marks = ctx["blocks"]
    out = []
    run = 0
    for number, line in enumerate(src.raw.split("\n"), 1):
        if not line.strip():
            run += 1
            continue
        if run >= 3:
            first = src.line_starts[number - 1 - run]
            b = marks[min(first, len(marks) - 1)]
            if b >= 0 and blist[b].kind == "code":
                out.append(first)
        run = 0
    return out


# C-style casts. One scanner over every `( ... )` group whose content reads as a
# type: what precedes it must not end an expression or name a function/keyword
# (so `f(int)`, `sizeof(T)`, `if (x) y` are not casts), and an expression must
# follow. A builtin or pointer type is a cast whatever operator follows
# (`(u32)-1`, `(T*)&x`); a bare user name `(name)` needs an operand to follow
# that only a cast admits (an identifier, `(`, a literal), since `(a) - b` is a
# parenthesized expression.
_BUILTIN = (r"(?:int|char|short|long|float|double|bool|unsigned|signed|wchar_t|__int64|size_t"
            r"|u8|u16|u32|u64|s8|s16|s32|s64|f32|f64|pstr|pcstr|pbyte|pcbyte|pvoid|pcvoid"
            r"|BYTE|WORD|DWORD|UINT|INT|LONG|ULONG|HRESULT|LPVOID|LPCSTR|LPSTR|HANDLE)")
_NAME = r"(?:::)?[A-Za-z_]\w*(?:\s*<[^()<>;]*(?:<[^()<>;]*>[^()<>;]*)*>)?(?:\s*::\s*[A-Za-z_]\w*(?:\s*<[^()<>;]*>)?)*"
_TYPE = re.compile(
    r"^\s*(?:(?:const|volatile|struct|class|enum|typename)\s+)*"
    r"(?P<name>" + _NAME + r")"
    r"(?:\s+(?:int|char|long|short|double|__int64))*"
    r"(?:\s+const|\s+volatile)*"
    r"(?P<ptr>(?:\s*[*&]\s*(?:const\b\s*|volatile\b\s*)*)*)\s*$")
_NOT_TYPE = {"this", "void", "return", "sizeof", "true", "false", "NULL", "new", "delete",
             "operator", "throw", "case", "else", "if", "while", "for", "switch", "do"}
_CAST_KEYWORDS = {"return", "case", "throw", "else", "delete"}
_STRONG_FOLLOW = re.compile(r"\s*(?:[\w(&*~!\"'-]|::)")
_WEAK_FOLLOW = re.compile(r"\s*(?:[A-Za-z_(\"']|\d|::)")
_PREV_TOKEN = re.compile(r"([\w]+|\S)?\s*$")


def c_cast_sites(src: Source, _ctx) -> list[int]:
    text = src.live
    out = []
    closes: set[int] = set()   # `)` of counted casts: `(u32)(s32)x` is two
    for m in re.finditer(r"\(", text):
        close = text.find(")", m.end())
        if close < 0 or "(" in text[m.end():close] or "\n" in text[m.end():close]:
            continue
        inner = text[m.end():close]
        t = _TYPE.match(inner)
        if not t:
            continue
        name = re.sub(r"\s+", "", t.group("name"))
        base = name.split("<")[0].split("::")[-1]
        ptr = t.group("ptr").strip()
        if base in _NOT_TYPE and not (ptr and base == "void"):
            continue   # `(void)x` is the discard idiom; `(void*)p` is a cast
        window = text[max(0, m.start() - 64):m.start()]
        prev = _PREV_TOKEN.search(window).group(1) or ""
        prev_at = m.start() - (len(window) - len(window.rstrip())) - 1
        chained = prev == ")" and prev_at in closes
        if prev and (prev[-1].isalnum() or prev[-1] in "_)]>") \
                and prev not in _CAST_KEYWORDS and not chained:
            continue
        strong = bool(ptr) or re.fullmatch(_BUILTIN, base) is not None
        if not strong and src.stmt[m.start()] != "(":
            continue   # a macro body: `(value1) operation (value2)` juxtaposes parameters
        follow = _STRONG_FOLLOW if strong else _WEAK_FOLLOW
        if not follow.match(text, close + 1):
            continue
        out.append(m.start())
        closes.add(close)
    return out


# stale matching prose: per comment block (consecutive comment lines)
_PROSE_TAG = re.compile(r"\b(?:claude|codex)@\w+|\bFUNCTION BODY\b")
_PROSE = re.compile(
    r"\b(?:claude|codex)@\w+"
    r"|\bLTCG\b|\bICF\b|\bobjdiff\b|\bdelinker\b|\bstructure[- ](?:diff|capped|match)\b"
    r"|\bSTRUCTURE MATCH\b|\bresidual\b|\binline-vs-call\b|\bcapped by\b"
    r"|\bbyte[- ](?:identical|equal|exact)\b|\bfold-rep\b|\bFUNCTION BODY\b"
    r"|\b(?:target|base|gold)\s+(?:inlines|out-of-lines|calls|emits|folds|schedules|keeps"
    r"|passes|records|compiles|spills)\b"
    r"|\b\d{1,3}\.\d{1,2}\s?%"
    r"|\?\?[0-9_?][A-Za-z0-9_?@]*@@|@@[QAIUM][AB]E"
    r"|\b(?:QUANTITY|SPLIT)\b(?= row| class|/)"
    r"|\b\d+\s+stmts?\b|\bsema blocks\b|\bvostok-pdb\b|\bpdb inspect\b")


_COMMENTED_CODE = re.compile(r"//[ \t]*[\w:.>-]+[ \t]*\(.*\)[ \t]*;[ \t]*$", re.MULTILINE)
_LIVE_MARKER = re.compile(r"\bsushi@TODO\b|\bSTATE\[[A-Z_]+\]")


def prose_sites(src: Source, _ctx) -> list[int]:
    """One site per comment block (comments on consecutive lines) that carries
    narration. A block anchoring a live marker (`sushi@TODO`, `STATE[...]`) is
    counted in that marker's column instead: the convention lets it name a wall."""
    out = []
    block: list[tuple[int, int]] = []

    def flush():
        hit = None
        for start, end in block:
            if _LIVE_MARKER.search(src.raw, start, end):
                return
            # commented-out code (`//\tLOG_INFO( "residual: %f", r );`) speaks
            # the domain's words, not ours: only an agent tag counts there
            pattern = _PROSE_TAG if _COMMENTED_CODE.match(src.raw, start, end) else _PROSE
            m = pattern.search(src.raw, start, end)
            if m and hit is None:
                hit = m.start()
        if hit is not None:
            out.append(hit)

    last_line = -2
    for start, end in src.comments:
        if src.line_of(start) > last_line + 1:
            flush()
            block = []
        block.append((start, end))
        last_line = src.line_of(max(start, end - 1))
    flush()
    return out


# (key, scanner, how-to-fix)
KINDS = (
    ("joined", joined_sites,
     "one statement per line; a retail record spanning two statements is evidence of "
     "a helper, macro or different expression, never a licence to glue lines"),
    ("ctl-line", ctl_line_sites,
     "put the controlled statement on its own line"),
    ("pragma", _regex(_STEER_PRAGMA),
     "recover the natural declaration/TU state instead of a codegen pragma"),
    ("line-pin", _regex(_LINE_PIN),
     "lay the source out on retail's lines instead of pinning them with #line"),
    ("inline-pin", _regex(_INLINE_PIN),
     "recover why retail inlined or out-lined the call; a pin is a sushi@TODO at best"),
    ("dead-cond", dead_cond_sites,
     "drop the constant/duplicate/empty test or find the real expression it stands for"),
    ("opacity", opacity_sites,
     "identity( ) around a runtime value only hides it from the optimizer; recover the real shape"),
    ("padding", padding_sites,
     "blank-line runs inside a body exist only to move __LINE__; recover retail's lines"),
    ("volatile", _regex(_VOLATILE),
     "volatile is not a codegen lever; recover the real type or lifetime"),
    ("c-cast", c_cast_sites,
     "spell it as a named C++ cast or fix the type"),
    ("unknown", unknown_sites,
     "recover the asserted expression"),
    ("state", _comment_sites(_STATE),
     "resolve the stub/inline/unmatchable marker and drop it"),
    ("todo", _comment_sites(_TODO),
     "answer the review question; clear the anchor and its review_todos.md row"),
    ("prose", prose_sites,
     "delete matching narration; keep at most a lean why"),
)
KIND_KEYS = tuple(k for k, _, _ in KINDS)
_SCANNERS = {k: fn for k, fn, _ in KINDS}
_FIX = {k: fix for k, _, fix in KINDS}


# --- the scan ----------------------------------------------------------------------


def read_text(path: Path) -> str:
    """The editor's WinForms headers are UTF-16; everything else is UTF-8/ANSI."""
    data = path.read_bytes()
    if data[:2] in (b"\xff\xfe", b"\xfe\xff"):
        text = data.decode("utf-16", errors="replace")
    elif b"\x00" in data[:256]:
        text = data.decode("utf-16-le", errors="replace")
    else:
        text = data.decode("utf-8", errors="replace")
    return text.lstrip("\ufeff").replace("\r\n", "\n").replace("\r", "\n")


def module_of(rel: str) -> str:
    parts = rel.split("/")
    return parts[0] if len(parts) > 1 else ROOT_MODULE


def is_vendored(rel: str) -> bool:
    return any(rel == v or rel.startswith(v) for v in VENDORED)


def iter_sources(root: Path):
    for path in sorted(root.rglob("*")):
        if path.suffix not in EXTS or not path.is_file():
            continue
        rel = path.relative_to(root).as_posix()
        if is_vendored(rel):
            continue
        yield rel, path


def scan_source(src: Source, kinds=KIND_KEYS) -> dict[str, list[int]]:
    ctx = {"blocks": blocks(src)}
    return {k: _SCANNERS[k](src, ctx) for k in kinds}


@dataclass
class Site:
    kind: str
    module: str
    rel: str
    line: int
    text: str


_BLANK_RUN = re.compile(r"(?:[ \t]*\n)+")


def _scan_file(job) -> list[tuple[str, int, str]]:
    path, kinds = job
    src = Source(path, read_text(path))
    out = []
    for kind, offsets in scan_source(src, kinds).items():
        for off in offsets:
            line = src.line_of(off)
            text = src.line_text(line).strip()
            if kind == "padding":
                run = _BLANK_RUN.match(src.raw, off).group()
                text = f"({run.count(chr(10))} blank lines)"
            out.append((kind, line, text))
    return out


def scan(root: Path | None = None, module: str | None = None, kinds=KIND_KEYS,
         jobs: int | None = None):
    """(board {module: Counter(kind)}, [Site]) over the engine tree."""
    files = [(rel, path) for rel, path in iter_sources(root or ENGINE)
             if not module or module_of(rel) == module]
    work = [(path, tuple(kinds)) for _rel, path in files]
    jobs = jobs if jobs is not None else min(os.cpu_count() or 1, 16)
    if jobs > 1 and len(work) > 64:
        with ProcessPoolExecutor(jobs) as pool:
            results = list(pool.map(_scan_file, work, chunksize=32))
    else:
        results = [_scan_file(job) for job in work]
    board: dict[str, Counter] = defaultdict(Counter)
    sites: list[Site] = []
    for (rel, _path), found in zip(files, results):
        mod = module_of(rel)
        board[mod]  # every scanned module gets a row
        for kind, line, text in found:
            board[mod][kind] += 1
            sites.append(Site(kind, mod, rel, line, text))
    return dict(board), sites


def totals(board) -> Counter:
    out = Counter()
    for counts in board.values():
        out.update(counts)
    return out


# --- baseline ----------------------------------------------------------------------


def load_baseline(path: Path | None = None) -> tuple[list[str], dict[str, dict[str, int]]]:
    """(kind columns, {module: {kind: floor}}); comment lines start with `#`."""
    path = path or CLEANLINESS_BASELINE
    if not path.is_file():
        return [], {}
    rows = [line.split("\t") for line in path.read_text().splitlines()
            if line.strip() and not line.startswith("#")]
    if not rows or rows[0][0] != "module":
        return [], {}
    columns = rows[0][1:]
    floors = {}
    for row in rows[1:]:
        floors[row[0]] = {k: int(v) for k, v in zip(columns, row[1:]) if v.strip()}
    return columns, floors


def save_baseline(board, path: Path | None = None) -> None:
    path = path or CLEANLINESS_BASELINE
    head = ("# Down-only floors for `vostok tool cleanliness --check`: one row per module,\n"
            "# one column per kind. Only `vostok tool cleanliness --update` writes this file;\n"
            "# lower a floor by cleaning the tree, raise one only as a reviewed act.\n")
    lines = ["\t".join(("module", *KIND_KEYS))]
    for mod in sorted(board):
        lines.append("\t".join((mod, *(str(board[mod][k]) for k in KIND_KEYS))))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(head + "\n".join(lines) + "\n")


def check(board, path: Path | None = None) -> list[str]:
    """Ratchet violations: every module x kind cell above its floor. Fails
    closed: a kind with no floor column is a violation, not a pass (gruntz's
    lesson - deleting the floors must not turn the gate green)."""
    path = path or CLEANLINESS_BASELINE
    columns, floors = load_baseline(path)
    bad = []
    missing = [k for k in KIND_KEYS if k not in columns]
    if missing:
        bad.append(f"no committed floor for kind(s) {', '.join(missing)} in "
                   f"{_rel(path)}; bless with `vostok tool cleanliness --update`")
    for mod in sorted(board):
        for kind in KIND_KEYS:
            if kind in missing:
                continue
            n = board[mod][kind]
            floor = floors.get(mod, {}).get(kind, 0)
            if n > floor:
                bad.append(f"{mod} {kind}: {floor} -> {n} (+{n - floor}; {_FIX[kind]})")
    return bad


def _rel(path: Path) -> str:
    try:
        return path.relative_to(REPO).as_posix()
    except ValueError:
        return str(path)


# --- selftest ----------------------------------------------------------------------
# {kind: (positives, negatives)}. Each sample is wrapped in a function body
# unless it starts with "@" (file scope) and must count >= 1 / exactly 0.

_SAMPLES = {
    "joined": (
        ("a = 1; b = 2;",
         "foo( ); bar( );",
         "u32 a = 0; u32 b = 1;",
         "if ( x ) { a( ); b( ); }",
         "ASSERT( p ); *p = v;",
         "@void set( int v ) { a = v; b = v; }"),
        ("for ( u32 i = 0; i < n; ++i )",
         "for ( ;; )",
         "x = 1;  // a; b;",
         "s = \"a; b;\";",
         "case 1: f( ); break;",
         "default: return 0;",
         "x = 1;;",
         "@struct s { int a; int b; };",
         "@int f( ); int g( );",
         "#define TWO( ) a( ); b( )",
         "#if 0\na( ); b( );\n#else\nc( );\n#endif",
         "#if 0\n#ifdef X\na( ); b( );\n#endif\na( ); b( );\n#endif",
         "#define THREE( ) \\\n  a( ); /* x */ \\\n  b( ); c( ); // y \\\n  d( ); e( );",
         "for ( u32 i = 0;\n i < n;\n ++i )\n  f( );")),
    "ctl-line": (
        ("if ( x ) return;",
         "if ( x ) { y( ); }",
         "while ( x ) step( );",
         "for ( i = 0; i < n; ++i ) f( i );",
         "if ( a )\n  x( );\nelse y( );"),
        ("if ( x )\n  return;",
         "if ( x ) {\n  y( );\n}",
         "do {\n  f( );\n} while ( x );",
         "while ( *p++ );",
         "if ( a )\n  x( );\nelse if ( b )\n  y( );",
         "if ( a ) {\n} else {\n}",
         "#define CHECK( x ) if ( x ) return",
         "switch ( x ) { default: break; }")),
    "pragma": (
        ("@#pragma optimize( \"\", off )",
         "@#\tpragma inline_depth( 0 )",
         "@#pragma auto_inline( off )"),
        ("@#pragma once",
         "@#line 120",
         "@#pragma warning( disable: 4127 )",
         "@#pragma pack( push, 4 )",
         "@// #pragma optimize( \"\", off )",
         "@#pragma comment( lib, \"x.lib\" )")),
    "line-pin": (
        ("@#line 120", "  #  line 7 \"x.cpp\""),
        ("@// #line 12", "@int line = 3;", "@#pragma once")),
    "inline-pin": (
        ("@__declspec( noinline ) void f( );",
         "@__forceinline int g( ) { return 0; }"),
        ("@inline int f( ) { return 0; }",
         "@// __declspec(noinline) was a probe",
         "@int no_inline_count;")),
    "dead-cond": (
        ("if ( 0 )\n  f( );",
         "if ( false ) {\n}",
         "if ( identity( false ) )\n  f( );",
         "if ( a && a )\n  f( );",
         "if ( a == a )\n  f( );",
         "if ( x ) ;",
         "if ( x )\n  ;",
         "if ( x ) { }",
         "if ( a )\n  f( );\nelse if ( b )\n  g( );\nelse if ( a )\n  h( );"),
        ("if ( a && b )\n  f( );",
         "while ( true )\n  f( );",
         "if ( a == b )\n  f( );",
         "if ( a )\n  f( );\nelse if ( b )\n  g( );",
         "if ( a )\n  f( );\nif ( a )\n  g( );",
         "if ( s == \"a\" )\n  f( );\nelse if ( s == \"b\" )\n  g( );",
         "if ( c == 'a' || c == 'b' )\n  f( );",
         "if ( a && b || c && b )\n  f( );",
         "if ( a )\n{\n}\nelse if ( b )\n  g( );",
         "if ( a )\n{\n  // f( );\n}",
         "if ( a )\n  if ( b )\n    f( );\n  else\n    g( );\nelse if ( b )\n  h( );",
         "@#if 0\n#endif",
         "@#define ASSERT( e ) if ( false ) { e; } else (void)0")),
    "opacity": (
        ("if ( !identity( m_is_forward_lighting_pass ) )\n  f( );",
         "bool hit = query( );\nif ( !identity( hit ) )\n  f( );",
         "if ( vostok::identity( this->m_flag ) )\n  f( );"),
        ("bool const c = x;\nif ( identity( c ) )\n  f( );",
         "@template < bool allow >\nvoid f( ) { if ( identity( allow ) ) g( ); }",
         "m = float4x4( ).identity( );",
         "while ( identity( true ) )\n  f( );",
         "if ( identity( sizeof( T ) < 4 ) )\n  f( );",
         "if ( identity( Size ) )\n  f( );",
         "@#define CHECK( e ) if ( !::vostok::identity( e ) ) fail( )")),
    "padding": (
        ("a( );\n\n\n\nb( );",),
        ("a( );\n\nb( );",
         "@int a;\n\n\n\nint b;")),
    "volatile": (
        ("volatile int homed = value;",
         "int* volatile p = q;"),
        ("int volatile_count = 0;",
         "#if 0\nvolatile int x;\n#endif",
         "s = \"volatile\";",
         "// volatile was a probe")),
    "c-cast": (
        ("x = (int)y;",
         "x = (u32)-1;",
         "p = (char const*)q;",
         "p = (render::buffer*)q;",
         "return (handle)this;",
         "v = (float)( a + b );",
         "x = (my_enum)value;",
         "f( (pcstr)name );"),
        ("void f( int );",
         "n = sizeof( int );",
         "if ( x ) y( );",
         "(void)unused;",
         "z = (a) - b;",
         "z = (a) * b;",
         "x = static_cast< int >( y );",
         "x = u32( y );",
         "@typedef void ( *fn )( int );",
         "s = \"(int)x\";",
         "// (char*)p in prose",
         "t = c ? (a) : (b);",
         "return (x);",
         "@#define CHECK( a, op, b ) if ( !((a) op (b)) ) fail( )")),
    "unknown": (
        ("ASSERT( UNKNOWN_EXPRESSION );",
         "ASSERT( UNKNOWN_EXPRESSION_T( x ) );"),
        ("@#define UNKNOWN_EXPRESSION true",
         "// UNKNOWN_EXPRESSION in prose")),
    "state": (
        ("// STATE[STUB]: body unrecovered",
         "/* STATE[INLINED] */"),
        ("s = \"STATE[STUB]\";",
         "state[ 0 ] = 1;")),
    "todo": (
        ("// sushi@TODO: why does retail out-line this?",),
        ("// sushi@NOTE: kept",
         "todo = 1;")),
    "prose": (
        ("// claude@NOTE: structure + locals match",
         "// residual = base inlines the helper",
         "// LTCG passes this in EAX",
         "// 98.57% after the change",
         "// ctor mangles ??0foo@@IAE@XZ -> protected",
         "// target inlines r<float2>, base calls it"),
        ("// retail leaks this buffer",
         "// the target is out of range",
         "// declared protected: derived classes read it",
         "// sushi@TODO: open question",
         "// sushi@TODO: target inlines this, base calls it (LTCG)",
         "// sushi@NOTE: we own this pointer",
         "//\tLOG_INFO( \"rtp residual: %f \", m_last_residual );",
         "s = \"LTCG\";")),
}

# exact counts: a positive only proves >= 1
_EXACT = {
    "joined": (("a( ); b( ); c( );", 1), ("a( ); b( );\nc( ); d( );", 2),
               ("#if 0\nx( );\n#elif 1\na( ); b( );\n#endif", 1)),
    "prose": (("// claude@NOTE: one\n// residual two\n// LTCG three", 1),
              ("// claude@NOTE: one\n\n// claude@NOTE: two", 2)),
    "c-cast": (("x = (u32)(s32)y;", 2),),
    "padding": (("a( );\n\n\n\n\n\nb( );", 1),),
}


def _sample_source(sample: str) -> Source:
    if sample.startswith("@"):
        text = sample[1:] + "\n"
    else:
        text = "void sample_function( )\n{\n" + sample + "\n}\n"
    return Source(Path("sample.cpp"), text)


def selftest() -> list[str]:
    failures = []
    for kind, (positives, negatives) in _SAMPLES.items():
        for sample in positives:
            got = len(scan_source(_sample_source(sample), (kind,))[kind])
            if got < 1:
                failures.append(f"{kind}: MISSED positive {sample!r}")
        for sample in negatives:
            got = len(scan_source(_sample_source(sample), (kind,))[kind])
            if got:
                failures.append(f"{kind}: FALSE POSITIVE ({got}) {sample!r}")
    for kind, samples in _EXACT.items():
        for sample, expected in samples:
            got = len(scan_source(_sample_source(sample), (kind,))[kind])
            if got != expected:
                failures.append(f"{kind}: counted {got}, expected {expected} in {sample!r}")
    # a one-line body is admitted in a header, counted in a .cpp
    body = ("[Serializable( )]\nref class s {\n property int p {\n"
            "  void set( int v ) { a = v; b = v; }\n }\n};\n")
    if len(scan_source(Source(Path("s.h"), body), ("joined",))["joined"]) != 0:
        failures.append("joined: one-line header body counted")
    if len(scan_source(Source(Path("s.cpp"), body), ("joined",))["joined"]) != 1:
        failures.append("joined: one-line .cpp body not counted")
    missing = set(KIND_KEYS) - set(_SAMPLES)
    failures.extend(f"{k}: NO SELFTEST SAMPLES" for k in sorted(missing))
    return failures


# --- output ------------------------------------------------------------------------


def board_rows(board, kinds=KIND_KEYS):
    order = sorted(board, key=lambda m: (-sum(board[m][k] for k in kinds), m))
    return [(m, [board[m][k] for k in kinds]) for m in order]


def render_text(board, floors=None, kinds=KIND_KEYS) -> str:
    rows = board_rows(board, kinds)
    tot = totals(board)
    width = max([len("module")] + [len(m) for m, _ in rows] + [len("total")])
    colw = [max(len(k), 4) for k in kinds]
    out = ["  ".join(["module".ljust(width)] + [k.rjust(w) for k, w in zip(kinds, colw)])]
    for mod, counts in rows:
        cells = []
        for k, n, w in zip(kinds, counts, colw):
            cell = str(n) if n else "."
            if floors is not None:
                floor = floors.get(mod, {}).get(k, 0)
                if n > floor:
                    cell += f"+{n - floor}"
            cells.append(cell.rjust(w))
        out.append("  ".join([mod.ljust(width)] + cells))
    out.append("  ".join(["total".ljust(width)] + [str(tot[k]).rjust(w) for k, w in zip(kinds, colw)]))
    return "\n".join(out)


def render_markdown(board, kinds=KIND_KEYS) -> str:
    rows = board_rows(board, kinds)
    tot = totals(board)
    out = ["| module | " + " | ".join(kinds) + " |",
           "|---|" + "---:|" * len(kinds)]
    for mod, counts in rows:
        if not any(counts):
            continue
        out.append(f"| {mod} | " + " | ".join(str(n) if n else "" for n in counts) + " |")
    out.append("| **total** | " + " | ".join(f"**{tot[k]}**" for k in kinds) + " |")
    return "\n".join(out)


def top_files(sites, n: int, kinds=KIND_KEYS) -> dict[str, list[tuple[str, int]]]:
    per = defaultdict(Counter)
    for s in sites:
        per[s.kind][s.rel] += 1
    return {k: per[k].most_common(n) for k in kinds if per[k]}


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(prog="vostok tool cleanliness",
                                 description=__doc__.split("\n\n")[1],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--module", help="one module (first directory under sources/vostok; "
                                     f"top-level headers are {ROOT_MODULE})")
    ap.add_argument("--list", metavar="KIND", choices=(*KIND_KEYS, "all"),
                    help="print file:line for every site of KIND (or all)")
    ap.add_argument("--top", type=int, metavar="N", help="the N files with most sites per kind")
    ap.add_argument("--json", action="store_true", help="the board as JSON")
    ap.add_argument("--markdown", action="store_true", help="the board as a Markdown table")
    ap.add_argument("--check", action="store_true",
                    help="exit 1 when a module x kind count rose above its committed floor")
    ap.add_argument("--update", action="store_true",
                    help="bless: rewrite config/cleanliness/baseline.tsv with the current counts")
    ap.add_argument("--selftest", action="store_true", help="run the embedded samples only")
    args = ap.parse_args(argv)

    broken = selftest()
    if broken:
        for line in broken:
            print(f"cleanliness SELFTEST BROKEN: {line}", file=sys.stderr)
        return 2
    if args.selftest:
        total = sum(len(p) + len(n) for p, n in _SAMPLES.values())
        print(f"selftest OK ({len(_SAMPLES)} kinds, {total} samples)")
        return 0
    if args.update and args.module:
        ap.error("--update rewrites every module's floors; drop --module")

    board, sites = scan(module=args.module)
    if args.update:
        save_baseline(board)
        print(f"cleanliness floors written to {_rel(CLEANLINESS_BASELINE)}: "
              + ", ".join(f"{k} {totals(board)[k]}" for k in KIND_KEYS))
        return 0
    if args.list:
        for s in sorted(sites, key=lambda s: (s.rel, s.line, s.kind)):
            if args.list in ("all", s.kind):
                print(f"sources/vostok/{s.rel}:{s.line}: [{s.kind}] {s.text}")
        return 0
    if args.json:
        print(json.dumps({"kinds": list(KIND_KEYS),
                          "modules": {m: {k: board[m][k] for k in KIND_KEYS} for m in sorted(board)},
                          "totals": {k: totals(board)[k] for k in KIND_KEYS}}, indent=1))
        return 0
    if args.top:
        for kind, files in top_files(sites, args.top).items():
            print(f"{kind}: " + ", ".join(f"sources/vostok/{f} ({n})" for f, n in files))
        return 0
    if args.markdown:
        print(render_markdown(board))
        return 0

    _columns, floors = load_baseline()
    print(render_text(board, floors if floors else None))
    if not args.check:
        return 0
    if args.module:
        board = {m: c for m, c in board.items() if m == args.module}
    bad = check(board)
    for line in bad:
        print(f"cleanliness RATCHET: {line}", file=sys.stderr)
    if bad:
        return 1
    print("cleanliness: no count above its committed floor")
    return 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.cleanliness", main))
