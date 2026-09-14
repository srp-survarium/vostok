// SPDX-License-Identifier: GPL-3.0-or-later

//! Query and compare raw CodeView function/class topology without flattening the PDB.

use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};
use std::fmt::Write as _;
use std::io::Write as _;
use std::path::PathBuf;

use crate::msf_layout::{MsfLayout, MsfStreamLayout};
use crate::pdb_parser::PdbParser;
use clap::{ArgGroup, Args};
use pdb::{FallibleIterator, SymbolData};

#[derive(Args)]
#[command(group(
    ArgGroup::new("input")
        .required(true)
        .args(["pdb", "target_pdb"])
), group(
    ArgGroup::new("query")
        .required(true)
        .args(["function", "classes", "order"])
))]
pub struct Cli {
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
    #[arg(long, conflicts_with_all = ["classes", "order"])]
    function: Option<String>,

    /// Compare every complete target class/struct/interface against the base PDB.
    #[arg(
        long,
        requires = "target_pdb",
        conflicts_with_all = ["pdb", "function", "module", "order"]
    )]
    classes: bool,

    /// Compare whole-PDB record sequences without treating linker order as source order.
    #[arg(
        long,
        requires = "target_pdb",
        conflicts_with_all = ["pdb", "function", "classes", "class_filter", "show_identical"]
    )]
    order: bool,

    /// Maximum number of order differences printed per sequence (JSON is uncapped).
    #[arg(long, default_value_t = 100, requires = "order")]
    limit: usize,

    /// Restrict --classes to one case-insensitive qualified class name.
    #[arg(long = "class", requires = "classes")]
    class_filter: Option<String>,

    /// Include classes with no semantic differences in --classes text output.
    #[arg(long, requires = "classes")]
    show_identical: bool,

    /// Optional case-insensitive module/object/library substring. With --order,
    /// emit focused DBI module and local file/C13 sequences.
    #[arg(long)]
    module: Option<String>,

    /// Optional repeatable named-type identity substring. With --order,
    /// emit only matching named TPI records and their direct module references.
    #[arg(
        long = "type",
        value_name = "SUBSTRING",
        requires = "order",
        conflicts_with = "module"
    )]
    type_filters: Vec<String>,

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

pub fn execute(cli: Cli) -> crate::Result<()> {
    run(&cli)
}

