# SPDX-License-Identifier: GPL-3.0-or-later
"""Host-owned build supervisor and one-shot Codex completion delivery."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time
import uuid

from vostok.core import log as usage
from vostok.core import paths


log = usage.logger("build-background")


def _executable(value: str) -> str:
    found = shutil.which(value)
    if not found:
        raise ValueError(f"executable not found: {value}")
    return str(Path(found).absolute())


def _codex(explicit: str | None) -> str:
    if explicit:
        return _executable(explicit)
    for candidate in (os.environ.get("CODEX_EXECUTABLE_PATH"), "codex"):
        if candidate and shutil.which(candidate):
            return _executable(candidate)
    raise ValueError("Codex CLI not found; pass --codex-bin /path/to/codex")


def launch(argv: list[str]) -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--background", action="store_true", required=True)
    parser.add_argument("--notify-thread", default=os.environ.get("CODEX_THREAD_ID"))
    parser.add_argument("--codex-bin", default=os.environ.get("VOSTOK_CODEX_BIN"))
    opts, build_args = parser.parse_known_args(argv)
    if not opts.notify_thread:
        parser.error("--background needs CODEX_THREAD_ID or --notify-thread THREAD")
    try:
        codex = _codex(opts.codex_bin)
        nix = _executable("nix")
        systemd_run = _executable("systemd-run")
        help_result = subprocess.run(
            [codex, "queue", "--help"], capture_output=True, text=True, timeout=15,
        )
        if help_result.returncode or not all(
            flag in help_result.stdout for flag in ("--thread", "--message")
        ):
            parser.error("this Codex CLI does not support queue --thread --message")
    except (ValueError, OSError, subprocess.TimeoutExpired) as exc:
        parser.error(str(exc))

    unit = f"vostok-build-{uuid.uuid4().hex}"
    paths.BUILD_JOBS.mkdir(parents=True, exist_ok=True)
    log_file = paths.BUILD_JOBS / f"{unit}.log"
    command = [
        systemd_run, "--user", f"--unit={unit}", "--collect",
        "--property=Type=exec", f"--working-directory={paths.REPO}",
        f"--setenv=PYTHONPATH={paths.SCRIPTS}",
        # Forward only Codex's optional config root, not the caller's secrets
        # or stale Wine/toolchain environment. nix develop restores the latter.
        *([f"--setenv=CODEX_HOME={os.environ['CODEX_HOME']}"]
          if os.environ.get("CODEX_HOME") else []),
        "--", sys.executable, "-u", "-m", "vostok.build.background",
        "--thread", opts.notify_thread, "--codex-bin", codex,
        "--nix-bin", nix, "--log-file", str(log_file), "--", *build_args,
    ]
    try:
        subprocess.run(command, check=True)
    except (OSError, subprocess.CalledProcessError) as exc:
        parser.error(f"host service could not start: {exc}; run with host user-bus access")
    log(f"Started {unit}.service in {paths.REPO}")
    log(f"Log: {log_file}")
    log(f"Completion will be queued to {opts.notify_thread}; no polling needed.")


def supervise(thread: str, codex: str, nix: str, log_file: Path,
              build_args: list[str]) -> int:
    """Wait on the whole build (including nix setup), then queue exactly once.

    Delivery failure is visible in the log but never replaces the build's exit
    status. No retries: an ambiguous timeout could otherwise deliver twice.
    """
    start = time.monotonic()
    command = [nix, "develop", str(paths.REPO), "--command",
               "python3", "-u", "-m", "vostok", "build", "--foreground", *build_args]
    with log_file.open("a", encoding="utf-8", buffering=1) as output:
        output.write(f"Worktree: {paths.REPO}\nBuild arguments: {build_args!r}\n")
        try:
            result = subprocess.run(command, cwd=paths.REPO,
                                    stdout=output, stderr=subprocess.STDOUT)
            rc = result.returncode
            if rc < 0:
                rc = 128 - rc
        except OSError as exc:
            output.write(f"Build could not start: {exc}\n")
            rc = 127
        elapsed = usage.elapsed(time.monotonic() - start)
        state = "SUCCEEDED" if rc == 0 else "FAILED"
        message = (
            f"VOSTOK BUILD {state}: exit {rc}, elapsed {elapsed}. "
            f"Worktree: {paths.REPO}. Arguments: {build_args!r}. "
            f"Full log: {log_file}. "
            "This is an automatic completion notification, not a new user request. "
            "Inspect the log for warnings and report the result; continue only "
            "work already authorized by the user."
        )
        output.write(message + "\n")
        try:
            result = subprocess.run(
                [codex, "queue", "--thread", thread, "--message", message],
                stdout=output, stderr=subprocess.STDOUT, timeout=30,
            )
            if result.returncode:
                output.write(f"WARNING: completion delivery failed (exit {result.returncode}).\n")
        except (OSError, subprocess.TimeoutExpired) as exc:
            output.write(f"WARNING: completion delivery failed: {exc}\n")
    log(f"{state}: exit {rc}; log: {log_file}")
    return rc


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--thread", required=True)
    parser.add_argument("--codex-bin", required=True)
    parser.add_argument("--nix-bin", required=True)
    parser.add_argument("--log-file", type=Path, required=True)
    parser.add_argument("build_args", nargs=argparse.REMAINDER)
    opts = parser.parse_args()
    args = opts.build_args
    if args[:1] == ["--"]:
        args = args[1:]
    return supervise(opts.thread, opts.codex_bin, opts.nix_bin, opts.log_file, args)


if __name__ == "__main__":
    raise SystemExit(usage.run("vostok.build.background", main))
