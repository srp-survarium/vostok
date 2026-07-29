---
name: vostok-rebuild-library-carcass
description: Clean-rebuild an entire Vostok library or render sublibrary from target PDB structure while preserving the old module as a drainable temp queue. Use when asked to regenerate a module interface, replace a legacy or mostly-excluded carcass, move a module through vostok-structure, drain a structure or legacy queue, or prepare every translation unit for later binary matching. Do not use for ordinary per-function or per-TU matching.
---

# Rebuild a Vostok library carcass

Create a fresh, compilable target-shaped interface while salvaging usable legacy
method bodies into it. Treat both temp trees as synchronized queues: every item
must be reproduced, ported, or explicitly triaged before its queue entry is
removed.

## Read first

Read:

- `AGENTS.md`
- `docs/binary_matching/library_carcass_rebuild.md`
- `docs/binary_matching/MATCHING.md`

For a shared namespace or a legacy-body harvest, also read:

- `docs/binary_matching/game/README.md`
- `docs/binary_matching/game/legacy_harvest.md`

## Establish scope and safety

1. Require explicit authorization for a whole-library replacement. Do not turn
   a routine matching request into a destructive carcass rebuild.
2. Start from a clean, fully rebuilt branch in its own worktree.
3. Define the actual build library and PDB compiland set. For `render`, process
   one sublibrary/project at a time (`facade`, `base`, `core`, platform core,
   `engine`); never treat the entire render umbrella as one queue.
4. Inventory keepers before moving anything: project files, PCH, API/export
   headers, library linkage, entry points, allocator/bootstrap infrastructure.
5. Record the baseline build, match report, enabled TUs, and current source
   tree. Use recoverable Git moves; never erase the only copy of legacy code.

## Create both queues

1. Move the replaceable old module files to
   `temp/<module>_legacy/`. This is the legacy implementation queue.
2. Keep only the inventoried infrastructure in the real module tree.
3. Regenerate canonical target structure:

   ```text
   python3 scripts/generate_structure.py target
   ```

4. Copy the module's generated `headers/` and `sources/` material from
   `binaries/structure/target/` into `temp/<module>_structure_queue/`.
5. For shared namespace pools, seed the whole pool and maintain a triage log.
   Remove an entry only after proving another module owns it or the real tree
   reproduces it.

The structure queue proves target type completeness. The legacy queue supplies
the missing header topology and candidate implementations, and proves that old
material was neither silently lost nor blindly copied.

## Reconstruct top-down

Start at the top of the target ownership and call tree, not at small helpers:

1. Establish public facade roots, entry points, worlds/managers, resource owners,
   and their exact interfaces.
2. Expand each owner through its member types, virtual surface, construction,
   and immediate dependencies.
3. Add leaf helpers only when a reconstructed owner requires them.

This ordering is mandatory for `/O2` + LTCG targets. A helper introduced before
its real owner and call path exists may be inlined, folded, discarded, or emitted
as a COMDAT in the bootstrap TU. That result cannot establish the helper's target
ownership or matching state. Treat early helper output as structural evidence
only and measure it again through the reconstructed owner.

Use the existing per-module anchor system while the real ownership graph is
incomplete. Do not recreate the deprecated monolithic
`temp_include_all.{h,cpp}`/"temp includes" convention:

- put a module's temporary `use_<thing>()` functions in one or more
  `sources/anchor_<module>*.cpp` files compiled by that module's project;
- expose one `vostok::anchor_<module>()` dispatcher for the module;
- declare that dispatcher in `game_core/sources/anchor.h`;
- call it once from `survarium::IncludeAll::IncludeAll()` in
  `game_core/sources/anchor.cpp`;
- keep the single `IncludeAll` instance at the real executable-rooted startup
  path in `game/sources/game_entry_point.cpp`.

Self-guard anchor bodies and source arguments through volatile placeholders so
LTCG keeps the references without executing scaffolding or specializing target
bodies from constants. Never duplicate another module's `use_*` ownership.
Anchors are disposable reachability scaffolding, not target ownership evidence.
Drain each temporary reference into the real top-down call path when that owner
is reconstructed, then retire the empty anchor file and dispatcher call.

