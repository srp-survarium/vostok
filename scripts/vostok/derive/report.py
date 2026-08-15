"""vostok.derive.report - the per-module / per-TU / per-function rollup.

One command with four scopes, because the question is always the same at a
different zoom: how much of this is matched, and where is the remainder. The
byte-weighted column is the honest headline - a 2 KB function at 50% outweighs
twenty 30-byte leaves at 100%.
"""

import sys

from vostok.derive.db import open_db, staleness_check
from vostok.derive.names import fn_from_mangled, shorten_fn
from vostok.derive.query import emit


def resolve_units(con, partial, module=None):
    """Unit names whose path CONTAINS `partial` (case-insensitive), optionally
    scoped to a module. A full path is a unique substring, so it resolves to one."""
    q, params = "SELECT name FROM units WHERE name LIKE ?", [f"%{partial}%"]
    if module:
        q, params = q + " AND module = ?", params + [module]
    return [r["name"] for r in con.execute(q + " ORDER BY name", params)]


def cmd_report(args):
    con = open_db(check_schema=True)
    staleness_check(con)

    # --function: every target fn whose demangled name CONTAINS the substring
    # (e.g. 'medkit::'), across units; same columns as --per-function plus a unit
    # column. --module optional; lists ALL matches (no ambiguity refusal).
    if args.function:
        mod = "AND t.module = ?" if args.module else ""
        mparams = [args.module] if args.module else []
        like = f"%{args.function}%"
        hdr = con.execute(
            f"""SELECT count(*) AS n,
                       printf('%.2f', sum(coalesce(p.fuzzy_pct, 0) * t.size) /
                                      nullif(sum(t.size), 0))        AS w,
                       printf('%.2f', avg(coalesce(p.fuzzy_pct, 0))) AS a
                FROM target_functions t JOIN symbols s ON s.id = t.sym
                LEFT JOIN pairs p ON p.sym = t.sym
                WHERE s.demangled LIKE ? {mod}""",
            [like] + mparams).fetchone()
        if not hdr["n"]:
            sys.exit(f"[match_db] no function matches '{args.function}'"
                     + (f" in module {args.module}" if args.module else ""))
        if not args.json:
            print(f"[match_db] {hdr['n']} fn(s) matching '{args.function}': "
                  f"weighted {hdr['w']}%  avg {hdr['a']}%", file=sys.stderr)
        ffq = f"""
          SELECT CASE WHEN p.fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', p.fuzzy_pct) END        AS pct,
                 CASE WHEN h.best_fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', h.best_fuzzy_pct) END   AS best,
                 coalesce(a.n, 0)                                 AS tries,
                 coalesce(p.struct_class, '-')                    AS cls,
                 t.size                                           AS size,
                 coalesce((SELECT group_concat(flag, '+') FROM flags
                           WHERE mangled = s.mangled), '-')       AS flag,
                 -- the source FILE (== the .cpp for a TU fn, the header for an
                 -- inline one); prefer it over the TU, strip the 'vostok/' prefix
                 CASE WHEN coalesce(fl.path, u.name) LIKE 'vostok/%'
                      THEN substr(coalesce(fl.path, u.name), 8)
                      ELSE coalesce(fl.path, u.name, '(no unit)') END AS file,
                 s.demangled                                      AS fn,
                 s.mangled                                        AS m
          FROM target_functions t
          JOIN symbols s ON s.id = t.sym
          LEFT JOIN units u ON u.id = t.unit
          LEFT JOIN files fl ON fl.id = t.file
          LEFT JOIN pairs p ON p.sym = t.sym
          LEFT JOIN history h ON h.mangled = s.mangled
          LEFT JOIN attempts a ON a.mangled = s.mangled
          WHERE s.demangled LIKE ? {mod}
          ORDER BY p.fuzzy_pct DESC, t.size DESC
        """
        rows = [dict(r) for r in con.execute(ffq, [like] + mparams)]
        for r in rows:
            m = r.pop("m")
            if not args.json:
                r["fn"] = (shorten_fn(r["fn"]) if args.verbose
                           else fn_from_mangled(m, r["fn"]))
        emit(rows, args.json)
        return

    # --unit: fuzzy-resolve to exactly ONE unit; refuse (with paste-ready full
    # names) on ambiguity. --module is optional - a unit substring stands alone.
    unit_full = None
    if args.unit:
        matches = resolve_units(con, args.unit, args.module)
        if not matches:
            sys.exit(f"[match_db] no unit matches '{args.unit}'"
                     + (f" in module {args.module}" if args.module else ""))
        if len(matches) > 1:
            sys.exit("Multiple units found:\n"
                     + "\n".join(f"  --unit {m}" for m in matches))
        unit_full = matches[0]

    # --per-function: list every function of ONE unit (pct / cls / size / flag),
    # sorted 100%->0% like the unit rollup; NULL pct = unpaired/open.
    if args.per_function:
        if not unit_full:
            sys.exit("[match_db] --per-function needs --unit (one unit)")
        if not args.json:  # unit-level weighted + avg % as a header (terminal only)
            s = con.execute(
                """SELECT printf('%.2f', sum(coalesce(p.fuzzy_pct, 0) * t.size) /
                                         sum(t.size))          AS w,
                          printf('%.2f', avg(coalesce(p.fuzzy_pct, 0))) AS a
                   FROM target_functions t LEFT JOIN pairs p ON p.sym = t.sym
                   JOIN units u ON u.id = t.unit WHERE u.name = ?""",
                [unit_full]).fetchone()
            print(f"[match_db] {unit_full}: weighted {s['w']}%  avg {s['a']}%",
                  file=sys.stderr)
        # best = best-ever fuzzy (history); best==100 with pct<100 is a TRANSIENT
        # (regressed) match. tries = how many matcher dispatches included this fn.
        fq = """
          SELECT CASE WHEN p.fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', p.fuzzy_pct) END        AS pct,
                 CASE WHEN h.best_fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', h.best_fuzzy_pct) END   AS best,
                 coalesce(a.n, 0)                                 AS tries,
                 coalesce(p.struct_class, '-')                    AS cls,
                 t.size                                           AS size,
                 coalesce((SELECT group_concat(flag, '+') FROM flags
                           WHERE mangled = s.mangled), '-')       AS flag,
                 s.demangled                                      AS fn,
                 s.mangled                                        AS m
          FROM target_functions t
          JOIN symbols s ON s.id = t.sym
          JOIN units u ON u.id = t.unit
          LEFT JOIN pairs p ON p.sym = t.sym
          LEFT JOIN history h ON h.mangled = s.mangled
          LEFT JOIN attempts a ON a.mangled = s.mangled
          WHERE u.name = ?
          ORDER BY p.fuzzy_pct DESC, t.size DESC
        """
        rows = [dict(r) for r in con.execute(fq, [unit_full])]
        for r in rows:
            m = r.pop("m")
            if not args.json:
                r["fn"] = (shorten_fn(r["fn"]) if args.verbose
                           else fn_from_mangled(m, r["fn"]))
        emit(rows, args.json)
        return

    per_unit = args.per_unit or unit_full is not None
    lite = args.lite or unit_full is not None  # one unit -> always the lean view
    scope = "unit_name" if per_unit else "module"
    if unit_full:
        where, params = "WHERE tf.unit_name = ?", [unit_full]
    elif args.module:
        where, params = "WHERE tf.module = ?", [args.module]
    else:
        where, params = "", []
    q = f"""
      WITH tf AS (
        SELECT t.rva, t.sym, t.size, t.frameless,
               coalesce(t.module, '(no unit)') AS module,
               coalesce(fl.path, un.name, '(no unit)') AS unit_name
        FROM target_functions t
        LEFT JOIN units un ON un.id = t.unit
        LEFT JOIN files fl ON fl.id = t.file)
      SELECT tf.{scope} AS scope,
             count(*)                                            AS target_fns,
             coalesce(sum(p.sym IS NOT NULL), 0)                 AS paired,
             coalesce(sum(p.fuzzy_pct >= 100), 0)                AS fuzzy_100,
             coalesce(sum(p.struct_class = 'MATCH'), 0)          AS struct_match,
             coalesce(sum(p.sym IS NULL), 0)                     AS target_only,
             coalesce(sum(tf.frameless), 0)                      AS custom_conv,
             printf('%.2f', sum(coalesce(p.fuzzy_pct, 0) * tf.size) /
                            sum(tf.size))                        AS weighted_pct,
             printf('%.2f', avg(coalesce(p.fuzzy_pct, 0)))       AS avg_pct
      FROM tf
      LEFT JOIN pairs p ON p.sym = tf.sym
      {where}
      GROUP BY scope
      ORDER BY sum(coalesce(p.fuzzy_pct, 0) * tf.size) / sum(tf.size) DESC, scope
    """
    rows = [dict(r) for r in con.execute(q, params)]
    # The base-only/out_of_scope/suspicious lints only ever scope BY MODULE; for a
    # single --unit they compute module-wide (or repo-wide) and the per-scope merge
    # below picks the row - and the lean --unit/--lite view drops them anyway.
    # per-unit scope prefers the source FILE (so header inlines show their .h, not
    # one '(no unit)' lump); module scope stays the module. The base_only/target_only
    # views carry both `file` and `unit`, so the counts merge against the same key.
    scope_expr = ("coalesce(file, unit, '(no unit)')" if per_unit
                  else "coalesce(module, '(no unit)')")
    sub_where = "WHERE module = ?" if args.module else ""
    sub_extra = "AND module = ?" if args.module else ""
    sub_params = [args.module] if args.module else []
    # base-only lint per scope
    bq = """SELECT {scope} AS scope, count(*) AS base_only
            FROM base_only {where} GROUP BY scope""".format(
        scope=scope_expr, where=sub_where)
    bonly = {r["scope"]: r["base_only"] for r in con.execute(bq, sub_params)}
    # out_of_scope: target-only functions whose history row survived (paired
    # once, vanished without a source touch)
    oq = """
      SELECT {scope} AS scope, count(*) AS n FROM target_only
      WHERE mangled IN (SELECT mangled FROM history) {extra} GROUP BY scope
    """.format(scope=scope_expr, extra=sub_extra)
    oos = {r["scope"]: r["n"] for r in con.execute(oq, sub_params)}
    # the fabricated-symbol lint: base-only rows nothing explains
    uq = """
      SELECT {scope} AS scope, count(*) AS n
      FROM base_only b JOIN base_only_status st ON st.mangled = b.mangled
      WHERE st.status IN ('UNEXPLAINED', 'NEAR_MISS')
        AND b.mangled NOT IN (
          SELECT mangled FROM flags WHERE flag = 'OUT_OF_SCOPE'
        ) {extra} GROUP BY scope
    """.format(scope=scope_expr, extra=sub_extra)
    suspicious = {r["scope"]: r["n"] for r in con.execute(uq, sub_params)}
    for r in rows:
        r["base_only"] = bonly.get(r["scope"], 0)
        r["suspicious"] = suspicious.get(r["scope"], 0)
        r["out_of_scope"] = oos.get(r["scope"], 0)
    if lite:
        rows = [r for r in rows if r["scope"] != "(no unit)"]
        for r in rows:
            for c in ("custom_conv", "out_of_scope", "suspicious"):
                r.pop(c, None)
    emit(rows, args.json)