fn run(cli: &Cli) -> crate::Result<()> {
    let output = if cli.order {
        let target_pdb = cli.target_pdb.as_ref().expect("clap requires target PDB");
        let base_pdb = cli.base_pdb.as_ref().expect("clap requires base PDB");
        if !cli.type_filters.is_empty() {
            let report = build_focused_type_order_report(target_pdb, base_pdb, &cli.type_filters)?;
            if cli.json {
                serde_json::to_string_pretty(&report).unwrap_or_else(|error| {
                    format!(
                        "{{\"error\":{}}}\n",
                        serde_json::Value::String(error.to_string())
                    )
                })
            } else {
                render_focused_type_order_report(&report, cli.limit)
            }
        } else if let Some(module) = &cli.module {
            let report = build_focused_module_order_report(target_pdb, base_pdb, module)?;
            if cli.json {
                serde_json::to_string_pretty(&report).unwrap_or_else(|error| {
                    format!(
                        "{{\"error\":{}}}\n",
                        serde_json::Value::String(error.to_string())
                    )
                })
            } else {
                render_focused_module_order_report(&report, cli.limit)
            }
        } else {
            let report = build_order_report(target_pdb, base_pdb)?;
            if cli.json {
                serde_json::to_string_pretty(&report).unwrap_or_else(|error| {
                    format!(
                        "{{\"error\":{}}}\n",
                        serde_json::Value::String(error.to_string())
                    )
                })
            } else {
                render_order_report(&report, cli.limit)
            }
        }
    } else if cli.classes {
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

fn load_matches(cli: &Cli, pdb: &PathBuf) -> crate::Result<Vec<Match>> {
    let mut matches = Vec::new();
    PdbParser::with(pdb, |fmt| {
        matches = inspect(cli, pdb, &fmt)?;
        Ok(())
    })?;
    Ok(matches)
}

fn inspect(cli: &Cli, pdb_path: &PathBuf, fmt: &PdbParser<'_, '_>) -> crate::Result<Vec<Match>> {
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
// Whole-PDB record-order comparison

#[derive(Clone, Debug, serde::Serialize)]
struct OrderItem {
    key: String,
    value: String,
    comparison_value: String,
}

#[derive(Default, serde::Serialize)]
struct OrderSide {
    #[serde(skip)]
    layout: Option<MsfLayout>,
    msf_free_pages: Vec<OrderItem>,
    stream_roles: Vec<StreamRoleBinding>,
    dbi_substreams: Vec<OrderItem>,
    dbi_ec_names: Vec<OrderItem>,
    dbi_ec_metadata: Vec<OrderItem>,
    dbi_ec_hash_buckets: Vec<OrderItem>,
    named_streams: Vec<OrderItem>,
    named_stream_buckets: Vec<OrderItem>,
    deleted_named_stream_buckets: Vec<OrderItem>,
    pdb_features: Vec<OrderItem>,
    string_table_names: Vec<OrderItem>,
    string_table_metadata: Vec<OrderItem>,
    string_table_hash_buckets: Vec<OrderItem>,
    modules: Vec<OrderItem>,
    section_contributions: Vec<OrderItem>,
    section_map: Vec<OrderItem>,
    section_headers: Vec<OrderItem>,
    legacy_fpo_records: Vec<OrderItem>,
    frame_data_records: Vec<OrderItem>,
    raw_type_records: Vec<OrderItem>,
    raw_id_records: Vec<OrderItem>,
    tpi_metadata: Vec<OrderItem>,
    tpi_hash_values: Vec<OrderItem>,
    tpi_index_offsets: Vec<OrderItem>,
    tpi_hash_adjustments: Vec<OrderItem>,
    ipi_metadata: Vec<OrderItem>,
    ipi_hash_values: Vec<OrderItem>,
    ipi_index_offsets: Vec<OrderItem>,
    ipi_hash_adjustments: Vec<OrderItem>,
    named_types: Vec<OrderItem>,
    named_type_module_references: Vec<OrderItem>,
    enum_values: Vec<ModuleOrderScope>,
    raw_global_symbols: Vec<OrderItem>,
    global_symbols: Vec<OrderItem>,
    global_hash_records: Vec<OrderItem>,
    global_hash_buckets: Vec<OrderItem>,
    public_hash_records: Vec<OrderItem>,
    public_hash_buckets: Vec<OrderItem>,
    public_address_map: Vec<OrderItem>,
    public_thunk_map: Vec<OrderItem>,
    public_section_map: Vec<OrderItem>,
    module_symbols: Vec<ModuleOrderScope>,
    module_records: Vec<ModuleOrderScope>,
    module_files: Vec<ModuleOrderScope>,
    module_lines: Vec<ModuleOrderScope>,
    module_subsections: Vec<ModuleOrderScope>,
    module_strings: Vec<ModuleOrderScope>,
    module_frame_data: Vec<ModuleOrderScope>,
    module_inlinees: Vec<ModuleOrderScope>,
    module_cross_imports: Vec<ModuleOrderScope>,
    module_cross_exports: Vec<ModuleOrderScope>,
    module_other_c13: Vec<ModuleOrderScope>,
    dbi_source_files: Vec<ModuleOrderScope>,
}

#[derive(Clone, serde::Serialize)]
struct StreamRoleBinding {
    key: String,
    value: String,
    stream_index: u32,
}

#[derive(Clone, serde::Serialize)]
struct ModuleOrderScope {
    key: String,
    value: String,
    symbols: Vec<OrderItem>,
}

#[derive(serde::Serialize)]
struct OrderReport {
    target_pdb: String,
    base_pdb: String,
    coverage: Vec<OrderCoverage>,
    msf_layout: MsfLayoutComparison,
    msf_free_pages: SequenceComparison,
    stream_roles: SequenceComparison,
    dbi_substreams: SequenceComparison,
    dbi_source_file_streams: ScopedStreamReport,
    dbi_ec_names: SequenceComparison,
    dbi_ec_metadata: SequenceComparison,
    dbi_ec_hash_buckets: SequenceComparison,
    named_streams: SequenceComparison,
    named_stream_buckets: SequenceComparison,
    deleted_named_stream_buckets: SequenceComparison,
    pdb_features: SequenceComparison,
    string_table_names: SequenceComparison,
    string_table_metadata: SequenceComparison,
    string_table_hash_buckets: SequenceComparison,
    modules: SequenceComparison,
    module_library_sequences: Vec<ScopedSequenceSummary>,
    section_contributions: SequenceComparison,
    section_map: SequenceComparison,
    section_headers: SequenceComparison,
    legacy_fpo_records: SequenceComparison,
    frame_data_records: SequenceComparison,
    raw_type_records: SequenceComparison,
    tpi_metadata: SequenceComparison,
    tpi_hash_values: SequenceComparison,
    tpi_index_offsets: SequenceComparison,
    tpi_hash_adjustments: SequenceComparison,
    named_types: SequenceComparison,
    named_type_module_references: SequenceComparison,
    named_type_kinds: Vec<ScopedSequenceSummary>,
    enum_value_streams: ScopedStreamReport,
    raw_id_records: SequenceComparison,
    ipi_metadata: SequenceComparison,
    ipi_hash_values: SequenceComparison,
    ipi_index_offsets: SequenceComparison,
    ipi_hash_adjustments: SequenceComparison,
    raw_global_symbols: SequenceComparison,
    global_symbols: SequenceComparison,
    global_hash_records: SequenceComparison,
    global_hash_buckets: SequenceComparison,
    public_hash_records: SequenceComparison,
    public_hash_buckets: SequenceComparison,
    public_address_map: SequenceComparison,
    public_thunk_map: SequenceComparison,
    public_section_map: SequenceComparison,
    global_symbol_kinds: Vec<ScopedSequenceSummary>,
    paired_module_symbol_streams: usize,
    different_module_symbol_streams: usize,
    ambiguous_module_scopes: Vec<MultiplicityDifference>,
    module_symbols: Vec<ScopedSequenceComparison>,
    module_symbol_kinds: Vec<ScopedSequenceSummary>,
    module_record_streams: ScopedStreamReport,
    module_file_streams: ScopedStreamReport,
    module_line_streams: ScopedStreamReport,
    module_subsection_streams: ScopedStreamReport,
    module_string_streams: ScopedStreamReport,
    module_frame_data_streams: ScopedStreamReport,
    module_inlinee_streams: ScopedStreamReport,
    module_cross_import_streams: ScopedStreamReport,
    module_cross_export_streams: ScopedStreamReport,
    module_other_c13_streams: ScopedStreamReport,
}

#[derive(serde::Serialize)]
struct OrderCoverage {
    channel: &'static str,
    status: &'static str,
    note: &'static str,
}

#[derive(serde::Serialize)]
struct ScopedStreamReport {
    paired: usize,
    different: usize,
    ambiguous_scopes: Vec<MultiplicityDifference>,
    only_base_scopes: Vec<String>,
    only_target_scopes: Vec<String>,
    streams: Vec<ScopedSequenceComparison>,
}

#[derive(serde::Serialize)]
struct MsfLayoutComparison {
    confidence: &'static str,
    base: MsfLayoutSummary,
    target: MsfLayoutSummary,
    stable_roles: Vec<StableStreamLayoutComparison>,
    unidentified_base: Vec<StreamLayoutObservation>,
    unidentified_target: Vec<StreamLayoutObservation>,
}

#[derive(serde::Serialize)]
struct MsfLayoutSummary {
    format: String,
    page_size: u32,
    free_page_map: u32,
    free_page_map_pages: Vec<u32>,
    free_pages: usize,
    pages_used: u32,
    file_bytes: u64,
    directory_size: u32,
    directory_map_pages: Vec<u32>,
    directory_pages: Vec<u32>,
    directory_page_runs: usize,
    stream_slots: usize,
    present_streams: usize,
    absent_streams: usize,
    stream_bytes: u64,
    stream_pages: usize,
    stream_page_runs: usize,
    fragmented_streams: usize,
}

#[derive(serde::Serialize)]
struct StableStreamLayoutComparison {
    role: String,
    value: String,
    base: Option<StreamLayoutObservation>,
    target: Option<StreamLayoutObservation>,
}

#[derive(Clone, serde::Serialize)]
struct StreamLayoutObservation {
    stream_index: u32,
    size: Option<u32>,
    page_count: usize,
    page_runs: usize,
    pages: Vec<u32>,
}

#[derive(serde::Serialize)]
struct ScopedSequenceComparison {
    scope: String,
    comparison: SequenceComparison,
}

#[derive(serde::Serialize)]
struct ScopedSequenceSummary {
    scope: String,
    comparison: SequenceSummary,
}

#[derive(serde::Serialize)]
struct SequenceSummary {
    name: String,
    confidence: &'static str,
    different: bool,
    base_total: usize,
    target_total: usize,
    shared_unique: usize,
    order_metrics: OrderMetrics,
    only_base: usize,
    only_target: usize,
    multiplicity: usize,
    excluded_nonunique: usize,
    changed: usize,
    moved: usize,
}

#[derive(serde::Serialize)]
struct SequenceComparison {
    name: String,
    confidence: &'static str,
    base_total: usize,
    target_total: usize,
    shared_unique: usize,
    order_metrics: OrderMetrics,
    only_base: Vec<PositionedOrderItem>,
    only_target: Vec<PositionedOrderItem>,
    multiplicity: Vec<MultiplicityDifference>,
    excluded_nonunique: Vec<MultiplicityDifference>,
    changed: Vec<ChangedOrderItem>,
    moved: Vec<MovedOrderItem>,
}

#[derive(Clone, Debug, PartialEq, Eq, serde::Serialize)]
struct OrderMetrics {
    // All metrics use only keys that occur exactly once on each side. One-sided
    // and ambiguous records therefore cannot turn insertions into false moves.
    comparable_pairs: u64,
    inversions: u64,
    longest_ordered_subsequence: usize,
    preserved_adjacent_pairs: usize,
    reversed_adjacent_pairs: usize,
    longest_contiguous_run: usize,
    increasing_runs: usize,
    rank_displacement_sum: u64,
    max_rank_displacement: usize,
}

#[derive(Clone, serde::Serialize)]
struct PositionedOrderItem {
    key: String,
    value: String,
    position: usize,
}

#[derive(Clone, serde::Serialize)]
struct MultiplicityDifference {
    key: String,
    value: String,
    base_count: usize,
    target_count: usize,
}

#[derive(serde::Serialize)]
struct MovedOrderItem {
    key: String,
    value: String,
    base_position: usize,
    target_position: usize,
}

#[derive(serde::Serialize)]
struct ChangedOrderItem {
    key: String,
    base_value: String,
    target_value: String,
    base_position: usize,
    target_position: usize,
}

#[derive(serde::Serialize)]
struct FocusedTypeOrderReport {
    target_pdb: String,
    base_pdb: String,
    filters: Vec<String>,
    target_sequence: Vec<OrderItem>,
    base_sequence: Vec<OrderItem>,
    target_module_references: Vec<OrderItem>,
    base_module_references: Vec<OrderItem>,
    named_types: SequenceComparison,
    named_type_module_references: SequenceComparison,
}

#[derive(serde::Serialize)]
struct FocusedModuleOrderReport {
    target_pdb: String,
    base_pdb: String,
    filter: String,
    target_sequence: Vec<OrderItem>,
    base_sequence: Vec<OrderItem>,
    modules: SequenceComparison,
    dbi_source_file_streams: ScopedStreamReport,
    module_file_streams: ScopedStreamReport,
    module_line_streams: ScopedStreamReport,
    module_subsection_streams: ScopedStreamReport,
}

#[derive(Default)]
struct FocusedModuleOrderSide {
    modules: Vec<OrderItem>,
    dbi_source_files: Vec<ModuleOrderScope>,
    module_files: Vec<ModuleOrderScope>,
    module_lines: Vec<ModuleOrderScope>,
    module_subsections: Vec<ModuleOrderScope>,
}

fn build_focused_type_order_report(
    target_pdb: &PathBuf,
    base_pdb: &PathBuf,
    filters: &[String],
) -> crate::Result<FocusedTypeOrderReport> {
    let mut target = load_order_side(target_pdb)?;
    let mut base = load_order_side(base_pdb)?;
    let needles: Vec<String> = filters.iter().map(|filter| filter.to_lowercase()).collect();
    target
        .named_types
        .retain(|item| type_order_item_matches_filters(item, &needles));
    target
        .named_type_module_references
        .retain(|item| type_order_item_matches_filters(item, &needles));
    base.named_types
        .retain(|item| type_order_item_matches_filters(item, &needles));
    base.named_type_module_references
        .retain(|item| type_order_item_matches_filters(item, &needles));

    let named_types = compare_sequence(
        "focused named complete TPI records",
        "physical/linker-deduplicated; filtered by named-type identity",
        &base.named_types,
        &target.named_types,
    );
    let named_type_module_references = compare_sequence(
        "focused named TPI direct module references",
        "compiler/linker-emitted; earliest retained module reference is a contributor clue, not proof of original ownership",
        &base.named_type_module_references,
        &target.named_type_module_references,
    );
    Ok(FocusedTypeOrderReport {
        target_pdb: target_pdb.display().to_string(),
        base_pdb: base_pdb.display().to_string(),
        filters: filters.to_vec(),
        target_sequence: target.named_types,
        base_sequence: base.named_types,
        target_module_references: target.named_type_module_references,
        base_module_references: base.named_type_module_references,
        named_types,
        named_type_module_references,
    })
}

fn type_order_item_matches_filters(item: &OrderItem, needles: &[String]) -> bool {
    let identity = item.key.to_lowercase();
    needles.iter().any(|needle| identity.contains(needle))
}

fn build_focused_module_order_report(
    target_pdb: &PathBuf,
    base_pdb: &PathBuf,
    filter: &str,
) -> crate::Result<FocusedModuleOrderReport> {
    let target = load_focused_module_order(target_pdb, filter)?;
    let base = load_focused_module_order(base_pdb, filter)?;
    let modules = compare_sequence(
        "focused DBI module/object sequence",
        "semantic/linker-derived; filtered from serialized DBI module order",
        &base.modules,
        &target.modules,
    );
    Ok(FocusedModuleOrderReport {
        target_pdb: target_pdb.display().to_string(),
        base_pdb: base_pdb.display().to_string(),
        filter: filter.to_owned(),
        target_sequence: target.modules,
        base_sequence: base.modules,
        modules,
        dbi_source_file_streams: compare_scoped_streams(
            "DBI source-file references",
            "semantic/compiler-derived within a linker-created module",
            &base.dbi_source_files,
            &target.dbi_source_files,
        ),
        module_file_streams: compare_scoped_streams(
            "C13 file checksum records",
            "semantic/compiler-emitted module-local order",
            &base.module_files,
            &target.module_files,
        ),
        module_line_streams: compare_scoped_streams(
            "C13 line records",
            "semantic/compiler-emitted module-local order",
            &base.module_lines,
            &target.module_lines,
        ),
        module_subsection_streams: compare_scoped_streams(
            "C13 subsection records",
            "physical module-stream serialization",
            &base.module_subsections,
            &target.module_subsections,
        ),
    })
}

fn load_focused_module_order(
    pdb_path: &PathBuf,
    filter: &str,
) -> crate::Result<FocusedModuleOrderSide> {
    let layout = MsfLayout::parse(pdb_path)?;
    let mut dbi = load_raw_dbi_inventory(pdb_path, &layout)?;
    let needle = filter.to_lowercase();
    dbi.modules
        .retain(|module| module_order_item_matches_filter(&module.value, &needle));
    let selected_keys: BTreeSet<&str> = dbi
        .modules
        .iter()
        .map(|module| module.key.as_str())
        .collect();
    dbi.source_files
        .retain(|scope| selected_keys.contains(scope.key.as_str()));
    let global_names = load_global_names_by_offset(pdb_path, &layout)?;
    let debug = load_module_debug_scopes(
        pdb_path,
        &layout,
        &dbi,
        &global_names,
        &HashMap::new(),
        &HashMap::new(),
    )?;
    Ok(FocusedModuleOrderSide {
        modules: dbi
            .modules
            .iter()
            .map(|module| OrderItem {
                key: module.key.clone(),
                value: module.value.clone(),
                comparison_value: module.key.clone(),
            })
            .collect(),
        dbi_source_files: dbi.source_files,
        module_files: debug.files,
        module_lines: debug.lines,
        module_subsections: debug.subsections,
    })
}

fn module_order_item_matches_filter(value: &str, needle: &str) -> bool {
    value.to_lowercase().contains(needle)
}

fn load_global_names_by_offset(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
) -> crate::Result<HashMap<usize, String>> {
    let file = std::fs::File::open(pdb_path)?;
    let mut pdb = pdb::PDB::open(file)?;
    let info = pdb.pdb_information()?;
    let names = info.stream_names()?;
    let names_stream = names.iter().find_map(|named| {
        named
            .name
            .to_string()
            .eq_ignore_ascii_case("/names")
            .then_some(u32::from(named.stream_id.0))
    });
    let Some(names_stream) = names_stream else {
        return Ok(HashMap::new());
    };
    Ok(load_global_string_table(pdb_path, layout, names_stream)?.names_by_offset)
}

fn build_order_report(target_pdb: &PathBuf, base_pdb: &PathBuf) -> crate::Result<OrderReport> {
    let target = load_order_side(target_pdb)?;
    let base = load_order_side(base_pdb)?;
    let msf_layout = compare_msf_layouts(base_pdb, &base, target_pdb, &target)?;
    let msf_free_pages = compare_sequence(
        "MSF active free-page-map sequence",
        "exact physical allocation state",
        &base.msf_free_pages,
        &target.msf_free_pages,
    );
    let base_stream_roles = ordered_stream_roles(&base);
    let target_stream_roles = ordered_stream_roles(&target);
    let stream_roles = compare_sequence(
        "identified MSF stream slots",
        "physical/container-derived",
        &base_stream_roles,
        &target_stream_roles,
    );
    let dbi_substreams = compare_sequence(
        "DBI logical substream layout",
        "physical/linker-derived",
        &base.dbi_substreams,
        &target.dbi_substreams,
    );
    let dbi_source_file_streams = compare_scoped_streams(
        "DBI source-file reference sequence",
        "physical/linker-derived",
        &base.dbi_source_files,
        &target.dbi_source_files,
    );
    let dbi_ec_names = compare_sequence(
        "DBI edit-and-continue string-table sequence",
        "compiler/linker-emitted string serialization",
        &base.dbi_ec_names,
        &target.dbi_ec_names,
    );
    let dbi_ec_metadata = compare_sequence(
        "DBI edit-and-continue string-table metadata",
        "physical/toolchain-derived",
        &base.dbi_ec_metadata,
        &target.dbi_ec_metadata,
    );
    let dbi_ec_hash_buckets = compare_sequence(
        "DBI edit-and-continue hash-bucket sequence",
        "physical/hash-derived",
        &base.dbi_ec_hash_buckets,
        &target.dbi_ec_hash_buckets,
    );
    let named_streams = compare_sequence(
        "PDB named-stream map",
        "physical/container-derived",
        &base.named_streams,
        &target.named_streams,
    );
    let named_stream_buckets = compare_sequence(
        "PDB named-stream hash buckets",
        "physical/hash-derived",
        &base.named_stream_buckets,
        &target.named_stream_buckets,
    );
    let deleted_named_stream_buckets = compare_sequence(
        "PDB named-stream deleted buckets",
        "physical/incremental-state-derived",
        &base.deleted_named_stream_buckets,
        &target.deleted_named_stream_buckets,
    );
    let pdb_features = compare_sequence(
        "PDB feature-code sequence",
        "physical/toolchain-derived",
        &base.pdb_features,
        &target.pdb_features,
    );
    let string_table_names = compare_sequence(
        "global /names string-table sequence",
        "compiler/linker-emitted string serialization",
        &base.string_table_names,
        &target.string_table_names,
    );
    let string_table_metadata = compare_sequence(
        "global /names metadata",
        "physical/toolchain-derived",
        &base.string_table_metadata,
        &target.string_table_metadata,
    );
    let string_table_hash_buckets = compare_sequence(
        "global /names hash-bucket sequence",
        "physical/hash-derived",
        &base.string_table_hash_buckets,
        &target.string_table_hash_buckets,
    );
    let modules = compare_sequence(
        "DBI module stream",
        "physical/linker-derived",
        &base.modules,
        &target.modules,
    );
    let module_library_sequences = summarize_grouped_sequences(
        "DBI modules within one library",
        "physical/linker-derived",
        &base.modules,
        &target.modules,
        module_library_group,
        2,
    );
    let section_contributions = compare_sequence(
        "DBI section-contribution stream",
        "physical/linker-derived; same-module contributions are paired by ordinal",
        &base.section_contributions,
        &target.section_contributions,
    );
    let section_map = compare_sequence(
        "DBI section map",
        "physical/linker-derived",
        &base.section_map,
        &target.section_map,
    );
    let section_headers = compare_sequence(
        "PDB image section-header stream",
        "physical/linker-derived",
        &base.section_headers,
        &target.section_headers,
    );
    let legacy_fpo_records = compare_sequence(
        "legacy FPO record sequence",
        "linker/address-derived",
        &base.legacy_fpo_records,
        &target.legacy_fpo_records,
    );
    let frame_data_records = compare_sequence(
        "frame-data record sequence",
        "compiler/linker/address-derived",
        &base.frame_data_records,
        &target.frame_data_records,
    );
    let raw_type_records = compare_sequence(
        "all TPI record kinds (same-kind occurrences paired by ordinal)",
        "physical/type-index-derived; kind insertions can shift occurrence pairing",
        &base.raw_type_records,
        &target.raw_type_records,
    );
    let tpi_metadata = compare_sequence(
        "TPI stream/hash layout metadata",
        "physical/type-index-derived",
        &base.tpi_metadata,
        &target.tpi_metadata,
    );
    let tpi_hash_values = compare_sequence(
        "TPI per-record hash-value sequence",
        "compiler/linker/type-index-derived",
        &base.tpi_hash_values,
        &target.tpi_hash_values,
    );
    let tpi_index_offsets = compare_sequence(
        "TPI index-offset checkpoint sequence",
        "physical/type-index-derived; checkpoints are paired by ordinal",
        &base.tpi_index_offsets,
        &target.tpi_index_offsets,
    );
    let tpi_hash_adjustments = compare_sequence(
        "TPI hash-adjustment bucket sequence",
        "physical/incremental-state-derived",
        &base.tpi_hash_adjustments,
        &target.tpi_hash_adjustments,
    );
    let named_types = compare_sequence(
        "named complete TPI records",
        "physical/linker-deduplicated",
        &base.named_types,
        &target.named_types,
    );
    let named_type_module_references = compare_sequence(
        "named TPI direct module references",
        "compiler/linker-emitted; earliest retained module reference is a contributor clue, not proof of original ownership",
        &base.named_type_module_references,
        &target.named_type_module_references,
    );
    let named_type_kinds = summarize_grouped_sequences(
        "named complete TPI records by kind",
        "physical/linker-deduplicated",
        &base.named_types,
        &target.named_types,
        key_prefix_group,
        1,
    );
    let enum_value_streams = compare_scoped_streams(
        "enumerator declaration sequence",
        "compiler-emitted/type-semantic; duplicate same-name LF_ENUM records are left ambiguous",
        &base.enum_values,
        &target.enum_values,
    );
    let raw_id_records = compare_sequence(
        "all IPI record kinds (same-kind occurrences paired by ordinal)",
        "physical/id-index-derived; kind insertions can shift occurrence pairing",
        &base.raw_id_records,
        &target.raw_id_records,
    );
    let ipi_metadata = compare_sequence(
        "IPI stream/hash layout metadata",
        "physical/id-index-derived",
        &base.ipi_metadata,
        &target.ipi_metadata,
    );
    let ipi_hash_values = compare_sequence(
        "IPI per-record hash-value sequence",
        "compiler/linker/id-index-derived",
        &base.ipi_hash_values,
        &target.ipi_hash_values,
    );
    let ipi_index_offsets = compare_sequence(
        "IPI index-offset checkpoint sequence",
        "physical/id-index-derived; checkpoints are paired by ordinal",
        &base.ipi_index_offsets,
        &target.ipi_index_offsets,
    );
    let ipi_hash_adjustments = compare_sequence(
        "IPI hash-adjustment bucket sequence",
        "physical/incremental-state-derived",
        &base.ipi_hash_adjustments,
        &target.ipi_hash_adjustments,
    );
    let raw_global_symbols = compare_sequence(
        "all global symbol record kinds (same-kind occurrences paired by ordinal)",
        "physical/linker-derived; kind insertions can shift occurrence pairing",
        &base.raw_global_symbols,
        &target.raw_global_symbols,
    );
    let global_symbols = compare_sequence(
        "global symbol stream",
        "physical/linker-derived",
        &base.global_symbols,
        &target.global_symbols,
    );
    let global_hash_records = compare_sequence(
        "GSI serialized hash-record sequence",
        "linker/hash-derived; records resolve through the global symbol stream",
        &base.global_hash_records,
        &target.global_hash_records,
    );
    let global_hash_buckets = compare_sequence(
        "GSI populated-bucket sequence",
        "linker/hash-derived",
        &base.global_hash_buckets,
        &target.global_hash_buckets,
    );
    let public_hash_records = compare_sequence(
        "PSI serialized hash-record sequence",
        "linker/hash-derived; records resolve through the global symbol stream",
        &base.public_hash_records,
        &target.public_hash_records,
    );
    let public_hash_buckets = compare_sequence(
        "PSI populated-bucket sequence",
        "linker/hash-derived",
        &base.public_hash_buckets,
        &target.public_hash_buckets,
    );
    let public_address_map = compare_sequence(
        "PSI public address-map sequence",
        "linker/address-derived",
        &base.public_address_map,
        &target.public_address_map,
    );
    let public_thunk_map = compare_sequence(
        "PSI thunk-map sequence",
        "linker/address-derived; entries are paired by ordinal",
        &base.public_thunk_map,
        &target.public_thunk_map,
    );
    let public_section_map = compare_sequence(
        "PSI thunk section-map sequence",
        "linker/address-derived; entries are paired by ordinal",
        &base.public_section_map,
        &target.public_section_map,
    );
    let global_symbol_kinds = summarize_grouped_sequences(
        "global symbols by kind",
        "physical/linker-derived",
        &base.global_symbols,
        &target.global_symbols,
        key_prefix_group,
        1,
    );

    let (base_scopes, base_ambiguous) = unique_module_scopes(&base.module_symbols);
    let (target_scopes, target_ambiguous) = unique_module_scopes(&target.module_symbols);
    let mut ambiguous_keys = BTreeSet::new();
    ambiguous_keys.extend(base_ambiguous);
    ambiguous_keys.extend(target_ambiguous);
    let ambiguous_module_scopes = ambiguous_keys
        .into_iter()
        .map(|key| {
            let base_count = base
                .module_symbols
                .iter()
                .filter(|scope| scope.key == key)
                .count();
            let target_count = target
                .module_symbols
                .iter()
                .filter(|scope| scope.key == key)
                .count();
            let value = base
                .module_symbols
                .iter()
                .chain(&target.module_symbols)
                .find(|scope| scope.key == key)
                .map(|scope| scope.value.clone())
                .unwrap_or_else(|| key.clone());
            MultiplicityDifference {
                key,
                value,
                base_count,
                target_count,
            }
        })
        .collect();

    let mut paired_module_symbol_streams = 0usize;
    let mut module_symbols = Vec::new();
    let mut base_module_symbol_kinds: BTreeMap<String, Vec<OrderItem>> = BTreeMap::new();
    let mut target_module_symbol_kinds: BTreeMap<String, Vec<OrderItem>> = BTreeMap::new();
    for (key, target_scope) in &target_scopes {
        let Some(base_scope) = base_scopes.get(key) else {
            continue;
        };
        paired_module_symbol_streams += 1;
        append_scoped_symbol_kinds(&mut base_module_symbol_kinds, key, base_scope);
        append_scoped_symbol_kinds(&mut target_module_symbol_kinds, key, target_scope);
        let comparison = compare_sequence(
            "top-level module symbol stream",
            "physical/linker-derived",
            &base_scope.symbols,
            &target_scope.symbols,
        );
        if sequence_differs(&comparison) {
            module_symbols.push(ScopedSequenceComparison {
                scope: target_scope.value.clone(),
                comparison,
            });
        }
    }
    module_symbols.sort_by(|left, right| left.scope.cmp(&right.scope));
    let different_module_symbol_streams = module_symbols.len();
    let module_symbol_kinds = summarize_pre_grouped_sequences(
        "top-level module symbols by kind in stable module-key order",
        "compiler-emitted/module-local",
        base_module_symbol_kinds,
        target_module_symbol_kinds,
        1,
    );
    let module_record_streams = compare_scoped_streams(
        "all module symbol record kinds (same-kind occurrences paired by ordinal)",
        "compiler-emitted/module-local; kind insertions can shift occurrence pairing",
        &base.module_records,
        &target.module_records,
    );
    let module_file_streams = compare_scoped_streams(
        "module file-checksum sequence",
        "compiler-emitted/module-local",
        &base.module_files,
        &target.module_files,
    );
    let module_line_streams = compare_scoped_streams(
        "module line-program sequence",
        "compiler/linker/address-derived; line subsections are iterated by section and offset",
        &base.module_lines,
        &target.module_lines,
    );
    let module_subsection_streams = compare_scoped_streams(
        "module C13 subsection sequence",
        "compiler-emitted/module-local",
        &base.module_subsections,
        &target.module_subsections,
    );
    let module_string_streams = compare_scoped_streams(
        "module C13 string-table sequence",
        "compiler-emitted/module-local",
        &base.module_strings,
        &target.module_strings,
    );
    let module_frame_data_streams = compare_scoped_streams(
        "module C13 frame-data sequence",
        "compiler/linker/address-derived",
        &base.module_frame_data,
        &target.module_frame_data,
    );
    let module_inlinee_streams = compare_scoped_streams(
        "module C13 inlinee-source-line sequence",
        "compiler-emitted/id-index-derived",
        &base.module_inlinees,
        &target.module_inlinees,
    );
    let module_cross_import_streams = compare_scoped_streams(
        "module C13 cross-scope import sequence",
        "compiler-emitted/module-reference-derived",
        &base.module_cross_imports,
        &target.module_cross_imports,
    );
    let module_cross_export_streams = compare_scoped_streams(
        "module C13 cross-scope export sequence",
        "compiler/linker/type-index-derived",
        &base.module_cross_exports,
        &target.module_cross_exports,
    );
    let module_other_c13_streams = compare_scoped_streams(
        "module other C13 payload word sequence",
        "physical/ordinal; used only for subsection kinds without a stable decoder",
        &base.module_other_c13,
        &target.module_other_c13,
    );

    Ok(OrderReport {
        target_pdb: target_pdb.display().to_string(),
        base_pdb: base_pdb.display().to_string(),
        coverage: order_coverage(),
        msf_layout,
        msf_free_pages,
        stream_roles,
        dbi_substreams,
        dbi_source_file_streams,
        dbi_ec_names,
        dbi_ec_metadata,
        dbi_ec_hash_buckets,
        named_streams,
        named_stream_buckets,
        deleted_named_stream_buckets,
        pdb_features,
        string_table_names,
        string_table_metadata,
        string_table_hash_buckets,
        modules,
        module_library_sequences,
        section_contributions,
        section_map,
        section_headers,
        legacy_fpo_records,
        frame_data_records,
        raw_type_records,
        tpi_metadata,
        tpi_hash_values,
        tpi_index_offsets,
        tpi_hash_adjustments,
        named_types,
        named_type_module_references,
        named_type_kinds,
        enum_value_streams,
        raw_id_records,
        ipi_metadata,
        ipi_hash_values,
        ipi_index_offsets,
        ipi_hash_adjustments,
        raw_global_symbols,
        global_symbols,
        global_hash_records,
        global_hash_buckets,
        public_hash_records,
        public_hash_buckets,
        public_address_map,
        public_thunk_map,
        public_section_map,
        global_symbol_kinds,
        paired_module_symbol_streams,
        different_module_symbol_streams,
        ambiguous_module_scopes,
        module_symbols,
        module_symbol_kinds,
        module_record_streams,
        module_file_streams,
        module_line_streams,
        module_subsection_streams,
        module_string_streams,
        module_frame_data_streams,
        module_inlinee_streams,
        module_cross_import_streams,
        module_cross_export_streams,
        module_other_c13_streams,
    })
}

fn order_coverage() -> Vec<OrderCoverage> {
    vec![
        OrderCoverage {
            channel: "MSF superblock, directory, stream slots, page lists and runs",
            status: "exact physical",
            note: "page allocation is diagnostic and can change without a source-order change",
        },
        OrderCoverage {
            channel: "MSF free-page-map bitmap",
            status: "exact physical allocation sequence",
            note: "every active FPM block and every free page bit within the declared file are compared",
        },
        OrderCoverage {
            channel: "PDB information and named-stream map",
            status: "semantic and physical hash sequence",
            note: "live entries, bucket positions, deleted slots, stream indices and trailing feature-code order are compared",
        },
        OrderCoverage {
            channel: "global /names string table",
            status: "semantic and physical hash sequence",
            note: "serialized strings, hash metadata, every hash bucket, and epilogue name count are compared",
        },
        OrderCoverage {
            channel: "TPI/IPI records and auxiliary hash streams",
            status: "records and hash buffers semantic/ordinal",
            note: "record hashes, index checkpoints and adjustment buckets are decoded; unnamed record identity remains insertion-sensitive",
        },
        OrderCoverage {
            channel: "named TPI direct module references",
            status: "semantic reference sets in DBI module order",
            note: "direct local/data/UDT symbol type indexes bind named records to retained compilands; earliest reference is a first-contributor clue, not a canonical owner claim",
        },
        OrderCoverage {
            channel: "complete enum field lists",
            status: "semantic sequence",
            note: "unique same-name records are compared by value; duplicate same-name records are reported as ambiguous",
        },
        OrderCoverage {
            channel: "DBI header and logical substreams",
            status: "physical layout",
            note: "substream offsets/sizes and referenced stream roles are compared",
        },
        OrderCoverage {
            channel: "DBI modules, contributions, section map and image sections",
            status: "semantic/ordinal sequence",
            note: "same-module contributions are paired by occurrence because the format has no stronger stable key",
        },
        OrderCoverage {
            channel: "DBI source-info, type-server-map and EC payloads",
            status: "source-info and EC semantic/hash; type-server absent",
            note: "per-module source-file references and the EC string/hash table are ordered; the type-server map is empty in both PDBs",
        },
        OrderCoverage {
            channel: "global symbol record stream",
            status: "raw ordinal and semantic sequence",
            note: "all raw records plus recognized symbol identities are compared",
        },
        OrderCoverage {
            channel: "GSI/PSI hash tables and public address map",
            status: "semantic/physical sequence",
            note: "hash records, populated buckets, and public address order resolve through stable symbol identities when available",
        },
        OrderCoverage {
            channel: "module symbol records",
            status: "raw ordinal and semantic sequence",
            note: "all serialized records plus recognized top-level symbols are compared per stable module scope",
        },
        OrderCoverage {
            channel: "module C13 subsections and inner records",
            status: "raw/semantic sequence",
            note: "subsections, local strings, checksums, lines, frame data, inlinees, cross-scope maps and raw words for unknown payload kinds are compared in serialized order",
        },
        OrderCoverage {
            channel: "optional DBI debug streams (FPO, OMAP, fixup, frame data, xdata/pdata)",
            status: "present payloads decoded; absent kinds inventoried",
            note: "this pair contains legacy FPO, frame-data and section-header streams, all decoded in serialized order; other optional kinds are absent",
        },
        OrderCoverage {
            channel: "unidentified MSF streams",
            status: "physical inventory only",
            note: "every present slot is listed even when no stable semantic role can be assigned",
        },
    ]
}

fn load_order_side(pdb_path: &PathBuf) -> crate::Result<OrderSide> {
    let mut side = OrderSide::default();
    let layout = MsfLayout::parse(pdb_path)?;
    side.msf_free_pages = layout
        .free_pages
        .iter()
        .map(|page| OrderItem {
            key: format!("page={page}"),
            value: format!("free page={page}"),
            comparison_value: "free".to_owned(),
        })
        .collect();
    let pdb_info = load_raw_pdb_info(pdb_path, &layout)?;
    side.named_stream_buckets = pdb_info.named_buckets;
    side.deleted_named_stream_buckets = pdb_info.deleted_buckets;
    side.pdb_features = pdb_info.features;
    for (index, key, value) in [
        (0, "fixed|old-directory", "fixed stream 0 (old directory)"),
        (1, "fixed|pdb-info", "fixed stream 1 (PDB information)"),
        (2, "fixed|tpi", "fixed stream 2 (TPI)"),
        (3, "fixed|dbi", "fixed stream 3 (DBI)"),
        (4, "fixed|ipi", "fixed stream 4 (IPI)"),
    ] {
        if layout.stream(index).is_some() {
            side.stream_roles.push(StreamRoleBinding {
                key: key.to_owned(),
                value: value.to_owned(),
                stream_index: index,
            });
        }
    }
    append_type_aux_stream_roles(pdb_path, &layout, 2, "tpi", &mut side.stream_roles)?;
    append_type_aux_stream_roles(pdb_path, &layout, 4, "ipi", &mut side.stream_roles)?;
    let raw_dbi = load_raw_dbi_inventory(pdb_path, &layout)?;
    side.dbi_substreams = raw_dbi.substreams.clone();
    side.dbi_source_files = raw_dbi.source_files.clone();
    side.dbi_ec_names = raw_dbi.ec_names.clone();
    side.dbi_ec_metadata = raw_dbi.ec_metadata.clone();
    side.dbi_ec_hash_buckets = raw_dbi.ec_hash_buckets.clone();
    append_raw_dbi_stream_roles(&mut side.stream_roles, &raw_dbi);
    side.layout = Some(layout);
    let mut global_symbol_by_offset = HashMap::new();
    let mut type_by_index = HashMap::new();
    let mut named_type_items = Vec::new();
    let mut id_by_index = HashMap::new();
    let mut procedure_identities: HashMap<u32, BTreeSet<String>> = HashMap::new();
    let mut module_type_references: HashMap<u32, BTreeSet<(usize, String)>> = HashMap::new();

    PdbParser::with(pdb_path, |fmt| {
        let file = std::fs::File::open(pdb_path)?;
        let mut pdb = pdb::PDB::open(file)?;
        let address_map = pdb.address_map()?;

        {
            let info = pdb.pdb_information()?;
            let names = info.stream_names()?;
            for named in names.iter() {
                let name = named.name.to_string().into_owned();
                let normalized = name.to_lowercase();
                side.named_streams.push(OrderItem {
                    key: normalized.clone(),
                    value: format!("stream={} name={name}", named.stream_id.0),
                    comparison_value: normalized.clone(),
                });
                side.stream_roles.push(StreamRoleBinding {
                    key: format!("named|{normalized}"),
                    value: format!("named stream {name}"),
                    stream_index: u32::from(named.stream_id.0),
                });
            }
        }

        {
            let types = pdb.type_information()?;
            let mut finder = types.finder();
            let mut finder_iter = types.iter();
            while finder_iter.next()?.is_some() {
                finder.update(&finder_iter);
            }
            let mut iter = types.iter();
            let mut occurrences = HashMap::new();
            while let Some(record) = iter.next()? {
                let raw_item = raw_record_order_item(
                    "type",
                    record.raw_kind(),
                    record.index().0,
                    record.len(),
                    &mut occurrences,
                );
                side.raw_type_records.push(raw_item.clone());
                let Ok(data) = record.parse() else {
                    type_by_index.insert(record.index().0, raw_item);
                    continue;
                };
                if let pdb::TypeData::Enumeration(enumeration) = &data {
                    if !enumeration.properties.forward_reference() {
                        side.enum_values.push(enum_order_scope(
                            &fmt,
                            &finder,
                            record.index().0,
                            enumeration,
                        )?);
                    }
                }
                if let Some(item) = named_type_order_item(&fmt, record.index().0, data) {
                    side.named_types.push(item.clone());
                    named_type_items.push((record.index().0, item.clone()));
                    type_by_index.insert(record.index().0, item);
                } else {
                    type_by_index.insert(record.index().0, raw_item);
                }
            }
        }

        {
            let ids = pdb.id_information()?;
            let mut iter = ids.iter();
            let mut occurrences = HashMap::new();
            while let Some(record) = iter.next()? {
                let raw_item = raw_record_order_item(
                    "id",
                    record.raw_kind(),
                    record.index().0,
                    record.len(),
                    &mut occurrences,
                );
                side.raw_id_records.push(raw_item.clone());
                id_by_index.insert(record.index().0, raw_item);
            }
        }

        {
            let dbi = pdb.debug_information()?;
            let mut modules = dbi.modules()?;
            let mut module_id = 0usize;
            let mut module_keys = Vec::new();
            while let Some(module) = modules.next()? {
                let module_name = module.module_name().into_owned();
                let object_file_name = module.object_file_name().into_owned();
                let key = module_order_key(&module_name, &object_file_name);
                let value = format!("module={module_name} object={object_file_name}");
                module_keys.push(key.clone());
                side.modules.push(OrderItem {
                    key: key.clone(),
                    value: value.clone(),
                    comparison_value: key.clone(),
                });

                let mut ordered_symbols = Vec::new();
                let mut ordered_records = Vec::new();
                if let Some(info) = pdb.module_info(&module)? {
                    let mut symbols = info.symbols()?;
                    let mut depth = 0usize;
                    let mut record_occurrences = HashMap::new();
                    while let Some(symbol) = symbols.next()? {
                        ordered_records.push(raw_record_order_item(
                            "symbol",
                            symbol.raw_kind(),
                            symbol.index().0,
                            symbol.raw_bytes().len(),
                            &mut record_occurrences,
                        ));
                        let closes = symbol.ends_scope();
                        let record_depth = if closes {
                            depth.saturating_sub(1)
                        } else {
                            depth
                        };
                        if let Ok(data) = symbol.parse() {
                            if let Some(type_index) = direct_symbol_type_index(&data) {
                                module_type_references
                                    .entry(type_index)
                                    .or_default()
                                    .insert((module_id, key.clone()));
                            }
                            if record_depth == 0 {
                                if let SymbolData::Procedure(value) = &data {
                                    if let Some(rva) = value.offset.to_rva(&address_map) {
                                        let name = function_name(
                                            &fmt,
                                            module_id,
                                            &value.name,
                                            value.type_index,
                                        );
                                        procedure_identities
                                            .entry(rva.0)
                                            .or_default()
                                            .insert(named_order_item("procedure", name).key);
                                    }
                                }
                                if let Some(item) = module_symbol_order_item(&fmt, module_id, data)
                                {
                                    ordered_symbols.push(item);
                                }
                            }
                        }
                        if symbol.starts_scope() {
                            depth += 1;
                        }
                        if closes {
                            depth = depth.saturating_sub(1);
                        }
                    }
                }
                side.module_symbols.push(ModuleOrderScope {
                    key: key.clone(),
                    value: value.clone(),
                    symbols: ordered_symbols,
                });
                side.module_records.push(ModuleOrderScope {
                    key: key.clone(),
                    value: value.clone(),
                    symbols: ordered_records,
                });
                module_id += 1;
            }

            let mut contribution_occurrences: HashMap<String, usize> = HashMap::new();
            let mut contributions = dbi.section_contributions()?;
            while let Some(contribution) = contributions.next()? {
                let module_key = contribution
                    .module
                    .checked_sub(1)
                    .and_then(|index| module_keys.get(index))
                    .cloned()
                    .unwrap_or_else(|| format!("<module-{}>", contribution.module));
                let occurrence = contribution_occurrences
                    .entry(module_key.clone())
                    .or_default();
                let key = format!("{module_key}|contribution#{occurrence}");
                *occurrence += 1;
                let detail = format!(
                    "section={} offset=0x{:x} size=0x{:x} characteristics={:?} data-crc=0x{:x} reloc-crc=0x{:x}",
                    contribution.offset.section,
                    contribution.offset.offset,
                    contribution.size,
                    contribution.characteristics,
                    contribution.data_crc,
                    contribution.reloc_crc,
                );
                side.section_contributions.push(OrderItem {
                    key,
                    value: format!("module={module_key} {detail}"),
                    comparison_value: format!(
                        "section={} size=0x{:x} characteristics={:?}",
                        contribution.offset.section,
                        contribution.size,
                        contribution.characteristics,
                    ),
                });
            }

            let mut section_occurrences = HashMap::new();
            let mut sections = dbi.section_map()?;
            while let Some(section) = sections.next()? {
                let occurrence = section_occurrences
                    .entry(section.section_number)
                    .or_insert(0usize);
                let key = format!("section={}|occurrence={occurrence}", section.section_number);
                *occurrence += 1;
                let detail = format!(
                    "section={} flags=0x{:02x} type=0x{:02x} overlay={} group={} seg-name={} class-name={} rva=0x{:x} length=0x{:x}",
                    section.section_number,
                    section.flags,
                    section.section_type,
                    section.overlay,
                    section.group,
                    section.seg_name_index,
                    section.class_name_index,
                    section.rva_offset,
                    section.section_length,
                );
                side.section_map.push(OrderItem {
                    key,
                    value: detail.clone(),
                    comparison_value: detail,
                });
            }
        }

        {
            if let Some(sections) = pdb.sections()? {
                let mut occurrences: HashMap<String, usize> = HashMap::new();
                for (position, section) in sections.into_iter().enumerate() {
                    let name_end = section
                        .name
                        .iter()
                        .position(|byte| *byte == 0)
                        .unwrap_or(section.name.len());
                    let name = String::from_utf8_lossy(&section.name[..name_end]).into_owned();
                    let occurrence = occurrences.entry(name.clone()).or_default();
                    let key = format!("{name}|occurrence={occurrence}");
                    *occurrence += 1;
                    let detail = format!(
                        "section#{position} name={name} virtual-size=0x{:x} rva=0x{:x} raw-size=0x{:x} raw-offset=0x{:x} reloc-offset=0x{:x} line-offset=0x{:x} relocations={} lines={} characteristics={:?}",
                        section.virtual_size,
                        section.virtual_address,
                        section.size_of_raw_data,
                        section.pointer_to_raw_data,
                        section.pointer_to_relocations,
                        section.pointer_to_line_numbers,
                        section.number_of_relocations,
                        section.number_of_line_numbers,
                        section.characteristics,
                    );
                    side.section_headers.push(OrderItem {
                        key,
                        value: detail.clone(),
                        comparison_value: detail,
                    });
                }
            }
        }

        {
            let globals = pdb.global_symbols()?;
            let mut symbols = globals.iter();
            let mut occurrences = HashMap::new();
            while let Some(symbol) = symbols.next()? {
                let raw_item = raw_record_order_item(
                    "symbol",
                    symbol.raw_kind(),
                    symbol.index().0,
                    symbol.raw_bytes().len(),
                    &mut occurrences,
                );
                side.raw_global_symbols.push(raw_item.clone());
                let semantic_item = symbol
                    .parse()
                    .ok()
                    .and_then(global_symbol_order_item)
                    .unwrap_or(raw_item);
                global_symbol_by_offset.insert(symbol.index().0, semantic_item.clone());
                if !semantic_item.key.starts_with("kind=0x") {
                    side.global_symbols.push(semantic_item);
                }
            }
        }
        Ok(())
    })?;
    let layout = side
        .layout
        .as_ref()
        .ok_or_else(|| crate::Error::new("missing MSF layout".into()))?;
    let tpi = load_type_hash_inventory(pdb_path, layout, 2, "tpi", &type_by_index)?;
    side.tpi_metadata = tpi.metadata;
    side.tpi_hash_values = tpi.hash_values;
    side.tpi_index_offsets = tpi.index_offsets;
    side.tpi_hash_adjustments = tpi.hash_adjustments;
    let ipi = load_type_hash_inventory(pdb_path, layout, 4, "ipi", &id_by_index)?;
    side.ipi_metadata = ipi.metadata;
    side.ipi_hash_values = ipi.hash_values;
    side.ipi_index_offsets = ipi.index_offsets;
    side.ipi_hash_adjustments = ipi.hash_adjustments;
    let gsi = load_gsi_psi_inventory(pdb_path, layout, &raw_dbi, &global_symbol_by_offset)?;
    side.global_hash_records = gsi.global_records;
    side.global_hash_buckets = gsi.global_buckets;
    side.public_hash_records = gsi.public_records;
    side.public_hash_buckets = gsi.public_buckets;
    side.public_address_map = gsi.public_address_map;
    side.public_thunk_map = gsi.public_thunk_map;
    side.public_section_map = gsi.public_section_map;
    let mut names_by_offset = HashMap::new();
    if let Some(names_stream) = side
        .stream_roles
        .iter()
        .find(|binding| binding.key == "named|/names")
        .map(|binding| binding.stream_index)
    {
        let layout = side
            .layout
            .as_ref()
            .ok_or_else(|| crate::Error::new("missing MSF layout".into()))?;
        let names = load_global_string_table(pdb_path, layout, names_stream)?;
        side.string_table_names = names.names;
        side.string_table_metadata = names.metadata;
        side.string_table_hash_buckets = names.hash_buckets;
        names_by_offset = names.names_by_offset;
    }
    let procedure_by_rva = procedure_identities
        .into_iter()
        .filter_map(|(rva, identities)| {
            (identities.len() == 1).then(|| (rva, identities.into_iter().next().unwrap()))
        })
        .collect();
    side.named_type_module_references =
        build_named_type_module_references(&named_type_items, &module_type_references);
    side.legacy_fpo_records =
        load_legacy_fpo_records(pdb_path, layout, &raw_dbi.debug_streams, &procedure_by_rva)?;
    side.frame_data_records = load_frame_data_records(
        pdb_path,
        layout,
        &raw_dbi.debug_streams,
        &names_by_offset,
        &procedure_by_rva,
    )?;
    let raw_module_debug = load_module_debug_scopes(
        pdb_path,
        layout,
        &raw_dbi,
        &names_by_offset,
        &type_by_index,
        &id_by_index,
    )?;
    side.module_subsections = raw_module_debug.subsections;
    side.module_files = raw_module_debug.files;
    side.module_lines = raw_module_debug.lines;
    side.module_strings = raw_module_debug.strings;
    side.module_frame_data = raw_module_debug.frame_data;
    side.module_inlinees = raw_module_debug.inlinees;
    side.module_cross_imports = raw_module_debug.cross_imports;
    side.module_cross_exports = raw_module_debug.cross_exports;
    side.module_other_c13 = raw_module_debug.other;
    side.stream_roles.sort_by(|left, right| {
        left.stream_index
            .cmp(&right.stream_index)
            .then(left.key.cmp(&right.key))
    });
    Ok(side)
}

fn raw_record_order_item(
    domain: &str,
    raw_kind: u16,
    raw_index: u32,
    raw_len: usize,
    occurrences: &mut HashMap<u16, usize>,
) -> OrderItem {
    let occurrence = occurrences.entry(raw_kind).or_default();
    let key = format!("kind=0x{raw_kind:04x}|occurrence={occurrence}");
    *occurrence += 1;
    OrderItem {
        key,
        value: format!("{domain}-index=0x{raw_index:x} kind=0x{raw_kind:04x} bytes=0x{raw_len:x}"),
        comparison_value: format!("kind=0x{raw_kind:04x}|bytes=0x{raw_len:x}"),
    }
}

fn normalize_pdb_path(value: &str) -> String {
    value.replace('\\', "/").to_lowercase()
}

#[derive(Default)]
struct RawGlobalStringTable {
    names: Vec<OrderItem>,
    metadata: Vec<OrderItem>,
    hash_buckets: Vec<OrderItem>,
    names_by_offset: HashMap<usize, String>,
}

fn load_global_string_table(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
    stream_index: u32,
) -> crate::Result<RawGlobalStringTable> {
    let Some(bytes) = layout.read_stream(pdb_path, stream_index)? else {
        return Ok(RawGlobalStringTable::default());
    };
    parse_global_string_table(&bytes, &format!("named /names stream {stream_index}"))
}

fn parse_global_string_table(bytes: &[u8], label: &str) -> crate::Result<RawGlobalStringTable> {
    if bytes.is_empty() {
        return Ok(RawGlobalStringTable::default());
    }
    if raw_u32(&bytes, 0)? != 0xeffe_effe {
        return crate::error!("{label} has an invalid signature");
    }
    let hash_version = raw_u32(&bytes, 4)?;
    let names_size = raw_u32(&bytes, 8)? as usize;
    let start = 12usize;
    let end = start
        .checked_add(names_size)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new("global string table is out of range".into()))?;
    let mut cursor = start;
    let mut occurrences: HashMap<String, usize> = HashMap::new();
    let mut result = RawGlobalStringTable::default();
    let mut name_by_offset = HashMap::new();
    while cursor < end {
        let (name, next) = raw_cstring(&bytes, cursor, end)?;
        let normalized = normalize_pdb_path(&name);
        let occurrence = occurrences.entry(normalized.clone()).or_default();
        let key = format!("{normalized}|occurrence={occurrence}");
        *occurrence += 1;
        result.names.push(OrderItem {
            key,
            value: format!("offset=0x{:x} string={name:?}", cursor - start),
            comparison_value: normalized.clone(),
        });
        name_by_offset.insert(cursor - start, normalized);
        cursor = next;
    }
    if end + 8 > bytes.len() {
        return crate::error!("global string-table hash tail is truncated");
    }
    let hash_count = raw_u32(&bytes, end)? as usize;
    let hash_start = end + 4;
    let hash_end = hash_start
        .checked_add(hash_count.saturating_mul(4))
        .filter(|end| end.saturating_add(4) == bytes.len())
        .ok_or_else(|| crate::Error::new("global string-table hash tail is out of range".into()))?;
    let name_count = raw_u32(&bytes, hash_end)?;
    for (key, value) in [
        ("signature", "0xeffeeffe".to_owned()),
        ("hash-version", hash_version.to_string()),
        ("string-bytes", names_size.to_string()),
        ("hash-buckets", hash_count.to_string()),
        ("name-count", name_count.to_string()),
    ] {
        let detail = format!("{key}={value}");
        result.metadata.push(OrderItem {
            key: key.to_owned(),
            value: detail.clone(),
            comparison_value: detail,
        });
    }
    for position in 0..hash_count {
        let id = raw_u32(&bytes, hash_start + position * 4)? as usize;
        let name = if id == 0 {
            "<empty>".to_owned()
        } else {
            name_by_offset
                .get(&id)
                .cloned()
                .unwrap_or_else(|| format!("<unresolved-id-0x{id:x}>"))
        };
        let detail = format!("bucket={position} id=0x{id:x} string={name}");
        result.hash_buckets.push(OrderItem {
            key: format!("bucket={position}"),
            value: detail.clone(),
            comparison_value: detail,
        });
    }
    result.names_by_offset = name_by_offset;
    Ok(result)
}

#[derive(Default)]
struct RawPdbInfoInventory {
    named_buckets: Vec<OrderItem>,
    deleted_buckets: Vec<OrderItem>,
    features: Vec<OrderItem>,
}

fn load_raw_pdb_info(pdb_path: &PathBuf, layout: &MsfLayout) -> crate::Result<RawPdbInfoInventory> {
    let Some(bytes) = layout.read_stream(pdb_path, 1)? else {
        return crate::error!("PDB has no information stream 1");
    };
    if bytes.len() < 32 {
        return crate::error!("PDB information stream is only {} bytes", bytes.len());
    }
    let string_bytes = raw_u32(&bytes, 28)? as usize;
    let strings_start = 32usize;
    let strings_end = strings_start
        .checked_add(string_bytes)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| {
            crate::Error::new("PDB named-stream string buffer is out of range".into())
        })?;
    let mut cursor = strings_end;
    let size = raw_u32(&bytes, cursor)? as usize;
    let capacity = raw_u32(&bytes, cursor + 4)? as usize;
    cursor += 8;
    if capacity == 0 || size > capacity.saturating_mul(2) / 3 + 1 {
        return crate::error!("invalid PDB named-stream hash size={size} capacity={capacity}");
    }
    let (present, next) = load_sparse_bit_vector(&bytes, cursor, capacity)?;
    cursor = next;
    let (deleted, next) = load_sparse_bit_vector(&bytes, cursor, capacity)?;
    cursor = next;
    if present.len() != size {
        return crate::error!(
            "PDB named-stream hash declares {size} entries but marks {} present",
            present.len()
        );
    }
    if present.iter().any(|bucket| deleted.contains(bucket)) {
        return crate::error!("PDB named-stream present/deleted buckets overlap");
    }
    let entry_end = cursor
        .checked_add(size.saturating_mul(8))
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new("PDB named-stream entries are out of range".into()))?;
    let mut inventory = RawPdbInfoInventory::default();
    for (entry, bucket) in present.into_iter().enumerate() {
        let name_offset = raw_u32(&bytes, cursor + entry * 8)? as usize;
        let stream_index = raw_u32(&bytes, cursor + entry * 8 + 4)?;
        let name =
            raw_string_at(&bytes[strings_start..strings_end], name_offset).ok_or_else(|| {
                crate::Error::new(format!(
                    "PDB named-stream string offset 0x{name_offset:x} is out of range"
                ))
            })?;
        let normalized = name.to_lowercase();
        let detail = format!(
            "bucket={bucket} name-offset=0x{name_offset:x} stream={stream_index} name={name}"
        );
        inventory.named_buckets.push(OrderItem {
            key: normalized,
            value: detail.clone(),
            comparison_value: detail,
        });
    }
    for bucket in deleted {
        inventory.deleted_buckets.push(OrderItem {
            key: format!("bucket={bucket}"),
            value: format!("deleted bucket={bucket}"),
            comparison_value: format!("deleted bucket={bucket}"),
        });
    }
    cursor = entry_end;
    if (bytes.len() - cursor) % 4 != 0 {
        return crate::error!("PDB feature-code tail is not u32-aligned");
    }
    let mut occurrences = HashMap::new();
    for position in 0..(bytes.len() - cursor) / 4 {
        let feature = raw_u32(&bytes, cursor + position * 4)?;
        let occurrence = occurrences.entry(feature).or_insert(0usize);
        let key = format!("feature=0x{feature:08x}|occurrence={occurrence}");
        *occurrence += 1;
        let detail = format!("feature#{position}=0x{feature:08x}");
        inventory.features.push(OrderItem {
            key,
            value: detail,
            comparison_value: format!("feature=0x{feature:08x}"),
        });
    }
    Ok(inventory)
}

