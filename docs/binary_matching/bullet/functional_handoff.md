# Handoff — Bullet functional (body) matching

**For:** the next agent picking up Bullet matching after the structural commit.
**Predecessor commit:** "bullet: structural delta vs target — add
RayResultCallback::getShapeId". Read [README.md](README.md)
first — it is the structural map this builds on.

## STATUS (2026-06-14) — the genuine vostok source changes are recovered

All of A–D plus C's one real edit are landed (commits on `match/bullet-local-changes`,
after `815dd337`). The remaining sub-100% bullet functions are **toolchain residuals
(SSE-vs-scalar drift on byte-identical `btDbvt.h` / LTCG inline+constant-prop), NOT
source-fixable** — do not chase them.

| Task | Result |
|---|---|
| **A** `objectQuerySingle` BVH rewrite | DONE — all 11 function-local symbols (`input_params`, `LocalInfoAdder` __l46/__l57, `VolumeTester`) 100% / STRUCTURE MATCH. The `objectQuerySingle` body itself stays objdiff-UNPAIRED: target LTCG dead-eliminates the `dbvt==NULL` else loop (keeping only its __l57 `LocalInfoAdder` COMDAT, which our live loop reproduces) and inlines the swept-volume math deeper. Not steerable. |
| **B** `btAllocDefault`/`btFreeDefault` | Source CORRECT (route through `g_crt_allocator->malloc_impl/free_impl` + lazy `initialize_crt_allocator`). Residual is an inline-vs-call wall: target keeps `initialize_crt_allocator` out-of-line, our LTCG inlines it (preinitialize + bind_pointer_to_buffer_mt_safe). ~2% but faithful. |
| **C** `btDbvtBroadphase::rayTest` | DONE — added `BT_PROFILE("btDbvtBroadphase::rayTest")` (the missing stmt). 58%→100%. The TU is checksum-DIFF only because of this edit. |
| **C** getBroadphaseAabb/setAabb/collide/performDeferredRemoval | SSE-vs-scalar toolchain drift on the byte-identical `btDbvt.h` volume ops (Merge/Intersect/min-max → minps/maxps/cmpltps in target, scalar in ours). performDeferredRemoval's "extra OR statement" is just the inlined SSE `Intersect` mask reduction, NOT a logic change. Leave. |
| **D** `integrateTransforms` | DONE — vostok ships the CCD velocity-clamp branch ENABLED (our `#if 0` → `#if 1`): linVel clamp + re-predict + `printf("sm2=%f\n")`, NO `resolveSingleCollision`. 75%→99% STRUCTURE MATCH (4-byte LTCG residual). |
| **E** `btSoftRigidDynamicsWorld::rayTestSingle` | 92%, statement-grouping/SSE residual in the normal computation; left as low-value. |
| **F** `btBvhTriangleMeshShape` 3-arg ctor | ~99.7% LTCG constant-prop: target proves `useQuantizedAabbCompression=true` / `buildBvh=false` whole-program and drops the `if(buildBvh) buildOptimizedBvh()` branch. Not steerable. |

Net: overall fuzzy 52.43%→52.48%, bullet module weighted 96.30%→96.41%. (Cross-module
exact-count wobble across these commits is ICF fold-rep churn, not regression.)

**Mission:** bring the remaining Bullet function *bodies* to a byte match with the
target — but ONLY where the source is genuinely Survarium-modified. A large share
of Bullet's sub-100% functions sit in source files that are **byte-identical to the
original**; their residual is toolchain/LTCG, NOT source-fixable, and must be left
alone. Read the rules in `docs/binary_matching/MATCHING.md` and
`docs/binary_matching/agentic_loop.md` before starting; the conventions there win
over this note.

---

## Rule #1 — apply the source-checksum filter before touching anything

The target PDB records a per-source-file checksum. `pdb_diff --source-dir` hashes
our on-disk source and compares it, printing **MATCH/DIFF per compiland**:

```
P=$(nix build .#vostok-pdb-parser --no-link --print-out-paths)
TGT=$(readlink -f binaries/nix-store/survarium-game)/survarium.pdb   # gcroot
"$P/bin/pdb_diff" --target-pdb "$TGT" --target-engine-path c:/survarium/sources \
  --source-dir sources | grep '^DIFF.*bullet/'
```

