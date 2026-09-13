//! Base↔target *structure* diff: the statement-skeleton analog of the assembly
//! [`crate::rich_diff`]. Where that view aligns instruction text, this one aligns
//! the per-statement `(offset, size, line)` skeleton — the cheap structural
//! signal a matcher reads before generating code — so an agent sees exactly which
//! statement sizes diverge and where a statement is present on one side only.
//!
//! Pipeline mirrors the carcass derivation in [`crate::gen_sources`]:
//!   1. [`structure_rows`] turns a [`FunctionEntry`] into BODY rows, skipping the
//!      synthetic first/last frame braces and collapsing each source-line GAP
//!      (comment/blank/`#ifdef` runs) into a single [`Row::Empty`].
//!   2. [`diff_structure`] LCS-aligns the two row streams keyed on statement SIZE
//!      (empties match empties), then merges adjacent delete+insert runs into
//!      `Changed` rows (same statement, different size).
//!   3. [`render_structure_diff`] prints the aligned rows side-by-side, target on
//!      the left (matching the `target:` / `base:` header order), divergences
//!      tagged.
//!
//! v1 carries no `[n]` block-depth marker: the rich index records no block-depth
//! field, so depth is simply unavailable here (unlike the carcass, which reads it
//! straight from the PDB line program).

use std::fmt::Write as _;

use crate::rich_context::FunctionEntry;

/// One BODY row of a single side's statement skeleton. `Empty` is a collapsed
/// source-line gap (the carcass `<0>..<n>` run squashed to one marker).
pub enum Row<'a> {
    Stmt {
        off: u32,
        size: u32,
        line: u32,
        source: Option<&'a str>,
    },
    Empty,
}

/// Derive the BODY row sequence for one side, mirroring the carcass logic in
/// `gen_sources`: skip the first+last synthetic frame-brace statements, and
/// collapse every source-line gap between consecutive body statements (and before
/// the closing brace) into a single [`Row::Empty`].
pub fn structure_rows(f: &FunctionEntry) -> Vec<Row<'_>> {
    // Body-less functions carry only the frame braces — no body rows at all.
    if f.is_body_less() {
        return Vec::new();
    }

    let stmts = &f.statements;
    let len = stmts.len();
    let mut rows = Vec::new();

    // Track the next expected source line; a body statement whose line jumps ahead
    // signals a gap (comments/blanks/#ifdef) collapsed to one Empty.
    let mut next_line = stmts[0].line;

    for (i, s) in stmts.iter().enumerate() {
        // Skip the synthetic frame braces (same rule the carcass uses).
        if i == 0 || i == len - 1 {
            continue;
        }
        if s.line > next_line {
            rows.push(Row::Empty);
        }
        next_line = s.line + 1;
        rows.push(Row::Stmt {
            off: s.off,
            size: s.size,
            line: s.line,
            source: s.source.as_deref(),
        });
    }

    // A trailing gap between the last body statement and the closing frame brace.
    if let Some(last) = stmts.last() {
        if last.line > next_line {
            rows.push(Row::Empty);
        }
    }

    // Empty rows at the very start or end are just blank-line/comment/#ifdef gaps at the
    // function's top or bottom - not structural signal. Trim them so the diff begins and
    // ends on a real statement (and we don't emit boundary `EMPTY only ...` rows).
    while matches!(rows.first(), Some(Row::Empty)) {
        rows.remove(0);
    }
    while matches!(rows.last(), Some(Row::Empty)) {
        rows.pop();
    }

    rows
}

/// One aligned row of the structure diff. `base`/`target` carry the per-side
/// statement when present.
pub enum StructRow<'a> {
    /// Statement present on both sides with equal size.
    Equal {
        base: &'a Row<'a>,
        target: &'a Row<'a>,
    },
    /// Statement present on both sides but with a different size (a real
    /// structural divergence the matcher must close).
    Changed {
        base: &'a Row<'a>,
        target: &'a Row<'a>,
    },
    /// Statement present on the target only (missing in base).
    OnlyTarget { stmt: &'a Row<'a> },
    /// Statement present on the base only (extra).
    OnlyBase { stmt: &'a Row<'a> },
    /// Collapsed empty-line run present on both sides.
    EmptyEqual,
    /// Collapsed empty-line run present on the target only.
    EmptyOnlyTarget,
    /// Collapsed empty-line run present on the base only.
    EmptyOnlyBase,
}

