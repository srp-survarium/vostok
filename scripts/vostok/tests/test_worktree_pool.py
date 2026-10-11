# SPDX-License-Identifier: GPL-3.0-or-later
"""The build-worktree pool and the shared build cache, on throwaway repositories."""

import contextlib
import json
import os
import socket
import stat
import subprocess
import tempfile
import time
import unittest
from pathlib import Path
from unittest import mock

from vostok.build import shared_cache
from vostok.core import paths
from vostok.tool import worktree


def _git(cwd, *args):
    subprocess.run(["git", *args], cwd=cwd, check=True, capture_output=True,
                   env={**os.environ, "GIT_AUTHOR_NAME": "t", "GIT_AUTHOR_EMAIL": "t@t",
                        "GIT_COMMITTER_NAME": "t", "GIT_COMMITTER_EMAIL": "t@t"})


class PoolTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        origin, main = root / "origin.git", root / "vostok"
        _git(root, "init", "-q", "--bare", "-b", "v0.10b", str(origin))
        _git(root, "init", "-q", "-b", "v0.10b", str(main))
        (main / "flake.nix").write_text("{}\n")
        (main / ".gitignore").write_text("binaries/\n")
        _git(main, "add", ".")
        _git(main, "commit", "-qm", "base")
        _git(main, "remote", "add", "origin", str(origin))
        _git(main, "push", "-q", "origin", "v0.10b")
        _git(main, "fetch", "-q", "origin")
        _git(main, "branch", "-q", "feature")
        seed = main / "binaries"
        (seed / "gen").mkdir(parents=True)
        (seed / "gen" / "keep.tsv").write_text("kept\n")
        (seed / "Win32").mkdir()
        (seed / "Win32" / "game.exe").write_text("rebuilt, never seeded\n")
        (seed / "rebuild.log").write_text("log\n")
        self.root, self.main = root, main
        binaries = main / "binaries"
        self.patches = [
            mock.patch.object(paths, "MAIN_WORKTREE", main),
            mock.patch.object(paths, "POOL_ROOT", root),
            mock.patch.object(paths, "POOL_PREFIX", "vostok-w"),
            mock.patch.object(paths, "POOL_SIZE", 2),
            mock.patch.object(paths, "POOL_LOCKS", main / ".git" / "vostok-pool"),
            mock.patch.object(paths, "REPO", main),
            mock.patch.object(paths, "BINARIES", binaries),
            mock.patch.object(paths, "BUILD_LOCK", binaries / ".build.lock"),
            mock.patch.object(paths, "REBUILD_LOG", binaries / "rebuild.log"),
            mock.patch.object(paths, "SCRIPTS", main / "scripts"),
            mock.patch.object(paths, "WINEPREFIX", binaries / ".wineprefix"),
            mock.patch.object(paths, "POOL_SEED_EXCLUDES", (binaries / "Win32",)),
            mock.patch.object(worktree, "log", lambda message: None),
        ]
        for patch in self.patches:
            patch.start()

    def tearDown(self):
        for patch in reversed(self.patches):
            patch.stop()
        self.tmp.cleanup()

    def test_claim_creates_lazily_seeds_and_releases_clean(self):
        held = worktree.claim("feature", "agent-a")
        self.assertEqual(held.path, self.root / "vostok-w1")
        self.assertEqual(worktree._current_branch(held.path), "feature")
        self.assertEqual((held.path / "binaries/gen/keep.tsv").read_text(), "kept\n")
        self.assertFalse((held.path / "binaries/Win32").exists())
        self.assertFalse((held.path / "binaries/rebuild.log").exists())
        lock = worktree.read_lock(held.path)
        self.assertEqual((lock["owner"], lock["branch"]), ("agent-a", "feature"))

        other = worktree.claim("v0.10b", "agent-b", detach=True)
        self.assertEqual(other.path, self.root / "vostok-w2")
        self.assertIsNone(worktree.claim("feature", "agent-c"), "pool of two is full")
        again = worktree.claim("feature", "agent-a")
        self.assertEqual(again.path, held.path, "same owner and branch re-claims")

        (held.path / "scratch.txt").write_text("work in progress\n")
        with self.assertRaisesRegex(worktree.PoolError, "uncommitted"):
            worktree.release(held.path, owner="agent-a")
        with self.assertRaisesRegex(worktree.PoolError, "held by agent-a"):
            worktree.release(held.path, owner="agent-b", discard=True)
        ref = worktree.release(held.path, owner="agent-a", save=True)
        self.assertTrue(ref.startswith("refs/vostok-pool/vostok-w1/"))
        saved = subprocess.run(["git", "show", f"{ref}:scratch.txt"], cwd=self.main,
                               capture_output=True, text=True, check=True).stdout
        self.assertEqual(saved, "work in progress\n")
        self.assertEqual(worktree.dirty(held.path), [])
        self.assertEqual(worktree._current_branch(held.path), "HEAD", "released detached")
        self.assertIsNone(worktree.read_lock(held.path))
        self.assertTrue((held.path / "binaries/gen/keep.tsv").is_file(), "binaries kept")

    def test_next_claim_prefers_the_worktree_already_on_the_branch(self):
        first = worktree.claim("feature", "a")
        second = worktree.claim("v0.10b", "b", detach=True)
        worktree.release(first.path, owner="a")
        worktree.release(second.path, owner="b")
        _git(first.path, "switch", "-q", "--detach", "v0.10b")
        _git(second.path, "switch", "-q", "feature")
        self.assertEqual(worktree.claim("feature", "c").path, second.path)

    def test_dirty_free_worktree_is_skipped(self):
        held = worktree.claim("feature", "a")
        worktree.release(held.path, owner="a")
        (held.path / "flake.nix").write_text("edited\n")
        self.assertEqual(worktree.claim("v0.10b", "b", detach=True).path,
                         self.root / "vostok-w2")
        with self.assertRaisesRegex(worktree.PoolError, "dirty"):
            worktree.claim("v0.10b", "c", wanted=held.path)

    def test_stale_claims_are_freed(self):
        held = worktree.claim("feature", "a")
        lock = worktree.read_lock(held.path)
        lock.update(pid=2 ** 22 + 12345, host=socket.gethostname())
        worktree._lock_path(held.path).write_text(json.dumps(lock))
        self.assertEqual(worktree.stale_reason(held.path, lock)[:13], "owner process")
        worktree.release(held.path, force=True)
        held = worktree.claim("feature", "a")
        lock = worktree.read_lock(held.path)
        lock["time"] = time.time() - (worktree.STALE_HOURS + 1) * 3600
        self.assertIn("limit", worktree.stale_reason(held.path, lock))
        lock["time"] = time.time()
        self.assertIsNone(worktree.stale_reason(held.path, lock))

        lock.update(pid=2 ** 22 + 12345, host=socket.gethostname())
        worktree._lock_path(held.path).write_text(json.dumps(lock))
        third = worktree.claim("v0.10b", "b", detach=True)
        self.assertEqual(third.path, held.path, "the dead owner's claim was recovered")

    def test_a_running_build_keeps_a_claim_alive(self):
        held = worktree.claim("feature", "a")
        lock = worktree.read_lock(held.path)
        lock["time"] = 0
        build_lock = held.path / "binaries" / ".build.lock"
        with build_lock.open("a") as handle:
            import fcntl
            fcntl.flock(handle, fcntl.LOCK_EX)
            self.assertIsNone(worktree.stale_reason(held.path, lock))
            with self.assertRaisesRegex(worktree.PoolError, "build is running"):
                worktree.release(held.path, owner="a")

    def test_run_releases_after_failure_and_saves_changes(self):
        code = worktree.run("feature", ["sh", "-c", "echo out > made.txt; exit 7"], owner="a")
        self.assertEqual(code, 7)
        path = self.root / "vostok-w1"
        self.assertIsNone(worktree.read_lock(path))
        self.assertEqual(worktree.dirty(path), [])
        refs = subprocess.run(["git", "for-each-ref", "--format=%(refname)", "refs/vostok-pool"],
                              cwd=self.main, capture_output=True, text=True).stdout.split()
        self.assertEqual(len(refs), 1)

    def test_run_child_sees_the_pool_worktree(self):
        out = self.root / "env.txt"
        worktree.run("feature", ["sh", "-c", f'echo "$VOSTOK_DIR|$PWD|${{CODEX_THREAD_ID:-}}" > {out}'],
                     owner="a")
        vostok_dir, cwd, codex = out.read_text().strip().split("|")
        self.assertEqual(Path(vostok_dir), self.root / "vostok-w1")
        self.assertEqual(Path(cwd).resolve(), (self.root / "vostok-w1").resolve())
        self.assertEqual(codex, "")

    def test_gc_removes_only_clean_pushed_idle_worktrees(self):
        merged, dirty, local = (self.root / name for name in ("merged", "dirty", "local"))
        _git(self.main, "worktree", "add", "-q", "--detach", str(merged), "v0.10b")
        _git(self.main, "worktree", "add", "-q", "--detach", str(dirty), "v0.10b")
        (dirty / "untracked.txt").write_text("x\n")
        _git(self.main, "worktree", "add", "-q", "-b", "local-only", str(local), "v0.10b")
        _git(local, "commit", "-q", "--allow-empty", "-m", "unpushed")
        pool = worktree.claim("feature", "a").path
        rows = {row["path"]: row for row in worktree.gc_candidates(idle_hours=0)}
        self.assertNotIn(pool, rows, "pool worktrees are never gc candidates")
        self.assertTrue(rows[merged]["remove"])
        self.assertIn("dirty", rows[dirty]["why"])
        self.assertIn("no remote", rows[local]["why"])
        busy = {row["path"]: row for row in worktree.gc_candidates(idle_hours=24)}
        self.assertIn("active", busy[merged]["why"])
        worktree.gc(apply=True, idle_hours=0)
        self.assertFalse(merged.exists())
        self.assertTrue(dirty.exists() and local.exists() and pool.exists())


class SharedCacheTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        self.root = root
        self.inputs = {"tool": "v1"}
        self.generated = 0

        def populate(out):
            self.generated += 1
            (out / "sub").mkdir(parents=True)
            (out / "a.lib").write_bytes(b"alpha")
            (out / "sub" / "b.lib").write_bytes(b"beta")

        self.populate = populate
        self.patches = [
            mock.patch.object(paths, "SHARED_CACHE", root / "cache"),
            mock.patch.object(shared_cache, "log", lambda message: None),
        ]
        for patch in self.patches:
            patch.start()

    def tearDown(self):
        for patch in reversed(self.patches):
            patch.stop()
        self.tmp.cleanup()

    @contextlib.contextmanager
    def worktree(self, name, keep_overrides=True):
        tree = self.root / name
        entry = shared_cache.Entry("libs", tree / "prebuilt", lambda: dict(self.inputs),
                                   self.populate, keep_overrides)
        with mock.patch.object(shared_cache, "entries", return_value=(entry,)), \
             mock.patch.object(paths, "SHARED_CACHE_STAMP", tree / ".shared-cache.json"):
            yield entry.path

    def test_worktrees_share_read_only_inodes(self):
        with self.worktree("w1") as one:
            self.assertEqual(shared_cache.sync(), {"libs"})
        with self.worktree("w2") as two:
            shared_cache.sync()
        self.assertEqual(self.generated, 1)
        for rel in ("a.lib", "sub/b.lib"):
            self.assertEqual((one / rel).stat().st_ino, (two / rel).stat().st_ino)
            self.assertFalse((one / rel).stat().st_mode & stat.S_IWUSR)
        with self.assertRaises(PermissionError):
            (one / "a.lib").write_bytes(b"in-place write")

    def test_a_replacing_writer_never_reaches_the_cache(self):
        with self.worktree("w1") as one:
            shared_cache.sync()
            temporary = one / "a.lib.tmp"
            temporary.write_bytes(b"local gfx build")
            os.replace(temporary, one / "a.lib")
            shared_cache.sync()
            self.assertEqual((one / "a.lib").read_bytes(), b"local gfx build", "override kept")
        cached = paths.SHARED_CACHE / "libs" / shared_cache.key_of(self.inputs) / "tree" / "a.lib"
        self.assertEqual(cached.read_bytes(), b"alpha")

    def test_retail_entries_replace_stale_copies_and_prune_extras(self):
        with self.worktree("w1", keep_overrides=False) as one:
            (one / "sub").mkdir(parents=True)
            (one / "a.lib").write_bytes(b"older generator")
            (one / "gone.h").write_bytes(b"no longer produced")
            shared_cache.sync()
            self.assertEqual((one / "a.lib").read_bytes(), b"alpha")
            self.assertFalse((one / "gone.h").exists())
            self.assertEqual((one / "a.lib").stat().st_nlink, 2)

    def test_identical_existing_copy_is_deduplicated(self):
        with self.worktree("w1") as one:
            (one / "sub").mkdir(parents=True)
            (one / "a.lib").write_bytes(b"alpha")
            shared_cache.sync()
            self.assertEqual((one / "a.lib").stat().st_nlink, 2)

    def test_new_inputs_generate_a_new_key_and_damage_is_regenerated(self):
        with self.worktree("w1") as one:
            shared_cache.sync()
            self.inputs["tool"] = "v2"
            shared_cache.sync()
            self.assertEqual(self.generated, 2)
            key = shared_cache.key_of(self.inputs)
            cached = paths.SHARED_CACHE / "libs" / key / "tree" / "a.lib"
            cached.chmod(0o644)
            cached.write_bytes(b"corrupt")
            self.assertEqual(shared_cache.verify_key("libs", key), ["a.lib"])
            self.inputs["tool"] = "v1"
            shared_cache.sync()                     # back to v1: no hashing needed
            self.inputs["tool"] = "v2"
            shared_cache.sync()                     # key changed: verified, regenerated
            self.assertEqual(self.generated, 3)
            self.assertEqual((one / "a.lib").read_bytes(), b"alpha")

    def test_disabled_or_unavailable_inputs_leave_the_worktree_alone(self):
        with self.worktree("w1") as one, \
             mock.patch.dict(os.environ, {"VOSTOK_SHARED_CACHE": "0"}):
            self.assertEqual(shared_cache.sync(), set())
            self.assertFalse(one.exists())
        self.inputs = None
        entry = shared_cache.Entry("libs", self.root / "w3", lambda: None, self.populate, True)
        with mock.patch.object(shared_cache, "entries", return_value=(entry,)), \
             mock.patch.object(paths, "SHARED_CACHE_STAMP", self.root / "stamp.json"):
            self.assertEqual(shared_cache.sync(), set())


if __name__ == "__main__":
    unittest.main()
