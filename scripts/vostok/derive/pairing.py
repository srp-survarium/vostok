"""vostok.derive.pairing - which base function IS this target function.

The obvious {mangled: mangled} join answers most of it and gets the rest wrong,
because two linkers spell one body several ways: ICF folds pick different
representative names, dynamic initializers appear as a readable backtick name on
one side and `??__E`/`??__F` on the other, and the retail PDB renders a handful
of template arguments differently. Each recovery is a separate pass with its own
evidence bar, applied in descending order of certainty:

    primary          identical mangled names and exact or alias-normalized code
    compiler alias   the exact name MSVC would emit, proven by a report score
    report alias     a folded PDB alias the delink report already compared
    strict rich      unique same-source alias with a byte-identical stream
    shared RVA       a second target name on an already-consumed ICF body
    dynamic thunk    ??__E/??__F <-> the backtick spelling, on a canonical owner
    PDB spelling     the audited target/candidate template-spelling map

Order matters: every pass consumes RVAs, and a later pass may only claim what is
still free. A wrong alias invents a match that is not there, so each pass
refuses on ambiguity rather than guessing.
"""

import re
from dataclasses import dataclass, field

from vostok.core import symbols as normalize_objdiff_symbols
from vostok.derive import log
from vostok.derive.aliases import (dyn_canon_base, dyn_canon_rich,
                                   dyn_owner_compatible, dynamic_pair_score,
                                   instruction_stream_exact,
                                   load_exact_fold_aliases,
                                   report_source_alias_candidates,
                                   strict_source_alias_candidates)
from vostok.derive.classify import classify
from vostok.derive.names import qualified_name


@dataclass(frozen=True)
class Pair:
    mangled: str          # the TARGET identity; the base side may spell it otherwise
    target_rva: int
    base_rva: int
    fuzzy: float | None
    cls: str              # MATCH | SIZE | QUANTITY | SPLIT
    t_stmts: int
    b_stmts: int
    n_size_rows: int
    n_trgt_only: int
    n_base_only: int


@dataclass
class Pairing:
    pairs: dict = field(default_factory=dict)          # {target mangled: Pair}
    paired: set = field(default_factory=set)           # every mangled, BOTH sides
    used_target_rvas: set = field(default_factory=set)
    used_base_rvas: set = field(default_factory=set)
    base_record_for: dict = field(default_factory=dict)  # cross-name pairs only
    # the same-RVA alias lookups the passes built; the base-only taxonomy asks
    # them the mirror question (is this base symbol an alias of a PAIRED body).
    target_aliases_by_name: dict = field(default_factory=dict)
    target_alias_names_by_rva: dict = field(default_factory=dict)
    base_alias_names_by_rva: dict = field(default_factory=dict)

    def base_record(self, mangled, artifacts):
        """The base rich record behind a pair, by name or (cross-name) by RVA."""
        return artifacts.base.get(mangled) or self.base_record_for.get(mangled)


def pair(artifacts):
    """Run every pass, in order, over one loaded artifact set."""
    pairer = _Pairer(artifacts)
    pairer.primary()
    pairer.compiler_aliases()
    pairer.report_aliases()
    pairer.strict_rich_aliases()
    pairer.shared_rva_aliases()
    pairer.dynamic_thunks()
    pairer.pdb_spelling_aliases()
    return pairer.finish()


