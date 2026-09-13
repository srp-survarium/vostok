//! Cross-PDB structural divergence report.
//!
//! Given two PDBs — conventionally `base` (our compiled build) and `target`
//! (the original game) — this builds an owned, comparable model of each side
//! and reports where the two diverge:
//!
//! * **headers** (classes / structs / unions / enums, joined by qualified name):
//!   instance size, member layout (offset / type / name / order), and
//!   member-function declaration order.
//! * **sources** (`.cpp` compilands, joined by engine-relative path): the order
//!   functions are defined in and per-function constants (matched by `(type,
//!   value)`, so a renamed-but-equal constant is surfaced as a *misname* rather
//!   than an add/remove). Raw CodeView line-table entry counts can be compared
//!   explicitly, but are not semantic statement counts and are off by default.
//!
//! Nothing is written to disk; the whole thing is a read + compare + print.
//! Headers are extracted straight off the type stream (no source join needed),
//! while sources reuse [`gen_sources::for_each_function`] so the parsed model
//! stays identical to what the generator emits.

use std::collections::BTreeMap;
use std::collections::HashMap;
use std::collections::HashSet;
use std::path::Path;

use pdb::FallibleIterator;
use pdb::TypeData;

use crate::GenFlags;
use crate::Namespace;
use crate::gen_sources;
use crate::helpers::FunctionLocation;
use crate::helpers::canonicalize_static_init_thunk;
use crate::pdb_parser::PdbParser;

/// Library namespace roots whose types are skipped by default (overridable with
/// `include_external`). The engine's own source path filter already keeps the
/// `sources` side clean; this is purely to stop the `headers` side from drowning
/// in `std::`/`boost:: ` template instantiations that always match anyway.
const EXTERNAL_ROOTS: &[&str] = &[
    "std::",
    "stlp_std::",
    "boost::",
    "eastl::",
    "stdext::",
    "__",
];

pub struct Config {
    /// Lowercased substrings; a header (by qualified name) or source (by
    /// engine-relative path) matching any of these is skipped.
    pub skip: Vec<String>,
    /// Compare every type, including the [`EXTERNAL_ROOTS`] library namespaces.
    pub include_external: bool,
    pub do_headers: bool,
    pub do_sources: bool,
    /// List the names of one-sided (base-only / target-only) headers and files
    /// instead of only counting them.
    pub list_presence: bool,
    /// List the per-function out-of-line PRESENCE divergences (a function with a
    /// standalone out-of-line body in exactly one side's compilands) instead of
    /// only counting them. See [`report_presence_functions`].
    pub list_presence_fns: bool,
    /// Compare raw per-function CodeView line-table entry counts. These vary
    /// with optimization attribution and source-line packing, so they are a
    /// forensic diagnostic rather than an actionable structural divergence.
    pub compare_raw_line_table_counts: bool,
}

// ── Owned comparison model ──────────────────────────────────────────────────

#[derive(Default)]
struct SideModel {
    classes: BTreeMap<String, ClassModel>,
    enums: BTreeMap<String, EnumModel>,
    files: BTreeMap<String, FileModel>,
    presence_functions: Vec<PresenceFn>,
}

struct PresenceFn {
    path: String,
    key: String,
    name_orig: String,
}

struct ClassModel {
    kind: &'static str,
    size: u64,
    fields: Vec<FieldModel>,
    methods: Vec<MethodModel>,
}

struct FieldModel {
    offset: u64,
    type_name: String,
    name: String,
    // CV_access_t: 0=unspecified, 1=private, 2=protected, 3=public.
    access: u8,
}

struct MethodModel {
    /// The formatted signature (also the join key for the fn-order diff).
    sig: String,
    // CV_access_t (see FieldModel::access).
    access: u8,
}

struct EnumModel {
    underlying: String,
    values: Vec<(String, i64)>,
}

struct FileModel {
    functions: Vec<FnModel>,
}

struct FnModel {
    /// Cross-PDB join key (see [`function_join_key`]): the decorated COFF symbol
    /// when one exists, else a side-independent canonical form of `name_orig`.
    /// `key` is identical on both sides for the same logical function; `name_orig`
    /// is the demangled signature, kept for DISPLAY only.
    key: String,
    name_orig: String,
    definition_line: u32,
    module_id: usize,
    symbol_order: usize,
    raw_line_table_count: usize,
    constants: Vec<ConstModel>,
}

struct ConstModel {
    name: String,
    type_name: String,
    value: i64,
}

// ── Entry point ─────────────────────────────────────────────────────────────

pub fn run(
    base_pdb: &Path,
    base_engine: &str,
    target_pdb: &Path,
    target_engine: &str,
    cfg: &Config,
) -> crate::Result<()> {
    let base = extract_side(base_pdb, base_engine, cfg)?;
    let target = extract_side(target_pdb, target_engine, cfg)?;

    if cfg.do_headers {
        report_headers(&base, &target, cfg);
    }
    if cfg.do_sources {
        report_sources(&base, &target, cfg);
    }

    Ok(())
}

fn extract_side(pdb_path: &Path, engine: &str, cfg: &Config) -> crate::Result<SideModel> {
    let mut out = SideModel::default();

    PdbParser::with(pdb_path, |fmt| {
        let file = std::fs::File::open(pdb_path)?;
        let mut pdb = pdb::PDB::open(file)?;

        if cfg.do_headers {
            extract_headers(&mut pdb, &fmt, cfg, &mut out)?;
        }
        if cfg.do_sources {
            extract_sources(&mut pdb, &fmt, engine, cfg, &mut out)?;
        }

        Ok(())
    })?;

    Ok(out)
}

// ── Header extraction (off the type stream) ─────────────────────────────────

fn extract_headers(
    pdb: &mut pdb::PDB<std::fs::File>,
    fmt: &PdbParser,
    cfg: &Config,
    out: &mut SideModel,
) -> crate::Result<()> {
    let type_information = pdb.type_information()?;
    let type_finder = {
        let mut finder = type_information.finder();
        let mut iter = type_information.iter();
        while iter.next()?.is_some() {
            finder.update(&iter);
        }
        finder
    };

    let type_information2 = pdb.type_information()?;
    let mut type_iter = type_information2.iter();

    while let Some(item) = type_iter.next()? {
        match item.parse() {
            Ok(TypeData::Class(data)) if !data.properties.forward_reference() => {
                let qualified_name = data.name.to_string().to_string();
                if !include_type(&qualified_name, cfg) {
                    continue;
                }

                let namespace = Namespace::get_from_class_name_impl(&qualified_name);
                let mut class = ClassModel {
                    kind: kind_label(data.kind),
                    size: data.size,
                    fields: Vec::new(),
                    methods: Vec::new(),
                };

                if let Some(fields) = data.fields {
                    if walk_fields(&type_finder, fmt, &namespace, fields, &mut class).is_err() {
                        continue;
                    }
                }

                insert_class(out, qualified_name, class);
            }

            Ok(TypeData::Union(data))
                if !data.properties.forward_reference() && !data.properties.is_nested_type() =>
            {
                let qualified_name = data.name.to_string().to_string();
                if !include_type(&qualified_name, cfg) {
                    continue;
                }

                let namespace = Namespace::get_from_class_name_impl(&qualified_name);
                let mut class = ClassModel {
                    kind: "union",
                    size: data.size,
                    fields: Vec::new(),
                    methods: Vec::new(),
                };

                if data.count > 0
                    && walk_fields(&type_finder, fmt, &namespace, data.fields, &mut class).is_err()
                {
                    continue;
                }

                insert_class(out, qualified_name, class);
            }

            Ok(TypeData::Enumeration(data))
                if !data.properties.forward_reference() && !data.properties.is_nested_type() =>
            {
                let qualified_name = data.name.to_string().to_string();
                if !include_type(&qualified_name, cfg) {
                    continue;
                }

                let namespace = Namespace::get_from_class_name_impl(&qualified_name);
                let Ok(underlying) = fmt.emit_type(0, data.underlying_type, &namespace) else {
                    continue;
                };

                let mut e = EnumModel {
                    underlying: clean_type(underlying.0),
                    values: Vec::new(),
                };
                if walk_enum(&type_finder, data.fields, &mut e).is_err() {
                    continue;
                }

                insert_enum(out, qualified_name, e);
            }

            _ => continue,
        }
    }

    Ok(())
}

fn walk_fields(
    finder: &pdb::TypeFinder,
    fmt: &PdbParser,
    namespace: &Namespace,
    fields: pdb::TypeIndex,
    class: &mut ClassModel,
) -> crate::Result<()> {
    match finder.find(fields)?.parse()? {
        TypeData::FieldList(data) => {
            for field in &data.fields {
                handle_field(finder, fmt, namespace, field, class)?;
            }
            if let Some(continuation) = data.continuation {
                walk_fields(finder, fmt, namespace, continuation, class)?;
            }
        }
        _ => {}
    }
    Ok(())
}

