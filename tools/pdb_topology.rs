//! Query and compare raw CodeView function/class topology without flattening the PDB.

use std::collections::{BTreeMap, HashMap, HashSet};
use std::fmt::Write as _;
use std::io::Write as _;
use std::path::PathBuf;

use clap::{ArgGroup, Parser};
use pdb::{FallibleIterator, SymbolData};
use vostok_pdb_parser::pdb_parser::PdbParser;

#[derive(Parser)]
#[command(group(
    ArgGroup::new("input")
        .required(true)
        .args(["pdb", "target_pdb"])
), group(
    ArgGroup::new("query")
        .required(true)
        .args(["function", "classes"])
))]
struct Cli {
    /// Inspect one PDB without comparing it.
    #[arg(long, value_hint = clap::ValueHint::FilePath)]
    pdb: Option<PathBuf>,

    /// Retail/reference PDB for a target-vs-base topology diff.
    #[arg(
        long,
        value_hint = clap::ValueHint::FilePath,
        requires = "base_pdb",
        conflicts_with = "pdb"
    )]
    target_pdb: Option<PathBuf>,

    /// Reconstructed PDB for a target-vs-base topology diff.
    #[arg(
        long,
        value_hint = clap::ValueHint::FilePath,
        requires = "target_pdb",
        conflicts_with = "pdb"
    )]
    base_pdb: Option<PathBuf>,

    /// Case-insensitive substring of the PDB procedure name.
    #[arg(long, conflicts_with = "classes")]
    function: Option<String>,

    /// Compare every complete target class/struct/interface against the base PDB.
    #[arg(
        long,
        requires = "target_pdb",
        conflicts_with_all = ["pdb", "function", "module"]
    )]
    classes: bool,

    /// Restrict --classes to one case-insensitive qualified class name.
    #[arg(long = "class", requires = "classes")]
    class_filter: Option<String>,

    /// Include classes with no semantic differences in --classes text output.
    #[arg(long, requires = "classes")]
    show_identical: bool,

    /// Optional case-insensitive module/object/library substring.
    #[arg(long)]
    module: Option<String>,

    /// Number of physical and top-level neighboring records to show.
    #[arg(long, default_value_t = 8)]
    context: usize,

    /// Emit the owned evidence model as JSON instead of the annotated text view.
    #[arg(long)]
    json: bool,
}

#[derive(Clone, serde::Serialize)]
struct Record {
    index: u32,
    raw_kind: u16,
    raw_len: usize,
    depth: usize,
    kind: &'static str,
    detail: String,
}

#[derive(Clone, serde::Serialize)]
struct Match {
    module_id: usize,
    module_name: String,
    object_file_name: String,
    procedure_pos: usize,
    procedure_end: u32,
    procedure_type: u32,
    procedure_name: String,
    records: Vec<Record>,
    lines: Vec<String>,
    type_rows: Vec<String>,
    declaration_rows: Vec<String>,
}

fn main() {
    let cli = Cli::parse();
    if let Err(error) = run(&cli) {
        eprintln!("{error}");
        std::process::exit(1);
    }
}

fn run(cli: &Cli) -> vostok_pdb_parser::Result<()> {
    let output = if cli.classes {
        let target_pdb = cli.target_pdb.as_ref().expect("clap requires target PDB");
        let base_pdb = cli.base_pdb.as_ref().expect("clap requires base PDB");
        let report = build_class_report(cli, target_pdb, base_pdb)?;
        if cli.json {
            serde_json::to_string_pretty(&report).unwrap_or_else(|error| {
                format!(
                    "{{\"error\":{}}}\n",
                    serde_json::Value::String(error.to_string())
                )
            })
        } else {
            render_class_report(&report, cli.show_identical)
        }
    } else if let Some(pdb) = &cli.pdb {
        let matches = load_matches(cli, pdb)?;
        render_single(cli, &matches)
    } else {
        let target_pdb = cli.target_pdb.as_ref().expect("clap requires target PDB");
        let base_pdb = cli.base_pdb.as_ref().expect("clap requires base PDB");
        let target = load_matches(cli, target_pdb)?;
        let base = load_matches(cli, base_pdb)?;
        let report = build_diff_report(target_pdb, base_pdb, target, base, cli.context);
        if cli.json {
            serde_json::to_string_pretty(&report).unwrap_or_else(|error| {
                format!(
                    "{{\"error\":{}}}\n",
                    serde_json::Value::String(error.to_string())
                )
            })
        } else {
            render_diff_report(&report)
        }
    };

    // `head`/`sed` routinely close a pipeline early. Treat that as a normal query
    // termination instead of panicking through Rust's `print!` convenience macro.
    let _ = std::io::stdout().lock().write_all(output.as_bytes());
    Ok(())
}

fn load_matches(cli: &Cli, pdb: &PathBuf) -> vostok_pdb_parser::Result<Vec<Match>> {
    let mut matches = Vec::new();
    PdbParser::with(pdb, |fmt| {
        matches = inspect(cli, pdb, &fmt)?;
        Ok(())
    })?;
    Ok(matches)
}

