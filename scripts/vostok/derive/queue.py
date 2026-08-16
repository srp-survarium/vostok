"""vostok.derive.queue - what to dispatch next.

One batch per TU, because a matcher owns a whole translation unit: its small
helpers then get matched in their real inlining context instead of as cross-TU
churn. Frameless (LTCG-customised) leaves are skipped - they have no stable
frame to match against and only ever pair inlined into a caller. Tried work is
demoted rather than dropped, so a hard TU comes back after everything else.
"""

import json

from vostok.core.paths import SOURCES
from vostok.derive.db import open_db, staleness_check
from vostok.derive.names import norm_name, qualified_name


def queue_host_units(con, module):
    """Header pseudo-units are a delink artifact (the PDB attributes inline
    methods to the header), not real TUs - fold their functions into a HOST
    .cpp TU: stem match first (x.h / x_inline.h -> sources/x.cpp), then the
    .cpp unit owning most functions of the same class; genuinely header-only
    classes stay standalone, labeled."""
    cpps = {
        r["name"]
        for r in con.execute(
            "SELECT name FROM units WHERE module = ? AND name LIKE '%.cpp'", (module,)
        )
    }

    def stem(path):
        b = path.rsplit("/", 1)[-1].rsplit(".", 1)[0]
        return b[:-7] if b.endswith("_inline") else b

    stem_to_cpp = {}
    for c in sorted(cpps):
        stem_to_cpp.setdefault(stem(c), c)
    class_owner = {}
    q = """SELECT u.name AS unit, s.demangled FROM target_functions t
           JOIN symbols s ON s.id = t.sym JOIN units u ON u.id = t.unit
           WHERE t.module = ? AND u.name LIKE '%.cpp'"""
    for r in con.execute(q, (module,)):
        qn = qualified_name(r["demangled"] or "")
        if qn and qn[0]:
            counts = class_owner.setdefault(norm_name(qn[0]), {})
            counts[r["unit"]] = counts.get(r["unit"], 0) + 1

    def host(unit, demangled):
        if unit.endswith(".cpp"):
            return unit
        c = stem_to_cpp.get(stem(unit))
        if c:
            return c
        qn = qualified_name(demangled or "")
        if qn and qn[0]:
            counts = class_owner.get(norm_name(qn[0]))
            if counts:
                return max(sorted(counts), key=lambda u: counts[u])
        return f"{unit} (header-only)"

    return host