fn is_stmt(r: &Row) -> bool {
    matches!(r, Row::Stmt { .. })
}

/// LCS alignment key for a row: empties match empties; statements match when
/// their SIZE is equal (the cheap structural identity the matcher compares).
fn row_key(r: &Row) -> String {
    match r {
        Row::Empty => "E".to_string(),
        Row::Stmt { size, .. } => format!("S{size}"),
    }
}

/// Internal op stream from the LCS backtrack (same shape as `rich_diff::diff`).
enum RawOp {
    Equal {
        base: usize,
        target: usize,
    },
    /// Base-only (delete to reach target).
    Delete {
        base: usize,
    },
    /// Target-only (insert to reach target).
    Insert {
        target: usize,
    },
}

/// Align the two BODY row streams (base = left, target = right, mirroring
/// `render_unified`'s `--- base / +++ target`) by statement size via LCS, then
/// merge adjacent delete+insert runs into `Changed` rows.
pub fn diff_structure<'a>(
    base_rows: &'a [Row<'a>],
    target_rows: &'a [Row<'a>],
) -> Vec<StructRow<'a>> {
    let b: Vec<String> = base_rows.iter().map(row_key).collect();
    let t: Vec<String> = target_rows.iter().map(row_key).collect();
    let (n, m) = (b.len(), t.len());

    // dp[i][j] = LCS length of b[i..] and t[j..] — copied from rich_diff::diff.
    let mut dp = vec![vec![0u32; m + 1]; n + 1];
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            dp[i][j] = if b[i] == t[j] {
                dp[i + 1][j + 1] + 1
            } else {
                dp[i + 1][j].max(dp[i][j + 1])
            };
        }
    }

    let mut ops = Vec::new();
    let (mut i, mut j) = (0, 0);
    while i < n && j < m {
        if b[i] == t[j] {
            ops.push(RawOp::Equal { base: i, target: j });
            i += 1;
            j += 1;
        } else if dp[i + 1][j] >= dp[i][j + 1] {
            ops.push(RawOp::Delete { base: i });
            i += 1;
        } else {
            ops.push(RawOp::Insert { target: j });
            j += 1;
        }
    }
    while i < n {
        ops.push(RawOp::Delete { base: i });
        i += 1;
    }
    while j < m {
        ops.push(RawOp::Insert { target: j });
        j += 1;
    }

    // POST-PROCESS: a run of Deletes immediately followed by a run of Inserts is
    // the same statement appearing on both sides with a different size; pair them
    // in order into `Changed` rows. Leftovers stay one-sided (a real quantity
    // divergence). Empty rows are never merged.
    let mut out = Vec::new();
    let mut k = 0;
    while k < ops.len() {
        match &ops[k] {
            RawOp::Equal { base, target } => {
                let (br, tr) = (&base_rows[*base], &target_rows[*target]);
                // Equal LCS key ⇒ same size (or both Empty).
                out.push(match (br, tr) {
                    (Row::Empty, Row::Empty) => StructRow::EmptyEqual,
                    _ => StructRow::Equal {
                        base: br,
                        target: tr,
                    },
                });
                k += 1;
            }
            RawOp::Delete { .. } => {
                // Collect the contiguous Delete run, then the following Insert run.
                let del_start = k;
                while k < ops.len() && matches!(ops[k], RawOp::Delete { .. }) {
                    k += 1;
                }
                let del_end = k;
                let ins_start = k;
                while k < ops.len() && matches!(ops[k], RawOp::Insert { .. }) {
                    k += 1;
                }
                let ins_end = k;

                // Statement rows of each run, in order — only these may pair into
                // `Changed`. Empty rows are never merged; they are emitted as the
                // one-sided empty variant in place.
                let del_stmts: Vec<&Row> = ops[del_start..del_end]
                    .iter()
                    .map(|o| {
                        let RawOp::Delete { base } = o else {
                            unreachable!()
                        };
                        &base_rows[*base]
                    })
                    .collect();
                let ins_stmts: Vec<&Row> = ops[ins_start..ins_end]
                    .iter()
                    .map(|o| {
                        let RawOp::Insert { target } = o else {
                            unreachable!()
                        };
                        &target_rows[*target]
                    })
                    .collect();

                let del_count = del_stmts.iter().filter(|r| is_stmt(r)).count();
                let ins_count = ins_stmts.iter().filter(|r| is_stmt(r)).count();
                let pairs = del_count.min(ins_count);

                // Emit the base side: each base Stmt up to `pairs` becomes a
                // `Changed` (paired with the matching target Stmt); leftover base
                // Stmts are only-in-base; Empties pass through one-sided.
                let mut ins_iter = ins_stmts.iter().filter(|r| is_stmt(r));
                let mut paired = 0;
                for br in &del_stmts {
                    match br {
                        Row::Empty => out.push(StructRow::EmptyOnlyBase),
                        Row::Stmt { .. } if paired < pairs => {
                            let tr = ins_iter.next().unwrap();
                            paired += 1;
                            out.push(StructRow::Changed {
                                base: br,
                                target: tr,
                            });
                        }
                        Row::Stmt { .. } => out.push(one_side(br, Side::Base)),
                    }
                }
                // Remaining target rows: target Stmts beyond `pairs` are
                // only-in-target; target Empties pass through one-sided.
                let mut consumed = 0;
                for tr in &ins_stmts {
                    match tr {
                        Row::Empty => out.push(StructRow::EmptyOnlyTarget),
                        Row::Stmt { .. } if consumed < pairs => consumed += 1,
                        Row::Stmt { .. } => out.push(one_side(tr, Side::Target)),
                    }
                }
            }
            RawOp::Insert { target } => {
                // An Insert run with no preceding Delete run (handled above).
                out.push(one_side(&target_rows[*target], Side::Target));
                k += 1;
            }
        }
    }

    out
}

