// SPDX-License-Identifier: GPL-3.0-or-later

//! Durable, queryable evidence extracted from one PDB/PE pair.
//!
//! SQLite is the canonical generated representation.  Every row names its
//! provenance; comparisons are derived views and never silently promote an
//! inference to an observed match.

use std::path::Path;

use rusqlite::{Connection, OptionalExtension, params};
use sha2::{Digest, Sha256};

use crate::rich_context::FunctionEntry;
use crate::rich_structure_diff::{StructRow, diff_structure, structure_rows};

pub const SCHEMA_VERSION: i64 = 1;

#[derive(Clone, Copy, Debug, serde::Serialize)]
#[serde(rename_all = "SCREAMING_SNAKE_CASE")]
pub enum Origin {
    Observed,
    Correlated,
    Inferred,
    Reviewed,
}

impl Origin {
    fn as_str(self) -> &'static str {
        match self {
            Self::Observed => "OBSERVED",
            Self::Correlated => "CORRELATED",
            Self::Inferred => "INFERRED",
            Self::Reviewed => "REVIEWED",
        }
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, serde::Serialize)]
#[serde(rename_all = "SCREAMING_SNAKE_CASE")]
pub enum Verdict {
    Match,
    Mismatch,
    TargetOnly,
    BaseOnly,
    Ambiguous,
    Unobservable,
    Unsupported,
    StaleInput,
}

impl Verdict {
    pub fn fails_strict(self) -> bool {
        matches!(
            self,
            Self::Mismatch
                | Self::TargetOnly
                | Self::BaseOnly
                | Self::Unsupported
                | Self::StaleInput
        )
    }

    pub fn as_str(self) -> &'static str {
        match self {
            Self::Match => "MATCH",
            Self::Mismatch => "MISMATCH",
            Self::TargetOnly => "TARGET_ONLY",
            Self::BaseOnly => "BASE_ONLY",
            Self::Ambiguous => "AMBIGUOUS",
            Self::Unobservable => "UNOBSERVABLE",
            Self::Unsupported => "UNSUPPORTED",
            Self::StaleInput => "STALE_INPUT",
        }
    }
}

#[derive(Debug, serde::Serialize)]
pub struct Finding {
    pub category: String,
    pub subject: String,
    pub verdict: Verdict,
    pub origin: Origin,
    pub detail: String,
}

#[derive(Clone, Debug, serde::Serialize, serde::Deserialize)]
pub struct SourceFact {
    pub category: String,
    pub subject: String,
    pub value: serde_json::Value,
    pub consumer: String,
}

fn hash_file(path: &Path) -> crate::Result<String> {
    let bytes = std::fs::read(path)?;
    Ok(format!("{:x}", Sha256::digest(bytes)))
}

fn staging_path(path: &Path) -> std::path::PathBuf {
    let mut name = path.as_os_str().to_owned();
    name.push(".tmp");
    name.into()
}

fn replace_database(staging: &Path, destination: &Path) -> crate::Result<()> {
    // The host tool runs on Linux; rename atomically replaces the old snapshot.
    std::fs::rename(staging, destination)?;
    Ok(())
}

