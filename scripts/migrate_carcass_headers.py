#!/usr/bin/env python3
"""Migrate bare `// FUNCTION BODY` carcass headers to the new addressed form.

Surgical: only the header LINE of each bare carcass is rewritten to the
`// FUNCTION BODY[0x<addr>]: <count>` produced by the regenerated structure.
Everything else (statement rows, annotations, other sub-blocks, code) is left
byte-for-byte unchanged.

Join strategy:
  * Row-bearing carcass: match by the rva of its first `// <0x...>|` row against
    the structure block (same source path, base or target side) whose rows
    contain that rva. Use that side's header.
  * Body-less carcass (no rva rows): match by the enclosing function's
    name/signature against the same-path structure file, preferring the side
    chosen by the file's row-bearing carcasses.
"""
import re
import sys
from pathlib import Path

DRY_RUN = "--dry-run" in sys.argv

ROOT = Path(__file__).resolve().parent.parent
SRC_ROOT = ROOT / "sources"
STRUCT = {
    "base": ROOT / "binaries" / "structure" / "base",
    "target": ROOT / "binaries" / "structure" / "target",
}

HEADER_BARE = "// FUNCTION BODY"
HEADER_ADDR_RE = re.compile(r"// FUNCTION BODY\[0x[0-9a-fA-F]+\]:\s*\d+\s*$")
ROW_RE = re.compile(r"^\s*//\s*<(0x[0-9a-fA-F]+)>\|")
ADDR_HEADER_RE = re.compile(r"^\s*// FUNCTION BODY\[(0x[0-9a-fA-F]+)\](?::\s*\d+)?\s*$")


def parse_structure_file(path):
    """Return list of blocks: dict(header_line, addr, first_row, all_rows, sig)."""
    lines = path.read_text(encoding="utf-8", errors="replace").split("\n")
    blocks = []
    i = 0
    while i < len(lines):
        m = ADDR_HEADER_RE.match(lines[i])
        if m:
            header_line = lines[i].rstrip("\n")
            addr = m.group(1)
            rows = []
            j = i + 1
            while j < len(lines):
                s = lines[j].strip()
                if s == "// ******":
                    break
                rm = ROW_RE.match(lines[j])
                if rm:
                    rows.append(rm.group(1))
                j += 1
            # function signature: nearest preceding non-comment, non-blank line
            # that is the declarator line (search back to '{')
            sig = find_struct_signature(lines, i)
            blocks.append({
                "header": header_line,
                "addr": addr,
                "rows": rows,
                "first_row": rows[0] if rows else None,
                "sig": sig,
            })
            i = j
        i += 1
    return blocks


def find_struct_signature(lines, header_idx):
    """Signature = function name token from the declarator preceding the block.

    We collect declarator text from the opening line(s) above the `{` that
    encloses this FUNCTION BODY. Returns a normalized signature key.
    """
    # find the '{' line above header_idx
    k = header_idx - 1
    brace = None
    while k >= 0:
        if lines[k].strip() == "{":
            brace = k
            break
        k -= 1
    if brace is None:
        return None
    # declarator: lines above '{' up to previous blank or comment block end
    decl = []
    k = brace - 1
    while k >= 0:
        s = lines[k].strip()
        if s == "" or s.startswith("//") or s == "}":
            break
        decl.insert(0, lines[k])
        k -= 1
    return normalize_sig("\n".join(decl))


def normalize_sig(text):
    """Normalize a declarator to a comparable key: collapse whitespace, drop
    param names / const qualifiers that differ between sides where possible.
    For body-less join we mostly rely on the function NAME, so extract that."""
    t = re.sub(r"\s+", " ", text).strip()
    return t


