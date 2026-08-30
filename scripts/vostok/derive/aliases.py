"""vostok.derive.aliases - pairing two sides that spell a function differently.

Three ways the obvious {mangled: mangled} join fails, all handled here:

  * dynamic initializers and atexit destructors, which the retail PDB spells as
    a readable backtick name and MSVC 8 emits as `??__E`/`??__F` - matched on a
    canonical owner, and only when the owners are compatible;
  * ICF fold groups whose two linkers picked different representative names -
    admitted only on strict evidence (identical alias clusters, or an exact
    instruction-stream identity), never on a name that merely looks similar;
  * reviewed identities for asymmetric fold groups, in
    `config/exact_fold_aliases.tsv`.

The bar is deliberately high: a wrong alias invents a match that is not there.
"""

import re
from pathlib import Path

from vostok.core import tsv
from vostok.core.paths import EXACT_FOLD_ALIASES
from vostok.derive.names import pdb_signature_canon
from vostok.derive.scores import island_report_score


_DYN_RE = re.compile(r"^(.*?)`dynamic (initializer|atexit destructor) for '(.*)''$")


_IDENT_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


_LOCAL_SCOPE_RE = re.compile(r"::`\d+'::")


_LOCAL_FUNCTION_SCOPE_RE = re.compile(r"^`([^'\r\n]+)'::`\d+'::")


_LOCAL_STATIC_SYMBOL_RE = re.compile(r"^\$?S\d+$")


def local_scope_canon(name):
    """Erase only MSVC's unstable numeric local-scope ordinal.

    The enclosing function, local type/member signature, source owner, and
    instruction stream remain available to the strict alias matcher.
    """
    return _LOCAL_SCOPE_RE.sub("::`#'::", name)


def rich_name_canon(name):
    """Canonicalize the two non-semantic rich-PDB spelling differences."""
    return pdb_signature_canon(local_scope_canon(name))


# ThunkFuncN<Class, mid, ...>::Method template statics: their initializers
# exist per-method (GFx AS3 emits ~1,500 under the retail PMF pragma), and the
# two sides spell them differently (rich backtick vs raw ??__E template
# mangling). Both reduce to a synthetic string identity - "?TF<n>:<class>:<mid>"
# - that no real fqn can collide with. funcN + class + mid is unique: each AS3
# method has one mid ordinal and one thunk instantiation.
_TF_RICH_RE = re.compile(r"^(?:[A-Za-z_][A-Za-z0-9_:]*::)?ThunkFunc(\d+)<(.+)>::Method$")
_TF_BASE_RE = re.compile(
    r"^\?\?__([EF])\?Method@\?\$ThunkFunc(\d+)@"
    r"V([A-Za-z_][A-Za-z0-9_]*(?:@[A-Za-z_][A-Za-z0-9_]*)*)@@"
    r"\$0([0-9]|[A-P]+@)"
)
# Nested-scope statics (InstanceTraits::fl::X::ti and friends): plain
# identifiers only - templated scopes stay deferred.
_NESTED_STATIC_RE = re.compile(
    r"^\?\?__([EF])\?([A-Za-z_][A-Za-z0-9_]*)@"
    r"((?:[A-Za-z_][A-Za-z0-9_]*@)+)@"
)


def _mangled_int(tok):
    """Decode MSVC's template-int encoding after `$0`.

    A single digit d encodes d+1; hex letters A-P terminated by `@` encode 0
    and values above 10 (A=0 .. P=15 per nibble).
    """
    if tok.isdigit():
        return int(tok) + 1
    value = 0
    for ch in tok[:-1]:  # strip the '@' terminator
        value = value * 16 + (ord(ch) - ord("A"))
    return value