pub fn write_database(
    path: &Path,
    side: &str,
    pdb_path: &Path,
    exe_path: &Path,
    entries: &[FunctionEntry],
) -> crate::Result<()> {
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent)?;
    }
    let staging = staging_path(path);
    if staging.exists() {
        std::fs::remove_file(&staging)?;
    }
    let mut db = Connection::open(&staging)?;
    db.pragma_update(None, "journal_mode", "DELETE")?;
    db.pragma_update(None, "foreign_keys", true)?;
    let tx = db.transaction()?;
    tx.execute_batch(
        "DROP TABLE IF EXISTS skipped_blocks;
         DROP TABLE IF EXISTS locals;
         DROP TABLE IF EXISTS instructions;
         DROP TABLE IF EXISTS statements;
         DROP TABLE IF EXISTS function_payloads;
         DROP TABLE IF EXISTS functions;
         DROP TABLE IF EXISTS facts;
         DROP TABLE IF EXISTS coverage;
         DROP TABLE IF EXISTS metadata;
         CREATE TABLE metadata(key TEXT PRIMARY KEY, value TEXT NOT NULL);
         CREATE TABLE functions(
           id INTEGER PRIMARY KEY,
           name TEXT NOT NULL,
           mangled TEXT NOT NULL,
           rva INTEGER NOT NULL,
           image_base INTEGER NOT NULL,
           size INTEGER NOT NULL,
           file TEXT NOT NULL,
           origin TEXT NOT NULL CHECK(origin IN ('OBSERVED','CORRELATED','INFERRED','REVIEWED'))
         );
         CREATE INDEX functions_name ON functions(name);
         CREATE INDEX functions_mangled ON functions(mangled);
         CREATE INDEX functions_rva ON functions(rva);
         CREATE TABLE function_payloads(
           function_id INTEGER PRIMARY KEY REFERENCES functions(id) ON DELETE CASCADE,
           statements_json TEXT NOT NULL,
           instructions_json TEXT NOT NULL,
           locals_json TEXT NOT NULL,
           skipped_blocks_json TEXT NOT NULL,
           statement_origin TEXT NOT NULL,
           instruction_origin TEXT NOT NULL
         );
         CREATE TABLE facts(
           id INTEGER PRIMARY KEY,
           category TEXT NOT NULL,
           subject TEXT NOT NULL,
           value_json TEXT NOT NULL,
           module_id INTEGER,
           type_index INTEGER,
           consumer TEXT,
           origin TEXT NOT NULL,
           observable INTEGER NOT NULL
         );
         CREATE INDEX facts_subject ON facts(category, subject);
         CREATE TABLE coverage(
           channel TEXT PRIMARY KEY,
           observed INTEGER NOT NULL,
           skipped INTEGER NOT NULL,
           unsupported INTEGER NOT NULL,
           detail TEXT NOT NULL
         );",
    )?;

    let meta = [
        ("schema_version", SCHEMA_VERSION.to_string()),
        ("side", side.to_string()),
        ("pdb_path", pdb_path.canonicalize()?.display().to_string()),
        ("pdb_sha256", hash_file(pdb_path)?),
        ("exe_path", exe_path.canonicalize()?.display().to_string()),
        ("exe_sha256", hash_file(exe_path)?),
    ];
    for (key, value) in meta {
        tx.execute(
            "INSERT INTO metadata(key,value) VALUES(?1,?2)",
            params![key, value],
        )?;
    }

    for entry in entries {
        tx.execute(
            "INSERT INTO functions(name,mangled,rva,image_base,size,file,origin)
             VALUES(?1,?2,?3,?4,?5,?6,?7)",
            params![
                entry.name,
                entry.mangled,
                entry.rva,
                entry.image_base,
                entry.size,
                entry.file,
                Origin::Observed.as_str()
            ],
        )?;
        let function_id = tx.last_insert_rowid();
        if side == "target" {
            tx.execute(
                "INSERT OR IGNORE INTO facts(category,subject,value_json,consumer,origin,observable)
                 VALUES('source_body',?1,?2,?3,'OBSERVED',0)",
                params![entry.mangled,
                    "{\"verdict\":\"UNOBSERVABLE\",\"reason\":\"PDB stores emitted code and line geometry, not source expressions\"}",
                    entry.file],
            )?;
        }
        tx.execute(
            "INSERT INTO function_payloads VALUES(?1,?2,?3,?4,?5,?6,?7)",
            params![
                function_id,
                serde_json::to_string(&entry.statements)?,
                serde_json::to_string(&entry.instructions)?,
                serde_json::to_string(&entry.locals)?,
                serde_json::to_string(&entry.skipped_blocks)?,
                Origin::Observed.as_str(),
                Origin::Correlated.as_str()
            ],
        )?;
    }

    let counts = [
        ("functions", entries.len()),
        (
            "statements",
            entries.iter().map(|e| e.statements.len()).sum(),
        ),
        (
            "instructions",
            entries.iter().map(|e| e.instructions.len()).sum(),
        ),
        ("locals", entries.iter().map(|e| e.locals.len()).sum()),
    ];
    for (channel, count) in counts {
        tx.execute(
            "INSERT INTO coverage VALUES(?1,?2,0,0,'complete for retained procedures')",
            params![channel, count as i64],
        )?;
    }
    tx.execute(
        "INSERT INTO coverage VALUES('symbol_records',0,0,1,
         'record-level skip accounting is not yet emitted by pdb2')",
        [],
    )?;
    tx.commit()?;
    db.execute_batch("PRAGMA optimize")?;
    drop(db);
    replace_database(&staging, path)?;
    Ok(())
}

pub fn write_source_database(
    path: &Path,
    compile_commands: &Path,
    scopes: &[std::path::PathBuf],
    facts: &[SourceFact],
    parsed_tus: usize,
    failed_tus: usize,
    external_diagnostics: usize,
) -> crate::Result<()> {
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent)?;
    }
    let staging = staging_path(path);
    if staging.exists() {
        std::fs::remove_file(&staging)?;
    }
    let mut db = Connection::open(&staging)?;
    let tx = db.transaction()?;
    tx.execute_batch(
        "DROP TABLE IF EXISTS facts;
         DROP TABLE IF EXISTS coverage;
         DROP TABLE IF EXISTS metadata;
         CREATE TABLE metadata(key TEXT PRIMARY KEY, value TEXT NOT NULL);
         CREATE TABLE facts(
           id INTEGER PRIMARY KEY,
           category TEXT NOT NULL,
           subject TEXT NOT NULL,
           value_json TEXT NOT NULL,
           module_id INTEGER,
           type_index INTEGER,
           consumer TEXT,
           origin TEXT NOT NULL,
           observable INTEGER NOT NULL
         );
         CREATE INDEX facts_subject ON facts(category, subject);
         CREATE TABLE coverage(
           channel TEXT PRIMARY KEY,
           observed INTEGER NOT NULL,
           skipped INTEGER NOT NULL,
           unsupported INTEGER NOT NULL,
           detail TEXT NOT NULL
         );",
    )?;
    for (key, value) in [
        ("schema_version", SCHEMA_VERSION.to_string()),
        ("side", "candidate".to_string()),
        (
            "compile_commands",
            compile_commands.canonicalize()?.display().to_string(),
        ),
        ("compile_commands_sha256", hash_file(compile_commands)?),
        ("scopes", serde_json::to_string(scopes)?),
        (
            "source_snapshot",
            serde_json::to_string(&source_snapshot(scopes)?)?,
        ),
    ] {
        tx.execute("INSERT INTO metadata VALUES(?1,?2)", params![key, value])?;
    }
    for fact in facts {
        tx.execute(
            "INSERT OR IGNORE INTO facts(category,subject,value_json,consumer,origin,observable)
             VALUES(?1,?2,?3,?4,'OBSERVED',1)",
            params![
                fact.category,
                fact.subject,
                serde_json::to_string(&fact.value)?,
                fact.consumer
            ],
        )?;
    }
    tx.execute(
        "INSERT INTO coverage VALUES('translation_units',?1,?2,0,?3)",
        params![
            parsed_tus as i64,
            failed_tus as i64,
            "libclang parse failures remain explicit and make strict comparison incomplete"
        ],
    )?;
    tx.execute(
        "INSERT INTO coverage VALUES('source_facts',?1,0,0,'declarations and placeholder markers in scope')",
        params![facts.len() as i64],
    )?;
    tx.execute(
        "INSERT INTO coverage VALUES('external_diagnostics',?1,0,0,
         'libclang errors outside the requested scopes; retained for coverage but do not invalidate scoped declarations')",
        params![external_diagnostics as i64],
    )?;
    tx.commit()?;
    db.execute_batch("PRAGMA optimize")?;
    drop(db);
    replace_database(&staging, path)?;
    Ok(())
}