def _param_span(t):
    """Return (open_idx, close_idx) of the parameter-list parens in a
    normalized declarator: the FIRST top-level balanced `(...)` group that is
    not nested inside `<...>`. Return-type parens (e.g. FastDelegate<
    void(...) >) sit inside angle brackets and are skipped; this avoids the
    later constructor init-list `: m_x(...)` parens too. None if no such group."""
    depth_angle = 0
    i = 0
    n = len(t)
    while i < n:
        ch = t[i]
        if ch == "<":
            depth_angle += 1
        elif ch == ">":
            if depth_angle:
                depth_angle -= 1
        elif ch == "(" and depth_angle == 0:
            d = 0
            j = i
            while j < n:
                if t[j] == "(":
                    d += 1
                elif t[j] == ")":
                    d -= 1
                    if d == 0:
                        return (i, j)
                j += 1
            return None
        i += 1
    return None


def func_name_key(text):
    """Extract `Class::method` (or special backtick name) from a declarator.

    Returns (name, arity) where arity is the count of top-level parameters
    (0 for empty `( )`), used to disambiguate overloads in body-less joins.
    """
    t = re.sub(r"\s+", " ", text).strip()
    span = _param_span(t)
    if span is None:
        head = t
        arity = None
    else:
        o, c = span
        head = t[:o]
        params = t[o + 1:c].strip()
        if params == "":
            arity = 0
        else:
            # count top-level commas
            d = 0
            commas = 0
            for ch in params:
                if ch in "<([":
                    d += 1
                elif ch in ">)]":
                    d -= 1
                elif ch == "," and d == 0:
                    commas += 1
            arity = commas + 1

    head = head.strip()
    if "`" in head:
        # special name like `dynamic initializer for 'x''` or
        # Class::`scalar deleting destructor'
        bt = head.index("`")
        name = head[bt:].strip()
        # keep any Class:: qualifier that precedes the backtick
        prefix = head[:bt].strip()
        # the return type is the leading token(s); the qualifier (if any)
        # is the part ending in '::'
        mq = re.search(r"([\w:]+::)\s*$", prefix)
        if mq:
            name = mq.group(1) + name
        return (name, arity)

    # ordinary: last whitespace token is the qualified name
    tok = head.split(" ")[-1].lstrip("*&")
    return (tok, arity)


def param_types_key(text):
    """Return a normalized list of parameter TYPES (names + matcher comments
    stripped) used to disambiguate same-arity body-less overloads."""
    t = re.sub(r"\s+", " ", text).strip()
    span = _param_span(t)
    if span is None:
        return ()
    o, c = span
    inner = t[o + 1:c].strip()
    if not inner:
        return ()
    # split on top-level commas
    parts = []
    d = 0
    cur = ""
    for ch in inner:
        if ch in "<([":
            d += 1
        elif ch in ">)]":
            d -= 1
        if ch == "," and d == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += ch
    parts.append(cur)
    out = []
    for p in parts:
        # drop matcher comment /* ... */
        p = re.sub(r"/\*.*?\*/", "", p)
        p = re.sub(r"\s+", " ", p).strip()
        # drop trailing parameter name: the last identifier token if it is a
        # plain name (heuristic) - keep types with *, &, ::, <> intact
        toks = p.split(" ")
        if len(toks) >= 2 and re.fullmatch(r"[A-Za-z_]\w*", toks[-1]):
            # only strip if previous token looks like a complete type
            p = " ".join(toks[:-1])
        # normalize pointer/ref spacing
        p = p.replace(" *", "*").replace(" &", "&")
        out.append(p)
    return tuple(out)


class StructIndex:
    def __init__(self):
        # rva -> list of (side, header)  (rva from any row OR the addr itself)
        self.by_row = {}
        # (side, funcname) -> list of header
        self.by_name = {}
        self.files = {}  # relpath -> {side: blocks}

    def load_for(self, relpath):
        if relpath in self.files:
            return self.files[relpath]
        rp = Path(relpath)
        # A header included by multiple TUs is emitted by the parser as
        # <stem>.h plus <stem>_1.h, <stem>_2.h ... siblings (one per TU,
        # each with distinct rvas). Merge all variants so rva joins find a
        # match regardless of which TU file holds it.
        per = {}
        for side, base in STRUCT.items():
            d = base / rp.parent
            cands = []
            exact = base / relpath
            if exact.is_file():
                cands.append(exact)
            cands += sorted(d.glob(f"{rp.stem}_[0-9]*{rp.suffix}")) if d.is_dir() else []
            blocks = []
            seen = set()
            for p in cands:
                if p in seen:
                    continue
                seen.add(p)
                blocks += parse_structure_file(p)
            if blocks:
                per[side] = blocks
        self.files[relpath] = per
        return per