fn inspect(
    cli: &Cli,
    pdb_path: &PathBuf,
    fmt: &PdbParser<'_, '_>,
) -> vostok_pdb_parser::Result<Vec<Match>> {
    let file = std::fs::File::open(pdb_path)?;
    let mut pdb = pdb::PDB::open(file)?;
    let address_map = pdb.address_map()?;
    let string_table = pdb.string_table()?;
    let dbi = pdb.debug_information()?;
    let mut modules = dbi.modules()?;
    let needle = cli
        .function
        .as_ref()
        .expect("clap requires a function outside --classes")
        .to_lowercase();
    let module_needle = cli.module.as_ref().map(|s| s.to_lowercase());
    let mut matches = Vec::new();
    let mut module_id = 0usize;

    while let Some(module) = modules.next()? {
        let this_module_id = module_id;
        module_id += 1;
        let module_name = module.module_name().into_owned();
        let object_file_name = module.object_file_name().into_owned();
        if let Some(want) = &module_needle {
            let hay = format!("{module_name}\n{object_file_name}").to_lowercase();
            if !hay.contains(want) {
                continue;
            }
        }

        let Some(info) = pdb.module_info(&module)? else {
            continue;
        };
        let program = info.line_program()?;
        let mut symbols = info.symbols()?;
        let mut records = Vec::new();
        let mut depth = 0usize;
        let mut selected: Vec<(usize, pdb::ProcedureSymbol<'_>, String)> = Vec::new();

        while let Some(symbol) = symbols.next()? {
            let index = symbol.index().0;
            let raw_kind = symbol.raw_kind();
            let raw_len = symbol.raw_bytes().len();
            let closes = symbol.ends_scope();
            let record_depth = if closes {
                depth.saturating_sub(1)
            } else {
                depth
            };

            match symbol.parse() {
                Ok(data) => {
                    if let SymbolData::Procedure(proc) = data {
                        let name = function_name(fmt, this_module_id, &proc.name, proc.type_index);
                        if name.to_lowercase().contains(&needle) {
                            selected.push((records.len(), proc, name));
                        }
                    }
                    records.push(summarize_record(
                        fmt,
                        this_module_id,
                        &address_map,
                        index,
                        raw_kind,
                        raw_len,
                        record_depth,
                        data,
                    ));
                }
                Err(error) => records.push(Record {
                    index,
                    raw_kind,
                    raw_len,
                    depth: record_depth,
                    kind: "Unparsed",
                    detail: error.to_string(),
                }),
            }

            if symbol.starts_scope() {
                depth += 1;
            }
            if closes {
                depth = depth.saturating_sub(1);
            }
        }

        for (procedure_pos, proc, procedure_name) in selected {
            let mut lines = Vec::new();
            let mut iter = program.lines_for_symbol(proc.offset);
            while let Some(line) = iter.next()? {
                let file = program
                    .get_file_info(line.file_index)?
                    .name
                    .to_string_lossy(&string_table)?;
                let rva = line.offset.to_rva(&address_map).map(|v| v.0);
                lines.push(format!(
                    "rva={} bytes={} source={}:{}-{} cols={:?}-{:?} kind={:?}",
                    rva.map_or_else(|| "?".into(), |v| format!("0x{v:x}")),
                    line.length
                        .map_or_else(|| "?".into(), |v| format!("0x{v:x}")),
                    file,
                    line.line_start,
                    line.line_end,
                    line.column_start,
                    line.column_end,
                    line.kind,
                ));
            }
            matches.push(Match {
                module_id: this_module_id,
                module_name: module_name.clone(),
                object_file_name: object_file_name.clone(),
                procedure_pos,
                procedure_end: proc.end.0,
                procedure_type: proc.type_index.0,
                procedure_name,
                records: records.clone(),
                lines,
                type_rows: Vec::new(),
                declaration_rows: Vec::new(),
            });
        }
    }

    drop(modules);
    drop(dbi);
    if !matches.is_empty() {
        let types = pdb.type_information()?;
        let mut iter = types.iter();
        while let Some(ty) = iter.next()? {
            let index = ty.index().0;
            for found in &mut matches {
                let lo = found.procedure_type.saturating_sub(cli.context as u32);
                let hi = found.procedure_type.saturating_add(cli.context as u32);
                if index >= lo && index <= hi {
                    found.type_rows.push(format!(
                        "{} type=0x{index:x} raw=0x{:04x}/0x{:x} {}",
                        if index == found.procedure_type {
                            ">"
                        } else {
                            " "
                        },
                        ty.raw_kind(),
                        ty.len(),
                        summarize_type(ty.parse()),
                    ));
                }
            }
        }
        bind_declarations(&types, &mut matches, cli.context)?;
    }

    Ok(matches)
}

fn render_single(cli: &Cli, matches: &[Match]) -> String {
    if matches.is_empty() {
        return if cli.json {
            "[]\n".into()
        } else {
            format!(
                "no procedure matched {:?}\n",
                cli.function.as_deref().unwrap_or_default()
            )
        };
    }

    if cli.json {
        serde_json::to_string_pretty(&matches).unwrap_or_else(|error| {
            format!(
                "{{\"error\":{}}}\n",
                serde_json::Value::String(error.to_string())
            )
        })
    } else {
        let mut output = format!("matched {} procedure(s)\n", matches.len());
        for found in matches {
            output.push_str(&render_match(found, cli.context));
        }
        output
    }
}

// ---------------------------------------------------------------------------
// Whole-PDB class comparison

#[derive(Clone, Debug, PartialEq, Eq, serde::Serialize)]
struct ClassEntry {
    kind: &'static str,
    name: String,
    type_name: String,
    access: &'static str,
    offset: Option<u64>,
    argument_count: Option<u16>,
    attributes: Vec<String>,
    raw_attributes: String,
    vtable_offset: Option<u32>,
    details: Vec<String>,
}

#[derive(Clone, Debug, PartialEq, Eq, serde::Serialize)]
struct ClassModel {
    name: String,
    kind: &'static str,
    size: u64,
    properties: String,
    entries: Vec<ClassEntry>,
}

#[derive(Default)]
struct ClassSide {
    classes: BTreeMap<String, Vec<ClassModel>>,
}

#[derive(serde::Serialize)]
struct ClassReport {
    target_pdb: String,
    base_pdb: String,
    class_filter: Option<String>,
    target_classes: usize,
    compared_classes: usize,
    identical_classes: usize,
    different_classes: usize,
    missing_base_classes: usize,
    base_only_classes: usize,
    target_duplicate_variants: usize,
    base_duplicate_variants: usize,
    target_unresolved_types: usize,
    base_unresolved_types: usize,
    difference_counts: BTreeMap<String, usize>,
    classes: Vec<ClassComparison>,
}

#[derive(serde::Serialize)]
struct ClassComparison {
    name: String,
    status: &'static str,
    target_variants: usize,
    base_variants: usize,
    differences: Vec<ClassDifference>,
}

#[derive(serde::Serialize)]
struct ClassDifference {
    category: &'static str,
    member: Option<String>,
    base: Option<String>,
    target: Option<String>,
}

fn build_class_report(
    cli: &Cli,
    target_pdb: &PathBuf,
    base_pdb: &PathBuf,
) -> vostok_pdb_parser::Result<ClassReport> {
    let filter = cli
        .class_filter
        .as_ref()
        .map(|value| normalize_cross_pdb_type(value.to_lowercase()));
    let target = load_classes(target_pdb, filter.as_deref())?;
    let base = load_classes(base_pdb, filter.as_deref())?;

    let target_classes = target.classes.len();
    let target_duplicate_variants = target
        .classes
        .values()
        .map(|variants| variants.len().saturating_sub(1))
        .sum();
    let base_duplicate_variants = base
        .classes
        .values()
        .map(|variants| variants.len().saturating_sub(1))
        .sum();
    let target_unresolved_types = unresolved_type_count(&target);
    let base_unresolved_types = unresolved_type_count(&base);
    let base_only_classes = base
        .classes
        .keys()
        .filter(|name| !target.classes.contains_key(*name))
        .count();
    let mut compared_classes = 0usize;
    let mut identical_classes = 0usize;
    let mut different_classes = 0usize;
    let mut missing_base_classes = 0usize;
    let mut difference_counts = BTreeMap::new();
    let mut classes = Vec::with_capacity(target_classes);

    for (key, target_variants) in &target.classes {
        let target_class = canonical_class(target_variants);
        let name = target_class.name.clone();
        let Some(base_variants) = base.classes.get(key) else {
            missing_base_classes += 1;
            *difference_counts
                .entry("class-presence".to_owned())
                .or_insert(0) += 1;
            classes.push(ClassComparison {
                name,
                status: "missing-base",
                target_variants: target_variants.len(),
                base_variants: 0,
                differences: vec![ClassDifference {
                    category: "class-presence",
                    member: None,
                    base: None,
                    target: Some(class_summary(target_class)),
                }],
            });
            continue;
        };

        compared_classes += 1;
        let base_class = canonical_class(base_variants);
        let differences = compare_class(base_class, target_class);
        let status = if differences.is_empty() {
            identical_classes += 1;
            "identical"
        } else {
            different_classes += 1;
            for difference in &differences {
                *difference_counts
                    .entry(difference.category.to_owned())
                    .or_insert(0) += 1;
            }
            "different"
        };
        classes.push(ClassComparison {
            name,
            status,
            target_variants: target_variants.len(),
            base_variants: base_variants.len(),
            differences,
        });
    }

    Ok(ClassReport {
        target_pdb: target_pdb.display().to_string(),
        base_pdb: base_pdb.display().to_string(),
        class_filter: cli.class_filter.clone(),
        target_classes,
        compared_classes,
        identical_classes,
        different_classes,
        missing_base_classes,
        base_only_classes,
        target_duplicate_variants,
        base_duplicate_variants,
        target_unresolved_types,
        base_unresolved_types,
        difference_counts,
        classes,
    })
}

fn load_classes(pdb_path: &PathBuf, filter: Option<&str>) -> vostok_pdb_parser::Result<ClassSide> {
    let mut side = ClassSide::default();
    PdbParser::with(pdb_path, |fmt| {
        let file = std::fs::File::open(pdb_path)?;
        let mut pdb = pdb::PDB::open(file)?;
        let types = pdb.type_information()?;
        let mut finder = types.finder();
        let mut finder_iter = types.iter();
        while finder_iter.next()?.is_some() {
            finder.update(&finder_iter);
        }

        let mut iter = types.iter();
        while let Some(ty) = iter.next()? {
            let Ok(pdb::TypeData::Class(class)) = ty.parse() else {
                continue;
            };
            if class.properties.forward_reference() {
                continue;
            }
            let name = class.name.to_string().into_owned();
            let key = normalize_cross_pdb_type(name.to_lowercase());
            if filter.is_some_and(|wanted| key != wanted) {
                continue;
            }

            let mut model = ClassModel {
                name: name.clone(),
                kind: class_kind(class.kind),
                size: class.size,
                properties: format!("{:?}", class.properties),
                entries: Vec::new(),
            };
            if let Some(fields) = class.fields {
                walk_class_fields(&finder, &fmt, fields, &mut model, &mut HashSet::new())?;
            }
            let variants = side.classes.entry(key).or_default();
            if !variants.contains(&model) {
                variants.push(model);
            }
        }
        Ok(())
    })?;
    Ok(side)
}

fn walk_class_fields(
    finder: &pdb::TypeFinder<'_>,
    fmt: &PdbParser<'_, '_>,
    field_index: pdb::TypeIndex,
    class: &mut ClassModel,
    seen: &mut HashSet<u32>,
) -> vostok_pdb_parser::Result<()> {
    if !seen.insert(field_index.0) {
        return Ok(());
    }
    let pdb::TypeData::FieldList(list) = finder.find(field_index)?.parse()? else {
        return Ok(());
    };
    for field in list.fields {
        append_class_entry(finder, fmt, field, class)?;
    }
    if let Some(continuation) = list.continuation {
        walk_class_fields(finder, fmt, continuation, class, seen)?;
    }
    Ok(())
}

fn append_class_entry(
    finder: &pdb::TypeFinder<'_>,
    fmt: &PdbParser<'_, '_>,
    field: pdb::TypeData<'_>,
    class: &mut ClassModel,
) -> vostok_pdb_parser::Result<()> {
    match field {
        pdb::TypeData::Member(member) => class.entries.push(ClassEntry {
            kind: "field",
            name: member.name.to_string().into_owned(),
            type_name: comparable_type_name(fmt, member.field_type),
            access: access_name(member.attributes.access()),
            offset: Some(member.offset),
            argument_count: None,
            attributes: field_attribute_labels(member.attributes),
            raw_attributes: comparable_field_attributes(member.attributes),
            vtable_offset: None,
            details: Vec::new(),
        }),
        pdb::TypeData::StaticMember(member) => class.entries.push(ClassEntry {
            kind: "static-field",
            name: member.name.to_string().into_owned(),
            type_name: comparable_type_name(fmt, member.field_type),
            access: access_name(member.attributes.access()),
            offset: None,
            argument_count: None,
            attributes: field_attribute_labels(member.attributes),
            raw_attributes: comparable_field_attributes(member.attributes),
            vtable_offset: None,
            details: Vec::new(),
        }),
        pdb::TypeData::Nested(nested) => class.entries.push(ClassEntry {
            kind: "nested-type",
            name: nested.name.to_string().into_owned(),
            type_name: comparable_type_name(fmt, nested.nested_type),
            access: access_name(nested.attributes.access()),
            offset: None,
            argument_count: None,
            attributes: field_attribute_labels(nested.attributes),
            raw_attributes: comparable_field_attributes(nested.attributes),
            vtable_offset: None,
            details: Vec::new(),
        }),
        pdb::TypeData::BaseClass(base) => class.entries.push(ClassEntry {
            kind: "base",
            name: comparable_type_name(fmt, base.base_class),
            type_name: comparable_type_name(fmt, base.base_class),
            access: access_name(base.attributes.access()),
            offset: Some(base.offset as u64),
            argument_count: None,
            attributes: field_attribute_labels(base.attributes),
            raw_attributes: comparable_field_attributes(base.attributes),
            vtable_offset: None,
            details: vec![format!("kind={:?}", base.kind)],
        }),
        pdb::TypeData::VirtualBaseClass(base) => class.entries.push(ClassEntry {
            kind: "virtual-base",
            name: comparable_type_name(fmt, base.base_class),
            type_name: comparable_type_name(fmt, base.base_class),
            access: access_name(base.attributes.access()),
            offset: Some(base.base_pointer_offset as u64),
            argument_count: None,
            attributes: field_attribute_labels(base.attributes),
            raw_attributes: comparable_field_attributes(base.attributes),
            vtable_offset: None,
            details: vec![
                format!("direct={}", base.direct),
                format!(
                    "base_pointer={}",
                    comparable_type_name(fmt, base.base_pointer)
                ),
                format!("virtual_base_offset=0x{:x}", base.virtual_base_offset),
            ],
        }),
        pdb::TypeData::VirtualFunctionTablePointer(table) => {
            class.entries.push(ClassEntry {
                kind: "vftable-pointer",
                name: "<vftable>".to_owned(),
                type_name: comparable_type_name(fmt, table.table),
                access: "unspecified",
                offset: None,
                argument_count: None,
                attributes: Vec::new(),
                raw_attributes: String::new(),
                vtable_offset: None,
                details: Vec::new(),
            });
        }
        pdb::TypeData::Method(method) => {
            class.entries.push(method_entry(
                fmt,
                finder,
                method.name,
                method.method_type,
                method.attributes,
                method.vtable_offset,
            )?);
        }
        pdb::TypeData::OverloadedMethod(overload) => {
            if let pdb::TypeData::MethodList(methods) =
                finder.find(overload.method_list)?.parse()?
            {
                for method in methods.methods {
                    class.entries.push(method_entry(
                        fmt,
                        finder,
                        overload.name,
                        method.method_type,
                        method.attributes,
                        method.vtable_offset,
                    )?);
                }
            }
        }
        _ => {}
    }
    Ok(())
}

fn method_entry(
    fmt: &PdbParser<'_, '_>,
    finder: &pdb::TypeFinder<'_>,
    name: pdb::RawString<'_>,
    method_type: pdb::TypeIndex,
    attributes: pdb::FieldAttributes,
    vtable_offset: Option<u32>,
) -> vostok_pdb_parser::Result<ClassEntry> {
    let placeholder = pdb::RawString::from("<method>");
    let signature = fmt
        .emit_function_orig(&placeholder, 0, method_type)
        .map(normalize_cross_pdb_type)
        .unwrap_or_else(|_| "<unresolved-method-signature>".to_owned());
    let (argument_count, details) = match finder.find(method_type)?.parse()? {
        pdb::TypeData::MemberFunction(function) => (
            Some(function.parameter_count),
            vec![
                format!(
                    "calling_convention={}",
                    function.attributes.calling_convention()
                ),
                format!("cxx_return_udt={}", function.attributes.cxx_return_udt()),
                format!("constructor={}", function.attributes.is_constructor()),
                format!(
                    "constructor_virtual_bases={}",
                    function.attributes.is_constructor_with_virtual_bases()
                ),
                format!("this_adjustment={}", function.this_adjustment),
            ],
        ),
        pdb::TypeData::Procedure(function) => (
            Some(function.parameter_count),
            vec![
                format!(
                    "calling_convention={}",
                    function.attributes.calling_convention()
                ),
                format!("cxx_return_udt={}", function.attributes.cxx_return_udt()),
                format!("constructor={}", function.attributes.is_constructor()),
                format!(
                    "constructor_virtual_bases={}",
                    function.attributes.is_constructor_with_virtual_bases()
                ),
            ],
        ),
        _ => (None, Vec::new()),
    };
    Ok(ClassEntry {
        kind: "method",
        name: name.to_string().into_owned(),
        type_name: signature,
        access: access_name(attributes.access()),
        offset: None,
        argument_count,
        attributes: field_attribute_labels(attributes),
        raw_attributes: comparable_field_attributes(attributes),
        vtable_offset,
        details,
    })
}

fn comparable_type_name(fmt: &PdbParser<'_, '_>, index: pdb::TypeIndex) -> String {
    fmt.emit_type_impl(0, index)
        .map(normalize_cross_pdb_type)
        .unwrap_or_else(|_| "<unresolved-type>".to_owned())
}

fn normalize_cross_pdb_type(value: String) -> String {
    let value = clean_type_indexes(&value);
    let value = value
        .replace("class ", "")
        .replace("struct ", "")
        .replace("enum ", "");
    let value = value.split_whitespace().collect::<Vec<_>>().join(" ");
    // The two PDB formatters sometimes disagree only on a space before a comma
    // in a nested template argument (`T const ,U` versus `T const,U`).
    value.replace(" ,", ",")
}

fn clean_type_indexes(value: &str) -> String {
    let mut out = String::with_capacity(value.len());
    let mut rest = value;
    while let Some(position) = rest.find("TypeIndex(") {
        out.push_str(&rest[..position]);
        out.push_str("TypeIndex(..)");
        rest = &rest[position + "TypeIndex(".len()..];
        match rest.find(')') {
            Some(close) => rest = &rest[close + 1..],
            None => return out,
        }
    }
    out.push_str(rest);
    out
}

fn class_kind(kind: pdb::ClassKind) -> &'static str {
    match kind {
        pdb::ClassKind::Class => "class",
        pdb::ClassKind::Struct => "struct",
        pdb::ClassKind::Interface => "interface",
    }
}

