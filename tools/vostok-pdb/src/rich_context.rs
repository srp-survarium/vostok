//! `pdb_rich_context` core: build, per engine function, a *structured* record
//! that pairs the disassembly with the source-level statements that produced it.
//!
//! The two halves come from existing tooling:
//!   * **source half** — the PDB line program gives, per function, a sequence of
//!     `(rva, source-line)` statement boundaries (the same data
//!     `gen_sources.rs` turns into the carcass `FUNCTION BODY` block).
//!   * **disasm half** — the EXE `.text` bytes for the function, decoded with
//!     `iced-x86` (the same slicing `vostok-delinker` does).
//!
//! Both are keyed by the *same* RVA (`proc.offset.to_rva`), so mapping
//! instructions to statements is an exact sorted merge: each statement owns the
//! instructions in `[its_rva, next_statement_rva)`, and its size is that span.
//!
//! We deliberately store this as **structured data** ([`FunctionEntry`]), not a
//! pre-rendered string: the query/fetch tools render it into whichever view is
//! asked for (full listing, structure-only, or a base↔target diff computed on
//! the raw instruction text *before* any metadata is attached). Rendering lives
//! in [`crate::rich_render`]; diffing in [`crate::rich_diff`].
//!
//! Base vs target differ only in [`Statement::source`]: base reads the real
//! source line from disk; target (the original game, no sources) leaves it
//! `None`. Offsets, sizes, labels and disassembly are identical.

use std::collections::BTreeMap;
use std::collections::HashMap;
use std::path::Path;
use std::path::PathBuf;
use std::rc::Rc;

use pdb::FallibleIterator;
use pdb::PDB;

use iced_x86::Formatter as _;

use object::LittleEndian;
use object::{Object, ObjectSection};

use crate::disasm;
use crate::pdb_parser::PdbParser;

/// RVA -> recovered name, for naming call/data targets in operands. Names prefer
/// the readable module-symbol form (`vostok::foo::bar`); public (mangled) names
/// are kept only as a fallback for library symbols without debug info.
pub struct SymbolMaps {
    pub functions: BTreeMap<usize, String>,
    pub data: BTreeMap<usize, String>,
    /// RVA -> decorated (mangled) name, from Public symbols only. Module
    /// Procedure symbols carry only the undecorated `ns::func`; the COFF objects
    /// (and thus objdiff) key on the decorated name, which lives here.
    pub public_functions: BTreeMap<usize, String>,
}

/// One decoded instruction. `text` is the rendered mnemonic+operands with branch
/// targets resolved to local labels and call/data targets to symbol names — this
/// is the *normalized* form the diff aligns on.
#[derive(Clone, Debug, serde::Serialize, serde::Deserialize)]
pub struct Instruction {
    /// Offset from the function start.
    pub off: u32,
    /// Instruction length in bytes.
    pub len: u8,
    /// Rendered mnemonic + operands.
    pub text: String,
    /// Local label sitting at this offset (e.g. `.1`), if it is a branch target.
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub label: Option<String>,
}

/// A PDB-recorded local variable (name + type). Exact in the non-optimized
/// parts of the build.
#[derive(Clone, Debug, serde::Serialize, serde::Deserialize)]
pub struct Local {
    pub name: String,
    pub ty: String,
    /// Lexical-scope depth the local is declared in (0 = function body; >0 = inside
    /// nested `{}` blocks). Mirrors the carcass `name<N>` scope marker.
    #[serde(default, skip_serializing_if = "is_zero_usize")]
    pub scope: usize,
}

fn is_zero_usize(v: &usize) -> bool {
    *v == 0
}

/// One source-level statement (a line-program boundary) and the byte span it
/// compiled to.
#[derive(Clone, Debug, serde::Serialize, serde::Deserialize)]
pub struct Statement {
    /// Offset from the function start where this statement's code begins.
    pub off: u32,
    /// Byte size of this statement's instruction run (span to the next statement).
    pub size: u32,
    /// Source line number (0 = unknown).
    pub line: u32,
    /// File attached to this exact line-program record. Inlined statements can
    /// switch files within one procedure, so this cannot be inherited from it.
    #[serde(default, skip_serializing_if = "String::is_empty")]
    pub file: String,
    /// Lexical-scope depth where a `{}` block *opens* at this statement (0 = none).
    /// Mirrors the carcass `[N]` marker; only the block-opening statement carries it.
    #[serde(default, skip_serializing_if = "is_zero_i32")]
    pub depth: i32,
    /// Real source text (base only; `None` for target / inlined-headerless code).
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub source: Option<String>,
}