fn load_sparse_bit_vector(
    bytes: &[u8],
    start: usize,
    capacity: usize,
) -> crate::Result<(Vec<usize>, usize)> {
    let word_count = raw_u32(bytes, start)? as usize;
    let end = start
        .checked_add(4)
        .and_then(|value| value.checked_add(word_count.saturating_mul(4)))
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new("serialized hash bit vector is out of range".into()))?;
    let mut set = Vec::new();
    for word_index in 0..word_count {
        let word = raw_u32(bytes, start + 4 + word_index * 4)?;
        for bit in 0..32 {
            if word & (1 << bit) != 0 {
                let bucket = word_index * 32 + bit;
                if bucket >= capacity {
                    return crate::error!(
                        "serialized hash bucket {bucket} exceeds capacity {capacity}"
                    );
                }
                set.push(bucket);
            }
        }
    }
    Ok((set, end))
}

#[derive(Default)]
struct RawDbiInventory {
    global_hash_stream: Option<u32>,
    public_hash_stream: Option<u32>,
    symbol_records_stream: Option<u32>,
    modules: Vec<RawDbiModule>,
    debug_streams: Vec<(&'static str, u32)>,
    substreams: Vec<OrderItem>,
    source_files: Vec<ModuleOrderScope>,
    ec_names: Vec<OrderItem>,
    ec_metadata: Vec<OrderItem>,
    ec_hash_buckets: Vec<OrderItem>,
}

struct RawDbiModule {
    key: String,
    value: String,
    stream_index: Option<u32>,
    symbols_size: u32,
    lines_size: u32,
    c13_lines_size: u32,
}

fn load_raw_dbi_inventory(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
) -> crate::Result<RawDbiInventory> {
    let Some(bytes) = layout.read_stream(pdb_path, 3)? else {
        return crate::error!("PDB has no DBI stream 3");
    };
    if bytes.len() < 64 {
        return crate::error!("DBI stream is only {} bytes", bytes.len());
    }

    let mut inventory = RawDbiInventory {
        global_hash_stream: optional_stream(raw_u16(&bytes, 12)?),
        public_hash_stream: optional_stream(raw_u16(&bytes, 16)?),
        symbol_records_stream: optional_stream(raw_u16(&bytes, 20)?),
        ..Default::default()
    };
    let module_list_size = raw_u32(&bytes, 24)? as usize;
    let section_contribution_size = raw_u32(&bytes, 28)? as usize;
    let section_map_size = raw_u32(&bytes, 32)? as usize;
    let file_info_size = raw_u32(&bytes, 36)? as usize;
    let type_server_map_size = raw_u32(&bytes, 40)? as usize;
    let debug_header_size = raw_u32(&bytes, 48)? as usize;
    let ec_substream_size = raw_u32(&bytes, 52)? as usize;

    let logical_substreams = [
        ("header", 64usize),
        ("module-info", module_list_size),
        ("section-contributions", section_contribution_size),
        ("section-map", section_map_size),
        ("source-info", file_info_size),
        ("type-server-map", type_server_map_size),
        ("ec", ec_substream_size),
        ("optional-debug-header", debug_header_size),
    ];
    let mut logical_offset = 0usize;
    for (name, size) in logical_substreams {
        let end = logical_offset
            .checked_add(size)
            .ok_or_else(|| crate::Error::new("DBI logical substream size overflow".into()))?;
        if end > bytes.len() {
            return crate::error!("DBI logical substream {name} crosses stream boundary");
        }
        let detail = format!("{name} offset=0x{logical_offset:x} bytes=0x{size:x}");
        inventory.substreams.push(OrderItem {
            key: name.to_owned(),
            value: detail.clone(),
            comparison_value: detail,
        });
        logical_offset = end;
    }
    if logical_offset < bytes.len() {
        let size = bytes.len() - logical_offset;
        let detail = format!("trailing offset=0x{logical_offset:x} bytes=0x{size:x}");
        inventory.substreams.push(OrderItem {
            key: "trailing".to_owned(),
            value: detail.clone(),
            comparison_value: detail,
        });
    }

    let module_end = 64usize
        .checked_add(module_list_size)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new("DBI module list is out of range".into()))?;
    let mut cursor = 64usize;
    while cursor < module_end {
        let fixed_end = cursor
            .checked_add(64)
            .filter(|end| *end <= module_end)
            .ok_or_else(|| crate::Error::new("truncated DBI module record".into()))?;
        let stream_index = optional_stream(raw_u16(&bytes, cursor + 34)?);
        let symbols_size = raw_u32(&bytes, cursor + 36)?;
        let lines_size = raw_u32(&bytes, cursor + 40)?;
        let c13_lines_size = raw_u32(&bytes, cursor + 44)?;
        cursor = fixed_end;
        let (module_name, next) = raw_cstring(&bytes, cursor, module_end)?;
        cursor = next;
        let (object_file_name, next) = raw_cstring(&bytes, cursor, module_end)?;
        cursor = align4(next);
        if cursor > module_end {
            return crate::error!("DBI module strings cross the module-list boundary");
        }
        let key = module_order_key(&module_name, &object_file_name);
        let value = format!("module={module_name} object={object_file_name}");
        inventory.modules.push(RawDbiModule {
            key,
            value,
            stream_index,
            symbols_size,
            lines_size,
            c13_lines_size,
        });
    }

    let file_info_offset = 64usize
        .checked_add(module_list_size)
        .and_then(|value| value.checked_add(section_contribution_size))
        .and_then(|value| value.checked_add(section_map_size))
        .ok_or_else(|| crate::Error::new("DBI file-info offset overflow".into()))?;
    let file_info_end = file_info_offset
        .checked_add(file_info_size)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new("DBI file-info is out of range".into()))?;
    inventory.source_files =
        load_dbi_source_file_scopes(&bytes[file_info_offset..file_info_end], &inventory.modules)?;

    let ec_offset = file_info_end
        .checked_add(type_server_map_size)
        .ok_or_else(|| crate::Error::new("DBI EC offset overflow".into()))?;
    let ec_end = ec_offset
        .checked_add(ec_substream_size)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new("DBI EC substream is out of range".into()))?;
    let ec = parse_global_string_table(&bytes[ec_offset..ec_end], "DBI EC string table")?;
    inventory.ec_names = ec.names;
    inventory.ec_metadata = ec.metadata;
    inventory.ec_hash_buckets = ec.hash_buckets;

    let debug_offset = 64usize
        .checked_add(module_list_size)
        .and_then(|value| value.checked_add(section_contribution_size))
        .and_then(|value| value.checked_add(section_map_size))
        .and_then(|value| value.checked_add(file_info_size))
        .and_then(|value| value.checked_add(type_server_map_size))
        .and_then(|value| value.checked_add(ec_substream_size))
        .ok_or_else(|| crate::Error::new("DBI substream size overflow".into()))?;
    let debug_end = debug_offset
        .checked_add(debug_header_size)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new("DBI debug header is out of range".into()))?;
    let debug_names = [
        "fpo",
        "exception",
        "fixup",
        "omap-to-src",
        "omap-from-src",
        "section-headers",
        "token-rid-map",
        "xdata",
        "pdata",
        "frame-data",
        "original-section-headers",
    ];
    for (position, name) in debug_names.into_iter().enumerate() {
        let offset = debug_offset + position * 2;
        if offset + 2 > debug_end {
            break;
        }
        if let Some(stream) = optional_stream(raw_u16(&bytes, offset)?) {
            inventory.debug_streams.push((name, stream));
        }
    }
    Ok(inventory)
}

fn load_dbi_source_file_scopes(
    bytes: &[u8],
    modules: &[RawDbiModule],
) -> crate::Result<Vec<ModuleOrderScope>> {
    if bytes.is_empty() {
        return Ok(modules
            .iter()
            .map(|module| ModuleOrderScope {
                key: module.key.clone(),
                value: module.value.clone(),
                symbols: Vec::new(),
            })
            .collect());
    }
    if bytes.len() < 4 {
        return crate::error!("DBI source-info header is truncated");
    }
    let module_count = raw_u16(bytes, 0)? as usize;
    let _truncated_file_count = raw_u16(bytes, 2)? as usize;
    let indices_start = 4usize;
    let counts_start = indices_start
        .checked_add(module_count.saturating_mul(2))
        .filter(|offset| *offset <= bytes.len())
        .ok_or_else(|| {
            crate::Error::new("DBI source-info module indices are out of range".into())
        })?;
    let offsets_start = counts_start
        .checked_add(module_count.saturating_mul(2))
        .filter(|offset| *offset <= bytes.len())
        .ok_or_else(|| crate::Error::new("DBI source-info file counts are out of range".into()))?;
    let mut file_counts = Vec::with_capacity(module_count);
    for module_index in 0..module_count {
        file_counts.push(raw_u16(bytes, counts_start + module_index * 2)? as usize);
    }
    let referenced_file_count: usize = file_counts.iter().sum();
    let names_start = offsets_start
        .checked_add(referenced_file_count.saturating_mul(4))
        .filter(|offset| *offset <= bytes.len())
        .ok_or_else(|| crate::Error::new("DBI source-info file offsets are out of range".into()))?;
    let mut scopes = Vec::with_capacity(module_count.max(modules.len()));
    let mut reference_index = 0usize;
    for module_index in 0..module_count {
        let mut symbols = Vec::with_capacity(file_counts[module_index]);
        let mut occurrences: HashMap<String, usize> = HashMap::new();
        for position in 0..file_counts[module_index] {
            let name_offset = raw_u32(bytes, offsets_start + reference_index * 4)? as usize;
            reference_index += 1;
            let name = raw_string_at(&bytes[names_start..], name_offset).ok_or_else(|| {
                crate::Error::new(format!(
                    "DBI source-info name offset 0x{name_offset:x} is out of range"
                ))
            })?;
            let normalized = normalize_pdb_path(&name);
            let occurrence = occurrences.entry(normalized.clone()).or_default();
            let key = format!("{normalized}|occurrence={occurrence}");
            *occurrence += 1;
            symbols.push(OrderItem {
                key,
                value: format!("module-file#{position} name-offset=0x{name_offset:x} file={name}"),
                comparison_value: normalized,
            });
        }
        let (key, value) = modules.get(module_index).map_or_else(
            || {
                (
                    format!("<module-{module_index}>"),
                    format!("module #{module_index}"),
                )
            },
            |module| (module.key.clone(), module.value.clone()),
        );
        scopes.push(ModuleOrderScope {
            key,
            value,
            symbols,
        });
    }
    for module in modules.iter().skip(module_count) {
        scopes.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: Vec::new(),
        });
    }
    Ok(scopes)
}

fn append_raw_dbi_stream_roles(roles: &mut Vec<StreamRoleBinding>, dbi: &RawDbiInventory) {
    for (key, value, stream_index) in [
        (
            "dbi|global-symbol-hash",
            "DBI global-symbol hash stream",
            dbi.global_hash_stream,
        ),
        (
            "dbi|public-symbol-hash",
            "DBI public-symbol hash/address stream",
            dbi.public_hash_stream,
        ),
        (
            "dbi|symbol-records",
            "DBI global symbol record stream",
            dbi.symbol_records_stream,
        ),
    ] {
        if let Some(stream_index) = stream_index {
            roles.push(StreamRoleBinding {
                key: key.to_owned(),
                value: value.to_owned(),
                stream_index,
            });
        }
    }
    let mut module_occurrences: HashMap<&str, usize> = HashMap::new();
    for module in &dbi.modules {
        let Some(stream_index) = module.stream_index else {
            continue;
        };
        let occurrence = module_occurrences.entry(&module.key).or_default();
        let key = format!("module|{}|occurrence={occurrence}", module.key);
        *occurrence += 1;
        roles.push(StreamRoleBinding {
            key,
            value: format!("module stream {}", module.value),
            stream_index,
        });
    }
    for &(name, stream_index) in &dbi.debug_streams {
        roles.push(StreamRoleBinding {
            key: format!("dbi-debug|{name}"),
            value: format!("DBI optional debug stream {name}"),
            stream_index,
        });
    }
}

