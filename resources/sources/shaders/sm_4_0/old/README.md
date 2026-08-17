# Pre-ship shader sources

These 84 files are the recovered source drop's *own* revisions, kept out of
the working tree so they cannot be mistaken for ship truth. Nothing here
compiles to a shipped v0.100b blob. Two kinds:

* **shaders the build never shipped** — `accum_sun_*`, `g_base*`,
  `geometry_material_pass`, `terrain_material_pass*`, the skin/organic
  family and the rest of the pre-deferred generation. `resources.db` carries
  no blob under these names at all, so there is nothing to prove them
  against;
* **drifted revisions of shipped names** — the file is one (or several)
  revisions away from what was compiled into the database, and no
  reconstruction has started on it. `python3 -m vostok.shaders roundtrip`
  scores these as `skip` now rather than `DIFF`, which is the honest state:
  the name has no ship-proven source yet.

A few carry mechanical edits from the tree-wide sweeps that fixed
`common_samplers.h`/`common_cbuffers.h` fallout (a removed local
`screen_res`, `CONFIG_TSPECULAR_INTENSITY` renamed to
`CONFIG_TSPECULAR_COLOR`); they are still pre-ship revisions.

They stay because they are the best *reading aid* for a reconstruction —
variable names, comments and intent survive in them even where the
arithmetic has moved. Read them, then trust the disassembly:
`binaries/shaders/disasm/` is ship truth and `roundtrip` is the acceptance
test. See `docs/shaders.md`.

When a name here is reconstructed and round-trips byte-identical, the new
file lands in `sm_4_0/` and this copy stays as history.
