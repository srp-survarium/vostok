# SPDX-License-Identifier: GPL-3.0-or-later

"""history - when a ledger row was at its peak, and which commit lost it.

Read-only: these verbs read the committed history of `config/match_state.tsv`
(and of `docs/binary_matching/match_state.tsv`, where it lived until
2026-08-30) and the source tree. They never build and never write the ledger.

  vostok ledger hist-gap [--module M] [--unit U] [--min-gap X] [--exact]
                         [--sort recoverable|gap|drop] [--limit N] [--json]
      every row whose `hist` exceeds `max`: the PEAK commit where the row first
      reached its `hist`, and the DROP commit after which it stayed below it
      (with the score it fell to and whether its body `hash` changed there).
      Sorted by recoverable bytes, size * (hist - max).

  vostok ledger last-exact FUNCTION [--diff] [--limit N] [--json]
      the last commit where the function was exact (cur or max at 100), the
      commit that lost it, the commits that touched its source file since,
      and with --diff the function's source then against HEAD: "here is the
      source that matched".

A ledger commit records what was measured on the source it was committed with,
so the change that lost a peak lies in the DROP commit (or, for a peak lost in
a reverted experiment, just before it).

How the history is read. Rows are tracked through the ledger's blobs, not by
replaying `git log -p`: the walk is `git log --topo-order` over every commit
that changed the file (history simplification keeps the PR commits and drops
the merges that only carried them), each commit maps to the blob it left, and
each blob's rows are its first parent's blob's rows plus one parsed blob-to-blob
`git diff -U0`. The parsed deltas are cached under `binaries/ledger_history/`
(see `paths.LEDGER_HISTORY_CACHE`) keyed by the two blob ids, so a delta is
computed once per clone and shared by every worktree; the uncached diffs run in
parallel. A cold walk over the ~700 commits takes 15-25 s, a warm one about
one.
"""

from __future__ import annotations

import argparse
import difflib
import json
import os
import pickle
import re
import sqlite3
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
from pathlib import Path

from vostok.core import paths
from vostok.ledger import store

TOLERANCE = 1e-3  # the ledger stores 4 decimals; older revisions rounded too
EXACT = store.EXACT
CACHE_VERSION = 2
NULL_BLOB = "0" * 40

# A row as the history keeps it: (cur, max, hist, hash). cur/max may be None.
COLUMNS = ("cur", "max", "hist", "hash")


def _log(message):
    print(f"[ledger] {message}", file=sys.stderr)


def _git(repo, *args, check=True):
    result = subprocess.run(["git", *args], cwd=repo, capture_output=True,
                            text=True, errors="replace", check=False)
    if check and result.returncode:
        raise SystemExit(f"git {' '.join(args[:3])}: {result.stderr.strip()}")
    return result


# --- the commits that touched the ledger ------------------------------------


@dataclass
class Commit:
    sha: str
    date: str
    subject: str
    parents: tuple = ()
    blob: str | None = None   # the ledger blob this commit leaves
    src: str | None = None    # the first parent's blob, when the commit changed it

    @property
    def short(self):
        return self.sha[:10]

    def label(self):
        return f"{self.short} {self.date} {self.subject}"


def parse_log(text):
    """Parse `git log --raw --parents --format=%x01%H%x02%P%x02%ad%x02%s`
    output into Commits (in the order given), resolving the ledger blob each
    leaves: the raw line's destination blob, else - for a merge whose first
    parent already had this ledger - that parent's blob."""
    commits = []
    by_sha = {}
    for chunk in text.split("\x01")[1:]:
        head, _, rest = chunk.partition("\n")
        sha, parent_text, date, subject = (head.split("\x02") + ["", "", ""])[:4]
        commit = Commit(sha, date, subject, tuple(parent_text.split()))
        src = dst = None
        changed = False
        for line in rest.splitlines():
            if not line.startswith(":"):
                continue
            meta = line[1:].split("\t", 1)[0].split()
            if len(meta) < 4:
                continue
            changed = True
            if meta[2] != NULL_BLOB:
                src = src or meta[2]
            if meta[3] != NULL_BLOB:
                dst = meta[3]
        if changed:
            commit.blob, commit.src = dst, src
        elif commit.parents and commit.parents[0] in by_sha:
            commit.blob = by_sha[commit.parents[0]].blob
        commits.append(commit)
        by_sha[sha] = commit
    return commits


def ledger_paths():
    return [Path(p).relative_to(paths.REPO).as_posix()
            for p in (paths.MATCH_STATE, paths.LEGACY_MATCH_STATE)]


def load_commits(repo=None, rev="HEAD", files=None):
    repo = repo or paths.REPO
    files = files or ledger_paths()
    out = _git(repo, "log", "--topo-order", "--reverse", "--parents", "--raw",
               "--no-abbrev", "--diff-merges=first-parent", "--date=short",
               "--format=%x01%H%x02%P%x02%ad%x02%s", rev, "--", *files).stdout
    return parse_log(out)


