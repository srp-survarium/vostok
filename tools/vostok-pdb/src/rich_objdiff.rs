//! Precise base↔target diff via `objdiff-core`: operand/relocation-aware, so the
//! match percentage is meaningful (a callee resolving to a different name, or a
//! relocation, doesn't count as a mismatch the way the text LCS backend would).
//!
//! Inputs are the delinker's per-unit COFF objects
//! (`binaries/objdiff/{base,target}/<file>.obj`); our [`FunctionEntry::file`]
//! maps straight to them and [`FunctionEntry::mangled`] is the COFF symbol name
//! to look up. objdiff matches symbols across the two objects by name; we pull
//! out the one we asked for as a structured row stream ([`ObjdiffResult`]).
//!
//! [`render`] then interleaves our own source/offset metadata back onto those
//! rows, walking the base [`FunctionEntry`] in lockstep — the diff is computed on
//! raw asm (objdiff), the metadata is attached only for display.

use std::collections::HashMap;
use std::fmt::Write as _;
use std::path::Path;

use objdiff_core::diff::{
    DiffObjConfig, ObjDiff, ObjInsDiff, ObjInsDiffKind, ObjSymbolDiff, diff_objs,
};
use objdiff_core::obj::ObjInfo;
use objdiff_core::obj::read::read;

use crate::rich_context::{FunctionEntry, Statement};

#[derive(Clone, Copy, PartialEq, Eq)]
pub enum RowKind {
    /// Identical in both.
    Equal,
    /// Same slot, different instruction (op/arg/replace mismatch).
    Replace,
    /// Base only (must be removed to reach target).
    Delete,
    /// Target only (must be added to reach target).
    Insert,
}

/// One aligned diff row. `base`/`target` hold the rendered instruction text for
/// each side (whichever is present for the kind); `base_off` is the row's offset
/// from the base function start (from objdiff's own addresses, so it survives the
/// two disassemblers splitting instructions differently).
pub struct ObjdiffRow {
    pub kind: RowKind,
    pub base: Option<String>,
    pub target: Option<String>,
    pub base_off: Option<u32>,
}

pub struct ObjdiffResult {
    /// Fuzzy instruction-level match percent (0..100): partial per-instruction
    /// credit, so it tracks the scoreboard (`report.json`) rather than objdiff-core
    /// 2.5.0's strict all-or-nothing match. The retry-budget signal.
    pub match_percent: f32,
    pub rows: Vec<ObjdiffRow>,
}

fn anyhow_to_err(e: anyhow::Error) -> crate::Error {
    crate::Error::new(format!("{e:#}"))
}

/// Diff the function named `mangled` (a COFF symbol name) between the two object
/// files. Returns `None` if either object lacks the symbol (caller can fall back
/// to the text diff).
pub fn diff(
    base_obj: &Path,
    target_obj: &Path,
    mangled: &str,
) -> crate::Result<Option<ObjdiffResult>> {
    let cfg = DiffObjConfig::default();
    let base = read(base_obj, &cfg).map_err(anyhow_to_err)?;
    let target = read(target_obj, &cfg).map_err(anyhow_to_err)?;

    let res = diff_objs(&cfg, Some(&base), Some(&target), None).map_err(anyhow_to_err)?;
    let (Some(base_diff), Some(target_diff)) = (res.left.as_ref(), res.right.as_ref()) else {
        return Ok(None);
    };

    let Some(bsym) = find_symbol(base_diff, &base, mangled) else {
        return Ok(None);
    };
    // The base function's first instruction address is the origin for offsets.
    let origin = bsym
        .instructions
        .iter()
        .find_map(|d| d.ins.as_ref().map(|i| i.address))
        .unwrap_or(0);

    // objdiff-core 2.5.0's symbol `match_percent` is STRICT (any differing
    // instruction is a full miss), which badly understates LTCG-heavy code and
    // disagrees with the scoreboard's fuzzy `report.json` (e.g. 56% vs 89%). When
    // the two symbols are paired we recompute a FUZZY match here via partial
    // per-instruction credit (see `fuzzy_credit`); unpaired -> strict fallback.
    let (rows, match_percent) = match bsym.target_symbol.map(|r| target_diff.symbol_diff(r)) {
        Some(tsym) if tsym.instructions.len() == bsym.instructions.len() => {
            let mut score = 0.0f32;
            let mut max = 0.0f32;
            let rows = bsym
                .instructions
                .iter()
                .zip(tsym.instructions.iter())
                .map(|(l, r)| {
                    let (s, m) = fuzzy_credit(l, r);
                    score += s;
                    max += m;
                    make_row(l, r, origin)
                })
                .collect();
            let pct = if max > 0.0 {
                score / max * 100.0
            } else {
                100.0
            };
            (rows, pct)
        }
        _ => (
            bsym.instructions
                .iter()
                .map(|l| make_row(l, l, origin))
                .collect(),
            bsym.match_percent.unwrap_or(0.0),
        ),
    };

    Ok(Some(ObjdiffResult {
        match_percent,
        rows,
    }))
}

