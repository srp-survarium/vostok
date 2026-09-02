# Survarium's local changes to the Bullet library

`sources/bullet/` is **Bullet 2.79** (`BT_BULLET_VERSION 279`) with Survarium's
own modifications layered on top. This note catalogs the **structural** delta
between the shipped game (`target`) and our reconstruction (`base` /
`sources/bullet/`): new functions, new fields, new classes. **Function-body
(behavioural) differences are deferred to a follow-up commit** — see
[Functional follow-ups](#functional-follow-ups-next-commit).

> TL;DR — the bullet library's structure is almost entirely reconstructed
> already. The **only** declaration-level addition still missing from our source
> was one virtual, **`btCollisionWorld::RayResultCallback::getShapeId`**, applied
> in this commit. **No new fields and no new bullet-library classes** remain.

## Method

Two authoritative, fully-local signals, cross-checked against each other:

1. **Functions** — the rich per-function inventory
   `binaries/rich/{target,base}/index.jsonl` (demangled `name`, `mangled`, `rva`,
   `size`, `statements`). Key on the demangled **`name`**: `rva` is shared by
   COMDAT-folded (ICF) functions and `mangled` is the linker's fold-representative,
   which differs between the two builds by link order — keying on `mangled`
   manufactures false target-only/base-only pairs.
2. **Fields / layout** — `STATIC_SIZE_ASSERT(<Type>, 0xNN)` lines under
   `binaries/structure/{target,base}/headers/`. The **target** side emits the full
   set (570 `bt*` types); the **base** side is generated with
   `--skip-non-engine-headers`, so it omits third-party (bullet) asserts (only the
   10 `vostok::physics` `bt_*` wrappers it instantiates appear). The target asserts
   are therefore the ground truth; a class whose target `sizeof` exceeds the layout
   our `sources/bullet/` header implies has gained a field.

Whole-TU confirmation: `pdb_diff --source-dir` compares the **target PDB's
declarations** against our on-disk source and emits MATCH/DIFF per TU. It cannot
compile bodies, so a DIFF is a declaration-level (structural) divergence *or* a
transitive consequence of an included header that changed. Of 77 bullet TUs, **65
MATCH and 12 DIFF**; drilling each DIFF with `pdb_fetch --view structure-diff`
plus a per-class size-assert check isolates the real structural change.

### Noise that masquerades as a structural change
- **`scalar deleting destructor` / `vftable` / `dynamic initializer|atexit for`**
  (`??_G`, `??__E`, `??__F`): compiler-emitted vtable/CRT thunks. They appear,
  vanish, or fold to different addresses between two optimized builds; a
  target-only thunk is an emission artifact, not a source edit.
- **Lexical-scope numbers shift**: `objectQuerySingle'::`46`` vs ``'47'`` and
  `__l45` vs `__l47` are the *same* anonymous class across builds.
- **ICF / inlining**: a stock helper (`btSimdDot3`, `btDbvtAabbMm::FromMM`) can be
  target-only purely because our build inlined it.

After filtering, the raw 33 target-only / 36 base-only function names collapse to a
single genuine addition.

## Structural delta (this commit)

### NEW VIRTUAL — `btCollisionWorld::RayResultCallback::getShapeId`

```cpp
virtual unsigned int getShapeId(unsigned int local_shape_id)
{
    return m_shape_id == (unsigned int)(-1) ? local_shape_id : m_shape_id;
}
```

- **Vtable slot**: appended **after** `addSingleResult` (target vtable order:
  `~RayResultCallback`, `needsCollision`, `addSingleResult`, `getShapeId`).
- **Evidence**: target carcass
  `binaries/structure/target/headers/others/btCollisionWorld__RayResultCallback.h`
  declares it; the target body (`pdb_fetch --function RayResultCallback::getShapeId
  --view target`) is `mov eax,[ecx+14h]; cmp eax,-1; jne .1; mov eax,[esp+4]; .1: ret 4`
  — i.e. return `m_shape_id` unless it is `(u32)-1`, then the caller's `local_shape_id`.
- The backing field **`m_shape_id`** (offset `0x14`) already existed in our source
  (it carried a `sushi@TODO`); only the accessor virtual was missing. The shape-id
  override is consumed by the `vostok::physics` ray callbacks
  (`closest_ray_result_callback`, etc., which live outside `sources/bullet/`).
- Adding a virtual grows the vtable of `RayResultCallback` and every derived ray
  callback — this is the intended match (the target has it).

### No new fields
Every `bt*` class that exists on both sides has an identical `sizeof`. The 10
bullet asserts the base build does emit match the target byte-for-byte, and each
DIFF-TU class was checked member-by-member (`btCollisionObject` 0x110,
`btDiscreteDynamicsWorld` 0x110, `btDynamicsWorld` 0xAC, `btSoftRigidDynamicsWorld`
0x1B0, `btDbvtBroadphase` 0x9C, `btBvhTriangleMeshShape` 0x60, `btConvexPolyhedron`
0xA0, `btConvexHullInternal` 0x80, `CProfileNode` 0x24, …). A field change forces
its defining TU to DIFF; all 12 DIFFs were accounted for without one.

### No new bullet-library classes
The size-assert scan turns up `bt*` types absent from `sources/bullet/`, but none
is a Survarium addition *to the bullet library*:
- **`bt_*` (snake_case)** — `bt_static_rigid_body`, `bt_ghost_object`,
  `bt_animated_rigid_body`, `bt_character_controller`, `bt_collision_shape`,
  `bt_constraint`, `bt_rigid_body_base`, `bt_rigid_body_construction_info`,
  `bt_soft_body_rope`, `btBvhTriangleMeshShapeResource` — Survarium wrappers that
  live in **`sources/vostok/physics/`**, not the bullet library.
- **`btCollisionWorld::objectQuerySingle::__l45::input_params` / `__l47::VolumeTester`**
  — *function-local* classes introduced by the `objectQuerySingle` body rewrite
  (functional, below).
- **`btTetrahedronShapeEx`, `btGImpactTriangleCallback`, `btPlaneShape`,
  `btGImpactCompoundShape::CompoundPrimitiveManager`,
  `btGImpactMeshShapePart::TrimeshPrimitiveManager`** — **stock** Bullet 2.79
  gimpact classes that our source trimmed; their presence in the target's type
  info is upstream Bullet, not a Survarium edit.

## Survarium structural mods already reconstructed in our source
Listed so the catalog is complete — these need no action (already MATCH):
- `RayResultCallback::m_flags` and `m_shape_id` fields.
- `btCollisionObject::m_checkCollideWith` + `checkCollideWithOverride`.
- `CProfileManager::dumpAll`, `CProfileManager::set_log_callback`,
  `CProfileNode::m_userPtr`, the `physics_log_fn` profiling hook.
- `btConvexHullInternal::vertexList`.

## Functional follow-ups (next commit)
The other 11 DIFF TUs carry **body-only** differences (no declaration change). In
rough order of remaining work (objdiff fuzzy %):

| TU | residual | nature |
|---|---|---|
| `btCollisionWorld::objectQuerySingle` | new BVH compound-sweep path | rewritten to a `btDbvt` traversal with function-local `input_params` / `VolumeTester` (`btDbvt::ICollide`) / per-child `LocalInfoAdder`, instantiating `btDbvt::collideTV<VolumeTester>` — mirrors the existing `rayTestSingle`/`RayTester` pattern. Our source still has the stock linear `for (getNumChildShapes())` loop. |
| `btAlignedAllocator` `btAllocDefault`/`btFreeDefault` | ~75% | route through a lazily-initialized `vostok::memory::g_crt_allocator` (`doug_lea_mt_allocator`) + `initialize_crt_allocator`; our source uses the older `preinitialize` + `bind_pointer_to_buffer_mt_safe` path. |
| `btDbvtBroadphase` (`getBroadphaseAabb`, `rayTest`, `setAabb`, `collide`, `performDeferredRemoval`) | ~86% | body diffs. |
| `btDiscreteDynamicsWorld::integrateTransforms` | CCD block | target uses `btClosestNotMeConvexResultCallback` directly for the sweep; our source wraps it in a local `StaticOnlyCallback`. |
| `btQuickprof` | ~95% | `physics_log_fn` wiring (the Survarium logging hook is already declared; body differs). |
| `btConvexHullComputer` | ~99% | body. |
| `btBvhTriangleMeshShape` ctor | ~99.7% | body. |
| `btSoftRigidDynamicsWorld::rayTestSingle` | 1 stmt | body. |
| `btGImpactQuantizedBvh`, `btPolyhedralContactClipping` | 100% | already matched; the pdb_diff DIFF flag is stale. |
| `btCollisionObject`, `btSimulationIslandManager` | — | byte-match; their DIFF is transitive (they `#include` the changed `btCollisionWorld.h`). |

## Reproduce
```bash
P=$(nix build .#vostok-pdb-parser --no-link --print-out-paths)
TGT=/nix/store/…/survarium/survarium.pdb   # gcroot binaries/nix-store/survarium-game

# whole-library TU verdict (declaration-level)
"$P/bin/pdb_diff" --target-pdb "$TGT" --target-engine-path c:/survarium/sources \
  --source-dir sources | grep '^DIFF.*bullet/'

# function inventory diff, keyed on demangled name (filter ICF/lexical/thunk noise)
for s in target base; do jq -r 'select(.file|test("^bullet/"))|.name' \
  binaries/rich/$s/index.jsonl | sort -u > /tmp/$s.bt; done
comm -23 /tmp/target.bt /tmp/base.bt          # target-only functions

# field/layout (target is ground truth; base omits bullet asserts)
grep -rh 'STATIC_SIZE_ASSERT(bt' binaries/structure/target/headers/ | sort -u

# per-function structure diff / target body
"$P/bin/pdb_fetch" --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl \
  --function 'RayResultCallback::getShapeId' --view structure-diff
```