# --- parsing ledger text ----------------------------------------------------


def _num(text):
    if text in (None, "", "-"):
        return None
    try:
        return float(text)
    except ValueError:
        return None


def _header(line):
    return tuple(line.rstrip("\r\n").split("\t"))


def _is_header(line):
    """The column header - not the stray data row some revisions keyed
    `mangled` with empty columns."""
    fields = line.split("\t")
    return fields[0] == "mangled" and "hist" in fields and "cur" in fields


def _row(fields, index):
    def get(name):
        i = index.get(name)
        return fields[i] if i is not None and i < len(fields) else None
    return (_num(get("cur")), _num(get("max")), _num(get("hist")),
            (get("hash") or "").strip())


def parse_blob(text):
    """A whole ledger revision -> (header, {mangled: row})."""
    header = None
    rows = {}
    index = {}
    for line in text.splitlines():
        if not line or line.startswith("#"):
            continue
        if header is None:
            header = _header(line)
            index = {name: i for i, name in enumerate(header)}
            continue
        fields = line.split("\t")
        if fields[0]:
            rows[fields[0]] = _row(fields, index)
    return header, rows


def parse_delta(diff_text, header):
    """A `git diff -U0` between two ledger blobs -> (new header or None,
    {mangled: row, or None when the row was removed}).

    `header` is the old blob's column header; a delta that changes the header
    carries the new one, and its rows are read with it."""
    new_header = None
    removed = set()
    added = []
    for line in diff_text.splitlines():
        if line.startswith(("+++", "---", "@@")) or len(line) < 2:
            continue
        sign, body = line[0], line[1:]
        if sign not in "+-" or body.startswith("#"):
            continue
        if _is_header(body):
            if sign == "+" and new_header is None:
                new_header = _header(body)
            continue
        mangled = body.split("\t", 1)[0]
        if sign == "-":
            removed.add(mangled)
        else:
            added.append(body)
    columns = new_header or header or ()
    index = {name: i for i, name in enumerate(columns)}
    delta = dict.fromkeys(removed)
    for body in added:
        fields = body.split("\t")
        delta[fields[0]] = _row(fields, index)
    return new_header, delta


# --- the walk ----------------------------------------------------------------


@dataclass
class History:
    """Every commit that touched the ledger, oldest first (topological), and
    the tracked rows' value at each of them."""
    commits: list
    series: dict = field(default_factory=dict)   # mangled -> [row | None per commit]
    stats: dict = field(default_factory=dict)

    def __post_init__(self):
        self.index = {c.sha: i for i, c in enumerate(self.commits)}

    def parents(self, i):
        return [self.index[p] for p in self.commits[i].parents if p in self.index]


class _DeltaStore:
    """Blob-to-blob deltas: parsed once, pickled under the cache directory."""

    def __init__(self, repo, cache_dir, jobs):
        self.repo = repo
        self.cache_dir = Path(cache_dir) if cache_dir else None
        self.jobs = jobs or min(32, os.cpu_count() or 4)
        self.hits = self.computed = 0

    def _path(self, src, dst):
        if self.cache_dir is None:
            return None
        return self.cache_dir / f"v{CACHE_VERSION}-{src or 'root'}-{dst}.pickle"

    def cached(self, src, dst):
        path = self._path(src, dst)
        if path is None or not path.exists():
            return None
        try:
            with open(path, "rb") as fh:
                value = pickle.load(fh)
        except (OSError, EOFError, pickle.UnpicklingError, ValueError):
            return None
        self.hits += 1
        return value

    def save(self, src, dst, value):
        path = self._path(src, dst)
        if path is None:
            return
        try:
            path.parent.mkdir(parents=True, exist_ok=True)
            tmp = path.with_suffix(f".{os.getpid()}.tmp")
            with open(tmp, "wb") as fh:
                pickle.dump(value, fh, protocol=pickle.HIGHEST_PROTOCOL)
            os.replace(tmp, path)
        except OSError:
            pass  # a cache that cannot be written only costs the next run time

    def raw_text(self, src, dst):
        """Diff text (or the whole blob for a root) - the parallel part."""
        if src is None:
            return _git(self.repo, "cat-file", "blob", dst).stdout
        return _git(self.repo, "diff", "--no-color", "--no-ext-diff",
                    "--no-textconv", "--no-renames", "-U0", src, dst).stdout

    def fetch_all(self, pairs):
        """({pair: cached delta}, {pair: raw text}) - every uncached pair's
        `git diff` runs in a thread pool; parsing stays in order (a delta is
        read with its source blob's header)."""
        loaded = {}
        for pair in pairs:
            value = self.cached(*pair)
            if value is not None:
                loaded[pair] = value
        missing = [pair for pair in pairs if pair not in loaded]
        texts = {}
        if missing:
            with ThreadPoolExecutor(max_workers=self.jobs) as pool:
                texts = dict(zip(missing, pool.map(
                    lambda pair: self.raw_text(*pair), missing)))
        return loaded, texts


