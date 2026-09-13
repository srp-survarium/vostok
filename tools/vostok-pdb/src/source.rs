// SPDX-License-Identifier: GPL-3.0-or-later

//! Scoped candidate-source extraction through libclang.
//!
//! This path is explicit and cached separately from normal PDB indexing. It
//! intentionally skips function bodies: declaration shape is the evidence here.

use std::collections::HashSet;
use std::path::{Path, PathBuf};

use clang::{Accessibility, Clang, Entity, EntityKind, Index, StorageClass, diagnostic::Severity};
use serde::Deserialize;
use serde_json::json;

use crate::evidence::{self, Finding, Origin, SourceFact, Verdict};

#[derive(Deserialize)]
struct CompileCommand {
    directory: PathBuf,
    file: PathBuf,
    arguments: Vec<String>,
}

pub fn index(compile_commands: &Path, database: &Path, scopes: &[PathBuf]) -> crate::Result<()> {
    let commands: Vec<CompileCommand> = serde_json::from_slice(&std::fs::read(compile_commands)?)?;
    let scopes: Vec<PathBuf> = scopes.iter().map(absolutize).collect();
    let selected: Vec<_> = commands
        .into_iter()
        .filter(|c| {
            scopes
                .iter()
                .any(|scope| absolutize(&c.file).starts_with(scope))
        })
        .collect();
    if selected.is_empty() {
        return crate::error!("no compile command falls below the requested --scope paths");
    }

    let clang = Clang::new().map_err(|e| crate::Error::new(format!("load libclang: {e}")))?;
    let clang_index = Index::new(&clang, false, false);
    let mut facts = Vec::new();
    let mut seen = HashSet::new();
    let mut failed = 0;
    let mut external_diagnostics = 0;
    let mut overlays = Vec::new();
    for command in &selected {
        let mut args = libclang_arguments(command);
        for argument in &mut args {
            if let Some(path) = argument.strip_prefix("/clang:-ivfsoverlay") {
                let mut overlay: serde_json::Value = serde_json::from_slice(&std::fs::read(path)?)?;
                normalize_overlay_paths(&mut overlay);
                let temporary = TemporaryOverlay(
                    std::env::temp_dir()
                        .join(format!("vostok-source-{}.json", uuid::Uuid::new_v4())),
                );
                std::fs::write(&temporary.0, serde_json::to_vec(&overlay)?)?;
                *argument = format!("/clang:-ivfsoverlay{}", temporary.0.display());
                overlays.push(temporary);
            }
        }
        let old_dir = std::env::current_dir()?;
        std::env::set_current_dir(&command.directory)?;
        let parsed = clang_index
            .parser(&command.file)
            .arguments(&args)
            .skip_function_bodies(true)
            .keep_going(true)
            .parse();
        std::env::set_current_dir(old_dir)?;
        let translation_unit = match parsed {
            Ok(tu) => tu,
            Err(error) => {
                failed += 1;
                eprintln!("source index: {}: {error}", command.file.display());
                continue;
            }
        };
        let diagnostics: Vec<_> = translation_unit
            .get_diagnostics()
            .into_iter()
            .filter(|diagnostic| diagnostic.get_severity() >= Severity::Error)
            .collect();
        let scoped_diagnostics: Vec<_> = diagnostics
            .iter()
            .filter(|diagnostic| {
                let location = diagnostic.get_location().get_file_location();
                location.file.as_ref().is_some_and(|file| {
                    let path = normalize_source_path(&file.get_path());
                    scopes.iter().any(|scope| path.starts_with(scope))
                })
            })
            .collect();
        external_diagnostics += diagnostics.len() - scoped_diagnostics.len();
        if !scoped_diagnostics.is_empty() {
            failed += 1;
            for diagnostic in diagnostics.iter().take(4) {
                eprintln!("source index dependency context: {diagnostic}");
            }
            for diagnostic in scoped_diagnostics.into_iter().take(8) {
                let location = diagnostic.get_location().get_file_location();
                let file = location
                    .file
                    .map(|file| {
                        normalize_source_path(&file.get_path())
                            .display()
                            .to_string()
                    })
                    .unwrap_or_else(|| command.file.display().to_string());
                eprintln!(
                    "source index: {file}:{}:{}: {:?}: {}",
                    location.line,
                    location.column,
                    diagnostic.get_severity(),
                    diagnostic.get_text()
                );
                for child in diagnostic.get_children() {
                    eprintln!("source index note: {child}");
                }
            }
        }
        visit(
            translation_unit.get_entity(),
            &scopes,
            &command.file.display().to_string(),
            &mut seen,
            &mut facts,
        );
    }
    scan_placeholders(&scopes, &mut facts)?;
    evidence::write_source_database(
        database,
        compile_commands,
        &scopes,
        &facts,
        selected.len().saturating_sub(failed),
        failed,
        external_diagnostics,
    )?;
    eprintln!(
        "indexed {} source facts from {} translation units into {}",
        facts.len(),
        selected.len() - failed,
        database.display()
    );
    Ok(())
}

