#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Rebase the offline lane (v0.10b-offline) onto the matching mainline and re-apply
# the strip of the matching tooling.
#
#   scripts/rebase-offline.sh [--upstream REF] [--no-fetch] [--no-checks]
#   scripts/rebase-offline.sh --continue    after resolving a reported conflict by hand
#   scripts/rebase-offline.sh --abort       give up; the branch returns to where it was
#     (mid-rebase the tree predates this script: the stop message names the copy
#      kept in the git dir, `bash $(git rev-parse --git-dir)/rebase-offline.sh`)
#   scripts/rebase-offline.sh --list-stripped [REV]
#                                           tracked paths at REV (default HEAD) outside KEEP
#
# The lane is v0.10b plus offline commits, one of which ("offline: strip the matching
# tooling") deletes everything outside KEEP below and trims the build entry points.
# Conflicts are resolved automatically when they are only about the strip:
#   - a path outside KEEP stays deleted (modify/delete, or upstream edits to it);
#   - a file in OFFLINE_OWNED takes the offline version (the strip rewrote it);
#   - a file in STRIP_EDITED keeps the regions the strip cut out (conflict hunks
#     whose offline side is empty), anything else in it is left for a human;
#   - flake.lock is re-locked from the resolved flake.nix.
# Every other conflict (offline game sources, the release script, ...) stops the
# rebase for a human: resolve, `git add`, then continue with the command it prints.
#
# After the rebase, tracked files outside KEEP (new matching files from v0.10b) are
# removed and folded into the strip commit, flake.lock is re-locked, and the kept
# tooling is checked: ruff, unit tests, every vostok import resolves, the flake
# evaluates. The script never pushes.
set -euo pipefail

STRIP_SUBJECT="offline: strip the matching tooling"
BRANCH="${REBASE_OFFLINE_BRANCH:-v0.10b-offline}"
DEFAULT_UPSTREAM="origin/v0.10b"

# Everything the offline lane keeps. An entry ending in / keeps the whole tree.
KEEP=(
	sources/
	resources/
	docs/bugs/
	docs/reference/
	docs/materials.md
	scripts/release/
	scripts/rebase-offline.sh
	scripts/vostok/__init__.py
	scripts/vostok/__main__.py
	scripts/vostok/cli.py
	scripts/vostok/core/__init__.py
	scripts/vostok/core/log.py
	scripts/vostok/core/paths.py
	scripts/vostok/core/wine.py
	scripts/vostok/build/__init__.py
	scripts/vostok/build/native_crt.py
	scripts/vostok/build/ninja.py
	scripts/vostok/build/ninja_regen.py
	scripts/vostok/build/rebuild.py
	scripts/vostok/tool/__init__.py
	scripts/vostok/tool/libs.py
	scripts/vostok/tool/toolchain.py
	scripts/vostok/tests/__init__.py
	scripts/vostok/tests/test_prebuilt_staging.py
	scripts/vostok/tests/test_setup_graph.py
	scripts/vostok/tests/test_setup_lock.py
	scripts/vostok/tests/test_watchdog_diagnostics.py
	.clangd
	.gitignore
	AGENTS.md
	CLAUDE.md
	LICENSE
	README.md
	flake.lock
	flake.nix
)

# Files the strip rewrote wholesale: on a conflict the offline version wins.
OFFLINE_OWNED=(
	CLAUDE.md
	README.md
	scripts/vostok/__init__.py
	scripts/vostok/build/__init__.py
	scripts/vostok/build/rebuild.py
	scripts/vostok/cli.py
	scripts/vostok/tests/__init__.py
	scripts/vostok/tool/__init__.py
)

# Files the strip cut regions out of: a conflict hunk whose offline side is empty
# stays cut; any other hunk is a real conflict.
STRIP_EDITED=(
	flake.nix
	scripts/vostok/tool/toolchain.py
)

# Modules the build needs; the import check starts from these.
ENTRY_MODULES=(vostok.cli vostok.build.rebuild vostok.build.ninja vostok.tool.toolchain)

log() { printf '[rebase-offline] %s\n' "$*" >&2; }
die() { log "error: $*"; exit 1; }