def walk(mangleds, repo=None, rev="HEAD", cache_dir=None, jobs=None,
         commits=None):
    """Follow `mangleds` through every ledger commit reachable from `rev`."""
    repo = repo or paths.REPO
    started = time.monotonic()
    commits = load_commits(repo, rev) if commits is None else commits
    wanted = set(mangleds)
    deltas = _DeltaStore(repo, cache_dir, jobs)

    # Each blob is derived once, from the first blob that produced it.
    order = []
    seen = set()
    for commit in commits:
        if commit.blob and commit.blob not in seen:
            seen.add(commit.blob)
            order.append((commit.src if commit.src in seen else None, commit.blob))
    loaded, texts = deltas.fetch_all(order)

    headers = {}
    states = {}
    for src, dst in order:
        value = loaded.get((src, dst))
        if value is None:
            text = texts[(src, dst)]
            if src is None:
                header, rows = parse_blob(text)
            else:
                header, rows = parse_delta(text, headers.get(src))
            value = (header, rows)
            deltas.save(src, dst, value)
            deltas.computed += 1
        header, rows = value
        if src is None:
            headers[dst] = header
            states[dst] = {m: rows[m] for m in wanted if m in rows}
            continue
        headers[dst] = header or headers.get(src)
        state = dict(states.get(src, {}))
        small, large = (rows, wanted) if len(rows) < len(wanted) else (wanted, rows)
        for m in small:
            if m in large:
                if rows[m] is None:
                    state.pop(m, None)
                else:
                    state[m] = rows[m]
        states[dst] = state

    history = History(commits)
    for m in wanted:
        history.series[m] = [states.get(c.blob, {}).get(m) if c.blob else None
                             for c in commits]
    history.stats = {"commits": len(commits), "blobs": len(order),
                     "cached": deltas.hits, "computed": deltas.computed,
                     "seconds": time.monotonic() - started}
    return history


# --- reading a row's series ---------------------------------------------------


def level(row):
    """The best a commit measured for this row: max, or an exact cur."""
    if row is None:
        return None
    cur, mx = row[0], row[1]
    if cur is not None and cur >= EXACT:
        cur = 100.0
    values = [v for v in (cur, mx) if v is not None]
    return max(values) if values else None


def _at(value, target):
    return value is not None and value >= target - TOLERANCE


@dataclass
class Event:
    target: float
    peak: int | None = None       # first commit at target
    last: int | None = None       # last commit at target
    drop: int | None = None       # the commit after which it stayed below
    measure: str | None = None    # max | best | hist - see `find_event`

    @property
    def inherited(self):
        """The peak is only in a carried `hist` column, never measured."""
        return self.measure == "hist"

    def score(self, row):
        """The row's value under the measure that found the peak."""
        if row is None:
            return None
        return MEASURES[self.measure or "best"](row)

    def to_json(self, history, series):
        def commit(i):
            if i is None:
                return None
            c = history.commits[i]
            row = series[i]
            return {"commit": c.sha, "date": c.date, "subject": c.subject,
                    "score": self.score(row), "hash": row[3] if row else None}
        out = {"target": self.target, "peak": commit(self.peak),
               "last": commit(self.last), "drop": commit(self.drop),
               "measure": self.measure, "inherited": self.inherited}
        if self.drop is not None:
            out["drop"]["body_changed"] = body_changed(history, series, self.drop)
        return out


def first_parent_row(history, series, i):
    parents = history.parents(i)
    return series[parents[0]] if parents else None


def body_changed(history, series, i):
    before, after = first_parent_row(history, series, i), series[i]
    if before is None or after is None:
        return None
    return before[3] != after[3]


MEASURES = {
    "max": lambda row: row[1],
    "best": level,
    "hist": lambda row: row[2],
}


def find_event(history, series, target, measures=("max", "best", "hist")):
    """Where a row reached `target` and where it lost it for good.

    Each measure is tried in turn until one reaches the target: `max` (the
    hash-scoped peak), `best` (max or an exact cur), then `hist` (a peak only
    carried in the hist column - seeded from older history, `inherited`).
    PEAK is the first commit at the target, LAST the last one (topological
    order: on a merged branch it can sit beside the drop rather than before
    it). When the newest commit is below the target, DROP is the last commit
    below it whose first parent was at it - preferring one whose every parent
    was at it, so a merge that only brought in a branch's drop does not hide
    the branch commit that made it."""
    event = Event(target)
    for name in measures:
        measure = MEASURES[name]
        levels = [None if row is None else measure(row) for row in series]
        hits = [i for i, value in enumerate(levels) if _at(value, target)]
        if hits:
            event.peak, event.last, event.measure = hits[0], hits[-1], name
            break
    else:
        return event
    if _at(levels[-1], target):
        return event
    clean = loose = None
    for i, value in enumerate(levels):
        if _at(value, target):
            continue
        parents = history.parents(i)
        if not parents or not _at(levels[parents[0]], target):
            continue
        loose = i
        if all(_at(levels[p], target) for p in parents):
            clean = i
    event.drop = clean if clean is not None else loose
    return event