/// Fuzzy per-instruction credit `(score, max)` for one aligned pair, **weighted by
/// instruction byte size** to track the scoreboard's `report.json` (which is
/// byte-weighted), not objdiff-core 2.5.0's strict all-or-nothing instruction
/// match. Within an instruction the match fraction is objdiff's "1 for the opcode +
/// 1 per operand" scheme, so a stack-slot-only `~` keeps most of its bytes.
fn fuzzy_credit(l: &ObjInsDiff, r: &ObjInsDiff) -> (f32, f32) {
    let bytes = |d: &ObjInsDiff| d.ins.as_ref().map_or(0u8, |i| i.size) as f32;
    // Target-relative (like report.json): weigh by the TARGET instruction's bytes,
    // so a base-only `-` (Delete, no target side) weighs 0 and extra base code does
    // not penalize - only how much of the target we reproduced counts.
    let weight = bytes(r);
    let frac = match l.kind {
        // Identical: full credit.
        ObjInsDiffKind::None => 1.0,
        // Same shape, some operands differ. ArgMismatch keeps the opcode (credit
        // it); OpMismatch changed the mnemonic (no opcode credit). Credit each
        // matching operand (a `None` entry in arg_diff).
        ObjInsDiffKind::ArgMismatch | ObjInsDiffKind::OpMismatch => {
            let total = l.arg_diff.len();
            let matched = l.arg_diff.iter().filter(|d| d.is_none()).count();
            let op = if matches!(l.kind, ObjInsDiffKind::ArgMismatch) {
                1.0
            } else {
                0.0
            };
            (op + matched as f32) / (1.0 + total as f32)
        }
        // Replace (different op/arg-count) or Delete/Insert (one side only): no
        // credit, but its bytes still count against the total.
        _ => 0.0,
    };
    (weight * frac, weight)
}

fn make_row(l: &ObjInsDiff, r: &ObjInsDiff, origin: u64) -> ObjdiffRow {
    let lt = || text(l).to_string();
    let rt = || text(r).to_string();
    let base_off = l.ins.as_ref().map(|i| (i.address - origin) as u32);
    let (kind, base, target) = match l.kind {
        ObjInsDiffKind::None => (RowKind::Equal, Some(lt()), Some(rt())),
        ObjInsDiffKind::Delete => (RowKind::Delete, Some(lt()), None),
        ObjInsDiffKind::Insert => (RowKind::Insert, None, Some(rt())),
        ObjInsDiffKind::Replace | ObjInsDiffKind::OpMismatch | ObjInsDiffKind::ArgMismatch => {
            (RowKind::Replace, Some(lt()), Some(rt()))
        }
    };
    ObjdiffRow {
        kind,
        base,
        target,
        base_off,
    }
}

/// Render the diff with our source/offset metadata interleaved. Source comes from
/// the *base* `FunctionEntry` (the compiled side that has it): each base-side
/// row's offset is `base_off`, and when that offset starts a source statement we
/// emit the statement header first. Keyed by offset, so it is robust to objdiff
/// and iced splitting instructions differently.
pub fn render(result: &ObjdiffResult, base: &FunctionEntry) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "{}:", base.name);
    let _ = writeln!(out, "; objdiff fuzzy match {:.2}%", result.match_percent);

    let stmt_at: HashMap<u32, &Statement> = base.statements.iter().map(|s| (s.off, s)).collect();

    for row in &result.rows {
        if let Some(off) = row.base_off {
            if let Some(stmt) = stmt_at.get(&off) {
                match &stmt.source {
                    Some(src) => {
                        let _ = writeln!(out, "{src}\t; <0x{:x}>", stmt.size);
                    }
                    None => {
                        let _ = writeln!(out, "[0x{off:x}]\t; <0x{:x}>", stmt.size);
                    }
                }
            }
        }
        render_row(&mut out, row, row.base_off);
    }
    out
}

fn render_row(out: &mut String, row: &ObjdiffRow, off: Option<u32>) {
    let at = off.map(|o| format!("0x{o:02x}: ")).unwrap_or_default();
    let base = row.base.as_deref().unwrap_or("");
    let target = row.target.as_deref().unwrap_or("");
    match row.kind {
        RowKind::Equal => {
            let _ = writeln!(out, "  {at}{base}");
        }
        RowKind::Replace => {
            let _ = writeln!(out, "~ {at}{base:<28} -> {target}");
        }
        RowKind::Delete => {
            let _ = writeln!(out, "- {at}{base}");
        }
        RowKind::Insert => {
            let _ = writeln!(out, "+ {target}");
        }
    }
}

fn find_symbol<'a>(diff: &'a ObjDiff, obj: &ObjInfo, mangled: &str) -> Option<&'a ObjSymbolDiff> {
    diff.sections
        .iter()
        .flat_map(|s| s.symbols.iter())
        .find(|sd| {
            let sym = &obj.sections[sd.symbol_ref.section_idx].symbols[sd.symbol_ref.symbol_idx];
            sym.name == mangled
        })
}

fn text(d: &ObjInsDiff) -> &str {
    d.ins.as_ref().map(|i| i.formatted.as_str()).unwrap_or("")
}