in_list() {
	local needle=$1; shift
	local item
	for item in "$@"; do [ "$item" = "$needle" ] && return 0; done
	return 1
}

is_kept() {
	local path=$1 entry
	for entry in "${KEEP[@]}"; do
		case $entry in
			*/) [[ $path == "$entry"* ]] && return 0 ;;
			*) [ "$path" = "$entry" ] && return 0 ;;
		esac
	done
	return 1
}

# python3/ruff from PATH, else from the repo's pinned nixpkgs.
tool() {
	local name=$1; shift
	if command -v "$name" >/dev/null; then
		"$name" "$@"
	else
		nix shell --inputs-from "$repo" "nixpkgs#$name" -c "$name" "$@"
	fi
}

repo=$(git rev-parse --show-toplevel) || die "run inside the offline worktree"
git_dir=$(git rev-parse --absolute-git-dir)
state_upstream="$git_dir/rebase-offline-upstream"
state_orig="$git_dir/rebase-offline-orig"
state_script="$git_dir/rebase-offline.sh"

# Bash reads a script as it runs, and the rebase checks out trees without this
# file: run from a private copy.
if [ -z "${REBASE_OFFLINE_COPY:-}" ]; then
	copy=$(mktemp "${TMPDIR:-/tmp}/rebase-offline.XXXXXX")
	cp "$0" "$copy"
	REBASE_OFFLINE_COPY=$copy exec bash "$copy" "$@"
fi
trap 'rm -f "$REBASE_OFFLINE_COPY"' EXIT
cd "$repo"

# Every rebase step runs with: diff3 markers (resolve_cut_hunks reads the base
# side); no rerere (it would record these mechanical resolutions and replay them
# where the rules do not apply); and a real 3-way merge for the "ours" driver,
# which a checkout's .git/info/attributes may bind to README.md - it would
# silently drop the offline README in favour of upstream's.
rebase() {
	GIT_EDITOR=true git -c merge.conflictStyle=diff3 -c rerere.enabled=false \
		-c "merge.ours.driver=git merge-file --diff3 -L ours -L base -L theirs %A %O %B" \
		rebase "$@" | { grep -Ev '^ (create|delete) mode |^ rename ' || true; }
	return "${PIPESTATUS[0]}"
}

rebase_in_progress() { [ -d "$git_dir/rebase-merge" ] || [ -d "$git_dir/rebase-apply" ]; }

has_stage() { git ls-files -u -- "$1" | awk -v s="$2" '$3 == s { found = 1 } END { exit !found }'; }

# Drop conflict hunks whose offline ("theirs") side is empty: the strip cut that
# region and upstream edited it. Exit 0 when no conflict markers remain.
resolve_cut_hunks() {
	tool python3 - "$1" <<'PY'
import sys
from pathlib import Path

path = Path(sys.argv[1])
lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
out, left, i = [], 0, 0
while i < len(lines):
    if not lines[i].startswith("<<<<<<< "):
        out.append(lines[i])
        i += 1
        continue
    start, parts, part = i, {"ours": [], "base": [], "theirs": []}, "ours"
    i += 1
    while i < len(lines) and not lines[i].startswith(">>>>>>> "):
        if lines[i].startswith("||||||| "):
            part = "base"
        elif lines[i].startswith("=======") and part == "base":
            part = "theirs"
        else:
            parts[part].append(lines[i])
        i += 1
    if i == len(lines):
        sys.exit("unterminated conflict hunk")
    end = i
    i += 1
    if not parts["theirs"]:
        continue  # the strip removed this region: keep it removed
    if parts["ours"] == parts["base"]:
        out.extend(parts["theirs"])
        continue
    out.extend(lines[start:end + 1])
    left += 1
path.write_text("".join(out), encoding="utf-8")
sys.exit(1 if left else 0)
PY
}

relock() {
	nix flake lock "$repo" >/dev/null 2>&1 || nix flake lock "$repo"
}

