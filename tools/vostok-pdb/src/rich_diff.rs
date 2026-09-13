//! Base↔target diff, computed on the *normalized instruction text* before any
//! offset/size/source metadata is attached (per the matching spec: "diff should
//! be done before metadata is appended").
//!
//! This is the simple built-in backend: an LCS alignment of the two instruction
//! streams producing an objdiff-style op sequence (`Equal | Delete | Insert`),
//! plus a match ratio for the retry-budget signal. A heavier `objdiff-core`
//! backend (operand/relocation-aware) is tracked separately; this one needs no
//! object files and a byte-identical (matched) function diffs to all-`Equal`.

use std::fmt::Write as _;

use crate::rich_context::FunctionEntry;

#[derive(Clone, Copy, PartialEq, Eq)]
pub enum Op {
    /// Same instruction text in both.
    Equal,
    /// Present in base only (must be removed to reach target).
    Delete,
    /// Present in target only (must be added to reach target).
    Insert,
}

pub struct DiffLine {
    pub op: Op,
    /// Index into `base.instructions` (set for `Equal`/`Delete`).
    pub base: Option<usize>,
    /// Index into `target.instructions` (set for `Equal`/`Insert`).
    pub target: Option<usize>,
}

pub struct Diff {
    pub lines: Vec<DiffLine>,
    /// Instructions equal in both streams.
    pub matched: usize,
    /// Distinct aligned slots (Equal + Delete + Insert).
    pub total: usize,
}

impl Diff {
    /// 0.0 = nothing matches, 1.0 = identical. The retry budget watches this
    /// shrink/grow across attempts.
    pub fn ratio(&self) -> f64 {
        if self.total == 0 {
            1.0
        } else {
            self.matched as f64 / self.total as f64
        }
    }
}

/// Align `base` vs `target` instruction streams by their text via LCS.
pub fn diff(base: &FunctionEntry, target: &FunctionEntry) -> Diff {
    let b: Vec<&str> = base.instructions.iter().map(|i| i.text.as_str()).collect();
    let t: Vec<&str> = target
        .instructions
        .iter()
        .map(|i| i.text.as_str())
        .collect();
    let (n, m) = (b.len(), t.len());

    // dp[i][j] = LCS length of b[i..] and t[j..].
    let mut dp = vec![vec![0u32; m + 1]; n + 1];
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            dp[i][j] = if b[i] == t[j] {
                dp[i + 1][j + 1] + 1
            } else {
                dp[i + 1][j].max(dp[i][j + 1])
            };
        }
    }

    let mut lines = Vec::new();
    let mut matched = 0;
    let (mut i, mut j) = (0, 0);
    while i < n && j < m {
        if b[i] == t[j] {
            lines.push(DiffLine {
                op: Op::Equal,
                base: Some(i),
                target: Some(j),
            });
            matched += 1;
            i += 1;
            j += 1;
        } else if dp[i + 1][j] >= dp[i][j + 1] {
            lines.push(DiffLine {
                op: Op::Delete,
                base: Some(i),
                target: None,
            });
            i += 1;
        } else {
            lines.push(DiffLine {
                op: Op::Insert,
                base: None,
                target: Some(j),
            });
            j += 1;
        }
    }
    while i < n {
        lines.push(DiffLine {
            op: Op::Delete,
            base: Some(i),
            target: None,
        });
        i += 1;
    }
    while j < m {
        lines.push(DiffLine {
            op: Op::Insert,
            base: None,
            target: Some(j),
        });
        j += 1;
    }

    let total = lines.len();
    Diff {
        lines,
        matched,
        total,
    }
}

/// Render a git-style unified view. `Equal` lines carry no offset (it differs
/// between the two builds and is metadata); changed lines show the offset on
/// their own side for cross-reference. A trailing summary line carries the match
/// ratio.
pub fn render_unified(base: &FunctionEntry, target: &FunctionEntry, d: &Diff) -> String {
    let mut out = String::new();
    let _ = writeln!(out, "--- base    {}", base.name);
    let _ = writeln!(out, "+++ target  {}", target.name);

    for line in &d.lines {
        match line.op {
            Op::Equal => {
                let bi = &base.instructions[line.base.unwrap()];
                let _ = writeln!(out, "  {}", bi.text);
            }
            Op::Delete => {
                let bi = &base.instructions[line.base.unwrap()];
                let _ = writeln!(out, "- 0x{:02x}: {}", bi.off, bi.text);
            }
            Op::Insert => {
                let ti = &target.instructions[line.target.unwrap()];
                let _ = writeln!(out, "+ 0x{:02x}: {}", ti.off, ti.text);
            }
        }
    }

    let _ = writeln!(
        out,
        "; {}/{} instructions equal ({:.1}%)",
        d.matched,
        d.total,
        d.ratio() * 100.0
    );
    out
}