Before writing a subsystem, inspect analogous code elsewhere in the engine.
Reuse established conventions for header layering, ownership, allocators,
resource cooks, entry points, containers, naming, assertions, and failure
handling. These analogues are style and architecture priors only: target
structure and symbols remain authoritative for ABI, declarations, and ownership.

## Rebuild headers by reconciling both queues

The generated directory names are record categories, not real file kinds:

- `headers/` is a synthetic namespace-keyed, one-type-record-per-file pool,
  not a recovered include tree;
- `sources/` preserves paths for files that contributed at least one function,
  so it contains source files, headers, and inline/include files.

Function-bearing headers in `sources/` are strong path anchors. Synthetic type
files in `headers/` must be folded into those anchored headers or into
legacy/consumer-derived paths. Therefore never copy generated `headers/`
verbatim into the real module.

The generator implementation also imposes these rules:

- regeneration removes and recreates both generated trees, so seed durable temp
  queues from one completed run before regenerating again;
- `sources/` is produced by walking DBI compilands, grouping functions by their
  recorded filename, stripping the engine prefix, and preserving the remainder;
- when multiple compilands emit the same recorded path, later files receive
  `_1`, `_2`, and further suffixes; reconcile their union by symbol and source
  line rather than treating them as original filenames;
- `headers/` is produced from the global type stream. Its filenames are derived
  from namespace and type names (including truncation and punctuation
  replacement), so a synthetic filename is not provenance for a real header.

For each real header:

1. Select related entries from both queues. Use legacy filenames and includes,
   function-bearing paths from generated `sources/`, source-line records,
   consumer includes, symbol ownership, and nested/type dependencies to
   determine grouping.
2. Reassemble the real header from the generated type records and compiland
   carcasses, using the legacy header as a topology and naming prior. Target
   symbols win when generated and legacy declarations disagree.
3. Preserve exact members, offsets, sizes, access order, signatures, nested
   types, enums, templates, and addressed `FUNCTION BODY[0xVA]` evidence.
4. Make only the transformations and buildability stubs allowed by
   `library_carcass_rebuild.md`. The legacy header may determine file placement
   and include relationships, but never override target layout.
5. Port straightforward related legacy method bodies as the declaration and
   owning header become available. Leave hard bodies queued for later.
6. Delete entries from either queue only when the corresponding real header or
   method is present and compile-validated, or its disposition is recorded.
7. Commit small green batches. Every commit requires a successful full
   `python3 scripts/rebuild.py` and its generated `README.md` and `match.db`.

## Drain the legacy implementation queue

Header reconstruction and body salvage normally interleave. For each remaining
legacy declaration or method:

1. Locate its canonical target declaration in the new interface.
2. If the old body is a plausible ancestor, port only the body, adapting it to
   canonical members and signatures. Keep addressed target evidence and leave
   it unmatched until measured.
3. If it belongs to a removed subsystem, another module, or cannot map
   faithfully, record the disposition in the triage log instead of forcing it.
4. Remove the legacy item from temp only after the body was ported or its
   disposition was recorded. Delete an empty legacy file.
5. Keep the module compiling and linking after every batch.

Do not copy legacy class layouts over the generated interface. Do not call a
ported body matched until the normal target/base loop proves it.

## Enable and verify

1. Drain the structure queue completely.
2. Enable TUs leaf-first in the project file. Re-exclude a failing TU rather
   than leaving the shared build broken, and record the blocker.
3. Add only required per-module anchor references; retire them when real call
   paths replace them.
4. Run a full rebuild and inspect structure, base-only/target-only symbols, and
   `report-changes.json`.
5. Use `$vostok-verify-structure` for the final interface audit, then use
   `$vostok-orchestrate-matching` for body matching.

## Completion gates

- The structure queue is empty.
- The legacy queue is empty; every item was ported or explicitly triaged.
- All expected TUs compile and link, or each remaining exclusion has a concrete
  recorded blocker.
- No addressless carcass or obsolete generated-date/state narration remains.
- The new interface matches target layout and symbol structure.
- The final commit contains a successful full rebuild's README and database.
