//! Render a structured [`FunctionEntry`] into a text view. Shared by the build
//! (tree files), the query tool and the fetch tool so every surface renders
//! identically. The build stores structure, never strings; rendering is here.

use std::collections::HashMap;
use std::fmt::Write as _;

use crate::rich_context::FunctionEntry;

/// Full listing: offset-prefixed instructions, with `; <0xSIZE> ; <source>`
/// appended on each statement's first instruction. `source` is omitted for
/// target functions (they carry none); the leading offset is the anchor either
/// way. Local labels print on their own line above the instruction.
///
/// (Display only — the exact column layout is intentionally loose; the diff and
/// structure views read the underlying [`FunctionEntry`], not this text.)
pub fn render_listing(f: &FunctionEntry) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "{}:", f.name);

    // Body-less functions carry only the synthetic frame-brace statements; don't
    // annotate the disassembly with them (keeps base/target listings identical).
    let stmt_at: HashMap<u32, &crate::rich_context::Statement> = if f.is_body_less() {
        HashMap::new()
    } else {
        f.statements.iter().map(|s| (s.off, s)).collect()
    };

    for insn in &f.instructions {
        // The statement heads its instruction group on its OWN line, labelled by
        // the statement's offset `[0xNN]:` (distinct from the `0xNN:` instruction
        // lines), with the source text on base.
        if let Some(stmt) = stmt_at.get(&insn.off) {
            match &stmt.source {
                Some(src) => {
                    let _ = writeln!(out, "[0x{:02x}]: {src}", insn.off);
                }
                None => {
                    let _ = writeln!(out, "[0x{:02x}]:", insn.off);
                }
            }
        }
        if let Some(label) = &insn.label {
            let _ = writeln!(out, "{label}:");
        }
        let _ = writeln!(out, "0x{:02x}:    {}", insn.off, insn.text);
    }
    out
}

/// Just ONE body statement's disassembly - the instructions in `[off, off+size)`
/// of the `n`-th body statement (1-based, matching the `--view structure` rows).
/// Lets an agent compare a single diverging statement target-vs-base without
/// pulling the whole function into context. A header `;` line names the statement
/// (VA, index, size, line, source); instruction offsets stay function-relative so
/// they line up with the full listing / structure view.
pub fn render_listing_statement(f: &FunctionEntry, n: usize) -> String {
    let mut out = String::new();
    // Body statements are `statements[1..len-1]` (index 0 / len-1 are the synthetic
    // frame braces), so the n-th body statement is `statements[n]`.
    let last_body = f.statements.len().saturating_sub(2);
    if n < 1 || n > last_body {
        let _ = writeln!(
            out,
            "{}: no body statement #{} ({} body statements)",
            f.name, n, last_body
        );
        return out;
    }
    let stmt = &f.statements[n];
    let (lo, hi) = (stmt.off, stmt.off + stmt.size);
    let va = f.image_base.wrapping_add(f.rva).wrapping_add(lo);
    let _ = writeln!(out, "; va=0x{:x} stmt #{} line {}", va, n, stmt.line);
    let _ = writeln!(out, "{}", f.name);
    // The statement heads its instructions on its own line `[0xNN]:`, like the
    // full listing; the source text follows on base.
    match &stmt.source {
        Some(src) => {
            let _ = writeln!(out, "[0x{:02x}]: {src}", lo);
        }
        None => {
            let _ = writeln!(out, "[0x{:02x}]:", lo);
        }
    }
    for insn in f.instructions.iter().filter(|i| i.off >= lo && i.off < hi) {
        if let Some(label) = &insn.label {
            let _ = writeln!(out, "{label}:");
        }
        let _ = writeln!(out, "0x{:02x}:    {}", insn.off, insn.text);
    }
    out
}

/// Function info: the PDB-recorded locals (`type  name`). Exact in the
/// non-optimized parts of the build.
pub fn render_info(f: &FunctionEntry) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "{}:", f.name);
    let _ = writeln!(out, "; locals ({}) — PDB-recorded", f.locals.len());
    for l in &f.locals {
        let _ = writeln!(out, "  {}\t{}", l.ty, l.name);
    }
    out
}