fn is_zero_i32(v: &i32) -> bool {
    *v == 0
}

/// One function's full structured record stored in the evidence database.
/// Base and target functions primarily join by [`FunctionEntry::mangled`].
#[derive(Clone, Debug, serde::Serialize, serde::Deserialize)]
pub struct FunctionEntry {
    /// Full demangled signature.
    pub name: String,
    /// Decorated (mangled) COFF symbol name — the join key to the delinker `.obj`
    /// files for the objdiff backend.
    pub mangled: String,
    /// Function RVA (image-relative) — the merge key shared with the line program.
    pub rva: u32,
    /// PE image base (from the EXE optional header). Stored per-entry so renderers
    /// derive absolute VAs (`image_base + rva + off`) without a hardcoded constant.
    #[serde(default)]
    pub image_base: u32,
    /// Function length in bytes.
    pub size: u32,
    /// Source file, `/`-separated (engine-relative in tree mode).
    pub file: String,
    /// Statement boundaries, sorted by offset.
    pub statements: Vec<Statement>,
    /// Instructions, in address order.
    pub instructions: Vec<Instruction>,
    /// PDB-recorded local variables.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub locals: Vec<Local>,
    /// `{}` blocks that open at an RVA with no line-program statement (so they
    /// can't be marked on a statement row). `(off, depth)`, mirrors the carcass
    /// "SKIPPED BLOCKS" section.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub skipped_blocks: Vec<(u32, i32)>,
}

impl FunctionEntry {
    /// A *body-less* function is one whose source body is empty `{}` — a
    /// member-initializer-list ctor, a default/empty ctor, an empty virtual
    /// override. Its only statements are the synthetic frame braces, so it
    /// carries no real body statement.
    ///
    /// The first and last statements are always the synthetic frame braces (the
    /// `{` init region and the closing `}`); a real body statement sits *strictly*
    /// between them. So the function is body-less iff it has no statement between
    /// those two, i.e. `statements.len() <= 2`. This is the same skip-first-last
    /// rule `gen_sources.rs` uses (it emits body rows only for `0 < i < len-1`),
    /// and it spans every encoding both PDBs produce:
    ///   * **target** (no sources): the frame may collapse to a *single* statement
    ///     (`len == 1`), or appear as two statements on separate source lines —
    ///     init region + closing brace (`len == 2`), with no body between.
    ///   * **base** (real sources): two statements whose source texts are the
    ///     opening `{` and closing `}` brace lines (`len == 2`).
    ///
    /// All render as an empty FUNCTION BODY (header only), which removes the
    /// confusing "0 vs 1/2 statements" mismatch for trivially-matching bodies. A
    /// real one-line body is recorded as >= 3 statements (`{`, the body, `}`), so
    /// no genuine body content is ever blanked.
    pub fn is_body_less(&self) -> bool {
        self.statements.len() <= 2
    }
}

pub struct Options {
    /// Recorded source-path prefix to strip (lowercased, `\`-separated, trailing
    /// `\`), e.g. `c:\survarium\sources\`. Used to identify engine files and to
    /// build relative paths for the output tree.
    pub engine_paths: Vec<String>,
    /// Local directory the engine sources actually live in (base mode). When set,
    /// statement text is read from `source_root / <relative path>`; on a miss the
    /// statement keeps `source: None`.
    pub source_root: Option<PathBuf>,
    /// Target mode = original game, no sources: never attempt source reads.
    pub target_mode: bool,
    /// Retain only procedures whose primary line record belongs to an engine path.
    pub engine_only: bool,
}

