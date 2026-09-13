// SPDX-License-Identifier: GPL-3.0-or-later

use std::path::PathBuf;

use clap::{Args, Parser, Subcommand, ValueEnum};
use vostok_pdb::evidence;
use vostok_pdb::rich_context::{Options, extract_rich_context};

#[derive(Parser)]
#[command(
    name = "vostok-pdb",
    about = "PDB, image, and candidate-source evidence engine"
)]
struct Cli {
    #[command(subcommand)]
    command: Command,
}

#[derive(Subcommand)]
enum Command {
    /// Build a canonical evidence database.
    Index(Index),
    /// Compare two evidence databases.
    Compare(Compare),
    /// Query and render indexed evidence.
    Inspect(Inspect),
    /// Generate reconstruction artifacts.
    Generate(Generate),
    /// Report extraction coverage and unsupported channels.
    Coverage(Coverage),
    /// Inspect raw CodeView topology without flattening duplicate variants.
    Topology(vostok_pdb::topology::Cli),
    /// Compare normalized class, enum, and source-definition evidence.
    Divergence(Divergence),
}

#[derive(Args)]
struct Index {
    #[command(subcommand)]
    kind: IndexKind,
}

#[derive(Subcommand)]
enum IndexKind {
    /// Extract observed PDB facts and correlated PE instructions.
    Pdb(IndexPdb),
    /// Index declarations from candidate source using libclang.
    Source(IndexSource),
}

#[derive(Copy, Clone, ValueEnum)]
enum Side {
    Base,
    Target,
    Candidate,
}

impl Side {
    fn as_str(self) -> &'static str {
        match self {
            Self::Base => "base",
            Self::Target => "target",
            Self::Candidate => "candidate",
        }
    }
}

#[derive(Args)]
struct IndexPdb {
    #[arg(long)]
    pdb: PathBuf,
    #[arg(long)]
    exe: PathBuf,
    #[arg(long)]
    database: PathBuf,
    #[arg(long, value_enum)]
    side: Side,
    #[arg(long = "engine-path", default_values_t = [String::from(r"c:\survarium\sources")])]
    engine_paths: Vec<String>,
    #[arg(long)]
    source_root: Option<PathBuf>,
}

#[derive(Args)]
struct IndexSource {
    #[arg(long, default_value = "compile_commands.json")]
    compile_commands: PathBuf,
    #[arg(long)]
    database: PathBuf,
    /// Restrict parsing to files below this path; repeatable.
    #[arg(long = "scope", required = true)]
    scopes: Vec<PathBuf>,
}

#[derive(Args)]
struct Compare {
    #[command(subcommand)]
    kind: CompareKind,
}

#[derive(Subcommand)]
enum CompareKind {
    /// Compare target and base PDB evidence.
    Pdb(ComparePdb),
    /// Compare target PDB declarations with candidate-source declarations.
    Source(CompareSource),
}

#[derive(Args)]
struct ComparePdb {
    #[arg(long)]
    target: PathBuf,
    #[arg(long)]
    base: PathBuf,
    #[arg(long)]
    function: Option<String>,
    /// Restrict target procedures to a normalized source-file substring.
    #[arg(long)]
    file: Option<String>,
    #[arg(long)]
    json: bool,
    /// Exit nonzero when a deterministic mismatch is present.
    #[arg(long)]
    strict: bool,
}

#[derive(Args)]
struct CompareSource {
    #[arg(long)]
    target: PathBuf,
    #[arg(long)]
    candidate: PathBuf,
    #[arg(long)]
    scope: Option<String>,
    #[arg(long)]
    json: bool,
    #[arg(long)]
    strict: bool,
}