struct TemporaryOverlay(PathBuf);
impl Drop for TemporaryOverlay {
    fn drop(&mut self) {
        let _ = std::fs::remove_file(&self.0);
    }
}

fn normalize_overlay_paths(value: &mut serde_json::Value) {
    match value {
        serde_json::Value::Object(fields) => {
            for (key, child) in fields {
                if matches!(key.as_str(), "name" | "external-contents") {
                    if let Some(path) = child.as_str() {
                        let path = path.replace('\\', "/");
                        let path = if path
                            .get(..3)
                            .is_some_and(|prefix| prefix.eq_ignore_ascii_case("z:/"))
                        {
                            &path[2..]
                        } else {
                            &path
                        };
                        *child = serde_json::Value::String(path.into());
                    }
                } else {
                    normalize_overlay_paths(child);
                }
            }
        }
        serde_json::Value::Array(items) => {
            for item in items {
                normalize_overlay_paths(item);
            }
        }
        _ => (),
    }
}

fn libclang_arguments(command: &CompileCommand) -> Vec<String> {
    let mut result = vec![
        "--driver-mode=cl".into(),
        "-fno-exceptions".into(),
        "/D_STLP_DONT_USE_AUTO_LINK".into(),
        "/D_USE_ATTRIBUTES_FOR_SAL=0".into(),
    ];
    for argument in command.arguments.iter().skip(1) {
        if Path::new(argument) == command.file || argument == "/c" {
            continue;
        }
        if argument.starts_with("-imsvc")
            || argument.starts_with("/I")
            || argument.starts_with("/D")
        {
            result.push(argument.clone());
        } else if argument.starts_with("/clang:") {
            // clang-cl requires this forwarding prefix for options such as
            // -std and -ivfsoverlay; stripping it silently ignores those flags.
            result.push(argument.clone());
        } else if argument == "/TP" {
            result.push(argument.clone());
        } else if argument.starts_with('/') {
            // Optimization, warning, PCH and output switches affect compilation,
            // not the declaration AST. They are clang-cl driver syntax and are
            // rejected by libclang's direct parser API.
            continue;
        } else {
            result.push(argument.clone());
        }
    }
    result
}

fn absolutize(path: &PathBuf) -> PathBuf {
    let absolute = if path.is_absolute() {
        path.clone()
    } else {
        std::env::current_dir().unwrap_or_default().join(path)
    };
    absolute.canonicalize().unwrap_or(absolute)
}

fn normalize_source_path(path: &Path) -> PathBuf {
    let text = path.to_string_lossy().replace('\\', "/");
    let text = if text
        .get(..3)
        .is_some_and(|prefix| prefix.eq_ignore_ascii_case("z:/"))
    {
        &text[2..]
    } else {
        &text
    };
    let path = PathBuf::from(text);
    path.canonicalize().unwrap_or(path)
}