fn access_name(access: u8) -> &'static str {
    match access {
        1 => "private",
        2 => "protected",
        3 => "public",
        _ => "unspecified",
    }
}

fn field_attribute_labels(attributes: pdb::FieldAttributes) -> Vec<String> {
    let mut labels = Vec::new();
    for (enabled, label) in [
        (attributes.is_static(), "static"),
        (attributes.is_virtual(), "virtual"),
        (attributes.is_pure_virtual(), "pure-virtual"),
        (attributes.is_intro_virtual(), "introducing-virtual"),
        (attributes.is_pseudo(), "pseudo"),
        (attributes.noinherit(), "no-inherit"),
        (attributes.noconstruct(), "no-construct"),
        (attributes.is_compgenx(), "compiler-generated"),
        (attributes.sealed(), "sealed"),
    ] {
        if enabled {
            labels.push(label.to_owned());
        }
    }
    labels
}

fn comparable_field_attributes(attributes: pdb::FieldAttributes) -> String {
    let debug = format!("{:?}", attributes);
    let value = debug
        .strip_prefix("FieldAttributes(")
        .and_then(|value| value.strip_suffix(')'))
        .and_then(|value| value.parse::<u16>().ok());
    value.map_or(debug, |value| format!("0x{:x}", value & !0x3))
}