def recoverable(row):
    return (row.get("size") or 0) * gap(row)


def gap(row):
    return (row.get("hist") or 0) - (row.get("max") or 0)


# --- hist-gap -------------------------------------------------------------------


def gap_rows(rows, module=None, unit=None, min_gap=0.01, exact=False):
    out = []
    for row in rows.values():
        if row.get("hist") is None or gap(row) <= min_gap:
            continue
        if exact and row["hist"] < 100 - TOLERANCE:
            continue
        if module and row.get("module") != module:
            continue
        if unit and unit not in (row.get("unit") or ""):
            continue
        out.append(row)
    return out


def _fmt(value):
    return "-" if value is None else f"{value:.1f}"


def _trim(text, width):
    return text if len(text) <= width else text[: width - 3] + "..."


def cmd_hist_gap(args, rows=None, history=None):
    rows = store.load() if rows is None else rows
    selected = gap_rows(rows, args.module, args.unit, args.min_gap, args.exact)
    if not selected:
        print("(no rows)", file=sys.stderr)
        return 0
    if history is None:
        history = walk([r["mangled"] for r in selected], cache_dir=args.cache_dir)
    records = []
    for row in selected:
        series = history.series.get(row["mangled"], [])
        event = find_event(history, series, row["hist"])
        records.append((row, series, event))

    def drop_key(item):
        event = item[2]
        return -(event.drop if event.drop is not None else -1)

    keys = {"recoverable": lambda item: (-recoverable(item[0]), item[0]["mangled"]),
            "gap": lambda item: (-gap(item[0]), item[0]["mangled"]),
            "drop": lambda item: (drop_key(item), item[0]["mangled"])}
    records.sort(key=keys[args.sort])
    shown = records[: args.limit] if args.limit else records

    if args.json:
        print(json.dumps([
            {"mangled": row["mangled"], "module": row.get("module"),
             "unit": row.get("unit"), "size": row.get("size"),
             "cur": row.get("cur"), "max": row.get("max"), "hist": row.get("hist"),
             "recoverable": round(recoverable(row), 2),
             **event.to_json(history, series)}
            for row, series, event in shown], indent=1))
    else:
        print(f"{'recov':>7s} {'size':>6s} {'cur%':>6s} {'max%':>6s} {'hist%':>6s}"
              f"  {'module':10s} mangled")
        for row, series, event in shown:
            print(f"{recoverable(row):7.0f} {row.get('size') or 0:6d} "
                  f"{_fmt(row.get('cur')):>6s} {_fmt(row.get('max')):>6s} "
                  f"{_fmt(row.get('hist')):>6s}  {(row.get('module') or '-'):10s} "
                  f"{row['mangled']}")
            print("        " + _peak_line(history, event))
            print("        " + _drop_line(history, series, event))
    with_drop = sum(1 for _r, _s, e in records if e.drop is not None)
    with_peak = sum(1 for _r, _s, e in records if e.peak is not None)
    stats = history.stats
    _log(f"{len(records)} row(s) with hist - max > {args.min_gap:g}"
         f"{' and hist = 100' if args.exact else ''}; {with_peak} with a peak, "
         f"{with_drop} with a drop"
         + (f"; showing {len(shown)}" if len(shown) < len(records) else "")
         + (f"; {stats['commits']} commits / {stats['blobs']} blobs "
            f"({stats['cached']} cached, {stats['computed']} parsed) in "
            f"{stats['seconds']:.1f}s" if stats else ""))
    return 0


def _peak_line(history, event):
    if event.peak is None:
        return "peak  - (no ledger commit reached it)"
    mark = "peak* " if event.inherited else "peak  "
    return mark + _trim(history.commits[event.peak].label(), 110)


def _drop_line(history, series, event):
    if event.drop is None:
        if event.peak is None:
            return "drop  -"
        if event.inherited:
            return "drop  - never measured at it here: hist carries a peak older than the ledger"
        if event.last == len(history.commits) - 1:
            row = series[event.last]
            return ("drop  - still at it by an exact cur; max never banked it"
                    + ("" if row and row[3] else " (no source hash)"))
        return ("drop  - no clean drop; last at it "
                + _trim(history.commits[event.last].label(), 90))
    c = history.commits[event.drop]
    before = first_parent_row(history, series, event.drop)
    after = series[event.drop]
    changed = body_changed(history, series, event.drop)
    why = ("row removed" if after is None
           else "body changed" if changed else "same body")
    return (f"drop  {c.short} {c.date} {_fmt(event.score(before))} -> "
            f"{_fmt(event.score(after))} ({why})  {_trim(c.subject, 80)}")


