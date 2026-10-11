# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok - the tooling that builds the Vostok Engine (offline lane).

    sources/ --build--> binaries/Win32/survarium-dx11-win32-gold.exe

Layers (imports point strictly downward):

    core/     repo paths, logging, Wine path helpers
    build/    driving the toolchain: the ninja graph and ninja under Wine
    tool/     toolchain setup and library staging
    cli       the umbrella dispatch (python3 -m vostok)

`scripts/` is the package root and the dev shell puts it on PYTHONPATH, so
every module runs as `python3 -m vostok.<pkg> ...` from anywhere in the tree.
The binary-matching tooling lives on the v0.10b branch.
"""