pub fn load_facts(path: &Path, filter: Option<&str>) -> crate::Result<Vec<SourceFact>> {
    load_facts_matching(path, filter, false)
}

pub fn load_facts_prefix(path: &Path, prefix: Option<&str>) -> crate::Result<Vec<SourceFact>> {
    load_facts_matching(path, prefix, true)
}

fn load_facts_matching(
    path: &Path,
    filter: Option<&str>,
    prefix: bool,
) -> crate::Result<Vec<SourceFact>> {
    let db = Connection::open(path)?;
    let mut q = db.prepare(
        "SELECT category,subject,value_json,COALESCE(consumer,'') FROM facts ORDER BY category,subject,id")?;
    let needle = filter.map(str::to_lowercase);
    let mut facts = Vec::new();
    for row in q.query_map([], |r| {
        Ok((
            r.get::<_, String>(0)?,
            r.get::<_, String>(1)?,
            r.get::<_, String>(2)?,
            r.get::<_, String>(3)?,
        ))
    })? {
        let (category, subject, value, consumer) = row?;
        if let Some(needle) = &needle {
            let subject = subject.to_lowercase();
            let matches = if prefix {
                subject.starts_with(needle)
            } else {
                subject.contains(needle)
            };
            if !matches {
                continue;
            }
        }
        facts.push(SourceFact {
            category,
            subject,
            value: serde_json::from_str(&value)?,
            consumer,
        });
    }
    Ok(facts)
}

pub fn metadata(path: &Path, key: &str) -> crate::Result<Option<String>> {
    let db = Connection::open(path)?;
    Ok(db
        .query_row("SELECT value FROM metadata WHERE key=?1", [key], |row| {
            row.get(0)
        })
        .optional()?)
}

fn source_snapshot(
    scopes: &[std::path::PathBuf],
) -> crate::Result<std::collections::BTreeMap<String, String>> {
    let mut snapshot = std::collections::BTreeMap::new();
    let mut pending = scopes.to_vec();
    let mut visited = std::collections::HashSet::new();
    while let Some(path) = pending.pop() {
        let path = path.canonicalize()?;
        if !visited.insert(path.clone()) {
            continue;
        }
        if path.is_dir() {
            for entry in std::fs::read_dir(path)? {
                pending.push(entry?.path());
            }
        } else if matches!(
            path.extension().and_then(|s| s.to_str()),
            Some("h" | "hpp" | "cpp" | "c" | "inl")
        ) {
            snapshot.insert(path.display().to_string(), hash_file(&path)?);
        }
    }
    Ok(snapshot)
}