fn load_legacy_fpo_records(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
    debug_streams: &[(&'static str, u32)],
    procedure_by_rva: &HashMap<u32, String>,
) -> crate::Result<Vec<OrderItem>> {
    let Some(stream_index) = debug_streams
        .iter()
        .find_map(|(name, index)| (*name == "fpo").then_some(*index))
    else {
        return Ok(Vec::new());
    };
    let bytes = layout
        .read_stream(pdb_path, stream_index)?
        .ok_or_else(|| crate::Error::new("legacy FPO stream is absent".into()))?;
    parse_legacy_fpo_records(
        &bytes,
        procedure_by_rva,
        &format!("legacy FPO stream {stream_index}"),
    )
}

fn parse_legacy_fpo_records(
    bytes: &[u8],
    procedure_by_rva: &HashMap<u32, String>,
    label: &str,
) -> crate::Result<Vec<OrderItem>> {
    const RECORD_SIZE: usize = 16;
    if bytes.len() % RECORD_SIZE != 0 {
        return crate::error!("{label} is not {RECORD_SIZE}-byte record aligned");
    }
    let mut output = Vec::with_capacity(bytes.len() / RECORD_SIZE);
    let mut occurrences: HashMap<String, usize> = HashMap::new();
    for position in 0..bytes.len() / RECORD_SIZE {
        let cursor = position * RECORD_SIZE;
        let rva = raw_u32(&bytes, cursor)?;
        let code_size = raw_u32(&bytes, cursor + 4)?;
        let locals_words = raw_u32(&bytes, cursor + 8)?;
        let params_words = raw_u16(&bytes, cursor + 12)?;
        let attributes = raw_u16(&bytes, cursor + 14)?;
        let identity = procedure_by_rva
            .get(&rva)
            .cloned()
            .unwrap_or_else(|| format!("rva=0x{rva:x}"));
        let occurrence = occurrences.entry(identity.clone()).or_insert(0usize);
        let key = format!("{identity}|occurrence={occurrence}");
        *occurrence += 1;
        let detail = format!(
            "rva=0x{rva:x} size=0x{code_size:x} locals-words={locals_words} params-words={params_words} prolog={} saved-regs={} seh={} use-bp={} reserved={} frame-type={} attributes=0x{attributes:04x}",
            attributes & 0xff,
            (attributes >> 8) & 0x7,
            (attributes >> 11) & 1,
            (attributes >> 12) & 1,
            (attributes >> 13) & 1,
            attributes >> 14,
        );
        let comparison_value = if procedure_by_rva.contains_key(&rva) {
            detail.replacen(&format!("rva=0x{rva:x} "), "", 1)
        } else {
            detail.clone()
        };
        output.push(OrderItem {
            key,
            value: format!("record#{position} {detail}"),
            comparison_value,
        });
    }
    Ok(output)
}

fn load_frame_data_records(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
    debug_streams: &[(&'static str, u32)],
    names_by_offset: &HashMap<usize, String>,
    procedure_by_rva: &HashMap<u32, String>,
) -> crate::Result<Vec<OrderItem>> {
    let Some(stream_index) = debug_streams
        .iter()
        .find_map(|(name, index)| (*name == "frame-data").then_some(*index))
    else {
        return Ok(Vec::new());
    };
    let bytes = layout
        .read_stream(pdb_path, stream_index)?
        .ok_or_else(|| crate::Error::new("frame-data stream is absent".into()))?;
    parse_frame_data_records(
        &bytes,
        names_by_offset,
        procedure_by_rva,
        &format!("frame-data stream {stream_index}"),
    )
}

fn parse_frame_data_records(
    bytes: &[u8],
    names_by_offset: &HashMap<usize, String>,
    procedure_by_rva: &HashMap<u32, String>,
    label: &str,
) -> crate::Result<Vec<OrderItem>> {
    const RECORD_SIZE: usize = 32;
    if bytes.len() % RECORD_SIZE != 0 {
        return crate::error!("{label} is not {RECORD_SIZE}-byte record aligned");
    }
    let mut output = Vec::with_capacity(bytes.len() / RECORD_SIZE);
    let mut occurrences: HashMap<String, usize> = HashMap::new();
    for position in 0..bytes.len() / RECORD_SIZE {
        let cursor = position * RECORD_SIZE;
        let rva = raw_u32(&bytes, cursor)?;
        let code_size = raw_u32(&bytes, cursor + 4)?;
        let locals_size = raw_u32(&bytes, cursor + 8)?;
        let params_size = raw_u32(&bytes, cursor + 12)?;
        let max_stack_size = raw_u32(&bytes, cursor + 16)?;
        let frame_func_offset = raw_u32(&bytes, cursor + 20)? as usize;
        let prolog_size = raw_u16(&bytes, cursor + 24)?;
        let saved_regs_size = raw_u16(&bytes, cursor + 26)?;
        let flags = raw_u32(&bytes, cursor + 28)?;
        let frame_func = names_by_offset
            .get(&frame_func_offset)
            .cloned()
            .unwrap_or_else(|| format!("<unresolved-offset-0x{frame_func_offset:x}>"));
        let identity = procedure_by_rva
            .get(&rva)
            .cloned()
            .unwrap_or_else(|| format!("rva=0x{rva:x}"));
        let occurrence = occurrences.entry(identity.clone()).or_insert(0usize);
        let key = format!("{identity}|occurrence={occurrence}");
        *occurrence += 1;
        let detail = format!(
            "rva=0x{rva:x} size=0x{code_size:x} locals=0x{locals_size:x} params=0x{params_size:x} max-stack=0x{max_stack_size:x} frame-func={frame_func:?} prolog={prolog_size} saved-regs={saved_regs_size} seh={} eh={} function-start={} reserved=0x{:x}",
            flags & 1,
            (flags >> 1) & 1,
            (flags >> 2) & 1,
            flags >> 3,
        );
        let comparison_value = if procedure_by_rva.contains_key(&rva) {
            detail.replacen(&format!("rva=0x{rva:x} "), "", 1)
        } else {
            detail.clone()
        };
        output.push(OrderItem {
            key,
            value: format!("record#{position} frame-func-offset=0x{frame_func_offset:x} {detail}"),
            comparison_value,
        });
    }
    Ok(output)
}

fn append_type_aux_stream_roles(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
    stream_index: u32,
    domain: &'static str,
    roles: &mut Vec<StreamRoleBinding>,
) -> crate::Result<()> {
    let Some(bytes) = layout.read_stream(pdb_path, stream_index)? else {
        return Ok(());
    };
    if bytes.is_empty() {
        return Ok(());
    }
    if bytes.len() < 56 {
        return crate::error!(
            "{domain} stream {stream_index} is only {} bytes",
            bytes.len()
        );
    }
    for (suffix, label, index) in [
        (
            "hash",
            "hash values/index offsets",
            optional_stream(raw_u16(&bytes, 20)?),
        ),
        (
            "hash-aux",
            "hash auxiliary/pad",
            optional_stream(raw_u16(&bytes, 22)?),
        ),
    ] {
        if let Some(stream_index) = index {
            roles.push(StreamRoleBinding {
                key: format!("{domain}|{suffix}"),
                value: format!("{} {label} stream", domain.to_uppercase()),
                stream_index,
            });
        }
    }
    Ok(())
}

#[derive(Default)]
struct TypeHashInventory {
    metadata: Vec<OrderItem>,
    hash_values: Vec<OrderItem>,
    index_offsets: Vec<OrderItem>,
    hash_adjustments: Vec<OrderItem>,
}

fn load_type_hash_inventory(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
    stream_index: u32,
    domain: &str,
    records: &HashMap<u32, OrderItem>,
) -> crate::Result<TypeHashInventory> {
    let Some(main_bytes) = layout.read_stream(pdb_path, stream_index)? else {
        return Ok(TypeHashInventory::default());
    };
    if main_bytes.is_empty() {
        return Ok(TypeHashInventory::default());
    }
    if main_bytes.len() < 56 {
        return crate::error!(
            "{domain} stream {stream_index} is only {} bytes",
            main_bytes.len()
        );
    }

    let version = raw_u32(&main_bytes, 0)?;
    let header_size = raw_u32(&main_bytes, 4)?;
    let index_begin = raw_u32(&main_bytes, 8)?;
    let index_end = raw_u32(&main_bytes, 12)?;
    let record_bytes = raw_u32(&main_bytes, 16)?;
    let hash_stream = raw_u16(&main_bytes, 20)?;
    let hash_aux_stream = raw_u16(&main_bytes, 22)?;
    let hash_key_size = raw_u32(&main_bytes, 24)?;
    let hash_buckets = raw_u32(&main_bytes, 28)?;
    let hash_values_offset = raw_i32(&main_bytes, 32)?;
    let hash_values_size = raw_u32(&main_bytes, 36)?;
    let index_offsets_offset = raw_i32(&main_bytes, 40)?;
    let index_offsets_size = raw_u32(&main_bytes, 44)?;
    let hash_adjustments_offset = raw_i32(&main_bytes, 48)?;
    let hash_adjustments_size = raw_u32(&main_bytes, 52)?;
    if header_size < 56 || header_size as usize > main_bytes.len() {
        return crate::error!(
            "{domain} header size 0x{header_size:x} is outside stream size 0x{:x}",
            main_bytes.len()
        );
    }
    if index_end < index_begin {
        return crate::error!(
            "{domain} index range 0x{index_begin:x}..0x{index_end:x} is reversed"
        );
    }
    let declared_end = (header_size as usize)
        .checked_add(record_bytes as usize)
        .ok_or_else(|| crate::Error::new(format!("{domain} record size overflow")))?;
    if declared_end != main_bytes.len() {
        return crate::error!(
            "{domain} header and record bytes end at 0x{declared_end:x}, stream has 0x{:x} bytes",
            main_bytes.len()
        );
    }

    let mut result = TypeHashInventory::default();
    for (key, value) in [
        ("version", format!("{version}")),
        ("header-bytes", format!("0x{header_size:x}")),
        ("index-begin", format!("0x{index_begin:x}")),
        ("index-end", format!("0x{index_end:x}")),
        ("record-bytes", format!("0x{record_bytes:x}")),
        ("hash-stream", format!("{hash_stream}")),
        ("hash-aux-stream", format!("{hash_aux_stream}")),
        ("hash-key-bytes", format!("{hash_key_size}")),
        ("hash-buckets", format!("{hash_buckets}")),
        (
            "hash-values-region",
            format!("offset={hash_values_offset} bytes=0x{hash_values_size:x}"),
        ),
        (
            "index-offsets-region",
            format!("offset={index_offsets_offset} bytes=0x{index_offsets_size:x}"),
        ),
        (
            "hash-adjustments-region",
            format!("offset={hash_adjustments_offset} bytes=0x{hash_adjustments_size:x}"),
        ),
    ] {
        let detail = format!("{key}={value}");
        result.metadata.push(OrderItem {
            key: key.to_owned(),
            value: detail.clone(),
            comparison_value: detail,
        });
    }

    let Some(hash_stream_index) = optional_stream(hash_stream) else {
        if hash_values_size != 0 || index_offsets_size != 0 || hash_adjustments_size != 0 {
            return crate::error!("{domain} has hash buffers but no hash stream");
        }
        return Ok(result);
    };
    let hash_bytes = layout
        .read_stream(pdb_path, hash_stream_index)?
        .ok_or_else(|| {
            crate::Error::new(format!(
                "{domain} references absent hash stream {hash_stream_index}"
            ))
        })?;

    let hash_values = raw_region(
        &hash_bytes,
        hash_values_offset,
        hash_values_size,
        &format!("{domain} hash-value buffer"),
    )?;
    let record_count = (index_end - index_begin) as usize;
    if !hash_values.is_empty() {
        if hash_key_size == 0 || hash_values.len() % hash_key_size as usize != 0 {
            return crate::error!(
                "{domain} hash-value buffer is not aligned to key size {hash_key_size}"
            );
        }
        let hash_count = hash_values.len() / hash_key_size as usize;
        if hash_count != record_count {
            return crate::error!(
                "{domain} has {hash_count} hash values for {record_count} records"
            );
        }
        for position in 0..hash_count {
            let index = index_begin + position as u32;
            let identity = record_identity(records, index, domain);
            let start = position * hash_key_size as usize;
            let hash = hex_bytes(&hash_values[start..start + hash_key_size as usize]);
            result.hash_values.push(OrderItem {
                key: identity.key.clone(),
                value: format!(
                    "{domain}-index=0x{index:x} hash=0x{hash} {}",
                    identity.comparison_value
                ),
                comparison_value: format!("{}|hash=0x{hash}", identity.comparison_value),
            });
        }
    }

    let index_offsets = raw_region(
        &hash_bytes,
        index_offsets_offset,
        index_offsets_size,
        &format!("{domain} index-offset buffer"),
    )?;
    if index_offsets.len() % 8 != 0 {
        return crate::error!("{domain} index-offset buffer is not pair-aligned");
    }
    for position in 0..index_offsets.len() / 8 {
        let index = raw_u32(index_offsets, position * 8)?;
        let record_offset = raw_u32(index_offsets, position * 8 + 4)?;
        let identity = record_identity(records, index, domain);
        let detail = format!(
            "checkpoint#{position} {domain}-index=0x{index:x} record-offset=0x{record_offset:x} {}",
            identity.comparison_value
        );
        result.index_offsets.push(OrderItem {
            key: format!("checkpoint#{position}"),
            value: detail,
            comparison_value: format!(
                "{domain}-index=0x{index:x}|record-offset=0x{record_offset:x}|{}",
                identity.comparison_value
            ),
        });
    }

    let hash_adjustments = raw_region(
        &hash_bytes,
        hash_adjustments_offset,
        hash_adjustments_size,
        &format!("{domain} hash-adjustment buffer"),
    )?;
    if !hash_adjustments.is_empty() {
        append_hash_adjustments(
            hash_adjustments,
            domain,
            records,
            &mut result.metadata,
            &mut result.hash_adjustments,
        )?;
    }
    Ok(result)
}

fn record_identity(records: &HashMap<u32, OrderItem>, index: u32, domain: &str) -> OrderItem {
    records.get(&index).cloned().unwrap_or_else(|| OrderItem {
        key: format!("unresolved-{domain}-index=0x{index:x}"),
        value: format!("unresolved {domain} index 0x{index:x}"),
        comparison_value: format!("unresolved-{domain}-index=0x{index:x}"),
    })
}

fn raw_region<'a>(bytes: &'a [u8], offset: i32, size: u32, label: &str) -> crate::Result<&'a [u8]> {
    if size == 0 {
        return Ok(&[]);
    }
    let start = usize::try_from(offset)
        .map_err(|_| crate::Error::new(format!("{label} has negative offset {offset}")))?;
    let end = start
        .checked_add(size as usize)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| {
            crate::Error::new(format!(
                "{label} range 0x{start:x}+0x{size:x} exceeds stream size 0x{:x}",
                bytes.len()
            ))
        })?;
    Ok(&bytes[start..end])
}

fn append_hash_adjustments(
    bytes: &[u8],
    domain: &str,
    records: &HashMap<u32, OrderItem>,
    metadata: &mut Vec<OrderItem>,
    adjustments: &mut Vec<OrderItem>,
) -> crate::Result<()> {
    if bytes.len() < 8 {
        return crate::error!("{domain} hash-adjustment table is truncated");
    }
    let size = raw_u32(bytes, 0)? as usize;
    let capacity = raw_u32(bytes, 4)? as usize;
    if capacity == 0 || size > capacity.saturating_mul(2) / 3 + 1 {
        return crate::error!("invalid {domain} hash-adjustment size={size} capacity={capacity}");
    }
    let (present, cursor) = load_sparse_bit_vector(bytes, 8, capacity)?;
    let (deleted, mut cursor) = load_sparse_bit_vector(bytes, cursor, capacity)?;
    if present.len() != size {
        return crate::error!(
            "{domain} hash-adjustment table declares {size} entries but marks {} present",
            present.len()
        );
    }
    if present.iter().any(|bucket| deleted.contains(bucket)) {
        return crate::error!("{domain} hash-adjustment present/deleted buckets overlap");
    }
    let entries_end = cursor
        .checked_add(size.saturating_mul(8))
        .filter(|end| *end == bytes.len())
        .ok_or_else(|| {
            crate::Error::new(format!(
                "{domain} hash-adjustment entries do not consume their buffer"
            ))
        })?;
    for (key, value) in [
        ("hash-adjustment-size", size.to_string()),
        ("hash-adjustment-capacity", capacity.to_string()),
        ("hash-adjustment-deleted", deleted.len().to_string()),
    ] {
        let detail = format!("{key}={value}");
        metadata.push(OrderItem {
            key: key.to_owned(),
            value: detail.clone(),
            comparison_value: detail,
        });
    }

    let mut buckets = BTreeMap::new();
    for bucket in present {
        let old_hash = raw_u32(bytes, cursor)?;
        let index = raw_u32(bytes, cursor + 4)?;
        cursor += 8;
        let identity = record_identity(records, index, domain);
        buckets.insert(
            bucket,
            OrderItem {
                key: format!("bucket={bucket}"),
                value: format!(
                    "bucket={bucket} old-hash=0x{old_hash:08x} {domain}-index=0x{index:x} {}",
                    identity.comparison_value
                ),
                comparison_value: format!(
                    "old-hash=0x{old_hash:08x}|{}",
                    identity.comparison_value
                ),
            },
        );
    }
    debug_assert_eq!(cursor, entries_end);
    for bucket in deleted {
        buckets.insert(
            bucket,
            OrderItem {
                key: format!("bucket={bucket}"),
                value: format!("deleted bucket={bucket}"),
                comparison_value: "deleted".to_owned(),
            },
        );
    }
    adjustments.extend(buckets.into_values());
    Ok(())
}

#[derive(Default)]
struct GsiPsiInventory {
    global_records: Vec<OrderItem>,
    global_buckets: Vec<OrderItem>,
    public_records: Vec<OrderItem>,
    public_buckets: Vec<OrderItem>,
    public_address_map: Vec<OrderItem>,
    public_thunk_map: Vec<OrderItem>,
    public_section_map: Vec<OrderItem>,
}

fn load_gsi_psi_inventory(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
    dbi: &RawDbiInventory,
    symbols: &HashMap<u32, OrderItem>,
) -> crate::Result<GsiPsiInventory> {
    let mut result = GsiPsiInventory::default();
    if let Some(stream_index) = dbi.global_hash_stream {
        if let Some(bytes) = layout.read_stream(pdb_path, stream_index)? {
            let (_, records, buckets) = load_gsi_hash_table(&bytes, 0, symbols, "global")?;
            result.global_records = records;
            result.global_buckets = buckets;
        }
    }
    if let Some(stream_index) = dbi.public_hash_stream {
        if let Some(bytes) = layout.read_stream(pdb_path, stream_index)? {
            if bytes.len() < 28 {
                return crate::error!(
                    "public hash stream {stream_index} is only {} bytes",
                    bytes.len()
                );
            }
            let hash_size = raw_u32(&bytes, 0)? as usize;
            let address_map_size = raw_u32(&bytes, 4)? as usize;
            let thunk_count = raw_u32(&bytes, 8)? as usize;
            let thunk_size = raw_u32(&bytes, 12)?;
            let thunk_section = raw_u16(&bytes, 16)?;
            let thunk_offset = raw_u32(&bytes, 20)?;
            let section_count = raw_u32(&bytes, 24)? as usize;
            let (hash_end, records, buckets) = load_gsi_hash_table(&bytes, 28, symbols, "public")?;
            let declared_hash_end = 28usize
                .checked_add(hash_size)
                .ok_or_else(|| crate::Error::new("public hash size overflow".into()))?;
            if hash_end != declared_hash_end {
                return crate::error!(
                    "public hash table ends at 0x{hash_end:x}, header declares 0x{declared_hash_end:x}"
                );
            }
            result.public_records = records;
            result.public_buckets = buckets;

            if address_map_size % 4 != 0 {
                return crate::error!("public address-map size is not u32-aligned");
            }
            let address_end = hash_end
                .checked_add(address_map_size)
                .filter(|end| *end <= bytes.len())
                .ok_or_else(|| crate::Error::new("public address map is out of range".into()))?;
            for position in 0..address_map_size / 4 {
                let symbol_offset = raw_u32(&bytes, hash_end + position * 4)?;
                result.public_address_map.push(symbol_reference_item(
                    symbols,
                    symbol_offset,
                    0,
                    "address-map",
                ));
            }

            let thunk_map_end = address_end
                .checked_add(thunk_count.saturating_mul(4))
                .filter(|end| *end <= bytes.len())
                .ok_or_else(|| crate::Error::new("public thunk map is out of range".into()))?;
            for position in 0..thunk_count {
                let offset = raw_u32(&bytes, address_end + position * 4)?;
                let detail = format!(
                    "thunk#{position} offset=0x{offset:x} entry-size=0x{thunk_size:x} table={thunk_section}:0x{thunk_offset:x}"
                );
                result.public_thunk_map.push(OrderItem {
                    key: format!("thunk#{position}"),
                    value: detail.clone(),
                    comparison_value: detail,
                });
            }

            let section_map_end = if thunk_map_end == bytes.len() {
                // The reference reader treats NumSections as stale unless a
                // section-map payload actually remains in the stream.
                thunk_map_end
            } else {
                let end = thunk_map_end
                    .checked_add(section_count.saturating_mul(8))
                    .filter(|end| *end <= bytes.len())
                    .ok_or_else(|| {
                        crate::Error::new(format!(
                            "public section map count={section_count} needs {} bytes after 0x{thunk_map_end:x}, stream has {}",
                            section_count.saturating_mul(8),
                            bytes.len()
                        ))
                    })?;
                for position in 0..section_count {
                    let cursor = thunk_map_end + position * 8;
                    let offset = raw_u32(&bytes, cursor)?;
                    let section = raw_u16(&bytes, cursor + 4)?;
                    let detail =
                        format!("section-map#{position} section={section} offset=0x{offset:x}");
                    result.public_section_map.push(OrderItem {
                        key: format!("section-map#{position}"),
                        value: detail.clone(),
                        comparison_value: detail,
                    });
                }
                end
            };
            if section_map_end != bytes.len() {
                return crate::error!(
                    "public hash stream has {} trailing bytes",
                    bytes.len() - section_map_end
                );
            }
        }
    }
    Ok(result)
}

fn load_gsi_hash_table(
    bytes: &[u8],
    start: usize,
    symbols: &HashMap<u32, OrderItem>,
    domain: &str,
) -> crate::Result<(usize, Vec<OrderItem>, Vec<OrderItem>)> {
    const GSI_VERSION: u32 = 0xeffe_0000 + 19_990_810;
    const HASH_BUCKETS: usize = 4096;
    const BITMAP_WORDS: usize = (HASH_BUCKETS + 32) / 32;
    let header_end = start
        .checked_add(16)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new(format!("{domain} hash header is truncated")))?;
    let signature = raw_u32(bytes, start)?;
    let version = raw_u32(bytes, start + 4)?;
    if signature != u32::MAX || version != GSI_VERSION {
        return crate::error!(
            "unsupported {domain} hash header signature=0x{signature:x} version=0x{version:x}"
        );
    }
    let record_bytes = raw_u32(bytes, start + 8)? as usize;
    let bucket_bytes = raw_u32(bytes, start + 12)? as usize;
    if record_bytes % 8 != 0 || bucket_bytes % 4 != 0 {
        return crate::error!("invalid {domain} hash table alignment");
    }
    let records_end = header_end
        .checked_add(record_bytes)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new(format!("{domain} hash records are out of range")))?;
    let table_end = records_end
        .checked_add(bucket_bytes)
        .filter(|end| *end <= bytes.len())
        .ok_or_else(|| crate::Error::new(format!("{domain} hash buckets are out of range")))?;
    let mut records = Vec::with_capacity(record_bytes / 8);
    for position in 0..record_bytes / 8 {
        let cursor = header_end + position * 8;
        let stored_offset = raw_u32(bytes, cursor)?;
        let reference_count = raw_u32(bytes, cursor + 4)?;
        if stored_offset == 0 {
            return crate::error!("{domain} hash record has zero symbol offset");
        }
        records.push(symbol_reference_item(
            symbols,
            stored_offset - 1,
            reference_count,
            "hash-record",
        ));
    }

    let mut buckets = Vec::new();
    if bucket_bytes != 0 {
        let bitmap_bytes = BITMAP_WORDS * 4;
        if bucket_bytes < bitmap_bytes {
            return crate::error!("{domain} hash bitmap is truncated");
        }
        let mut populated = Vec::new();
        for bucket in 0..=HASH_BUCKETS {
            let word = raw_u32(bytes, records_end + bucket / 32 * 4)?;
            if word & (1 << (bucket % 32)) != 0 {
                populated.push(bucket);
            }
        }
        if bitmap_bytes + populated.len() * 4 != bucket_bytes {
            return crate::error!(
                "{domain} hash bitmap names {} buckets but payload has {} entries",
                populated.len(),
                (bucket_bytes - bitmap_bytes) / 4
            );
        }
        for (position, bucket) in populated.into_iter().enumerate() {
            let start_offset = raw_u32(bytes, records_end + bitmap_bytes + position * 4)?;
            let detail = format!(
                "bucket={bucket} start-byte=0x{start_offset:x} start-record={}",
                start_offset / 12
            );
            buckets.push(OrderItem {
                key: format!("bucket={bucket}"),
                value: detail.clone(),
                comparison_value: detail,
            });
        }
    }
    Ok((table_end, records, buckets))
}

fn symbol_reference_item(
    symbols: &HashMap<u32, OrderItem>,
    symbol_offset: u32,
    reference_count: u32,
    domain: &str,
) -> OrderItem {
    let symbol = symbols.get(&symbol_offset);
    let key = symbol.map_or_else(
        || format!("unresolved-symbol-offset=0x{symbol_offset:x}"),
        |item| item.key.clone(),
    );
    let semantic = symbol.map_or_else(|| key.clone(), |item| item.comparison_value.clone());
    OrderItem {
        key,
        value: format!(
            "{domain} symbol-offset=0x{symbol_offset:x} references={reference_count} {semantic}"
        ),
        comparison_value: format!("{semantic}|references={reference_count}"),
    }
}

#[derive(Default)]
struct RawModuleDebugScopes {
    subsections: Vec<ModuleOrderScope>,
    files: Vec<ModuleOrderScope>,
    lines: Vec<ModuleOrderScope>,
    strings: Vec<ModuleOrderScope>,
    frame_data: Vec<ModuleOrderScope>,
    inlinees: Vec<ModuleOrderScope>,
    cross_imports: Vec<ModuleOrderScope>,
    cross_exports: Vec<ModuleOrderScope>,
    other: Vec<ModuleOrderScope>,
}

struct RawC13Subsection<'a> {
    kind: u32,
    offset: usize,
    data: &'a [u8],
}

fn load_module_debug_scopes(
    pdb_path: &PathBuf,
    layout: &MsfLayout,
    dbi: &RawDbiInventory,
    global_names: &HashMap<usize, String>,
    types: &HashMap<u32, OrderItem>,
    ids: &HashMap<u32, OrderItem>,
) -> crate::Result<RawModuleDebugScopes> {
    let mut result = RawModuleDebugScopes {
        subsections: Vec::with_capacity(dbi.modules.len()),
        files: Vec::with_capacity(dbi.modules.len()),
        lines: Vec::with_capacity(dbi.modules.len()),
        strings: Vec::with_capacity(dbi.modules.len()),
        frame_data: Vec::with_capacity(dbi.modules.len()),
        inlinees: Vec::with_capacity(dbi.modules.len()),
        cross_imports: Vec::with_capacity(dbi.modules.len()),
        cross_exports: Vec::with_capacity(dbi.modules.len()),
        other: Vec::with_capacity(dbi.modules.len()),
    };
    for module in &dbi.modules {
        let mut subsection_items = Vec::new();
        let mut file_items = Vec::new();
        let mut line_items = Vec::new();
        let mut string_items = Vec::new();
        let mut frame_data_items = Vec::new();
        let mut inlinee_items = Vec::new();
        let mut cross_import_items = Vec::new();
        let mut cross_export_items = Vec::new();
        let mut other_items = Vec::new();
        if module.c13_lines_size != 0 {
            if let Some(stream_index) = module.stream_index {
                if let Some(bytes) = layout.read_stream(pdb_path, stream_index)? {
                    let start = usize::try_from(module.symbols_size)?
                        .checked_add(usize::try_from(module.lines_size)?)
                        .ok_or_else(|| {
                            crate::Error::new("module subsection offset overflow".into())
                        })?;
                    let end = start
                        .checked_add(usize::try_from(module.c13_lines_size)?)
                        .filter(|end| *end <= bytes.len())
                        .ok_or_else(|| {
                            crate::Error::new(format!(
                                "C13 subsection region is out of range for {}",
                                module.value
                            ))
                        })?;
                    let mut cursor = start;
                    let mut occurrences = HashMap::new();
                    let mut subsections = Vec::new();
                    while cursor < end {
                        if cursor + 8 > end {
                            return crate::error!(
                                "truncated C13 subsection header for {}",
                                module.value
                            );
                        }
                        let raw_kind = raw_u32(&bytes, cursor)?;
                        let size = raw_u32(&bytes, cursor + 4)? as usize;
                        let occurrence = occurrences.entry(raw_kind).or_insert(0usize);
                        let key = format!("kind=0x{raw_kind:08x}|occurrence={occurrence}");
                        *occurrence += 1;
                        subsection_items.push(OrderItem {
                            key,
                            value: format!(
                                "offset=0x{:x} kind=0x{raw_kind:08x} bytes=0x{size:x}",
                                cursor - start
                            ),
                            comparison_value: format!("kind=0x{raw_kind:08x}|bytes=0x{size:x}"),
                        });
                        let data_start = cursor.checked_add(8).ok_or_else(|| {
                            crate::Error::new("C13 subsection offset overflow".into())
                        })?;
                        let data_end = data_start.checked_add(size).ok_or_else(|| {
                            crate::Error::new("C13 subsection size overflow".into())
                        })?;
                        if data_end > end {
                            return crate::error!(
                                "C13 subsection crosses region boundary for {}",
                                module.value
                            );
                        }
                        subsections.push(RawC13Subsection {
                            kind: raw_kind,
                            offset: cursor - start,
                            data: &bytes[data_start..data_end],
                        });
                        cursor = align4(data_end);
                        if cursor > end {
                            return crate::error!(
                                "C13 subsection crosses region boundary for {}",
                                module.value
                            );
                        }
                    }

                    let string_table = subsections
                        .iter()
                        .find(|subsection| subsection.kind == 0xf3)
                        .map(|subsection| subsection.data)
                        .unwrap_or_default();
                    let (parsed_strings, local_names) =
                        parse_raw_c13_string_table(string_table, &module.value)?;
                    string_items = parsed_strings;
                    let mut file_names = HashMap::new();
                    let mut file_occurrences = HashMap::new();
                    for subsection in subsections
                        .iter()
                        .filter(|subsection| subsection.kind == 0xf4)
                    {
                        append_raw_c13_files(
                            subsection,
                            string_table,
                            global_names,
                            &mut file_names,
                            &mut file_occurrences,
                            &mut file_items,
                            &module.value,
                        )?;
                    }
                    let mut line_occurrences = HashMap::new();
                    for subsection in subsections
                        .iter()
                        .filter(|subsection| subsection.kind == 0xf2)
                    {
                        append_raw_c13_lines(
                            subsection,
                            &file_names,
                            &mut line_occurrences,
                            &mut line_items,
                            &module.value,
                        )?;
                    }
                    for (occurrence, subsection) in subsections
                        .iter()
                        .filter(|subsection| subsection.kind == 0xf5)
                        .enumerate()
                    {
                        append_prefixed_items(
                            "frame-data",
                            occurrence,
                            parse_frame_data_records(
                                subsection.data,
                                &local_names,
                                &HashMap::new(),
                                &format!("C13 frame data for {}", module.value),
                            )?,
                            &mut frame_data_items,
                        );
                    }
                    for (occurrence, subsection) in subsections
                        .iter()
                        .filter(|subsection| subsection.kind == 0xf6)
                        .enumerate()
                    {
                        append_prefixed_items(
                            "inlinees",
                            occurrence,
                            parse_raw_c13_inlinees(
                                subsection.data,
                                &file_names,
                                ids,
                                &module.value,
                            )?,
                            &mut inlinee_items,
                        );
                    }
                    for (occurrence, subsection) in subsections
                        .iter()
                        .filter(|subsection| subsection.kind == 0xf7)
                        .enumerate()
                    {
                        append_prefixed_items(
                            "cross-imports",
                            occurrence,
                            parse_raw_c13_cross_imports(
                                subsection.data,
                                global_names,
                                &module.value,
                            )?,
                            &mut cross_import_items,
                        );
                    }
                    for (occurrence, subsection) in subsections
                        .iter()
                        .filter(|subsection| subsection.kind == 0xf8)
                        .enumerate()
                    {
                        append_prefixed_items(
                            "cross-exports",
                            occurrence,
                            parse_raw_c13_cross_exports(
                                subsection.data,
                                types,
                                ids,
                                &module.value,
                            )?,
                            &mut cross_export_items,
                        );
                    }
                    for (occurrence, subsection) in subsections
                        .iter()
                        .filter(|subsection| !matches!(subsection.kind, 0xf2..=0xf8))
                        .enumerate()
                    {
                        append_prefixed_items(
                            &format!("kind=0x{:x}", subsection.kind),
                            occurrence,
                            parse_raw_c13_words(subsection.data),
                            &mut other_items,
                        );
                    }
                }
            }
        }
        result.subsections.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: subsection_items,
        });
        result.files.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: file_items,
        });
        result.lines.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: line_items,
        });
        result.strings.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: string_items,
        });
        result.frame_data.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: frame_data_items,
        });
        result.inlinees.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: inlinee_items,
        });
        result.cross_imports.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: cross_import_items,
        });
        result.cross_exports.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: cross_export_items,
        });
        result.other.push(ModuleOrderScope {
            key: module.key.clone(),
            value: module.value.clone(),
            symbols: other_items,
        });
    }
    Ok(result)
}

