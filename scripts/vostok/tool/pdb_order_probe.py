# SPDX-License-Identifier: GPL-3.0-or-later

"""Compile tiny VS2008 programs to attribute PDB order to one input at a time.

The probes use the worktree's isolated Wine prefix and write only generated
artifacts below binaries/gen/pdb-order-probes. They never compile or link game
sources. Each pair keeps the full pdb_topology JSON and the exact linker log.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
from dataclasses import dataclass
from pathlib import Path

from vostok.core import log as _log
from vostok.core.paths import (
    BINARIES,
    PDB_ORDER_PROBE_FIXTURES,
    PDB_ORDER_PROBE_OUTPUT,
    REPO,
    WINEPREFIX,
)
from vostok.core.wine import drive_path


CASES = (
    "object-order",
    "archive-member-order",
    "root-demand-order",
    "library-order",
    "ltcg-object-order",
    "ltcg-archive-member-order",
    "ltcg-root-demand-order",
    "ltcg-indexed-archive-member-order",
    "ltcg-indexed-root-demand-order",
    "ltcg-library-order",
    "ltcg-code-dependency-order",
    "ltcg-data-dependency-order",
    "ltcg-compile-order",
    "ltcg-batch-source-order",
    "ltcg-shared-pdb-compile-order",
    "ltcg-shared-pdb-batch-source-order",
    "header-order",
    "pch-order",
    "post-pch-header-order",
    "post-pch-declaration-order",
    "pch-preinclude-order",
    "pch-boundary-include-order",
    "pch-guard-boundary-order",
    "zi-pch-guard-boundary-order",
    "ltcg-pch-guard-boundary-order",
    "ltcg-pch-batch-source-order",
    "checksum-content-order",
    "ltcg-checksum-content-order",
    "type-contributor-order",
    "type-use-order",
    "pch-composition",
    "pch-archive-retention",
    "function-definition-order",
    "ltcg-function-definition-order",
    "deleting-destructor-demand-order",
    "ltcg-deleting-destructor-demand-order",
    "function-order",
    "module-stream-roster",
    "clean-relink",
    "incremental-relink",
)

SEQUENCE_CHANNELS = (
    "stream_roles",
    "modules",
    "section_contributions",
    "named_types",
    "named_type_module_references",
    "raw_type_records",
    "public_address_map",
    "legacy_fpo_records",
    "frame_data_records",
)

SCOPED_CHANNELS = (
    "module_file_streams",
    "module_line_streams",
    "module_subsection_streams",
)

COMMON_CL = (
    "/nologo",
    "/c",
    "/Z7",
    "/Od",
    "/Ob0",
    "/Oy-",
    "/Gy",
    "/GS-",
    "/EHsc-",
)

COMMON_LINK = (
    "/nologo",
    "/DEBUG",
    "/OPT:NOREF",
    "/OPT:NOICF",
    "/NODEFAULTLIB",
    "/ENTRY:entry",
    "/SUBSYSTEM:CONSOLE",
)

OUTPUT_MARKER = ".vostok-pdb-order-probe"


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _count_rows(channel: dict, key: str) -> int:
    value = channel.get(key, [])
    return value if isinstance(value, int) else len(value)


def summarize_topology(report: dict) -> dict:
    """Reduce a full topology report without discarding the owned raw JSON."""
    channels = {}
    for name in SEQUENCE_CHANNELS:
        channel = report[name]
        metrics = channel["order_metrics"]
        channels[name] = {
            "base_total": channel["base_total"],
            "target_total": channel["target_total"],
            "shared_unique": channel["shared_unique"],
            "inversions": metrics["inversions"],
            "moved": _count_rows(channel, "moved"),
            "changed": _count_rows(channel, "changed"),
            "only_base": _count_rows(channel, "only_base"),
            "only_target": _count_rows(channel, "only_target"),
        }
    for name in SCOPED_CHANNELS:
        channel = report[name]
        comparisons = [scope["comparison"] for scope in channel["streams"]]
        channels[name] = {
            "paired": channel["paired"],
            "different": channel["different"],
            "inversions": sum(
                value["order_metrics"]["inversions"] for value in comparisons
            ),
            "moved": sum(_count_rows(value, "moved") for value in comparisons),
            "changed": sum(_count_rows(value, "changed") for value in comparisons),
            "only_base_scopes": len(channel["only_base_scopes"]),
            "only_target_scopes": len(channel["only_target_scopes"]),
        }
    layout = report["msf_layout"]
    role_changes = sum(
        binding["base"] != binding["target"]
        for binding in layout["stable_roles"]
    )
    return {
        "channels": channels,
        "msf_layout": {
            "base": layout["base"],
            "target": layout["target"],
            "changed_stable_roles": role_changes,
            "unidentified_base": len(layout["unidentified_base"]),
            "unidentified_target": len(layout["unidentified_target"]),
        },
    }


@dataclass
class Snapshot:
    name: str
    pdb: Path
    exe: Path
    log: Path


class ProbeRunner:
    def __init__(self, output: Path, topology: str):
        self.output = output.resolve()
        if self.output in {Path("/"), Path.home().resolve(), REPO, BINARIES}:
            raise SystemExit(f"refusing unsafe probe output directory: {self.output}")
        self.work = self.output / "work"
        self.artifacts = self.output / "artifacts"
        self.reports = self.output / "comparisons"
        self.logs = self.output / "logs"
        self.output_marker = self.output / OUTPUT_MARKER
        self.topology = topology
        msvc_dir = os.environ.get("MSVC_DIR")
        if not msvc_dir:
            raise SystemExit("MSVC_DIR is not set; enter the repository Nix shell")
        self.cl = Path(msvc_dir) / "VC" / "bin" / "cl.exe"
        self.link = Path(msvc_dir) / "VC" / "bin" / "link.exe"
        self.lib = Path(msvc_dir) / "VC" / "bin" / "lib.exe"
        for tool in (self.cl, self.link, self.lib):
            if not tool.is_file():
                raise SystemExit(f"required VS2008 tool is missing: {tool}")
        if not WINEPREFIX.is_dir():
            raise SystemExit(
                f"isolated Wine prefix is missing: {WINEPREFIX}; "
                "run `python3 -m vostok tool toolchain` in this worktree"
            )
        self.env = dict(os.environ)
        self.env["WINEPREFIX"] = str(WINEPREFIX)
        self.env.setdefault("WINEDEBUG", "fixme-all,err-kerberos")
        self.commands: list[dict] = []

    def prepare(self) -> None:
        if self.output.exists():
            if not self.output.is_dir():
                raise SystemExit(
                    f"probe output exists but is not a directory: {self.output}"
                )
            default_output = PDB_ORDER_PROBE_OUTPUT.resolve()
            if self.output != default_output and not self.output_marker.is_file():
                raise SystemExit(
                    "refusing to replace a custom directory not created by "
                    f"pdb-order-probe: {self.output}"
                )
            shutil.rmtree(self.output)
        for directory in (self.work, self.artifacts, self.reports, self.logs):
            directory.mkdir(parents=True, exist_ok=True)
        self.output_marker.write_text("generated; safe to replace\n", encoding="utf-8")

    def reset_work(self, fixture: str, variant: str | None = None) -> None:
        if self.work.exists():
            shutil.rmtree(self.work)
        self.work.mkdir(parents=True)
        source = PDB_ORDER_PROBE_FIXTURES / fixture / "common"
        shutil.copytree(source, self.work, dirs_exist_ok=True)
        if variant:
            overlay = PDB_ORDER_PROBE_FIXTURES / fixture / variant
            shutil.copytree(overlay, self.work, dirs_exist_ok=True)

    def overlay(self, fixture: str, variant: str) -> None:
        source = PDB_ORDER_PROBE_FIXTURES / fixture / variant
        shutil.copytree(source, self.work, dirs_exist_ok=True)

    def run(self, label: str, command: list[str]) -> Path:
        log_path = self.logs / f"{label}.log"
        # mspdbsrv.exe can outlive cl/link and inherit their stdout handle.  A
        # captured pipe then withholds EOF even though the tool and its outputs
        # are complete.  Write directly to the durable command log so waiting
        # is scoped to the invoked Wine process rather than every descendant
        # that inherited a pipe.
        with log_path.open("w", encoding="utf-8") as log_file:
            result = subprocess.run(
                command,
                cwd=self.work,
                env=self.env,
                text=True,
                stdout=log_file,
                stderr=subprocess.STDOUT,
            )
        self.commands.append(
            {
                "label": label,
                "command": command,
                "cwd": str(self.work),
                "returncode": result.returncode,
                "log": str(log_path.relative_to(self.output)),
            }
        )
        if result.returncode:
            raise RuntimeError(
                f"{label} failed with exit code {result.returncode}; see {log_path}"
            )
        return log_path

    def wine(self, label: str, tool: Path, arguments: list[str]) -> Path:
        return self.run(label, ["wine", str(tool), *arguments])

    def compile(
        self,
        label: str,
        source: str,
        output: str,
        extra: tuple[str, ...] = (),
    ) -> Path:
        output_path = self.work / output
        self.wine(
            label,
            self.cl,
            [
                *COMMON_CL,
                *extra,
                f"/Fo{drive_path(output_path)}",
                drive_path(self.work / source),
            ],
        )
        return output_path

    def compile_batch(
        self,
        label: str,
        sources: tuple[str, ...],
        *,
        extra: tuple[str, ...] = (),
    ) -> dict[str, Path]:
        self.wine(
            label,
            self.cl,
            [
                *COMMON_CL,
                *extra,
                f"/Fo{drive_path(self.work)}\\",
                *(drive_path(self.work / source) for source in sources),
            ],
        )
        return {source: self.work / f"{Path(source).stem}.obj" for source in sources}

    def make_lib(
        self,
        label: str,
        output: str,
        objects: list[Path],
        *,
        extra: tuple[str, ...] = (),
    ) -> Path:
        output_path = self.work / output
        output_path.unlink(missing_ok=True)
        self.wine(
            label,
            self.lib,
            [
                "/nologo",
                f"/OUT:{drive_path(output_path)}",
                *extra,
                *map(drive_path, objects),
            ],
        )
        return output_path

    def link_once(
        self,
        label: str,
        inputs: list[Path],
        *,
        extra: tuple[str, ...] = (),
        incremental: bool = False,
        preserve_state: bool = False,
    ) -> Snapshot:
        active_exe = self.work / "probe.exe"
        active_pdb = self.work / "probe.pdb"
        active_ilk = self.work / "probe.ilk"
        if not preserve_state:
            for path in (active_exe, active_pdb, active_ilk):
                path.unlink(missing_ok=True)
        log_path = self.wine(
            label,
            self.link,
            [
                *COMMON_LINK,
                "/INCREMENTAL:YES" if incremental else "/INCREMENTAL:NO",
                f"/OUT:{drive_path(active_exe)}",
                f"/PDB:{drive_path(active_pdb)}",
                *extra,
                *map(drive_path, inputs),
            ],
        )
        snapshot_dir = self.artifacts / label
        snapshot_dir.mkdir(parents=True, exist_ok=True)
        pdb = snapshot_dir / "probe.pdb"
        exe = snapshot_dir / "probe.exe"
        shutil.copy2(active_pdb, pdb)
        shutil.copy2(active_exe, exe)
        return Snapshot(label, pdb, exe, log_path)

    def compare(
        self,
        name: str,
        variable: str,
        baseline: Snapshot,
        variant: Snapshot,
    ) -> dict:
        report_path = self.reports / f"{name}.json"
        result = subprocess.run(
            [
                self.topology,
                "--target-pdb",
                str(baseline.pdb),
                "--base-pdb",
                str(variant.pdb),
                "--order",
                "--json",
            ],
            cwd=self.work,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        if result.returncode:
            raise RuntimeError(
                f"pdb_topology failed for {name}: {result.stderr.strip()}"
            )
        report_path.write_text(result.stdout, encoding="utf-8")
        report = json.loads(result.stdout)
        return {
            "name": name,
            "variable": variable,
            "baseline": {
                "name": baseline.name,
                "pdb_sha256": _sha256(baseline.pdb),
                "exe_sha256": _sha256(baseline.exe),
                "link_log": str(baseline.log.relative_to(self.output)),
            },
            "variant": {
                "name": variant.name,
                "pdb_sha256": _sha256(variant.pdb),
                "exe_sha256": _sha256(variant.exe),
                "link_log": str(variant.log.relative_to(self.output)),
            },
            "topology_report": str(report_path.relative_to(self.output)),
            **summarize_topology(report),
        }

    def basic_objects(
        self,
        fixture: str = "object_order",
        *,
        label_prefix: str | None = None,
        compile_extra: tuple[str, ...] = (),
    ) -> tuple[Path, Path, Path]:
        self.reset_work(fixture)
        prefix = label_prefix or fixture
        root = self.compile(
            f"{prefix}-compile-root", "root.cpp", "root.obj", compile_extra
        )
        alpha = self.compile(
            f"{prefix}-compile-alpha", "alpha.cpp", "alpha.obj", compile_extra
        )
        beta = self.compile(
            f"{prefix}-compile-beta", "beta.cpp", "beta.obj", compile_extra
        )
        return root, alpha, beta

    def object_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects()
        ab = self.link_once("object-order-ab", [root, alpha, beta])
        ba = self.link_once("object-order-ba", [root, beta, alpha])
        return [self.compare("object-order", "final object order", ab, ba)]

    def archive_member_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects()
        archive = self.make_lib("archive-member-ab-lib", "probe.lib", [alpha, beta])
        ab = self.link_once(
            "archive-member-ab", [root, archive], extra=("/VERBOSE",)
        )
        archive = self.make_lib("archive-member-ba-lib", "probe.lib", [beta, alpha])
        ba = self.link_once(
            "archive-member-ba", [root, archive], extra=("/VERBOSE",)
        )
        return [
            self.compare("archive-member-order", "archive member order", ab, ba)
        ]

    def root_demand_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects("root_demand_order")
        archive = self.make_lib("root-demand-lib", "probe.lib", [alpha, beta])
        ab = self.link_once(
            "root-demand-ab", [root, archive], extra=("/VERBOSE",)
        )
        self.overlay("root_demand_order", "ba")
        root = self.compile("root-demand-compile-root-ba", "root.cpp", "root.obj")
        ba = self.link_once(
            "root-demand-ba", [root, archive], extra=("/VERBOSE",)
        )
        return [self.compare("root-demand-order", "root reference order", ab, ba)]

    def library_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects()
        alpha_lib = self.make_lib("library-order-alpha-lib", "alpha.lib", [alpha])
        beta_lib = self.make_lib("library-order-beta-lib", "beta.lib", [beta])
        ab = self.link_once(
            "library-order-ab", [root, alpha_lib, beta_lib], extra=("/VERBOSE",)
        )
        ba = self.link_once(
            "library-order-ba", [root, beta_lib, alpha_lib], extra=("/VERBOSE",)
        )
        return [self.compare("library-order", "final library order", ab, ba)]

    def ltcg_object_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects(
            label_prefix="ltcg-object-order", compile_extra=("/GL",)
        )
        ab = self.link_once("ltcg-object-order-ab", [root, alpha, beta], extra=("/LTCG",))
        ba = self.link_once("ltcg-object-order-ba", [root, beta, alpha], extra=("/LTCG",))
        return [
            self.compare("ltcg-object-order", "final /GL object order", ab, ba)
        ]

    def ltcg_archive_member_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects(
            label_prefix="ltcg-archive-member", compile_extra=("/GL",)
        )
        archive = self.make_lib(
            "ltcg-archive-member-ab-lib", "probe.lib", [alpha, beta]
        )
        ab = self.link_once(
            "ltcg-archive-member-ab",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        archive = self.make_lib(
            "ltcg-archive-member-ba-lib", "probe.lib", [beta, alpha]
        )
        ba = self.link_once(
            "ltcg-archive-member-ba",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        return [
            self.compare(
                "ltcg-archive-member-order",
                "/GL archive member order with fixed root demand",
                ab,
                ba,
            )
        ]

    def ltcg_root_demand_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects(
            "root_demand_order",
            label_prefix="ltcg-root-demand",
            compile_extra=("/GL",),
        )
        archive = self.make_lib(
            "ltcg-root-demand-lib", "probe.lib", [alpha, beta]
        )
        ab = self.link_once(
            "ltcg-root-demand-ab",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        self.overlay("root_demand_order", "ba")
        root = self.compile(
            "ltcg-root-demand-compile-root-ba",
            "root.cpp",
            "root.obj",
            ("/GL",),
        )
        ba = self.link_once(
            "ltcg-root-demand-ba",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        return [
            self.compare(
                "ltcg-root-demand-order",
                "/GL unresolved-root demand with fixed archive",
                ab,
                ba,
            )
        ]

    def ltcg_indexed_archive_member_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects(
            label_prefix="ltcg-indexed-archive-member", compile_extra=("/GL",)
        )
        archive = self.make_lib(
            "ltcg-indexed-archive-member-ab-lib",
            "probe.lib",
            [alpha, beta],
            extra=("/LTCG",),
        )
        ab = self.link_once(
            "ltcg-indexed-archive-member-ab",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        archive = self.make_lib(
            "ltcg-indexed-archive-member-ba-lib",
            "probe.lib",
            [beta, alpha],
            extra=("/LTCG",),
        )
        ba = self.link_once(
            "ltcg-indexed-archive-member-ba",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        return [
            self.compare(
                "ltcg-indexed-archive-member-order",
                "/GL archive member order with a librarian /LTCG index and "
                "fixed root demand",
                ab,
                ba,
            )
        ]

    def ltcg_indexed_root_demand_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects(
            "root_demand_order",
            label_prefix="ltcg-indexed-root-demand",
            compile_extra=("/GL",),
        )
        archive = self.make_lib(
            "ltcg-indexed-root-demand-lib",
            "probe.lib",
            [alpha, beta],
            extra=("/LTCG",),
        )
        ab = self.link_once(
            "ltcg-indexed-root-demand-ab",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        self.overlay("root_demand_order", "ba")
        root = self.compile(
            "ltcg-indexed-root-demand-compile-root-ba",
            "root.cpp",
            "root.obj",
            ("/GL",),
        )
        ba = self.link_once(
            "ltcg-indexed-root-demand-ba",
            [root, archive],
            extra=("/LTCG", "/VERBOSE"),
        )
        return [
            self.compare(
                "ltcg-indexed-root-demand-order",
                "/GL unresolved-root demand with a fixed librarian /LTCG "
                "index",
                ab,
                ba,
            )
        ]

    def ltcg_library_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects(
            label_prefix="ltcg-library-order", compile_extra=("/GL",)
        )
        alpha_lib = self.make_lib("ltcg-library-alpha", "alpha.lib", [alpha])
        beta_lib = self.make_lib("ltcg-library-beta", "beta.lib", [beta])
        ab = self.link_once(
            "ltcg-library-order-ab",
            [root, alpha_lib, beta_lib],
            extra=("/LTCG", "/VERBOSE"),
        )
        ba = self.link_once(
            "ltcg-library-order-ba",
            [root, beta_lib, alpha_lib],
            extra=("/LTCG", "/VERBOSE"),
        )
        return [
            self.compare(
                "ltcg-library-order", "/GL final library order", ab, ba
            )
        ]

    def ltcg_code_dependency_order(self) -> list[dict]:
        snapshots = []
        for variant in ("alpha-to-beta", "beta-to-alpha"):
            self.reset_work("ltcg_code_dependency_order", variant)
            root = self.compile(
                f"ltcg-code-dependency-{variant}-root",
                "root.cpp",
                "root.obj",
                ("/GL",),
            )
            alpha = self.compile(
                f"ltcg-code-dependency-{variant}-alpha",
                "alpha.cpp",
                "alpha.obj",
                ("/GL",),
            )
            beta = self.compile(
                f"ltcg-code-dependency-{variant}-beta",
                "beta.cpp",
                "beta.obj",
                ("/GL",),
            )
            archive = self.make_lib(
                f"ltcg-code-dependency-{variant}-lib",
                "probe.lib",
                [alpha, beta],
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-code-dependency-{variant}",
                    [root, archive],
                    extra=("/LTCG", "/VERBOSE"),
                )
            )
        return [
            self.compare(
                "ltcg-code-dependency-order",
                "direction of one cross-module call with fixed roots and archive",
                *snapshots,
            )
        ]

    def ltcg_data_dependency_order(self) -> list[dict]:
        snapshots = []
        for variant in ("alpha-to-beta", "beta-to-alpha"):
            self.reset_work("ltcg_data_dependency_order", variant)
            root = self.compile(
                f"ltcg-data-dependency-{variant}-root",
                "root.cpp",
                "root.obj",
                ("/GL",),
            )
            alpha = self.compile(
                f"ltcg-data-dependency-{variant}-alpha",
                "alpha.cpp",
                "alpha.obj",
                ("/GL",),
            )
            beta = self.compile(
                f"ltcg-data-dependency-{variant}-beta",
                "beta.cpp",
                "beta.obj",
                ("/GL",),
            )
            archive = self.make_lib(
                f"ltcg-data-dependency-{variant}-lib",
                "probe.lib",
                [alpha, beta],
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-data-dependency-{variant}",
                    [root, archive],
                    extra=("/LTCG", "/VERBOSE"),
                )
            )
        return [
            self.compare(
                "ltcg-data-dependency-order",
                "direction of one cross-module data reference with fixed roots "
                "and archive",
                *snapshots,
            )
        ]

    def ltcg_compile_order(self) -> list[dict]:
        snapshots = []
        for variant, order in (("ab", ("alpha", "beta")), ("ba", ("beta", "alpha"))):
            self.reset_work("object_order")
            root = self.compile(
                f"ltcg-compile-order-{variant}-root",
                "root.cpp",
                "root.obj",
                ("/GL",),
            )
            objects = {}
            for name in order:
                objects[name] = self.compile(
                    f"ltcg-compile-order-{variant}-{name}",
                    f"{name}.cpp",
                    f"{name}.obj",
                    ("/GL",),
                )
            archive = self.make_lib(
                f"ltcg-compile-order-{variant}-lib",
                "probe.lib",
                [objects["alpha"], objects["beta"]],
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-compile-order-{variant}",
                    [root, archive],
                    extra=("/LTCG", "/VERBOSE"),
                )
            )
        return [
            self.compare(
                "ltcg-compile-order",
                "/GL object compilation order with fixed link and archive order",
                *snapshots,
            )
        ]

    def ltcg_batch_source_order(self) -> list[dict]:
        snapshots = []
        for variant, order in (
            ("ab", ("alpha.cpp", "beta.cpp")),
            ("ba", ("beta.cpp", "alpha.cpp")),
        ):
            self.reset_work("object_order")
            root = self.compile(
                f"ltcg-batch-source-order-{variant}-root",
                "root.cpp",
                "root.obj",
                ("/GL",),
            )
            objects = self.compile_batch(
                f"ltcg-batch-source-order-{variant}-batch",
                order,
                extra=("/GL",),
            )
            archive = self.make_lib(
                f"ltcg-batch-source-order-{variant}-lib",
                "probe.lib",
                [objects["alpha.cpp"], objects["beta.cpp"]],
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-batch-source-order-{variant}",
                    [root, archive],
                    extra=("/LTCG", "/VERBOSE"),
                )
            )
        return [
            self.compare(
                "ltcg-batch-source-order",
                "source order within one /GL compiler invocation with fixed "
                "link and archive order",
                *snapshots,
            )
        ]

    def ltcg_shared_pdb_compile_order(self) -> list[dict]:
        snapshots = []
        for variant, order in (("ab", ("alpha", "beta")), ("ba", ("beta", "alpha"))):
            self.reset_work("object_order")
            compiler_pdb = f"/Fd{drive_path(self.work / 'vc90.pdb')}"
            debug_options = ("/GL", "/Zi", "/FD", compiler_pdb)
            root = self.compile(
                f"ltcg-shared-pdb-compile-order-{variant}-root",
                "root.cpp",
                "root.obj",
                debug_options,
            )
            objects = {}
            for name in order:
                objects[name] = self.compile(
                    f"ltcg-shared-pdb-compile-order-{variant}-{name}",
                    f"{name}.cpp",
                    f"{name}.obj",
                    debug_options,
                )
            archive = self.make_lib(
                f"ltcg-shared-pdb-compile-order-{variant}-lib",
                "probe.lib",
                [objects["alpha"], objects["beta"]],
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-shared-pdb-compile-order-{variant}",
                    [root, archive],
                    extra=("/LTCG", "/VERBOSE"),
                )
            )
        return [
            self.compare(
                "ltcg-shared-pdb-compile-order",
                "/GL /Zi compilation chronology in one shared compiler PDB with "
                "fixed link and archive order",
                *snapshots,
            )
        ]

    def ltcg_shared_pdb_batch_source_order(self) -> list[dict]:
        snapshots = []
        for variant, order in (
            ("ab", ("alpha.cpp", "beta.cpp")),
            ("ba", ("beta.cpp", "alpha.cpp")),
        ):
            self.reset_work("object_order")
            compiler_pdb = f"/Fd{drive_path(self.work / 'vc90.pdb')}"
            debug_options = ("/GL", "/Zi", "/FD", compiler_pdb)
            root = self.compile(
                f"ltcg-shared-pdb-batch-source-order-{variant}-root",
                "root.cpp",
                "root.obj",
                debug_options,
            )
            objects = self.compile_batch(
                f"ltcg-shared-pdb-batch-source-order-{variant}-batch",
                order,
                extra=debug_options,
            )
            archive = self.make_lib(
                f"ltcg-shared-pdb-batch-source-order-{variant}-lib",
                "probe.lib",
                [objects["alpha.cpp"], objects["beta.cpp"]],
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-shared-pdb-batch-source-order-{variant}",
                    [root, archive],
                    extra=("/LTCG", "/VERBOSE"),
                )
            )
        return [
            self.compare(
                "ltcg-shared-pdb-batch-source-order",
                "source order inside one /GL /Zi compiler invocation sharing "
                "one compiler PDB, with fixed link and archive order",
                *snapshots,
            )
        ]

    def header_order(self) -> list[dict]:
        self.reset_work("header_order", "ab")
        probe = self.compile("header-order-compile-ab", "probe.cpp", "probe.obj")
        ab = self.link_once("header-order-ab", [probe])
        self.overlay("header_order", "ba")
        probe = self.compile("header-order-compile-ba", "probe.cpp", "probe.obj")
        ba = self.link_once("header-order-ba", [probe])
        return [self.compare("header-order", "direct include order", ab, ba)]

    def pch_order(self) -> list[dict]:
        snapshots = []
        for variant in ("ab", "ba"):
            self.reset_work("pch_order", variant)
            pch = self.compile(
                f"pch-order-create-{variant}",
                "pch.cpp",
                "pch.obj",
                ("/Ycpch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            probe = self.compile(
                f"pch-order-use-{variant}",
                "probe.cpp",
                "probe.obj",
                ("/Yupch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            snapshots.append(self.link_once(f"pch-order-{variant}", [pch, probe]))
        return [
            self.compare("pch-order", "PCH header include order", *snapshots)
        ]

    def post_pch_header_order(self) -> list[dict]:
        snapshots = []
        for variant in ("ab", "ba"):
            self.reset_work("post_pch_header_order", variant)
            pch = self.compile(
                f"post-pch-header-order-create-{variant}",
                "pch.cpp",
                "pch.obj",
                ("/Ycpch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            probe = self.compile(
                f"post-pch-header-order-use-{variant}",
                "probe.cpp",
                "probe.obj",
                ("/Yupch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            snapshots.append(
                self.link_once(f"post-pch-header-order-{variant}", [pch, probe])
            )
        return [
            self.compare(
                "post-pch-header-order",
                "nested direct-header order after consuming a fixed PCH",
                *snapshots,
            )
        ]

    def post_pch_declaration_order(self) -> list[dict]:
        snapshots = []
        for variant in ("declarations-first", "definitions-first"):
            self.reset_work("post_pch_declaration_order", variant)
            pch = self.compile(
                f"post-pch-declaration-order-create-{variant}",
                "pch.cpp",
                "pch.obj",
                ("/Ycpch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            probe = self.compile(
                f"post-pch-declaration-order-use-{variant}",
                "probe.cpp",
                "probe.obj",
                ("/Yupch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            snapshots.append(
                self.link_once(
                    f"post-pch-declaration-order-{variant}", [pch, probe]
                )
            )
        return [
            self.compare(
                "post-pch-declaration-order",
                "declaration-only versus function-bearing header order after a PCH",
                *snapshots,
            )
        ]

    def pch_preinclude_order(self) -> list[dict]:
        snapshots = []
        for variant in ("transitive", "preinclude"):
            self.reset_work("pch_preinclude_order", variant)
            pch = self.compile(
                f"pch-preinclude-order-create-{variant}",
                "pch.cpp",
                "pch.obj",
                ("/Ycpch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            probe = self.compile(
                f"pch-preinclude-order-use-{variant}",
                "probe.cpp",
                "probe.obj",
                ("/Yupch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            snapshots.append(
                self.link_once(f"pch-preinclude-order-{variant}", [pch, probe])
            )
        return [
            self.compare(
                "pch-preinclude-order",
                "transitively late PCH header explicitly included before its umbrella",
                *snapshots,
            )
        ]

    def pch_boundary_include_order(self) -> list[dict]:
        snapshots = []
        for variant in ("after", "before"):
            self.reset_work("pch_boundary_include_order", variant)
            pch = self.compile(
                f"pch-boundary-include-order-create-{variant}",
                "pch.cpp",
                "pch.obj",
                ("/Ycpch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            probe = self.compile(
                f"pch-boundary-include-order-use-{variant}",
                "probe.cpp",
                "probe.obj",
                ("/Yupch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            snapshots.append(
                self.link_once(
                    f"pch-boundary-include-order-{variant}", [pch, probe]
                )
            )
        return [
            self.compare(
                "pch-boundary-include-order",
                "redundant direct include after versus before the PCH boundary",
                *snapshots,
            )
        ]

    def _pch_guard_boundary_order(self, *, zi: bool, ltcg: bool) -> list[dict]:
        if ltcg:
            case = "ltcg-pch-guard-boundary-order"
        elif zi:
            case = "zi-pch-guard-boundary-order"
        else:
            case = "pch-guard-boundary-order"
        snapshots = []
        for variant in ("after", "before"):
            self.reset_work("pch_guard_boundary_order", variant)
            compiler_pdb = f"/Fd{drive_path(self.work / 'vc90.pdb')}"
            debug_options = ("/Zi", "/FD", compiler_pdb) if zi else ()
            codegen_options = ("/O2", "/Ob2", "/Oy", "/GL") if ltcg else ()
            pch = self.compile(
                f"{case}-create-{variant}",
                "pch.cpp",
                "pch.obj",
                (
                    *debug_options,
                    *codegen_options,
                    "/Ycpch.h",
                    f"/Fp{drive_path(self.work / 'probe.pch')}",
                ),
            )
            probe = self.compile(
                f"{case}-use-{variant}",
                "probe.cpp",
                "probe.obj",
                (
                    *debug_options,
                    *codegen_options,
                    "/Yupch.h",
                    f"/Fp{drive_path(self.work / 'probe.pch')}",
                ),
            )
            snapshots.append(
                self.link_once(
                    f"{case}-{variant}",
                    [pch, probe],
                    extra=("/LTCG",) if ltcg else (),
                )
            )
        return [
            self.compare(
                case,
                f"traditional include-guard header repeated after versus before "
                f"the {'/Zi /GL ' if ltcg else '/Zi ' if zi else ''}PCH boundary",
                *snapshots,
            )
        ]

    def pch_guard_boundary_order(self) -> list[dict]:
        return self._pch_guard_boundary_order(zi=False, ltcg=False)

    def zi_pch_guard_boundary_order(self) -> list[dict]:
        return self._pch_guard_boundary_order(zi=True, ltcg=False)

    def ltcg_pch_guard_boundary_order(self) -> list[dict]:
        return self._pch_guard_boundary_order(zi=True, ltcg=True)

    def ltcg_pch_batch_source_order(self) -> list[dict]:
        snapshots = []
        for variant, order in (
            ("probe-first", ("probe.cpp", "alpha.cpp", "beta.cpp")),
            ("probe-last", ("alpha.cpp", "beta.cpp", "probe.cpp")),
        ):
            self.reset_work("pch_batch_source_order")
            compiler_pdb = f"/Fd{drive_path(self.work / 'vc90.pdb')}"
            compile_extra = (
                "/Zi",
                "/FD",
                "/MP",
                compiler_pdb,
                "/O2",
                "/Ob2",
                "/Oy",
                "/GL",
            )
            pch = self.compile(
                f"ltcg-pch-batch-source-order-{variant}-create",
                "pch.cpp",
                "pch.obj",
                (
                    *compile_extra,
                    "/Ycpch.h",
                    f"/Fp{drive_path(self.work / 'probe.pch')}",
                ),
            )
            objects = self.compile_batch(
                f"ltcg-pch-batch-source-order-{variant}-batch",
                order,
                extra=(
                    *compile_extra,
                    "/Yupch.h",
                    f"/Fp{drive_path(self.work / 'probe.pch')}",
                ),
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-pch-batch-source-order-{variant}",
                    [
                        pch,
                        objects["probe.cpp"],
                        objects["alpha.cpp"],
                        objects["beta.cpp"],
                    ],
                    extra=("/LTCG",),
                )
            )
        return [
            self.compare(
                "ltcg-pch-batch-source-order",
                "source position inside one /MP /Zi /GL PCH-consuming batch "
                "with fixed final object order",
                *snapshots,
            )
        ]

    def _checksum_content_order(self, *, ltcg: bool) -> list[dict]:
        case = "ltcg-checksum-content-order" if ltcg else "checksum-content-order"
        compile_extra = ("/O2", "/Oy", "/GL") if ltcg else ()
        link_extra = ("/LTCG",) if ltcg else ()
        snapshots = {}
        for variant in ("baseline", "header-comment", "source-comment"):
            self.reset_work(
                "checksum_content_order",
                None if variant == "baseline" else variant,
            )
            probe = self.compile(
                f"{case}-{variant}-compile",
                "probe.cpp",
                "probe.obj",
                compile_extra,
            )
            snapshots[variant] = self.link_once(
                f"{case}-{variant}", [probe], extra=link_extra
            )
        return [
            self.compare(
                f"{case}-header",
                f"harmless header-comment checksum change under "
                f"{'/GL' if ltcg else 'ordinary compilation'}",
                snapshots["baseline"],
                snapshots["header-comment"],
            ),
            self.compare(
                f"{case}-source",
                f"harmless source-comment checksum change under "
                f"{'/GL' if ltcg else 'ordinary compilation'}",
                snapshots["baseline"],
                snapshots["source-comment"],
            ),
        ]

    def checksum_content_order(self) -> list[dict]:
        return self._checksum_content_order(ltcg=False)

    def ltcg_checksum_content_order(self) -> list[dict]:
        return self._checksum_content_order(ltcg=True)

    def type_contributor_order(self) -> list[dict]:
        root, alpha, beta = self.basic_objects("type_contributor_order")
        ab = self.link_once("type-contributor-ab", [root, alpha, beta])
        ba = self.link_once("type-contributor-ba", [root, beta, alpha])
        return [
            self.compare(
                "type-contributor-order",
                "which object first contributes a shared type",
                ab,
                ba,
            )
        ]

    def type_use_order(self) -> list[dict]:
        snapshots = []
        for variant in ("ab", "ba"):
            self.reset_work("type_use_order", variant)
            probe = self.compile(
                f"type-use-compile-{variant}", "probe.cpp", "probe.obj"
            )
            snapshots.append(self.link_once(f"type-use-{variant}", [probe]))
        return [
            self.compare(
                "type-use-order",
                "first complete-type use within one translation unit",
                *snapshots,
            )
        ]

    def pch_composition(self) -> list[dict]:
        snapshots = []
        for variant in ("pch", "direct"):
            self.reset_work("pch_composition", variant)
            pch = self.compile(
                f"pch-composition-create-{variant}",
                "pch.cpp",
                "pch.obj",
                ("/Ycpch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            probe = self.compile(
                f"pch-composition-use-{variant}",
                "probe.cpp",
                "probe.obj",
                ("/Yupch.h", f"/Fp{drive_path(self.work / 'probe.pch')}"),
            )
            snapshots.append(
                self.link_once(f"pch-composition-{variant}", [pch, probe])
            )
        return [
            self.compare(
                "pch-composition",
                "same header provided by the PCH or included directly",
                *snapshots,
            )
        ]

    def pch_archive_retention(self) -> list[dict]:
        comparisons = []
        for debug_mode in ("z7", "zi"):
            snapshots = []
            for export_mode in ("plain", "exported"):
                self.reset_work("pch_archive_retention", export_mode)
                compiler_pdb = f"/Fd{drive_path(self.work / 'vc90.pdb')}"
                debug_options = (
                    ("/Zi", "/FD", "/MP", compiler_pdb)
                    if debug_mode == "zi"
                    else ()
                )
                pch = self.compile(
                    f"pch-archive-retention-{debug_mode}-create-{export_mode}",
                    "pch.cpp",
                    "pch.obj",
                    (
                        *debug_options,
                        "/Ycpch.h",
                        f"/Fp{drive_path(self.work / 'probe.pch')}",
                    ),
                )
                consumer = self.compile(
                    f"pch-archive-retention-{debug_mode}-use-{export_mode}",
                    "consumer.cpp",
                    "consumer.obj",
                    (
                        *debug_options,
                        "/Yupch.h",
                        f"/Fp{drive_path(self.work / 'probe.pch')}",
                    ),
                )
                root = self.compile(
                    f"pch-archive-retention-{debug_mode}-root-{export_mode}",
                    "root.cpp",
                    "root.obj",
                    debug_options,
                )
                archive = self.make_lib(
                    f"pch-archive-retention-{debug_mode}-lib-{export_mode}",
                    "input.lib",
                    [pch, consumer],
                )
                snapshots.append(
                    self.link_once(
                        f"pch-archive-retention-{debug_mode}-{export_mode}",
                        [root, archive],
                        extra=("/VERBOSE",),
                    )
                )
            comparisons.append(
                self.compare(
                    f"pch-archive-retention-{debug_mode}",
                    f"plain versus dllexport inline definition in an "
                    f"otherwise identical /{debug_mode.upper()} PCH",
                    *snapshots,
                )
            )
        return comparisons

    def function_definition_order(self) -> list[dict]:
        snapshots = []
        for variant in ("ab", "ba"):
            self.reset_work("function_definition_order", variant)
            root = self.compile(
                f"function-definition-order-{variant}-root",
                "root.cpp",
                "root.obj",
                ("/O2", "/Oy"),
            )
            functions = self.compile(
                f"function-definition-order-{variant}-functions",
                "functions.cpp",
                "functions.obj",
                ("/O2", "/Oy"),
            )
            snapshots.append(
                self.link_once(
                    f"function-definition-order-{variant}", [root, functions]
                )
            )
        return [
            self.compare(
                "function-definition-order",
                "definition order of two /Gy functions in one source file",
                *snapshots,
            )
        ]

    def ltcg_function_definition_order(self) -> list[dict]:
        snapshots = []
        for variant in ("ab", "ba"):
            self.reset_work("function_definition_order", variant)
            root = self.compile(
                f"ltcg-function-definition-order-{variant}-root",
                "root.cpp",
                "root.obj",
                ("/O2", "/Oy", "/GL"),
            )
            functions = self.compile(
                f"ltcg-function-definition-order-{variant}-functions",
                "functions.cpp",
                "functions.obj",
                ("/O2", "/Oy", "/GL"),
            )
            snapshots.append(
                self.link_once(
                    f"ltcg-function-definition-order-{variant}",
                    [root, functions],
                    extra=("/LTCG",),
                )
            )
        return [
            self.compare(
                "ltcg-function-definition-order",
                "definition order of two /Gy /GL functions in one source file",
                *snapshots,
            )
        ]

    def deleting_destructor_demand_order(self) -> list[dict]:
        return self._deleting_destructor_demand_order(ltcg=False)

    def ltcg_deleting_destructor_demand_order(self) -> list[dict]:
        return self._deleting_destructor_demand_order(ltcg=True)

    def _deleting_destructor_demand_order(self, *, ltcg: bool) -> list[dict]:
        mode = "ltcg-" if ltcg else ""
        compile_extra = (
            ("/O2", "/Oy", "/GR-", "/GL")
            if ltcg
            else ("/O2", "/Oy", "/GR-")
        )
        link_extra = ("/LTCG", "/OPT:ICF") if ltcg else ("/OPT:ICF",)
        snapshots = []
        for variant in ("scalar-first", "vector-first"):
            self.reset_work("deleting_destructor_order", variant)
            item = self.compile(
                f"{mode}deleting-destructor-{variant}-item",
                "item.cpp",
                "item.obj",
                compile_extra,
            )
            root = self.compile(
                f"{mode}deleting-destructor-{variant}-root",
                "root.cpp",
                "root.obj",
                compile_extra,
            )
            snapshots.append(
                self.link_once(
                    f"{mode}deleting-destructor-{variant}",
                    [root, item],
                    extra=link_extra,
                )
            )
        return [
            self.compare(
                f"{mode}deleting-destructor-demand-order",
                f"{'/GL ' if ltcg else ''}scalar-delete versus vector-delete call order with /OPT:ICF",
                *snapshots,
            )
        ]

    def function_order(self) -> list[dict]:
        self.reset_work("function_order")
        probe = self.compile(
            "function-order-compile",
            "probe.cpp",
            "probe.obj",
            ("/O2", "/Oy"),
        )
        ab = self.link_once(
            "function-order-ab",
            [probe],
            extra=(f"/ORDER:@{drive_path(self.work / 'order_ab.txt')}",),
        )
        ba = self.link_once(
            "function-order-ba",
            [probe],
            extra=(f"/ORDER:@{drive_path(self.work / 'order_ba.txt')}",),
        )
        return [self.compare("function-order", "linker /ORDER file", ab, ba)]

    def module_stream_roster(self) -> list[dict]:
        self.reset_work("stream_roster")
        root = self.compile(
            "module-stream-roster-compile-root", "root.cpp", "root.obj"
        )
        alpha = self.compile(
            "module-stream-roster-compile-alpha", "alpha.cpp", "alpha.obj"
        )
        beta = self.compile(
            "module-stream-roster-compile-beta", "beta.cpp", "beta.obj"
        )
        two_modules = self.link_once(
            "module-stream-roster-two", [root, alpha]
        )
        three_modules = self.link_once(
            "module-stream-roster-three", [root, alpha, beta]
        )
        return [
            self.compare(
                "module-stream-roster",
                "one additional direct object and DBI module stream",
                two_modules,
                three_modules,
            )
        ]

    def clean_relink(self) -> list[dict]:
        root, alpha, beta = self.basic_objects()
        first = self.link_once("clean-relink-first", [root, alpha, beta])
        second = self.link_once("clean-relink-second", [root, alpha, beta])
        return [
            self.compare(
                "clean-relink",
                "no input change; output PDB and EXE removed before relink",
                first,
                second,
            )
        ]

    def incremental_relink(self) -> list[dict]:
        root, alpha, beta = self.basic_objects()
        clean = self.link_once("incremental-clean-control", [root, alpha, beta])
        first = self.link_once(
            "incremental-relink-first", [root, alpha, beta], incremental=True
        )
        second = self.link_once(
            "incremental-relink-second",
            [root, alpha, beta],
            incremental=True,
            preserve_state=True,
        )
        third = self.link_once(
            "incremental-relink-third",
            [root, alpha, beta],
            incremental=True,
            preserve_state=True,
        )
        replay_first = self.link_once(
            "incremental-replay-first", [root, alpha, beta], incremental=True
        )
        replay_second = self.link_once(
            "incremental-replay-second",
            [root, alpha, beta],
            incremental=True,
            preserve_state=True,
        )
        replay_third = self.link_once(
            "incremental-replay-third",
            [root, alpha, beta],
            incremental=True,
            preserve_state=True,
        )
        return [
            self.compare(
                "clean-vs-incremental",
                "linker /INCREMENTAL mode with identical objects",
                clean,
                first,
            ),
            self.compare(
                "incremental-relink-step-1-to-2",
                "no input change; second link preserves PDB/ILK state",
                first,
                second,
            ),
            self.compare(
                "incremental-relink-step-2-to-3",
                "no input change; third link preserves PDB/ILK state",
                second,
                third,
            ),
            self.compare(
                "incremental-relink-cycle-1-to-3",
                "first versus third update; tests a possible two-step cycle",
                first,
                third,
            ),
            self.compare(
                "incremental-replay-step-1",
                "equivalent first updates from two fresh incremental states",
                first,
                replay_first,
            ),
            self.compare(
                "incremental-replay-step-2",
                "equivalent second updates from two fresh incremental states",
                second,
                replay_second,
            ),
            self.compare(
                "incremental-replay-step-3",
                "equivalent third updates from two fresh incremental states",
                third,
                replay_third,
            ),
        ]


def _parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--case",
        action="append",
        choices=CASES,
        dest="cases",
        help="run only this experiment (repeatable; default: all)",
    )
    parser.add_argument(
        "--list", action="store_true", help="list experiment names and exit"
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=PDB_ORDER_PROBE_OUTPUT,
        help=f"generated evidence directory (default: {PDB_ORDER_PROBE_OUTPUT})",
    )
    parser.add_argument(
        "--pdb-topology",
        default=os.environ.get("PDB_TOPOLOGY", "pdb_topology"),
        help="pdb_topology binary (default: PDB_TOPOLOGY or PATH)",
    )
    return parser


def main(argv: list[str] | None = None) -> int:
    args = _parser().parse_args(argv)
    if args.list:
        print("\n".join(CASES))
        return 0
    selected = args.cases or list(CASES)
    runner = ProbeRunner(args.output, args.pdb_topology)
    runner.prepare()
    results = []
    for case in selected:
        print(f"[pdb-order-probe] {case}", flush=True)
        results.extend(getattr(runner, case.replace("-", "_"))())
    manifest = {
        "schema": 1,
        "cases": selected,
        "toolchain": {
            "cl": str(runner.cl),
            "link": str(runner.link),
            "lib": str(runner.lib),
            "pdb_topology": runner.topology,
            "wineprefix": str(WINEPREFIX),
        },
        "repository": {
            "root": str(REPO),
            "head": subprocess.check_output(
                ["git", "rev-parse", "HEAD"], cwd=REPO, text=True
            ).strip(),
            "fixture_sha256": {
                str(path.relative_to(PDB_ORDER_PROBE_FIXTURES)): _sha256(path)
                for path in sorted(PDB_ORDER_PROBE_FIXTURES.rglob("*"))
                if path.is_file()
            },
        },
        "commands": runner.commands,
        "experiments": results,
    }
    output = runner.output / "results.json"
    output.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"[pdb-order-probe] wrote {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.pdb_order_probe", main))