#[derive(Args)]
struct Inspect {
    #[arg(long)]
    database: Option<PathBuf>,
    #[arg(long)]
    target: Option<PathBuf>,
    #[arg(long)]
    base: Option<PathBuf>,
    #[arg(long)]
    function: Option<String>,
    #[arg(long)]
    file: Option<String>,
    #[arg(long, value_parser = parse_hex)]
    rva: Option<u32>,
    #[arg(long, value_parser = parse_hex)]
    va: Option<u32>,
    #[arg(long, value_parser = parse_hex)]
    address: Option<u32>,
    #[arg(long, value_parser = parse_hex)]
    offset: Option<u32>,
    #[arg(long)]
    index: Option<usize>,
    #[arg(long, default_value = "listing")]
    view: String,
    #[arg(long)]
    list: bool,
    #[arg(long)]
    json: bool,
}

#[derive(Args)]
struct Generate {
    #[command(subcommand)]
    kind: GenerateKind,
}

#[derive(Subcommand)]
enum GenerateKind {
    Carcass(GenerateCarcass),
}

#[derive(Args)]
struct GenerateCarcass {
    #[arg(long)]
    pdb: PathBuf,
    #[arg(long)]
    output: PathBuf,
    #[arg(long)]
    engine_path: String,
    #[arg(long)]
    as_base: bool,
    #[arg(long)]
    no_cache: bool,
    #[arg(long)]
    skip_non_engine_headers: bool,
}

#[derive(Args)]
struct Coverage {
    #[arg(long)]
    database: PathBuf,
    #[arg(long)]
    json: bool,
}

#[derive(Args)]
struct Divergence {
    #[arg(long)]
    base_pdb: PathBuf,
    #[arg(long)]
    base_engine_path: String,
    #[arg(long)]
    target_pdb: PathBuf,
    #[arg(long)]
    target_engine_path: String,
    #[arg(long)]
    skip: Vec<String>,
    #[arg(long)]
    include_external: bool,
    /// Compare normalized class and enum declarations.
    #[arg(long)]
    headers: bool,
    /// Compare source definition order and constants.
    #[arg(long)]
    sources: bool,
    #[arg(long)]
    list_presence: bool,
    #[arg(long)]
    list_presence_fns: bool,
    #[arg(long)]
    raw_line_table_counts: bool,
}

fn parse_hex(text: &str) -> Result<u32, std::num::ParseIntError> {
    u32::from_str_radix(text.trim_start_matches("0x").trim_start_matches("0X"), 16)
}

fn normalize_prefix(path: &str) -> String {
    let mut path = path.to_lowercase().replace('/', "\\");
    if !path.ends_with('\\') {
        path.push('\\');
    }
    path
}

fn main() {
    if let Err(error) = run(Cli::parse()) {
        eprintln!("{error}");
        std::process::exit(1);
    }
}

fn run(cli: Cli) -> vostok_pdb::Result<()> {
    match cli.command {
        Command::Index(index) => match index.kind {
            IndexKind::Pdb(args) => index_pdb(args),
            IndexKind::Source(args) => {
                vostok_pdb::source::index(&args.compile_commands, &args.database, &args.scopes)
            }
        },
        Command::Compare(compare) => match compare.kind {
            CompareKind::Pdb(args) => compare_databases(
                &args.target,
                &args.base,
                args.function.as_deref(),
                args.file.as_deref(),
                args.json,
                args.strict,
            ),
            CompareKind::Source(args) => compare_source(args),
        },
        Command::Inspect(args) => inspect(args),
        Command::Generate(generate) => match generate.kind {
            GenerateKind::Carcass(args) => generate_carcass(args),
        },
        Command::Coverage(args) => coverage(args),
        Command::Topology(args) => vostok_pdb::topology::execute(args),
        Command::Divergence(args) => divergence(args),
    }
}