fn append_prefixed_items(
    domain: &str,
    subsection_occurrence: usize,
    items: Vec<OrderItem>,
    output: &mut Vec<OrderItem>,
) {
    output.extend(items.into_iter().map(|item| OrderItem {
        key: format!("{domain}|subsection={subsection_occurrence}|{}", item.key),
        value: format!("{domain} subsection#{subsection_occurrence} {}", item.value),
        comparison_value: item.comparison_value,
    }));
}

fn parse_raw_c13_string_table(
    bytes: &[u8],
    module: &str,
) -> crate::Result<(Vec<OrderItem>, HashMap<usize, String>)> {
    let mut output = Vec::new();
    let mut by_offset = HashMap::new();
    let mut occurrences = HashMap::new();
    let mut cursor = 0usize;
    while cursor < bytes.len() {
        let offset = cursor;
        let (value, next) = raw_cstring(bytes, cursor, bytes.len()).map_err(|_| {
            crate::Error::new(format!(
                "unterminated C13 string at 0x{cursor:x} for {module}"
            ))
        })?;
        let semantic = if value.contains('\\') {
            normalize_pdb_path(&value)
        } else {
            value.clone()
        };
        let occurrence = occurrences.entry(semantic.clone()).or_insert(0usize);
        output.push(OrderItem {
            key: format!("{semantic}|occurrence={occurrence}"),
            value: format!("offset=0x{offset:x} string={value:?}"),
            comparison_value: semantic.clone(),
        });
        *occurrence += 1;
        by_offset.insert(offset, semantic);
        cursor = next;
    }
    Ok((output, by_offset))
}

fn parse_raw_c13_inlinees(
    bytes: &[u8],
    file_names: &HashMap<u32, String>,
    ids: &HashMap<u32, OrderItem>,
    module: &str,
) -> crate::Result<Vec<OrderItem>> {
    if bytes.len() < 4 {
        return crate::error!("C13 inlinee subsection is truncated for {module}");
    }
    let signature = raw_u32(bytes, 0)?;
    if signature > 1 {
        return crate::error!(
            "C13 inlinee subsection has unknown signature {signature} for {module}"
        );
    }
    let mut output = Vec::new();
    let mut occurrences = HashMap::new();
    let mut cursor = 4usize;
    while cursor < bytes.len() {
        if cursor + 12 > bytes.len() {
            return crate::error!("truncated C13 inlinee record for {module}");
        }
        let inlinee = raw_u32(bytes, cursor)?;
        let file_id = raw_u32(bytes, cursor + 4)?;
        let line = raw_u32(bytes, cursor + 8)?;
        cursor += 12;
        let mut extra_files = Vec::new();
        if signature == 1 {
            let count = raw_u32(bytes, cursor)? as usize;
            cursor += 4;
            let end = cursor
                .checked_add(count.saturating_mul(4))
                .filter(|end| *end <= bytes.len())
                .ok_or_else(|| {
                    crate::Error::new(format!(
                        "C13 inlinee extra files cross subsection for {module}"
                    ))
                })?;
            for position in 0..count {
                let extra_id = raw_u32(bytes, cursor + position * 4)?;
                extra_files.push(resolve_c13_file(file_names, extra_id));
            }
            cursor = end;
        }
        let identity = record_identity(ids, inlinee, "ipi");
        let file = resolve_c13_file(file_names, file_id);
        let semantic_key = format!("{}|file={file}|line={line}", identity.key);
        let occurrence = occurrences.entry(semantic_key.clone()).or_insert(0usize);
        let key = format!("{semantic_key}|occurrence={occurrence}");
        *occurrence += 1;
        let detail = format!(
            "{} file={file} line={line} extra-files={extra_files:?} signature={signature}",
            identity.comparison_value
        );
        output.push(OrderItem {
            key,
            value: format!("inlinee-index=0x{inlinee:x} file-id=0x{file_id:x} {detail}"),
            comparison_value: detail,
        });
    }
    Ok(output)
}

fn resolve_c13_file(file_names: &HashMap<u32, String>, file_id: u32) -> String {
    file_names
        .get(&file_id)
        .cloned()
        .unwrap_or_else(|| format!("<unresolved-file-id-0x{file_id:x}>"))
}

fn parse_raw_c13_cross_imports(
    bytes: &[u8],
    global_names: &HashMap<usize, String>,
    module: &str,
) -> crate::Result<Vec<OrderItem>> {
    let mut output = Vec::new();
    let mut module_occurrences = HashMap::new();
    let mut cursor = 0usize;
    while cursor < bytes.len() {
        if cursor + 8 > bytes.len() {
            return crate::error!("truncated C13 cross-import header for {module}");
        }
        let module_offset = raw_u32(bytes, cursor)? as usize;
        let count = raw_u32(bytes, cursor + 4)? as usize;
        cursor += 8;
        let end = cursor
            .checked_add(count.saturating_mul(4))
            .filter(|end| *end <= bytes.len())
            .ok_or_else(|| {
                crate::Error::new(format!("C13 cross imports cross subsection for {module}"))
            })?;
        let imported_module = global_names
            .get(&module_offset)
            .cloned()
            .unwrap_or_else(|| format!("<unresolved-module-offset-0x{module_offset:x}>"));
        let occurrence = module_occurrences
            .entry(imported_module.clone())
            .or_insert(0usize);
        let prefix = format!("module={imported_module}|occurrence={occurrence}");
        *occurrence += 1;
        output.push(OrderItem {
            key: format!("{prefix}|header"),
            value: format!(
                "module-offset=0x{module_offset:x} module={imported_module} imports={count}"
            ),
            comparison_value: format!("module={imported_module}|imports={count}"),
        });
        for position in 0..count {
            let local_index = raw_u32(bytes, cursor + position * 4)?;
            let detail = format!("module={imported_module}|local-index=0x{local_index:x}");
            output.push(OrderItem {
                key: format!("{prefix}|import#{position}"),
                value: detail.clone(),
                comparison_value: detail,
            });
        }
        cursor = end;
    }
    Ok(output)
}

fn parse_raw_c13_cross_exports(
    bytes: &[u8],
    types: &HashMap<u32, OrderItem>,
    ids: &HashMap<u32, OrderItem>,
    module: &str,
) -> crate::Result<Vec<OrderItem>> {
    if bytes.len() % 8 != 0 {
        return crate::error!("C13 cross-export subsection is not pair-aligned for {module}");
    }
    let mut output = Vec::with_capacity(bytes.len() / 8);
    let mut occurrences = HashMap::new();
    for position in 0..bytes.len() / 8 {
        let local = raw_u32(bytes, position * 8)?;
        let global = raw_u32(bytes, position * 8 + 4)?;
        let is_id = local & 0x8000_0000 != 0;
        let identity = record_identity(
            if is_id { ids } else { types },
            global,
            if is_id { "ipi" } else { "tpi" },
        );
        let semantic_key = format!(
            "{}|local=0x{local:x}|{}",
            if is_id { "id" } else { "type" },
            identity.key
        );
        let occurrence = occurrences.entry(semantic_key.clone()).or_insert(0usize);
        output.push(OrderItem {
            key: format!("{semantic_key}|occurrence={occurrence}"),
            value: format!(
                "export#{position} local=0x{local:x} global=0x{global:x} {}",
                identity.comparison_value
            ),
            comparison_value: format!(
                "kind={}|local=0x{local:x}|{}",
                if is_id { "id" } else { "type" },
                identity.comparison_value
            ),
        });
        *occurrence += 1;
    }
    Ok(output)
}

fn parse_raw_c13_words(bytes: &[u8]) -> Vec<OrderItem> {
    let mut output = Vec::with_capacity(bytes.len().div_ceil(4));
    for (position, chunk) in bytes.chunks(4).enumerate() {
        let value = format!("0x{}", hex_bytes(chunk));
        output.push(OrderItem {
            key: format!("word#{position}"),
            value: value.clone(),
            comparison_value: value,
        });
    }
    output
}

fn append_raw_c13_files(
    subsection: &RawC13Subsection<'_>,
    string_table: &[u8],
    global_names: &HashMap<usize, String>,
    file_names: &mut HashMap<u32, String>,
    occurrences: &mut HashMap<String, usize>,
    output: &mut Vec<OrderItem>,
    module: &str,
) -> crate::Result<()> {
    let mut cursor = 0usize;
    while cursor < subsection.data.len() {
        if cursor + 6 > subsection.data.len() {
            return crate::error!("truncated C13 file checksum in {module}");
        }
        let name_offset = raw_u32(subsection.data, cursor)?;
        let checksum_size = subsection.data[cursor + 4] as usize;
        let checksum_kind = subsection.data[cursor + 5];
        let checksum_start = cursor + 6;
        let checksum_end = checksum_start
            .checked_add(checksum_size)
            .filter(|end| *end <= subsection.data.len())
            .ok_or_else(|| {
                crate::Error::new(format!(
                    "C13 file checksum payload is out of range in {module}"
                ))
            })?;
        let name = if string_table.is_empty() {
            global_names.get(&(name_offset as usize)).cloned()
        } else {
            raw_string_at(string_table, name_offset as usize)
        }
        .unwrap_or_else(|| format!("<string-offset-0x{name_offset:x}>"));
        let normalized = normalize_pdb_path(&name);
        let occurrence = occurrences.entry(normalized.clone()).or_default();
        let key = format!("{normalized}|occurrence={occurrence}");
        *occurrence += 1;
        let checksum = hex_bytes(&subsection.data[checksum_start..checksum_end]);
        let detail = format!(
            "file-index=0x{cursor:x} name={name} name-offset=0x{name_offset:x} checksum-kind={checksum_kind} checksum={checksum}"
        );
        output.push(OrderItem {
            key,
            value: detail,
            comparison_value: format!(
                "path={normalized}|checksum-kind={checksum_kind}|checksum={checksum}"
            ),
        });
        file_names.entry(cursor as u32).or_insert(normalized);
        cursor = align4(checksum_end);
        if cursor > subsection.data.len() {
            return crate::error!("C13 file checksum alignment crosses {module}");
        }
    }
    Ok(())
}

fn append_raw_c13_lines(
    subsection: &RawC13Subsection<'_>,
    file_names: &HashMap<u32, String>,
    occurrences: &mut HashMap<String, usize>,
    output: &mut Vec<OrderItem>,
    module: &str,
) -> crate::Result<()> {
    if subsection.data.len() < 12 {
        return crate::error!("truncated C13 line header in {module}");
    }
    let contribution_offset = raw_u32(subsection.data, 0)?;
    let contribution_section = raw_u16(subsection.data, 4)?;
    let flags = raw_u16(subsection.data, 6)?;
    let code_size = raw_u32(subsection.data, 8)?;
    let has_columns = flags & 1 != 0;
    let mut cursor = 12usize;
    while cursor < subsection.data.len() {
        if cursor + 12 > subsection.data.len() {
            return crate::error!("truncated C13 line block in {module}");
        }
        let file_index = raw_u32(subsection.data, cursor)?;
        let line_count = raw_u32(subsection.data, cursor + 4)? as usize;
        let block_size = raw_u32(subsection.data, cursor + 8)? as usize;
        if block_size < 12 {
            return crate::error!("invalid C13 line block size in {module}");
        }
        let block_end = cursor
            .checked_add(block_size)
            .filter(|end| *end <= subsection.data.len())
            .ok_or_else(|| {
                crate::Error::new(format!("C13 line block crosses subsection in {module}"))
            })?;
        let lines_start = cursor + 12;
        let lines_end = lines_start
            .checked_add(line_count.saturating_mul(8))
            .filter(|end| *end <= block_end)
            .ok_or_else(|| {
                crate::Error::new(format!("C13 line records cross block in {module}"))
            })?;
        let columns_end = if has_columns {
            lines_end
                .checked_add(line_count.saturating_mul(4))
                .filter(|end| *end <= block_end)
                .ok_or_else(|| {
                    crate::Error::new(format!("C13 column records cross block in {module}"))
                })?
        } else {
            lines_end
        };
        let path = file_names
            .get(&file_index)
            .cloned()
            .unwrap_or_else(|| format!("<file-index-0x{file_index:x}>"));
        for line_index in 0..line_count {
            let line_cursor = lines_start + line_index * 8;
            let line_offset = raw_u32(subsection.data, line_cursor)?;
            let line_flags = raw_u32(subsection.data, line_cursor + 4)?;
            let line_start = line_flags & 0x00ff_ffff;
            let line_delta = (line_flags >> 24) & 0x7f;
            let kind = if line_flags & 0x8000_0000 != 0 {
                "statement"
            } else {
                "expression"
            };
            let marker = match line_start {
                0x00fe_efee => "do-not-step-onto",
                0x00f0_0f00 => "do-not-step-into",
                _ => "line",
            };
            let columns = if has_columns {
                let column_cursor = lines_end + line_index * 4;
                format!(
                    "{}-{}",
                    raw_u16(subsection.data, column_cursor)?,
                    raw_u16(subsection.data, column_cursor + 2)?
                )
            } else {
                "none".to_owned()
            };
            let semantic = format!(
                "{path}|start={line_start}|delta={line_delta}|kind={kind}|marker={marker}|columns={columns}"
            );
            let occurrence = occurrences.entry(semantic.clone()).or_default();
            let key = format!("{semantic}|occurrence={occurrence}");
            *occurrence += 1;
            output.push(OrderItem {
                key,
                value: format!(
                    "file={path} file-index=0x{file_index:x} line={line_start}+{line_delta} kind={kind} marker={marker} section={contribution_section} offset=0x{:x} contribution-size=0x{code_size:x} columns={columns} subsection=0x{:x}",
                    contribution_offset.saturating_add(line_offset),
                    subsection.offset,
                ),
                comparison_value: semantic,
            });
        }
        let _unparsed_extension = &subsection.data[columns_end..block_end];
        cursor = block_end;
    }
    Ok(())
}

fn raw_string_at(bytes: &[u8], offset: usize) -> Option<String> {
    let remainder = bytes.get(offset..)?;
    let end = remainder.iter().position(|byte| *byte == 0)?;
    Some(String::from_utf8_lossy(&remainder[..end]).into_owned())
}

fn hex_bytes(bytes: &[u8]) -> String {
    bytes.iter().map(|byte| format!("{byte:02x}")).collect()
}

fn optional_stream(value: u16) -> Option<u32> {
    (value != u16::MAX).then_some(u32::from(value))
}

fn raw_u16(bytes: &[u8], offset: usize) -> crate::Result<u16> {
    let Some(value) = bytes.get(offset..offset.saturating_add(2)) else {
        return crate::error!("short raw PDB read at 0x{offset:x}");
    };
    Ok(u16::from_le_bytes([value[0], value[1]]))
}

fn raw_u32(bytes: &[u8], offset: usize) -> crate::Result<u32> {
    let Some(value) = bytes.get(offset..offset.saturating_add(4)) else {
        return crate::error!("short raw PDB read at 0x{offset:x}");
    };
    Ok(u32::from_le_bytes([value[0], value[1], value[2], value[3]]))
}

fn raw_i32(bytes: &[u8], offset: usize) -> crate::Result<i32> {
    Ok(raw_u32(bytes, offset)? as i32)
}

fn raw_cstring(bytes: &[u8], start: usize, end: usize) -> crate::Result<(String, usize)> {
    let Some(relative_end) = bytes
        .get(start..end)
        .and_then(|value| value.iter().position(|byte| *byte == 0))
    else {
        return crate::error!("unterminated DBI module string at 0x{start:x}");
    };
    let finish = start + relative_end;
    Ok((
        String::from_utf8_lossy(&bytes[start..finish]).into_owned(),
        finish + 1,
    ))
}

fn align4(value: usize) -> usize {
    value.saturating_add(3) & !3
}

fn ordered_stream_roles(side: &OrderSide) -> Vec<OrderItem> {
    let Some(layout) = &side.layout else {
        return Vec::new();
    };
    side.stream_roles
        .iter()
        .map(|binding| {
            let stream = layout.stream(binding.stream_index);
            let (size, pages, runs) = stream.map_or((None, 0, 0), |stream| {
                (stream.size, stream.pages.len(), stream.page_runs())
            });
            OrderItem {
                key: binding.key.clone(),
                value: format!(
                    "stream={} role={} size={size:?} pages={pages} runs={runs}",
                    binding.stream_index, binding.value
                ),
                comparison_value: format!(
                    "stream={}|size={size:?}|pages={pages}|runs={runs}",
                    binding.stream_index
                ),
            }
        })
        .collect()
}

fn compare_msf_layouts(
    base_path: &PathBuf,
    base: &OrderSide,
    target_path: &PathBuf,
    target: &OrderSide,
) -> crate::Result<MsfLayoutComparison> {
    let base_layout = base
        .layout
        .as_ref()
        .ok_or_else(|| crate::Error::new("missing base MSF layout".into()))?;
    let target_layout = target
        .layout
        .as_ref()
        .ok_or_else(|| crate::Error::new("missing target MSF layout".into()))?;
    let base_roles: BTreeMap<&str, &StreamRoleBinding> = base
        .stream_roles
        .iter()
        .map(|binding| (binding.key.as_str(), binding))
        .collect();
    let target_roles: BTreeMap<&str, &StreamRoleBinding> = target
        .stream_roles
        .iter()
        .map(|binding| (binding.key.as_str(), binding))
        .collect();
    let role_keys: BTreeSet<&str> = base_roles
        .keys()
        .chain(target_roles.keys())
        .copied()
        .collect();
    let stable_roles = role_keys
        .into_iter()
        .map(|role| {
            let base_binding = base_roles.get(role).copied();
            let target_binding = target_roles.get(role).copied();
            let value = base_binding
                .or(target_binding)
                .map(|binding| binding.value.clone())
                .unwrap_or_else(|| role.to_owned());
            StableStreamLayoutComparison {
                role: role.to_owned(),
                value,
                base: base_binding.and_then(|binding| {
                    base_layout
                        .stream(binding.stream_index)
                        .map(stream_layout_observation)
                }),
                target: target_binding.and_then(|binding| {
                    target_layout
                        .stream(binding.stream_index)
                        .map(stream_layout_observation)
                }),
            }
        })
        .collect();
    let base_known: HashSet<u32> = base
        .stream_roles
        .iter()
        .map(|binding| binding.stream_index)
        .collect();
    let target_known: HashSet<u32> = target
        .stream_roles
        .iter()
        .map(|binding| binding.stream_index)
        .collect();

    Ok(MsfLayoutComparison {
        confidence: "physical MSF serialization; page numbers are diagnostic, not source-order proof",
        base: msf_layout_summary(base_path, base_layout)?,
        target: msf_layout_summary(target_path, target_layout)?,
        stable_roles,
        unidentified_base: base_layout
            .streams
            .iter()
            .filter(|stream| stream.size.is_some() && !base_known.contains(&stream.index))
            .map(stream_layout_observation)
            .collect(),
        unidentified_target: target_layout
            .streams
            .iter()
            .filter(|stream| stream.size.is_some() && !target_known.contains(&stream.index))
            .map(stream_layout_observation)
            .collect(),
    })
}

fn msf_layout_summary(path: &PathBuf, layout: &MsfLayout) -> crate::Result<MsfLayoutSummary> {
    let present: Vec<&MsfStreamLayout> = layout
        .streams
        .iter()
        .filter(|stream| stream.size.is_some())
        .collect();
    Ok(MsfLayoutSummary {
        format: layout.format.to_owned(),
        page_size: layout.page_size,
        free_page_map: layout.free_page_map,
        free_page_map_pages: layout.free_page_map_pages.clone(),
        free_pages: layout.free_pages.len(),
        pages_used: layout.pages_used,
        file_bytes: std::fs::metadata(path)?.len(),
        directory_size: layout.directory_size,
        directory_map_pages: layout.directory_map_pages.clone(),
        directory_pages: layout.directory_pages.clone(),
        directory_page_runs: page_runs(&layout.directory_pages),
        stream_slots: layout.streams.len(),
        present_streams: present.len(),
        absent_streams: layout.streams.len() - present.len(),
        stream_bytes: present
            .iter()
            .filter_map(|stream| stream.size)
            .map(u64::from)
            .sum(),
        stream_pages: present.iter().map(|stream| stream.pages.len()).sum(),
        stream_page_runs: present.iter().map(|stream| stream.page_runs()).sum(),
        fragmented_streams: present
            .iter()
            .filter(|stream| stream.page_runs() > 1)
            .count(),
    })
}

fn page_runs(pages: &[u32]) -> usize {
    if pages.is_empty() {
        0
    } else {
        1 + pages
            .windows(2)
            .filter(|pair| pair[1] != pair[0].saturating_add(1))
            .count()
    }
}

fn stream_layout_observation(stream: &MsfStreamLayout) -> StreamLayoutObservation {
    StreamLayoutObservation {
        stream_index: stream.index,
        size: stream.size,
        page_count: stream.pages.len(),
        page_runs: stream.page_runs(),
        pages: stream.pages.clone(),
    }
}

fn enum_order_scope(
    fmt: &PdbParser<'_, '_>,
    finder: &pdb::TypeFinder<'_>,
    type_index: u32,
    enumeration: &pdb::EnumerationType<'_>,
) -> crate::Result<ModuleOrderScope> {
    let name = enumeration.name.to_string().into_owned();
    let key = normalize_cross_pdb_type(name.to_lowercase());
    let underlying = comparable_type_name(fmt, enumeration.underlying_type);
    let header = format!(
        "underlying={underlying}|count={}|properties={:?}",
        enumeration.count, enumeration.properties
    );
    let mut symbols = vec![OrderItem {
        key: "<enum-header>".to_owned(),
        value: header.clone(),
        comparison_value: header.clone(),
    }];
    append_enum_order_items(
        finder,
        enumeration.fields,
        &mut symbols,
        &mut HashSet::new(),
    )?;
    Ok(ModuleOrderScope {
        key,
        value: format!("type=0x{type_index:x} enum {name} {header}"),
        symbols,
    })
}

fn append_enum_order_items(
    finder: &pdb::TypeFinder<'_>,
    field_index: pdb::TypeIndex,
    symbols: &mut Vec<OrderItem>,
    seen: &mut HashSet<u32>,
) -> crate::Result<()> {
    if !seen.insert(field_index.0) {
        return Ok(());
    }
    let pdb::TypeData::FieldList(list) = finder.find(field_index)?.parse()? else {
        return Ok(());
    };
    for field in list.fields {
        if let pdb::TypeData::Enumerate(enumerator) = field {
            let name = enumerator.name.to_string().into_owned();
            let value = format!("{:?}", enumerator.value);
            let detail = format!(
                "value={value} name={name} attributes={:?}",
                enumerator.attributes
            );
            symbols.push(OrderItem {
                key: format!("value={value}"),
                value: detail.clone(),
                comparison_value: detail,
            });
        }
    }
    if let Some(continuation) = list.continuation {
        append_enum_order_items(finder, continuation, symbols, seen)?;
    }
    Ok(())
}

fn named_type_order_item(
    fmt: &PdbParser<'_, '_>,
    index: u32,
    data: pdb::TypeData<'_>,
) -> Option<OrderItem> {
    let (kind, name, complete, detail) = match data {
        pdb::TypeData::Class(value) => (
            class_kind(value.kind),
            value.name.to_string().into_owned(),
            !value.properties.forward_reference(),
            format!("size=0x{:x}", value.size),
        ),
        pdb::TypeData::Union(value) => (
            "union",
            value.name.to_string().into_owned(),
            !value.properties.forward_reference(),
            format!("size=0x{:x}", value.size),
        ),
        pdb::TypeData::Enumeration(value) => (
            "enum",
            value.name.to_string().into_owned(),
            !value.properties.forward_reference(),
            format!(
                "underlying={}",
                comparable_type_name(fmt, value.underlying_type)
            ),
        ),
        pdb::TypeData::Alias(value) => (
            "alias",
            value.name.to_string().into_owned(),
            true,
            format!(
                "underlying={}",
                comparable_type_name(fmt, value.underlying_type)
            ),
        ),
        _ => return None,
    };
    if !complete {
        return None;
    }
    let normalized = normalize_cross_pdb_type(name.clone()).to_lowercase();
    Some(OrderItem {
        key: format!("{kind}|{normalized}"),
        value: format!("type=0x{index:x} {kind} {name} {detail}"),
        comparison_value: format!("{kind}|{normalized}|{detail}"),
    })
}

fn direct_symbol_type_index(data: &SymbolData<'_>) -> Option<u32> {
    let index = match data {
        SymbolData::Procedure(value) => value.type_index,
        SymbolData::RegisterVariable(value) => value.type_index,
        SymbolData::MultiRegisterVariable(value) => value.type_index,
        SymbolData::Constant(value) => value.type_index,
        SymbolData::UserDefinedType(value) => value.type_index,
        SymbolData::Data(value) => value.type_index,
        SymbolData::ThreadStorage(value) => value.type_index,
        SymbolData::Local(value) => value.type_index,
        SymbolData::ManagedSlot(value) => value.type_index,
        SymbolData::RegisterRelative(value) => value.type_index,
        SymbolData::BasePointerRelative(value) => value.type_index,
        SymbolData::CallSiteInfo(value) => value.type_index,
        _ => return None,
    };
    Some(index.0)
}