enum Side {
    Base,
    Target,
}

/// A one-sided row: an Empty becomes the empty-only variant, a Stmt the
/// only-base/only-target variant.
fn one_side<'a>(r: &'a Row<'a>, side: Side) -> StructRow<'a> {
    match (r, side) {
        (Row::Empty, Side::Base) => StructRow::EmptyOnlyBase,
        (Row::Empty, Side::Target) => StructRow::EmptyOnlyTarget,
        (Row::Stmt { .. }, Side::Base) => StructRow::OnlyBase { stmt: r },
        (Row::Stmt { .. }, Side::Target) => StructRow::OnlyTarget { stmt: r },
    }
}

/// Render the full side-by-side structure diff. Target is the left column (to
/// match the `target:` / `base:` header order); each divergence carries a
/// trailing tag. When `condensed`, collapse aligned-equal runs to `.. same ..`
/// and emit only the divergence rows in a compact one-line form.
pub fn render_structure_diff(base: &FunctionEntry, target: &FunctionEntry) -> String {
    let base_rows = structure_rows(base);
    let target_rows = structure_rows(target);
    let rows = diff_structure(&base_rows, &target_rows);
    let va = |f: &FunctionEntry, off: u32| f.image_base.wrapping_add(f.rva).wrapping_add(off);

    // One row per DIVERGING statement; equal statements and `Empty` blank-line gaps
    // are dropped (the table IS the divergence list, tagged by `b.diff`).
    struct D {
        taddr: Option<u32>,
        baddr: Option<u32>,
        tsize: Option<u32>,
        bsize: Option<u32>,
        tline: Option<u32>,
        bline: Option<u32>,
        code: String,
        tag: String,
    }
    // Signed `b.sz - t.sz` (base minus target): positive = base is LARGER and must
    // shrink, negative = base is smaller and must grow.
    let signed_hex = |d: i64| {
        if d >= 0 {
            format!("+0x{:x}", d)
        } else {
            format!("-0x{:x}", -d)
        }
    };
    // No source text (target side, or a base stmt without captured source) renders
    // as `--`; the statement's LINE always goes in the dedicated line column - never
    // an `L<n>` stand-in here (it read as inconsistent output).
    let code_of = |r: &Row, _line: u32| match r {
        Row::Stmt {
            source: Some(s), ..
        } => s.to_string(),
        _ => "--".to_string(),
    };

    let mut ds: Vec<D> = Vec::new();
    for r in &rows {
        match r {
            // Equal statements and blank-line gaps are not divergences - skip.
            StructRow::Equal { .. }
            | StructRow::EmptyEqual
            | StructRow::EmptyOnlyTarget
            | StructRow::EmptyOnlyBase => {}
            // SIZE: same statement, different byte size. Anchor on the editable BASE
            // side (line/code); the target contributes its size (the goal).
            StructRow::Changed { base: b, target: t } => {
                if let (
                    Row::Stmt {
                        off: toff,
                        size: ts,
                        line: tline,
                        ..
                    },
                    Row::Stmt {
                        off: boff,
                        size: bs,
                        line,
                        ..
                    },
                ) = (t, b)
                {
                    let delta = signed_hex(*bs as i64 - *ts as i64);
                    ds.push(D {
                        taddr: Some(va(target, *toff)),
                        baddr: Some(va(base, *boff)),
                        tsize: Some(*ts),
                        bsize: Some(*bs),
                        tline: Some(*tline),
                        bline: Some(*line),
                        code: code_of(b, *line),
                        tag: format!("SIZE {delta}"),
                    });
                }
            }
            // Quantity: a statement present on one side only.
            StructRow::OnlyTarget { stmt } => {
                if let Row::Stmt {
                    off, size, line, ..
                } = stmt
                {
                    ds.push(D {
                        taddr: Some(va(target, *off)),
                        baddr: None,
                        tsize: Some(*size),
                        bsize: None,
                        tline: Some(*line),
                        bline: None,
                        code: code_of(stmt, *line),
                        tag: "TRGT_ONLY".to_string(),
                    });
                }
            }
            StructRow::OnlyBase { stmt } => {
                if let Row::Stmt {
                    off, size, line, ..
                } = stmt
                {
                    ds.push(D {
                        taddr: None,
                        baddr: Some(va(base, *off)),
                        tsize: None,
                        bsize: Some(*size),
                        tline: None,
                        bline: Some(*line),
                        code: code_of(stmt, *line),
                        tag: "BASE_ONLY".to_string(),
                    });
                }
            }
        }
    }

    let mut out = String::new();
    // A clean match has no diverging rows - say so explicitly (the table would be empty).
    if ds.is_empty() {
        let _ = writeln!(out, "; STRUCTURE MATCH");
    }
    // Then per-side stats, signature, and a braced body - same shape as the
    // single-side `--view structure`.
    let tstmts = target_rows
        .iter()
        .filter(|r| matches!(r, Row::Stmt { .. }))
        .count();
    let bstmts = base_rows
        .iter()
        .filter(|r| matches!(r, Row::Stmt { .. }))
        .count();
    let _ = writeln!(
        out,
        "; target va=0x{:x} rva=0x{:x}  {} stmts  0x{:x} bytes",
        va(target, 0),
        target.rva,
        tstmts,
        target.size
    );
    let _ = writeln!(
        out,
        "; base   va=0x{:x} rva=0x{:x}  {} stmts  0x{:x} bytes",
        va(base, 0),
        base.rva,
        bstmts,
        base.size
    );
    let _ = writeln!(out, "{}", base.name);
    let _ = writeln!(out, "{{");

    if !ds.is_empty() {
        // One row per diverging statement: base-anchored line/code + both VAs (so the
        // agent can `--address` either side) + both sizes. Data-driven hex widths.
        let sz = |o: Option<u32>| o.map(|v| format!("0x{v:x}")).unwrap_or_else(|| "--".into());
        let ah = ds
            .iter()
            .flat_map(|d| [d.taddr, d.baddr])
            .flatten()
            .map(|a| format!("{:x}", a).len())
            .max()
            .unwrap_or(1);
        let addr = |o: Option<u32>| {
            o.map(|a| format!("0x{:0ah$x}", a, ah = ah))
                .unwrap_or_else(|| "--".into())
        };
        let wd = ds
            .iter()
            .map(|d| d.tag.len())
            .max()
            .unwrap_or(0)
            .max("b.diff".len());
        let wta = ds
            .iter()
            .map(|d| addr(d.taddr).len())
            .max()
            .unwrap_or(0)
            .max("t.va".len());
        let wba = ds
            .iter()
            .map(|d| addr(d.baddr).len())
            .max()
            .unwrap_or(0)
            .max("b.va".len());
        let wt = ds
            .iter()
            .map(|d| sz(d.tsize).len())
            .max()
            .unwrap_or(0)
            .max("t.sz".len());
        let wb = ds
            .iter()
            .map(|d| sz(d.bsize).len())
            .max()
            .unwrap_or(0)
            .max("b.sz".len());
        // Lines print as DELTAS from each side's first statement line ("0", "+2",
        // ...), one scale per column - corresponding rows align across sides
        // without mixing the two absolute line-number spaces. A TRGT_ONLY row's
        // lookup identity is its t.addr (feed it to --address).
        let tref = target_rows
            .iter()
            .find_map(|r| {
                if let Row::Stmt { line, .. } = r {
                    Some(*line)
                } else {
                    None
                }
            })
            .unwrap_or(0);
        let bref = base_rows
            .iter()
            .find_map(|r| {
                if let Row::Stmt { line, .. } = r {
                    Some(*line)
                } else {
                    None
                }
            })
            .unwrap_or(0);
        let dl = |l: Option<u32>, r: u32| {
            l.map(|l| {
                let d = l as i64 - r as i64;
                if d > 0 {
                    format!("+{d}")
                } else {
                    d.to_string()
                }
            })
            .unwrap_or_else(|| "--".into())
        };
        let wtl = ds
            .iter()
            .map(|d| dl(d.tline, tref).len())
            .max()
            .unwrap_or(0)
            .max("t.ln".len());
        let wbl = ds
            .iter()
            .map(|d| dl(d.bline, bref).len())
            .max()
            .unwrap_or(0)
            .max("b.ln".len());
        let _ = writeln!(
            out,
            "; t.ln/b.ln: line deltas from each side's first statement"
        );
        let _ = writeln!(
            out,
            "{:<wd$}|{:<wta$}|{:<wba$}|{:<wt$}|{:<wb$}|{:<wtl$}|{:<wbl$}|b.code",
            "b.diff", "t.va", "b.va", "t.sz", "b.sz", "t.ln", "b.ln"
        );
        let _ = writeln!(
            out,
            "{}+{}+{}+{}+{}+{}+{}+------",
            "-".repeat(wd),
            "-".repeat(wta),
            "-".repeat(wba),
            "-".repeat(wt),
            "-".repeat(wb),
            "-".repeat(wtl),
            "-".repeat(wbl)
        );
        for d in &ds {
            let bcode: &str = if d.baddr.is_some() {
                d.code.as_str()
            } else {
                "--"
            };
            let _ = writeln!(
                out,
                "{:<wd$}|{:<wta$}|{:<wba$}|{:<wt$}|{:<wb$}|{:<wtl$}|{:<wbl$}|{}",
                d.tag,
                addr(d.taddr),
                addr(d.baddr),
                sz(d.tsize),
                sz(d.bsize),
                dl(d.tline, tref),
                dl(d.bline, bref),
                bcode
            );
        }
    }
    let _ = writeln!(out, "}}");
    out
}