resolve_stop() {
	local unresolved=() lock_pending=0 path
	mapfile -t conflicted < <(git diff --name-only --diff-filter=U)
	for path in "${conflicted[@]}"; do
		if ! is_kept "$path"; then
			git rm -q -f --ignore-unmatch -- "$path"
			log "kept deleted (stripped): $path"
		elif in_list "$path" "${OFFLINE_OWNED[@]}"; then
			if has_stage "$path" 3; then
				git checkout --theirs -- "$path"
				git add -- "$path"
			else
				git rm -q -f --ignore-unmatch -- "$path"
			fi
			log "took the offline version: $path"
		elif in_list "$path" "${STRIP_EDITED[@]}" && has_stage "$path" 1 \
				&& has_stage "$path" 2 && has_stage "$path" 3; then
			if resolve_cut_hunks "$path"; then
				git add -- "$path"
				log "kept the strip's cuts: $path"
			else
				unresolved+=("$path")
			fi
		elif [ "$path" = flake.lock ]; then
			lock_pending=1
		else
			unresolved+=("$path")
		fi
	done
	if [ "$lock_pending" = 1 ]; then
		if in_list flake.nix "${unresolved[@]}"; then
			unresolved+=(flake.lock)
		else
			git checkout --ours -- flake.lock
			relock
			git add -- flake.lock
			log "re-locked: flake.lock"
		fi
	fi
	if [ "${#unresolved[@]}" -gt 0 ]; then
		log "real conflicts, resolve by hand:"
		printf '    %s\n' "${unresolved[@]}" >&2
		log "then: git add <files> && bash $state_script --continue"
		log "(or bash $state_script --abort)"
		exit 1
	fi
}

# Continue (or skip, when the resolution left nothing to commit) until done.
drive_rebase() {
	local stopped
	while rebase_in_progress; do
		if [ -n "$(git diff --name-only --diff-filter=U)" ]; then
			resolve_stop
		fi
		stopped=$(git rev-parse -q --verify REBASE_HEAD || true)
		if git diff --cached --quiet; then
			rebase --skip || true
		else
			rebase --continue || true
		fi
		if rebase_in_progress && [ -z "$(git diff --name-only --diff-filter=U)" ] \
				&& [ "$(git rev-parse -q --verify REBASE_HEAD || true)" = "$stopped" ]; then
			die "the rebase stopped at $stopped without a conflict; finish it by hand, then --continue"
		fi
	done
}

strip_commit() {
	git log --format='%H%x09%s' "$1..HEAD" | awk -F'\t' -v s="$STRIP_SUBJECT" '$2 == s { print $1; exit }'
}

restrip() {
	local upstream=$1 strip extra
	strip=$(strip_commit "$upstream")
	[ -n "$strip" ] || die "no \"$STRIP_SUBJECT\" commit above $upstream"
	mapfile -t extra < <(git ls-files | while IFS= read -r p; do is_kept "$p" || printf '%s\n' "$p"; done)
	if [ "${#extra[@]}" -gt 0 ]; then
		log "removing ${#extra[@]} tracked path(s) outside KEEP:"
		printf '    %s\n' "${extra[@]}" >&2
		printf '%s\0' "${extra[@]}" | git rm -q -r --pathspec-from-file=- --pathspec-file-nul
	fi
	relock
	git add flake.lock
	if git diff --cached --quiet; then
		log "strip still complete: nothing to fold in"
		return
	fi
	git commit -q --fixup="$strip"
	GIT_SEQUENCE_EDITOR=: rebase -q -i --autosquash --empty=drop "$upstream" \
		|| die "folding the re-strip into the strip commit stopped; finish it by hand"
	log "folded the re-strip into: $STRIP_SUBJECT"
}