fn handle_field(
    finder: &pdb::TypeFinder,
    fmt: &PdbParser,
    namespace: &Namespace,
    field: &TypeData,
    class: &mut ClassModel,
) -> crate::Result<()> {
    match *field {
        TypeData::Member(ref data) => {
            let type_name = clean_type(fmt.emit_type(0, data.field_type, namespace)?.0);
            class.fields.push(FieldModel {
                offset: data.offset,
                type_name,
                name: data.name.to_string().to_string(),
                access: data.attributes.access(),
            });
        }

        TypeData::BaseClass(ref data) => {
            let type_name = clean_type(fmt.emit_type(0, data.base_class, namespace)?.0);
            class.fields.push(FieldModel {
                offset: data.offset as u64,
                name: format!("<base> {type_name}"),
                type_name,
                access: data.attributes.access(),
            });
        }

        TypeData::VirtualBaseClass(ref data) => {
            let type_name = clean_type(fmt.emit_type(0, data.base_class, namespace)?.0);
            class.fields.push(FieldModel {
                offset: data.base_pointer_offset as u64,
                name: format!("<vbase> {type_name}"),
                type_name,
                access: data.attributes.access(),
            });
        }

        TypeData::Method(ref data) => {
            add_method(
                fmt,
                data.name,
                data.method_type,
                data.attributes.access(),
                class,
            )?;
        }

        TypeData::OverloadedMethod(ref data) => {
            if let TypeData::MethodList(method_list) = finder.find(data.method_list)?.parse()? {
                // LF_METHODLIST order is not declaration order and differs between
                // otherwise-equivalent incremental and retail PDBs. Keep the
                // overload group in a stable signature order while preserving the
                // group's position in the enclosing LF_FIELDLIST.
                let start = class.methods.len();
                for entry in method_list.methods {
                    add_method(
                        fmt,
                        data.name,
                        entry.method_type,
                        entry.attributes.access(),
                        class,
                    )?;
                }
                class.methods[start..].sort_by(|a, b| a.sig.cmp(&b.sig));
            }
        }

        _ => {}
    }

    Ok(())
}

fn add_method(
    fmt: &PdbParser,
    name: pdb::RawString,
    method_type: pdb::TypeIndex,
    access: u8,
    class: &mut ClassModel,
) -> crate::Result<()> {
    // Compiler-synthesised thunks (`__vecDelDtor`, vftable ctor closures, …) are
    // noise for a declaration-order comparison.
    if name.as_bytes().starts_with(b"__") {
        return Ok(());
    }
    class.methods.push(MethodModel {
        sig: clean_type(fmt.emit_function_orig(&name, 0, method_type)?),
        access,
    });
    Ok(())
}

fn walk_enum(
    finder: &pdb::TypeFinder,
    fields: pdb::TypeIndex,
    e: &mut EnumModel,
) -> crate::Result<()> {
    if let TypeData::FieldList(data) = finder.find(fields)?.parse()? {
        for field in &data.fields {
            if let TypeData::Enumerate(ref d) = *field {
                e.values
                    .push((d.name.to_string().to_string(), variant_to_i64(d.value)));
            }
        }
        if let Some(continuation) = data.continuation {
            walk_enum(finder, continuation, e)?;
        }
    }
    Ok(())
}

fn insert_class(out: &mut SideModel, name: String, class: ClassModel) {
    // Different compilands can emit compatible partial records for one class:
    // the layout is identical, but each record lists only the methods used by
    // that compiland. Union those method sets. Treat conflicting layouts as the
    // older richness case; they may be stale/incremental records and must not be
    // combined into a class that never existed.
    if let Some(existing) = out.classes.get_mut(&name) {
        if same_class_layout(existing, &class) {
            for incoming in class.methods {
                if let Some(current) = existing
                    .methods
                    .iter_mut()
                    .find(|method| method.sig == incoming.sig)
                {
                    if current.access == 0 {
                        current.access = incoming.access;
                    }
                } else {
                    existing.methods.push(incoming);
                }
            }
            for (current, incoming) in existing.fields.iter_mut().zip(class.fields) {
                if current.access == 0 {
                    current.access = incoming.access;
                }
            }
            canonicalize_overload_runs(&mut existing.methods);
            return;
        }

        if richness(existing) >= richness(&class) {
            return;
        }
    }
    out.classes.insert(name, class);
}

fn richness(class: &ClassModel) -> usize {
    class.fields.len() + class.methods.len()
}

fn same_class_layout(left: &ClassModel, right: &ClassModel) -> bool {
    left.kind == right.kind
        && left.size == right.size
        && left.fields.len() == right.fields.len()
        && left.fields.iter().zip(&right.fields).all(|(left, right)| {
            left.offset == right.offset
                && left.type_name == right.type_name
                && left.name == right.name
        })
}

fn canonicalize_overload_runs(methods: &mut [MethodModel]) {
    let mut start = 0;
    while start < methods.len() {
        let family = method_family(&methods[start].sig).to_string();
        let mut end = start + 1;
        while end < methods.len() && method_family(&methods[end].sig) == family {
            end += 1;
        }
        if end - start > 1 {
            methods[start..end].sort_by(|left, right| left.sig.cmp(&right.sig));
        }
        start = end;
    }
}

fn method_family(signature: &str) -> &str {
    let end = signature
        .find("operator()(")
        .map(|position| position + "operator()".len())
        .or_else(|| signature.find('('))
        .unwrap_or(signature.len());
    signature[..end]
        .rsplit(' ')
        .next()
        .unwrap_or(&signature[..end])
}

fn insert_enum(out: &mut SideModel, name: String, e: EnumModel) {
    let name = if name.ends_with("::<unnamed-tag>") {
        let mut enumerators: Vec<&str> = e.values.iter().map(|(name, _)| name.as_str()).collect();
        enumerators.sort_unstable();
        format!("{name}#{}", enumerators.join("|"))
    } else {
        name
    };

    match out.enums.get(&name) {
        Some(existing) if existing.values.len() >= e.values.len() => {}
        _ => {
            out.enums.insert(name, e);
        }
    }
}

// ── Source extraction (reuse the generator's parse) ─────────────────────────

fn extract_sources(
    pdb: &mut pdb::PDB<std::fs::File>,
    fmt: &PdbParser,
    engine: &str,
    cfg: &Config,
    out: &mut SideModel,
) -> crate::Result<()> {
    // RVA -> decorated (mangled) COFF symbol, from function Public symbols. This
    // is the cross-PDB join key: the same logical function carries an identical
    // mangled symbol in both PDBs, whereas its DEMANGLED signature can differ
    // between the two (each PDB's demangler renders template args differently —
    // base drops the `enum`/`class`/`struct` keyword, target keeps it). Built
    // before the source walk because both borrow `pdb`.
    let public = public_function_symbols(pdb)?;

    gen_sources::for_each_function(pdb, fmt, GenFlags::empty(), |filename, fun| {
        let is_source = matches!(FunctionLocation::get(filename), FunctionLocation::Source);
        let lowered = filename.to_lowercase().replace('/', "\\");
        let Some(relative) = lowered.strip_prefix(engine) else {
            return;
        };
        let relative = relative.trim_start_matches('\\').replace('\\', "/");
        if skipped(&relative, cfg) {
            return;
        }

        let mangled = public.get(&fun.offset.0).map(String::as_str);
        let key = function_join_key(mangled, &fun.name_orig);
        out.presence_functions.push(PresenceFn {
            path: relative.clone(),
            key: key.clone(),
            name_orig: fun.name_orig.clone(),
        });

        // Header procedures participate in whole-PDB symbol presence above,
        // but only real source compilands carry comparable definition order.
        if !is_source {
            return;
        }

        let constants = fun
            .constants
            .iter()
            .map(|(name, type_name, value)| ConstModel {
                name: name.to_string().to_string(),
                type_name: clean_type(type_name.0.clone()),
                value: variant_to_i64(*value),
            })
            .collect();

        out.files
            .entry(relative)
            .or_insert_with(|| FileModel {
                functions: Vec::new(),
            })
            .functions
            .push(FnModel {
                key,
                name_orig: fun.name_orig.clone(),
                definition_line: if fun.definition_line_is_reliable() {
                    fun.proc_start
                } else {
                    0
                },
                module_id: fun.module_id,
                symbol_order: fun.symbol_order,
                raw_line_table_count: fun.statements.len(),
                constants,
            });
    })?;

    for file in out.files.values_mut() {
        file.functions.sort_by_key(|fun| fun.definition_line);
    }
    Ok(())
}

/// Build the `RVA -> decorated (mangled) symbol` map for *functions*, from the
/// global stream's Public symbols. Module Procedure symbols carry only the
/// undecorated `ns::func` form, so the decorated COFF name — the side-stable
/// join key — comes from here.
fn public_function_symbols(
    pdb: &mut pdb::PDB<std::fs::File>,
) -> crate::Result<HashMap<u32, String>> {
    let address_map = pdb.address_map()?;
    let global = pdb.global_symbols()?;

    let mut map = HashMap::new();
    let mut it = global.iter();
    while let Some(sym) = it.next()? {
        if let Ok(pdb::SymbolData::Public(p)) = sym.parse() {
            if p.function {
                if let Some(rva) = p.offset.to_rva(&address_map) {
                    map.entry(rva.0)
                        .or_insert_with(|| p.name.to_string().into_owned());
                }
            }
        }
    }
    Ok(map)
}

/// The cross-PDB join key for one source function.
///
/// * When the function has a Public (decorated) symbol, use it verbatim — it is
///   byte-identical across the two PDBs (the demangled signature is not).
/// * Otherwise it is a compiler-generated static-init/atexit thunk (`??__E` /
///   `??__F`), which carries no Public symbol and which the two PDBs render
///   differently in `name_orig`: the base keeps the raw mangled `??__E…` form,
///   the target stores a demangled `` `dynamic initializer for 'X'' `` form (and
///   even the target's own rendering varies — a namespace-scope static comes out
///   `` ns::`dynamic initializer for 'leaf'' `` while a class static comes out
///   `` `dynamic initializer for 'qualified::leaf'' `` with no outer prefix).
///   Normalize every variant to a single `kind|fully-qualified-var` key so the
///   same thunk keys identically on both sides. A non-thunk without a Public
///   symbol falls back to `name_orig` unchanged.
fn function_join_key(mangled: Option<&str>, name_orig: &str) -> String {
    if let Some(m) = mangled {
        return m.to_string();
    }
    canonicalize_thunk_name_orig(name_orig).unwrap_or_else(|| name_orig.to_string())
}