# --- last-exact ------------------------------------------------------------------


def select(rows, selector):
    """Ledger rows for a selector: an exact mangled name, a `a::b::c`
    qualified name (matched as `c@b@a@` in the mangled form), or a substring
    of the mangled name."""
    if selector in rows:
        return [rows[selector]]
    needle = selector
    if "::" in selector:
        needle = "@".join(reversed(selector.split("::"))) + "@"
    hits = [r for m, r in rows.items() if needle in m]
    if not hits and needle != selector:
        hits = [r for m, r in rows.items() if selector in m]
    return sorted(hits, key=lambda r: r["mangled"])


def last_exact_event(history, series):
    """Last commit at 100 (max, or an exact cur); the drop after it."""
    return find_event(history, series, 100.0, measures=("best", "hist"))


def source_file(relative):
    """A ledger unit (or PDB file) path -> repo-relative posix path, or None."""
    from vostok.derive.maxima import _source_file
    for root in (paths.SOURCES, paths.SCALEFORM_SOURCES):
        found = _source_file(str(root), relative)
        if found is not None:
            return Path(found).relative_to(paths.REPO).as_posix()
    return None


def evidence_record(mangled):
    """(demangled name, file) from the PDB evidence, when a build left one."""
    for db in (paths.BASE_EVIDENCE, paths.TARGET_EVIDENCE):
        if not Path(db).exists():
            continue
        try:
            with sqlite3.connect(f"file:{db}?mode=ro", uri=True) as con:
                hit = con.execute("SELECT name, file FROM functions WHERE mangled = ?",
                                  (mangled,)).fetchone()
        except sqlite3.Error:
            continue
        if hit:
            return hit
    return None, None


def show(repo, rev, path):
    result = subprocess.run(["git", "show", f"{rev}:{path}"], cwd=repo,
                            capture_output=True, check=False)
    return None if result.returncode else result.stdout.decode("latin-1")


def cmd_last_exact(args, rows=None, history=None):
    rows = store.load() if rows is None else rows
    hits = select(rows, args.function)
    if not hits:
        print(f"[ledger] no ledger row matches {args.function!r}", file=sys.stderr)
        return 1
    if len(hits) > args.limit:
        for row in hits[:20]:
            print(f"  {row['mangled']}", file=sys.stderr)
        print(f"[ledger] {len(hits)} rows match {args.function!r}; narrow it or "
              f"raise --limit", file=sys.stderr)
        return 1
    if history is None:
        history = walk([r["mangled"] for r in hits], cache_dir=args.cache_dir)
    repo = paths.REPO
    records = []
    for row in hits:
        series = history.series.get(row["mangled"], [])
        event = last_exact_event(history, series)
        record = {"mangled": row["mangled"], "module": row.get("module"),
                  "unit": row.get("unit"), "size": row.get("size"),
                  "cur": row.get("cur"), "max": row.get("max"),
                  "hist": row.get("hist"), "hash": row.get("hash"),
                  **event.to_json(history, series)}
        name, pdb_file = evidence_record(row["mangled"])
        record["name"] = name
        path = source_file(pdb_file or row.get("unit") or "") if (
            pdb_file or row.get("unit")) else None
        record["path"] = path
        if event.last is not None and path:
            sha = history.commits[event.last].sha
            log = _git(repo, "log", "--format=%h %ad %s", "--date=short",
                       f"{sha}..HEAD", "--", path, check=False)
            record["file_commits"] = log.stdout.splitlines()
            if args.diff:
                record["diff"] = function_diff(
                    repo, sha, path, row["mangled"], name,
                    old_hash=series[event.last][3], new_hash=row.get("hash"))
        records.append(record)

    if args.json:
        print(json.dumps(records, indent=1))
        return 0
    for record in records:
        _print_last_exact(record, args.diff)
    return 0