fn divergence(args: Divergence) -> vostok_pdb::Result<()> {
    let both = !args.headers && !args.sources;
    vostok_pdb::divergence::run(
        &args.base_pdb,
        &normalize_prefix(&args.base_engine_path),
        &args.target_pdb,
        &normalize_prefix(&args.target_engine_path),
        &vostok_pdb::divergence::Config {
            skip: args
                .skip
                .into_iter()
                .map(|value| value.to_lowercase())
                .collect(),
            include_external: args.include_external,
            do_headers: both || args.headers,
            do_sources: both || args.sources,
            list_presence: args.list_presence,
            list_presence_fns: args.list_presence_fns,
            compare_raw_line_table_counts: args.raw_line_table_counts,
        },
    )
}

fn compare_source(args: CompareSource) -> vostok_pdb::Result<()> {
    let findings =
        vostok_pdb::source::compare(&args.target, &args.candidate, args.scope.as_deref())?;
    if args.json {
        println!("{}", serde_json::to_string_pretty(&findings)?);
    } else {
        for f in &findings {
            println!(
                "{}\t{}\t{}\t{}",
                f.verdict.as_str(),
                f.category,
                f.subject,
                f.detail
            );
        }
    }
    if args.strict && findings.iter().any(|f| f.verdict.fails_strict()) {
        return vostok_pdb::error!("deterministic source-evidence mismatch or unsupported channel");
    }
    Ok(())
}

fn index_pdb(args: IndexPdb) -> vostok_pdb::Result<()> {
    let options = Options {
        engine_paths: args
            .engine_paths
            .iter()
            .map(|p| normalize_prefix(p))
            .collect(),
        source_root: args.source_root,
        target_mode: !matches!(args.side, Side::Base),
        engine_only: true,
    };
    let entries = extract_rich_context(&args.pdb, &args.exe, &options)?;
    evidence::write_database(
        &args.database,
        args.side.as_str(),
        &args.pdb,
        &args.exe,
        &entries,
    )?;
    let class_facts = vostok_pdb::topology::extract_class_facts(&args.pdb)?;
    evidence::append_facts(&args.database, &class_facts)?;
    eprintln!(
        "indexed {} procedures and {} class/declaration facts into {}",
        entries.len(),
        class_facts.len(),
        args.database.display()
    );
    Ok(())
}

fn compare_databases(
    target: &std::path::Path,
    base: &std::path::Path,
    function: Option<&str>,
    file: Option<&str>,
    json: bool,
    strict: bool,
) -> vostok_pdb::Result<()> {
    let findings = evidence::compare(target, base, function, file)?;
    if json {
        println!("{}", serde_json::to_string_pretty(&findings)?);
    } else {
        for f in &findings {
            println!(
                "{}\t{}\t{}\t{}",
                f.verdict.as_str(),
                f.category,
                f.subject,
                f.detail
            );
        }
    }
    if strict && findings.iter().any(|f| f.verdict.fails_strict()) {
        return vostok_pdb::error!("deterministic evidence mismatch");
    }
    Ok(())
}