check() {
	local failed=0
	log "check: ruff"
	tool ruff check -q scripts/ || failed=1
	log "check: vostok imports resolve"
	tool python3 - "${ENTRY_MODULES[@]}" <<'PY' || failed=1
import ast
import sys
from pathlib import Path

root = Path("scripts")


def module_file(name):
    base = root.joinpath(*name.split("."))
    for candidate in (base.with_suffix(".py"), base / "__init__.py"):
        if candidate.is_file():
            return candidate
    return None


missing, seen, queue = [], set(), list(sys.argv[1:])
while queue:
    name = queue.pop()
    if name in seen:
        continue
    seen.add(name)
    path = module_file(name)
    if path is None:
        missing.append(name)
        continue
    for node in ast.walk(ast.parse(path.read_text(encoding="utf-8"))):
        if isinstance(node, ast.Import):
            names = [alias.name for alias in node.names]
        elif isinstance(node, ast.ImportFrom) and node.module and not node.level:
            # `from pkg import sub` names a module when sub is one
            submodules = (f"{node.module}.{alias.name}" for alias in node.names)
            names = [node.module, *(n for n in submodules if module_file(n))]
        else:
            continue
        queue.extend(n for n in names if n.split(".")[0] == "vostok")
    parts = name.split(".")
    queue.extend(".".join(parts[:i]) for i in range(1, len(parts)))
for name in sorted(set(missing)):
    print(f"missing module: {name}", file=sys.stderr)
sys.exit(1 if missing else 0)
PY
	log "check: unit tests"
	local tests_log
	tests_log=$(PYTHONPATH="$repo/scripts" tool python3 -m unittest discover -q -s scripts/vostok/tests -t scripts 2>&1) \
		|| { printf '%s\n' "$tests_log" >&2; failed=1; }
	log "check: flake evaluates"
	nix eval --raw "$repo#devShells.x86_64-linux.default.drvPath" >/dev/null || failed=1
	return "$failed"
}

finish() {
	local upstream orig branch checks=$1
	upstream=$(cat "$state_upstream")
	branch=$(git symbolic-ref -q --short HEAD || echo "$BRANCH")
	orig=$(cat "$state_orig")
	restrip "$upstream"
	rm -f "$state_upstream" "$state_orig" "$state_script"
	log "rebased $branch: $(git rev-parse --short "$orig") -> $(git rev-parse --short HEAD) on $(git rev-parse --short "$upstream") ($upstream)"
	git log --format='    %h %s' "$upstream..HEAD" >&2
	if [ "$checks" = 1 ]; then
		check || die "checks failed on the rebased branch (it is rebased; fix and commit before pushing)"
		log "checks passed"
	fi
	log "push with: git push --force-with-lease=$branch:$(git rev-parse "$orig") origin $branch"
}

mode="start" upstream=$DEFAULT_UPSTREAM fetch=1 checks=1 list_rev=HEAD
while [ $# -gt 0 ]; do
	case $1 in
		--upstream) upstream=$2; shift 2 ;;
		--no-fetch) fetch=0; shift ;;
		--no-checks) checks=0; shift ;;
		--continue) mode="continue"; shift ;;
		--abort) mode="abort"; shift ;;
		--list-stripped) mode="list"; shift; [ $# -gt 0 ] && { list_rev=$1; shift; } ;;
		-h|--help) sed -n '3,9p' "$0" | sed 's/^# \{0,1\}//'; exit 0 ;;
		*) die "unknown argument: $1" ;;
	esac
done

case $mode in
	list)
		git ls-tree -r --name-only "$list_rev" | while IFS= read -r p; do is_kept "$p" || printf '%s\n' "$p"; done
		;;
	abort)
		rebase_in_progress && git rebase --abort
		rm -f "$state_upstream" "$state_orig" "$state_script"
		log "aborted"
		;;
	continue)
		[ -f "$state_upstream" ] || die "no rebase-offline in progress"
		drive_rebase
		finish "$checks"
		;;
	start)
		[ "$(git symbolic-ref -q --short HEAD)" = "$BRANCH" ] || die "check out $BRANCH first"
		rebase_in_progress && die "a rebase is already in progress (--continue or --abort)"
		[ -z "$(git status --porcelain --untracked-files=no)" ] || die "the worktree has uncommitted changes"
		if [ "$fetch" = 1 ]; then
			git fetch -q origin "${upstream#origin/}" 2>/dev/null || git fetch -q origin
		fi
		git rev-parse -q --verify "$upstream^{commit}" >/dev/null || die "unknown upstream: $upstream"
		printf '%s\n' "$upstream" >"$state_upstream"
		cp "$REBASE_OFFLINE_COPY" "$state_script"
		git rev-parse HEAD >"$state_orig"
		log "rebasing $BRANCH onto $upstream ($(git rev-parse --short "$upstream"))"
		rebase --empty=drop "$upstream" || true
		drive_rebase
		finish "$checks"
		;;
esac