fn unresolved_type_count(side: &ClassSide) -> usize {
    side.classes
        .values()
        .map(|variants| canonical_class(variants))
        .flat_map(|class| &class.entries)
        .filter(|entry| entry.type_name.starts_with("<unresolved-"))
        .count()
}

fn canonical_class(variants: &[ClassModel]) -> &ClassModel {
    variants
        .iter()
        .max_by_key(|class| (class.entries.len(), class.size))
        .expect("complete class variant list cannot be empty")
}

fn class_summary(class: &ClassModel) -> String {
    format!(
        "{} {} size=0x{:x} declarations={}",
        class.kind,
        class.name,
        class.size,
        class.entries.len()
    )
}

fn compare_class(base: &ClassModel, target: &ClassModel) -> Vec<ClassDifference> {
    let mut differences = Vec::new();
    if base.kind != target.kind {
        differences.push(class_difference("class-kind", None, base.kind, target.kind));
    }
    if base.size != target.size {
        differences.push(class_difference(
            "class-size",
            None,
            format!("0x{:x}", base.size),
            format!("0x{:x}", target.size),
        ));
    }
    if base.properties != target.properties {
        differences.push(class_difference(
            "class-properties",
            None,
            &base.properties,
            &target.properties,
        ));
    }

    let pairs = pair_class_entries(&base.entries, &target.entries);
    let paired_target: HashSet<usize> = pairs.iter().map(|(target, _)| *target).collect();
    let paired_base: HashSet<usize> = pairs.iter().map(|(_, base)| *base).collect();

    for (target_index, base_index) in &pairs {
        compare_class_entry(
            &base.entries[*base_index],
            &target.entries[*target_index],
            &mut differences,
        );
    }
    for (index, entry) in target.entries.iter().enumerate() {
        if !paired_target.contains(&index) {
            differences.push(ClassDifference {
                category: entry_presence_category(entry.kind),
                member: Some(entry.name.clone()),
                base: None,
                target: Some(entry_summary(entry)),
            });
        }
    }
    for (index, entry) in base.entries.iter().enumerate() {
        if !paired_base.contains(&index) {
            differences.push(ClassDifference {
                category: entry_presence_category(entry.kind),
                member: Some(entry.name.clone()),
                base: Some(entry_summary(entry)),
                target: None,
            });
        }
    }

    append_order_differences(&pairs, &base.entries, &target.entries, &mut differences);
    differences
}

fn pair_class_entries(base: &[ClassEntry], target: &[ClassEntry]) -> Vec<(usize, usize)> {
    let mut used = vec![false; base.len()];
    let mut pairs = Vec::new();
    for (target_index, target_entry) in target.iter().enumerate() {
        let best = base
            .iter()
            .enumerate()
            .filter(|(index, _)| !used[*index])
            .filter_map(|(index, base_entry)| {
                entry_pair_score(base_entry, target_entry)
                    .map(|score| (score, index.abs_diff(target_index), index))
            })
            .min();
        if let Some((_, _, base_index)) = best {
            used[base_index] = true;
            pairs.push((target_index, base_index));
        }
    }
    pairs
}

fn entry_pair_score(base: &ClassEntry, target: &ClassEntry) -> Option<u8> {
    if base.kind != target.kind {
        return None;
    }
    match target.kind {
        "method" => {
            if base.name == target.name && base.type_name == target.type_name {
                Some(0)
            } else if base.name == target.name && base.argument_count == target.argument_count {
                Some(1)
            } else if base.name == target.name {
                Some(2)
            } else if base.type_name == target.type_name
                && base.attributes == target.attributes
                && base.vtable_offset == target.vtable_offset
            {
                Some(3)
            } else if base.vtable_offset.is_some()
                && base.vtable_offset == target.vtable_offset
                && base.argument_count == target.argument_count
            {
                Some(4)
            } else {
                None
            }
        }
        "field" => {
            if base.name == target.name {
                Some(0)
            } else if base.offset == target.offset && base.type_name == target.type_name {
                Some(1)
            } else {
                None
            }
        }
        "static-field" | "nested-type" => {
            if base.name == target.name {
                Some(0)
            } else if base.type_name == target.type_name {
                Some(1)
            } else {
                None
            }
        }
        "base" | "virtual-base" => (base.type_name == target.type_name).then_some(0),
        "vftable-pointer" => Some(0),
        _ => None,
    }
}

fn compare_class_entry(
    base: &ClassEntry,
    target: &ClassEntry,
    differences: &mut Vec<ClassDifference>,
) {
    let member = Some(target.name.clone());
    if base.name != target.name {
        differences.push(class_difference(
            entry_name_category(target.kind),
            member.clone(),
            &base.name,
            &target.name,
        ));
    }
    if base.type_name != target.type_name {
        differences.push(class_difference(
            entry_type_category(target.kind),
            member.clone(),
            &base.type_name,
            &target.type_name,
        ));
    }
    if base.access != target.access {
        differences.push(class_difference(
            entry_access_category(target.kind),
            member.clone(),
            base.access,
            target.access,
        ));
    }
    if base.offset != target.offset {
        differences.push(class_difference(
            entry_layout_category(target.kind),
            member.clone(),
            optional_hex(base.offset),
            optional_hex(target.offset),
        ));
    }
    if base.raw_attributes != target.raw_attributes {
        differences.push(class_difference(
            entry_attribute_category(target.kind),
            member.clone(),
            attribute_summary(base),
            attribute_summary(target),
        ));
    }
    if base.vtable_offset != target.vtable_offset {
        differences.push(class_difference(
            "method-vtable-slot",
            member.clone(),
            optional_hex(base.vtable_offset.map(u64::from)),
            optional_hex(target.vtable_offset.map(u64::from)),
        ));
    }
    if base.details != target.details {
        differences.push(class_difference(
            entry_details_category(target.kind),
            member,
            base.details.join(" "),
            target.details.join(" "),
        ));
    }
}

fn append_order_differences(
    pairs: &[(usize, usize)],
    base: &[ClassEntry],
    target: &[ClassEntry],
    differences: &mut Vec<ClassDifference>,
) {
    if pairs.len() < 2 {
        return;
    }
    let mut target_order: Vec<usize> = pairs.iter().map(|(target, _)| *target).collect();
    target_order.sort_unstable();
    let mut base_order = pairs.to_vec();
    base_order.sort_unstable_by_key(|(_, base)| *base);
    let base_order: Vec<usize> = base_order.into_iter().map(|(target, _)| target).collect();
    let stable = lcs_values(&base_order, &target_order);

    for (target_index, base_index) in pairs {
        if !stable.contains(target_index) {
            differences.push(class_difference(
                "declaration-order",
                Some(target[*target_index].name.clone()),
                format!("#{} {}", base_index, entry_short(&base[*base_index])),
                format!("#{} {}", target_index, entry_short(&target[*target_index])),
            ));
        }
    }
}

fn lcs_values(left: &[usize], right: &[usize]) -> HashSet<usize> {
    let mut lengths = vec![vec![0usize; right.len() + 1]; left.len() + 1];
    for left_index in (0..left.len()).rev() {
        for right_index in (0..right.len()).rev() {
            lengths[left_index][right_index] = if left[left_index] == right[right_index] {
                lengths[left_index + 1][right_index + 1] + 1
            } else {
                lengths[left_index + 1][right_index].max(lengths[left_index][right_index + 1])
            };
        }
    }
    let mut stable = HashSet::new();
    let (mut left_index, mut right_index) = (0usize, 0usize);
    while left_index < left.len() && right_index < right.len() {
        if left[left_index] == right[right_index] {
            stable.insert(left[left_index]);
            left_index += 1;
            right_index += 1;
        } else if lengths[left_index + 1][right_index] >= lengths[left_index][right_index + 1] {
            left_index += 1;
        } else {
            right_index += 1;
        }
    }
    stable
}