pub fn extract_rich_context(
    pdb_path: &Path,
    exe_path: &Path,
    opts: &Options,
) -> crate::Result<Vec<FunctionEntry>> {
    // ── EXE: image base + .text bytes ────────────────────────────────────────
    let exe_bytes = std::fs::read(exe_path)?;
    let exe = object::read::pe::PeFile32::parse(exe_bytes.as_slice())?;
    let image_base = exe
        .nt_headers()
        .optional_header
        .image_base
        .get(LittleEndian) as u64;

    let Some(text) = exe.section_by_name(".text") else {
        return crate::error!("EXE has no .text section");
    };
    let text_rva = (text.address() - image_base) as usize;
    let text_data = text.data()?.to_vec();
    drop(exe);
    drop(exe_bytes);

    PdbParser::with(pdb_path, |fmt| {
        let file = std::fs::File::open(pdb_path)?;
        let mut pdb = PDB::open(file)?;

        let address_map = pdb.address_map()?;
        let string_table = pdb.string_table()?;

        let symbols = Rc::new(build_symbol_maps(&mut pdb, &address_map)?);

        let mut entries: Vec<FunctionEntry> = Vec::new();
        let mut source_cache: HashMap<String, Option<Vec<String>>> = HashMap::new();

        let dbi = pdb.debug_information()?;
        let mut modules = dbi.modules()?;
        let mut module_id: usize = usize::MAX;

        while let Some(module) = modules.next()? {
            module_id = module_id.wrapping_add(1);

            let Some(module_info) = pdb.module_info(&module)? else {
                continue;
            };
            let program = module_info.line_program()?;
            let mut syms = module_info.symbols()?;

            // Track the procedure whose scope we are inside, so its locals
            // (which follow the Procedure symbol) attach to the right entry.
            let mut current_entry: Option<usize> = None;
            let mut current_end = pdb::SymbolIndex(0);
            // Lexical-scope depth: 1 inside a procedure body, +1 per nested `{}`
            // block, back down on each scope-end. Used to mark block-opening
            // statements and to scope locals (mirrors gen_sources).
            let mut depth: i32 = 0;

            while let Some(sym) = syms.next()? {
                // Once we pass the current procedure's end symbol, its scope (and
                // any nested blocks) is closed.
                if current_entry.is_some() && sym.index() >= current_end {
                    current_entry = None;
                }

                match sym.parse() {
                    Ok(pdb::SymbolData::Procedure(proc)) => {
                        current_end = proc.end;
                        depth = 1; // a procedure opens the body scope

                        // Build the entry; `break 'build None` skips (non-.text,
                        // no line info, non-engine in tree mode, …) without losing
                        // scope tracking. `?` still propagates real PDB errors.
                        let built: Option<usize> = 'build: {
                            if proc.len == 0 {
                                break 'build None;
                            }
                            let Some(func_rva) = proc.offset.to_rva(&address_map) else {
                                break 'build None;
                            };
                            let func_rva = func_rva.0 as usize;
                            let size = proc.len as usize;
                            if func_rva < text_rva || func_rva + size > text_rva + text_data.len() {
                                break 'build None;
                            }

                            // ── statements: (rva, source-line) from line program ──
                            let mut stmts: Vec<(u32, u32, String, Option<String>)> = Vec::new();
                            let mut lines = program.lines_for_symbol(proc.offset);
                            while let Some(li) = lines.next()? {
                                let Some(rva) = li.offset.to_rva(&address_map) else {
                                    continue;
                                };
                                let fi = program.get_file_info(li.file_index)?;
                                let recorded = fi.name.to_string_lossy(&string_table)?.into_owned();
                                let lower = recorded.to_lowercase().replace('/', "\\");
                                let relative = opts
                                    .engine_paths
                                    .iter()
                                    .find_map(|prefix| lower.strip_prefix(prefix.as_str()))
                                    .map(str::to_string);
                                let file = relative.clone().unwrap_or(recorded).replace('\\', "/");
                                let source = match (&opts.source_root, &relative) {
                                    (Some(root), Some(rel)) if !opts.target_mode => {
                                        let source_lines =
                                            source_cache.entry(rel.clone()).or_insert_with(|| {
                                                let path = root.join(rel.replace('\\', "/"));
                                                std::fs::read_to_string(path).ok().map(|s| {
                                                    s.lines().map(str::to_string).collect()
                                                })
                                            });
                                        source_lines
                                            .as_ref()
                                            .and_then(|lines| {
                                                lines.get((li.line_start as usize).wrapping_sub(1))
                                            })
                                            .map(|s| strip_carcass_comment(s).to_string())
                                            .filter(|s| !s.is_empty())
                                    }
                                    _ => None,
                                };
                                stmts.push((rva.0, li.line_start, file, source));
                            }
                            if stmts.is_empty() {
                                break 'build None;
                            }
                            stmts.sort_by_key(|(rva, _, _, _)| *rva);
                            stmts.dedup_by_key(|(rva, _, _, _)| *rva);

                            let file = stmts.first().map(|s| s.2.clone()).unwrap_or_default();
                            let is_engine = !file.contains(':') && !file.starts_with('/');

                            if opts.engine_only && !is_engine {
                                break 'build None;
                            }

                            let proc_name = proc.name.to_string();

                            // Decorated name for the objdiff/.obj join; module
                            // symbols only give the undecorated form.
                            let mangled = symbols
                                .public_functions
                                .get(&func_rva)
                                .cloned()
                                .unwrap_or_else(|| proc_name.clone().into_owned());

                            // Static-init thunks (`??__E` / `??__F`) carry no
                            // Public symbol, so `mangled` falls back to the
                            // Procedure name: the raw mangled form on base, the
                            // demangled `` `dynamic initializer for 'X'' `` form on
                            // target. Canonicalize the base side to the target's
                            // demangled form so objdiff pairs the same thunk.
                            let canon_thunk =
                                crate::helpers::canonicalize_static_init_thunk(&mangled);
                            let mangled = canon_thunk.clone().unwrap_or(mangled);

                            let signature = match &canon_thunk {
                                Some(c) => c.clone(),
                                None => fmt
                                    .emit_function_orig(&proc.name, module_id, proc.type_index)
                                    .unwrap_or_else(|_| proc_name.into_owned()),
                            };

                            entries.push(build_function(
                                signature, mangled, &symbols, image_base, text_rva, &text_data,
                                func_rva, size, &stmts, file,
                            ));
                            Some(entries.len() - 1)
                        };
                        current_entry = built;
                    }

                    // Locals within the current procedure scope. Stack locals are
                    // negative-offset base-pointer-relative; register locals come
                    // through under optimization. Args (offset >= 0) are skipped —
                    // they are already in the signature.
                    Ok(pdb::SymbolData::BasePointerRelative(b)) if b.offset < 0 => {
                        push_local(
                            &mut entries,
                            current_entry,
                            &fmt,
                            module_id,
                            b.type_index,
                            b.name,
                            depth,
                        );
                    }
                    Ok(pdb::SymbolData::RegisterRelative(r)) => {
                        push_local(
                            &mut entries,
                            current_entry,
                            &fmt,
                            module_id,
                            r.type_index,
                            r.name,
                            depth,
                        );
                    }
                    Ok(pdb::SymbolData::RegisterVariable(v)) => {
                        push_local(
                            &mut entries,
                            current_entry,
                            &fmt,
                            module_id,
                            v.type_index,
                            v.name,
                            depth,
                        );
                    }

                    // A `{}` block opening at depth N: mark its opening statement
                    // (the one at the block's RVA) and descend. Blocks with no
                    // matching statement are dropped (no-address rows aren't shown).
                    Ok(pdb::SymbolData::Block(blk)) if depth >= 1 => {
                        if let (Some(ci), Some(rva)) =
                            (current_entry, blk.offset.to_rva(&address_map))
                        {
                            let off = rva.0.wrapping_sub(entries[ci].rva);
                            match entries[ci].statements.iter_mut().find(|s| s.off == off) {
                                Some(st) => st.depth = depth,
                                None => entries[ci].skipped_blocks.push((off, depth)),
                            }
                        }
                        depth += 1;
                    }
                    Ok(pdb::SymbolData::ScopeEnd) => {
                        depth = (depth - 1).max(0);
                    }
                    _ => {}
                }
            }
        }

        Ok(entries)
    })
}