fn build_named_type_module_references(
    named_types: &[(u32, OrderItem)],
    references: &HashMap<u32, BTreeSet<(usize, String)>>,
) -> Vec<OrderItem> {
    named_types
        .iter()
        .filter_map(|(type_index, item)| {
            let modules = references.get(type_index)?;
            let module_keys: Vec<&str> = modules
                .iter()
                .map(|(_, module_key)| module_key.as_str())
                .collect();
            let first = module_keys.first()?;
            let module_sequence = module_keys.join(" -> ");
            Some(OrderItem {
                key: item.key.clone(),
                value: format!(
                    "{} first-direct-module={} direct-modules=[{}]",
                    item.value, first, module_sequence
                ),
                comparison_value: format!(
                    "{}|direct-modules=[{}]",
                    item.comparison_value, module_sequence
                ),
            })
        })
        .collect()
}

fn module_symbol_order_item(
    fmt: &PdbParser<'_, '_>,
    module_id: usize,
    data: SymbolData<'_>,
) -> Option<OrderItem> {
    let (kind, name) = match data {
        SymbolData::Procedure(value) => (
            "procedure",
            function_name(fmt, module_id, &value.name, value.type_index),
        ),
        SymbolData::Data(value) => ("data", value.name.to_string().into_owned()),
        SymbolData::ThreadStorage(value) => ("thread-data", value.name.to_string().into_owned()),
        SymbolData::Constant(value) => ("constant", value.name.to_string().into_owned()),
        SymbolData::UserDefinedType(value) => ("udt", value.name.to_string().into_owned()),
        SymbolData::Thunk(value) => ("thunk", value.name.to_string().into_owned()),
        _ => return None,
    };
    Some(named_order_item(kind, name))
}

fn global_symbol_order_item(data: SymbolData<'_>) -> Option<OrderItem> {
    let name = data.name()?.to_string().into_owned();
    let kind = match &data {
        SymbolData::Public(_) => "public",
        SymbolData::ProcedureReference(_) => "procedure-ref",
        SymbolData::DataReference(_) => "data-ref",
        SymbolData::AnnotationReference(_) => "annotation-ref",
        SymbolData::TokenReference(_) => "token-ref",
        SymbolData::UserDefinedType(_) => "udt",
        SymbolData::Data(_) => "data",
        SymbolData::ThreadStorage(_) => "thread-data",
        SymbolData::Constant(_) => "constant",
        _ => return None,
    };
    Some(named_order_item(kind, name))
}

fn named_order_item(kind: &str, name: String) -> OrderItem {
    let normalized = normalize_cross_pdb_type(name.clone()).to_lowercase();
    OrderItem {
        key: format!("{kind}|{normalized}"),
        value: format!("{kind} {name}"),
        comparison_value: format!("{kind}|{normalized}"),
    }
}

fn module_order_key(module_name: &str, object_file_name: &str) -> String {
    format!(
        "{}|{}",
        module_leaf(module_name).to_lowercase(),
        module_leaf(object_file_name).to_lowercase()
    )
}

fn module_library_group(item: &OrderItem) -> Option<String> {
    let (_, container) = item.key.rsplit_once('|')?;
    container.ends_with(".lib").then(|| container.to_owned())
}

fn key_prefix_group(item: &OrderItem) -> Option<String> {
    item.key
        .split_once('|')
        .map(|(prefix, _)| prefix.to_owned())
}

fn summarize_grouped_sequences(
    name: &str,
    confidence: &'static str,
    base: &[OrderItem],
    target: &[OrderItem],
    group_for: fn(&OrderItem) -> Option<String>,
    minimum_total: usize,
) -> Vec<ScopedSequenceSummary> {
    let mut base_groups: BTreeMap<String, Vec<OrderItem>> = BTreeMap::new();
    let mut target_groups: BTreeMap<String, Vec<OrderItem>> = BTreeMap::new();
    for item in base {
        if let Some(group) = group_for(item) {
            base_groups.entry(group).or_default().push(item.clone());
        }
    }
    for item in target {
        if let Some(group) = group_for(item) {
            target_groups.entry(group).or_default().push(item.clone());
        }
    }
    summarize_pre_grouped_sequences(name, confidence, base_groups, target_groups, minimum_total)
}

fn summarize_pre_grouped_sequences(
    name: &str,
    confidence: &'static str,
    base_groups: BTreeMap<String, Vec<OrderItem>>,
    target_groups: BTreeMap<String, Vec<OrderItem>>,
    minimum_total: usize,
) -> Vec<ScopedSequenceSummary> {
    let groups: BTreeSet<String> = base_groups
        .keys()
        .chain(target_groups.keys())
        .cloned()
        .collect();
    groups
        .into_iter()
        .filter_map(|scope| {
            let base = base_groups.get(&scope).map(Vec::as_slice).unwrap_or(&[]);
            let target = target_groups.get(&scope).map(Vec::as_slice).unwrap_or(&[]);
            (base.len().max(target.len()) >= minimum_total).then(|| ScopedSequenceSummary {
                scope,
                comparison: summarize_sequence(compare_sequence(name, confidence, base, target)),
            })
        })
        .collect()
}

fn summarize_sequence(comparison: SequenceComparison) -> SequenceSummary {
    let different = sequence_differs(&comparison);
    let SequenceComparison {
        name,
        confidence,
        base_total,
        target_total,
        shared_unique,
        order_metrics,
        only_base,
        only_target,
        multiplicity,
        excluded_nonunique,
        changed,
        moved,
    } = comparison;
    SequenceSummary {
        name,
        confidence,
        different,
        base_total,
        target_total,
        shared_unique,
        order_metrics,
        only_base: only_base.len(),
        only_target: only_target.len(),
        multiplicity: multiplicity.len(),
        excluded_nonunique: excluded_nonunique.len(),
        changed: changed.len(),
        moved: moved.len(),
    }
}

fn append_scoped_symbol_kinds(
    destination: &mut BTreeMap<String, Vec<OrderItem>>,
    scope_key: &str,
    scope: &ModuleOrderScope,
) {
    for item in &scope.symbols {
        let Some((kind, _)) = item.key.split_once('|') else {
            continue;
        };
        destination
            .entry(kind.to_owned())
            .or_default()
            .push(OrderItem {
                key: format!("{scope_key}|{}", item.key),
                value: format!("module={} {}", scope.value, item.value),
                comparison_value: format!("{scope_key}|{}", item.comparison_value),
            });
    }
}

fn unique_module_scopes<'a>(
    scopes: &'a [ModuleOrderScope],
) -> (BTreeMap<String, &'a ModuleOrderScope>, BTreeSet<String>) {
    let mut positions: BTreeMap<String, Vec<&ModuleOrderScope>> = BTreeMap::new();
    for scope in scopes {
        positions.entry(scope.key.clone()).or_default().push(scope);
    }
    let mut unique = BTreeMap::new();
    let mut ambiguous = BTreeSet::new();
    for (key, values) in positions {
        if values.len() == 1 {
            unique.insert(key, values[0]);
        } else {
            ambiguous.insert(key);
        }
    }
    (unique, ambiguous)
}

fn compare_scoped_streams(
    name: &str,
    confidence: &'static str,
    base: &[ModuleOrderScope],
    target: &[ModuleOrderScope],
) -> ScopedStreamReport {
    let (base_scopes, base_ambiguous) = unique_module_scopes(base);
    let (target_scopes, target_ambiguous) = unique_module_scopes(target);
    let ambiguous_keys: BTreeSet<String> =
        base_ambiguous.into_iter().chain(target_ambiguous).collect();
    let ambiguous_scopes = ambiguous_keys
        .iter()
        .cloned()
        .map(|key| {
            let base_count = base.iter().filter(|scope| scope.key == key).count();
            let target_count = target.iter().filter(|scope| scope.key == key).count();
            let value = base
                .iter()
                .chain(target)
                .find(|scope| scope.key == key)
                .map(|scope| scope.value.clone())
                .unwrap_or_else(|| key.clone());
            MultiplicityDifference {
                key,
                value,
                base_count,
                target_count,
            }
        })
        .collect();
    let mut paired = 0usize;
    let mut streams = Vec::new();
    for (key, target_scope) in &target_scopes {
        let Some(base_scope) = base_scopes.get(key) else {
            continue;
        };
        paired += 1;
        let comparison =
            compare_sequence(name, confidence, &base_scope.symbols, &target_scope.symbols);
        if sequence_differs(&comparison) {
            streams.push(ScopedSequenceComparison {
                scope: target_scope.value.clone(),
                comparison,
            });
        }
    }
    streams.sort_by(|left, right| left.scope.cmp(&right.scope));
    let only_base_scopes = base_scopes
        .iter()
        .filter(|(key, _)| !target_scopes.contains_key(*key) && !ambiguous_keys.contains(*key))
        .map(|(_, scope)| scope.value.clone())
        .collect();
    let only_target_scopes = target_scopes
        .iter()
        .filter(|(key, _)| !base_scopes.contains_key(*key) && !ambiguous_keys.contains(*key))
        .map(|(_, scope)| scope.value.clone())
        .collect();
    ScopedStreamReport {
        paired,
        different: streams.len(),
        ambiguous_scopes,
        only_base_scopes,
        only_target_scopes,
        streams,
    }
}

fn compare_sequence(
    name: &str,
    confidence: &'static str,
    base: &[OrderItem],
    target: &[OrderItem],
) -> SequenceComparison {
    let base_positions = order_positions(base);
    let target_positions = order_positions(target);
    let keys: BTreeSet<String> = base_positions
        .keys()
        .chain(target_positions.keys())
        .map(|key| (*key).clone())
        .collect();
    let mut only_base = Vec::new();
    let mut only_target = Vec::new();
    let mut multiplicity = Vec::new();
    let mut excluded_nonunique = Vec::new();
    let mut changed = Vec::new();
    let mut target_unique = HashMap::new();

    for key in keys {
        let base_values = base_positions.get(&key).map(Vec::as_slice).unwrap_or(&[]);
        let target_values = target_positions.get(&key).map(Vec::as_slice).unwrap_or(&[]);
        if target_values.is_empty() {
            only_base.extend(
                base_values
                    .iter()
                    .map(|(position, item)| PositionedOrderItem {
                        key: key.clone(),
                        value: item.value.clone(),
                        position: *position,
                    }),
            );
        } else if base_values.is_empty() {
            only_target.extend(
                target_values
                    .iter()
                    .map(|(position, item)| PositionedOrderItem {
                        key: key.clone(),
                        value: item.value.clone(),
                        position: *position,
                    }),
            );
        } else if base_values.len() != 1 || target_values.len() != 1 {
            let item = MultiplicityDifference {
                key: key.clone(),
                value: base_values
                    .first()
                    .or_else(|| target_values.first())
                    .map(|(_, item)| item.value.clone())
                    .unwrap_or_else(|| key.clone()),
                base_count: base_values.len(),
                target_count: target_values.len(),
            };
            if base_values.len() == target_values.len() {
                excluded_nonunique.push(item);
            } else {
                multiplicity.push(item);
            }
        } else {
            if base_values[0].1.comparison_value != target_values[0].1.comparison_value {
                changed.push(ChangedOrderItem {
                    key: key.clone(),
                    base_value: base_values[0].1.value.clone(),
                    target_value: target_values[0].1.value.clone(),
                    base_position: base_values[0].0,
                    target_position: target_values[0].0,
                });
            }
            target_unique.insert(key, target_values[0].0);
        }
    }

    let common: Vec<(&OrderItem, usize, usize)> = base
        .iter()
        .enumerate()
        .filter_map(|(base_position, item)| {
            target_unique
                .get(&item.key)
                .copied()
                .map(|target_position| (item, base_position, target_position))
        })
        .collect();
    let order_metrics = order_metrics(&common);
    let moved = inversion_participants(&common);

    SequenceComparison {
        name: name.to_owned(),
        confidence,
        base_total: base.len(),
        target_total: target.len(),
        shared_unique: common.len(),
        order_metrics,
        only_base,
        only_target,
        multiplicity,
        excluded_nonunique,
        changed,
        moved,
    }
}

fn order_metrics(common: &[(&OrderItem, usize, usize)]) -> OrderMetrics {
    let mut target_positions: Vec<usize> = common
        .iter()
        .map(|(_, _, target_position)| *target_position)
        .collect();
    target_positions.sort_unstable();
    let target_ranks: HashMap<usize, usize> = target_positions
        .into_iter()
        .enumerate()
        .map(|(rank, position)| (position, rank))
        .collect();
    let ranks: Vec<usize> = common
        .iter()
        .map(|(_, _, target_position)| target_ranks[target_position])
        .collect();
    let comparable_pairs =
        (ranks.len() as u64).saturating_mul(ranks.len().saturating_sub(1) as u64) / 2;
    let preserved_adjacent_pairs = ranks
        .windows(2)
        .filter(|pair| pair[1] == pair[0] + 1)
        .count();
    let reversed_adjacent_pairs = ranks
        .windows(2)
        .filter(|pair| pair[0] == pair[1] + 1)
        .count();
    let increasing_runs = if ranks.is_empty() {
        0
    } else {
        1 + ranks.windows(2).filter(|pair| pair[1] < pair[0]).count()
    };
    let longest_contiguous_run = if ranks.is_empty() {
        0
    } else {
        let mut longest = 1;
        let mut current = 1;
        for pair in ranks.windows(2) {
            if pair[1] == pair[0] + 1 {
                current += 1;
                longest = longest.max(current);
            } else {
                current = 1;
            }
        }
        longest
    };
    let mut tails = Vec::new();
    for &rank in &ranks {
        let position = tails.partition_point(|&tail| tail < rank);
        if position == tails.len() {
            tails.push(rank);
        } else {
            tails[position] = rank;
        }
    }
    let rank_displacements: Vec<usize> = ranks
        .iter()
        .enumerate()
        .map(|(base_rank, target_rank)| base_rank.abs_diff(*target_rank))
        .collect();

    OrderMetrics {
        comparable_pairs,
        inversions: inversion_count(&ranks),
        longest_ordered_subsequence: tails.len(),
        preserved_adjacent_pairs,
        reversed_adjacent_pairs,
        longest_contiguous_run,
        increasing_runs,
        rank_displacement_sum: rank_displacements.iter().map(|value| *value as u64).sum(),
        max_rank_displacement: rank_displacements.into_iter().max().unwrap_or(0),
    }
}

fn inversion_count(ranks: &[usize]) -> u64 {
    let mut tree = vec![0_u64; ranks.len() + 1];
    let mut inversions = 0_u64;
    for (seen, &rank) in ranks.iter().enumerate() {
        let mut index = rank + 1;
        let mut less_or_equal = 0_u64;
        while index > 0 {
            less_or_equal += tree[index];
            index &= index - 1;
        }
        inversions += seen as u64 - less_or_equal;

        let mut index = rank + 1;
        while index < tree.len() {
            tree[index] += 1;
            index += index & (!index + 1);
        }
    }
    inversions
}

fn order_positions<'a>(
    items: &'a [OrderItem],
) -> BTreeMap<&'a String, Vec<(usize, &'a OrderItem)>> {
    let mut positions: BTreeMap<&String, Vec<(usize, &OrderItem)>> = BTreeMap::new();
    for (position, item) in items.iter().enumerate() {
        positions
            .entry(&item.key)
            .or_default()
            .push((position, item));
    }
    positions
}

fn inversion_participants(common: &[(&OrderItem, usize, usize)]) -> Vec<MovedOrderItem> {
    if common.len() < 2 {
        return Vec::new();
    }
    let mut prefix_max = vec![0usize; common.len()];
    let mut suffix_min = vec![usize::MAX; common.len()];
    for index in 0..common.len() {
        prefix_max[index] = common[index].2;
        if index > 0 {
            prefix_max[index] = prefix_max[index].max(prefix_max[index - 1]);
        }
    }
    for index in (0..common.len()).rev() {
        suffix_min[index] = common[index].2;
        if index + 1 < common.len() {
            suffix_min[index] = suffix_min[index].min(suffix_min[index + 1]);
        }
    }
    common
        .iter()
        .enumerate()
        .filter(|(index, (_, _, target_position))| {
            (*index > 0 && prefix_max[*index - 1] > *target_position)
                || (*index + 1 < common.len() && suffix_min[*index + 1] < *target_position)
        })
        .map(
            |(_, (item, base_position, target_position))| MovedOrderItem {
                key: item.key.clone(),
                value: item.value.clone(),
                base_position: *base_position,
                target_position: *target_position,
            },
        )
        .collect()
}

fn sequence_differs(comparison: &SequenceComparison) -> bool {
    !comparison.only_base.is_empty()
        || !comparison.only_target.is_empty()
        || !comparison.multiplicity.is_empty()
        || !comparison.changed.is_empty()
        || !comparison.moved.is_empty()
}

fn render_focused_type_order_report(report: &FocusedTypeOrderReport, limit: usize) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "target PDB: {}", report.target_pdb);
    let _ = writeln!(out, "base PDB:   {}", report.base_pdb);
    let _ = writeln!(out, "named-type filters: {:?}", report.filters);
    let _ = writeln!(
        out,
        "focused mode filters named-type identities only; direct modules are contributor clues, not ownership proof"
    );
    let _ = writeln!(out, "\n[target named TPI sequence]");
    for (position, item) in report.target_sequence.iter().enumerate() {
        let _ = writeln!(out, "  {position:>5}: {}", item.value);
    }
    let _ = writeln!(out, "\n[base named TPI sequence]");
    for (position, item) in report.base_sequence.iter().enumerate() {
        let _ = writeln!(out, "  {position:>5}: {}", item.value);
    }
    render_sequence_comparison(&mut out, &report.named_types, limit);
    render_sequence_comparison(&mut out, &report.named_type_module_references, limit);
    out
}

fn render_focused_module_order_report(report: &FocusedModuleOrderReport, limit: usize) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "target PDB: {}", report.target_pdb);
    let _ = writeln!(out, "base PDB:   {}", report.base_pdb);
    let _ = writeln!(out, "DBI module filter: {:?}", report.filter);
    let _ = writeln!(
        out,
        "focused mode reads serialized DBI modules and their local file/C13 streams; it does not imply source ownership"
    );
    let _ = writeln!(out, "\n[target DBI module sequence]");
    for (position, item) in report.target_sequence.iter().enumerate() {
        let _ = writeln!(out, "  {position:>5}: {}", item.value);
    }
    let _ = writeln!(out, "\n[base DBI module sequence]");
    for (position, item) in report.base_sequence.iter().enumerate() {
        let _ = writeln!(out, "  {position:>5}: {}", item.value);
    }
    render_sequence_comparison(&mut out, &report.modules, limit);
    render_scoped_stream_report(
        &mut out,
        "focused DBI per-module source-file scopes",
        &report.dbi_source_file_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "focused C13 file-checksum scopes",
        &report.module_file_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "focused C13 line-program scopes",
        &report.module_line_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "focused C13 subsection scopes",
        &report.module_subsection_streams,
        limit,
    );
    out
}

fn render_order_report(report: &OrderReport, limit: usize) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "target PDB: {}", report.target_pdb);
    let _ = writeln!(out, "base PDB:   {}", report.base_pdb);
    let _ = writeln!(
        out,
        "order evidence is reported by channel; physical/container/linker order is diagnostic, not source-order proof"
    );
    let _ = writeln!(out, "\n[coverage]");
    for row in &report.coverage {
        let _ = writeln!(out, "  {} — {}: {}", row.channel, row.status, row.note);
    }
    render_msf_layout(&mut out, &report.msf_layout, limit);
    render_sequence_comparison(&mut out, &report.msf_free_pages, limit);
    render_sequence_comparison(&mut out, &report.stream_roles, limit);
    render_sequence_comparison(&mut out, &report.dbi_substreams, limit);
    render_scoped_stream_report(
        &mut out,
        "DBI per-module source-file scopes",
        &report.dbi_source_file_streams,
        limit,
    );
    render_sequence_comparison(&mut out, &report.dbi_ec_names, limit);
    render_sequence_comparison(&mut out, &report.dbi_ec_metadata, limit);
    render_sequence_comparison(&mut out, &report.dbi_ec_hash_buckets, limit);
    render_sequence_comparison(&mut out, &report.named_streams, limit);
    render_sequence_comparison(&mut out, &report.named_stream_buckets, limit);
    render_sequence_comparison(&mut out, &report.deleted_named_stream_buckets, limit);
    render_sequence_comparison(&mut out, &report.pdb_features, limit);
    render_sequence_comparison(&mut out, &report.string_table_names, limit);
    render_sequence_comparison(&mut out, &report.string_table_metadata, limit);
    render_sequence_comparison(&mut out, &report.string_table_hash_buckets, limit);
    render_sequence_comparison(&mut out, &report.modules, limit);
    render_scoped_summaries(
        &mut out,
        "DBI order within individual libraries",
        &report.module_library_sequences,
        limit,
    );
    render_sequence_comparison(&mut out, &report.section_contributions, limit);
    render_sequence_comparison(&mut out, &report.section_map, limit);
    render_sequence_comparison(&mut out, &report.section_headers, limit);
    render_sequence_comparison(&mut out, &report.legacy_fpo_records, limit);
    render_sequence_comparison(&mut out, &report.frame_data_records, limit);
    render_sequence_comparison(&mut out, &report.raw_type_records, limit);
    render_sequence_comparison(&mut out, &report.tpi_metadata, limit);
    render_sequence_comparison(&mut out, &report.tpi_hash_values, limit);
    render_sequence_comparison(&mut out, &report.tpi_index_offsets, limit);
    render_sequence_comparison(&mut out, &report.tpi_hash_adjustments, limit);
    render_sequence_comparison(&mut out, &report.named_types, limit);
    render_sequence_comparison(&mut out, &report.named_type_module_references, limit);
    render_scoped_summaries(
        &mut out,
        "TPI order by named-record kind",
        &report.named_type_kinds,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "complete enum value-order scopes",
        &report.enum_value_streams,
        limit,
    );
    render_sequence_comparison(&mut out, &report.raw_id_records, limit);
    render_sequence_comparison(&mut out, &report.ipi_metadata, limit);
    render_sequence_comparison(&mut out, &report.ipi_hash_values, limit);
    render_sequence_comparison(&mut out, &report.ipi_index_offsets, limit);
    render_sequence_comparison(&mut out, &report.ipi_hash_adjustments, limit);
    render_sequence_comparison(&mut out, &report.raw_global_symbols, limit);
    render_sequence_comparison(&mut out, &report.global_symbols, limit);
    render_sequence_comparison(&mut out, &report.global_hash_records, limit);
    render_sequence_comparison(&mut out, &report.global_hash_buckets, limit);
    render_sequence_comparison(&mut out, &report.public_hash_records, limit);
    render_sequence_comparison(&mut out, &report.public_hash_buckets, limit);
    render_sequence_comparison(&mut out, &report.public_address_map, limit);
    render_sequence_comparison(&mut out, &report.public_thunk_map, limit);
    render_sequence_comparison(&mut out, &report.public_section_map, limit);
    render_scoped_summaries(
        &mut out,
        "global-symbol order by kind",
        &report.global_symbol_kinds,
        limit,
    );
    render_scoped_summaries(
        &mut out,
        "module-local symbol order by kind (stable module-key order)",
        &report.module_symbol_kinds,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "all module symbol-record scopes",
        &report.module_record_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module file-checksum scopes",
        &report.module_file_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module line-program scopes",
        &report.module_line_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module C13-subsection scopes",
        &report.module_subsection_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module C13 string-table scopes",
        &report.module_string_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module C13 frame-data scopes",
        &report.module_frame_data_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module C13 inlinee scopes",
        &report.module_inlinee_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module C13 cross-import scopes",
        &report.module_cross_import_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module C13 cross-export scopes",
        &report.module_cross_export_streams,
        limit,
    );
    render_scoped_stream_report(
        &mut out,
        "module other C13 payload scopes",
        &report.module_other_c13_streams,
        limit,
    );
    let _ = writeln!(
        out,
        "\n[module symbol scopes] paired={} different={} ambiguous={}",
        report.paired_module_symbol_streams,
        report.different_module_symbol_streams,
        report.ambiguous_module_scopes.len(),
    );
    for scope in report.module_symbols.iter().take(limit) {
        let _ = writeln!(out, "\n=== {} ===", scope.scope);
        render_sequence_comparison(&mut out, &scope.comparison, limit);
    }
    if report.module_symbols.len() > limit {
        let _ = writeln!(
            out,
            "  ... {} more differing module scopes (use --json for the uncapped report)",
            report.module_symbols.len() - limit
        );
    }
    out
}

fn render_msf_layout(out: &mut String, comparison: &MsfLayoutComparison, limit: usize) {
    let base = &comparison.base;
    let target = &comparison.target;
    let _ = writeln!(
        out,
        "\n[MSF container layout — {}]\n  base:   {} page=0x{:x} fpm={} ({} map pages, {} free) pages={} bytes={} directory={} ({} pages/{} runs via {} map pages) streams={}/{} present, bytes={}, stream-pages={}, runs={}, fragmented={}\n  target: {} page=0x{:x} fpm={} ({} map pages, {} free) pages={} bytes={} directory={} ({} pages/{} runs via {} map pages) streams={}/{} present, bytes={}, stream-pages={}, runs={}, fragmented={}\n  identified roles={} unidentified base={} target={}",
        comparison.confidence,
        base.format,
        base.page_size,
        base.free_page_map,
        base.free_page_map_pages.len(),
        base.free_pages,
        base.pages_used,
        base.file_bytes,
        base.directory_size,
        base.directory_pages.len(),
        base.directory_page_runs,
        base.directory_map_pages.len(),
        base.present_streams,
        base.stream_slots,
        base.stream_bytes,
        base.stream_pages,
        base.stream_page_runs,
        base.fragmented_streams,
        target.format,
        target.page_size,
        target.free_page_map,
        target.free_page_map_pages.len(),
        target.free_pages,
        target.pages_used,
        target.file_bytes,
        target.directory_size,
        target.directory_pages.len(),
        target.directory_page_runs,
        target.directory_map_pages.len(),
        target.present_streams,
        target.stream_slots,
        target.stream_bytes,
        target.stream_pages,
        target.stream_page_runs,
        target.fragmented_streams,
        comparison.stable_roles.len(),
        comparison.unidentified_base.len(),
        comparison.unidentified_target.len(),
    );
    let different =
        comparison
            .stable_roles
            .iter()
            .filter(|role| match (&role.base, &role.target) {
                (Some(base), Some(target)) => {
                    base.stream_index != target.stream_index
                        || base.size != target.size
                        || base.pages != target.pages
                }
                (None, None) => false,
                _ => true,
            });
    for role in different.take(limit) {
        let render = |value: &Option<StreamLayoutObservation>| {
            value.as_ref().map_or_else(
                || "absent".to_owned(),
                |item| {
                    format!(
                        "stream={} size={:?} pages={} runs={} first={:?} last={:?}",
                        item.stream_index,
                        item.size,
                        item.page_count,
                        item.page_runs,
                        item.pages.first(),
                        item.pages.last(),
                    )
                },
            )
        };
        let _ = writeln!(
            out,
            "  role {}: base {} | target {}",
            role.value,
            render(&role.base),
            render(&role.target),
        );
    }
}