class _Pairer:
    def __init__(self, artifacts):
        self.art = artifacts
        self.target, self.base = artifacts.target, artifacts.base
        self.pairs = {}
        self.primary_names = set(artifacts.target) & set(artifacts.base)
        self.cross_named = set()       # both sides' names, paired across a name gap
        self.used_target_rvas = set()
        self.used_base_rvas = set()

        # alias lookups every cross-name pass (and the base-only taxonomy) needs
        self.target_aliases_by_name = _by_name(artifacts.target_records)
        self.base_aliases_by_name = _by_name(artifacts.base_records)
        self.base_aliases_by_mangled = _by_mangled(artifacts.base_records)
        self.target_alias_names_by_rva = _names_by_rva(artifacts.target_records)
        self.base_alias_names_by_rva = _names_by_rva(artifacts.base_records)
        (
            self.target_call_rvas_by_operand,
            self.target_call_operands_by_rva,
        ) = _call_operand_aliases(artifacts.target_records)
        (
            self.base_call_rvas_by_operand,
            self.base_call_operands_by_rva,
        ) = _call_operand_aliases(artifacts.base_records)
        self.exact_fold_aliases = load_exact_fold_aliases()

    # -- the passes ---------------------------------------------------------

    def primary(self):
        """Identical mangled names. A function objdiff never scored but whose
        rich instruction streams are identical, including uniquely proven ICF
        call aliases, is exact by construction."""
        recovered = 0
        for mangled in sorted(self.primary_names):
            trec, brec = self.target[mangled], self.base[mangled]
            fuzzy = self.art.fuzzy.get(mangled)
            if fuzzy is None and instruction_stream_exact(
                trec, brec, self._call_alias_equivalent
            ):
                fuzzy = 100.0
                recovered += 1
            self._add(mangled, trec, brec, fuzzy)
        self.used_target_rvas = {p.target_rva for p in self.pairs.values()}
        self.used_base_rvas = {p.base_rva for p in self.pairs.values()}
        if recovered:
            log(
                f"function-scoped rich-index exact attribution recovered "
                f"{recovered} objdiff score gap(s)"
            )

    def compiler_aliases(self):
        """The base rich record carries the exact name MSVC emits for this retail
        spelling. The delink report proves the target name was compared; also
        require one free RVA on each side."""
        n = 0
        for tm in sorted(set(self.target) - self.primary_names):
            bm = self.art.compiler_alias(tm)
            if (
                bm not in self.base
                or tm not in self.art.fuzzy
                or self.target[tm]["rva"] in self.used_target_rvas
                or self.base[bm]["rva"] in self.used_base_rvas
            ):
                continue
            self._add_cross(tm, bm, self.target[tm], self.base[bm], self.art.fuzzy[tm])
            n += 1
        if n:
            log(f"cross-name paired {n} exact compiler aliases")

    def report_aliases(self):
        """A folded PDB alias whose base record carries another symbol's mangled
        name. Admitted on a report score plus a unique free candidate with an
        exact demangled signature and compatible source ownership."""
        n = 0
        for tm in sorted(set(self.target) - self.primary_names):
            if tm not in self.art.fuzzy:
                continue
            trec = self.target[tm]
            candidates = report_source_alias_candidates(
                trec,
                self.base_aliases_by_name,
                self.used_base_rvas,
                target_alias_names_by_rva=self.target_alias_names_by_rva,
                base_alias_names_by_rva=self.base_alias_names_by_rva,
            )
            if trec["rva"] in self.used_target_rvas or len(candidates) != 1:
                continue
            self._add_cross(tm, candidates[0]["mangled"], trec, candidates[0],
                            self.art.fuzzy[tm])
            n += 1
        if n:
            log(f"cross-name paired {n} report-grounded folded PDB aliases")

    def strict_rich_aliases(self):
        """Highly-COMDAT functions objdiff never reaches, because the two sides
        chose different canonical names for one folded RVA. Unlike the
        report-grounded pass this also covers target-only score gaps, so the bar
        is a unique same-source alias with an identical instruction stream."""
        n = 0
        for tm, trec in self._unclaimed_targets():
            candidates = self._strict_candidates(trec, allow_used=False)
            if len(candidates) != 1:
                continue
            self._add_cross(tm, candidates[0]["mangled"], trec, candidates[0], 100.0)
            n += 1
        if n:
            log(f"cross-name paired {n} strict same-source rich aliases")

    def shared_rva_aliases(self):
        """One ICF-selected body can legitimately own several PDB aliases. When
        the base RVA is already consumed, retain a second target pairing only
        when the base PDB still records this exact demangled signature in the
        same source file with a byte-exact stream - a genuine header island, not
        an arbitrary same-body fold."""
        n = 0
        for tm, trec in self._unclaimed_targets():
            candidates = self._strict_candidates(trec, allow_used=True)
            if len(candidates) != 1 or candidates[0]["rva"] not in self.used_base_rvas:
                continue
            brec = candidates[0]
            self._add_cross(tm, brec["mangled"], trec, brec, 100.0, claim_base=False)
            n += 1
        if n:
            log(f"cross-name paired {n} shared-RVA same-source rich aliases")

    def dynamic_thunks(self):
        """Dynamic-init/atexit thunks across their several spellings. The
        canonical identity must be unique on each side; emitted size may
        legitimately differ, which is exactly what the fuzzy matcher measures."""
        t_canon, b_canon = {}, {}     # (kind, fqn) -> [mangled, ...]
        for mangled in set(self.target) - self.primary_names:
            canon = dyn_canon_rich(mangled)
            if canon:
                t_canon.setdefault(canon, []).append(mangled)
        for mangled in set(self.base) - self.primary_names:
            canon = dyn_canon_base(mangled)
            if canon:
                b_canon.setdefault(canon, []).append(mangled)
        n = 0
        for canon in t_canon.keys() & b_canon.keys():
            tm_list, bm_list = t_canon[canon], b_canon[canon]
            if len(tm_list) != 1 or len(bm_list) != 1:
                continue  # ambiguous - leave for the Rust-side demangler
            tm, bm = tm_list[0], bm_list[0]
            trec, brec = self.target[tm], self.base[bm]
            if trec["rva"] in self.used_target_rvas or brec["rva"] in self.used_base_rvas:
                continue
            if not dyn_owner_compatible(trec, brec, canon):
                continue  # same-named statics in different known owners are not proven identical
            self._add_cross(
                tm, bm, trec, brec,
                dynamic_pair_score(tm, bm, trec, brec, self.art.fuzzy),
            )
            n += 1
        if n:
            log(f"cross-name paired {n} dynamic-init/atexit thunks (??__E/??__F <-> demangled)")

    def pdb_spelling_aliases(self):
        """The retail and candidate PDBs render a small audited set of template
        type spellings differently (an explicit ``enum`` keyword, say). The
        disposable target COFF normalizer gives objdiff the candidate-side name;
        pair the unchanged rich identities through that exact same map."""
        n = 0
        for tm in sorted(set(self.target) - self.primary_names - self.cross_named):
            bm = normalize_objdiff_symbols.pdb_alias_name(tm)
            if not bm or bm not in self.base:
                continue
            if bm in self.primary_names or bm in self.cross_named:
                continue
            trec, brec = self.target[tm], self.base[bm]
            if trec["file"] != brec["file"]:
                continue
            self._add_cross(
                tm, bm, trec, brec,
                self.art.fuzzy.get(tm, self.art.fuzzy.get(bm)),
                claim_rvas=False,
            )
            n += 1
        if n:
            log(f"cross-name paired {n} exact PDB spelling aliases")

    # -- shared machinery ---------------------------------------------------

    def _unclaimed_targets(self):
        """Target names no pass has claimed yet, by name or by RVA."""
        for tm in sorted(set(self.target) - self.primary_names - self.cross_named):
            trec = self.target[tm]
            if trec["rva"] not in self.used_target_rvas:
                yield tm, trec

    def _strict_candidates(self, trec, *, allow_used):
        return strict_source_alias_candidates(
            trec,
            self.base_aliases_by_name,
            self.used_base_rvas,
            allow_used=allow_used,
            target_alias_names_by_rva=self.target_alias_names_by_rva,
            base_alias_names_by_rva=self.base_alias_names_by_rva,
            exact_fold_aliases=self.exact_fold_aliases,
            base_aliases_by_mangled=self.base_aliases_by_mangled,
        )

    def _call_alias_equivalent(self, target_operand, base_operand):
        """Whether two printed call targets are PDB aliases of one body.

        The linker may print a different representative name for the same ICF
        group on each side. An overlapping alias label at both called RVAs is
        strict PDB evidence that only the representative spelling changed.
        """
        target_rvas = self.target_call_rvas_by_operand.get(target_operand, set())
        base_rvas = self.base_call_rvas_by_operand.get(base_operand, set())
        if len(target_rvas) != 1 or len(base_rvas) != 1:
            return False
        target_rva = next(iter(target_rvas))
        base_rva = next(iter(base_rvas))
        return bool(
            self.target_call_operands_by_rva[target_rva]
            & self.base_call_operands_by_rva[base_rva]
        )

    def _add(self, mangled, trec, brec, fuzzy):
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(trec, brec)
        self.pairs[mangled] = Pair(
            mangled, trec["rva"], brec["rva"], fuzzy, cls,
            t_n, b_n, n_size, n_tonly, n_bonly,
        )

    def _add_cross(self, tm, bm, trec, brec, fuzzy, *, claim_rvas=True, claim_base=True):
        self._add(tm, trec, brec, fuzzy)
        self.cross_named.update((tm, bm))
        if claim_rvas:
            self.used_target_rvas.add(trec["rva"])
            if claim_base:
                self.used_base_rvas.add(brec["rva"])

    def finish(self):
        return Pairing(
            pairs=self.pairs,
            # cross-name pairs are keyed by the TARGET name, so the base twin has
            # to be named here too - otherwise it is re-classified as base-only
            # and double-counted.
            paired=self.primary_names | self.cross_named,
            used_target_rvas=self.used_target_rvas,
            used_base_rvas=self.used_base_rvas,
            base_record_for=self._base_records_by_name(),
            target_aliases_by_name=self.target_aliases_by_name,
            target_alias_names_by_rva=self.target_alias_names_by_rva,
            base_alias_names_by_rva=self.base_alias_names_by_rva,
        )

    def _base_records_by_name(self):
        """For a cross-name pair the target mangled is absent from `base`; keep
        the matched record, found by base RVA, so MAX can hash its source."""
        by_rva = {rec["rva"]: rec for rec in self.base.values()}
        return {
            mangled: by_rva.get(p.base_rva)
            for mangled, p in self.pairs.items()
            if mangled not in self.base
        }


def _by_name(records):
    out = {}
    for rec in records:
        out.setdefault(rec["name"], {})[rec["rva"]] = rec
    return out


def _by_mangled(records):
    out = {}
    for rec in records:
        out.setdefault(rec["mangled"], {})[rec["rva"]] = rec
    return out


def _names_by_rva(records):
    out = {}
    for rec in records:
        out.setdefault(rec["rva"], set()).add(rec["name"])
    return out


def _call_operand_aliases(records):
    """Index the operand spelling each demangled PDB alias contributes."""
    rvas_by_operand = {}
    operands_by_rva = {}
    for rec in records:
        qualified = qualified_name(rec.get("name", ""))
        if qualified is None:
            continue
        owner, leaf = qualified
        operand = f"{owner}::{leaf}" if owner else leaf
        operand = re.sub(r"\s+>", ">", operand)
        rvas_by_operand.setdefault(operand, set()).add(rec["rva"])
        operands_by_rva.setdefault(rec["rva"], set()).add(operand)
    return rvas_by_operand, operands_by_rva