- **checksum-MATCH compiland** ⇒ its source is byte-identical to the original.
  Any objdiff residual there is **VS2008 RTM(21022)-vs-SP1(30729) toolchain drift
  (see `docs/target-vs-base.md` §1) or LTCG** — DO NOT try to "match" it from
  source. (Proof: `btSequentialImpulseConstraintSolver.cpp` is MATCH yet has 17
  byte-diffs — all toolchain.)
- **checksum-DIFF compiland** ⇒ the `.cpp` *or a header it includes* differs.
  This is where the real work is. A header is byte-identical (untouched) if ANY
  MATCH compiland includes it — so the stock containers (`btDbvt.h`,
  `btHashMap.h`, `btOverlappingPairCache.h`) are toolchain residual too.

> The raw "171 functions below 100%" headline is ~4× the real source-fixable work.
> Do not quote it as the backlog.

---

## The actual worklist (de-noised, prioritized)

12 compilands are checksum-DIFF; **5 of them carry zero body work** — `btcollisionobject.cpp`,
`btsimulationislandmanager.cpp`, `btgimpactquantizedbvh.cpp`,
`btpolyhedralcontactclipping.cpp`, `btconvexhullcomputer.cpp` are DIFF only from
cosmetic source text / a transitively-included touched header; their functions
already byte-match. Skip them. The real targets:

### A. `btCollisionWorld.cpp` — the marquee task
- **`objectQuerySingle` REWRITE** (currently UNPAIRED — not in our source yet).
  Survarium replaced the stock linear `for (getNumChildShapes())` compound-shape
  loop with a `btDbvt` BVH traversal, adding three function-local classes
  `input_params`, `VolumeTester` (a `btDbvt::ICollide` with `Process(int)` +
  `Process(btDbvtNode const*)`), and a per-child `LocalInfoAdder`, and
  instantiating `btDbvt::collideTV<VolumeTester>`. **Mirror the existing
  `rayTestSingle`/`RayTester` pattern already in our source** — it is the same
  shape. This single rewrite pairs ~9 currently-UNPAIRED symbols at once.
- `debugDrawObject` (37%), `rayTestSingle` internals (`LocalInfoAdder2` 58%,
  `RayTester::Process` 81%, `BridgeTriangleRaycastCallback::reportHit` 94%),
  `objectQuerySingle::BridgeTriangleConvexcastCallback::reportHit` (97%),
  `rayTestSingle` (97%), `debugDrawWorld`/`updateSingleAabb` (99%).

### B. `btAlignedAllocator.cpp` — `btAllocDefault` / `btFreeDefault` (2%)
Survarium routes both through a lazily-initialized
`vostok::memory::g_crt_allocator` (`doug_lea_mt_allocator`):
`if (!g_crt_allocator) initialize_crt_allocator(); g_crt_allocator->malloc_impl/free_impl(...)`.
Our source still has the older `vostok::debug::preinitialize` +
`bind_pointer_to_buffer_mt_safe<doug_lea_mt_allocator,…>` path. Same signatures,
different body. (`btAlignedAllocSetCustomAligned` is `/OPT:REF`-stripped on both
sides — ignore.)

### C. `btDbvtBroadphase.cpp` (5 fns)
`getBroadphaseAabb` (21%), `rayTest` (58%), `setAabb` (83%), `collide` (88%),
`performDeferredRemoval` (91%). Pure body diffs.

### D. `btDiscreteDynamicsWorld.cpp` — `integrateTransforms` (75%)
The CCD (continuous-collision) sweep-clamp block. Target instantiates
`btClosestNotMeConvexResultCallback` **directly** for the sweep; our source wraps
it in a local `StaticOnlyCallback` subclass. (`debugDrawConstraint`/`debugDrawWorld`/
`stepSimulation` are at 99% — trivial tail.)

### E. `btSoftRigidDynamicsWorld.cpp` — `rayTestSingle` (92%, one statement); `debugDrawWorld` (99%).

### F. `btBvhTriangleMeshShape.cpp` — the 3-arg ctor (80%, body).

**SKIP as toolchain/LTCG residual (checksum-MATCH source, do NOT edit):**
`btSequentialImpulseConstraintSolver.cpp` (17), `btSoftBodyHelpers.cpp`,
`btGImpactCollisionAlgorithm.cpp`, `btConeTwistConstraint.cpp`, `btDbvt.cpp`,
`btCompoundCollisionAlgorithm.cpp`, `btConvexConvexAlgorithm.cpp`, `btSoftBody.cpp`,
`btContinuousConvexCollision.cpp`, and all stock header templates
(`btDbvt.h` ×13, `btOverlappingPairCache.h` ×10, `btHashMap.h` ×6, …).

