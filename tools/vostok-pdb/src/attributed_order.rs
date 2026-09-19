// SPDX-License-Identifier: GPL-3.0-or-later

//! Relative order of observed source-line attributions, including headers.
//! This is not physical definition order: #line and optimized attribution can
//! change it independently of syntax or compiland procedure-symbol order.

use std::collections::{BTreeMap, HashMap};
use std::path::Path;

use crate::evidence::{self, Finding, Origin, Verdict};
use crate::rich_context::FunctionEntry;

fn normalized(file: &str) -> String {
    file.replace('\\', "/").to_lowercase()
}

fn own_line(entry: &FunctionEntry) -> Option<u32> {
    let file = normalized(&entry.file);
    if file.is_empty() {
        return None;
    }
    let rows: Vec<_> = entry.statements.iter().filter(|s| s.line > 0).collect();
    if rows.iter().any(|s| {
        normalized(&s.file) != file
            || s.off > entry.size
            || s.size > entry.size.saturating_sub(s.off)
    }) {
        return None;
    }
    rows.iter().map(|s| s.line).min()
}

fn multiplicities(entries: &[FunctionEntry]) -> (HashMap<&str, usize>, HashMap<u32, usize>) {
    let mut names = HashMap::new();
    let mut addresses = HashMap::new();
    for entry in entries {
        *names.entry(entry.name.as_str()).or_insert(0) += 1;
        *addresses.entry(entry.rva).or_insert(0) += 1;
    }
    (names, addresses)
}

pub fn compare(
    target: &Path,
    base: &Path,
    function: Option<&str>,
    file: Option<&str>,
) -> crate::Result<Vec<Finding>> {
    let mut stale = evidence::freshness(target)?;
    stale.extend(evidence::freshness(base)?);
    if !stale.is_empty() {
        return Ok(stale);
    }
    let targets = evidence::search(target, None, None, None, None)?;
    let bases = evidence::search(base, None, None, None, None)?;
    Ok(compare_entries(&targets, &bases, function, file))
}

fn compare_entries(
    targets: &[FunctionEntry],
    bases: &[FunctionEntry],
    function: Option<&str>,
    file: Option<&str>,
) -> Vec<Finding> {
    let (target_names, target_addresses) = multiplicities(targets);
    let (base_names, base_addresses) = multiplicities(bases);
    let by_name: HashMap<_, _> = bases.iter().map(|e| (e.name.as_str(), e)).collect();
    let mut files = BTreeMap::<String, Vec<(&str, u32, u32)>>::new();
    let mut findings = Vec::new();
    let mut selected = 0;
    for t in targets {
        if file.is_some_and(|f| !normalized(&t.file).contains(&normalized(f)))
            || function.is_some_and(|f| {
                t.mangled != f && !t.name.to_lowercase().contains(&f.to_lowercase())
            })
        {
            continue;
        }
        selected += 1;
        let path = normalized(&t.file);
        files.entry(path.clone()).or_default();
        let b = by_name.get(t.name.as_str()).copied();
        let excluded = if target_names[t.name.as_str()] != 1
            || base_names.get(t.name.as_str()).is_some_and(|n| *n != 1)
        {
            Some((Verdict::Ambiguous, "duplicate full-signature records"))
        } else if target_addresses[&t.rva] != 1 || b.is_some_and(|b| base_addresses[&b.rva] != 1) {
            Some((
                Verdict::Ambiguous,
                "shared-RVA attribution; folded aliases excluded",
            ))
        } else if b.is_none() {
            Some((Verdict::Unobservable, "no exact full-signature candidate"))
        } else if b.is_some_and(|b| normalized(&b.file) != path) {
            Some((Verdict::Unobservable, "procedure file differs"))
        } else if own_line(t).is_none() || b.is_some_and(|b| own_line(b).is_none()) {
            Some((
                Verdict::Unobservable,
                "missing, out-of-bounds or mixed-file positive line evidence",
            ))
        } else {
            None
        };
        if let Some((verdict, detail)) = excluded {
            findings.push(Finding {
                category: "attributed_line_order_coverage".into(),
                subject: t.name.clone(),
                verdict,
                origin: Origin::Observed,
                detail: format!("{path}: excluded: {detail}"),
            });
            continue;
        }
        let b = b.unwrap();
        files
            .get_mut(&path)
            .unwrap()
            .push((&t.name, own_line(t).unwrap(), own_line(b).unwrap()));
    }
    if selected == 0 {
        findings.push(Finding {
            category: "attributed_line_order_coverage".into(),
            subject: file.or(function).unwrap_or("all procedures").into(),
            verdict: Verdict::Unobservable,
            origin: Origin::Observed,
            detail: "no selected target procedures".into(),
        });
    }
    for (file, mut entries) in files {
        entries.sort_unstable();
        let mut comparable = 0;
        let mut tied = 0;
        let mut inverted = 0;
        for (i, &(a, at, ab)) in entries.iter().enumerate() {
            for &(b, bt, bb) in &entries[i + 1..] {
                if at == bt || ab == bb {
                    tied += 1;
                    continue;
                }
                comparable += 1;
                if (at < bt) != (ab < bb) {
                    inverted += 1;
                    findings.push(Finding {
                        category: "attributed_line_order".into(),
                        subject: format!("{a} <> {b}"),
                        verdict: Verdict::Mismatch,
                        origin: Origin::Correlated,
                        detail: format!("{file}: target lines {at}/{bt}; base lines {ab}/{bb}; attributed-line inversion, not proof of physical definition order"),
                    });
                }
            }
        }
        findings.push(Finding {
            category: "attributed_line_order_coverage".into(),
            subject: file,
            verdict: if comparable == 0 || tied > 0 {
                Verdict::Unobservable
            } else if inverted > 0 {
                Verdict::Mismatch
            } else {
                Verdict::Match
            },
            origin: Origin::Observed,
            detail: format!("{} unique paired procedures; {comparable} comparable pairs; {tied} tied pairs excluded; {inverted} inversions; subset only, see per-procedure exclusions; no syntax-order claim", entries.len()),
        });
    }
    findings
}