/// Validate recorded inputs before deriving a fresh verdict from a snapshot.
pub fn freshness(path: &Path) -> crate::Result<Vec<Finding>> {
    let mut findings = Vec::new();
    let mut report = |subject: String, verdict: Verdict, detail: String| {
        findings.push(Finding {
            category: "input_freshness".into(),
            subject,
            verdict,
            origin: Origin::Observed,
            detail,
        });
    };
    if metadata(path, "schema_version")?.as_deref() != Some(&SCHEMA_VERSION.to_string()) {
        report(
            path.display().to_string(),
            Verdict::Unsupported,
            "unsupported evidence schema; re-index inputs".into(),
        );
        return Ok(findings);
    }
    let candidate = metadata(path, "side")?.as_deref() == Some("candidate");
    let inputs: &[(&str, &str)] = if candidate {
        &[("compile_commands", "compile_commands_sha256")]
    } else {
        &[("pdb_path", "pdb_sha256"), ("exe_path", "exe_sha256")]
    };
    for (path_key, hash_key) in inputs {
        match (metadata(path, path_key)?, metadata(path, hash_key)?) {
            (Some(input), Some(expected)) => match hash_file(Path::new(&input)) {
                Ok(actual) if actual == expected => (),
                Ok(_) => report(
                    input,
                    Verdict::StaleInput,
                    "input content changed; re-index before comparing".into(),
                ),
                Err(error) => report(
                    input,
                    Verdict::StaleInput,
                    format!("input unavailable: {error}"),
                ),
            },
            _ => report(
                path.display().to_string(),
                Verdict::Unsupported,
                format!("missing {path_key}/{hash_key} provenance; re-index inputs"),
            ),
        }
    }
    if candidate {
        match (
            metadata(path, "scopes")?,
            metadata(path, "source_snapshot")?,
        ) {
            (Some(scopes), Some(snapshot)) => {
                let scopes: Vec<std::path::PathBuf> = serde_json::from_str(&scopes)?;
                let expected: std::collections::BTreeMap<String, String> =
                    serde_json::from_str(&snapshot)?;
                match source_snapshot(&scopes) {
                    Ok(actual) if actual == expected => (),
                    Ok(_) => report(
                        path.display().to_string(),
                        Verdict::StaleInput,
                        "scoped source files changed, appeared, or disappeared; re-index source"
                            .into(),
                    ),
                    Err(error) => report(
                        path.display().to_string(),
                        Verdict::StaleInput,
                        format!("scoped source unavailable: {error}"),
                    ),
                }
            }
            _ => report(
                path.display().to_string(),
                Verdict::Unsupported,
                "missing source snapshot; re-index source".into(),
            ),
        }
    }
    Ok(findings)
}

pub fn append_facts(path: &Path, facts: &[SourceFact]) -> crate::Result<()> {
    let mut db = Connection::open(path)?;
    let tx = db.transaction()?;
    for fact in facts {
        tx.execute(
            "INSERT INTO facts(category,subject,value_json,consumer,origin,observable)
             VALUES(?1,?2,?3,?4,'OBSERVED',1)",
            params![
                fact.category,
                fact.subject,
                serde_json::to_string(&fact.value)?,
                fact.consumer
            ],
        )?;
    }
    tx.execute(
        "INSERT OR REPLACE INTO coverage VALUES('class_declarations',?1,0,0,
         'all complete semantic variants; duplicate type indices retained in provenance')",
        params![facts.len() as i64],
    )?;
    tx.execute(
        "INSERT OR REPLACE INTO coverage VALUES('physical_pdb_topology',0,0,0,
         'available on demand through vostok-pdb topology; physical stream order is not source declaration order')",
        [],
    )?;
    tx.commit()?;
    Ok(())
}

pub fn search(
    path: &Path,
    name: Option<&str>,
    mangled: Option<&str>,
    rva: Option<u32>,
    containing_va: Option<u32>,
) -> crate::Result<Vec<FunctionEntry>> {
    let db = Connection::open(path)?;
    let mut sql = String::from(
        "SELECT f.name,f.mangled,f.rva,f.image_base,f.size,f.file,
         p.statements_json,p.instructions_json,p.locals_json,p.skipped_blocks_json
         FROM functions f JOIN function_payloads p ON p.function_id=f.id WHERE 1=1",
    );
    let mut values = Vec::<rusqlite::types::Value>::new();
    if let Some(name) = name {
        sql.push_str(" AND (f.mangled=? OR instr(lower(f.name),?) > 0)");
        values.push(name.to_string().into());
        values.push(name.to_lowercase().into());
    }
    if let Some(mangled) = mangled {
        sql.push_str(" AND f.mangled=?");
        values.push(mangled.to_string().into());
    }
    if let Some(rva) = rva {
        sql.push_str(" AND f.rva=?");
        values.push((rva as i64).into());
    }
    if let Some(va) = containing_va {
        sql.push_str(" AND ? >= f.image_base+f.rva AND ? < f.image_base+f.rva+f.size");
        values.push((va as i64).into());
        values.push((va as i64).into());
    }
    sql.push_str(" ORDER BY f.file,f.rva,f.id");
    let mut query = db.prepare(&sql)?;
    let rows = query.query_map(rusqlite::params_from_iter(values), |row| {
        Ok((
            row.get::<_, String>(0)?,
            row.get::<_, String>(1)?,
            row.get::<_, i64>(2)? as u32,
            row.get::<_, i64>(3)? as u32,
            row.get::<_, i64>(4)? as u32,
            row.get::<_, String>(5)?,
            row.get::<_, String>(6)?,
            row.get::<_, String>(7)?,
            row.get::<_, String>(8)?,
            row.get::<_, String>(9)?,
        ))
    })?;
    let mut result = Vec::new();
    for row in rows {
        let (name, mangled, rva, image_base, size, file, statements, instructions, locals, blocks) =
            row?;
        result.push(FunctionEntry {
            name,
            mangled,
            rva,
            image_base,
            size,
            file,
            statements: serde_json::from_str(&statements)?,
            instructions: serde_json::from_str(&instructions)?,
            locals: serde_json::from_str(&locals)?,
            skipped_blocks: serde_json::from_str(&blocks)?,
        });
    }
    if let Some(name) = name {
        if result.iter().any(|entry| entry.mangled == name) {
            result.retain(|entry| entry.mangled == name);
        }
    }
    Ok(result)
}