fn visit(
    entity: Entity<'_>,
    scopes: &[PathBuf],
    consumer: &str,
    seen: &mut HashSet<String>,
    facts: &mut Vec<SourceFact>,
) {
    let location = entity.get_location().map(|l| l.get_file_location());
    let in_scope = location
        .as_ref()
        .and_then(|l| l.file.as_ref())
        .is_some_and(|file| {
            let path = normalize_source_path(&file.get_path());
            scopes.iter().any(|scope| path.starts_with(scope))
        });
    if in_scope && declaration_kind(entity.get_kind()).is_some() {
        let file_location = location.unwrap();
        let file = file_location
            .file
            .map(|f| normalize_source_path(&f.get_path()).display().to_string())
            .unwrap_or_default();
        let subject = qualified_name(entity);
        let identity = entity.get_usr().map(|u| u.0).unwrap_or_else(|| {
            format!(
                "{file}:{}:{}:{subject}",
                file_location.line, file_location.column
            )
        });
        if seen.insert(identity) {
            let children = entity.get_children();
            let declaration_order = entity
                .get_lexical_parent()
                .map(|parent| {
                    parent
                        .get_children()
                        .into_iter()
                        .filter(|child| declaration_kind(child.get_kind()).is_some())
                        .position(|child| child == entity)
                        .unwrap_or(0)
                })
                .unwrap_or(0);
            facts.push(SourceFact {
                category: declaration_kind(entity.get_kind()).unwrap().into(),
                subject,
                consumer: consumer.into(),
                value: json!({
                    "kind": source_kind(entity.get_kind()),
                    "access": access(entity.get_accessibility()),
                    "static": entity.is_static_method()
                        || entity.get_storage_class() == Some(StorageClass::Static)
                        || (entity.get_kind() == EntityKind::VarDecl
                            && entity.get_semantic_parent().is_some_and(|parent|
                                declaration_kind(parent.get_kind()) == Some("type"))),
                    "virtual": entity.is_virtual_method(),
                    "pure_virtual": entity.is_pure_virtual_method(),
                    "const": entity.is_const_method(),
                    "mutable": entity.is_mutable(),
                    "definition": entity.is_definition(),
                    "type": entity.get_type().map(|t| t.get_display_name()),
                    "size": entity.get_type().and_then(|t| t.get_sizeof().ok()),
                    "result_type": entity.get_result_type().map(|t| t.get_display_name()),
                    "offset": entity.get_offset_of_field().ok().map(|bits| bits / 8),
                    "order": declaration_order,
                    "child_count": children.len(),
                    "file": file,
                    "line": file_location.line,
                    "column": file_location.column,
                }),
            });
        }
    }
    for child in entity.get_children() {
        visit(child, scopes, consumer, seen, facts);
    }
}

fn declaration_kind(kind: EntityKind) -> Option<&'static str> {
    match kind {
        EntityKind::StructDecl
        | EntityKind::ClassDecl
        | EntityKind::UnionDecl
        | EntityKind::ClassTemplate
        | EntityKind::ClassTemplatePartialSpecialization => Some("type"),
        EntityKind::FieldDecl | EntityKind::VarDecl => Some("field"),
        EntityKind::Method
        | EntityKind::Constructor
        | EntityKind::Destructor
        | EntityKind::ConversionFunction
        | EntityKind::FunctionDecl
        | EntityKind::FunctionTemplate => Some("function"),
        EntityKind::BaseSpecifier => Some("base"),
        EntityKind::EnumDecl | EntityKind::TypedefDecl | EntityKind::TypeAliasDecl => {
            Some("declaration")
        }
        _ => None,
    }
}

fn source_kind(kind: EntityKind) -> &'static str {
    match kind {
        EntityKind::StructDecl => "struct",
        EntityKind::ClassDecl
        | EntityKind::ClassTemplate
        | EntityKind::ClassTemplatePartialSpecialization => "class",
        EntityKind::UnionDecl => "union",
        EntityKind::FieldDecl => "field",
        EntityKind::VarDecl => "static-field",
        EntityKind::Method
        | EntityKind::Constructor
        | EntityKind::Destructor
        | EntityKind::ConversionFunction
        | EntityKind::FunctionDecl
        | EntityKind::FunctionTemplate => "method",
        EntityKind::BaseSpecifier => "base",
        EntityKind::EnumDecl => "nested-type",
        EntityKind::TypedefDecl | EntityKind::TypeAliasDecl => "nested-type",
        _ => "declaration",
    }
}

fn access(value: Option<Accessibility>) -> &'static str {
    match value {
        Some(Accessibility::Public) => "public",
        Some(Accessibility::Protected) => "protected",
        Some(Accessibility::Private) => "private",
        None => "none",
    }
}