/// Normalize a static-init/atexit thunk's `name_orig` to a side-independent
/// `kind|fully-qualified-var` key (e.g. `dynamic initializer for|vostok::core::s_x`).
/// Handles all three renderings the two PDBs produce for the same thunk:
///
/// * base mangled — `void ??__E…YAXXZ()`
/// * target namespace-scope — `` void ns::`dynamic initializer for 'leaf''() ``
/// * target class-scope — `` void `dynamic initializer for 'qualified::leaf''() ``
///
/// Returns `None` for anything that is not a `??__E`/`??__F` thunk.
fn canonicalize_thunk_name_orig(name_orig: &str) -> Option<String> {
    // `emit_function_orig` wraps the symbol as `void <sym>()`; strip that first.
    let inner = name_orig.strip_prefix("void ")?.strip_suffix("()")?;

    // Base side: reuse the shared mangled→demangled canonicalizer, then reduce its
    // `` `dynamic initializer for 'FQ'' `` output to the `kind|FQ` key.
    if let Some(demangled) = canonicalize_static_init_thunk(inner) {
        return thunk_kind_var_key(&demangled);
    }
    // Target side: already demangled. Split off any `ns::` prefix that sits OUTSIDE
    // the backtick form and fold it back into the fully-qualified variable.
    let (prefix, tail) = match inner.find('`') {
        Some(pos) => (&inner[..pos], &inner[pos..]),
        None => ("", inner),
    };
    let (kind, var) = split_thunk_kind_var(tail)?;
    Some(format!("{kind}|{prefix}{var}"))
}

/// From the demangled `` `dynamic initializer for 'FQ'' `` (no outer prefix),
/// derive the `kind|FQ` key.
fn thunk_kind_var_key(demangled: &str) -> Option<String> {
    let (kind, var) = split_thunk_kind_var(demangled)?;
    Some(format!("{kind}|{var}"))
}

/// Parse a `` `dynamic initializer for 'VAR'' `` / `` `dynamic atexit destructor
/// for 'VAR'' `` token into `(kind, VAR)`.
fn split_thunk_kind_var(s: &str) -> Option<(&'static str, &str)> {
    for kind in ["dynamic initializer for", "dynamic atexit destructor for"] {
        if let Some(rest) = s.strip_prefix(&format!("`{kind} '")) {
            let var = rest.strip_suffix("''")?;
            return Some((kind, var));
        }
    }
    None
}

// ── Header reporting ────────────────────────────────────────────────────────

fn report_headers(base: &SideModel, target: &SideModel, cfg: &Config) {
    println!("================ HEADERS ================\n");

    let mut counts = HeaderCounts::default();

    let keys = union_keys(base.classes.keys(), target.classes.keys());
    for name in keys {
        match (base.classes.get(name), target.classes.get(name)) {
            (Some(b), Some(t)) => {
                counts.both += 1;
                if diff_class(name, b, t, &mut counts) {
                    counts.diverged += 1;
                }
            }
            (Some(_), None) => counts.only_base.push(name.clone()),
            (None, Some(_)) => counts.only_target.push(name.clone()),
            (None, None) => unreachable!(),
        }
    }

    let enum_keys = union_keys(base.enums.keys(), target.enums.keys());
    for name in enum_keys {
        match (base.enums.get(name), target.enums.get(name)) {
            (Some(b), Some(t)) => {
                counts.enums_both += 1;
                if diff_enum(name, b, t) {
                    counts.enums_diverged += 1;
                }
            }
            (Some(_), None) => counts.enum_only_base.push(name.clone()),
            (None, Some(_)) => counts.enum_only_target.push(name.clone()),
            (None, None) => unreachable!(),
        }
    }

    report_presence(
        "classes/structs/unions",
        &counts.only_base,
        &counts.only_target,
        cfg,
    );
    report_presence(
        "enums",
        &counts.enum_only_base,
        &counts.enum_only_target,
        cfg,
    );

    println!("---- header summary ----");
    println!(
        "types:  {both} compared, {div} diverged ({size} size, {member} member, {order} fn-order, \
         {vis} visibility); base-only {ob}, target-only {ot}",
        both = counts.both,
        div = counts.diverged,
        size = counts.size_diff,
        member = counts.member_diff,
        order = counts.order_diff,
        vis = counts.visibility_diff,
        ob = counts.only_base.len(),
        ot = counts.only_target.len(),
    );
    println!(
        "enums:  {both} compared, {div} diverged; base-only {ob}, target-only {ot}\n",
        both = counts.enums_both,
        div = counts.enums_diverged,
        ob = counts.enum_only_base.len(),
        ot = counts.enum_only_target.len(),
    );
}

/// Returns `true` if `b` and `t` diverge in any way.
fn diff_class(name: &str, b: &ClassModel, t: &ClassModel, counts: &mut HeaderCounts) -> bool {
    let mut lines: Vec<String> = Vec::new();

    if b.size != t.size {
        counts.size_diff += 1;
        lines.push(format!(
            "  [size]   base=0x{:X}  target=0x{:X}",
            b.size, t.size
        ));
    }

    let fields = diff_fields(&b.fields, &t.fields);
    if fields.diverged() {
        counts.member_diff += 1;
        lines.push("  [member]".to_string());
        for (n, bf, tf) in &fields.changed {
            lines.push(format!(
                "    changed  {n}: base({} @0x{:X})  target({} @0x{:X})",
                bf.0, bf.1, tf.0, tf.1
            ));
        }
        push_list(&mut lines, "    only-base  ", &fields.only_base);
        push_list(&mut lines, "    only-tgt   ", &fields.only_target);
        push_list(&mut lines, "    reordered  ", &fields.moved);
    }

    let base_sigs: Vec<String> = b.methods.iter().map(|m| m.sig.clone()).collect();
    let target_sigs: Vec<String> = t.methods.iter().map(|m| m.sig.clone()).collect();
    let methods = seq_diff(&base_sigs, &target_sigs);
    if methods.diverged() {
        counts.order_diff += 1;
        lines.push("  [fn-order]".to_string());
        push_list(&mut lines, "    only-base  ", &methods.only_base);
        push_list(&mut lines, "    only-tgt   ", &methods.only_target);
        push_list(&mut lines, "    moved      ", &methods.moved);
    }

    // Access (visibility) only applies to members present on BOTH sides, joined
    // by name (data) / signature (methods). A one-sided member is already
    // surfaced by [member]/[fn-order] above, so we never double-report it here.
    let vis = diff_visibility(b, t, &fields, &methods);
    if !vis.is_empty() {
        counts.visibility_diff += vis.len();
        lines.push("  [visibility]".to_string());
        for (member, base_access, tgt_access) in &vis {
            lines.push(format!(
                "    {member}: base={} tgt={}",
                access_name(*base_access),
                access_name(*tgt_access),
            ));
        }
    }

    if lines.is_empty() {
        return false;
    }

    println!("{kind} {name}", kind = b.kind);
    for line in lines {
        println!("{line}");
    }
    println!();
    true
}

fn diff_enum(name: &str, b: &EnumModel, t: &EnumModel) -> bool {
    let mut lines: Vec<String> = Vec::new();

    if b.underlying != t.underlying {
        lines.push(format!(
            "  [underlying]  base={}  target={}",
            b.underlying, t.underlying
        ));
    }

    let base_vals: Vec<((String, i64), String)> = b
        .values
        .iter()
        .map(|(n, v)| (("".to_string(), *v), n.clone()))
        .collect();
    let target_vals: Vec<((String, i64), String)> = t
        .values
        .iter()
        .map(|(n, v)| (("".to_string(), *v), n.clone()))
        .collect();
    let vals = value_match(&base_vals, &target_vals);

    if vals.diverged() {
        lines.push("  [values]".to_string());
        for (key, bn, tn) in &vals.misnamed {
            lines.push(format!(
                "    misname  0x{:X}  base={bn}  target={tn}",
                key.1
            ));
        }
        for (key, n) in &vals.only_base {
            lines.push(format!("    only-base  {n} = 0x{:X}", key.1));
        }
        for (key, n) in &vals.only_target {
            lines.push(format!("    only-tgt   {n} = 0x{:X}", key.1));
        }
    }

    if lines.is_empty() {
        return false;
    }

    println!("enum {name}");
    for line in lines {
        println!("{line}");
    }
    println!();
    true
}

// ── Source reporting ────────────────────────────────────────────────────────

fn report_sources(base: &SideModel, target: &SideModel, cfg: &Config) {
    println!("================ SOURCES ================\n");

    let mut counts = SourceCounts::default();

    let keys = union_keys(base.files.keys(), target.files.keys());
    for path in keys {
        match (base.files.get(path), target.files.get(path)) {
            (Some(b), Some(t)) => {
                counts.both += 1;
                if diff_file(path, b, t, cfg, &mut counts) {
                    counts.diverged += 1;
                }
            }
            (Some(_), None) => counts.only_base.push(path.clone()),
            (None, Some(_)) => counts.only_target.push(path.clone()),
            (None, None) => unreachable!(),
        }
    }

    report_presence("files", &counts.only_base, &counts.only_target, cfg);

    let presence = report_presence_functions(base, target, cfg);

    println!("---- source summary ----");
    println!(
        "files:  {both} compared, {div} diverged; base-only {ob}, target-only {ot}",
        both = counts.both,
        div = counts.diverged,
        ob = counts.only_base.len(),
        ot = counts.only_target.len(),
    );
    if cfg.compare_raw_line_table_counts {
        println!(
            "        {order} files w/ fn-order diff, {lines} functions w/ raw line-table-count \
             diff, {cst} functions w/ const diff",
            order = counts.order_diff,
            lines = counts.raw_line_table_diff,
            cst = counts.const_diff,
        );
    } else {
        println!(
            "        {order} files w/ fn-order diff, {cst} functions w/ const diff",
            order = counts.order_diff,
            cst = counts.const_diff,
        );
    }
    println!(
        "        out-of-line presence: {pb} base-only (we emit standalone; target inlines), \
         {pt} target-only (target emits standalone; we inline / no source)\n",
        pb = presence.0,
        pt = presence.1,
    );
}