fn render_scoped_stream_report(
    out: &mut String,
    title: &str,
    report: &ScopedStreamReport,
    limit: usize,
) {
    let _ = writeln!(
        out,
        "\n[{title}] paired={} different={} ambiguous={} only-base={} only-target={}",
        report.paired,
        report.different,
        report.ambiguous_scopes.len(),
        report.only_base_scopes.len(),
        report.only_target_scopes.len(),
    );
    for scope in report.streams.iter().take(limit) {
        let _ = writeln!(out, "\n=== {} ===", scope.scope);
        render_sequence_comparison(out, &scope.comparison, limit);
    }
    if report.streams.len() > limit {
        let _ = writeln!(
            out,
            "  ... {} more differing scopes (use --json for the uncapped report)",
            report.streams.len() - limit
        );
    }
}

fn render_sequence_comparison(out: &mut String, comparison: &SequenceComparison, limit: usize) {
    let _ = writeln!(
        out,
        "\n[{} — {}] base={} target={} shared-unique={} moved={} changed={} only-base={} only-target={} multiplicity={} excluded-nonunique={}",
        comparison.name,
        comparison.confidence,
        comparison.base_total,
        comparison.target_total,
        comparison.shared_unique,
        comparison.moved.len(),
        comparison.changed.len(),
        comparison.only_base.len(),
        comparison.only_target.len(),
        comparison.multiplicity.len(),
        comparison.excluded_nonunique.len(),
    );
    render_order_metrics(out, &comparison.order_metrics, comparison.shared_unique);
    for item in comparison.moved.iter().take(limit) {
        let _ = writeln!(
            out,
            "  moved base#{} -> target#{}: {}",
            item.base_position, item.target_position, item.value
        );
    }
    for item in comparison.changed.iter().take(limit) {
        let _ = writeln!(
            out,
            "  changed base#{} -> target#{}:\n    - {}\n    + {}",
            item.base_position, item.target_position, item.base_value, item.target_value
        );
    }
    for item in comparison.only_base.iter().take(limit) {
        let _ = writeln!(out, "  only-base #{}: {}", item.position, item.value);
    }
    for item in comparison.only_target.iter().take(limit) {
        let _ = writeln!(out, "  only-target #{}: {}", item.position, item.value);
    }
    for item in comparison.multiplicity.iter().take(limit) {
        let _ = writeln!(
            out,
            "  multiplicity base={} target={}: {}",
            item.base_count, item.target_count, item.value
        );
    }
    for item in comparison.excluded_nonunique.iter().take(limit) {
        let _ = writeln!(
            out,
            "  excluded from order pairing (non-unique key, base={} target={}): {}",
            item.base_count, item.target_count, item.value
        );
    }
}

fn render_order_metrics(out: &mut String, metrics: &OrderMetrics, shared_unique: usize) {
    let adjacent_pairs = shared_unique.saturating_sub(1);
    let inversion_percent = if metrics.comparable_pairs == 0 {
        0.0
    } else {
        100.0 * metrics.inversions as f64 / metrics.comparable_pairs as f64
    };
    let _ = writeln!(
        out,
        "  locality: inversions={}/{} ({:.4}%) lis={}/{} adjacent={}/{} reversed-adjacent={} longest-contiguous={} increasing-runs={} displacement-sum={} displacement-max={}",
        metrics.inversions,
        metrics.comparable_pairs,
        inversion_percent,
        metrics.longest_ordered_subsequence,
        shared_unique,
        metrics.preserved_adjacent_pairs,
        adjacent_pairs,
        metrics.reversed_adjacent_pairs,
        metrics.longest_contiguous_run,
        metrics.increasing_runs,
        metrics.rank_displacement_sum,
        metrics.max_rank_displacement,
    );
}

fn render_scoped_summaries(
    out: &mut String,
    title: &str,
    summaries: &[ScopedSequenceSummary],
    limit: usize,
) {
    let mut different: Vec<&ScopedSequenceSummary> = summaries
        .iter()
        .filter(|summary| summary.comparison.different)
        .collect();
    different.sort_by(|left, right| {
        let ratio = |summary: &ScopedSequenceSummary| {
            let metrics = &summary.comparison.order_metrics;
            if metrics.comparable_pairs == 0 {
                0.0
            } else {
                metrics.inversions as f64 / metrics.comparable_pairs as f64
            }
        };
        ratio(right)
            .partial_cmp(&ratio(left))
            .unwrap_or(std::cmp::Ordering::Equal)
            .then_with(|| {
                right
                    .comparison
                    .order_metrics
                    .inversions
                    .cmp(&left.comparison.order_metrics.inversions)
            })
            .then_with(|| left.scope.cmp(&right.scope))
    });
    let _ = writeln!(
        out,
        "\n[{title}] groups={} different={}",
        summaries.len(),
        different.len(),
    );
    for summary in different.iter().take(limit) {
        let comparison = &summary.comparison;
        let _ = writeln!(
            out,
            "  {}: base={} target={} shared-unique={} moved={} changed={} only-base={} only-target={} multiplicity={} excluded-nonunique={}",
            summary.scope,
            comparison.base_total,
            comparison.target_total,
            comparison.shared_unique,
            comparison.moved,
            comparison.changed,
            comparison.only_base,
            comparison.only_target,
            comparison.multiplicity,
            comparison.excluded_nonunique,
        );
        render_order_metrics(out, &comparison.order_metrics, comparison.shared_unique);
    }
    if different.len() > limit {
        let _ = writeln!(
            out,
            "  ... {} more differing groups",
            different.len() - limit
        );
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

#[derive(Clone, Debug, serde::Serialize)]
struct ClassModel {
    name: String,
    kind: &'static str,
    size: u64,
    properties: String,
    entries: Vec<ClassEntry>,
    type_indices: Vec<u32>,
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
    record_multiplicity_classes: usize,
    variant_overlap_classes: usize,
    different_classes: usize,
    missing_base_classes: usize,
    base_only_classes: usize,
    base_only_class_names: Vec<String>,
    target_complete_records: usize,
    base_complete_records: usize,
    target_semantic_variants: usize,
    base_semantic_variants: usize,
    target_names_with_multiple_variants: usize,
    base_names_with_multiple_variants: usize,
    target_names_with_duplicate_records: usize,
    base_names_with_duplicate_records: usize,
    target_unresolved_types: usize,
    base_unresolved_types: usize,
    difference_counts: BTreeMap<String, usize>,
    classes: Vec<ClassComparison>,
}

#[derive(serde::Serialize)]
struct ClassComparison {
    name: String,
    status: &'static str,
    target_variants: Vec<ClassVariantSummary>,
    base_variants: Vec<ClassVariantSummary>,
    diagnostic_pair: Option<DiagnosticVariantPair>,
    differences: Vec<ClassDifference>,
}

#[derive(serde::Serialize)]
struct ClassVariantSummary {
    type_indices: Vec<u32>,
    summary: String,
    kind: &'static str,
    size: u64,
    properties: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    declarations: Option<Vec<ClassEntry>>,
    differences_from_first: BTreeMap<String, usize>,
}

#[derive(serde::Serialize)]
struct DiagnosticVariantPair {
    base_type_indices: Vec<u32>,
    target_type_indices: Vec<u32>,
    reason: &'static str,
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
) -> crate::Result<ClassReport> {
    let filter = cli
        .class_filter
        .as_ref()
        .map(|value| normalize_cross_pdb_type(value.to_lowercase()));
    let target = load_classes(target_pdb, filter.as_deref())?;
    let base = load_classes(base_pdb, filter.as_deref())?;

    let target_classes = target.classes.len();
    let target_complete_records = complete_record_count(&target);
    let base_complete_records = complete_record_count(&base);
    let target_semantic_variants = semantic_variant_count(&target);
    let base_semantic_variants = semantic_variant_count(&base);
    let target_names_with_multiple_variants = target
        .classes
        .values()
        .filter(|variants| variants.len() > 1)
        .count();
    let base_names_with_multiple_variants = base
        .classes
        .values()
        .filter(|variants| variants.len() > 1)
        .count();
    let target_names_with_duplicate_records = names_with_duplicate_records(&target);
    let base_names_with_duplicate_records = names_with_duplicate_records(&base);
    let target_unresolved_types = unresolved_type_count(&target);
    let base_unresolved_types = unresolved_type_count(&base);
    let base_only_class_names: Vec<String> = base
        .classes
        .iter()
        .filter(|(name, _)| !target.classes.contains_key(*name))
        .map(|(_, variants)| display_class_name(variants))
        .collect();
    let base_only_classes = base_only_class_names.len();
    let mut compared_classes = 0usize;
    let mut identical_classes = 0usize;
    let mut record_multiplicity_classes = 0usize;
    let mut variant_overlap_classes = 0usize;
    let mut different_classes = 0usize;
    let mut missing_base_classes = 0usize;
    let mut difference_counts = BTreeMap::new();
    let mut classes = Vec::with_capacity(target_classes);

    for (key, target_variants) in &target.classes {
        let name = display_class_name(target_variants);
        let Some(base_variants) = base.classes.get(key) else {
            missing_base_classes += 1;
            *difference_counts
                .entry("class-presence".to_owned())
                .or_insert(0) += 1;
            classes.push(ClassComparison {
                name,
                status: "missing-base",
                target_variants: variant_summaries(target_variants),
                base_variants: Vec::new(),
                diagnostic_pair: None,
                differences: vec![ClassDifference {
                    category: "class-presence",
                    member: None,
                    base: None,
                    target: Some(variant_set_summary(target_variants)),
                }],
            });
            continue;
        };

        compared_classes += 1;
        let matches = matching_variants(base_variants, target_variants);
        let exact_variant_sets =
            matches.len() == base_variants.len() && matches.len() == target_variants.len();
        let multiplicity_differences =
            record_multiplicity_differences(base_variants, target_variants, &matches);
        let (status, diagnostic_pair, differences) = if exact_variant_sets
            && multiplicity_differences.is_empty()
        {
            identical_classes += 1;
            ("identical", None, Vec::new())
        } else if exact_variant_sets {
            record_multiplicity_classes += 1;
            ("record-multiplicity", None, multiplicity_differences)
        } else if !matches.is_empty() {
            variant_overlap_classes += 1;
            (
                "variant-overlap",
                None,
                variant_set_differences(base_variants, target_variants, &matches),
            )
        } else {
            different_classes += 1;
            let (base_class, target_class) = closest_variant_pair(base_variants, target_variants);
            (
                "different",
                Some(DiagnosticVariantPair {
                    base_type_indices: base_class.type_indices.clone(),
                    target_type_indices: target_class.type_indices.clone(),
                    reason: "closest disjoint semantic variants; diagnostic detail only",
                }),
                compare_class(base_class, target_class),
            )
        };
        for difference in &differences {
            *difference_counts
                .entry(difference.category.to_owned())
                .or_insert(0) += 1;
        }
        classes.push(ClassComparison {
            name,
            status,
            target_variants: variant_summaries(target_variants),
            base_variants: variant_summaries(base_variants),
            diagnostic_pair,
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
        record_multiplicity_classes,
        variant_overlap_classes,
        different_classes,
        missing_base_classes,
        base_only_classes,
        base_only_class_names,
        target_complete_records,
        base_complete_records,
        target_semantic_variants,
        base_semantic_variants,
        target_names_with_multiple_variants,
        base_names_with_multiple_variants,
        target_names_with_duplicate_records,
        base_names_with_duplicate_records,
        target_unresolved_types,
        base_unresolved_types,
        difference_counts,
        classes,
    })
}

fn load_classes(pdb_path: &PathBuf, filter: Option<&str>) -> crate::Result<ClassSide> {
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
                type_indices: vec![ty.index().0],
            };
            if let Some(fields) = class.fields {
                walk_class_fields(&finder, &fmt, fields, &mut model, &mut HashSet::new())?;
            }
            let variants = side.classes.entry(key).or_default();
            if let Some(existing) = variants
                .iter_mut()
                .find(|existing| same_class_shape(existing, &model))
            {
                existing.type_indices.push(ty.index().0);
            } else {
                variants.push(model);
            }
        }
        Ok(())
    })?;
    Ok(side)
}

/// Extract every complete class variant as normalized declaration facts. Equal
/// semantic variants keep their complete `type_indices` list, so duplicate
/// CodeView records remain attributable instead of being discarded.
pub fn extract_class_facts(
    pdb_path: &std::path::Path,
) -> crate::Result<Vec<crate::evidence::SourceFact>> {
    let side = load_classes(&pdb_path.to_path_buf(), None)?;
    let mut facts = Vec::new();
    for variants in side.classes.values() {
        for variant in variants {
            let consumer = format!("type_indices={:?}", variant.type_indices);
            facts.push(crate::evidence::SourceFact {
                category: "type".into(),
                subject: variant.name.clone(),
                consumer: consumer.clone(),
                value: serde_json::json!({
                    "kind": variant.kind,
                    "size": variant.size,
                    "order": 0,
                }),
            });
            for (order, entry) in variant
                .entries
                .iter()
                .filter(|entry| entry.kind != "vftable-pointer")
                .enumerate()
            {
                let category = match entry.kind {
                    "field" | "static-field" => "field",
                    "method" => "function",
                    "base" | "virtual-base" => "base",
                    _ => "declaration",
                };
                facts.push(crate::evidence::SourceFact {
                    category: category.into(),
                    subject: format!("{}::{}", variant.name, entry.name),
                    consumer: consumer.clone(),
                    value: serde_json::json!({
                        "kind": entry.kind,
                        "access": entry.access,
                        "static": entry.kind == "static-field" || entry.attributes.iter().any(|a| a == "static"),
                        "virtual": entry.attributes.iter().any(|a| a == "virtual" || a == "introducing-virtual"),
                        "pure_virtual": entry.attributes.iter().any(|a| a == "pure-virtual"),
                        "introducing_virtual": entry.attributes.iter().any(|a| a == "introducing-virtual"),
                        "const": entry.kind == "method" && entry.type_name.trim_end().ends_with(" const"),
                        "type": entry.type_name,
                        "order": order,
                        "offset": entry.offset,
                        "vtable_offset": entry.vtable_offset,
                    }),
                });
            }
        }
    }
    Ok(facts)
}

