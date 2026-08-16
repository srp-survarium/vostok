"""vostok.build - driving the era toolchain, and the artifacts it produces.

The loop, in order:

    ninja_regen   .vcprojs + sources -> binaries/ninja/build.ninja, write-if-
                  changed so a no-op regen bumps no mtimes and rebuilds nothing.
                  Includes are tracked, so editing a header recompiles its TUs.
    ninja         run that graph under Wine, with the watchdog that stops a
                  stalled mspdbsrv from holding the build pipe for ten minutes.
    generate_delink     the linked exe -> per-unit COFF, both sides, + report.json
    generate_structure  each PDB -> rendered headers/statements (pdb_parser)
    generate_rich       each PDB+EXE -> the pdb_fetch index
    rebuild             all of the above, then the derive + ledger refresh.
                        THE canonical build: a module-only build does not relink
                        and cannot establish a match score.

`gfx`/`gfx_mspdbsrv` build the from-source GFx suite, which is separate from
the engine graph and driven directly rather than through ninja.

The target side (the original game) does not change between builds; it is
generated once by `vostok.tool.toolchain` at first dev-shell entry.
"""