fn qualified_name(entity: Entity<'_>) -> String {
    let name = entity
        .get_name()
        .or_else(|| entity.get_display_name())
        .unwrap_or_default();
    let mut parents = Vec::new();
    let mut parent = entity.get_semantic_parent();
    while let Some(value) = parent {
        if matches!(
            value.get_kind(),
            EntityKind::Namespace
                | EntityKind::StructDecl
                | EntityKind::ClassDecl
                | EntityKind::UnionDecl
                | EntityKind::ClassTemplate
        ) {
            if let Some(parent_name) = value.get_name() {
                if !parent_name.is_empty() {
                    parents.push(parent_name);
                }
            }
        }
        parent = value.get_semantic_parent();
    }
    parents.reverse();
    if entity.get_kind() == EntityKind::BaseSpecifier {
        let derived = entity
            .get_lexical_parent()
            .and_then(|parent| parent.get_name())
            .unwrap_or_default();
        if parents.last() != Some(&derived) && !derived.is_empty() {
            parents.push(derived);
        }
    }
    parents.push(name);
    parents.join("::")
}

fn scan_placeholders(scopes: &[PathBuf], facts: &mut Vec<SourceFact>) -> crate::Result<()> {
    for scope in scopes {
        for entry in walkdir(scope)? {
            let text = match std::fs::read_to_string(&entry) {
                Ok(text) => text,
                Err(_) => continue,
            };
            for (line, source) in text.lines().enumerate() {
                for marker in ["/* no source */", "STATE[STUB]", "STATE[REMOVED]"] {
                    if source.contains(marker) {
                        facts.push(SourceFact {
                            category: "placeholder".into(),
                            subject: format!("{}:{}", entry.display(), line + 1),
                            value: json!({"marker": marker, "source": source.trim()}),
                            consumer: entry.display().to_string(),
                        });
                    }
                }
            }
        }
    }
    Ok(())
}

fn walkdir(root: &Path) -> crate::Result<Vec<PathBuf>> {
    let mut pending = vec![root.to_path_buf()];
    let mut files = Vec::new();
    while let Some(path) = pending.pop() {
        if path.is_dir() {
            for entry in std::fs::read_dir(path)? {
                pending.push(entry?.path());
            }
        } else if matches!(
            path.extension().and_then(|s| s.to_str()),
            Some("h" | "hpp" | "cpp" | "inl")
        ) {
            files.push(path);
        }
    }
    Ok(files)
}