/// Decode a function's bytes + merge in its statement boundaries, producing the
/// structured [`FunctionEntry`]. No rendering happens here.
#[allow(clippy::too_many_arguments)]
fn build_function(
    signature: String,
    mangled: String,
    symbols: &Rc<SymbolMaps>,
    image_base: u64,
    text_rva: usize,
    text_data: &[u8],
    func_rva: usize,
    size: usize,
    stmts: &[(u32, u32, String, Option<String>)],
    file: String,
) -> FunctionEntry {
    let off = func_rva - text_rva;
    let code = &text_data[off..off + size];
    let va_base = image_base + func_rva as u64;

    let decoded = disasm::decode(code, va_base);
    let mut formatter = disasm::make_formatter(image_base, decoded.labels.clone(), symbols.clone());

    let instructions = decoded
        .instructions
        .iter()
        .map(|insn| {
            let mut text = String::new();
            formatter.format(insn, &mut text);
            Instruction {
                off: (insn.ip() - va_base) as u32,
                len: insn.len() as u8,
                text,
                label: decoded.labels.get(&insn.ip()).cloned(),
            }
        })
        .collect();

    let func_size = size as u32;
    let func_rva32 = func_rva as u32;

    // Statement starts as offsets within the function. The first start is clamped
    // to 0 so the prologue is grouped under the opening statement.
    let mut starts: Vec<(u32, u32, String, Option<String>)> = stmts
        .iter()
        .filter(|(rva, _, _, _)| *rva >= func_rva32 && *rva - func_rva32 < func_size)
        .map(|(rva, line, file, source)| (rva - func_rva32, *line, file.clone(), source.clone()))
        .collect();
    if starts.is_empty() {
        starts.push((0, 0, file.clone(), None));
    }
    if let Some(first) = starts.first_mut() {
        first.0 = 0;
    }
    starts.sort_by_key(|(off, _, _, _)| *off);
    starts.dedup_by_key(|(off, _, _, _)| *off);

    let statements = (0..starts.len())
        .map(|g| {
            let (start_off, line, statement_file, source) = &starts[g];
            let end_off = starts
                .get(g + 1)
                .map(|(off, _, _, _)| *off)
                .unwrap_or(func_size);
            Statement {
                off: *start_off,
                size: end_off.saturating_sub(*start_off),
                line: *line,
                file: statement_file.clone(),
                depth: 0,
                source: source.clone(),
            }
        })
        .collect();

    FunctionEntry {
        name: signature,
        mangled,
        rva: func_rva as u32,
        image_base: image_base as u32,
        size: size as u32,
        file,
        statements,
        instructions,
        locals: Vec::new(),
        skipped_blocks: Vec::new(),
    }
}