/// Per-function out-of-line PRESENCE divergence, across the whole source corpus.
///
/// `extract_sources` collected, per side, the set of functions that have a
/// standalone out-of-line body (a real code symbol / compiland definition with
/// statements). Joining those two sets by `(engine-relative path, join key)` —
/// the same cross-PDB key the `[line-table]`/`[const]`/`[fn-order]` diffs use (the
/// decorated COFF symbol, or the canonical thunk form; see
/// [`function_join_key`]) — a function present in exactly one side's set is an
/// out-of-line presence divergence:
///
/// * **base-only**: we emit the function out-of-line but the target inlines it
///   (no standalone body there) — a noinline/forceinline decision.
/// * **tgt-only**: the target emits the function out-of-line but our base
///   inlines it / it is a `/* no source */` (inlined-only) function — a real
///   reconstruction target: the target's standalone symbol gives us a body.
///
/// This is deliberately distinct from `[fn-order]`: that reports the relative
/// definition ORDER of functions present on *both* sides within a matched file
/// (mirroring the header `[fn-order]` decl-order check), so the one-sided lists
/// are owned here and stripped from the source `[fn-order]` to avoid
/// double-reporting. Returns `(base_only_count, target_only_count)`.
///
/// The join key is the decorated COFF symbol (identical across both PDBs), so a
/// function the two PDBs merely DEMANGLE differently — template args rendered
/// with vs without the `enum`/`class`/`struct` keyword, or the static-init
/// thunks (`??__E…` mangled on base vs `` `dynamic initializer for '…'' ``
/// demangled on target) — pairs cleanly and is no longer reported here. The
/// surplus entries (genuine reconstruction targets) are still displayed with
/// their readable demangled `name_orig`.
fn report_presence_functions(base: &SideModel, target: &SideModel, cfg: &Config) -> (usize, usize) {
    let presence = presence_functions(base, target);

    if cfg.list_presence_fns && (!presence.base_only.is_empty() || !presence.target_only.is_empty())
    {
        if !presence.base_only.is_empty() {
            println!(
                "---- base-only out-of-line functions ({}) ----",
                presence.base_only.len()
            );
            for (path, sig) in &presence.base_only {
                println!("  [presence] base-only  {path}  {sig}");
            }
        }
        if !presence.target_only.is_empty() {
            println!(
                "---- target-only out-of-line functions ({}) ----",
                presence.target_only.len()
            );
            for (path, sig) in &presence.target_only {
                println!("  [presence] tgt-only   {path}  {sig}");
            }
        }
        println!();
    }

    (presence.base_only.len(), presence.target_only.len())
}

struct PresenceDiff {
    base_only: Vec<(String, String)>,
    target_only: Vec<(String, String)>,
}

/// Build the `(path, signature)` sets of functions present out-of-line on
/// exactly one side. Functions are JOINED by their cross-PDB key (the decorated
/// COFF symbol / canonical thunk form — see [`function_join_key`]), so the same
/// logical function never shows up one-sided just because the two PDBs demangle
/// its signature differently; the readable demangled `name_orig` is what gets
/// reported for the surplus entries. Exact full signatures are a secondary join
/// for anonymous/local symbols and functions without a Public record. Presence
/// is compared across the whole PDB: ICF can attribute the same linked body to a
/// different source file on each side, which is not one-sided symbol presence.
/// Repeated module procedure observations of one function likewise still
/// describe one linked out-of-line body and must not create a multiplicity
/// divergence.
fn presence_functions(base: &SideModel, target: &SideModel) -> PresenceDiff {
    let mut base_only = Vec::new();
    let mut target_only = Vec::new();

    let base_fns = &base.presence_functions;
    let target_fns = &target.presence_functions;
    let target_keys: HashSet<&str> = target_fns.iter().map(|fun| fun.key.as_str()).collect();
    let target_names: HashSet<&str> = target_fns
        .iter()
        .map(|fun| fun.name_orig.as_str())
        .collect();
    let base_keys: HashSet<&str> = base_fns.iter().map(|fun| fun.key.as_str()).collect();
    let base_names: HashSet<&str> = base_fns.iter().map(|fun| fun.name_orig.as_str()).collect();

    let mut reported = HashSet::new();
    for fun in base_fns {
        if !target_keys.contains(fun.key.as_str())
            && !target_names.contains(fun.name_orig.as_str())
            && reported.insert(fun.name_orig.as_str())
        {
            base_only.push((fun.path.clone(), fun.name_orig.clone()));
        }
    }

    let mut reported = HashSet::new();
    for fun in target_fns {
        if !base_keys.contains(fun.key.as_str())
            && !base_names.contains(fun.name_orig.as_str())
            && reported.insert(fun.name_orig.as_str())
        {
            target_only.push((fun.path.clone(), fun.name_orig.clone()));
        }
    }

    PresenceDiff {
        base_only,
        target_only,
    }
}

fn cross_pdb_function_key(fun: &FnModel, other_names: &HashSet<&str>) -> String {
    if other_names.contains(fun.name_orig.as_str()) {
        format!("name|{}", fun.name_orig)
    } else {
        format!("symbol|{}", fun.key)
    }
}

fn function_lines(file: &FileModel, other: &FileModel) -> HashMap<String, u32> {
    let other_names: HashSet<&str> = other
        .functions
        .iter()
        .map(|fun| fun.name_orig.as_str())
        .collect();
    let mut lines: HashMap<String, Option<u32>> = HashMap::new();
    for fun in &file.functions {
        if fun.definition_line == 0 {
            continue;
        }
        let key = cross_pdb_function_key(fun, &other_names);
        lines
            .entry(key)
            .and_modify(|line| {
                if *line != Some(fun.definition_line) {
                    *line = None;
                }
            })
            .or_insert(Some(fun.definition_line));
    }
    lines
        .into_iter()
        .filter_map(|(key, line)| line.map(|line| (key, line)))
        .collect()
}

fn function_symbol_positions(
    file: &FileModel,
    other: &FileModel,
) -> HashMap<String, (usize, usize)> {
    let other_names: HashSet<&str> = other
        .functions
        .iter()
        .map(|fun| fun.name_orig.as_str())
        .collect();
    let mut positions: HashMap<String, Option<(usize, usize)>> = HashMap::new();
    for fun in &file.functions {
        if fun.definition_line == 0 {
            continue;
        }
        let key = cross_pdb_function_key(fun, &other_names);
        let position = (fun.module_id, fun.symbol_order);
        positions
            .entry(key)
            .and_modify(|existing| {
                if *existing != Some(position) {
                    *existing = None;
                }
            })
            .or_insert(Some(position));
    }
    positions
        .into_iter()
        .filter_map(|(key, position)| position.map(|position| (key, position)))
        .collect()
}

fn function_order_moved(base: &FileModel, target: &FileModel) -> Vec<String> {
    let base_lines = function_lines(base, target);
    let target_lines = function_lines(target, base);
    let base_symbols = function_symbol_positions(base, target);
    let target_symbols = function_symbol_positions(target, base);
    let mut common: Vec<&String> = base_lines
        .keys()
        .filter(|key| target_lines.contains_key(*key))
        .collect();
    common.sort_unstable();

    let mut moved = HashSet::new();
    for (index, left) in common.iter().enumerate() {
        for right in &common[index + 1..] {
            let base_left = base_lines[*left];
            let base_right = base_lines[*right];
            let target_left = target_lines[*left];
            let target_right = target_lines[*right];
            if base_left == base_right || target_left == target_right {
                continue;
            }
            let Some(&(base_left_module, base_left_symbol)) = base_symbols.get(*left) else {
                continue;
            };
            let Some(&(base_right_module, base_right_symbol)) = base_symbols.get(*right) else {
                continue;
            };
            let Some(&(target_left_module, target_left_symbol)) = target_symbols.get(*left) else {
                continue;
            };
            let Some(&(target_right_module, target_right_symbol)) = target_symbols.get(*right)
            else {
                continue;
            };
            if base_left_module != base_right_module || target_left_module != target_right_module {
                continue;
            }
            let line_inverted = (base_left < base_right) != (target_left < target_right);
            let symbol_inverted = (base_left_symbol < base_right_symbol)
                != (target_left_symbol < target_right_symbol);
            if line_inverted && symbol_inverted {
                moved.insert((*left).clone());
                moved.insert((*right).clone());
            }
        }
    }

    let mut moved: Vec<String> = moved.into_iter().collect();
    moved.sort_unstable_by(|left, right| {
        base_lines[left]
            .cmp(&base_lines[right])
            .then_with(|| left.cmp(right))
    });
    moved
}