pub fn compare(
    target: &Path,
    candidate: &Path,
    filter: Option<&str>,
) -> crate::Result<Vec<Finding>> {
    let mut freshness = evidence::freshness(target)?;
    freshness.extend(evidence::freshness(candidate)?);
    if !freshness.is_empty() {
        return Ok(freshness);
    }
    let target_facts = evidence::load_facts_prefix(target, filter)?;
    let mut candidate_facts = evidence::load_facts_prefix(candidate, filter)?;
    candidate_facts.extend(
        evidence::load_facts(candidate, None)?
            .into_iter()
            .filter(|fact| fact.category == "placeholder"),
    );
    if target_facts.is_empty() {
        return Ok(vec![Finding { category: "declarations".into(),
            subject: filter.unwrap_or("*").into(), verdict: Verdict::Unsupported,
            origin: Origin::Observed,
            detail: "target database has no normalized declaration facts; re-index with topology extraction".into() }]);
    }
    let mut findings = Vec::new();
    for (channel, _observed, skipped, unsupported, detail) in evidence::coverage(candidate)? {
        if skipped != 0 || unsupported != 0 {
            findings.push(Finding {
                category: "coverage".into(),
                subject: channel,
                verdict: Verdict::Unsupported,
                origin: Origin::Observed,
                detail: format!("skipped={skipped} unsupported={unsupported}: {detail}"),
            });
        }
    }
    for candidate in candidate_facts
        .iter()
        .filter(|fact| fact.category == "placeholder")
    {
        findings.push(Finding {
            category: "placeholder".into(),
            subject: candidate.subject.clone(),
            verdict: Verdict::Mismatch,
            origin: Origin::Observed,
            detail: candidate.value.to_string(),
        });
    }
    let mut target_groups = std::collections::BTreeMap::<_, Vec<_>>::new();
    let mut candidate_groups = std::collections::BTreeMap::<_, Vec<_>>::new();
    for fact in &target_facts {
        target_groups
            .entry((fact.category.as_str(), fact.subject.as_str()))
            .or_default()
            .push(fact);
    }
    for fact in &candidate_facts {
        candidate_groups
            .entry((fact.category.as_str(), fact.subject.as_str()))
            .or_default()
            .push(fact);
    }
    let target_types: std::collections::BTreeSet<_> = target_facts
        .iter()
        .filter(|fact| fact.category == "type")
        .map(|fact| fact.subject.as_str())
        .collect();
    let candidate_types: std::collections::BTreeSet<_> = candidate_facts
        .iter()
        .filter(|fact| fact.category == "type")
        .map(|fact| fact.subject.as_str())
        .collect();
    for ((category, subject), target_variants) in target_groups {
        let candidates = candidate_groups
            .get(&(category, subject))
            .cloned()
            .unwrap_or_default();
        let comparisons: Vec<_> = target_variants
            .iter()
            .flat_map(|target| {
                candidates
                    .iter()
                    .map(move |candidate| compare_fact(target, candidate))
            })
            .collect();
        let target_values: std::collections::BTreeSet<_> = target_variants
            .iter()
            .map(|fact| fact.value.to_string())
            .collect();
        let target_nested_type_exists =
            category == "declaration" && candidate_types.contains(subject);
        let target_is_anonymous = subject.ends_with("::<unnamed-tag>");
        let target_nested_in_candidate_type = category == "declaration"
            && owning_type(subject, &target_types)
                .is_some_and(|owner| candidate_types.contains(owner));
        let (verdict, detail) = if candidates.is_empty() && target_nested_type_exists {
            (
                Verdict::Match,
                "target nested-type record is represented by the candidate nested type definition"
                    .into(),
            )
        } else if candidates.is_empty() && target_is_anonymous {
            (
                Verdict::Unobservable,
                "anonymous nested declarations cannot be paired by source identity".into(),
            )
        } else if candidates.is_empty() && target_nested_in_candidate_type {
            (
                Verdict::TargetOnly,
                "target nested declaration is absent from a candidate class that was parsed in scope"
                    .into(),
            )
        } else if candidates.is_empty() {
            (
                Verdict::Unobservable,
                "candidate declaration absent or not materialized by the scoped AST".into(),
            )
        } else if comparisons
            .iter()
            .any(|(verdict, _)| *verdict == Verdict::Match)
        {
            if target_values.len() > 1 || candidates.len() > 1 {
                (
                    Verdict::Ambiguous,
                    format!(
                        "{} target variants and {} candidate declarations; at least one qualifier projection matches",
                        target_values.len(),
                        candidates.len()
                    ),
                )
            } else {
                comparisons
                    .iter()
                    .find(|(verdict, _)| *verdict == Verdict::Match)
                    .cloned()
                    .unwrap()
            }
        } else if comparisons
            .iter()
            .all(|(verdict, _)| *verdict == Verdict::Unobservable)
        {
            comparisons.into_iter().next().unwrap()
        } else {
            comparisons
                .into_iter()
                .find(|(verdict, _)| *verdict == Verdict::Mismatch)
                .unwrap()
        };
        findings.push(Finding {
            category: category.into(),
            subject: subject.into(),
            verdict,
            origin: Origin::Observed,
            detail,
        });
    }
    for ((category, subject), candidates) in &candidate_groups {
        if *category != "declaration"
            || subject.contains("(unnamed ")
            || target_facts
                .iter()
                .any(|fact| fact.category == *category && fact.subject == *subject)
        {
            continue;
        }
        if owning_type(subject, &candidate_types).is_some_and(|owner| target_types.contains(owner))
        {
            findings.push(Finding {
                category: (*category).into(),
                subject: (*subject).into(),
                verdict: Verdict::BaseOnly,
                origin: Origin::Observed,
                detail: format!(
                    "{} candidate nested declaration(s) absent from the target type stream",
                    candidates.len()
                ),
            });
        }
    }
    findings.extend(compare_declaration_order(&target_facts, &candidate_facts));
    for candidate in &candidate_facts {
        if candidate.category == "function"
            && candidate
                .value
                .get("pure_virtual")
                .and_then(serde_json::Value::as_bool)
                == Some(true)
        {
            findings.push(Finding {
                category: "pure_virtual".into(),
                subject: candidate.subject.clone(),
                verdict: Verdict::Unobservable,
                origin: Origin::Observed,
                detail: "candidate is pure virtual; this MSVC CodeView method record encodes its virtual slot but not purity reliably".into(),
            });
        }
        if candidate.category == "field"
            && candidate
                .value
                .get("mutable")
                .and_then(serde_json::Value::as_bool)
                == Some(true)
        {
            findings.push(Finding {
                category: "mutability".into(),
                subject: candidate.subject.clone(),
                verdict: Verdict::Unobservable,
                origin: Origin::Observed,
                detail: "candidate is mutable; CodeView field records do not encode C++ mutable"
                    .into(),
            });
        }
    }
    Ok(findings)
}