def cmd_queue(args):
    """One batch per TU: a matcher owns the WHOLE TU, so small helpers are
    matched in their real context (same inlining/LTCG environment as their
    callers) instead of as cross-TU small-function churn. Header pseudo-units
    fold into their host .cpp TU; frameless (LTCG-customized) leaves are
    skipped. TUs are ordered LOWEST match level first (real unmatched code
    before 99% polish - that tail is verifier work), size breaking ties."""
    con = open_db(check_schema=True)
    staleness_check(con, strict=not args.stale_ok)
    where, params = ["module = ?"], [args.module]
    if not args.include_frameless:
        where.append("frameless = 0")
    q = f"""
      WITH cand AS (
        SELECT s.demangled, s.mangled, coalesce(u.name,'(no unit)') AS unit,
               t.module, t.size, t.frameless, p.fuzzy_pct, p.struct_class,
               h.best_fuzzy_pct AS best,
               coalesce(a.n, 0) AS attempts,
               fl.path AS def_file,
               bos.mangled AS near_miss_base,
               CASE WHEN p.sym IS NULL AND bos.mangled IS NOT NULL THEN 'NEAR_MISS'
                    WHEN p.sym IS NULL THEN 'TARGET_ONLY'
                    ELSE 'PAIRED' END AS presence
        FROM target_functions t
        JOIN symbols s ON s.id = t.sym
        LEFT JOIN units u ON u.id = t.unit
        LEFT JOIN pairs p ON p.sym = t.sym
        LEFT JOIN history h ON h.mangled = s.mangled
        LEFT JOIN attempts a ON a.mangled = s.mangled
        LEFT JOIN files fl ON fl.id = t.file
        -- a base-only NEAR_MISS row's detail IS this target symbol: the body
        -- exists under a different mangling (access/const) - a header fix
        LEFT JOIN base_only_status bos
               ON bos.detail = s.mangled AND bos.status = 'NEAR_MISS'
        -- skip near-ceiling fns whose STRUCTURE already matches (MATCH/SIZE/SPLIT all
        -- have the target's statement count; the residual is non-steerable LTCG -
        -- inline-vs-call / reg-alloc). Don't hand an effectively-done fn to a matcher.
        -- Keyed on MAX (best-ever %, not current): an LTO drop below the peak is itself
        -- non-steerable, so a fn that ONCE reached >=95 structure-matched stays skipped -
        -- UNLESS its source was edited, which resets best to current (refresh), re-queuing
        -- it. QUANTITY (wrong statement count, incl. the high-%-over-wrong-structure trap)
        -- STAYS at any %, as do all fns whose max is below 95%.
        WHERE NOT (coalesce(h.best_fuzzy_pct, p.fuzzy_pct, 0) >= 95 AND coalesce(p.struct_class, '') != 'QUANTITY')
          AND s.mangled NOT IN (SELECT mangled FROM flags WHERE flag IN ('SKIP','OUT_OF_SCOPE'))
          -- compiler-generated machinery is not source-steerable standalone:
          -- deleting dtors / dynamic initializers (backtick names), thunks,
          -- anonymous-namespace symbols
          AND s.demangled NOT LIKE '%`%'
          AND s.demangled NOT LIKE '[thunk]%'
          AND s.mangled NOT LIKE '%?A0x%'
          -- seen-before, vanished without a source touch: external inline/link
          -- decision, out of scope (design: history does the classifying)
          AND NOT (p.sym IS NULL AND h.mangled IS NOT NULL)
          -- matched at 100 before and untouched since: a later regression is
          -- outside this function (LTCG non-steerable) - skip
          AND NOT (coalesce(h.best_fuzzy_pct, 0) >= 100 AND coalesce(p.fuzzy_pct, 0) < 100)
      )
      SELECT * FROM cand WHERE {" AND ".join(where)}
      ORDER BY unit, size
    """
    host = queue_host_units(con, args.module)
    src_cache = {}

    def body_in_source(row):
        """Cheap dispatch hint for unpaired rows: does the defining file
        already name this function with an argument list? Distinguishes
        'write it' from 'it exists - check the anchor / mangling first'."""
        if row["presence"] == "PAIRED" or not row["def_file"]:
            return None
        path = SOURCES / row["def_file"]
        if path not in src_cache:
            try:
                src_cache[path] = path.read_text(encoding="latin-1")
            except OSError:
                src_cache[path] = ""
        qn = qualified_name(row["demangled"] or "")
        if not qn:
            return None
        short = qn[1].split("<", 1)[0]
        return f"{short}(" in src_cache[path] or f"{short} (" in src_cache[path]

    by_unit = {}
    for r in con.execute(q, params):
        row = dict(r)
        row["body_in_source"] = body_in_source(row)
        by_unit.setdefault(host(row["unit"], row["demangled"]), []).append(row)

    def matched_pct(fns):
        """Size-weighted match level by MAX (best-ever %), NOT current: an LTO drop below
        a fn's peak is non-steerable, so it must NOT make its TU look urgent. A genuine
        re-match resets best to current (refresh), so real regressions do re-prioritize.
        TARGET_ONLY counts as 0%."""
        total = sum(f["size"] for f in fns)
        return sum((f["best"] or f["fuzzy_pct"] or 0) * f["size"] for f in fns) / total if total else 0.0

    # UNTRIED work first: a TU whose open functions were ALL dispatched before
    # is demoted by its least-tried function's attempt count, so parked walls
    # stop jumping to the front but still come back later (sushi, 2026-06-13).
    # Then LOWEST match level (real unmatched code beats polishing 99% TUs),
    # real .cpp TUs before header-only batches, size ascending.
    units = sorted(
        by_unit.items(),
        key=lambda kv: (
            min(f["attempts"] for f in kv[1]),
            matched_pct(kv[1]),
            0 if kv[0].endswith(".cpp") else 1,
            sum(f["size"] for f in kv[1]),
            kv[0],
        ),
    )
    if args.limit:
        units = units[: args.limit]

    if args.json:
        out = [
            {
                "unit": unit,
                "total_size": sum(f["size"] for f in fns),
                "matched_pct": round(matched_pct(fns), 2),
                "functions": [
                    {
                        k: f[k]
                        for k in (
                            "demangled", "mangled", "unit", "size", "fuzzy_pct",
                            "struct_class", "presence", "attempts",
                            "near_miss_base", "body_in_source",
                        )
                    }
                    for f in fns
                ],
            }
            for unit, fns in units
        ]
        print(json.dumps(out, indent=1))
        return
    for unit, fns in units:
        total = sum(f["size"] for f in fns)
        print(f"=== {unit}: {len(fns)} functions, {total:#x} bytes, {matched_pct(fns):.1f}% matched")
        for f in fns:
            pct = "-" if f["fuzzy_pct"] is None else f"{f['fuzzy_pct']:.1f}"
            via = "" if f["unit"] == unit else f"   [defined in {f['unit']}]"
            tried = f"  tried:{f['attempts']}" if f["attempts"] else ""
            hints = ""
            if f["near_miss_base"]:
                b = f["near_miss_base"]
                tgt = f["mangled"]
                cut = b.find("@@")
                hints = (
                    f"   [NEAR_MISS: body exists as @@{b[cut + 2:]} - change access/const "
                    f"to match target @@{tgt[tgt.find('@@') + 2:]}]"
                )
            elif f["body_in_source"]:
                hints = "   [body already in source - check anchor/mangling first]"
            print(
                f"  {f['size']:>6}  {pct:>6}  {f['struct_class'] or f['presence']:<11}  "
                f"{f['demangled'][:110]}{via}{tried}{hints}"
            )