fn diff_file(
    path: &str,
    b: &FileModel,
    t: &FileModel,
    cfg: &Config,
    counts: &mut SourceCounts,
) -> bool {
    let mut lines: Vec<String> = Vec::new();

    // [fn-order] reports only the relative DEFINITION ORDER of functions present
    // out-of-line on BOTH sides (the `moved` set). Functions present out-of-line
    // on exactly one side are an out-of-line PRESENCE divergence, owned by the
    // global [presence] report (report_presence_functions) so we never
    // double-report a one-sided body here. Order is joined by the cross-PDB key
    // (so a demangle-only difference never reads as a reorder). Exact full
    // signatures pair functions whose local/anonymous decorated names are not
    // stable. An inversion is reportable only when both functions have distinct
    // attributed lines in both PDBs AND the compiland procedure-symbol order
    // independently inverts. This rejects non-monotonic `#line` mappings without
    // trusting linker-global order. Same-line, line-zero, cross-module, and
    // ambiguous records provide no relative source-order evidence.
    let order_moved = function_order_moved(b, t);
    if !order_moved.is_empty() {
        counts.order_diff += 1;
        let target_names: HashSet<&str> =
            t.functions.iter().map(|f| f.name_orig.as_str()).collect();
        let display: HashMap<String, &str> = b
            .functions
            .iter()
            .map(|f| {
                (
                    cross_pdb_function_key(f, &target_names),
                    f.name_orig.as_str(),
                )
            })
            .collect();
        let moved: Vec<String> = order_moved
            .iter()
            .map(|k| display.get(k).copied().unwrap_or(k).to_string())
            .collect();
        let base_lines = function_lines(b, t);
        let target_lines = function_lines(t, b);
        let base_order = ordered_function_display(&order_moved, &base_lines, &display);
        let target_order = ordered_function_display(&order_moved, &target_lines, &display);
        lines.push("  [fn-order]".to_string());
        push_list(&mut lines, "    moved      ", &moved);
        push_list(&mut lines, "    base order ", &base_order);
        push_list(&mut lines, "    tgt order  ", &target_order);
    }

    // Per-function raw line-table/const comparison over functions present on
    // both sides, joined by the cross-PDB key. The raw count is deliberately
    // opt-in: CodeView line entries reflect source-line packing and optimized
    // attribution, not the number of semantic C++ statements.
    let target_by_key: HashMap<&str, &FnModel> =
        t.functions.iter().map(|f| (f.key.as_str(), f)).collect();
    let target_by_name: HashMap<&str, &FnModel> = t
        .functions
        .iter()
        .map(|f| (f.name_orig.as_str(), f))
        .collect();

    for bf in &b.functions {
        let Some(tf) = target_by_name
            .get(bf.name_orig.as_str())
            .or_else(|| target_by_key.get(bf.key.as_str()))
        else {
            continue;
        };

        if cfg.compare_raw_line_table_counts && bf.raw_line_table_count != tf.raw_line_table_count {
            counts.raw_line_table_diff += 1;
            lines.push(format!(
                "  [line-table]  {}: base={} target={}",
                bf.name_orig, bf.raw_line_table_count, tf.raw_line_table_count
            ));
        }

        let base_consts: Vec<((String, i64), String)> = bf
            .constants
            .iter()
            .map(|c| ((c.type_name.clone(), c.value), c.name.clone()))
            .collect();
        let target_consts: Vec<((String, i64), String)> = tf
            .constants
            .iter()
            .map(|c| ((c.type_name.clone(), c.value), c.name.clone()))
            .collect();
        let consts = value_match(&base_consts, &target_consts);

        if consts.diverged() {
            counts.const_diff += 1;
            lines.push(format!("  [const]  {}", bf.name_orig));
            for (key, bn, tn) in &consts.misnamed {
                lines.push(format!(
                    "    misname  {} = {}  base={bn}  target={tn}",
                    key.0, key.1
                ));
            }
            for (key, n) in &consts.only_base {
                lines.push(format!("    only-base  {} {} = {}", key.0, n, key.1));
            }
            for (key, n) in &consts.only_target {
                lines.push(format!("    only-tgt   {} {} = {}", key.0, n, key.1));
            }
        }
    }

    if lines.is_empty() {
        return false;
    }

    println!("{path}");
    for line in lines {
        println!("{line}");
    }
    println!();
    true
}

fn ordered_function_display(
    keys: &[String],
    source_lines: &HashMap<String, u32>,
    display: &HashMap<String, &str>,
) -> Vec<String> {
    let mut ordered = keys.to_vec();
    ordered.sort_unstable_by(|left, right| {
        source_lines[left]
            .cmp(&source_lines[right])
            .then_with(|| left.cmp(right))
    });
    ordered
        .into_iter()
        .map(|key| {
            format!(
                "line {}: {}",
                source_lines[&key],
                display.get(&key).copied().unwrap_or(&key)
            )
        })
        .collect()
}

// ── Diff primitives ─────────────────────────────────────────────────────────

struct SeqDiff {
    only_base: Vec<String>,
    only_target: Vec<String>,
    moved: Vec<String>,
}

impl SeqDiff {
    fn diverged(&self) -> bool {
        !self.only_base.is_empty() || !self.only_target.is_empty() || !self.moved.is_empty()
    }
}

/// Compare two ordered key sequences. One-sided keys are reported as presence
/// diffs; among the keys common to both, those not on a longest common
/// subsequence are reported as `moved` (i.e. their relative order changed).
fn seq_diff(base: &[String], target: &[String]) -> SeqDiff {
    let bset: HashSet<&String> = base.iter().collect();
    let tset: HashSet<&String> = target.iter().collect();

    let only_base = dedup_filter(base, |k| !tset.contains(k));
    let only_target = dedup_filter(target, |k| !bset.contains(k));

    let common_base: Vec<&String> = base.iter().filter(|k| tset.contains(*k)).collect();
    let common_target: Vec<&String> = target.iter().filter(|k| bset.contains(*k)).collect();

    let lcs = lcs_set(&common_base, &common_target);
    let mut seen = HashSet::new();
    let mut moved = Vec::new();
    for k in &common_base {
        if !lcs.contains(*k) && seen.insert((*k).clone()) {
            moved.push((*k).clone());
        }
    }

    SeqDiff {
        only_base,
        only_target,
        moved,
    }
}

fn lcs_set(a: &[&String], b: &[&String]) -> HashSet<String> {
    let n = a.len();
    let m = b.len();
    let mut dp = vec![vec![0u32; m + 1]; n + 1];
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            dp[i][j] = if a[i] == b[j] {
                dp[i + 1][j + 1] + 1
            } else {
                dp[i + 1][j].max(dp[i][j + 1])
            };
        }
    }

    let mut set = HashSet::new();
    let (mut i, mut j) = (0, 0);
    while i < n && j < m {
        if a[i] == b[j] {
            set.insert(a[i].clone());
            i += 1;
            j += 1;
        } else if dp[i + 1][j] >= dp[i][j + 1] {
            i += 1;
        } else {
            j += 1;
        }
    }
    set
}

struct FieldDiff {
    changed: Vec<(String, (String, u64), (String, u64))>,
    only_base: Vec<String>,
    only_target: Vec<String>,
    moved: Vec<String>,
}

impl FieldDiff {
    fn diverged(&self) -> bool {
        !self.changed.is_empty()
            || !self.only_base.is_empty()
            || !self.only_target.is_empty()
            || !self.moved.is_empty()
    }
}

fn diff_fields(base: &[FieldModel], target: &[FieldModel]) -> FieldDiff {
    let base_names: Vec<String> = base.iter().map(|f| f.name.clone()).collect();
    let target_names: Vec<String> = target.iter().map(|f| f.name.clone()).collect();
    let order = seq_diff(&base_names, &target_names);

    let target_by_name: HashMap<&str, &FieldModel> =
        target.iter().map(|f| (f.name.as_str(), f)).collect();

    let mut changed = Vec::new();
    let mut seen = HashSet::new();
    for bf in base {
        if !seen.insert(bf.name.clone()) {
            continue;
        }
        if let Some(tf) = target_by_name.get(bf.name.as_str()) {
            if bf.type_name != tf.type_name || bf.offset != tf.offset {
                changed.push((
                    bf.name.clone(),
                    (bf.type_name.clone(), bf.offset),
                    (tf.type_name.clone(), tf.offset),
                ));
            }
        }
    }

    FieldDiff {
        changed,
        only_base: order.only_base,
        only_target: order.only_target,
        moved: order.moved,
    }
}

/// Access mismatches for members present on BOTH sides. Data members join by
/// name, methods by formatted signature; the join keys flagged one-sided by the
/// preceding `[member]`/`[fn-order]` diffs are excluded so visibility never
/// double-reports a member that is really just added/removed. `unspecified` (0,
/// e.g. compiler thunks) inherits the class default and is not compared.
///
/// Returns `(member, base_access, target_access)` triples.
fn diff_visibility(
    b: &ClassModel,
    t: &ClassModel,
    fields: &FieldDiff,
    methods: &SeqDiff,
) -> Vec<(String, u8, u8)> {
    let mut out = Vec::new();

    // ── data members (join by name) ──
    let field_one_sided: HashSet<&str> = fields
        .only_base
        .iter()
        .chain(fields.only_target.iter())
        .map(String::as_str)
        .collect();
    let mut tgt_fields: HashMap<&str, u8> = HashMap::new();
    for f in &t.fields {
        tgt_fields.entry(f.name.as_str()).or_insert(f.access);
    }
    let mut seen = HashSet::new();
    for bf in &b.fields {
        if field_one_sided.contains(bf.name.as_str()) || !seen.insert(bf.name.as_str()) {
            continue;
        }
        if let Some(&ta) = tgt_fields.get(bf.name.as_str()) {
            if access_differs(bf.access, ta) {
                out.push((bf.name.clone(), bf.access, ta));
            }
        }
    }

    // ── member functions (join by signature) ──
    let method_one_sided: HashSet<&str> = methods
        .only_base
        .iter()
        .chain(methods.only_target.iter())
        .map(String::as_str)
        .collect();
    let mut tgt_methods: HashMap<&str, u8> = HashMap::new();
    for m in &t.methods {
        tgt_methods.entry(m.sig.as_str()).or_insert(m.access);
    }
    let mut seen = HashSet::new();
    for bm in &b.methods {
        if method_one_sided.contains(bm.sig.as_str()) || !seen.insert(bm.sig.as_str()) {
            continue;
        }
        if let Some(&ta) = tgt_methods.get(bm.sig.as_str()) {
            if access_differs(bm.access, ta) {
                out.push((bm.sig.clone(), bm.access, ta));
            }
        }
    }

    out
}

