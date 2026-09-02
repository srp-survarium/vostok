# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.derive.names - reading MSVC names, and rendering them short.

Two directions. `qualified_name`/`norm_name`/`mangled_name_part` normalize a
demangled signature down to something two sides can be compared on.
`fn_from_mangled` goes the other way: it parses `scope::name` straight out of
the mangled name for report rows, because a full demangled signature is
unreadable in a table and the mangled form carries the scope unambiguously.
"""

import re



_OPERATOR_PLACEHOLDERS = [
    ("operator<<", "operator\x01"),
    ("operator>>", "operator\x02"),
    ("operator<=", "operator\x03"),
    ("operator>=", "operator\x04"),
    ("operator->", "operator\x05"),
    ("operator<", "operator\x06"),
    ("operator>", "operator\x07"),
    ("operator()", "operator\x08"),
]


_ELABORATED_ENUM_RE = re.compile(
    r"(?<![A-Za-z0-9_])enum\s+(?=[A-Za-z_`])"
)


def pdb_signature_canon(text):
    """Erase an optional PDB ``enum`` elaborated-type keyword.

    Retail and candidate PDB demanglers can render the same decorated enum type
    as either ``enum namespace::type`` or ``namespace::type``.  The keyword is
    not part of the C++ type identity; all scopes, template arguments, pointer
    qualifiers, and function parameters remain in the canonical signature.
    """
    return _ELABORATED_ENUM_RE.sub("", text) if text else text


def qualified_name(demangled):
    """Demangled signature -> (class_or_None, name); None for compiler-generated
    names (thunks, backtick names) that have no source declaration."""
    if demangled.startswith("[thunk]") or "`" in demangled:
        return None
    s = demangled
    for op, ph in _OPERATOR_PLACEHOLDERS:
        s = s.replace(op, ph)
    depth = 0
    cut = None
    for i, c in enumerate(s):
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == "(" and depth == 0:
            cut = i
            break
    head = s[:cut] if cut is not None else s
    # qualified name = last space-separated token at angle-depth 0
    depth = 0
    token_start = 0
    for i, c in enumerate(head):
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == " " and depth == 0:
            token_start = i + 1
    qual = head[token_start:]
    # split class::name at the last depth-0 '::'
    depth = 0
    split = None
    i = 0
    while i < len(qual):
        c = qual[i]
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == ":" and depth == 0 and i + 1 < len(qual) and qual[i + 1] == ":":
            split = i
            i += 1
        i += 1
    def restore(t):
        for op, ph in _OPERATOR_PLACEHOLDERS:
            t = t.replace(ph, op)
        return t
    if split is None:
        return None, restore(qual)
    return restore(qual[:split]), restore(qual[split + 2 :])


def norm_name(text):
    return text.replace(" ", "") if text else text


def mangled_name_part(mangled):
    """The qualified-name portion of an MSVC-mangled symbol (everything before
    the first '@@', where the access/convention/type encoding starts)."""
    i = mangled.find("@@")
    return mangled[:i] if i > 0 else mangled


RET_MAX = 24   # return type


SIG_MAX = 80   # name + args (+ trailing const, etc.)


def _split_return(dem):
    """(return_type, name+args) for a demangled signature. The arg list opens at the
    first '(' at angle-bracket depth 0; the return type ends at the nearest depth-0
    space to its left. Ctors/dtors (no return type) yield ('', dem)."""
    depth, paren = 0, -1
    for i, ch in enumerate(dem):
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch == "(" and depth == 0:
            paren = i
            break
    if paren < 0:
        return "", dem
    depth = 0
    for i in range(paren - 1, -1, -1):
        ch = dem[i]
        if ch == ">":
            depth += 1
        elif ch == "<":
            depth -= 1
        elif ch == " " and depth == 0:
            return dem[:i], dem[i + 1:]
    return "", dem


def shorten_fn(dem):
    """Cap the return type and the name+args independently (RET_MAX / SIG_MAX)."""
    ret, rest = _split_return(dem)
    if len(ret) > RET_MAX:
        ret = ret[:RET_MAX - 1] + "…"
    if len(rest) > SIG_MAX:
        rest = rest[:SIG_MAX - 1] + "…"
    return f"{ret} {rest}".strip()


_MANGLED_OPS = {
    "2": "operator new", "3": "operator delete", "4": "operator=",
    "8": "operator==", "9": "operator!=", "A": "operator[]",
    "R": "operator()", "E": "operator++", "F": "operator--",
    "_7": "`vftable'", "_8": "`vbtable'",
}


def _scope_tokens(s):
    """`@`-separated scope identifiers up to the terminating `@@`."""
    toks = []
    for t in s.split("@"):
        if t == "":
            break
        toks.append(t)
    return toks


def _name_from_demangled(dem):
    """Clean 'scope::name' from a demangled signature: drop the return type, every
    <template-arg> block (any depth), and the parameter list. Robust where the
    mangled name has templated scopes the lightweight parser can't walk."""
    if not dem:
        return ""
    _, rest = _split_return(dem)
    out, depth = [], 0
    for ch in rest:                          # strip balanced <...>
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out.append(ch)
    s = "".join(out)
    rp = s.rfind(")")                         # cut the parameter list
    if rp != -1:
        depth = 0
        for i in range(rp, -1, -1):
            if s[i] == ")":
                depth += 1
            elif s[i] == "(":
                depth -= 1
                if depth == 0:
                    s = s[:i]
                    break
    return s.strip()


def _looks_clean(name):
    """A parsed qualified name with no leftover mangling artifacts."""
    return bool(name) and not any(c in name for c in "?$@")


def fn_from_mangled(mangled, demangled=""):
    """A short 'scope::name' that reads far easier than the demangled signature.
    Parsed straight from the MSVC mangled name for the simple cases; for a template
    function or a templated scope (a '?$' - nested type encodings the lightweight
    parser can't skip) it falls back to stripping <...>+params off the demangled."""
    try:
        m = mangled
        if not m.startswith("?"):
            return m  # already a plain qualified name (free functions, etc.)
        body = m[1:]
        if "?$" in body:                      # template fn / templated scope
            return _name_from_demangled(demangled) or body
        if body.startswith("?"):              # ctor / dtor / operator
            body = body[1:]
            code = body[:2] if body[:1] == "_" else body[:1]
            toks = _scope_tokens(body[len(code):].lstrip("@"))
            if not toks:
                raise ValueError
            cls = toks[0]
            leaf = (cls if code == "0"
                    else "~" + cls if code in ("1", "_G", "_E")
                    else _MANGLED_OPS.get(code, "operator" + code))
            name = "::".join(reversed(toks)) + "::" + leaf
        else:                                 # normal function
            base, _, rest = body.partition("@")
            toks = _scope_tokens(rest)
            name = ("::".join(reversed(toks)) + "::" + base) if toks else base
        return name if _looks_clean(name) else (_name_from_demangled(demangled) or name)
    except Exception:
        return _name_from_demangled(demangled) or shorten_fn(demangled)