def _print_last_exact(record, want_diff):
    print(record["mangled"])
    if record.get("name"):
        print(f"  {record['name']}")
    print(f"  {record.get('module') or '-'}  {record.get('path') or record.get('unit') or '-'}"
          f"  size {record.get('size') or 0}  cur {_fmt(record['cur'])} "
          f"max {_fmt(record['max'])} hist {_fmt(record['hist'])}  hash {record['hash'] or '-'}")
    last, drop = record["last"], record["drop"]
    if last is None:
        print("  never at 100 in the ledger history"
              + (" (hist carries 100 from before it)" if (record["hist"] or 0) >= 100 else ""))
        return
    inherited = " (only the carried hist column; never measured)" if record["inherited"] else ""
    same = ("same body as HEAD" if last["hash"] == record["hash"]
            else f"body since changed, HEAD hash {record['hash'] or '-'}")
    print(f"  last exact  {last['commit'][:10]} {last['date']} {last['subject']}{inherited}")
    print(f"              hash {last['hash'] or '-'} ({same})")
    if drop is not None:
        why = ("body changed" if drop.get("body_changed")
               else "same body" if drop.get("body_changed") is False else "row removed")
        print(f"  dropped     {drop['commit'][:10]} {drop['date']} -> "
              f"{_fmt(drop['score'])} ({why}) {drop['subject']}")
    elif (record["max"] or 0) >= 100 - TOLERANCE:
        print("  still exact at HEAD")
    if record.get("file_commits"):
        mark = drop["commit"][:7] if drop else None
        print(f"  commits touching {record['path']} since (newest first; > the drop):")
        for line in record["file_commits"]:
            sha = line.split(" ", 1)[0]
            flag = ">" if mark and drop["commit"].startswith(sha) else " "
            print(f"   {flag} {line}")
    elif record.get("path"):
        print(f"  no commit touched {record['path']} since")
    if want_diff:
        diff = record.get("diff")
        if diff is None:
            print("  (no source file to diff)")
        else:
            print(f"  {diff['scope']}")
            print(diff["text"] if diff["text"] else
                  "  (no source change - the hash moved with the PDB extent)"
                  if last["hash"] != record["hash"] else "  (no source change)")


# --- locating a function body in source text -------------------------------------


_OPERATORS = {"4": "=", "5": ">>", "6": "<<", "7": "!", "8": "==", "9": "!=",
              "A": "[]", "C": "->", "D": "*", "E": "++", "F": "--", "G": "-",
              "H": "+", "I": "&", "K": "/", "L": "%", "M": "<", "N": "<=",
              "O": ">", "P": ">=", "R": "()", "S": "~", "T": "^", "U": "|",
              "X": "*=", "Y": "+=", "Z": "-=", "2": "new", "3": "delete"}


def _mangled_part(text):
    """One `name@` component (a `?$tmpl@...` keeps only `tmpl`)."""
    if text.startswith("?$"):
        text = text[2:]
    return text.split("@", 1)[0]


def names_from_mangled(mangled):
    """(function name, owning class) from an MSVC decorated name, or None.

    Enough for the common shapes: plain and member functions, constructors,
    destructors and operators. Template instantiations whose name is itself a
    special name are given up on."""
    if mangled.startswith("??$"):
        name = mangled[3:].split("@", 1)[0]
        return None if name.startswith("?") else (name, None)
    if mangled.startswith("??"):
        code = mangled[2]
        if code == "_":
            return None
        rest = mangled[3:]
        owner = _mangled_part(rest) if rest else None
        if not owner or owner[0].isdigit():
            return None
        if code == "0":
            return owner, owner
        if code == "1":
            return "~" + owner, owner
        op = _OPERATORS.get(code)
        return ("operator" + op, owner) if op else None
    if not mangled.startswith("?"):
        return None
    name, _, rest = mangled[1:].partition("@")
    owner = None
    if rest and not rest.startswith("@"):
        part = _mangled_part(rest)
        owner = part if part and not part[0].isdigit() else None
    return name, owner


def names_from_demangled(name):
    """(function name, owning class) from a PDB name like
    `ns::cls<T>::fn<U>(int, char)`."""
    depth = 0
    cut = len(name)
    i = 0
    while i < len(name):
        ch = name[i]
        if name.startswith("operator", i):
            i += len("operator")
            while i < len(name) and name[i] in " ()[]<>=!+-*/%&|^~,":
                if name[i] == "(" and name.startswith("()", i) and depth == 0:
                    i += 2
                    continue
                if name[i] == "(":
                    break
                i += 1
            continue
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch == "(" and depth == 0:
            cut = i
            break
        i += 1
    qualified = name[:cut]
    parts = []
    depth = 0
    start = 0
    for i, ch in enumerate(qualified):
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch == ":" and depth == 0 and qualified.startswith("::", i) and i >= start:
            parts.append(qualified[start:i])
            start = i + 2
    parts.append(qualified[start:])

    def bare(part):
        part = part.strip()
        if part.startswith("operator"):
            return part
        return part.split("<", 1)[0].strip()
    parts = [bare(p) for p in parts if p.strip()]
    if not parts or not parts[-1]:
        return None
    return parts[-1], (parts[-2] if len(parts) > 1 else None)