fn class_difference(
    category: &'static str,
    member: Option<String>,
    base: impl ToString,
    target: impl ToString,
) -> ClassDifference {
    ClassDifference {
        category,
        member,
        base: Some(base.to_string()),
        target: Some(target.to_string()),
    }
}

fn optional_hex(value: Option<u64>) -> String {
    value.map_or_else(|| "none".to_owned(), |value| format!("0x{value:x}"))
}

fn attribute_summary(entry: &ClassEntry) -> String {
    if entry.attributes.is_empty() {
        entry.raw_attributes.clone()
    } else {
        format!("{} ({})", entry.attributes.join(","), entry.raw_attributes)
    }
}

fn entry_short(entry: &ClassEntry) -> String {
    format!("{} {}", entry.kind, entry.name)
}

fn entry_summary(entry: &ClassEntry) -> String {
    let mut value = format!(
        "{} {} : {} access={}",
        entry.kind, entry.name, entry.type_name, entry.access
    );
    if let Some(offset) = entry.offset {
        let _ = write!(value, " offset=0x{offset:x}");
    }
    if let Some(slot) = entry.vtable_offset {
        let _ = write!(value, " vtable_offset=0x{slot:x}");
    }
    if !entry.attributes.is_empty() {
        let _ = write!(value, " attributes={}", entry.attributes.join(","));
    }
    value
}

fn entry_presence_category(kind: &str) -> &'static str {
    match kind {
        "base" | "virtual-base" => "inheritance-presence",
        "field" => "field-presence",
        "static-field" => "static-field-presence",
        "method" => "method-presence",
        "nested-type" => "nested-type-presence",
        "vftable-pointer" => "vftable-presence",
        _ => "declaration-presence",
    }
}

fn entry_name_category(kind: &str) -> &'static str {
    match kind {
        "field" => "field-name",
        "static-field" => "static-field-name",
        "method" => "method-name",
        "nested-type" => "nested-type-name",
        _ => "declaration-name",
    }
}

fn entry_type_category(kind: &str) -> &'static str {
    match kind {
        "base" | "virtual-base" => "inheritance-type",
        "field" => "field-type",
        "static-field" => "static-field-type",
        "method" => "method-signature",
        "nested-type" => "nested-type",
        "vftable-pointer" => "vftable-type",
        _ => "declaration-type",
    }
}

fn entry_access_category(kind: &str) -> &'static str {
    match kind {
        "base" | "virtual-base" => "inheritance-access",
        "field" => "field-visibility",
        "static-field" => "static-field-visibility",
        "method" => "method-visibility",
        "nested-type" => "nested-type-visibility",
        _ => "declaration-visibility",
    }
}

fn entry_layout_category(kind: &str) -> &'static str {
    match kind {
        "base" | "virtual-base" => "inheritance-layout",
        "field" => "field-offset",
        _ => "declaration-offset",
    }
}

fn entry_attribute_category(kind: &str) -> &'static str {
    match kind {
        "base" | "virtual-base" => "inheritance-attributes",
        "field" => "field-attributes",
        "static-field" => "static-field-attributes",
        "method" => "method-qualifiers",
        "nested-type" => "nested-type-attributes",
        _ => "declaration-attributes",
    }
}

fn entry_details_category(kind: &str) -> &'static str {
    match kind {
        "base" | "virtual-base" => "inheritance-layout",
        "method" => "method-qualifiers",
        _ => "declaration-details",
    }
}

fn render_class_report(report: &ClassReport, show_identical: bool) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "target PDB: {}", report.target_pdb);
    let _ = writeln!(out, "base PDB:   {}", report.base_pdb);
    if let Some(filter) = &report.class_filter {
        let _ = writeln!(out, "class filter: {filter:?}");
    }
    let _ = writeln!(
        out,
        "target classes={} compared={} identical={} different={} missing-base={} base-only={}",
        report.target_classes,
        report.compared_classes,
        report.identical_classes,
        report.different_classes,
        report.missing_base_classes,
        report.base_only_classes,
    );
    let _ = writeln!(
        out,
        "deduplicated variants beyond canonical: target={} base={}",
        report.target_duplicate_variants, report.base_duplicate_variants,
    );
    let _ = writeln!(
        out,
        "unresolved declaration types: target={} base={}",
        report.target_unresolved_types, report.base_unresolved_types,
    );
    if !report.difference_counts.is_empty() {
        let _ = writeln!(out, "difference counts:");
        for (category, count) in &report.difference_counts {
            let _ = writeln!(out, "  {category}: {count}");
        }
    }

    for class in &report.classes {
        if class.status == "identical" && !show_identical {
            continue;
        }
        let _ = writeln!(out, "\n=== {} [{}] ===", class.name, class.status);
        if class.target_variants > 1 || class.base_variants > 1 {
            let _ = writeln!(
                out,
                "  PDB variants: target={} base={} (richest/latest compared)",
                class.target_variants, class.base_variants,
            );
        }
        if class.differences.is_empty() {
            let _ = writeln!(out, "  no semantic differences");
            continue;
        }
        for difference in &class.differences {
            let member = difference
                .member
                .as_ref()
                .map_or(String::new(), |name| format!(" {name}"));
            let _ = writeln!(out, "  [{}]{member}", difference.category);
            if let Some(base) = &difference.base {
                let _ = writeln!(out, "    - base:   {base}");
            }
            if let Some(target) = &difference.target {
                let _ = writeln!(out, "    + target: {target}");
            }
        }
    }
    out
}

#[derive(serde::Serialize)]
struct DiffReport {
    target_pdb: String,
    base_pdb: String,
    target_matches: usize,
    base_matches: usize,
    pairs: Vec<MatchDiff>,
}

#[derive(serde::Serialize)]
struct MatchDiff {
    procedure: String,
    target_module: Option<String>,
    base_module: Option<String>,
    status: &'static str,
    sections: Vec<DiffSection>,
}

#[derive(serde::Serialize)]
struct DiffSection {
    name: &'static str,
    confidence: &'static str,
    differences: Vec<DiffLine>,
}

#[derive(serde::Serialize)]
struct DiffLine {
    kind: &'static str,
    base: Option<String>,
    target: Option<String>,
}

struct ComparableRow {
    key: String,
    value: String,
}

fn module_leaf(name: &str) -> &str {
    name.rsplit(['\\', '/']).next().unwrap_or(name)
}

fn build_diff_report(
    target_pdb: &PathBuf,
    base_pdb: &PathBuf,
    target: Vec<Match>,
    base: Vec<Match>,
    context: usize,
) -> DiffReport {
    let target_matches = target.len();
    let base_matches = base.len();
    let mut base: Vec<Option<Match>> = base.into_iter().map(Some).collect();
    let mut pairs = Vec::new();

    for target_match in target {
        let exact = base.iter().position(|candidate| {
            candidate.as_ref().is_some_and(|base_match| {
                base_match.procedure_name == target_match.procedure_name
                    && module_leaf(&base_match.module_name)
                        .eq_ignore_ascii_case(module_leaf(&target_match.module_name))
            })
        });
        let by_name = exact.or_else(|| {
            base.iter().position(|candidate| {
                candidate.as_ref().is_some_and(|base_match| {
                    base_match.procedure_name == target_match.procedure_name
                })
            })
        });
        let base_match = by_name.and_then(|position| base[position].take());
        pairs.push(build_match_diff(Some(target_match), base_match, context));
    }
    for base_match in base.into_iter().flatten() {
        pairs.push(build_match_diff(None, Some(base_match), context));
    }

    DiffReport {
        target_pdb: target_pdb.display().to_string(),
        base_pdb: base_pdb.display().to_string(),
        target_matches,
        base_matches,
        pairs,
    }
}

