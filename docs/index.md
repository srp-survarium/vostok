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
Each matched module has its own folder under `binary_matching/` (module notes in `README.md`).
* [game_core](binary_matching/game_core/README.md)
* [network_core](binary_matching/network_core/README.md)
* [network](binary_matching/network/README.md)
* [logging](binary_matching/logging/README.md)

Matchable (`/Od` in the target) but not started — no folder yet: `sound`, `vfs`, `particle`, `ai`, `ai_navigation`, `fs`, `debug`.

Parked — optimized, not 1:1-matchable (under `next_binary_matching/`):
* [physics](next_binary_matching/physics.md)

### Matching dept (general)
* `static` functions are not delinked by the ghidra script.
* Delinker extensions fails for `vostok/core` functions.
* `pdb-parser` script is missing `static` keyword for static functions.
* `pdb-parser` script is missing typedefs for `pstr`, `pvoid`.

### Matching status tags
* `DONE` - the function is matched. No further attention is required.
* `PARTIAL` - the function is matched, but there are some unanswered questions.
* `SKIPPED` - the function is not matched. I tried and failed. Will return to it later.
* `BLOCKED` - the function is not matched, since it requires some other function to be present/matched first.
* `INPROGRESS` - the function is being matched.
* `STUB` - the function is in the state of the being matching.