fn walk_class_fields(
    finder: &pdb::TypeFinder<'_>,
    fmt: &PdbParser<'_, '_>,
    field_index: pdb::TypeIndex,
    class: &mut ClassModel,
    seen: &mut HashSet<u32>,
) -> crate::Result<()> {
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
) -> crate::Result<()> {
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
            details: nested_type_details(finder, nested.nested_type)?,
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

fn nested_type_details(
    finder: &pdb::TypeFinder<'_>,
    type_index: pdb::TypeIndex,
) -> crate::Result<Vec<String>> {
    let Ok(record) = finder.find(type_index) else {
        return Ok(Vec::new());
    };
    let Ok(pdb::TypeData::Enumeration(enumeration)) = record.parse() else {
        return Ok(Vec::new());
    };

    let mut details = Vec::new();
    collect_enumerators(
        finder,
        enumeration.fields,
        &mut details,
        &mut HashSet::new(),
    )?;
    Ok(details)
}

fn collect_enumerators(
    finder: &pdb::TypeFinder<'_>,
    field_index: pdb::TypeIndex,
    details: &mut Vec<String>,
    seen: &mut HashSet<u32>,
) -> crate::Result<()> {
    if !seen.insert(field_index.0) {
        return Ok(());
    }
    let Ok(record) = finder.find(field_index) else {
        return Ok(());
    };
    let Ok(pdb::TypeData::FieldList(list)) = record.parse() else {
        return Ok(());
    };
    for field in list.fields {
        if let pdb::TypeData::Enumerate(enumerator) = field {
            details.push(format!("{}={:?}", enumerator.name, enumerator.value));
        }
    }
    if let Some(continuation) = list.continuation {
        collect_enumerators(finder, continuation, details, seen)?;
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
) -> crate::Result<ClassEntry> {
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
        .flatten()
        .flat_map(|class| &class.entries)
        .filter(|entry| entry.type_name.starts_with("<unresolved-"))
        .count()
}

fn complete_record_count(side: &ClassSide) -> usize {
    side.classes
        .values()
        .flatten()
        .map(|variant| variant.type_indices.len())
        .sum()
}

fn semantic_variant_count(side: &ClassSide) -> usize {
    side.classes.values().map(Vec::len).sum()
}

fn names_with_duplicate_records(side: &ClassSide) -> usize {
    side.classes
        .values()
        .filter(|variants| {
            variants
                .iter()
                .any(|variant| variant.type_indices.len() > 1)
        })
        .count()
}

fn same_class_shape(left: &ClassModel, right: &ClassModel) -> bool {
    left.kind == right.kind
        && left.size == right.size
        && left.properties == right.properties
        && left.entries == right.entries
}

fn matching_variants(base: &[ClassModel], target: &[ClassModel]) -> Vec<(usize, usize)> {
    target
        .iter()
        .enumerate()
        .filter_map(|(target_index, target_variant)| {
            base.iter()
                .position(|base_variant| same_class_shape(base_variant, target_variant))
                .map(|base_index| (base_index, target_index))
        })
        .collect()
}

fn closest_variant_pair<'a>(
    base: &'a [ClassModel],
    target: &'a [ClassModel],
) -> (&'a ClassModel, &'a ClassModel) {
    base.iter()
        .flat_map(|base_variant| {
            target
                .iter()
                .map(move |target_variant| (base_variant, target_variant))
        })
        .min_by_key(|(base_variant, target_variant)| {
            (
                compare_class(base_variant, target_variant).len(),
                base_variant
                    .type_indices
                    .first()
                    .copied()
                    .unwrap_or(u32::MAX),
                target_variant
                    .type_indices
                    .first()
                    .copied()
                    .unwrap_or(u32::MAX),
            )
        })
        .expect("complete class variant lists cannot be empty")
}

fn record_multiplicity_differences(
    base: &[ClassModel],
    target: &[ClassModel],
    matches: &[(usize, usize)],
) -> Vec<ClassDifference> {
    matches
        .iter()
        .filter_map(|(base_index, target_index)| {
            let base_variant = &base[*base_index];
            let target_variant = &target[*target_index];
            (base_variant.type_indices.len() != target_variant.type_indices.len()).then(|| {
                ClassDifference {
                    category: "record-multiplicity",
                    member: Some(class_summary(target_variant)),
                    base: Some(type_index_summary(&base_variant.type_indices)),
                    target: Some(type_index_summary(&target_variant.type_indices)),
                }
            })
        })
        .collect()
}

fn variant_set_differences(
    base: &[ClassModel],
    target: &[ClassModel],
    matches: &[(usize, usize)],
) -> Vec<ClassDifference> {
    let matched_base: HashSet<usize> = matches.iter().map(|(base, _)| *base).collect();
    let matched_target: HashSet<usize> = matches.iter().map(|(_, target)| *target).collect();
    let mut differences = record_multiplicity_differences(base, target, matches);
    differences.extend(
        base.iter()
            .enumerate()
            .filter(|(index, _)| !matched_base.contains(index))
            .map(|(_, variant)| ClassDifference {
                category: "variant-set",
                member: None,
                base: Some(variant_summary(variant)),
                target: None,
            }),
    );
    differences.extend(
        target
            .iter()
            .enumerate()
            .filter(|(index, _)| !matched_target.contains(index))
            .map(|(_, variant)| ClassDifference {
                category: "variant-set",
                member: None,
                base: None,
                target: Some(variant_summary(variant)),
            }),
    );
    differences
}

fn display_class_name(variants: &[ClassModel]) -> String {
    variants
        .first()
        .map(|variant| variant.name.clone())
        .unwrap_or_else(|| "<missing-class-name>".to_owned())
}

fn variant_summaries(variants: &[ClassModel]) -> Vec<ClassVariantSummary> {
    let first = variants.first();
    let include_declarations = variants.len() > 1
        || variants
            .iter()
            .any(|variant| variant.type_indices.len() > 1);
    variants
        .iter()
        .map(|variant| ClassVariantSummary {
            type_indices: variant.type_indices.clone(),
            summary: class_summary(variant),
            kind: variant.kind,
            size: variant.size,
            properties: variant.properties.clone(),
            declarations: include_declarations.then(|| variant.entries.clone()),
            differences_from_first: first
                .map(|first| difference_category_counts(&compare_class(first, variant)))
                .unwrap_or_default(),
        })
        .collect()
}

fn difference_category_counts(differences: &[ClassDifference]) -> BTreeMap<String, usize> {
    let mut counts = BTreeMap::new();
    for difference in differences {
        *counts.entry(difference.category.to_owned()).or_insert(0) += 1;
    }
    counts
}

fn variant_set_summary(variants: &[ClassModel]) -> String {
    variants
        .iter()
        .map(variant_summary)
        .collect::<Vec<_>>()
        .join("; ")
}

fn variant_summary(variant: &ClassModel) -> String {
    format!(
        "{} records={}",
        class_summary(variant),
        type_index_summary(&variant.type_indices)
    )
}

fn type_index_summary(indices: &[u32]) -> String {
    indices
        .iter()
        .map(|index| format!("0x{index:x}"))
        .collect::<Vec<_>>()
        .join(",")
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
                format!("#{} {}", base_index, entry_summary(&base[*base_index])),
                format!(
                    "#{} {}",
                    target_index,
                    entry_summary(&target[*target_index])
                ),
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
    if !entry.details.is_empty() {
        let _ = write!(value, " details={}", entry.details.join(","));
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
        "target names={} compared={} identical={} record-multiplicity={} variant-overlap={} disjoint-different={} missing-base={} base-only={}",
        report.target_classes,
        report.compared_classes,
        report.identical_classes,
        report.record_multiplicity_classes,
        report.variant_overlap_classes,
        report.different_classes,
        report.missing_base_classes,
        report.base_only_classes,
    );
    let _ = writeln!(
        out,
        "complete records: target={} base={}; semantic variants: target={} base={}",
        report.target_complete_records,
        report.base_complete_records,
        report.target_semantic_variants,
        report.base_semantic_variants,
    );
    let _ = writeln!(
        out,
        "names with multiple semantic variants: target={} base={}; names with duplicate equal records: target={} base={}",
        report.target_names_with_multiple_variants,
        report.base_names_with_multiple_variants,
        report.target_names_with_duplicate_records,
        report.base_names_with_duplicate_records,
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
    if !report.base_only_class_names.is_empty() {
        let _ = writeln!(out, "base-only class names:");
        for name in &report.base_only_class_names {
            let _ = writeln!(out, "  {name}");
        }
    }

    for class in &report.classes {
        let has_variant_provenance = class.target_variants.len() > 1
            || class.base_variants.len() > 1
            || class
                .target_variants
                .iter()
                .chain(&class.base_variants)
                .any(|variant| variant.type_indices.len() > 1);
        if class.status == "identical"
            && !show_identical
            && report.class_filter.is_none()
            && !has_variant_provenance
        {
            continue;
        }
        let _ = writeln!(out, "\n=== {} [{}] ===", class.name, class.status);
        if has_variant_provenance {
            let _ = writeln!(
                out,
                "  PDB semantic variants: target={} base={}",
                class.target_variants.len(),
                class.base_variants.len(),
            );
            for variant in &class.base_variants {
                let _ = writeln!(
                    out,
                    "    base records={}: {}{}",
                    type_index_summary(&variant.type_indices),
                    variant.summary,
                    render_variant_difference_counts(&variant.differences_from_first),
                );
            }
            for variant in &class.target_variants {
                let _ = writeln!(
                    out,
                    "    target records={}: {}{}",
                    type_index_summary(&variant.type_indices),
                    variant.summary,
                    render_variant_difference_counts(&variant.differences_from_first),
                );
            }
        }
        if let Some(pair) = &class.diagnostic_pair {
            let _ = writeln!(
                out,
                "  diagnostic pair: base={} target={} ({})",
                type_index_summary(&pair.base_type_indices),
                type_index_summary(&pair.target_type_indices),
                pair.reason,
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

fn render_variant_difference_counts(counts: &BTreeMap<String, usize>) -> String {
    if counts.is_empty() {
        return String::new();
    }
    let categories = counts
        .iter()
        .map(|(category, count)| format!("{category}={count}"))
        .collect::<Vec<_>>()
        .join(",");
    format!(" differences-from-first=[{categories}]")
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

    fn order_item(key: &str) -> OrderItem {
        OrderItem {
            key: key.to_owned(),
            value: key.to_owned(),
            comparison_value: key.to_owned(),
        }
    }

    #[test]
    fn sequence_order_does_not_call_insertions_moves() {
        let base = [order_item("a"), order_item("b"), order_item("c")];
        let target = [
            order_item("a"),
            order_item("inserted"),
            order_item("b"),
            order_item("c"),
        ];
        let comparison = compare_sequence("test", "diagnostic", &base, &target);
        assert!(comparison.moved.is_empty());
        assert_eq!(comparison.only_target.len(), 1);
        assert_eq!(comparison.only_target[0].key, "inserted");
        assert_eq!(
            comparison.order_metrics,
            OrderMetrics {
                comparable_pairs: 3,
                inversions: 0,
                longest_ordered_subsequence: 3,
                preserved_adjacent_pairs: 2,
                reversed_adjacent_pairs: 0,
                longest_contiguous_run: 3,
                increasing_runs: 1,
                rank_displacement_sum: 0,
                max_rank_displacement: 0,
            }
        );
    }

    #[test]
    fn sequence_order_marks_both_sides_of_an_inversion() {
        let base = [order_item("a"), order_item("b"), order_item("c")];
        let target = [order_item("a"), order_item("c"), order_item("b")];
        let comparison = compare_sequence("test", "diagnostic", &base, &target);
        let moved: BTreeSet<&str> = comparison
            .moved
            .iter()
            .map(|item| item.key.as_str())
            .collect();
        assert_eq!(moved, BTreeSet::from(["b", "c"]));
        assert_eq!(comparison.order_metrics.comparable_pairs, 3);
        assert_eq!(comparison.order_metrics.inversions, 1);
        assert_eq!(comparison.order_metrics.longest_ordered_subsequence, 2);
        assert_eq!(comparison.order_metrics.preserved_adjacent_pairs, 0);
        assert_eq!(comparison.order_metrics.reversed_adjacent_pairs, 1);
        assert_eq!(comparison.order_metrics.longest_contiguous_run, 1);
        assert_eq!(comparison.order_metrics.increasing_runs, 2);
        assert_eq!(comparison.order_metrics.rank_displacement_sum, 2);
        assert_eq!(comparison.order_metrics.max_rank_displacement, 1);
    }

    #[test]
    fn sequence_order_metrics_distinguish_an_intact_rotated_block() {
        let base = [
            order_item("a"),
            order_item("b"),
            order_item("c"),
            order_item("d"),
        ];
        let target = [
            order_item("c"),
            order_item("d"),
            order_item("a"),
            order_item("b"),
        ];
        let comparison = compare_sequence("test", "diagnostic", &base, &target);

        assert_eq!(comparison.moved.len(), 4);
        assert_eq!(comparison.order_metrics.comparable_pairs, 6);
        assert_eq!(comparison.order_metrics.inversions, 4);
        assert_eq!(comparison.order_metrics.longest_ordered_subsequence, 2);
        assert_eq!(comparison.order_metrics.preserved_adjacent_pairs, 2);
        assert_eq!(comparison.order_metrics.reversed_adjacent_pairs, 0);
        assert_eq!(comparison.order_metrics.longest_contiguous_run, 2);
        assert_eq!(comparison.order_metrics.increasing_runs, 2);
        assert_eq!(comparison.order_metrics.rank_displacement_sum, 8);
        assert_eq!(comparison.order_metrics.max_rank_displacement, 2);
    }

    #[test]
    fn sequence_order_metrics_handle_empty_and_singleton_sequences() {
        let empty = compare_sequence("test", "diagnostic", &[], &[]);
        assert_eq!(
            empty.order_metrics,
            OrderMetrics {
                comparable_pairs: 0,
                inversions: 0,
                longest_ordered_subsequence: 0,
                preserved_adjacent_pairs: 0,
                reversed_adjacent_pairs: 0,
                longest_contiguous_run: 0,
                increasing_runs: 0,
                rank_displacement_sum: 0,
                max_rank_displacement: 0,
            }
        );

        let singleton = [order_item("a")];
        let singleton = compare_sequence("test", "diagnostic", &singleton, &singleton);
        assert_eq!(singleton.order_metrics.comparable_pairs, 0);
        assert_eq!(singleton.order_metrics.longest_ordered_subsequence, 1);
        assert_eq!(singleton.order_metrics.longest_contiguous_run, 1);
        assert_eq!(singleton.order_metrics.increasing_runs, 1);
    }

    #[test]
    fn grouped_order_summaries_isolate_cross_kind_interleaving() {
        let base = [order_item("procedure|a"), order_item("data|b")];
        let target = [order_item("data|b"), order_item("procedure|a")];
        let whole = compare_sequence("test", "diagnostic", &base, &target);
        assert_eq!(whole.order_metrics.inversions, 1);

        let groups = summarize_grouped_sequences(
            "test by kind",
            "diagnostic",
            &base,
            &target,
            key_prefix_group,
            1,
        );
        assert_eq!(groups.len(), 2);
        assert!(groups.iter().all(|group| !group.comparison.different));
        assert!(
            groups
                .iter()
                .all(|group| group.comparison.order_metrics.inversions == 0)
        );
    }

    #[test]
    fn module_library_groups_exclude_direct_link_objects() {
        assert_eq!(
            module_library_group(&order_item("sample.obj|engine.lib")),
            Some("engine.lib".to_owned())
        );
        assert_eq!(
            module_library_group(&order_item("sample.obj|sample.obj")),
            None
        );
    }

    #[test]
    fn focused_module_filter_checks_module_and_container_paths() {
        assert!(module_order_item_matches_filter(
            "module=c:\\build\\vfs\\find.obj object=c:\\build\\vostok_vfs.lib",
            "vfs"
        ));
        assert!(module_order_item_matches_filter(
            "module=c:\\build\\core\\find.obj object=c:\\build\\vostok_vfs.lib",
            "vostok_vfs"
        ));
        assert!(!module_order_item_matches_filter(
            "module=c:\\build\\core\\find.obj object=c:\\build\\vostok_core.lib",
            "vfs"
        ));
    }

    #[test]
    fn focused_type_filter_checks_only_named_type_identity() {
        let collision = OrderItem {
            key: "class|survarium::collision_result".to_owned(),
            value: "type=0x1001 class survarium::collision_result size=0x10".to_owned(),
            comparison_value: "class|survarium::collision_result|size=0x10".to_owned(),
        };
        let unrelated = OrderItem {
            key: "class|survarium::unrelated".to_owned(),
            value: "first-direct-module=collision_result.obj".to_owned(),
            comparison_value: "class|survarium::unrelated".to_owned(),
        };
        let needles = ["collision_result".to_owned()];
        assert!(type_order_item_matches_filters(&collision, &needles));
        assert!(!type_order_item_matches_filters(&unrelated, &needles));

        let multiple = ["missing".to_owned(), "SURVARIUM::COLLISION".to_lowercase()];
        assert!(type_order_item_matches_filters(&collision, &multiple));
    }

    #[test]
    fn sequence_order_excludes_duplicate_keys_from_order_claims() {
        let base = [order_item("a"), order_item("a"), order_item("b")];
        let target = [order_item("a"), order_item("b")];
        let comparison = compare_sequence("test", "diagnostic", &base, &target);
        assert!(comparison.moved.is_empty());
        assert_eq!(comparison.multiplicity.len(), 1);
        assert_eq!(comparison.multiplicity[0].key, "a");
    }

    #[test]
    fn equal_duplicate_keys_are_ambiguous_but_not_different() {
        let base = [order_item("a"), order_item("a"), order_item("b")];
        let target = [order_item("a"), order_item("a"), order_item("b")];
        let comparison = compare_sequence("test", "diagnostic", &base, &target);
        assert!(!sequence_differs(&comparison));
        assert!(comparison.multiplicity.is_empty());
        assert_eq!(comparison.excluded_nonunique.len(), 1);
        assert_eq!(comparison.excluded_nonunique[0].key, "a");
    }

    #[test]
    fn sequence_order_reports_unique_record_value_changes_separately() {
        let base = [OrderItem {
            key: "type|sample".to_owned(),
            value: "class sample size=4".to_owned(),
            comparison_value: "type|sample|size=4".to_owned(),
        }];
        let target = [OrderItem {
            key: "type|sample".to_owned(),
            value: "class sample size=8".to_owned(),
            comparison_value: "type|sample|size=8".to_owned(),
        }];
        let comparison = compare_sequence("test", "diagnostic", &base, &target);
        assert!(comparison.moved.is_empty());
        assert_eq!(comparison.changed.len(), 1);
        assert_eq!(comparison.changed[0].key, "type|sample");
    }

    #[test]
    fn sequence_order_ignores_pdb_local_record_identity() {
        let base = [OrderItem {
            key: "type|sample".to_owned(),
            value: "type=0x1000 class sample size=4".to_owned(),
            comparison_value: "class|sample|size=4".to_owned(),
        }];
        let target = [OrderItem {
            key: "type|sample".to_owned(),
            value: "type=0x2000 class sample size=4".to_owned(),
            comparison_value: "class|sample|size=4".to_owned(),
        }];
        let comparison = compare_sequence("test", "diagnostic", &base, &target);
        assert!(comparison.changed.is_empty());
    }

    #[test]
    fn named_type_module_references_preserve_dbi_order_without_claiming_ownership() {
        let named_types = [(
            0x1001,
            OrderItem {
                key: "struct|shared_type".to_owned(),
                value: "type=0x1001 struct shared_type size=0x4".to_owned(),
                comparison_value: "struct|shared_type|size=0x4".to_owned(),
            },
        )];
        let references = HashMap::from([(
            0x1001,
            BTreeSet::from([
                (2, "beta.obj|probe.lib".to_owned()),
                (1, "alpha.obj|probe.lib".to_owned()),
            ]),
        )]);

        let output = build_named_type_module_references(&named_types, &references);
        assert_eq!(output.len(), 1);
        assert!(
            output[0]
                .value
                .contains("first-direct-module=alpha.obj|probe.lib")
        );
        assert!(
            output[0]
                .comparison_value
                .ends_with("direct-modules=[alpha.obj|probe.lib -> beta.obj|probe.lib]")
        );
    }

    #[test]
    fn raw_c13_files_and_lines_preserve_serialized_order() {
        let mut checksums = Vec::new();
        checksums.extend_from_slice(&1_u32.to_le_bytes());
        checksums.extend_from_slice(&[4, 1, 0xaa, 0xbb, 0xcc, 0xdd, 0, 0]);
        let file_subsection = RawC13Subsection {
            kind: 0xf4,
            offset: 0x20,
            data: &checksums,
        };
        let mut file_names = HashMap::new();
        let mut file_occurrences = HashMap::new();
        let mut files = Vec::new();
        append_raw_c13_files(
            &file_subsection,
            b"\0src\\sample.cpp\0",
            &HashMap::new(),
            &mut file_names,
            &mut file_occurrences,
            &mut files,
            "sample.obj",
        )
        .unwrap();
        assert_eq!(
            file_names.get(&0).map(String::as_str),
            Some("src/sample.cpp")
        );
        assert_eq!(files.len(), 1);
        assert!(files[0].comparison_value.contains("checksum=aabbccdd"));

        let global_names = HashMap::from([(1_usize, "src\\global.cpp".to_owned())]);
        let mut global_file_names = HashMap::new();
        let mut global_occurrences = HashMap::new();
        let mut global_files = Vec::new();
        append_raw_c13_files(
            &file_subsection,
            b"",
            &global_names,
            &mut global_file_names,
            &mut global_occurrences,
            &mut global_files,
            "sample.obj",
        )
        .unwrap();
        assert_eq!(
            global_file_names.get(&0).map(String::as_str),
            Some("src/global.cpp")
        );
        assert!(
            global_files[0]
                .comparison_value
                .contains("path=src/global.cpp")
        );

        let mut lines = Vec::new();
        lines.extend_from_slice(&0x100_u32.to_le_bytes());
        lines.extend_from_slice(&2_u16.to_le_bytes());
        lines.extend_from_slice(&1_u16.to_le_bytes());
        lines.extend_from_slice(&0x20_u32.to_le_bytes());
        lines.extend_from_slice(&0_u32.to_le_bytes());
        lines.extend_from_slice(&2_u32.to_le_bytes());
        lines.extend_from_slice(&36_u32.to_le_bytes());
        lines.extend_from_slice(&8_u32.to_le_bytes());
        lines.extend_from_slice(&(10_u32 | 0x8000_0000).to_le_bytes());
        lines.extend_from_slice(&4_u32.to_le_bytes());
        lines.extend_from_slice(&(20_u32 | (1 << 24) | 0x8000_0000).to_le_bytes());
        lines.extend_from_slice(&1_u16.to_le_bytes());
        lines.extend_from_slice(&3_u16.to_le_bytes());
        lines.extend_from_slice(&4_u16.to_le_bytes());
        lines.extend_from_slice(&7_u16.to_le_bytes());
        let line_subsection = RawC13Subsection {
            kind: 0xf2,
            offset: 0x40,
            data: &lines,
        };
        let mut line_occurrences = HashMap::new();
        let mut output = Vec::new();
        append_raw_c13_lines(
            &line_subsection,
            &file_names,
            &mut line_occurrences,
            &mut output,
            "sample.obj",
        )
        .unwrap();
        assert_eq!(output.len(), 2);
        assert!(output[0].key.contains("start=10"));
        assert!(output[0].key.contains("columns=1-3"));
        assert!(output[1].key.contains("start=20"));
        assert!(output[1].key.contains("delta=1"));
        assert!(output[1].value.contains("offset=0x104"));
    }

    #[test]
    fn dbi_source_info_uses_summed_counts_not_truncated_header_or_indices() {
        let mut bytes = Vec::new();
        bytes.extend_from_slice(&2_u16.to_le_bytes());
        bytes.extend_from_slice(&1_u16.to_le_bytes());
        bytes.extend_from_slice(&10_u16.to_le_bytes());
        bytes.extend_from_slice(&20_u16.to_le_bytes());
        bytes.extend_from_slice(&2_u16.to_le_bytes());
        bytes.extend_from_slice(&1_u16.to_le_bytes());
        bytes.extend_from_slice(&0_u32.to_le_bytes());
        bytes.extend_from_slice(&6_u32.to_le_bytes());
        bytes.extend_from_slice(&0_u32.to_le_bytes());
        bytes.extend_from_slice(b"a.cpp\0b.cpp\0");
        let module = |key: &str| RawDbiModule {
            key: key.to_owned(),
            value: key.to_owned(),
            stream_index: None,
            symbols_size: 0,
            lines_size: 0,
            c13_lines_size: 0,
        };
        let scopes = load_dbi_source_file_scopes(&bytes, &[module("one"), module("two")]).unwrap();
        assert_eq!(scopes.len(), 2);
        assert_eq!(scopes[0].symbols.len(), 2);
        assert!(scopes[0].symbols[0].key.starts_with("a.cpp|"));
        assert!(scopes[0].symbols[1].key.starts_with("b.cpp|"));
        assert_eq!(scopes[1].symbols.len(), 1);
        assert!(scopes[1].symbols[0].key.starts_with("a.cpp|"));
    }

    #[test]
    fn gsi_hash_parser_resolves_symbols_and_bucket_order() {
        let mut bytes = Vec::new();
        bytes.extend_from_slice(&u32::MAX.to_le_bytes());
        bytes.extend_from_slice(&(0xeffe_0000_u32 + 19_990_810).to_le_bytes());
        bytes.extend_from_slice(&16_u32.to_le_bytes());
        bytes.extend_from_slice(&520_u32.to_le_bytes());
        bytes.extend_from_slice(&0x11_u32.to_le_bytes());
        bytes.extend_from_slice(&1_u32.to_le_bytes());
        bytes.extend_from_slice(&0x21_u32.to_le_bytes());
        bytes.extend_from_slice(&2_u32.to_le_bytes());
        for word in 0..129 {
            bytes.extend_from_slice(&(if word == 0 { 1_u32 << 5 } else { 0 }).to_le_bytes());
        }
        bytes.extend_from_slice(&12_u32.to_le_bytes());
        let symbols = HashMap::from([(0x10, order_item("first")), (0x20, order_item("second"))]);
        let (end, records, buckets) = load_gsi_hash_table(&bytes, 0, &symbols, "test").unwrap();
        assert_eq!(end, bytes.len());
        assert_eq!(records.len(), 2);
        assert_eq!(records[0].key, "first");
        assert!(records[1].comparison_value.ends_with("references=2"));
        assert_eq!(buckets.len(), 1);
        assert_eq!(buckets[0].key, "bucket=5");
        assert!(buckets[0].value.contains("start-record=1"));
    }

    #[test]
    fn type_hash_adjustments_preserve_present_and_deleted_bucket_order() {
        let mut bytes = Vec::new();
        bytes.extend_from_slice(&2_u32.to_le_bytes());
        bytes.extend_from_slice(&8_u32.to_le_bytes());
        bytes.extend_from_slice(&1_u32.to_le_bytes());
        bytes.extend_from_slice(&((1_u32 << 1) | (1_u32 << 5)).to_le_bytes());
        bytes.extend_from_slice(&1_u32.to_le_bytes());
        bytes.extend_from_slice(&(1_u32 << 3).to_le_bytes());
        bytes.extend_from_slice(&0xaaaa_u32.to_le_bytes());
        bytes.extend_from_slice(&0x1000_u32.to_le_bytes());
        bytes.extend_from_slice(&0xbbbb_u32.to_le_bytes());
        bytes.extend_from_slice(&0x1001_u32.to_le_bytes());
        let records = HashMap::from([
            (0x1000, order_item("first")),
            (0x1001, order_item("second")),
        ]);
        let mut metadata = Vec::new();
        let mut adjustments = Vec::new();
        append_hash_adjustments(&bytes, "tpi", &records, &mut metadata, &mut adjustments).unwrap();

        assert_eq!(metadata.len(), 3);
        assert_eq!(adjustments.len(), 3);
        assert_eq!(adjustments[0].key, "bucket=1");
        assert!(adjustments[0].comparison_value.contains("first"));
        assert_eq!(adjustments[1].key, "bucket=3");
        assert_eq!(adjustments[1].comparison_value, "deleted");
        assert_eq!(adjustments[2].key, "bucket=5");
        assert!(adjustments[2].comparison_value.contains("second"));
    }

    #[test]
    fn optional_frame_stream_records_preserve_serialized_fields() {
        let attributes = 5_u16 | (2 << 8) | (1 << 11) | (1 << 12) | (3 << 14);
        let mut legacy = Vec::new();
        legacy.extend_from_slice(&0x100_u32.to_le_bytes());
        legacy.extend_from_slice(&0x20_u32.to_le_bytes());
        legacy.extend_from_slice(&3_u32.to_le_bytes());
        legacy.extend_from_slice(&2_u16.to_le_bytes());
        legacy.extend_from_slice(&attributes.to_le_bytes());
        let records = parse_legacy_fpo_records(&legacy, &HashMap::new(), "test FPO").unwrap();
        assert_eq!(records.len(), 1);
        assert_eq!(records[0].key, "rva=0x100|occurrence=0");
        assert!(records[0].comparison_value.contains("prolog=5"));
        assert!(records[0].comparison_value.contains("saved-regs=2"));
        assert!(records[0].comparison_value.contains("frame-type=3"));

        let mut frame = Vec::new();
        for value in [0x200_u32, 0x30, 0x10, 0x08, 0x40, 7] {
            frame.extend_from_slice(&value.to_le_bytes());
        }
        frame.extend_from_slice(&6_u16.to_le_bytes());
        frame.extend_from_slice(&4_u16.to_le_bytes());
        frame.extend_from_slice(&7_u32.to_le_bytes());
        let names = HashMap::from([(7, "frame program".to_owned())]);
        let records =
            parse_frame_data_records(&frame, &names, &HashMap::new(), "test frame data").unwrap();
        assert_eq!(records.len(), 1);
        assert_eq!(records[0].key, "rva=0x200|occurrence=0");
        assert!(records[0].comparison_value.contains("frame program"));
        assert!(records[0].comparison_value.contains("function-start=1"));
    }

    #[test]
    fn frame_data_order_uses_unique_procedure_identity_instead_of_rva() {
        let record = |rva: u32, code_size: u32, locals_size: u32| {
            let mut bytes = Vec::new();
            for value in [rva, code_size, locals_size, 4, 0, 0] {
                bytes.extend_from_slice(&value.to_le_bytes());
            }
            bytes.extend_from_slice(&3_u16.to_le_bytes());
            bytes.extend_from_slice(&0_u16.to_le_bytes());
            bytes.extend_from_slice(&4_u32.to_le_bytes());
            bytes
        };
        let mut base_bytes = record(0x100, 0x20, 0x0c);
        base_bytes.extend(record(0x200, 0x30, 0x1c));
        let mut target_bytes = record(0x100, 0x30, 0x1c);
        target_bytes.extend(record(0x200, 0x20, 0x0c));
        let base_procedures = HashMap::from([
            (0x100, "procedure|alpha".to_owned()),
            (0x200, "procedure|beta".to_owned()),
        ]);
        let target_procedures = HashMap::from([
            (0x100, "procedure|beta".to_owned()),
            (0x200, "procedure|alpha".to_owned()),
        ]);
        let base = parse_frame_data_records(
            &base_bytes,
            &HashMap::new(),
            &base_procedures,
            "base frame data",
        )
        .unwrap();
        let target = parse_frame_data_records(
            &target_bytes,
            &HashMap::new(),
            &target_procedures,
            "target frame data",
        )
        .unwrap();
        let comparison = compare_sequence("frame data", "address-derived", &base, &target);
        assert_eq!(comparison.order_metrics.inversions, 1);
        assert_eq!(comparison.moved.len(), 2);
        assert!(comparison.changed.is_empty());
    }

    #[test]
    fn legacy_fpo_order_uses_unique_procedure_identity_instead_of_rva() {
        let record = |rva: u32, code_size: u32, locals_words: u32| {
            let mut bytes = Vec::new();
            bytes.extend_from_slice(&rva.to_le_bytes());
            bytes.extend_from_slice(&code_size.to_le_bytes());
            bytes.extend_from_slice(&locals_words.to_le_bytes());
            bytes.extend_from_slice(&1_u16.to_le_bytes());
            bytes.extend_from_slice(&5_u16.to_le_bytes());
            bytes
        };
        let mut base_bytes = record(0x100, 0x20, 3);
        base_bytes.extend(record(0x200, 0x30, 7));
        let mut target_bytes = record(0x100, 0x30, 7);
        target_bytes.extend(record(0x200, 0x20, 3));
        let base_procedures = HashMap::from([
            (0x100, "procedure|alpha".to_owned()),
            (0x200, "procedure|beta".to_owned()),
        ]);
        let target_procedures = HashMap::from([
            (0x100, "procedure|beta".to_owned()),
            (0x200, "procedure|alpha".to_owned()),
        ]);
        let base = parse_legacy_fpo_records(&base_bytes, &base_procedures, "base FPO").unwrap();
        let target =
            parse_legacy_fpo_records(&target_bytes, &target_procedures, "target FPO").unwrap();
        let comparison = compare_sequence("legacy FPO", "address-derived", &base, &target);
        assert_eq!(comparison.order_metrics.inversions, 1);
        assert_eq!(comparison.moved.len(), 2);
        assert!(comparison.changed.is_empty());
    }

    #[test]
    fn pdb_string_table_preserves_strings_metadata_and_hash_buckets() {
        let names = b"\0sample\0";
        let mut bytes = Vec::new();
        bytes.extend_from_slice(&0xeffe_effe_u32.to_le_bytes());
        bytes.extend_from_slice(&1_u32.to_le_bytes());
        bytes.extend_from_slice(&(names.len() as u32).to_le_bytes());
        bytes.extend_from_slice(names);
        bytes.extend_from_slice(&2_u32.to_le_bytes());
        bytes.extend_from_slice(&0_u32.to_le_bytes());
        bytes.extend_from_slice(&1_u32.to_le_bytes());
        bytes.extend_from_slice(&2_u32.to_le_bytes());

        let table = parse_global_string_table(&bytes, "test string table").unwrap();
        assert_eq!(table.names.len(), 2);
        assert_eq!(
            table.names_by_offset.get(&1).map(String::as_str),
            Some("sample")
        );
        assert_eq!(table.metadata.len(), 5);
        assert_eq!(table.hash_buckets.len(), 2);
        assert!(table.hash_buckets[1].comparison_value.contains("sample"));
    }

    #[test]
    fn c13_inner_record_decoders_preserve_nested_order() {
        let strings = b"\0src\\sample.cpp\0frame program\0";
        let (items, local_names) = parse_raw_c13_string_table(strings, "sample.obj").unwrap();
        assert_eq!(items.len(), 3);
        assert_eq!(
            local_names.get(&1).map(String::as_str),
            Some("src/sample.cpp")
        );

        let file_names = HashMap::from([(0_u32, "src/sample.cpp".to_owned())]);
        let ids = HashMap::from([(0x1000_u32, order_item("inline-fn"))]);
        let mut inlinees = Vec::new();
        inlinees.extend_from_slice(&1_u32.to_le_bytes());
        inlinees.extend_from_slice(&0x1000_u32.to_le_bytes());
        inlinees.extend_from_slice(&0_u32.to_le_bytes());
        inlinees.extend_from_slice(&42_u32.to_le_bytes());
        inlinees.extend_from_slice(&1_u32.to_le_bytes());
        inlinees.extend_from_slice(&0_u32.to_le_bytes());
        let inlinees = parse_raw_c13_inlinees(&inlinees, &file_names, &ids, "sample.obj").unwrap();
        assert_eq!(inlinees.len(), 1);
        assert!(inlinees[0].comparison_value.contains("inline-fn"));
        assert!(
            inlinees[0]
                .comparison_value
                .contains("extra-files=[\"src/sample.cpp\"]")
        );

        let global_names = HashMap::from([(7_usize, "other.obj".to_owned())]);
        let mut imports = Vec::new();
        imports.extend_from_slice(&7_u32.to_le_bytes());
        imports.extend_from_slice(&2_u32.to_le_bytes());
        imports.extend_from_slice(&0x1000_u32.to_le_bytes());
        imports.extend_from_slice(&0x1001_u32.to_le_bytes());
        let imports = parse_raw_c13_cross_imports(&imports, &global_names, "sample.obj").unwrap();
        assert_eq!(imports.len(), 3);
        assert!(imports[0].comparison_value.contains("imports=2"));
        assert!(imports[2].comparison_value.contains("local-index=0x1001"));

        let types = HashMap::from([(0x2000_u32, order_item("sample-type"))]);
        let mut exports = Vec::new();
        exports.extend_from_slice(&0x1000_u32.to_le_bytes());
        exports.extend_from_slice(&0x2000_u32.to_le_bytes());
        let exports =
            parse_raw_c13_cross_exports(&exports, &types, &HashMap::new(), "sample.obj").unwrap();
        assert_eq!(exports.len(), 1);
        assert!(exports[0].comparison_value.contains("sample-type"));

        let words = parse_raw_c13_words(&[1, 2, 3, 4, 5, 6]);
        assert_eq!(words.len(), 2);
        assert_eq!(words[1].comparison_value, "0x0506");
    }

    fn class_variant(size: u64, type_indices: &[u32]) -> ClassModel {
        ClassModel {
            name: "sample".to_owned(),
            kind: "class",
            size,
            properties: "ClassProperties(0)".to_owned(),
            entries: Vec::new(),
            type_indices: type_indices.to_vec(),
        }
    }

    #[test]
    fn class_shape_ignores_record_identity_but_not_semantics() {
        assert!(same_class_shape(
            &class_variant(4, &[0x1000]),
            &class_variant(4, &[0x2000, 0x2001]),
        ));
        assert!(!same_class_shape(
            &class_variant(4, &[0x1000]),
            &class_variant(8, &[0x2000]),
        ));
    }

    #[test]
    fn overlapping_class_variant_sets_are_not_canonicalized() {
        let base = [class_variant(4, &[0x1000]), class_variant(8, &[0x1001])];
        let target = [class_variant(4, &[0x2000]), class_variant(12, &[0x2001])];
        let matches = matching_variants(&base, &target);
        assert_eq!(matches, vec![(0, 0)]);
        let differences = variant_set_differences(&base, &target, &matches);
        assert_eq!(
            differences
                .iter()
                .filter(|difference| difference.category == "variant-set")
                .count(),
            2
        );
    }
}