fn inspect(args: Inspect) -> vostok_pdb::Result<()> {
    let target_path = args
        .target
        .as_ref()
        .or(args.database.as_ref())
        .or(args.base.as_ref());
    let containing = args
        .va
        .or(if args.function.is_none() && args.rva.is_none() {
            args.address
        } else {
            None
        });
    let mut target = match target_path {
        Some(path) => evidence::search(path, args.function.as_deref(), None, args.rva, containing)?,
        None => Vec::new(),
    };
    if let Some(file) = &args.file {
        target.retain(|entry| entry.file.eq_ignore_ascii_case(file));
    }
    let selected = target.first();
    if args.list {
        if args.json {
            println!("{}", serde_json::to_string(&target)?);
        } else {
            for f in &target {
                println!("0x{:06x}\t{}\t{}", f.rva, f.file, f.name);
            }
        }
        return Ok(());
    }
    if target.len() > 1 && args.rva.is_none() && containing.is_none() {
        return vostok_pdb::error!(
            "ambiguous selection ({} procedures); use --list, --file or an exact --rva",
            target.len()
        );
    }
    let Some(function) = selected else {
        return vostok_pdb::error!("no function matched");
    };
    if args.json {
        println!("{}", serde_json::to_string_pretty(function)?);
        return Ok(());
    }
    let mut base = match &args.base {
        Some(path) => {
            let exact = evidence::search(path, None, Some(&function.mangled), None, None)?;
            if exact.is_empty() {
                evidence::search(path, Some(&function.name), None, None, None)?
            } else {
                exact
            }
        }
        None => Vec::new(),
    };
    if base
        .iter()
        .any(|entry| entry.file.eq_ignore_ascii_case(&function.file))
    {
        base.retain(|entry| entry.file.eq_ignore_ascii_case(&function.file));
    }
    if base.len() > 1 && matches!(args.view.as_str(), "base" | "diff" | "structure-diff") {
        return vostok_pdb::error!(
            "ambiguous base selection ({} procedures); inspect the base with an exact --rva",
            base.len()
        );
    }
    match args.view.as_str() {
        "listing" | "target" => render_selected(function, &args),
        "base" => match base.first() {
            Some(f) => render_selected(f, &args),
            None => return vostok_pdb::error!("no base function matched"),
        },
        "structure" => print!("{}", vostok_pdb::rich_render::render_structure(function)),
        "info" => print!("{}", vostok_pdb::rich_render::render_info(function)),
        "diff" => match base.first() {
            Some(f) => {
                let diff = vostok_pdb::rich_diff::diff(f, function);
                print!(
                    "{}",
                    vostok_pdb::rich_diff::render_unified(f, function, &diff)
                );
            }
            None => return vostok_pdb::error!("diff requires --base"),
        },
        "structure-diff" => match base.first() {
            Some(f) => print!(
                "{}",
                vostok_pdb::rich_structure_diff::render_structure_diff(f, function)
            ),
            None => return vostok_pdb::error!("structure-diff requires --base"),
        },
        other => return vostok_pdb::error!("unknown view {other}"),
    }
    Ok(())
}

fn render_selected(function: &vostok_pdb::rich_context::FunctionEntry, args: &Inspect) {
    let index = args.index.or_else(|| {
        let start = function.image_base.wrapping_add(function.rva);
        let offset = args
            .offset
            .or_else(|| args.address.map(|address| address.wrapping_sub(start)))?;
        (1..function.statements.len().saturating_sub(1)).find(|&i| {
            let s = &function.statements[i];
            offset >= s.off && offset < s.off.wrapping_add(s.size)
        })
    });
    match index {
        Some(index) => print!(
            "{}",
            vostok_pdb::rich_render::render_listing_statement(function, index)
        ),
        None => print!("{}", vostok_pdb::rich_render::render_listing(function)),
    }
}

fn generate_carcass(args: GenerateCarcass) -> vostok_pdb::Result<()> {
    let mut flags = vostok_pdb::GenFlags::empty();
    flags.set(vostok_pdb::GenFlags::AS_BASE, args.as_base);
    flags.set(vostok_pdb::GenFlags::NO_CACHE, args.no_cache);
    flags.set(
        vostok_pdb::GenFlags::SKIP_NON_ENGINE_HEADERS,
        args.skip_non_engine_headers,
    );
    vostok_pdb::dump_pdb::dump_pdb(
        &args.pdb,
        &args.output,
        &normalize_prefix(&args.engine_path),
        flags,
    )
}

fn coverage(args: Coverage) -> vostok_pdb::Result<()> {
    let rows = evidence::coverage(&args.database)?;
    if args.json {
        println!("{}", serde_json::to_string_pretty(&rows)?);
    } else {
        println!("channel\tobserved\tskipped\tunsupported\tdetail");
        for (channel, observed, skipped, unsupported, detail) in rows {
            println!("{channel}\t{observed}\t{skipped}\t{unsupported}\t{detail}");
        }
    }
    Ok(())
}