fn compare_fact(target: &SourceFact, candidate: &SourceFact) -> (Verdict, String) {
    let keys: &[&str] = match target.category.as_str() {
        "type" => &["kind"],
        "field" => &["access", "static", "offset"],
        "function" => &["access", "static", "virtual", "const"],
        "base" => &["access"],
        "declaration" => &["kind"],
        _ => &["access"],
    };
    let comparable: Vec<_> = keys
        .iter()
        .filter_map(|key| {
            let expected = target.value.get(*key)?;
            if expected.is_null() || (*key == "access" && expected.as_str() == Some("unspecified"))
            {
                return None;
            }
            let actual = candidate.value.get(*key);
            if actual.is_none_or(serde_json::Value::is_null) {
                return None;
            }
            Some((*key, expected, actual))
        })
        .collect();
    if comparable.is_empty() {
        return (
            Verdict::Unobservable,
            "PDB exposes no directly comparable property for this declaration".into(),
        );
    }
    let differences: Vec<String> = comparable
        .iter()
        .filter(|(_, expected, actual)| actual.as_ref() != Some(expected))
        .map(|(key, expected, actual)| {
            format!(
                "{key}: target={expected} candidate={}",
                actual.unwrap_or(&serde_json::Value::Null)
            )
        })
        .collect();
    if differences.is_empty() {
        (
            Verdict::Match,
            format!(
                "observed {}",
                comparable
                    .iter()
                    .map(|(key, _, _)| *key)
                    .collect::<Vec<_>>()
                    .join(",")
            ),
        )
    } else {
        (Verdict::Mismatch, differences.join("; "))
    }
}

fn owning_type<'a>(subject: &str, types: &'a std::collections::BTreeSet<&str>) -> Option<&'a str> {
    types
        .iter()
        .copied()
        .filter(|owner| {
            subject.len() > owner.len()
                && subject.starts_with(*owner)
                && subject.as_bytes().get(owner.len()..owner.len() + 2) == Some(b"::")
        })
        .max_by_key(|owner| owner.len())
}