#[cfg(test)]
mod tests {
    use super::*;

    fn entry(name: &str, rva: u32, line: u32) -> FunctionEntry {
        serde_json::from_value(serde_json::json!({
            "name":name, "mangled":name, "rva":rva, "size":8, "file":"inline.h",
            "statements":[{"off":0,"size":8,"line":line,"file":"inline.h"}],
            "instructions":[]
        }))
        .unwrap()
    }

    #[test]
    fn singleton_header_inversion_is_reported_with_both_lines() {
        let target = [entry("a", 1, 10), entry("b", 2, 20)];
        let base = [entry("a", 3, 30), entry("b", 4, 15)];
        let f = compare_entries(&target, &base, None, None);
        let inversion = f
            .iter()
            .find(|f| f.category == "attributed_line_order")
            .unwrap();
        assert_eq!(inversion.verdict, Verdict::Mismatch);
        assert!(
            inversion
                .detail
                .contains("target lines 10/20; base lines 30/15")
        );
    }

    #[test]
    fn borrowed_out_of_bounds_line_records_are_not_order_evidence() {
        let mut a = entry("a", 1, 10);
        a.statements[0].off = u32::MAX;
        let entries = [a, entry("b", 2, 20)];
        let findings = compare_entries(&entries, &entries, None, None);
        assert!(
            findings
                .iter()
                .any(|f| f.subject == "a" && f.verdict == Verdict::Unobservable)
        );
        assert!(!findings.iter().any(|f| f.verdict == Verdict::Match));
    }

    #[test]
    fn ties_and_singletons_cannot_certify_order() {
        for entries in [
            vec![entry("a", 1, 10)],
            vec![entry("a", 1, 10), entry("b", 2, 10)],
        ] {
            let findings = compare_entries(&entries, &entries, None, None);
            assert!(findings.iter().all(|f| f.verdict == Verdict::Unobservable));
        }
    }

    #[test]
    fn folds_outside_scope_still_make_attribution_ambiguous() {
        let mut alias = entry("alias", 1, 80);
        alias.file = "outside.h".into();
        let entries = [entry("a", 1, 10), alias, entry("b", 2, 20)];
        let f = compare_entries(&entries, &entries, None, Some("inline.h"));
        assert!(
            f.iter()
                .any(|f| f.subject == "a" && f.verdict == Verdict::Ambiguous)
        );
        assert!(!f.iter().any(|f| f.verdict == Verdict::Match));
    }

    #[test]
    fn missing_zero_mixed_and_duplicate_evidence_stays_visible() {
        let mut mixed = entry("mixed", 2, 20);
        mixed.statements[0].file = "other.h".into();
        let target = [
            entry("zero", 1, 0),
            mixed,
            entry("missing", 3, 30),
            entry("dup", 4, 40),
        ];
        let mut base = target[..2].to_vec();
        base.extend([entry("dup", 5, 40), entry("dup", 6, 40)]);
        let f = compare_entries(&target, &base, None, None);
        assert_eq!(f.iter().filter(|f| f.subject != "inline.h").count(), 4);
        assert!(
            f.iter()
                .any(|f| f.subject == "dup" && f.verdict == Verdict::Ambiguous)
        );
        assert!(!f.iter().any(|f| f.verdict == Verdict::Match));
    }

    #[test]
    fn shifted_lines_preserve_relative_order_and_empty_selection_is_unobservable() {
        let target = [entry("a", 1, 10), entry("b", 2, 20)];
        let base = [entry("a", 3, 100), entry("b", 4, 200)];
        let f = compare_entries(&target, &base, None, None);
        assert_eq!(f.len(), 1);
        assert_eq!(f[0].verdict, Verdict::Match);
        let f = compare_entries(&target, &base, None, Some("absent"));
        assert_eq!(f[0].verdict, Verdict::Unobservable);
    }
}
