#!/usr/bin/env python3
"""
wire_gfx_libs_sln.py - Register the 8 sibling GFx libs in `vostok v2.0.sln`.

Mirrors how libgfx is wired (gen_libgfx_vcproj + the sln edits the pilot made):
  1. A Project(...) entry per lib in the project list.
  2. A `<guid>.Master Gold|Win32.{ActiveCfg,Build.0} = Master Gold|Win32` pair in
     GlobalSection(ProjectConfigurationPlatforms).
  3. A ProjectDependencies edge from game_core to each lib's GUID (puts the lib in
     the exe build cone so vcproj2ninja emits its .ninja + rsp).

Idempotent: skips a lib already present. Run after gen_gfx_lib_vcproj.py.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gfx_libs as G

SLN = G.VOSTOK_DIR / "sources" / "vostok v2.0.sln"
SOLUTION_PROJECT_GUID = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
GAME_CORE_GUID = "{DF4E4F25-7D8B-4CB6-B5F4-95DFB333CC29}"
ANCHOR_GUID = "{B9F3A1C2-7E4D-4A18-9C56-2D8F1B3E64A0}"  # libgfx, our insert anchor


def main():
    text = SLN.read_text(encoding="utf-8-sig")
    nl = "\n"

    # 1. Project entries - insert right after libgfx's EndProject.
    proj_anchor = (f'Project("{SOLUTION_PROJECT_GUID}") = "libgfx", '
                   f'"vostok\\libgfx\\sources\\libgfx.vcproj", "{ANCHOR_GUID}"'
                   f'{nl}EndProject{nl}')
    if proj_anchor not in text:
        raise SystemExit("libgfx Project anchor not found in sln")
    new_projects = ""
    for name, guid in G.GUIDS.items():
        entry = (f'Project("{SOLUTION_PROJECT_GUID}") = "{name}", '
                 f'"vostok\\{name}\\sources\\{name}.vcproj", "{guid}"'
                 f'{nl}EndProject{nl}')
        if entry not in text:
            new_projects += entry
    text = text.replace(proj_anchor, proj_anchor + new_projects, 1)

    # 2. ProjectConfigurationPlatforms - after libgfx's Build.0 line.
    cfg_anchor = (f'\t\t{ANCHOR_GUID}.Master Gold|Win32.ActiveCfg = Master Gold|Win32{nl}'
                  f'\t\t{ANCHOR_GUID}.Master Gold|Win32.Build.0 = Master Gold|Win32{nl}')
    if cfg_anchor not in text:
        raise SystemExit("libgfx config-platform anchor not found in sln")
    new_cfgs = ""
    for guid in G.GUIDS.values():
        pair = (f'\t\t{guid}.Master Gold|Win32.ActiveCfg = Master Gold|Win32{nl}'
                f'\t\t{guid}.Master Gold|Win32.Build.0 = Master Gold|Win32{nl}')
        if pair not in text:
            new_cfgs += pair
    text = text.replace(cfg_anchor, cfg_anchor + new_cfgs, 1)

    # 3. game_core ProjectDependencies - add an edge on each lib GUID next to libgfx's.
    dep_anchor = f'\t\t{ANCHOR_GUID} = {ANCHOR_GUID}{nl}'
    if dep_anchor not in text:
        raise SystemExit("game_core->libgfx dependency anchor not found in sln")
    new_deps = ""
    for guid in G.GUIDS.values():
        edge = f'\t\t{guid} = {guid}{nl}'
        if edge not in text:
            new_deps += edge
    text = text.replace(dep_anchor, dep_anchor + new_deps, 1)

    SLN.write_text(text, encoding="utf-8-sig")
    print(f"sln wired: {len(G.GUIDS)} libs (projects + config-platforms + game_core deps)")


if __name__ == "__main__":
    main()