fn build_match_diff(target: Option<Match>, base: Option<Match>, context: usize) -> MatchDiff {
    let procedure = target
        .as_ref()
        .or(base.as_ref())
        .map(|found| found.procedure_name.clone())
        .unwrap_or_default();
    let target_module = target.as_ref().map(|found| found.module_name.clone());
    let base_module = base.as_ref().map(|found| found.module_name.clone());
    let status = match (&target, &base) {
        (Some(_), Some(_)) => "paired",
        (Some(_), None) => "target-only",
        (None, Some(_)) => "base-only",
        (None, None) => unreachable!(),
    };

    let sections = if let (Some(target), Some(base)) = (&target, &base) {
        vec![
            diff_section(
                "explicit procedure evidence",
                "high",
                owned_record_rows(base),
                owned_record_rows(target),
            ),
            diff_section(
                "line-program geometry",
                "high",
                line_rows(base),
                line_rows(target),
            ),
            diff_section(
                "TPI record neighborhood",
                "heuristic/linker-deduplicated",
                indexed_rows(&base.type_rows, normalize_type_row),
                indexed_rows(&target.type_rows, normalize_type_row),
            ),
            diff_section(
                "class field-list binding",
                "high",
                declaration_rows(base),
                declaration_rows(target),
            ),
            diff_section(
                "physical record adjacency",
                "heuristic",
                physical_rows(base, context),
                physical_rows(target, context),
            ),
            diff_section(
                "top-level record neighborhood",
                "medium/heuristic",
                top_level_rows(base, context),
                top_level_rows(target, context),
            ),
        ]
    } else {
        vec![DiffSection {
            name: "procedure pairing",
            confidence: "high",
            differences: vec![DiffLine {
                kind: status,
                base: base.as_ref().map(|found| found.procedure_name.clone()),
                target: target.as_ref().map(|found| found.procedure_name.clone()),
            }],
        }]
    };

    MatchDiff {
        procedure,
        target_module,
        base_module,
        status,
        sections,
    }
}

fn strip_tokens(text: &str, prefixes: &[&str]) -> String {
    text.split_whitespace()
        .filter(|token| !prefixes.iter().any(|prefix| token.starts_with(prefix)))
        .collect::<Vec<_>>()
        .join(" ")
}

fn replace_hex_ids(text: &str) -> String {
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len());
    let mut position = 0usize;
    while position < bytes.len() {
        if bytes[position] == b'0'
            && bytes.get(position + 1) == Some(&b'x')
            && bytes
                .get(position + 2)
                .is_some_and(|byte| byte.is_ascii_hexdigit())
        {
            out.push_str("0x#");
            position += 2;
            while bytes
                .get(position)
                .is_some_and(|byte| byte.is_ascii_hexdigit())
            {
                position += 1;
            }
        } else {
            out.push(bytes[position] as char);
            position += 1;
        }
    }
    out
}

fn clean_record(record: &Record) -> String {
    let detail = strip_tokens(
        &record.detail,
        &["rva=", "type=", "parent=", "end=", "next=", "handler="],
    );
    format!("depth={} {} {}", record.depth, record.kind, detail)
        .trim_end()
        .to_owned()
}

fn record_key(record: &Record) -> String {
    let identity = match record.kind {
        "Procedure" => record
            .detail
            .split(" rva=")
            .next()
            .unwrap_or(&record.detail),
        "BasePointerRelative"
        | "RegisterRelative"
        | "RegisterVariable"
        | "Data"
        | "Constant"
        | "UserDefinedType" => record.detail.split(" : ").next().unwrap_or(&record.detail),
        "CallSiteInfo" => record
            .detail
            .split(" signature=")
            .nth(1)
            .and_then(|value| value.split(" type=").next())
            .unwrap_or("indirect"),
        _ => "",
    };
    format!("{}:{identity}", record.kind)
}

fn record_row(record: &Record) -> ComparableRow {
    ComparableRow {
        key: record_key(record),
        value: clean_record(record),
    }
}

fn owned_record_rows(found: &Match) -> Vec<ComparableRow> {
    found
        .records
        .iter()
        .skip(found.procedure_pos)
        .take_while(|record| record.index <= found.procedure_end)
        .map(record_row)
        .collect()
}

fn line_value(line: &str) -> String {
    strip_tokens(line, &["rva=", "source="])
}

fn line_rows(found: &Match) -> Vec<ComparableRow> {
    found
        .lines
        .iter()
        .enumerate()
        .map(|(index, line)| ComparableRow {
            key: format!("line#{index}"),
            value: line_value(line),
        })
        .collect()
}

fn normalize_type_row(row: &str) -> String {
    let without_index = strip_tokens(row.trim_start_matches([' ', '>']), &["type="]);
    let mut tokens = without_index.split_whitespace();
    let mut normalized = Vec::new();
    if let Some(raw) = tokens.next() {
        normalized.push(raw.to_owned());
    }
    normalized.extend(tokens.map(replace_hex_ids));
    normalized.join(" ")
}

fn indexed_rows(rows: &[String], normalize: fn(&str) -> String) -> Vec<ComparableRow> {
    rows.iter()
        .enumerate()
        .map(|(index, row)| ComparableRow {
            key: format!("row#{index}"),
            value: normalize(row),
        })
        .collect()
}

fn declaration_value(row: &str) -> String {
    replace_hex_ids(&strip_tokens(row, &["type="]))
}

fn declaration_key(row: &str) -> String {
    let normalized = declaration_value(row);
    strip_tokens(normalized.trim_start_matches([' ', '>']), &["field#"])
}

fn declaration_rows(found: &Match) -> Vec<ComparableRow> {
    found
        .declaration_rows
        .iter()
        .map(|row| ComparableRow {
            key: declaration_key(row),
            value: declaration_value(row),
        })
        .collect()
}

fn physical_rows(found: &Match, context: usize) -> Vec<ComparableRow> {
    let lo = found.procedure_pos.saturating_sub(context);
    let hi = (found.procedure_pos + context + 1).min(found.records.len());
    found.records[lo..hi].iter().map(record_row).collect()
}

fn top_level_rows(found: &Match, context: usize) -> Vec<ComparableRow> {
    let top: Vec<(usize, &Record)> = found
        .records
        .iter()
        .enumerate()
        .filter(|(_, record)| record.depth == 0)
        .collect();
    let Some(center) = top
        .iter()
        .position(|(position, _)| *position == found.procedure_pos)
    else {
        return Vec::new();
    };
    let lo = center.saturating_sub(context);
    let hi = (center + context + 1).min(top.len());
    top[lo..hi]
        .iter()
        .map(|(_, record)| record_row(record))
        .collect()
}

fn diff_section(
    name: &'static str,
    confidence: &'static str,
    base: Vec<ComparableRow>,
    target: Vec<ComparableRow>,
) -> DiffSection {
    DiffSection {
        name,
        confidence,
        differences: diff_rows(&base, &target),
    }
}

fn diff_rows(base: &[ComparableRow], target: &[ComparableRow]) -> Vec<DiffLine> {
    let mut lcs = vec![vec![0usize; target.len() + 1]; base.len() + 1];
    for base_index in (0..base.len()).rev() {
        for target_index in (0..target.len()).rev() {
            lcs[base_index][target_index] = if base[base_index].key == target[target_index].key {
                lcs[base_index + 1][target_index + 1] + 1
            } else {
                lcs[base_index + 1][target_index].max(lcs[base_index][target_index + 1])
            };
        }
    }

    let mut differences = Vec::new();
    let (mut base_index, mut target_index) = (0usize, 0usize);
    while base_index < base.len() || target_index < target.len() {
        if base_index < base.len()
            && target_index < target.len()
            && base[base_index].key == target[target_index].key
        {
            if base[base_index].value != target[target_index].value {
                differences.push(DiffLine {
                    kind: "changed",
                    base: Some(base[base_index].value.clone()),
                    target: Some(target[target_index].value.clone()),
                });
            }
            base_index += 1;
            target_index += 1;
        } else if target_index == target.len()
            || (base_index < base.len()
                && lcs[base_index + 1][target_index] >= lcs[base_index][target_index + 1])
        {
            differences.push(DiffLine {
                kind: "removed",
                base: Some(base[base_index].value.clone()),
                target: None,
            });
            base_index += 1;
        } else {
            differences.push(DiffLine {
                kind: "added",
                base: None,
                target: Some(target[target_index].value.clone()),
            });
            target_index += 1;
        }
    }
    differences
}