def mask(text):
    """Blank out comments, string/char literals and preprocessor lines (a
    `#line` pin may sit between a declarator and its `{`), keeping every
    offset."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        ch = text[i]
        two = text[i:i + 2]
        if ch == "#" and not text[text.rfind("\n", 0, i) + 1:i].strip():
            j = i
            while True:  # through backslash continuations
                j = text.find("\n", j)
                if j < 0:
                    j = n
                    break
                if text[j - 1] != "\\":
                    break
                j += 1
        elif two == "//":
            j = text.find("\n", i)
            j = n if j < 0 else j
        elif two == "/*":
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
        elif ch in "\"'":
            j = i + 1
            while j < n and text[j] != ch and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
        else:
            i += 1
            continue
        for k in range(i, j):
            if out[k] != "\n":
                out[k] = " "
        i = j
    return "".join(out)


def _match_close(masked, start, opener, closer):
    depth = 0
    for i in range(start, len(masked)):
        if masked[i] == opener:
            depth += 1
        elif masked[i] == closer:
            depth -= 1
            if depth == 0:
                return i
    return None


_WORD = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")


def _body_open(masked, close_paren):
    """Offset of the `{` opening a definition whose parameter list closes at
    `close_paren`, or None for a declaration, call or expression."""
    i, n = close_paren + 1, len(masked)
    in_init = False
    while i < n and i < close_paren + 4000:
        ch = masked[i]
        if ch.isspace():
            i += 1
        elif ch == "{":
            return i
        elif ch == "(":
            end = _match_close(masked, i, "(", ")")
            if end is None:
                return None
            i = end + 1
        elif ch == ":" and not in_init and not masked.startswith("::", i):
            in_init = True
            i += 1
        elif ch == "," and in_init:
            i += 1
        elif ch == ":" and masked.startswith("::", i):
            i += 2
        elif ch == "<" and in_init:
            end = _match_close(masked, i, "<", ">")
            i = n if end is None else end + 1
        else:
            word = _WORD.match(masked, i)
            if word is None:
                return None
            i = word.end()
    return None


@dataclass
class Body:
    first: int        # 1-based line of the declarator (or the line above it)
    open: int         # line of `{`
    last: int         # line of the closing `}`
    qualified: bool   # spelled `Owner::name`, or inside `class Owner { }`
    verified: bool = False


def find_bodies(text, name, owner=None):
    """Every definition of `name` in a source text, as line extents."""
    masked = mask(text)
    if name.startswith("operator"):
        pattern = r"\boperator\s*" + r"\s*".join(map(re.escape, name[len("operator"):]))
    elif name.startswith("~"):
        pattern = r"~\s*" + re.escape(name[1:]) + r"\b"
    else:
        pattern = r"(?<![\w~])" + re.escape(name) + r"\b"
    regex = re.compile(pattern + r"\s*(?:<[^;{}()]*>)?\s*\(")
    qualifier = re.compile(r"(?:\b" + re.escape(owner) + r"\s*(?:<[^;{}()]*>)?\s*)::\s*$"
                           ) if owner else None

    def line_of(offset):
        return masked.count("\n", 0, offset) + 1

    # `class Owner ... { ... }` spans: an in-class definition is not spelled
    # `Owner::name`, but it is still Owner's
    scopes = []
    if owner:
        for head in re.finditer(r"\b(?:class|struct|union)\s+(?:\w+\s+)*"
                                + re.escape(owner) + r"\b[^;{()]*\{", masked):
            end = _match_close(masked, head.end() - 1, "{", "}")
            if end is not None:
                scopes.append((head.end(), end))

    bodies = []
    for match in regex.finditer(masked):
        paren = match.end() - 1
        close = _match_close(masked, paren, "(", ")")
        if close is None:
            continue
        brace = _body_open(masked, close)
        if brace is None:
            continue
        end = _match_close(masked, brace, "{", "}")
        if end is None:
            continue
        line_start = masked.rfind("\n", 0, match.start()) + 1
        lead = masked[line_start:match.start()]
        qualified = bool(qualifier and qualifier.search(lead)) or any(
            lo <= match.start() < hi for lo, hi in scopes)
        first = line_of(match.start())
        # a return type or template<> on its own line belongs to the definition
        lines = text.splitlines()
        while first > 1:
            above = lines[first - 2].strip()
            if not above or above.endswith((";", "}", "{", ")", ":")) or above.startswith(
                    ("#", "//", "/*", "*")):
                break
            first -= 1
        bodies.append(Body(first, line_of(brace), line_of(end), qualified))
    return bodies


def _extent_hashes(text, body):
    """Hashes of the PDB extents a definition could have been hashed over
    (see `vostok.derive.maxima`: the first to the last statement line)."""
    from vostok.derive.maxima import _LINE_DIRECTIVE, _extent_text, source_hash
    source_lines = text.splitlines(keepends=True)
    # PDB lines are the compiler's: a `#line N` pin renumbers what follows
    virtual, number = [], 1
    for line in source_lines:
        directive = _LINE_DIRECTIVE.match(line)
        if directive:
            number = int(directive.group(1))
            virtual.append(None)
            continue
        virtual.append(number)
        number += 1

    def numbers(lo, hi, physical):
        picked = range(max(1, lo), min(hi, len(source_lines)) + 1)
        return {n if physical else virtual[n - 1] for n in picked} - {None}

    hashes = set()
    for physical in (False, True):
        starts = numbers(body.first - 1, body.open + 1, physical)
        ends = numbers(body.last - 2, body.last, physical)
        for lo in starts:
            for hi in ends:
                if hi < lo:
                    continue
                if physical:
                    hashes.add(source_hash("".join(source_lines[lo - 1:hi])))
                else:
                    hashes.add(source_hash(_extent_text(source_lines, lo, hi)))
    return hashes


def locate(text, names, ledger_hash=None):
    """The best definition of a function in `text`: one whose extent hashes
    to the ledger's `hash` (verified), else the only, or the first
    `Owner::`-qualified, candidate."""
    if text is None or names is None:
        return None
    bodies = find_bodies(text, *names)
    if not bodies:
        return None
    if ledger_hash:
        for body in bodies:
            if ledger_hash in _extent_hashes(text, body):
                body.verified = True
                return body
    qualified = [b for b in bodies if b.qualified]
    return (qualified or bodies)[0]


def function_diff(repo, sha, path, mangled, demangled, old_hash, new_hash):
    """Diff of the function's body between `sha` and HEAD, else of the file."""
    names = (names_from_demangled(demangled) if demangled else None) \
        or names_from_mangled(mangled)
    old_text = show(repo, sha, path)
    new_text = show(repo, "HEAD", path)
    old_label, new_label = f"{sha[:10]}:{path}", f"HEAD:{path}"
    if old_text is None:
        return {"scope": f"{path} did not exist at {sha[:10]}; whole file",
                "text": _git(repo, "diff", sha, "HEAD", "--", path,
                             check=False).stdout.rstrip("\n")}
    old_body = locate(old_text, names, old_hash)
    new_body = locate(new_text, names, new_hash)
    if old_body is None or new_body is None:
        why = ("its name could not be derived" if names is None
               else f"no definition of `{names[0]}` found "
               + ("then" if old_body is None else "at HEAD"))
        return {"scope": f"whole-file diff ({why})",
                "text": _git(repo, "diff", sha, "HEAD", "--", path,
                             check=False).stdout.rstrip("\n")}

    def cut(text, body):
        return text.splitlines()[body.first - 1: body.last]
    verified = {(True, True): "both extents verified against the ledger hash",
                (True, False): "verified at the exact commit; located by name at HEAD",
                (False, True): "located by name then; verified at HEAD",
                (False, False): "located by name"}[(old_body.verified, new_body.verified)]
    text = "\n".join(difflib.unified_diff(
        cut(old_text, old_body), cut(new_text, new_body),
        f"{old_label} (lines {old_body.first}-{old_body.last})",
        f"{new_label} (lines {new_body.first}-{new_body.last})", lineterm=""))
    return {"scope": f"function `{names[0]}` ({verified})", "text": text}