**Ignore as noise (never real work):** `??_E…`/`??_G…` vector/scalar deleting
destructors, `vftable`, `dynamic initializer|atexit for 'X'` — compiler thunks
that show UNPAIRED purely from ICF fold-rep churn (`btQuickprof.cpp`'s only
sub-100 entries are these — it has no real work left).

---

## Tooling & environment

- **Parser binaries** (path changes on flake bump — resolve fresh):
  `P=$(nix build .#vostok-pdb-parser --no-link --print-out-paths)` → `$P/bin/{pdb_fetch,pdb_diff,pdb_parser}`.
- **PDBs:** base `binaries/Win32/survarium-dx11-win32-gold.pdb`; target
  `$(readlink -f binaries/nix-store/survarium-game)/survarium.pdb`.
- **Per-function structure diff (do this FIRST for every function — structure
  before %):**
  `"$P/bin/pdb_fetch" --target-index binaries/rich/target/index.jsonl \
     --base-index binaries/rich/base/index.jsonl --function '<demangled substr>' \
     --view structure-diff` (also `--view target` for the target asm/body,
  `--view diff` with `--objdiff-target-dir/-base-dir binaries/objdiff/{target,base}`
  for the operand-aware byte %).
- **Build + rescore:** `nix develop --command python3 -m vostok build`
  (~10 min Wine build, then regen delink/structure/rich + ledger + README).
- **objdiff report:** `binaries/objdiff/report.json` (per-function
  `fuzzy_match_percent`).

### Two environment traps that already bit this work
1. **Stale target ⇒ phantom score crash.** If `vostok build` prints
   `WARNING: target index is >7 days older than base` or the base delink logs
   `no target symbol map … yet; emitting local defaults`, the score will smear
   DOWN across EVERY module (a phantom ~-1% / -800 exact) — NOT a regression.
   Fix before believing any drop:
   `cd scripts && python3 vostok.build.generate_delink target && python3 vostok.build.generate_rich target \
    && python3 vostok.build.generate_structure target && python3 vostok.build.generate_delink base \
    && python3 vostok derive refresh && python3 vostok ledger readme --write-readme`.
   See memory `per-worktree-target-staleness`.
2. **Don't bank build-artifact churn.** Everything under `binaries/` is gitignored;
   only `README.md` + `config/match_state.tsv` are tracked. Commit source
   per-TU; let a clean rebuild own the README/ledger refresh.

### Tools that DON'T work here (don't waste time)
- `pdb_diff --base-pdb` (the header-level base-vs-target checksum compare) is
  **broken** in the pinned parser (clap forbids `--base-engine-path` next to
  `--base-pdb`, code unwraps it → panic at `pdb_diff.rs:92`). If you want exact
  per-header touched verdicts (to classify the 88 header-attributed diffs), fix
  that one unwrap first — it's the cleanest way to finish the header triage.
- `llvm-pdbutil` cannot read these PDBs (`Too many directory blocks`, BigMSF).

---

## Workflow conventions (from MATCHING.md / agentic_loop.md)
- **Reproduce the target exactly** — never "fix" a bug; the disassembly decides.
- **Structure first**: get `pdb_fetch --view structure-diff` to `STRUCTURE MATCH`
  (statement quantity + per-statement size) before chasing the last %; loop to
  100% or until only an LTCG **argument-passing** residual remains.
- **LTCG is an excuse ONLY for argument passing** at the call boundary — register
  vs stack slot, or a dropped const arg. Everything else (frame size, slot
  assignment, switch shape, inline-vs-call) is a source problem; a missing
  `ASSERT` emits `call empty_stub` (~0xc bytes) and must be recovered.
- One commit per TU; record residuals as a `claude@NOTE:` above the function +
  a ledger park with the concrete next step. **Never merge** without an explicit
  order naming the PR.
- Suggested order: A (objectQuerySingle — unlocks the most) → B (allocator, tiny
  + high-value) → C/D → E/F → the 99% tails.

---

## Done already (don't redo)
- `RayResultCallback::getShapeId` virtual — added, 100% byte / STRUCTURE MATCH
  in that predecessor commit. Its field `m_shape_id` already existed.
- No new fields, no new bullet-library classes (verified — see
  README.md).
