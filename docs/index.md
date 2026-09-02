## Index

### Writing matched code
* [binary_matching/matching_guide.md](binary_matching/matching_guide.md) - the matching guide: tools, the SRP projects, tips for reading assembly.
* [binary_matching/MATCHING.md](binary_matching/MATCHING.md) - conventions for the source we generate (naming, alignment, asserts, the `UNKNOWN_EXPRESSION` placeholder, STATE markers, carcass comments, `sushi@`/`claude@` `MATCH/NOTE/TODO` tags). Scoped to the non-optimized `/Od` modules (per target codegen): `game_core`, `network_core`, `logging` active, plus `sound`, `network`, `vfs`, `particle`, `ai`, `ai_navigation`, `fs`, `debug` matchable.
* [binary_matching/agentic_loop.md](binary_matching/agentic_loop.md) - the matching loop (queue, rebuild/diff, compile wiring, missing types, inlining, one commit + PR per unit/batch).
* [binary_matching/assembly_patterns.md](binary_matching/assembly_patterns.md) - the pattern knowledge base entry point: search protocol, per-file schema, tag vocabulary. One file per pattern under [binary_matching/patterns/](binary_matching/patterns/), indexed in [binary_matching/patterns/INDEX.md](binary_matching/patterns/INDEX.md) (grep by `cpp:`/`asm:`/`topic:` tag or symptom; new pattern = new file + one INDEX line).
* [binary_matching/unanswered_questions.md](binary_matching/unanswered_questions.md) - tooling the matching agent wants but does not have yet.

### Build & setup
* [../README.md](../README.md) — Nix/Linux quickstart (the supported workflow).
* [materials.md](materials.md) — recovered SDK material authoring, runtime material/effect/shader interaction, and the map-porting workflow.
* [shaders.md](shaders.md) — shipped shader cache format, permutation recovery, and byte-exact roundtrip validation.
* [build/toolchain-build.md](build/toolchain-build.md) — how the vostok-toolchain is built on Linux/Wine (mspdb DLLs, SP1 via `PATCH=`) and why Wine must be **staging** (cl `/Zi` → C1902 on wine-10.0 stable).
* [build-flags.md](build-flags.md) — per-project compiler-flag comparison (target vs. base) recovered from the PDBs, and which vcproj were built with/without optimizations.
* [target-vs-base.md](target-vs-base.md) — synthesis of every target-vs-base comparison the PDB/EXE tooling makes (PE/Rich header, flags, link order, RTTI): what's the same, what's different, and which differences are actionable.

### Per-module matching notes
Each matched module has its own folder under `binary_matching/` (module notes in `README.md` where one exists).
* [game](binary_matching/game/README.md)
* [game_core](binary_matching/game_core/README.md)
* [network](binary_matching/network/README.md)
* [network_core](binary_matching/network_core/README.md)
* [logging](binary_matching/logging/README.md)
* [scaleform](binary_matching/scaleform/README.md)
* [physics](binary_matching/physics/README.md) (Bullet notes in [binary_matching/bullet/](binary_matching/bullet/README.md))
* [render](binary_matching/render/) — no README yet; the campaign log is [matching_campaign.md](binary_matching/render/matching_campaign.md)
* [sound](binary_matching/sound/) — no README yet

Matchable (`/Od` in the target) but with no folder yet: `vfs`, `particle`, `ai`, `ai_navigation`, `fs`, `debug`.

### Data matching
* [binary_matching/data_matching.md](binary_matching/data_matching.md) — the data lane; per-module dossiers are regenerated into `data_matching/` by every build (not committed), the handoff is [data_matching/data_extent_handoff.md](data_matching/data_extent_handoff.md).

### Matching status tags
Per-function status lives in the ledger (`config/match_state.tsv`):
* `done` - `max` reached 100: the function is matched.
* `inprogress` - not matched yet, still open work.
* `blocked` - something is missing (including target-only functions).
* `parked` - worked and could not be raised further; the row's `note` says why.