/// True only when both sides carry a *specified* access (1/2/3) that disagree.
/// `unspecified` (0) inherits the class default and is treated as a non-diff.
fn access_differs(base: u8, target: u8) -> bool {
    base != 0 && target != 0 && base != target
}

fn access_name(access: u8) -> &'static str {
    match access {
        1 => "private",
        2 => "protected",
        3 => "public",
        _ => "unspecified",
    }
}

struct ValueDiff {
    misnamed: Vec<((String, i64), String, String)>,
    only_base: Vec<((String, i64), String)>,
    only_target: Vec<((String, i64), String)>,
}

impl ValueDiff {
    fn diverged(&self) -> bool {
        !self.misnamed.is_empty() || !self.only_base.is_empty() || !self.only_target.is_empty()
    }
}

/// Match two name lists keyed by `(type, value)`. Same key, same name → fine;
/// same key, different name → misname; unmatched → one-sided. Multiple entries
/// sharing a key are paired positionally.
fn value_match(base: &[((String, i64), String)], target: &[((String, i64), String)]) -> ValueDiff {
    let mut base_by_key: HashMap<(String, i64), Vec<String>> = HashMap::new();
    for (key, name) in base {
        base_by_key
            .entry(key.clone())
            .or_default()
            .push(name.clone());
    }
    let mut target_by_key: HashMap<(String, i64), Vec<String>> = HashMap::new();
    for (key, name) in target {
        target_by_key
            .entry(key.clone())
            .or_default()
            .push(name.clone());
    }

    let mut keys: Vec<(String, i64)> = base_by_key.keys().cloned().collect();
    for key in target_by_key.keys() {
        if !base_by_key.contains_key(key) {
            keys.push(key.clone());
        }
    }
    keys.sort();

    let mut misnamed = Vec::new();
    let mut only_base = Vec::new();
    let mut only_target = Vec::new();

    for key in keys {
        let bn = base_by_key.get(&key).cloned().unwrap_or_default();
        let tn = target_by_key.get(&key).cloned().unwrap_or_default();
        let paired = bn.len().min(tn.len());

        for i in 0..paired {
            if bn[i] != tn[i] {
                misnamed.push((key.clone(), bn[i].clone(), tn[i].clone()));
            }
        }
        for name in bn.into_iter().skip(paired) {
            only_base.push((key.clone(), name));
        }
        for name in tn.into_iter().skip(paired) {
            only_target.push((key.clone(), name));
        }
    }

    ValueDiff {
        misnamed,
        only_base,
        only_target,
    }
}

// ── Shared helpers ──────────────────────────────────────────────────────────

#[derive(Default)]
struct HeaderCounts {
    both: usize,
    diverged: usize,
    size_diff: usize,
    member_diff: usize,
    order_diff: usize,
    visibility_diff: usize,
    only_base: Vec<String>,
    only_target: Vec<String>,
    enums_both: usize,
    enums_diverged: usize,
    enum_only_base: Vec<String>,
    enum_only_target: Vec<String>,
}

#[derive(Default)]
struct SourceCounts {
    both: usize,
    diverged: usize,
    order_diff: usize,
    raw_line_table_diff: usize,
    const_diff: usize,
    only_base: Vec<String>,
    only_target: Vec<String>,
}

fn union_keys<'a>(
    a: impl Iterator<Item = &'a String>,
    b: impl Iterator<Item = &'a String>,
) -> Vec<&'a String> {
    let mut keys: Vec<&String> = a.chain(b).collect();
    keys.sort();
    keys.dedup();
    keys
}

fn report_presence(label: &str, only_base: &[String], only_target: &[String], cfg: &Config) {
    if only_base.is_empty() && only_target.is_empty() {
        return;
    }
    if cfg.list_presence {
        if !only_base.is_empty() {
            println!("---- base-only {label} ({}) ----", only_base.len());
            for name in only_base {
                println!("  {name}");
            }
        }
        if !only_target.is_empty() {
            println!("---- target-only {label} ({}) ----", only_target.len());
            for name in only_target {
                println!("  {name}");
            }
        }
        println!();
    }
}

fn push_list(lines: &mut Vec<String>, prefix: &str, items: &[String]) {
    if items.is_empty() {
        return;
    }
    lines.push(format!("{prefix}{}", items.join(", ")));
}

fn dedup_filter(seq: &[String], keep: impl Fn(&String) -> bool) -> Vec<String> {
    let mut seen = HashSet::new();
    let mut out = Vec::new();
    for k in seq {
        if keep(k) && seen.insert(k.clone()) {
            out.push(k.clone());
        }
    }
    out
}

fn include_type(qualified_name: &str, cfg: &Config) -> bool {
    if !cfg.include_external {
        for root in EXTERNAL_ROOTS {
            if qualified_name.starts_with(root) {
                return false;
            }
        }
    }
    !skipped(qualified_name, cfg)
}

fn skipped(name: &str, cfg: &Config) -> bool {
    if cfg.skip.is_empty() {
        return false;
    }
    let lowered = name.to_lowercase();
    cfg.skip.iter().any(|pat| lowered.contains(pat))
}

/// Drop PDB-internal `TypeIndex( 0x… )` payloads that leak into the formatter's
/// "unhandled type" debug fallback (most visibly for bitfields). The numeric
/// index names a slot in *this* PDB's type stream and so differs between two
/// PDBs for the very same logical type; leaving it in would flag every
/// bitfielded struct as a spurious member divergence. The rest of the rendered
/// shape (bitfield length/position, surrounding type) is preserved and still
/// compared.
fn clean_type(s: String) -> String {
    if !s.contains("TypeIndex(") {
        return s;
    }

    let mut out = String::with_capacity(s.len());
    let mut rest = s.as_str();
    while let Some(pos) = rest.find("TypeIndex(") {
        out.push_str(&rest[..pos]);
        out.push_str("TypeIndex(..)");
        rest = &rest[pos + "TypeIndex(".len()..];
        match rest.find(')') {
            Some(close) => rest = &rest[close + 1..],
            None => {
                rest = "";
                break;
            }
        }
    }
    out.push_str(rest);
    out
}

fn kind_label(kind: pdb::ClassKind) -> &'static str {
    match kind {
        pdb::ClassKind::Class => "class",
        pdb::ClassKind::Struct => "struct",
        pdb::ClassKind::Interface => "interface",
    }
}