/// Structure-only: the statement skeleton (offset, size, line, source) without
/// any disassembly — "the amount of statements and their length", which is the
/// cheap structural signal a matcher can compare before generating code.
pub fn render_structure(f: &FunctionEntry) -> String {
    let mut out = String::new();

    // Body-less functions (empty `{}`) carry only the synthetic frame braces,
    // which the two PDBs encode with a varying statement count (1 or 2). Normalize
    // every form to an empty body: header line only, zero statement rows.
    let func_va = f.image_base.wrapping_add(f.rva);
    if f.is_body_less() {
        let _ = writeln!(
            out,
            "; va=0x{:x} rva=0x{:x}, 0 statements, 0x{:x} bytes",
            func_va, f.rva, f.size
        );
        let _ = writeln!(out, "{}", f.name);
        let _ = writeln!(out, "{{");
        render_locals_into(&mut out, f);
        let _ = writeln!(out, "}}");
        return out;
    }

    // The FIRST and LAST statements are the synthetic frame braces (`{` and `}`);
    // the real body is strictly between them (the same skip-first-last rule that
    // `gen_sources` and the structure-diff use). Show only those body statements,
    // and the locals, so this view matches the carcass's `// FUNCTION BODY` +
    // `// LOCALS` (each declared local is its own statement under /Od).
    let n = f.statements.len();
    // Stats on their own comment line so the signature line stays short.
    let _ = writeln!(
        out,
        "; va=0x{:x} rva=0x{:x}, {} statements, 0x{:x} bytes",
        func_va,
        f.rva,
        n - 2,
        f.size
    );
    let _ = writeln!(out, "{}", f.name);
    // Wrap the body in braces for clarity (echoes the synthetic frame `{`/`}`).
    let _ = writeln!(out, "{{");
    render_locals_into(&mut out, f);

    // Body rows (strictly between the synthetic frame braces): absolute VA, function
    // offset, signed byte-size to the next statement (the closing `}` for the last
    // row; negative when the next statement sits at a lower address) and source line
    // (+ source text on base). VA is derived from the entry's own image base.
    struct Row {
        va: u32,
        off: u32,
        delta: i64,
        depth: i32,
        line: u32,
        src: Option<String>,
    }
    let rows: Vec<Row> = (1..n - 1)
        .map(|i| {
            let s = &f.statements[i];
            Row {
                va: f.image_base.wrapping_add(f.rva).wrapping_add(s.off),
                off: s.off,
                delta: f.statements[i + 1].off as i64 - s.off as i64,
                depth: s.depth,
                line: s.line,
                src: s.source.clone(),
            }
        })
        .collect();

    // Hex-digit width per column: the widest value present, but never narrower than
    // what the header label needs (label minus its `0x`/`+0x` prefix). Zero-padding
    // to this width keeps the `0x006`/`+0x00b` look yet grows cleanly when the VA,
    // offset or size get larger - nothing is ever a hardcoded width.
    let hexw = |vals: &mut dyn Iterator<Item = u64>, min: usize| {
        vals.map(|v| format!("{:x}", v).len())
            .max()
            .unwrap_or(1)
            .max(min)
    };
    let da = hexw(&mut rows.iter().map(|r| r.va as u64), "va".len());
    let dofs = hexw(&mut rows.iter().map(|r| r.off as u64), "offst".len() - 2);
    let dd = hexw(
        &mut rows.iter().map(|r| r.delta.unsigned_abs()),
        "size".len() - 3,
    );
    // Column widths follow from the prefixes: `0x`+da, `0x`+dofs, sign+`0x`+dd.
    let (wa, wo, ws) = (2 + da, 2 + dofs, 3 + dd);
    let wl = rows
        .iter()
        .map(|r| format!("{}", r.line).len())
        .max()
        .unwrap_or(0)
        .max("line".len());

    // `{}` blocks that open at a no-statement RVA can't be marked on a row; list
    // them on their own (VA + offset aligned to the table below), like the carcass
    // "SKIPPED BLOCKS" section. The local's `scope: N` still flags they exist.
    if !f.skipped_blocks.is_empty() {
        let _ = writeln!(out, "; skipped blocks ({}):", f.skipped_blocks.len());
        for &(off, depth) in &f.skipped_blocks {
            let va = f.image_base.wrapping_add(f.rva).wrapping_add(off);
            let _ = writeln!(
                out,
                ";   0x{:0da$x}|0x{:0dofs$x}  scope: {}",
                va, off, depth
            );
        }
    }

    // The `scope` column (carcass `[N]`: a `{}` block opens at this statement) only
    // appears when some block actually opens, so simple bodies stay 4 columns wide.
    let scope_cell = |d: i32| {
        if d > 0 {
            format!("[{}]", d)
        } else {
            String::new()
        }
    };
    let has_scope = rows.iter().any(|r| r.depth > 0);
    let wsc = rows
        .iter()
        .map(|r| scope_cell(r.depth).len())
        .max()
        .unwrap_or(0)
        .max("scope".len());

    // `code` column (the base's source text) appears only when some row carries it.
    let has_code = rows.iter().any(|r| r.src.is_some());

    // Header + separator, assembled from the present columns so they always align.
    let mut header = format!("{:<wa$}|{:<wo$}|{:^ws$}", "va", "offst", "size");
    let mut sep = format!("{}+{}+{}", "-".repeat(wa), "-".repeat(wo), "-".repeat(ws));
    if has_scope {
        header += &format!("|{:^wsc$}", "scope");
        sep += &format!("+{}", "-".repeat(wsc));
    }
    header += &format!("|{:<wl$}", "line");
    sep += &format!("+{}", "-".repeat(wl));
    if has_code {
        header += "|code";
        sep += "+----";
    }
    let _ = writeln!(out, "{}", header);
    let _ = writeln!(out, "{}", sep);

    for r in &rows {
        let sign = if r.delta < 0 { "-" } else { "+" };
        let _ = write!(
            out,
            "0x{:0da$x}|0x{:0dofs$x}|{sign}0x{:0dd$x}|",
            r.va,
            r.off,
            r.delta.unsigned_abs(),
        );
        if has_scope {
            let _ = write!(out, "{:<wsc$}|", scope_cell(r.depth));
        }
        // When a `code` column is present, pad `line` to its width and add a `|` so
        // the source lines up; otherwise emit `line` bare (no trailing whitespace).
        match (&r.src, has_code) {
            (Some(s), _) => {
                let _ = writeln!(out, "{:<wl$}|{s}", r.line);
            }
            (None, true) => {
                let _ = writeln!(out, "{:<wl$}|", r.line);
            }
            (None, false) => {
                let _ = writeln!(out, "{}", r.line);
            }
        }
    }
    let _ = writeln!(out, "}}");
    out
}

/// Append the PDB-recorded locals as `; locals (N):` then `;   <type>\t<name>`
/// rows (nothing when there are none).
fn render_locals_into(out: &mut String, f: &FunctionEntry) {
    if f.locals.is_empty() {
        return;
    }
    // Blank lines around the block keep it from crowding the signature / table.
    let _ = writeln!(out);
    let _ = writeln!(out, "; locals ({}):", f.locals.len());
    // Align the type and name columns; spell out the scope depth (a local declared
    // inside nested `{}` blocks) so it doesn't read like the type's template brackets.
    let wt = f.locals.iter().map(|l| l.ty.len()).max().unwrap_or(0);
    let wn = f.locals.iter().map(|l| l.name.len()).max().unwrap_or(0);
    for l in &f.locals {
        let mut line = format!(";   {:<wt$}  {:<wn$}", l.ty, l.name);
        if l.scope > 0 {
            line += &format!(" | scope: {}", l.scope);
        }
        let _ = writeln!(out, "{}", line.trim_end());
    }
    let _ = writeln!(out);
}
