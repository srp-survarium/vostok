"""vostok.derive.aliases - pairing two sides that spell a function differently.

Three ways the obvious {mangled: mangled} join fails, all handled here:

  * dynamic initializers and atexit destructors, which the retail PDB spells as
    a readable backtick name and MSVC 8 emits as `??__E`/`??__F` - matched on a
    canonical owner, and only when the owners are compatible;
  * ICF fold groups whose two linkers picked different representative names -
    admitted only on strict evidence (identical alias clusters, or an exact
    instruction-stream identity), never on a name that merely looks similar;
  * reviewed identities for asymmetric fold groups, in
    `docs/binary_matching/exact_fold_aliases.tsv`.

The bar is deliberately high: a wrong alias invents a match that is not there.
"""

import re
from pathlib import Path

from vostok.core import tsv
from vostok.core.paths import EXACT_FOLD_ALIASES
from vostok.derive.scores import island_report_score


_DYN_RE = re.compile(r"^(.*?)`dynamic (initializer|atexit destructor) for '(.*)''$")


_IDENT_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


_LOCAL_SCOPE_RE = re.compile(r"::`\d+'::")


_LOCAL_FUNCTION_SCOPE_RE = re.compile(r"^`([^'\r\n]+)'::`\d+'::")


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
    return (kc, fqn)


def dyn_canon_base(mangled):
    """Canonical identity for either rich spelling or raw ??__E/??__F form."""
    rich = dyn_canon_rich(mangled)
    if rich:
        return rich
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


def dynamic_pair_score(target_mangled, base_mangled, target_rec, base_rec, scores):
    """Use report score, or strict rich-stream exact evidence when unscored."""
    fuzzy = scores.get(target_mangled, scores.get(base_mangled))
    if fuzzy is None and instruction_stream_exact(target_rec, base_rec):
        return 100.0
    return fuzzy


def instruction_stream_exact(target_rec, base_rec, call_alias_equivalent=None):
    """Prove exact code when objdiff omitted a function score.

    The rich-index producer has already normalized branch labels and relocation
    operands to symbolic instruction text. Equal size plus an identical,
    non-empty ordered instruction stream is therefore strict function-scoped
    exact evidence. The optional callback may prove that otherwise identical
    calls or jumps merely use different ICF representative names. An absent
    instruction stream is never evidence.
    """
    if target_rec is None or base_rec is None:
        return False
    target_instructions = target_rec.get("instructions") or []
    base_instructions = base_rec.get("instructions") or []
    if not target_instructions or target_rec.get("size") != base_rec.get("size"):
        return False

    def normalized_text(text):
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
    if call_alias_equivalent is None or len(target_identity) != len(base_identity):
        return False

    for target, base in zip(target_identity, base_identity):
        if target == base:
            continue
        if target[:2] != base[:2]:
            return False
        target_parts = target[2].split(None, 1)
        base_parts = base[2].split(None, 1)
        if (
            len(target_parts) != 2
            or len(base_parts) != 2
            or target_parts[0] != base_parts[0]
            or target_parts[0] not in {"call", "jmp"}
            or not call_alias_equivalent(target_parts[1], base_parts[1])
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
    candidates = [
        rec
        for rva, rec in base_aliases_by_name.get(target_rec["name"], {}).items()
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
        and instruction_stream_exact(target_rec, rec)
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
            if instruction_stream_exact(target_rec, rec):
                candidates.append(rec)
                seen_rvas.add(rva)
    return candidates


def report_source_alias_candidates(
    target_rec,
    base_aliases_by_name,
    used_base_rvas,
    target_alias_names_by_rva=None,
    base_alias_names_by_rva=None,
):
    """Find a unique owner-compatible alias for an objdiff-measured symbol.

    The report score already proves which target symbol was compared.  Unlike
    an unmeasured rich-only alias, its source statements and instructions may
    legitimately differ; requiring either to be exact would hide the mismatch
    that the report is meant to expose.
    """
    return [
        rec
        for rva, rec in base_aliases_by_name.get(target_rec["name"], {}).items()
        if rva not in used_base_rvas
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