fn render_diff_report(report: &DiffReport) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "target PDB: {}", report.target_pdb);
    let _ = writeln!(out, "base PDB:   {}", report.base_pdb);
    let _ = writeln!(
        out,
        "matched target={} base={} paired={}",
        report.target_matches,
        report.base_matches,
        report
            .pairs
            .iter()
            .filter(|pair| pair.status == "paired")
            .count()
    );

    for pair in &report.pairs {
        let _ = writeln!(out, "\n=== {} [{}] ===", pair.procedure, pair.status);
        if let Some(module) = &pair.target_module {
            let _ = writeln!(out, "+ target module: {module}");
        }
        if let Some(module) = &pair.base_module {
            let _ = writeln!(out, "- base module:   {module}");
        }
        for section in &pair.sections {
            let _ = writeln!(
                out,
                "\n[{} — {} confidence]",
                section.name, section.confidence
            );
            if section.differences.is_empty() {
                let _ = writeln!(out, "  no semantic differences");
                continue;
            }
            for difference in &section.differences {
                if let Some(base) = &difference.base {
                    let _ = writeln!(out, "- {base}");
                }
                if let Some(target) = &difference.target {
                    let _ = writeln!(out, "+ {target}");
                }
            }
        }
    }
    out
}

#[derive(Clone)]
struct Declaration {
    method_type: Option<u32>,
    text: String,
}

fn method_attributes(attributes: pdb::FieldAttributes) -> String {
    let access = match attributes.access() {
        1 => "private",
        2 => "protected",
        3 => "public",
        _ => "none",
    };
    format!(
        "access={access} static={} virtual={} pure={} intro={} compiler_generated={} sealed={}",
        attributes.is_static(),
        attributes.is_virtual(),
        attributes.is_pure_virtual(),
        attributes.is_intro_virtual(),
        attributes.is_compgenx(),
        attributes.sealed(),
    )
}

fn bind_declarations(
    types: &pdb::TypeInformation<'_>,
    matches: &mut [Match],
    context: usize,
) -> pdb::Result<()> {
    let wanted: HashSet<u32> = matches.iter().map(|m| m.procedure_type).collect();
    let mut finder = types.finder();
    let mut classes = Vec::new();
    let mut iter = types.iter();
    while let Some(ty) = iter.next()? {
        finder.update(&iter);
        if let Ok(pdb::TypeData::Class(class)) = ty.parse()
            && !class.properties.forward_reference()
            && let Some(fields) = class.fields
        {
            classes.push((class.name.to_string().into_owned(), fields));
        }
    }

    let mut bindings: HashMap<u32, Vec<Vec<String>>> = HashMap::new();
    for (class_name, first_fields) in classes {
        let mut declarations = Vec::new();
        let mut fields = Some(first_fields);
        let mut seen = HashSet::new();
        while let Some(field_index) = fields {
            if !seen.insert(field_index.0) {
                break;
            }
            let Ok(pdb::TypeData::FieldList(list)) = finder.find(field_index)?.parse() else {
                break;
            };
            fields = list.continuation;
            for field in list.fields {
                match field {
                    pdb::TypeData::Method(method) => declarations.push(Declaration {
                        method_type: Some(method.method_type.0),
                        text: format!(
                            "method {} type=0x{:x} {} vslot={:?}",
                            method.name,
                            method.method_type.0,
                            method_attributes(method.attributes),
                            method.vtable_offset,
                        ),
                    }),
                    pdb::TypeData::OverloadedMethod(overload) => {
                        match finder.find(overload.method_list)?.parse() {
                            Ok(pdb::TypeData::MethodList(methods)) => {
                                for method in methods.methods {
                                    declarations.push(Declaration {
                                        method_type: Some(method.method_type.0),
                                        text: format!(
                                            "method {} type=0x{:x} {} vslot={:?} [overload]",
                                            overload.name,
                                            method.method_type.0,
                                            method_attributes(method.attributes),
                                            method.vtable_offset,
                                        ),
                                    });
                                }
                            }
                            _ => declarations.push(Declaration {
                                method_type: None,
                                text: format!(
                                    "unresolved overload {}: {overload:?}",
                                    overload.name
                                ),
                            }),
                        }
                    }
                    other => declarations.push(Declaration {
                        method_type: None,
                        text: format!("{other:?}"),
                    }),
                }
            }
        }

        for (position, declaration) in declarations.iter().enumerate() {
            let Some(method_type) = declaration.method_type else {
                continue;
            };
            if !wanted.contains(&method_type) {
                continue;
            }
            let lo = position.saturating_sub(context);
            let hi = (position + context + 1).min(declarations.len());
            let mut rows = Vec::new();
            rows.push(format!("candidate owner: {class_name}"));
            for (relative, item) in declarations[lo..hi].iter().enumerate() {
                rows.push(format!(
                    "{} field#{} {}",
                    if lo + relative == position { ">" } else { " " },
                    lo + relative,
                    item.text,
                ));
            }
            let candidates = bindings.entry(method_type).or_default();
            if !candidates.contains(&rows) {
                candidates.push(rows);
            }
        }
    }

    for found in matches {
        if let Some(candidates) = bindings.remove(&found.procedure_type) {
            for (i, rows) in candidates.into_iter().enumerate() {
                if i > 0 {
                    found.declaration_rows.push(String::new());
                }
                found.declaration_rows.extend(rows);
            }
        }
    }
    Ok(())
}

fn type_name(fmt: &PdbParser<'_, '_>, module_id: usize, index: pdb::TypeIndex) -> String {
    fmt.emit_type_impl(module_id, index)
        .unwrap_or_else(|_| format!("type#0x{:x}", index.0))
}

fn function_name(
    fmt: &PdbParser<'_, '_>,
    module_id: usize,
    raw: &pdb::RawString<'_>,
    index: pdb::TypeIndex,
) -> String {
    fmt.emit_function_orig(raw, module_id, index)
        .unwrap_or_else(|_| raw.to_string().into_owned())
}

fn signed_hex(value: i32) -> String {
    if value < 0 {
        format!("-0x{:x}", value.unsigned_abs())
    } else {
        format!("+0x{:x}", value as u32)
    }
}

#[allow(clippy::too_many_arguments)]
fn summarize_record(
    fmt: &PdbParser<'_, '_>,
    module_id: usize,
    address_map: &pdb::AddressMap,
    index: u32,
    raw_kind: u16,
    raw_len: usize,
    depth: usize,
    data: SymbolData<'_>,
) -> Record {
    let (kind, detail) = match data {
        SymbolData::Procedure(p) => (
            "Procedure",
            format!(
                "{} rva={} len=0x{:x} body=0x{:x}..0x{:x} type=0x{:x} global={} flags={:?} parent={:?} end=0x{:x} next={:?}",
                function_name(fmt, module_id, &p.name, p.type_index),
                p.offset
                    .to_rva(address_map)
                    .map_or_else(|| "?".into(), |v| format!("0x{:x}", v.0)),
                p.len,
                p.dbg_start_offset,
                p.dbg_end_offset,
                p.type_index.0,
                p.global,
                p.flags,
                p.parent,
                p.end.0,
                p.next,
            ),
        ),
        SymbolData::FrameProcedure(f) => (
            "FrameProcedure",
            format!(
                "frame=0x{:x} padding=0x{:x}@0x{:x} saved_regs=0x{:x} handler={:?} flags={:?}",
                f.frame_byte_count,
                f.padding_byte_count,
                f.offset_padding,
                f.callee_save_registers_byte_count,
                f.exception_handler_offset,
                f.flags,
            ),
        ),
        SymbolData::BasePointerRelative(v) => (
            "BasePointerRelative",
            format!(
                "{} : {} type=0x{:x} fp_off={} slot={:?}",
                v.name,
                type_name(fmt, module_id, v.type_index),
                v.type_index.0,
                signed_hex(v.offset),
                v.slot,
            ),
        ),
        SymbolData::RegisterRelative(v) => (
            "RegisterRelative",
            format!(
                "{} : {} type=0x{:x} {:?}{} slot={:?}",
                v.name,
                type_name(fmt, module_id, v.type_index),
                v.type_index.0,
                v.register,
                signed_hex(v.offset),
                v.slot,
            ),
        ),
        SymbolData::RegisterVariable(v) => (
            "RegisterVariable",
            format!(
                "{} : {} type=0x{:x} register={:?} slot={:?}",
                v.name,
                type_name(fmt, module_id, v.type_index),
                v.type_index.0,
                v.register,
                v.slot,
            ),
        ),
        SymbolData::UserDefinedType(v) => (
            "UserDefinedType",
            format!(
                "{} = {} type=0x{:x}",
                v.name,
                type_name(fmt, module_id, v.type_index),
                v.type_index.0,
            ),
        ),
        SymbolData::Data(v) => (
            "Data",
            format!(
                "{} : {} type=0x{:x} rva={} global={} managed={}",
                v.name,
                type_name(fmt, module_id, v.type_index),
                v.type_index.0,
                v.offset
                    .to_rva(address_map)
                    .map_or_else(|| "?".into(), |r| format!("0x{:x}", r.0)),
                v.global,
                v.managed,
            ),
        ),
        SymbolData::Constant(v) => (
            "Constant",
            format!(
                "{} : {} type=0x{:x} value={:?}",
                v.name,
                type_name(fmt, module_id, v.type_index),
                v.type_index.0,
                v.value,
            ),
        ),
        SymbolData::CallSiteInfo(v) => (
            "CallSiteInfo",
            format!(
                "rva={} signature={} type=0x{:x}",
                v.offset
                    .to_rva(address_map)
                    .map_or_else(|| "?".into(), |r| format!("0x{:x}", r.0)),
                function_name(
                    fmt,
                    module_id,
                    &pdb::RawString::from("<indirect>"),
                    v.type_index
                ),
                v.type_index.0,
            ),
        ),
        SymbolData::Block(v) => (
            "Block",
            format!(
                "rva={} len=0x{:x} parent=0x{:x} end=0x{:x} name={}",
                v.offset
                    .to_rva(address_map)
                    .map_or_else(|| "?".into(), |r| format!("0x{:x}", r.0)),
                v.len,
                v.parent.0,
                v.end.0,
                v.name,
            ),
        ),
        SymbolData::FrameCookie(v) => ("FrameCookie", format!("{v:?}")),
        SymbolData::Label(v) => ("Label", format!("{v:?}")),
        SymbolData::Thunk(v) => ("Thunk", format!("{v:?}")),
        SymbolData::CompileFlags(v) => ("CompileFlags", format!("{v:?}")),
        SymbolData::EnvBlock(v) => ("EnvBlock", format!("{v:?}")),
        SymbolData::ObjName(v) => ("ObjName", format!("{v:?}")),
        SymbolData::UsingNamespace(v) => ("UsingNamespace", format!("{v:?}")),
        SymbolData::ScopeEnd => ("ScopeEnd", String::new()),
        other => ("Other", format!("{other:?}")),
    };
    Record {
        index,
        raw_kind,
        raw_len,
        depth,
        kind,
        detail,
    }
}