pub fn compare(
    target: &Path,
    base: &Path,
    function: Option<&str>,
    file: Option<&str>,
) -> crate::Result<Vec<Finding>> {
    let mut findings = freshness(target)?;
    findings.extend(freshness(base)?);
    if !findings.is_empty() {
        return Ok(findings);
    }
    let mut targets = search(target, function, None, None, None)?;
    if let Some(file) = file {
        let file = file.to_lowercase().replace('\\', "/");
        targets.retain(|entry| entry.file.to_lowercase().replace('\\', "/").contains(&file));
    }
    let bases = search(base, None, None, None, None)?;
    if targets.is_empty() && (function.is_some() || file.is_some()) {
        return Ok(vec![Finding {
            category: "selection".into(),
            subject: function.or(file).unwrap().into(),
            verdict: Verdict::Unsupported,
            origin: Origin::Observed,
            detail: "no target procedure matched the requested scope".into(),
        }]);
    }
    let mut by_identity = std::collections::HashMap::<&str, Vec<(usize, &FunctionEntry)>>::new();
    let mut by_name = std::collections::HashMap::<&str, Vec<(usize, &FunctionEntry)>>::new();
    for (index, entry) in bases.iter().enumerate() {
        by_identity
            .entry(&entry.mangled)
            .or_default()
            .push((index, entry));
        by_name.entry(&entry.name).or_default().push((index, entry));
    }
    let mut consumed = std::collections::HashSet::new();
    for target_fn in targets {
        let mut candidates = if target_fn.mangled.is_empty() {
            by_name.get(target_fn.name.as_str())
        } else {
            by_identity.get(target_fn.mangled.as_str())
        }
        .cloned()
        .unwrap_or_default();
        if candidates.is_empty() {
            // An ICF group can select a different decorated representative while
            // retaining the same full procedure signature. Keep the identity
            // discrepancy below; pairing is not evidence of symbol equality.
            candidates = by_name
                .get(target_fn.name.as_str())
                .cloned()
                .unwrap_or_default();
        }
        let same_name: Vec<_> = candidates
            .iter()
            .copied()
            .filter(|(_, b)| b.name == target_fn.name)
            .collect();
        if !same_name.is_empty() {
            candidates = same_name;
        }
        let same_file: Vec<_> = candidates
            .iter()
            .copied()
            .filter(|(_, b)| b.file.eq_ignore_ascii_case(&target_fn.file))
            .collect();
        if !same_file.is_empty() {
            candidates = same_file;
        }
        let subject = target_fn.mangled.clone();
        if candidates.is_empty() {
            findings.push(Finding {
                category: "presence".into(),
                subject,
                verdict: Verdict::TargetOnly,
                origin: Origin::Observed,
                detail: target_fn.name,
            });
            continue;
        }
        if candidates.len() > 1 {
            consumed.extend(candidates.iter().map(|(index, _)| *index));
            findings.push(Finding {
                category: "identity".into(),
                subject,
                verdict: Verdict::Ambiguous,
                origin: Origin::Observed,
                detail: format!("{} candidate base procedures", candidates.len()),
            });
            continue;
        }
        let (index, base_fn) = candidates[0];
        consumed.insert(index);
        if target_fn.mangled != base_fn.mangled {
            findings.push(Finding {
                category: "identity".into(),
                subject: subject.clone(),
                verdict: Verdict::Mismatch,
                origin: Origin::Correlated,
                detail: format!(
                    "paired exact full signature {}; decorated representatives differ: target={} base={}",
                    target_fn.name, target_fn.mangled, base_fn.mangled
                ),
            });
        }
        let locals = |entry: &FunctionEntry| {
            let mut rows: Vec<_> = entry
                .locals
                .iter()
                .map(|local| (local.name.clone(), local.ty.clone(), local.scope))
                .collect();
            rows.sort();
            rows
        };
        findings.push(Finding {
            category: "locals".into(),
            subject: subject.clone(),
            verdict: if locals(&target_fn) == locals(base_fn) {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: format!(
                "target={} base={} recorded local name/type/scope entries",
                target_fn.locals.len(),
                base_fn.locals.len()
            ),
        });
        let location_match = target_fn.file.eq_ignore_ascii_case(&base_fn.file);
        findings.push(Finding {
            category: "location".into(),
            subject: subject.clone(),
            verdict: if location_match {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: format!("target={} base={}", target_fn.file, base_fn.file),
        });
        let target_rows = structure_rows(&target_fn);
        let base_rows = structure_rows(base_fn);
        let structure_diff = diff_structure(&base_rows, &target_rows);
        let structure_match = structure_diff
            .iter()
            .all(|row| matches!(row, StructRow::Equal { .. } | StructRow::EmptyEqual));
        findings.push(Finding {
            category: "statement_structure".into(),
            subject: subject.clone(),
            verdict: if target_rows.is_empty() && base_rows.is_empty() {
                Verdict::Unobservable
            } else if structure_match {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: format!(
                "target={} base={} body rows",
                target_rows.len(),
                base_rows.len()
            ),
        });
        findings.push(Finding {
            category: "function_size".into(),
            subject: subject.clone(),
            verdict: if target_fn.size == base_fn.size {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: format!(
                "target={} base={} bytes (whole procedure, including frame code)",
                target_fn.size, base_fn.size
            ),
        });
        let target_files = statement_files(&target_fn);
        let base_files = statement_files(base_fn);
        findings.push(Finding {
            category: "statement_files".into(),
            subject: subject.clone(),
            verdict: if target_files.is_empty() || base_files.is_empty() {
                Verdict::Unobservable
            } else if target_files == base_files {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: format!(
                "target={} base={} observed file runs (all positive-line records; independent of statement packing)",
                target_files.len(),
                base_files.len()
            ),
        });
        let target_lines = relative_statement_lines(&target_fn);
        let base_lines = relative_statement_lines(base_fn);
        findings.push(Finding {
            category: "statement_line_geometry".into(),
            subject: subject.clone(),
            verdict: if target_lines.is_empty() && base_lines.is_empty() {
                Verdict::Unobservable
            } else if target_lines == base_lines {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: format!(
                "target={} base={} relative line records",
                target_lines.len(),
                base_lines.len()
            ),
        });
        let target_raw = raw_line_records(&target_fn);
        let base_raw = raw_line_records(base_fn);
        findings.push(Finding {
            category: "raw_line_records".into(),
            subject,
            verdict: if target_raw.is_empty() || base_raw.is_empty() {
                Verdict::Unobservable
            } else if target_raw == base_raw {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: format!("target={} base={} observed (offset,size,file,relative line) records including boundaries; not byte equality", target_raw.len(), base_raw.len()),
        });
    }
    for (index, base_fn) in bases.iter().enumerate() {
        let in_scope = function.is_none_or(|needle| {
            base_fn.mangled == needle
                || base_fn.name.to_lowercase().contains(&needle.to_lowercase())
        }) && file
            .is_none_or(|needle| normalize_file(&base_fn.file).contains(&normalize_file(needle)));
        if !consumed.contains(&index) && in_scope {
            findings.push(Finding {
                category: "presence".into(),
                subject: base_fn.mangled.clone(),
                verdict: Verdict::BaseOnly,
                origin: Origin::Observed,
                detail: base_fn.name.clone(),
            });
        }
    }
    Ok(findings)
}

fn body_statements(entry: &FunctionEntry) -> &[crate::rich_context::Statement] {
    if entry.statements.len() <= 2 {
        &[]
    } else {
        &entry.statements[1..entry.statements.len() - 1]
    }
}

fn normalize_file(file: &str) -> String {
    file.to_lowercase().replace('\\', "/")
}

fn statement_files(entry: &FunctionEntry) -> Vec<String> {
    // A singleton can cover the entire procedure. Dropping prologue/epilogue
    // rows here would turn a line-packing difference into a false file change.
    let mut files: Vec<_> = entry
        .statements
        .iter()
        .filter(|statement| statement.line != 0 && !statement.file.is_empty())
        .map(|statement| normalize_file(&statement.file))
        .collect();
    files.dedup();
    files
}

fn relative_statement_lines(entry: &FunctionEntry) -> Vec<(String, i64)> {
    let mut origins = std::collections::BTreeMap::<String, u32>::new();
    body_statements(entry)
        .iter()
        .map(|statement| {
            let file = normalize_file(&statement.file);
            let origin = origins.entry(file.clone()).or_insert(statement.line);
            (file, i64::from(statement.line) - i64::from(*origin))
        })
        .collect()
}

fn raw_line_records(entry: &FunctionEntry) -> Vec<(u32, u32, String, i64)> {
    let mut origins = std::collections::BTreeMap::<String, u32>::new();
    entry
        .statements
        .iter()
        .filter(|s| s.line != 0 && !s.file.is_empty())
        .map(|s| {
            let file = normalize_file(&s.file);
            let origin = origins.entry(file.clone()).or_insert(s.line);
            (s.off, s.size, file, i64::from(s.line) - i64::from(*origin))
        })
        .collect()
}

pub fn coverage(path: &Path) -> crate::Result<Vec<(String, i64, i64, i64, String)>> {
    let db = Connection::open(path)?;
    let mut q = db.prepare(
        "SELECT channel,observed,skipped,unsupported,detail FROM coverage ORDER BY channel",
    )?;
    Ok(q.query_map([], |r| {
        Ok((r.get(0)?, r.get(1)?, r.get(2)?, r.get(3)?, r.get(4)?))
    })?
    .collect::<Result<_, _>>()?)
}

#[cfg(test)]
mod tests {
    use super::*;

    struct Fixture(std::path::PathBuf);
    impl Fixture {
        fn new() -> Self {
            let path =
                std::env::temp_dir().join(format!("vostok-evidence-{}", uuid::Uuid::new_v4()));
            std::fs::create_dir(&path).unwrap();
            Self(path)
        }
        fn file(&self, name: &str, content: &str) -> std::path::PathBuf {
            let path = self.0.join(name);
            std::fs::write(&path, content).unwrap();
            path
        }
    }
    impl Drop for Fixture {
        fn drop(&mut self) {
            let _ = std::fs::remove_dir_all(&self.0);
        }
    }
    fn entry(name: &str) -> FunctionEntry {
        serde_json::from_value(serde_json::json!({
            "name":name,"mangled":name,"rva":4096,"size":8,"file":"unit.cpp",
            "statements":[],"instructions":[]
        }))
        .unwrap()
    }

    #[test]
    fn ambiguous_candidates_are_not_reported_base_only() {
        let fixture = Fixture::new();
        let pdb = fixture.file("input.pdb", "pdb");
        let exe = fixture.file("input.exe", "exe");
        let target = fixture.0.join("target.sqlite");
        let base = fixture.0.join("base.sqlite");
        write_database(&target, "target", &pdb, &exe, &[entry("overload")]).unwrap();
        write_database(
            &base,
            "base",
            &pdb,
            &exe,
            &[entry("overload"), entry("overload")],
        )
        .unwrap();
        let findings = compare(&target, &base, None, None).unwrap();
        assert_eq!(findings.len(), 1);
        assert_eq!(findings[0].verdict, Verdict::Ambiguous);
        std::fs::write(&exe, "changed").unwrap();
        assert!(
            compare(&target, &base, None, None)
                .unwrap()
                .iter()
                .all(|f| f.verdict == Verdict::StaleInput)
        );
        assert!(Verdict::StaleInput.fails_strict());
    }

    #[test]
    fn alias_signature_pairing_preserves_identity_and_location_differences() {
        let fixture = Fixture::new();
        let pdb = fixture.file("input.pdb", "pdb");
        let exe = fixture.file("input.exe", "exe");
        let target = fixture.0.join("target.sqlite");
        let base = fixture.0.join("base.sqlite");
        let mut t = entry("void a::execute()");
        let mut b = t.clone();
        t.mangled = "target_representative".into();
        b.mangled = "base_representative".into();
        b.file = "other.cpp".into();
        write_database(&target, "target", &pdb, &exe, &[t]).unwrap();
        write_database(&base, "base", &pdb, &exe, &[b]).unwrap();
        let findings = compare(&target, &base, None, None).unwrap();
        assert!(
            findings
                .iter()
                .any(|f| f.category == "identity" && f.verdict == Verdict::Mismatch)
        );
        assert!(
            findings
                .iter()
                .any(|f| f.category == "location" && f.verdict == Verdict::Mismatch)
        );
        assert!(findings.iter().any(|f| f.category == "locals"));
        assert!(!findings.iter().any(|f| f.category == "presence"));
    }

    #[test]
    fn shared_representative_does_not_make_distinct_signatures_ambiguous() {
        let fixture = Fixture::new();
        let pdb = fixture.file("input.pdb", "pdb");
        let exe = fixture.file("input.exe", "exe");
        let target = fixture.0.join("target.sqlite");
        let base = fixture.0.join("base.sqlite");
        let mut first = entry("void a::execute()");
        first.mangled = "shared".into();
        let mut second = first.clone();
        second.name = "void b::execute()".into();
        let entries = [first, second];
        write_database(&target, "target", &pdb, &exe, &entries).unwrap();
        write_database(&base, "base", &pdb, &exe, &entries).unwrap();
        let findings = compare(&target, &base, None, None).unwrap();
        assert!(findings.iter().all(|f| f.verdict
            == if matches!(
                f.category.as_str(),
                "statement_files"
                    | "statement_structure"
                    | "statement_line_geometry"
                    | "raw_line_records"
            ) {
                Verdict::Unobservable // This identity-only fixture has no line records.
            } else {
                Verdict::Match
            }));
        assert_eq!(findings.len(), 14);
    }

    #[test]
    fn source_snapshot_detects_edits_additions_and_removals() {
        let fixture = Fixture::new();
        let commands = fixture.file("compile_commands.json", "[]");
        let header = fixture.file("sample.h", "class sample {};");
        let db = fixture.0.join("candidate.sqlite");
        let save = || {
            write_source_database(
                &db,
                &commands,
                std::slice::from_ref(&fixture.0),
                &[],
                1,
                0,
                0,
            )
            .unwrap()
        };
        save();
        assert!(freshness(&db).unwrap().is_empty());
        std::fs::write(&header, "struct sample {};").unwrap();
        assert_eq!(freshness(&db).unwrap()[0].verdict, Verdict::StaleInput);
        save();
        let added = fixture.file("added.h", "class added {};");
        assert_eq!(freshness(&db).unwrap()[0].verdict, Verdict::StaleInput);
        save();
        std::fs::remove_file(added).unwrap();
        assert_eq!(freshness(&db).unwrap()[0].verdict, Verdict::StaleInput);
        save();
        std::fs::write(commands, "[{}]").unwrap();
        assert_eq!(freshness(&db).unwrap()[0].verdict, Verdict::StaleInput);
    }

    #[test]
    fn empty_body_does_not_hide_size_or_raw_boundary_changes() {
        let fixture = Fixture::new();
        let pdb = fixture.file("input.pdb", "pdb");
        let exe = fixture.file("input.exe", "exe");
        let target = fixture.0.join("target.sqlite");
        let base = fixture.0.join("base.sqlite");
        let mut t = entry("constructor");
        t.statements = serde_json::from_value(serde_json::json!([
            {"file":"unit.cpp","line":10,"off":0,"size":7},
            {"file":"unit.cpp","line":12,"off":7,"size":1}
        ]))
        .unwrap();
        let mut b = t.clone();
        b.size = 9;
        b.statements[0].size = 8;
        b.statements[1].off = 8;
        b.statements[1].line = 11;
        write_database(&target, "target", &pdb, &exe, &[t]).unwrap();
        write_database(&base, "base", &pdb, &exe, &[b]).unwrap();
        let findings = compare(&target, &base, None, None).unwrap();
        for category in ["function_size", "raw_line_records"] {
            assert_eq!(
                findings
                    .iter()
                    .find(|f| f.category == category)
                    .unwrap()
                    .verdict,
                Verdict::Mismatch
            );
        }
        for category in ["statement_structure", "statement_line_geometry"] {
            assert_eq!(
                findings
                    .iter()
                    .find(|f| f.category == category)
                    .unwrap()
                    .verdict,
                Verdict::Unobservable
            );
        }
    }

    #[test]
    fn raw_line_records_keep_equal_size_shifted_boundaries_and_closing_gaps() {
        let mut t = entry("raw");
        t.statements = serde_json::from_value(serde_json::json!([
            {"file":"unit.cpp","line":10,"off":0,"size":1},
            {"file":"unit.cpp","line":11,"off":1,"size":6},
            {"file":"unit.cpp","line":20,"off":7,"size":1}
        ]))
        .unwrap();
        let mut b = t.clone();
        b.statements[0].size = 2;
        b.statements[1].off = 2;
        b.statements[2].size = 0;
        assert_ne!(raw_line_records(&t), raw_line_records(&b));
        b = t.clone();
        b.statements[2].line = 12;
        assert_eq!(relative_statement_lines(&t), relative_statement_lines(&b));
        assert_ne!(raw_line_records(&t), raw_line_records(&b));
        b = t.clone();
        for s in &mut b.statements {
            s.line += 100;
        }
        assert_eq!(raw_line_records(&t), raw_line_records(&b));
    }

    #[test]
    fn file_provenance_is_independent_of_line_packing() {
        let mut singleton = entry("packed");
        singleton.statements = serde_json::from_value(serde_json::json!([
            {"file":"Packet.h","line":46,"off":0,"size":56}
        ]))
        .unwrap();
        let mut split = entry("packed");
        split.statements = serde_json::from_value(serde_json::json!([
            {"file":"packet.h","line":50,"off":0,"size":1},
            {"file":"packet.h","line":51,"off":1,"size":54},
            {"file":"packet.h","line":52,"off":55,"size":1}
        ]))
        .unwrap();
        assert_eq!(statement_files(&singleton), vec!["packet.h"]);
        assert_eq!(statement_files(&singleton), statement_files(&split));
        assert_ne!(
            relative_statement_lines(&singleton),
            relative_statement_lines(&split)
        );
    }

    #[test]
    fn file_provenance_preserves_returning_to_an_earlier_header() {
        let mut function = entry("switches");
        function.statements = serde_json::from_value(serde_json::json!([
            {"file":"a.h","line":10,"off":0,"size":1},
            {"file":"b.h","line":20,"off":1,"size":1},
            {"file":"b.h","line":21,"off":2,"size":1},
            {"file":"a.h","line":11,"off":3,"size":1}
        ]))
        .unwrap();
        assert_eq!(statement_files(&function), vec!["a.h", "b.h", "a.h"]);
    }

    #[test]
    fn synthetic_file_attribution_is_not_observed_provenance() {
        let fixture = Fixture::new();
        let pdb = fixture.file("input.pdb", "pdb");
        let exe = fixture.file("input.exe", "exe");
        let target = fixture.0.join("target.sqlite");
        let base = fixture.0.join("base.sqlite");
        let mut function = entry("synthetic");
        function.statements = serde_json::from_value(serde_json::json!([
            {"file":"unit.cpp","line":0,"off":0,"size":8}
        ]))
        .unwrap();
        assert!(statement_files(&function).is_empty());
        write_database(&target, "target", &pdb, &exe, &[function.clone()]).unwrap();
        write_database(&base, "base", &pdb, &exe, &[function]).unwrap();
        let findings = compare(&target, &base, None, None).unwrap();
        let finding = findings
            .iter()
            .find(|f| f.category == "statement_files")
            .unwrap();
        assert_eq!(finding.verdict, Verdict::Unobservable);
    }

    #[test]
    fn relative_lines_preserve_backwards_file_transitions() {
        let mut function = entry("lines");
        function.statements = [
            ("a.h", 100),
            ("a.h", 100),
            ("b.h", 40),
            ("a.h", 80),
            ("a.h", 101),
        ]
        .into_iter()
        .map(|(file, line)| {
            serde_json::from_value(serde_json::json!({
                "file":file,"line":line,"off":0,"size":1,"depth":0
            }))
            .unwrap()
        })
        .collect();
        assert_eq!(
            relative_statement_lines(&function),
            vec![("a.h".into(), 0), ("b.h".into(), 0), ("a.h".into(), -20)]
        );
    }
}