def find_carcasses(lines):
    """Yield carcass dicts in a source file's lines."""
    out = []
    i = 0
    while i < len(lines):
        if lines[i].strip() == HEADER_BARE:
            header_idx = i
            rows = []
            j = i + 1
            while j < len(lines):
                s = lines[j].strip()
                if s == "// ******":
                    break
                rm = ROW_RE.match(lines[j])
                if rm:
                    rows.append(rm.group(1))
                j += 1
            # enclosing function signature
            sig = find_struct_signature(lines, i)
            out.append({
                "idx": header_idx,
                "indent": lines[i][:len(lines[i]) - len(lines[i].lstrip())],
                "rows": rows,
                "first_row": rows[0] if rows else None,
                "sig": sig,
            })
            i = j
        i += 1
    return out


def main():
    idx = StructIndex()
    migrated = 0
    skipped = []
    files_touched = []
    samples = []
    join_counts = {}
    join_log = []

    src_files = sorted(list(SRC_ROOT.glob("vostok/**/*.cpp")) + list(SRC_ROOT.glob("vostok/**/*.h")))
    for sf in src_files:
        text = sf.read_text(encoding="utf-8", errors="replace")
        lines = text.split("\n")
        carcasses = find_carcasses(lines)
        if not carcasses:
            continue
        relpath = str(sf.relative_to(ROOT))
        per = idx.load_for(relpath)
        if not per:
            for c in carcasses:
                skipped.append((relpath, c["idx"] + 1, "no structure file at path"))
            continue

        # First pass: determine side for the file from row-bearing carcasses
        side_votes = {"base": 0, "target": 0}
        # build row-rva index per side for this file
        row_index = {}  # side -> {rva: block}
        hdr_index = {}  # side -> {header_addr: block}
        name_index = {}  # side -> {funcname: [blocks]}
        for side, blocks in per.items():
            ri = {}
            hi = {}
            ni = {}  # name -> {arity: [blocks]}
            for b in blocks:
                hi.setdefault(b["addr"], b)
                for r in b["rows"]:
                    ri.setdefault(r, b)
                if b["sig"]:
                    nm, ar = func_name_key(b["sig"])
                    if nm:
                        ni.setdefault(nm, {}).setdefault(ar, []).append(b)
            row_index[side] = ri
            hdr_index[side] = hi
            name_index[side] = ni

        for c in carcasses:
            if c["first_row"]:
                for side in ("base", "target"):
                    if (c["first_row"] in row_index.get(side, {})
                            or c["first_row"] in hdr_index.get(side, {})):
                        side_votes[side] += 1
                        break

        file_side = None
        if side_votes["target"] and not side_votes["base"]:
            file_side = "target"
        elif side_votes["base"] and not side_votes["target"]:
            file_side = "base"
        elif side_votes["target"] >= side_votes["base"] and side_votes["target"]:
            file_side = "target"
        elif side_votes["base"]:
            file_side = "base"

        def name_join(c):
            """Join a carcass to a structure block by its enclosing function
            name (+ arity, + param types). Returns (block, side, reason).
            block is None on failure; reason is "AMBIG:..." or "NOTFOUND:..."."""
            if not c["sig"]:
                return None, None, "no enclosing signature"
            nm, ar = func_name_key(c["sig"])
            if not nm:
                return None, None, "no enclosing signature"
            # try the qualified name, then the unqualified leaf (structure
            # sometimes prints the name without its namespace qualifier).
            name_variants = [nm]
            if "::" in nm:
                name_variants.append(nm.rsplit("::", 1)[1])
            order = [file_side] if file_side else []
            order += [s for s in ("target", "base") if s != file_side]
            for variant in name_variants:
                for side in order:
                    if side is None:
                        continue
                    by_arity = name_index.get(side, {}).get(variant)
                    if not by_arity:
                        continue
                    bl = by_arity.get(ar)
                    if not bl:
                        allb = [b for lst in by_arity.values() for b in lst]
                        if len(allb) == 1:
                            bl = allb
                    if bl and len(bl) > 1:
                        want = param_types_key(c["sig"])
                        typed = [b for b in bl
                                 if param_types_key(b["sig"] or "") == want]
                        if len(typed) == 1:
                            bl = typed
                    if bl and len(bl) == 1:
                        return bl[0], side, None
                    if bl and len(bl) > 1:
                        return None, side, f"AMBIG:'{variant}' arity={ar} ({len(bl)} on {side})"
            return None, None, f"NOTFOUND:'{nm}' arity={ar}"

        # Second pass: assign headers
        edits = []  # (lineidx, newheader)
        for c in carcasses:
            header_new = None
            join_kind = None
            if c["first_row"]:
                # join by first row rva: prefer a structure ROW rva match,
                # fall back to the structure's FUNCTION BODY header address
                # (the source carcass often carries an extra first row at the
                # function start, offset 0x000, whose rva == header addr).
                for side in ("base", "target"):
                    b = row_index.get(side, {}).get(c["first_row"])
                    if b is not None:
                        header_new = b["header"]
                        join_kind = "rva"
                        break
                if header_new is None:
                    for side in ("base", "target"):
                        b = hdr_index.get(side, {}).get(c["first_row"])
                        if b is not None:
                            header_new = b["header"]
                            join_kind = "hdr-addr"
                            break
                if header_new is None:
                    # rva absent from regenerated structure (e.g. atexit
                    # destructors whose address shifted); fall back to a
                    # unique name join before giving up.
                    b, side, reason = name_join(c)
                    if b is not None:
                        header_new = b["header"]
                        join_kind = "name-fallback"
                    else:
                        skipped.append((relpath, c["idx"] + 1,
                                        f"first-row rva {c['first_row']} not in structure; "
                                        f"name fallback: {reason}"))
                        continue
            else:
                # body-less: join by function name
                b, side, reason = name_join(c)
                if b is None:
                    skipped.append((relpath, c["idx"] + 1, f"body-less, {reason}"))
                    continue
                header_new = b["header"]
                join_kind = "name"

            join_counts[join_kind] = join_counts.get(join_kind, 0) + 1
            if join_kind in ("name", "name-fallback", "hdr-addr"):
                join_log.append((relpath, c["idx"] + 1, join_kind,
                                 header_new.strip(), c.get("sig")))

            new_line = c["indent"] + header_new.strip()
            old_line = lines[c["idx"]]
            if new_line != old_line:
                edits.append((c["idx"], new_line))
                if len(samples) < 8 and c["first_row"]:
                    samples.append((relpath, old_line.strip(), new_line.strip()))

        if edits:
            if not DRY_RUN:
                for li, nl in edits:
                    lines[li] = nl
                sf.write_text("\n".join(lines), encoding="utf-8")
            migrated += len(edits)
            files_touched.append((relpath, len(edits)))

    # Report
    print(f"MIGRATED HEADERS: {migrated}")
    print(f"FILES TOUCHED: {len(files_touched)}")
    print(f"SKIPPED: {len(skipped)}")
    for s in skipped:
        print("  SKIP", s)
    print("\nJOIN KINDS:", join_counts)
    print("\nNON-RVA JOINS (verify these):")
    for relp, ln, kind, hdr, sig in join_log:
        print(f"  [{kind}] {relp}:{ln} -> {hdr}")
        if sig:
            print(f"        sig: {sig}")
    print("\nSAMPLES (relpath | before | after):")
    for relp, b, a in samples:
        print(f"  {relp}\n    - {b}\n    + {a}")


if __name__ == "__main__":
    main()