fn summarize_type(data: pdb::Result<pdb::TypeData<'_>>) -> String {
    match data {
        Ok(pdb::TypeData::MemberFunction(v)) => format!(
            "MemberFunction return=0x{:x} class=0x{:x} this={:?} argc={} args=0x{:x} attrs={:?} this_adjust={}",
            v.return_type.0,
            v.class_type.0,
            v.this_pointer_type,
            v.parameter_count,
            v.argument_list.0,
            v.attributes,
            v.this_adjustment,
        ),
        Ok(pdb::TypeData::Procedure(v)) => format!(
            "Procedure return={:?} argc={} args=0x{:x} attrs={:?}",
            v.return_type, v.parameter_count, v.argument_list.0, v.attributes,
        ),
        Ok(pdb::TypeData::ArgumentList(v)) => format!("ArgumentList {:?}", v.arguments),
        Ok(pdb::TypeData::Pointer(v)) => format!(
            "Pointer underlying=0x{:x} containing={:?} attrs={:?}",
            v.underlying_type.0, v.containing_class, v.attributes,
        ),
        Ok(pdb::TypeData::Modifier(v)) => format!(
            "Modifier underlying=0x{:x} const={} volatile={} unaligned={}",
            v.underlying_type.0, v.constant, v.volatile, v.unaligned,
        ),
        Ok(pdb::TypeData::Class(v)) => format!(
            "{:?} {} size=0x{:x} fields={:?} derived={:?} vshape={:?} props={:?}",
            v.kind, v.name, v.size, v.fields, v.derived_from, v.vtable_shape, v.properties,
        ),
        Ok(pdb::TypeData::Enumeration(v)) => format!(
            "Enum {} underlying=0x{:x} fields=0x{:x} props={:?}",
            v.name, v.underlying_type.0, v.fields.0, v.properties,
        ),
        Ok(pdb::TypeData::Union(v)) => format!(
            "Union {} size=0x{:x} fields=0x{:x} props={:?}",
            v.name, v.size, v.fields.0, v.properties,
        ),
        Ok(pdb::TypeData::Array(v)) => format!(
            "Array element=0x{:x} index=0x{:x} dims={:?}",
            v.element_type.0, v.indexing_type.0, v.dimensions,
        ),
        Ok(pdb::TypeData::FieldList(v)) => format!(
            "FieldList entries={} continuation={:?}",
            v.fields.len(),
            v.continuation,
        ),
        Ok(other) => format!("{other:?}"),
        Err(error) => format!("unparsed: {error}"),
    }
}

fn render_record(out: &mut String, record: &Record, marker: &str) {
    let _ = writeln!(
        out,
        "{marker} sym=0x{:06x} raw=0x{:04x}/0x{:x} depth={} {:<22} {}",
        record.index, record.raw_kind, record.raw_len, record.depth, record.kind, record.detail,
    );
}

fn render_match(found: &Match, context: usize) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "\n=== module #{} ===", found.module_id);
    let _ = writeln!(out, "module: {}", found.module_name);
    let _ = writeln!(out, "object/library: {}", found.object_file_name);

    let selected = &found.records[found.procedure_pos];
    let _ = writeln!(out, "\n[explicit procedure evidence — high confidence]");
    render_record(&mut out, selected, ">");
    for record in found.records.iter().skip(found.procedure_pos + 1) {
        if record.index > found.procedure_end {
            break;
        }
        render_record(&mut out, record, " ");
    }

    let _ = writeln!(out, "\n[line-program geometry — high confidence]");
    for line in &found.lines {
        let _ = writeln!(out, "  {line}");
    }

    let _ = writeln!(
        out,
        "\n[TPI record neighborhood — heuristic only; linker-deduplicated]"
    );
    for row in &found.type_rows {
        let _ = writeln!(out, "{row}");
    }

    let _ = writeln!(
        out,
        "\n[class field-list binding — high confidence; duplicate signatures may have several candidates]"
    );
    if found.declaration_rows.is_empty() {
        let _ = writeln!(
            out,
            "  no class field-list entry references type 0x{:x}",
            found.procedure_type
        );
    } else {
        for row in &found.declaration_rows {
            let _ = writeln!(out, "{row}");
        }
    }

    let lo = found.procedure_pos.saturating_sub(context);
    let hi = (found.procedure_pos + context + 1).min(found.records.len());
    let _ = writeln!(out, "\n[physical record adjacency — heuristic only]");
    for (pos, record) in found.records[lo..hi].iter().enumerate() {
        render_record(
            &mut out,
            record,
            if lo + pos == found.procedure_pos {
                ">"
            } else {
                " "
            },
        );
    }

    let top: Vec<(usize, &Record)> = found
        .records
        .iter()
        .enumerate()
        .filter(|(_, r)| r.depth == 0)
        .collect();
    if let Some(center) = top.iter().position(|(i, _)| *i == found.procedure_pos) {
        let lo = center.saturating_sub(context);
        let hi = (center + context + 1).min(top.len());
        let _ = writeln!(out, "\n[top-level record neighborhood — medium/heuristic]");
        for (pos, (original, record)) in top[lo..hi].iter().enumerate() {
            render_record(
                &mut out,
                record,
                if lo + pos == center && *original == found.procedure_pos {
                    ">"
                } else {
                    " "
                },
            );
        }
    }

    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn cross_pdb_type_normalization_drops_formatter_only_spelling() {
        assert_eq!(
            normalize_cross_pdb_type(
                "boost::f<enum vostok::kind,class vostok::item const , struct X>".to_owned()
            ),
            "boost::f<vostok::kind,vostok::item const, X>"
        );
        assert_eq!(
            normalize_cross_pdb_type("bitfield TypeIndex(0x1234)".to_owned()),
            "bitfield TypeIndex(..)"
        );
    }

    #[test]
    fn declaration_order_lcs_ignores_insertions_but_finds_moves() {
        assert_eq!(lcs_values(&[0, 1, 2], &[0, 1, 2]).len(), 3);
        let stable = lcs_values(&[0, 2, 1], &[0, 1, 2]);
        assert_eq!(stable.len(), 2);
        assert!(stable.contains(&0));
    }
}