fn variant_to_i64(value: pdb::Variant) -> i64 {
    match value {
        pdb::Variant::U8(v) => v as i64,
        pdb::Variant::U16(v) => v as i64,
        pdb::Variant::U32(v) => v as i64,
        pdb::Variant::U64(v) => v as i64,
        pdb::Variant::I8(v) => v as i64,
        pdb::Variant::I16(v) => v as i64,
        pdb::Variant::I32(v) => v as i64,
        pdb::Variant::I64(v) => v,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn s(v: &[&str]) -> Vec<String> {
        v.iter().map(|x| x.to_string()).collect()
    }

    #[test]
    fn seq_diff_identical_is_clean() {
        let diff = seq_diff(&s(&["a", "b", "c"]), &s(&["a", "b", "c"]));
        assert!(!diff.diverged());
    }

    #[test]
    fn seq_diff_reports_one_sided() {
        let diff = seq_diff(&s(&["a", "b", "c"]), &s(&["a", "c", "d"]));
        assert_eq!(diff.only_base, s(&["b"]));
        assert_eq!(diff.only_target, s(&["d"]));
        // a and c keep their relative order, so nothing is "moved".
        assert!(diff.moved.is_empty());
    }

    #[test]
    fn seq_diff_reports_reorder() {
        // Common set {a,b,c}; target swaps a after c. The minimal move is one
        // element falling off the longest common subsequence.
        let diff = seq_diff(&s(&["a", "b", "c"]), &s(&["b", "c", "a"]));
        assert!(diff.only_base.is_empty());
        assert!(diff.only_target.is_empty());
        assert_eq!(diff.moved, s(&["a"]));
    }

    #[test]
    fn value_match_pairs_by_value_not_name() {
        // Same (type, value) but different names → misname, not add/remove.
        let base = vec![(("s32".to_string(), 5), "THRESHOLD".to_string())];
        let target = vec![(("s32".to_string(), 5), "LIMIT".to_string())];
        let diff = value_match(&base, &target);
        assert_eq!(
            diff.misnamed,
            vec![(
                ("s32".to_string(), 5),
                "THRESHOLD".to_string(),
                "LIMIT".to_string()
            )]
        );
        assert!(diff.only_base.is_empty());
        assert!(diff.only_target.is_empty());
    }

    #[test]
    fn value_match_same_name_is_clean() {
        let base = vec![(("s32".to_string(), 5), "MAX".to_string())];
        let target = vec![(("s32".to_string(), 5), "MAX".to_string())];
        assert!(!value_match(&base, &target).diverged());
    }

    #[test]
    fn value_match_unmatched_value_is_one_sided() {
        // Different value → not a misname; it is a genuine add/remove.
        let base = vec![(("s32".to_string(), 5), "A".to_string())];
        let target = vec![(("s32".to_string(), 7), "A".to_string())];
        let diff = value_match(&base, &target);
        assert!(diff.misnamed.is_empty());
        assert_eq!(
            diff.only_base,
            vec![(("s32".to_string(), 5), "A".to_string())]
        );
        assert_eq!(
            diff.only_target,
            vec![(("s32".to_string(), 7), "A".to_string())]
        );
    }

    #[test]
    fn access_differs_ignores_unspecified() {
        // Both specified and disagreeing → diff.
        assert!(access_differs(3, 1));
        // Equal → no diff.
        assert!(!access_differs(3, 3));
        // Either side unspecified inherits the class default → no diff.
        assert!(!access_differs(0, 3));
        assert!(!access_differs(3, 0));
        assert!(!access_differs(0, 0));
    }

    fn class_with(fields: &[(&str, u8)], methods: &[(&str, u8)]) -> ClassModel {
        ClassModel {
            kind: "class",
            size: 0,
            fields: fields
                .iter()
                .map(|(n, a)| FieldModel {
                    offset: 0,
                    type_name: "int".to_string(),
                    name: n.to_string(),
                    access: *a,
                })
                .collect(),
            methods: methods
                .iter()
                .map(|(s, a)| MethodModel {
                    sig: s.to_string(),
                    access: *a,
                })
                .collect(),
        }
    }

    #[test]
    fn duplicate_class_records_merge_compatible_method_sets() {
        let mut side = SideModel::default();
        insert_class(
            &mut side,
            "predicate".to_string(),
            class_with(&[("m_value", 3)], &[("bool operator()(b)", 3)]),
        );
        insert_class(
            &mut side,
            "predicate".to_string(),
            class_with(&[("m_value", 3)], &[("bool operator()(a)", 3)]),
        );
        let methods: Vec<&str> = side.classes["predicate"]
            .methods
            .iter()
            .map(|method| method.sig.as_str())
            .collect();
        assert_eq!(methods, vec!["bool operator()(a)", "bool operator()(b)"]);
    }

    #[test]
    fn unrelated_anonymous_enums_do_not_replace_each_other() {
        let mut side = SideModel::default();
        insert_enum(
            &mut side,
            "render::<unnamed-tag>".to_string(),
            EnumModel {
                underlying: "i32".to_string(),
                values: vec![("terrain_size".to_string(), 1024)],
            },
        );
        insert_enum(
            &mut side,
            "render::<unnamed-tag>".to_string(),
            EnumModel {
                underlying: "i32".to_string(),
                values: vec![("component_count".to_string(), 5)],
            },
        );
        assert_eq!(side.enums.len(), 2);
    }

    fn empty_seq() -> SeqDiff {
        SeqDiff {
            only_base: vec![],
            only_target: vec![],
            moved: vec![],
        }
    }

    #[test]
    fn diff_visibility_flags_matched_mismatch() {
        let b = class_with(&[("m_foo", 3)], &[("void f()", 1)]);
        let t = class_with(&[("m_foo", 1)], &[("void f()", 3)]);
        let fields = diff_fields(&b.fields, &t.fields);
        let vis = diff_visibility(&b, &t, &fields, &empty_seq());
        assert_eq!(
            vis,
            vec![("m_foo".to_string(), 3, 1), ("void f()".to_string(), 1, 3),]
        );
    }

    #[test]
    fn diff_visibility_skips_one_sided_members() {
        // m_only is base-only (a [member] diff), so it must NOT be reported as a
        // visibility diff even though the matched member m_foo agrees.
        let b = class_with(&[("m_foo", 3), ("m_only", 1)], &[]);
        let t = class_with(&[("m_foo", 3)], &[]);
        let fields = diff_fields(&b.fields, &t.fields);
        assert!(fields.diverged());
        let vis = diff_visibility(&b, &t, &fields, &empty_seq());
        assert!(vis.is_empty());
    }

    /// Build a side where each function's join key equals its displayed
    /// signature (no demangle divergence) — the common case.
    fn side_with(files: &[(&str, &[&str])]) -> SideModel {
        let mut out = SideModel::default();
        for (path, sigs) in files {
            out.presence_functions
                .extend(sigs.iter().map(|sig| PresenceFn {
                    path: path.to_string(),
                    key: sig.to_string(),
                    name_orig: sig.to_string(),
                }));
            out.files.insert(
                path.to_string(),
                FileModel {
                    functions: sigs
                        .iter()
                        .enumerate()
                        .map(|(index, sig)| FnModel {
                            key: sig.to_string(),
                            name_orig: sig.to_string(),
                            definition_line: index as u32,
                            module_id: 0,
                            symbol_order: index,
                            raw_line_table_count: 0,
                            constants: Vec::new(),
                        })
                        .collect(),
                },
            );
        }
        out
    }

    /// Build a side from explicit `(join key, displayed signature)` pairs, so a
    /// test can model a function whose two PDBs render the signature differently
    /// while the COFF key is shared.
    fn side_with_keyed(files: &[(&str, &[(&str, &str)])]) -> SideModel {
        let mut out = SideModel::default();
        for (path, fns) in files {
            out.presence_functions
                .extend(fns.iter().map(|(key, display)| PresenceFn {
                    path: path.to_string(),
                    key: key.to_string(),
                    name_orig: display.to_string(),
                }));
            out.files.insert(
                path.to_string(),
                FileModel {
                    functions: fns
                        .iter()
                        .enumerate()
                        .map(|(index, (key, display))| FnModel {
                            key: key.to_string(),
                            name_orig: display.to_string(),
                            definition_line: index as u32,
                            module_id: 0,
                            symbol_order: index,
                            raw_line_table_count: 0,
                            constants: Vec::new(),
                        })
                        .collect(),
                },
            );
        }
        out
    }

    fn source_config(compare_raw_line_table_counts: bool) -> Config {
        Config {
            skip: Vec::new(),
            include_external: false,
            do_headers: false,
            do_sources: true,
            list_presence: false,
            list_presence_fns: false,
            compare_raw_line_table_counts,
        }
    }

    #[test]
    fn raw_line_table_count_diff_is_opt_in() {
        let mut base = side_with(&[("m/u.cpp", &["void f()"])]);
        let mut target = side_with(&[("m/u.cpp", &["void f()"])]);
        base.files.get_mut("m/u.cpp").unwrap().functions[0].raw_line_table_count = 2;
        target.files.get_mut("m/u.cpp").unwrap().functions[0].raw_line_table_count = 3;

        let base_file = &base.files["m/u.cpp"];
        let target_file = &target.files["m/u.cpp"];
        let mut default_counts = SourceCounts::default();
        assert!(!diff_file(
            "m/u.cpp",
            base_file,
            target_file,
            &source_config(false),
            &mut default_counts,
        ));
        assert_eq!(default_counts.raw_line_table_diff, 0);

        let mut opt_in_counts = SourceCounts::default();
        assert!(diff_file(
            "m/u.cpp",
            base_file,
            target_file,
            &source_config(true),
            &mut opt_in_counts,
        ));
        assert_eq!(opt_in_counts.raw_line_table_diff, 1);
    }

    #[test]
    fn presence_self_compare_is_clean() {
        // Identical source-function sets on both sides → zero presence diffs.
        // This is the binary's `self-compare MUST report 0 [presence]` invariant.
        let side = side_with(&[
            ("a/x.cpp", &["void a::f()", "int a::g()"]),
            ("b/y.cpp", &["void b::h()"]),
        ]);
        let other = side_with(&[
            ("a/x.cpp", &["void a::f()", "int a::g()"]),
            ("b/y.cpp", &["void b::h()"]),
        ]);
        let diff = presence_functions(&side, &other);
        assert!(diff.base_only.is_empty());
        assert!(diff.target_only.is_empty());
    }

    #[test]
    fn presence_flags_one_sided_out_of_line_bodies() {
        // base emits f() and g() out-of-line; target emits f() and h(). So:
        //   g() is base-only  (we emit standalone, target inlines it)
        //   h() is tgt-only   (target emits standalone, we inline it / no source)
        // f() is on both sides → NOT a presence diff (it is the [fn-order] domain).
        let base = side_with(&[("m/u.cpp", &["void f()", "int g()"])]);
        let target = side_with(&[("m/u.cpp", &["void f()", "void h()"])]);
        let diff = presence_functions(&base, &target);
        assert_eq!(
            diff.base_only,
            vec![("m/u.cpp".to_string(), "int g()".to_string())]
        );
        assert_eq!(
            diff.target_only,
            vec![("m/u.cpp".to_string(), "void h()".to_string())]
        );
    }

    #[test]
    fn presence_covers_one_sided_files() {
        // A whole file present out-of-line on only one side: every function in it
        // is a presence diff for that side (the file-level presence list reports
        // the file; this reports its bodies as reconstruction targets).
        let base = side_with(&[("only/base.cpp", &["void only_base()"])]);
        let target = side_with(&[("only/tgt.cpp", &["void only_tgt()"])]);
        let diff = presence_functions(&base, &target);
        assert_eq!(
            diff.base_only,
            vec![("only/base.cpp".to_string(), "void only_base()".to_string())]
        );
        assert_eq!(
            diff.target_only,
            vec![("only/tgt.cpp".to_string(), "void only_tgt()".to_string())]
        );
    }

    #[test]
    fn presence_deduplicates_repeated_symbol_observations() {
        // A decorated symbol may be observed through more than one module
        // procedure record, but the linked executable still contains one body.
        let base = side_with(&[("m/u.cpp", &["void f()", "void f()"])]);
        let target = side_with(&[("m/u.cpp", &["void f()"])]);
        let diff = presence_functions(&base, &target);
        assert!(diff.base_only.is_empty());
        assert!(diff.target_only.is_empty());
    }

    #[test]
    fn presence_ignores_icf_source_attribution_drift() {
        let base = side_with(&[("m/header.h", &["void folded_f()"])]);
        let target = side_with(&[("m/owner.cpp", &["void folded_f()"])]);
        let diff = presence_functions(&base, &target);
        assert!(diff.base_only.is_empty());
        assert!(diff.target_only.is_empty());
    }

    #[test]
    fn presence_joins_exact_signature_when_public_key_is_unstable() {
        let base = side_with_keyed(&[("m/u.cpp", &[("?base_hash", "void local_f()")])]);
        let target = side_with_keyed(&[("m/u.cpp", &[("?target_hash", "void local_f()")])]);
        let diff = presence_functions(&base, &target);
        assert!(diff.base_only.is_empty());
        assert!(diff.target_only.is_empty());
    }

    #[test]
    fn function_order_ignores_order_within_one_source_line() {
        let mut base = side_with(&[("m/u.cpp", &["void f()", "void g()"])]);
        let mut target = side_with(&[("m/u.cpp", &["void g()", "void f()"])]);
        for side in [&mut base, &mut target] {
            for fun in &mut side.files.get_mut("m/u.cpp").unwrap().functions {
                fun.definition_line = 10;
            }
        }
        assert!(function_order_moved(&base.files["m/u.cpp"], &target.files["m/u.cpp"]).is_empty());
    }

    #[test]
    fn function_order_excludes_records_without_a_source_line() {
        let mut base = side_with(&[("m/u.cpp", &["void generated()", "void source_f()"])]);
        let mut target = side_with(&[("m/u.cpp", &["void source_f()"])]);
        base.files.get_mut("m/u.cpp").unwrap().functions[0].definition_line = 0;
        base.files.get_mut("m/u.cpp").unwrap().functions[1].definition_line = 10;
        target.files.get_mut("m/u.cpp").unwrap().functions[0].definition_line = 10;
        assert!(function_order_moved(&base.files["m/u.cpp"], &target.files["m/u.cpp"]).is_empty());
    }

    #[test]
    fn function_order_rejects_a_line_directive_only_inversion() {
        let mut base = side_with(&[("m/u.cpp", &["void clear()", "void set()"])]);
        let mut target = side_with(&[("m/u.cpp", &["void clear()", "void set()"])]);
        let base_functions = &mut base.files.get_mut("m/u.cpp").unwrap().functions;
        base_functions[0].definition_line = 104;
        base_functions[1].definition_line = 96;
        let target_functions = &mut target.files.get_mut("m/u.cpp").unwrap().functions;
        target_functions[0].definition_line = 90;
        target_functions[1].definition_line = 96;

        assert!(function_order_moved(&base.files["m/u.cpp"], &target.files["m/u.cpp"]).is_empty());
    }

    #[test]
    fn function_order_requires_a_definite_pairwise_inversion() {
        let mut base = side_with(&[("m/u.cpp", &["void f()", "void generated()", "void g()"])]);
        let mut target = side_with(&[("m/u.cpp", &["void f()", "void generated()", "void g()"])]);
        let base_functions = &mut base.files.get_mut("m/u.cpp").unwrap().functions;
        base_functions[0].definition_line = 10;
        base_functions[1].definition_line = 20;
        base_functions[2].definition_line = 20;
        let target_functions = &mut target.files.get_mut("m/u.cpp").unwrap().functions;
        target_functions[0].definition_line = 10;
        target_functions[1].definition_line = 10;
        target_functions[2].definition_line = 20;
        assert!(function_order_moved(&base.files["m/u.cpp"], &target.files["m/u.cpp"]).is_empty());

        let base_functions = &mut base.files.get_mut("m/u.cpp").unwrap().functions;
        base_functions[0].definition_line = 10;
        base_functions[1].definition_line = 10;
        base_functions[2].definition_line = 30;
        let target_functions = &mut target.files.get_mut("m/u.cpp").unwrap().functions;
        target_functions[0].definition_line = 30;
        target_functions[1].definition_line = 10;
        target_functions[2].definition_line = 10;
        target_functions[0].symbol_order = 2;
        target_functions[2].symbol_order = 0;
        assert_eq!(
            function_order_moved(&base.files["m/u.cpp"], &target.files["m/u.cpp"]),
            vec!["name|void f()".to_string(), "name|void g()".to_string()]
        );
    }

    #[test]
    fn ordered_function_display_shows_each_sides_source_order() {
        let keys = vec![
            "name|void second()".to_string(),
            "name|void first()".to_string(),
        ];
        let source_lines = HashMap::from([
            ("name|void first()".to_string(), 10),
            ("name|void second()".to_string(), 20),
        ]);
        let display = HashMap::from([
            ("name|void first()".to_string(), "void first()"),
            ("name|void second()".to_string(), "void second()"),
        ]);

        assert_eq!(
            ordered_function_display(&keys, &source_lines, &display),
            vec![
                "line 10: void first()".to_string(),
                "line 20: void second()".to_string(),
            ]
        );
    }

    #[test]
    fn join_key_prefers_mangled_symbol() {
        // With a Public symbol, the key is that decorated name verbatim — it is
        // identical across the two PDBs even when their demangled signatures differ.
        let mangled = "?find_ignored_object@pre_perceptors_filter@ai@vostok@@ABE...@Z";
        assert_eq!(
            function_join_key(Some(mangled), "stlp_std::pair<...> vostok::ai::...()"),
            mangled
        );
    }

    #[test]
    fn join_key_canonicalizes_static_init_thunk() {
        // No Public symbol → every rendering of the same thunk normalizes to one
        // `kind|fully-qualified-var` key, so the sides pair.
        let key = "dynamic initializer for|s_flow_emulator";

        // base mangled form.
        assert_eq!(
            function_join_key(None, "void ??__Es_flow_emulator@@YAXXZ()"),
            key
        );
        // target class-scope form (FQ inside the backticks, no outer prefix).
        assert_eq!(
            function_join_key(None, "void `dynamic initializer for 's_flow_emulator''()"),
            key
        );
    }

    #[test]
    fn join_key_unifies_namespace_and_class_thunk_renderings() {
        // The SAME namespaced static `vostok::core::s_show_help` is rendered three
        // ways across the two PDBs; all must collapse to one key.
        let key = "dynamic initializer for|vostok::core::s_show_help";
        // base mangled.
        assert_eq!(
            function_join_key(None, "void ??__Es_show_help@core@vostok@@YAXXZ()"),
            key
        );
        // target namespace-scope: `ns::` prefix OUTSIDE the backticks, short inner.
        assert_eq!(
            function_join_key(
                None,
                "void vostok::core::`dynamic initializer for 's_show_help''()"
            ),
            key
        );
        // target class-scope-style: fully-qualified INSIDE the backticks.
        assert_eq!(
            function_join_key(
                None,
                "void `dynamic initializer for 'vostok::core::s_show_help''()"
            ),
            key
        );
    }

    #[test]
    fn join_key_atexit_destructor_distinct_from_initializer() {
        // initializer and atexit destructor for the same var are different thunks.
        let init = function_join_key(None, "void ??__Es_world@@YAXXZ()");
        let dtor = function_join_key(None, "void ??__Fs_world@@YAXXZ()");
        assert_eq!(init, "dynamic initializer for|s_world");
        assert_eq!(dtor, "dynamic atexit destructor for|s_world");
        assert_ne!(init, dtor);
    }

    #[test]
    fn join_key_passes_through_plain_signature() {
        // A non-thunk function without a Public symbol keeps its signature as key.
        let sig = "void vostok::foo::bar(int)";
        assert_eq!(function_join_key(None, sig), sig);
    }

    #[test]
    fn presence_joins_by_key_despite_demangle_divergence() {
        // The same logical function whose signature the two PDBs demangle
        // differently (base drops the `enum` keyword inside a template arg, target
        // keeps it) shares one COFF key. Joining on the key, it must NOT show up as
        // a paired base-only / target-only presence false positive.
        let mangled = "?ignore@pre_perceptors_filter@ai@vostok@@QAEX...@Z";
        let base = side_with_keyed(&[(
            "ai/pre_perceptors_filter.cpp",
            &[(
                mangled,
                "void vostok::ai::...<...,vostok::ai::ignorance_types_enum>...()",
            )],
        )]);
        let target = side_with_keyed(&[(
            "ai/pre_perceptors_filter.cpp",
            &[(
                mangled,
                "void vostok::ai::...<...,enum vostok::ai::ignorance_types_enum>...()",
            )],
        )]);
        let diff = presence_functions(&base, &target);
        assert!(diff.base_only.is_empty(), "{:?}", diff.base_only);
        assert!(diff.target_only.is_empty(), "{:?}", diff.target_only);
    }

    #[test]
    fn clean_type_strips_pdb_type_indices() {
        let a = clean_type(
            "Bitfield( BitfieldType { underlying_type: TypeIndex( 0x65038 ), length: 4 } )"
                .to_string(),
        );
        let b = clean_type(
            "Bitfield( BitfieldType { underlying_type: TypeIndex( 0x68899 ), length: 4 } )"
                .to_string(),
        );
        // Same logical bitfield, different per-PDB index → equal after cleaning.
        assert_eq!(a, b);
        assert!(a.contains("length: 4"));
        // A plain type is returned untouched.
        assert_eq!(clean_type("s32".to_string()), "s32");
    }
}
