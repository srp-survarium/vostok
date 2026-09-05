# SPDX-License-Identifier: GPL-3.0-or-later

"""Derived Visual Studio solution views used by non-matching game builds."""

from __future__ import annotations

import os
import re
import tempfile
import xml.etree.ElementTree as ET
from copy import deepcopy
from collections.abc import Iterator
from contextlib import contextmanager
from pathlib import Path

from vostok.core import paths


_PROJECT_RE = re.compile(
    r'^Project\("[^"]+"\) = "([^"]+)", "([^"]+)", "(\{[^}]+\})"'
)
_PROJECT_MAPPING_RE = re.compile(
    r"^(\s*)(\{[^}]+\})\.([^=]+?)\.(ActiveCfg|Build\.0) = (.+?)(\r?\n)?$"
)

# These are coherent non-game subsystems: authoring tools plus the incomplete
# bullet diagnostic. Keep the source projects authoritative for later recovery
# and remove the units only from the temporary game view consumed by
# vcproj2ninja.
GAME_EXCLUDED_SOURCES = {
    "animation": frozenset(
        (
            "bi_spline_bone_animation.cpp",
            "bi_spline_skeleton_animation.cpp",
            "check_animation_data.cpp",
            "editor_mixer.cpp",
            "mixing_binary_tree_writer.cpp",
            "mixing_n_ary_tree_dumper.cpp",
        )
    ),
    "maya_animation": frozenset(("discrete_data.cpp",)),
    "game_core": frozenset(("bullet_manager_debug_render.cpp",)),
    "ai_navigation": frozenset(
        (
            "constrained_delaunay_triangulator.cpp",
            "graph_generator.cpp",
            "graph_generator_adjacency_builder.cpp",
            "graph_generator_disconnected_regions_remover.cpp",
            "graph_generator_extruder.cpp",
            "graph_generator_fuser.cpp",
            "graph_generator_generate.cpp",
            "graph_generator_merger.cpp",
            "graph_generator_merger_constraint_collector.cpp",
            "graph_generator_merger_constraint_subdivider.cpp",
            "graph_generator_merger_duplicates_remover.cpp",
            "graph_generator_merger_remove_similar_edges.cpp",
            "graph_generator_path_find_test.cpp",
            "graph_generator_restricted_area_cleaner.cpp",
            "graph_generator_subdivider.cpp",
            "graph_generator_t_junction_remover.cpp",
            "graph_generator_tessellator.cpp",
            "graph_generator_tessellator_coplanar_case.cpp",
            "graph_generator_tessellator_degenerated_cases.cpp",
            "graph_generator_tessellator_generic_case.cpp",
            "graph_generator_tessellator_triangle_subtraction.cpp",
            "graph_generator_test_playground.cpp",
            "graph_generator_triangles_intersection.cpp",
        )
    ),
}


def _project_configurations(source: str) -> dict[str, set[str]]:
    result: dict[str, set[str]] = {}
    for line in source.splitlines():
        match = _PROJECT_RE.match(line)
        if not match:
            continue
        _, relative, guid = match.groups()
        project = paths.SLN.parent / relative.replace("\\", "/")
        if project.suffix.casefold() != ".vcproj" or not project.is_file():
            continue
        text = project.read_text(encoding="windows-1251")
        result[guid.casefold()] = set(
            re.findall(r'<Configuration\s+Name="([^"]+)"', text)
        )
    return result


def _static_project_configuration(current: str, available: set[str]) -> str:
    flavor = current.split("|", 1)[0].casefold()
    if flavor.startswith("release"):
        candidates = ("Release(static)|Win32", "Release (static)|Win32")
    elif flavor.startswith("debug"):
        candidates = ("Debug(static)|Win32", "Debug (static)|Win32")
    else:
        return current
    return next((candidate for candidate in candidates if candidate in available), current)


def static_configuration_view(source: str, base: str) -> tuple[str, str]:
    """Add a derived static solution configuration and return its name/text."""
    flavor, platform = base.split("|", 1)
    alias = f"{flavor} Static|{platform}"
    configurations = _project_configurations(source)
    output: list[str] = []
    derived_mappings: list[str] = []
    in_project_mappings = False
    solution_line = f"\t\t{base} = {base}"
    for line in source.splitlines(keepends=True):
        if line.startswith("\tGlobalSection(ProjectConfigurationPlatforms)"):
            in_project_mappings = True
        if in_project_mappings and line.strip() == "EndGlobalSection":
            output.extend(derived_mappings)
            in_project_mappings = False
        output.append(line)
        if line.rstrip("\r\n") == solution_line:
            ending = "\r\n" if line.endswith("\r\n") else "\n"
            output.append(f"\t\t{alias} = {alias}{ending}")
            continue
        match = _PROJECT_MAPPING_RE.match(line)
        if not match or match.group(3) != base:
            continue
        indent, guid, _, kind, current, ending = match.groups()
        selected = _static_project_configuration(
            current, configurations.get(guid.casefold(), set())
        )
        derived_mappings.append(
            f"{indent}{guid}.{alias}.{kind} = {selected}{ending or ''}"
        )
    return alias, "".join(output)