def _template_args_toplevel(args):
    """Split template argument text on depth-0 commas."""
    parts, depth, cur = [], 0, []
    for ch in args:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append("".join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    parts.append("".join(cur).strip())
    return parts


def _thunkfunc_canon_rich(fqn):
    m = _TF_RICH_RE.match(fqn)
    if not m:
        return None
    args = _template_args_toplevel(m.group(2))
    if len(args) < 2 or not args[1].isdigit():
        return None
    return f"?TF{m.group(1)}:{args[0]}:{args[1]}"


def dyn_canon_rich(mangled):
    """Canonical identity for a rich-index dynamic-init thunk."""
    m = _DYN_RE.match(mangled)
    if not m:
        return None
    pfx, kind, inner = m.group(1), m.group(2), m.group(3)
    kc = "E" if kind == "initializer" else "F"
    # pfx is the namespace/local scope, or empty when it sits inside the quotes.
    if pfx:
        if not pfx.endswith("::"):
            return None
        fqn = pfx + inner
    else:
        fqn = inner
    if not fqn or "\n" in fqn or "\r" in fqn:
        return None
    tf = _thunkfunc_canon_rich(fqn)
    if tf:
        return (kc, tf)
    return (kc, fqn)


def dyn_canon_base(mangled):
    """Canonical identity for either rich spelling or raw ??__E/??__F form."""
    rich = dyn_canon_rich(mangled)
    if rich:
        return rich
    m = _TF_BASE_RE.match(mangled)
    if m:
        scopes = m.group(3).split("@")
        fqn = "::".join(reversed(scopes))
        return (m.group(1), f"?TF{m.group(2)}:{fqn}:{_mangled_int(m.group(4))}")
    m = _NESTED_STATIC_RE.match(mangled)
    if m and mangled.endswith("@@YAXXZ"):
        scopes = [s for s in m.group(3).split("@") if s]
        fqn = "::".join(list(reversed(scopes)) + [m.group(2)])
        return (m.group(1), fqn)
    if mangled.startswith("??__E"):
        kc = "E"
    elif mangled.startswith("??__F"):
        kc = "F"
    else:
        return None
    body = mangled[5:]
    if not body.endswith("@@YAXXZ"):
        return None
    inner = body[: -len("@@YAXXZ")]
    if "?" in inner or "$" in inner:  # local/anon/template scope - defer
        return None
    parts = [p for p in inner.split("@") if p]
    if not parts:
        return None
    var, scopes = parts[0], list(reversed(parts[1:]))  # mangled scopes are inner-first
    if not _IDENT_RE.match(var) or any(not _IDENT_RE.match(s) for s in scopes):
        return None
    return (kc, "::".join(scopes + [var]))


def dyn_owner_compatible(target_rec, base_rec, canon):
    """Whether source attribution can disambiguate a dynamic thunk pair.

    Compiler-generated local-static destructors can have no base-side source
    record even when the target PDB assigns them to an unrelated inline header.
    A unique canonical local scope is sufficient even when both PDBs report
    different owners: the enclosing function and compiler local-scope ordinal
    disambiguate it. Global/static names with two known owners still require an
    exact owner match.
    """
    target_file = target_rec.get("file")
    base_file = base_rec.get("file")
    return (
        not target_file
        or not base_file
        or target_file == base_file
        or bool(_LOCAL_SCOPE_RE.search(canon[1]))
    )


def matching_dynamic_initializer_owners(target_records, base_records):
    """Global statics whose initializer proves the source owner on both sides.

    An ICF-folded atexit destructor can inherit an unrelated header as its base
    PDB owner.  Its companion initializer is not folded with the destructor and
    retains the defining source file, so one unique agreeing initializer owner
    is strict evidence for the destructor's canonical variable identity.
    """
    def owners(records, canonicalize):
        result = {}
        for rec in records:
            canon = canonicalize(rec["mangled"])
            owner = rec.get("file")
            if canon and canon[0] == "E" and owner:
                result.setdefault(canon[1], set()).add(owner)
        return result

    target_owners = owners(target_records, dyn_canon_rich)
    base_owners = owners(base_records, dyn_canon_base)
    result = set()
    for identity in target_owners.keys() & base_owners.keys():
        target_files = target_owners[identity]
        base_files = base_owners[identity]
        if len(target_files) == 1 and target_files == base_files:
            result.add((identity, next(iter(target_files))))
    return result


def dynamic_pair_score(
    target_mangled,
    base_mangled,
    target_rec,
    base_rec,
    scores,
    symbol_alias_equivalent=None,
):
    """Use report score, or strict rich-stream exact evidence when unscored.

    MSVC's local-static bookkeeping symbols are ordinal names (``$S3``,
    ``S4``), not stable identities.  A unique canonical dynamic thunk may
    therefore compare those operands by kind while ordinary relocations still
    require the rich-PDB alias proof supplied by the pairing pass.
    """
    fuzzy = scores.get(target_mangled, scores.get(base_mangled))

    def dynamic_alias(target_operand, base_operand):
        if (
            _LOCAL_STATIC_SYMBOL_RE.fullmatch(target_operand)
            and _LOCAL_STATIC_SYMBOL_RE.fullmatch(base_operand)
        ):
            return True
        return bool(
            symbol_alias_equivalent
            and symbol_alias_equivalent(target_operand, base_operand)
        )

    if fuzzy is None and instruction_stream_exact(
        target_rec,
        base_rec,
        dynamic_alias,
        # A canon-unique initializer stores through image-layout absolutes
        # (the +4 word of an 8-byte PMF has no relocation symbol to render);
        # those differ by construction, exactly like the $S ordinals above.
        normalize_absolute_data=True,
    ):
        return 100.0
    return fuzzy


_ABSOLUTE_DATA_RE = re.compile(r"\[[0-9A-Fa-f]+h\]")


def instruction_stream_exact(
    target_rec,
    base_rec,
    symbol_alias_equivalent=None,
    normalize_absolute_data=False,
):
    """Prove exact code when objdiff omitted a function score.

    The rich-index producer has already normalized branch labels and relocation
    operands to symbolic instruction text. Equal size plus an identical,
    non-empty ordered instruction stream is therefore strict function-scoped
    exact evidence. The optional callback may prove that otherwise identical
    relocation operands merely use different ICF representative names. An
    absent instruction stream is never evidence.
    """
    if target_rec is None or base_rec is None:
        return False
    target_instructions = target_rec.get("instructions") or []
    base_instructions = base_rec.get("instructions") or []
    if not target_instructions or target_rec.get("size") != base_rec.get("size"):
        return False

    def normalized_text(text):
        if normalize_absolute_data:
            text = _ABSOLUTE_DATA_RE.sub("[imgaddr]", text)
        parts = text.split(None, 1)
        if len(parts) == 2 and parts[0] in {"call", "jmp"}:
            operand = {
                "operator delete": "??3@YAXPAX@Z",
            }.get(parts[1], parts[1])
            operand = re.sub(r"\s+>", ">", operand)
            return f"{parts[0]} {operand}"
        return text

    def identity(instructions):
        return [
            (ins.get("off"), ins.get("len"), normalized_text(ins.get("text", "")))
            for ins in instructions
        ]

    target_identity = identity(target_instructions)
    base_identity = identity(base_instructions)
    if target_identity == base_identity:
        return True
    if symbol_alias_equivalent is None or len(target_identity) != len(base_identity):
        return False

    def trailing_symbol_operand(text):
        """Split the operand position that can carry a rich-PDB function name.

        Besides direct ``call``/``jmp`` operands, MSVC emits function addresses
        as relocations in instructions such as ``mov ecx, <function>``.  Keep
        the entire opcode/destination prefix in the identity; only the trailing
        symbolic operand is eligible for ICF-alias reconciliation.
        """
        parts = text.split(None, 1)
        if len(parts) != 2:
            return None
        mnemonic, operands = parts
        depths = {"<": 0, "[": 0, "(": 0}
        closers = {">": "<", "]": "[", ")": "("}
        commas = []
        for index, character in enumerate(operands):
            if character in depths:
                depths[character] += 1
            elif character in closers:
                opener = closers[character]
                depths[opener] = max(0, depths[opener] - 1)
            elif character == "," and not any(depths.values()):
                commas.append(index)
        if not commas:
            return mnemonic, operands.strip()
        split = commas[-1]
        prefix, operand = operands[:split], operands[split + 1:]
        return f"{mnemonic} {prefix.strip()},", operand.strip()

    for target, base in zip(target_identity, base_identity):
        if target == base:
            continue
        if target[:2] != base[:2]:
            return False
        target_parts = trailing_symbol_operand(target[2])
        base_parts = trailing_symbol_operand(base[2])
        if (
            target_parts is None
            or base_parts is None
            or target_parts[0] != base_parts[0]
            or not symbol_alias_equivalent(target_parts[1], base_parts[1])
        ):
            return False
    return True


def island_candidate_score(expected, mangled, scores, target_rec, candidate_rec):
    """Prefer strict function-scoped rich exactness over a stale report pair."""
    if instruction_stream_exact(target_rec, candidate_rec):
        return 100.0
    return island_report_score(expected, mangled, scores)


def load_exact_fold_aliases(path=EXACT_FOLD_ALIASES):
    """Load reviewed target-to-base identities for asymmetric ICF groups."""
    aliases = {}
    if not Path(path).is_file():
        return aliases
    for line_number, fields in tsv.read(path):
        if len(fields) != 2 or not all(fields):
            raise RuntimeError(
                f"{path}:{line_number}: expected <target>\\t<base alias>"
            )
        target, base = fields
        aliases[target] = base
    return aliases


def shared_icf_alias_owner_compatible(
    target_rec, base_rec, target_alias_names_by_rva, base_alias_names_by_rva
):
    """Prove an owner-file mismatch is only a shared ICF alias choice.

    A single matching demangled spelling is insufficient because unrelated
    internal helpers can have the same readable name in different TUs. When
    both RVAs expose the same two-or-more-name alias cluster, however, the PDBs
    identify the same folded body even if each linker selected a different
    source record as its owner.
    """
    if not target_alias_names_by_rva or not base_alias_names_by_rva:
        return False
    target_names = target_alias_names_by_rva.get(target_rec["rva"], set())
    base_names = base_alias_names_by_rva.get(base_rec["rva"], set())
    return (
        target_names == base_names
        and target_rec["name"] in target_names
        and len(target_names) >= 2
    )


def strict_source_alias_candidates(
    target_rec,
    base_aliases_by_name,
    used_base_rvas,
    allow_used=False,
    target_alias_names_by_rva=None,
    base_alias_names_by_rva=None,
    exact_fold_aliases=None,
    base_aliases_by_mangled=None,
    symbol_alias_equivalent=None,
):
    """Find exact same-source bodies hidden behind a different ICF name.

    A folded RVA may inherit another function's mangled identity independently
    on each side.  The PDB still records every alias's demangled signature and
    source owner, so a unique same-name, byte-exact rich record is sufficient
    when it either has the same source file or both RVAs expose the same
    multi-name ICF alias cluster. The latter proves differing owner files came
    from linker alias selection, not a same-named internal helper in another
    TU.
    """
    candidate_records = dict(base_aliases_by_name.get(target_rec["name"], {}))
    canonical_name = rich_name_canon(target_rec["name"])
    if canonical_name != target_rec["name"]:
        candidate_records.update(base_aliases_by_name.get(canonical_name, {}))

    candidates = [
        rec
        for rva, rec in candidate_records.items()
        if (allow_used or rva not in used_base_rvas)
        and (
            rec["file"] == target_rec["file"]
            or shared_icf_alias_owner_compatible(
                target_rec,
                rec,
                target_alias_names_by_rva,
                base_alias_names_by_rva,
            )
        )
        and instruction_stream_exact(
            target_rec, rec, symbol_alias_equivalent
        )
    ]

    # A reviewed asymmetric ICF identity may bridge disjoint readable PDB
    # aliases. It still earns a pair only when the complete body is identical;
    # uniqueness is enforced by the caller.
    exact_fold_aliases = exact_fold_aliases or {}
    base_aliases_by_mangled = base_aliases_by_mangled or {}
    base_mangled = exact_fold_aliases.get(target_rec.get("mangled"))
    if base_mangled:
        seen_rvas = {rec["rva"] for rec in candidates}
        for rva, rec in base_aliases_by_mangled.get(base_mangled, {}).items():
            if rva in seen_rvas or (not allow_used and rva in used_base_rvas):
                continue
            if instruction_stream_exact(
                target_rec, rec, symbol_alias_equivalent
            ):
                candidates.append(rec)
                seen_rvas.add(rva)
    return candidates


def report_source_alias_candidates(
    target_rec,
    base_aliases_by_name,
    used_base_rvas,
    target_alias_names_by_rva=None,
    base_alias_names_by_rva=None,
    *,
    allow_used=False,
):
    """Find a unique owner-compatible alias for an objdiff-measured symbol.

    The report score already proves which target symbol was compared.  Unlike
    an unmeasured rich-only alias, its source statements and instructions may
    legitimately differ; requiring either to be exact would hide the mismatch
    that the report is meant to expose.
    """
    candidate_records = dict(base_aliases_by_name.get(target_rec["name"], {}))
    canonical_name = rich_name_canon(target_rec["name"])
    if canonical_name != target_rec["name"]:
        candidate_records.update(base_aliases_by_name.get(canonical_name, {}))
    return [
        rec
        for rva, rec in candidate_records.items()
        if (allow_used or rva not in used_base_rvas)
        and (
            rec["file"] == target_rec["file"]
            or shared_icf_alias_owner_compatible(
                target_rec,
                rec,
                target_alias_names_by_rva,
                base_alias_names_by_rva,
            )
        )
    ]


def exact_paired_source_alias(
    source_rec,
    candidate_aliases_by_name,
    paired_candidate_rvas,
    source_alias_names_by_rva=None,
    candidate_alias_names_by_rva=None,
):
    """Return one exact source alias already represented by a paired RVA.

    ICF may fold several target functions onto one RVA while the base keeps a
    separate body for every symbol (or vice versa).  The extra bodies are not
    unexplained source: a rich record with the same complete demangled name,
    compatible source ownership, and identical instructions proves that the
    paired side already represents the function.
    """
    candidates = strict_source_alias_candidates(
        source_rec,
        candidate_aliases_by_name,
        set(),
        allow_used=True,
        target_alias_names_by_rva=source_alias_names_by_rva,
        base_alias_names_by_rva=candidate_alias_names_by_rva,
    )
    candidates = [
        rec for rec in candidates if rec["rva"] in paired_candidate_rvas
    ]
    return candidates[0] if len(candidates) == 1 else None