fn compare_declaration_order(target: &[SourceFact], candidate: &[SourceFact]) -> Vec<Finding> {
    use std::collections::{BTreeMap, BTreeSet};

    fn positions(facts: &[SourceFact]) -> BTreeMap<String, BTreeMap<String, BTreeSet<u64>>> {
        let mut result: BTreeMap<String, BTreeMap<String, BTreeSet<u64>>> = BTreeMap::new();
        for fact in facts.iter().filter(|fact| {
            matches!(
                fact.category.as_str(),
                "field" | "function" | "base" | "declaration"
            )
        }) {
            let Some((owner, _)) = fact.subject.rsplit_once("::") else {
                continue;
            };
            let Some(order) = fact.value.get("order").and_then(serde_json::Value::as_u64) else {
                continue;
            };
            result
                .entry(owner.into())
                .or_default()
                .entry(fact.subject.clone())
                .or_default()
                .insert(order);
        }
        result
    }

    let target = positions(target);
    let candidate = positions(candidate);
    let mut findings = Vec::new();
    for (owner, target_members) in target {
        let Some(candidate_members) = candidate.get(&owner) else {
            continue;
        };
        let common: Vec<_> = target_members
            .iter()
            .filter_map(|(subject, target_order)| {
                let candidate_order = candidate_members.get(subject)?;
                (target_order.len() == 1 && candidate_order.len() == 1).then(|| {
                    (
                        subject,
                        *target_order.first().unwrap(),
                        *candidate_order.first().unwrap(),
                    )
                })
            })
            .collect();
        if common.len() < 2 {
            continue;
        }
        let mut inversions = Vec::new();
        for left in 0..common.len() {
            for right in left + 1..common.len() {
                let target_cmp = common[left].1.cmp(&common[right].1);
                let candidate_cmp = common[left].2.cmp(&common[right].2);
                if target_cmp != candidate_cmp {
                    inversions.push(format!("{} <> {}", common[left].0, common[right].0));
                }
            }
        }
        findings.push(Finding {
            category: "declaration_order".into(),
            subject: owner,
            verdict: if inversions.is_empty() {
                Verdict::Match
            } else {
                Verdict::Mismatch
            },
            origin: Origin::Observed,
            detail: if inversions.is_empty() {
                format!("{} common uniquely-positioned declarations", common.len())
            } else {
                format!(
                    "{} relative-order inversions: {}",
                    inversions.len(),
                    inversions.join(", ")
                )
            },
        });
    }
    findings
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn clang_cl_forwarded_dialect_and_overlay_flags_are_not_stripped() {
        let command = CompileCommand {
            directory: PathBuf::from("/work"),
            file: PathBuf::from("/work/unit.cpp"),
            arguments: [
                "clang-cl",
                "/clang:-std=c++98",
                "/clang:-ivfsoverlay/work/overlay.json",
                "/c",
                "/work/unit.cpp",
            ]
            .into_iter()
            .map(str::to_string)
            .collect(),
        };
        let args = libclang_arguments(&command);
        assert!(args.contains(&"/clang:-std=c++98".into()));
        assert!(args.contains(&"/clang:-ivfsoverlay/work/overlay.json".into()));
        assert!(!args.contains(&"/c".into()));
    }

    #[test]
    fn overlay_paths_map_wine_roots_and_preserve_case_insensitivity() {
        let mut overlay = json!({"version":0,"case-sensitive":"false","roots":[{
            "name":"Z:\\work\\sources","type":"directory","contents":[{
                "name":"FastDelegate.h","type":"file","external-contents":"Z:\\work\\sources\\FastDelegate.h"
            }]
        }]});
        normalize_overlay_paths(&mut overlay);
        assert_eq!(overlay["roots"][0]["name"], "/work/sources");
        assert_eq!(
            overlay["roots"][0]["contents"][0]["external-contents"],
            "/work/sources/FastDelegate.h"
        );
        assert_eq!(overlay["case-sensitive"], "false");
    }

    fn fact(category: &str, value: serde_json::Value) -> SourceFact {
        SourceFact {
            category: category.into(),
            subject: "vostok::sample::member".into(),
            value,
            consumer: String::new(),
        }
    }

    #[test]
    fn qualifier_comparison_ignores_source_only_location_metadata() {
        let target = fact(
            "function",
            json!({"access":"private", "static":true, "virtual":false,
                   "pure_virtual":false, "const":true, "type":"void () const",
                   "order":3}),
        );
        let candidate = fact(
            "function",
            json!({"access":"private", "static":true, "virtual":false,
                   "pure_virtual":false, "const":true, "type":"void () const",
                   "order":3, "file":"sample.h", "line":44}),
        );
        assert_eq!(compare_fact(&target, &candidate).0, Verdict::Match);
    }

    #[test]
    fn qualifier_comparison_reports_access_and_static_mismatches() {
        let target = fact(
            "field",
            json!({"access":"private", "static":true, "type":"long", "order":1}),
        );
        let candidate = fact(
            "field",
            json!({"access":"public", "static":false, "type":"long", "order":1}),
        );
        let (verdict, detail) = compare_fact(&target, &candidate);
        assert_eq!(verdict, Verdict::Mismatch);
        assert!(detail.contains("access"));
        assert!(detail.contains("static"));
    }
}