def dll_dependency_view(source: str) -> str:
    """Add dependencies needed only by the aggregate game DLL graph."""
    projects = {
        name: guid
        for line in source.splitlines()
        if (match := _PROJECT_RE.match(line))
        for name, _, guid in (match.groups(),)
    }
    target = projects["engine_pc_dx11"]
    dependencies = tuple(projects[name] for name in ("network_core", "scaleform"))
    lines = source.splitlines(keepends=True)
    target_line = next(
        index
        for index, line in enumerate(lines)
        if (match := _PROJECT_RE.match(line)) and match.group(3) == target
    )
    section_end = next(
        index
        for index in range(target_line + 1, len(lines))
        if lines[index].strip() == "EndProjectSection"
    )
    present = lines[target_line:section_end]
    ending = "\r\n" if lines[target_line].endswith("\r\n") else "\n"
    additions = [
        f"\t\t{guid} = {guid}{ending}"
        for guid in dependencies
        if not any(guid in line for line in present)
    ]
    lines[section_end:section_end] = additions
    return "".join(lines)


def _without_sources(project: Path, excluded: frozenset[str]) -> ET.ElementTree:
    tree = ET.parse(project)
    removed: set[str] = set()
    for parent in tree.iter():
        for child in list(parent):
            if child.tag != "File":
                continue
            relative = child.attrib.get("RelativePath", "").replace("\\", "/")
            name = relative.rsplit("/", 1)[-1].casefold()
            if name not in excluded:
                continue
            parent.remove(child)
            removed.add(name)
    missing = excluded - removed
    if missing:
        raise ValueError(
            f"game source exclusions missing from {project}: {', '.join(sorted(missing))}"
        )
    return tree


def _add_static_debug_configuration(tree: ET.ElementTree, project: Path) -> None:
    """Derive the one missing Win32 static-Debug third-party configuration."""
    configurations = tree.getroot().find("Configurations")
    if configurations is None:
        raise ValueError(f"project has no configurations: {project}")
    if any(
        item.attrib.get("Name") == "Debug(static)|Win32"
        for item in configurations
    ):
        return
    source = next(
        (
            item
            for item in configurations
            if item.attrib.get("Name") == "Debug|Win32"
        ),
        None,
    )
    if source is None:
        raise ValueError(f"project has no Debug|Win32 configuration: {project}")
    derived = deepcopy(source)
    derived.set("Name", "Debug(static)|Win32")
    compiler = next(
        item
        for item in derived
        if item.tag == "Tool" and item.attrib.get("Name") == "VCCLCompilerTool"
    )
    compiler.set("RuntimeLibrary", "0")
    librarian = next(
        item
        for item in derived
        if item.tag == "Tool" and item.attrib.get("Name") == "VCLibrarianTool"
    )
    librarian.set(
        "OutputFile",
        "$(SolutionDir)../binaries/$(PlatformName)/libraries/"
        "vostok_$(ProjectName)-static-debug.lib",
    )
    configurations.append(derived)

    for parent in tree.iter():
        source_files = [
            item
            for item in parent
            if item.tag == "FileConfiguration"
            and item.attrib.get("Name") == "Debug|Win32"
        ]
        for item in source_files:
            file_configuration = deepcopy(item)
            file_configuration.set("Name", "Debug(static)|Win32")
            parent.append(file_configuration)


@contextmanager
def game_project_views(source: str, linkage: str) -> Iterator[str]:
    """Yield a solution whose temporary projects omit authoring-only units."""
    projects = {
        name: relative
        for line in source.splitlines()
        if (match := _PROJECT_RE.match(line))
        for name, relative, _ in (match.groups(),)
    }
    temporary_projects: list[Path] = []
    try:
        project_names = set(GAME_EXCLUDED_SOURCES)
        if linkage == "static":
            project_names.add("LibFoundation")
        for project_name in project_names:
            excluded = GAME_EXCLUDED_SOURCES.get(project_name, frozenset())
            relative = projects[project_name]
            project = paths.SLN.parent / relative.replace("\\", "/")
            descriptor, name = tempfile.mkstemp(
                prefix=".vostok-game-", suffix=".vcproj", dir=project.parent
            )
            os.close(descriptor)
            temporary = Path(name)
            temporary_projects.append(temporary)
            tree = _without_sources(project, excluded)
            if project_name == "LibFoundation":
                _add_static_debug_configuration(tree, project)
            tree.write(
                temporary, encoding="windows-1251", xml_declaration=True
            )
            replacement = str(temporary.relative_to(paths.SLN.parent)).replace(
                "/", "\\"
            )
            source = source.replace(f'"{relative}"', f'"{replacement}"')
        yield source
    finally:
        for project in temporary_projects:
            project.unlink(missing_ok=True)


@contextmanager
def build_solution(configuration: str, linkage: str) -> Iterator[tuple[Path, str]]:
    """Yield the authoritative solution or a derived game-only view."""
    if configuration == paths.GOLD_CONFIGURATION:
        yield paths.SLN, configuration
        return
    source = paths.SLN.read_text(encoding="utf-8-sig")
    with game_project_views(source, linkage) as game_source:
        if linkage == "static":
            selected, derived = static_configuration_view(game_source, configuration)
        else:
            selected, derived = configuration, dll_dependency_view(game_source)
        descriptor, name = tempfile.mkstemp(
            prefix=f".vostok-game-{linkage}-", suffix=".sln", dir=paths.SLN.parent
        )
        os.close(descriptor)
        temporary = Path(name)
        try:
            temporary.write_text(derived, encoding="utf-8-sig")
            yield temporary, selected
        finally:
            temporary.unlink(missing_ok=True)