# --- argparse wiring (called from vostok.ledger.cli) -------------------------------


def add_parsers(sub):
    p = sub.add_parser(
        "hist-gap", help="rows whose hist exceeds max, with the commits that "
        "reached and lost the peak",
        description="For every row with hist > max: the PEAK commit where it "
        "first reached its hist and the DROP commit after which it stayed "
        "below. Reads committed ledger history only.")
    p.add_argument("--module")
    p.add_argument("--unit", help="substring of the TU path")
    p.add_argument("--min-gap", type=float, default=0.01, metavar="PCT",
                   help="smallest hist - max to report (default 0.01, the "
                        "--headroom threshold)")
    p.add_argument("--exact", action="store_true", help="only rows whose hist is 100")
    p.add_argument("--sort", choices=("recoverable", "gap", "drop"),
                   default="recoverable",
                   help="recoverable = size * (hist - max) (default); drop = "
                        "most recent drop first")
    p.add_argument("--limit", type=int)
    p.add_argument("--json", action="store_true")
    p.add_argument("--cache-dir", default=paths.LEDGER_HISTORY_CACHE,
                   help=argparse.SUPPRESS)
    p.set_defaults(func=cmd_hist_gap)

    p = sub.add_parser(
        "last-exact", help="the last commit a function was exact, and its "
        "source diff since",
        description="FUNCTION is a mangled name, an `ns::cls::fn` name, or a "
        "substring of the mangled name. Prints the last ledger commit where it "
        "was at 100, the commit that lost it and the commits that touched its "
        "source file since; --diff shows the function body then against HEAD "
        "(the whole file's diff when the body cannot be found).")
    p.add_argument("function")
    p.add_argument("--diff", action="store_true",
                   help="diff the function's source at that commit against HEAD")
    p.add_argument("--limit", type=int, default=5,
                   help="refuse when more than N rows match (default 5)")
    p.add_argument("--json", action="store_true")
    p.add_argument("--cache-dir", default=paths.LEDGER_HISTORY_CACHE,
                   help=argparse.SUPPRESS)
    p.set_defaults(func=cmd_last_exact)