/// Append a PDB local (`name: ty`) to the in-scope function entry, if any. Skips
/// the implicit `this`. Type formatting is best-effort (LTO loses some).
fn push_local(
    entries: &mut [FunctionEntry],
    current: Option<usize>,
    fmt: &PdbParser,
    module_id: usize,
    type_index: pdb::TypeIndex,
    name: pdb::RawString<'_>,
    depth: i32,
) {
    let Some(ci) = current else {
        return;
    };
    if name.as_bytes() == b"this" {
        return;
    }
    let ty = fmt
        .emit_type_impl(module_id, type_index)
        .unwrap_or_default();
    entries[ci].locals.push(Local {
        name: name.to_string().into_owned(),
        ty,
        // Function-body locals are depth 1 -> scope 0; each nested block adds one.
        scope: (depth - 1).max(0) as usize,
    });
}

/// Strip a trailing carcass annotation comment (`// <0x...>|...:'NNN'`) that the
/// vostok carcass stubs embed inline in source lines, leaving just the code.
/// Only the angle-bracket address marker `// <` is matched, so ordinary `//`
/// comments are untouched. Only the TRAILING whitespace (the padding before the
/// comment) is trimmed - the source's own leading indentation is preserved.
fn strip_carcass_comment(line: &str) -> &str {
    let cut = line.find("// <").unwrap_or(line.len());
    line[..cut].trim_end()
}

/// Build RVA -> name maps for call/data target annotation. Module symbols
/// (readable names) win; public (mangled) names fill gaps for library code.
fn build_symbol_maps(
    pdb: &mut PDB<'_, std::fs::File>,
    address_map: &pdb::AddressMap,
) -> crate::Result<SymbolMaps> {
    let mut functions: BTreeMap<usize, String> = BTreeMap::new();
    let mut data: BTreeMap<usize, String> = BTreeMap::new();
    let mut public_functions: BTreeMap<usize, String> = BTreeMap::new();

    {
        let dbi = pdb.debug_information()?;
        let mut modules = dbi.modules()?;
        while let Some(module) = modules.next()? {
            let Some(info) = pdb.module_info(&module)? else {
                continue;
            };
            let mut syms = info.symbols()?;
            while let Some(sym) = syms.next()? {
                match sym.parse() {
                    Ok(pdb::SymbolData::Procedure(p)) => {
                        if let Some(rva) = p.offset.to_rva(address_map) {
                            functions
                                .entry(rva.0 as usize)
                                .or_insert_with(|| p.name.to_string().into_owned());
                        }
                    }
                    Ok(pdb::SymbolData::Thunk(t)) => {
                        if let Some(rva) = t.offset.to_rva(address_map) {
                            functions
                                .entry(rva.0 as usize)
                                .or_insert_with(|| t.name.to_string().into_owned());
                        }
                    }
                    Ok(pdb::SymbolData::Data(d)) => {
                        if let Some(rva) = d.offset.to_rva(address_map) {
                            data.entry(rva.0 as usize)
                                .or_insert_with(|| d.name.to_string().into_owned());
                        }
                    }
                    _ => {}
                }
            }
        }
    }

    let global = pdb.global_symbols()?;
    let mut it = global.iter();
    while let Some(sym) = it.next()? {
        if let Ok(pdb::SymbolData::Public(p)) = sym.parse() {
            if let Some(rva) = p.offset.to_rva(address_map) {
                let rva = rva.0 as usize;
                if p.function {
                    let name = p.name.to_string().into_owned();
                    public_functions.entry(rva).or_insert_with(|| name.clone());
                    functions.entry(rva).or_insert(name);
                } else {
                    data.entry(rva)
                        .or_insert_with(|| p.name.to_string().into_owned());
                }
            }
        }
    }

    Ok(SymbolMaps {
        functions,
        data,
        public_functions,
    })
}
