# collision_geometry.cpp - structure-verifier pass

Verifier (v2) pass over `sources/vostok/game_core/sources/collision_geometry.cpp`
on `chain/game_core-sv-collision_geometry`. Re-confirmed every function's % against
a fresh `report.json` (rebuilt), corrected the stale STATE markers, deleted the
matcher carcasses, and embedded condensed structure-diffs for the non-100% ones.

## Stale markers vs fresh report.json

The STATE markers carried old percents; after rebuild the real numbers are:

| function                     | old marker | report.json | outcome |
|------------------------------|-----------:|------------:|---------|
| collision_geometry (ctor)    |     83.12% |      88.12% | wall    |
| ~collision_geometry (dtor)   |     99.74% |      99.74% | wall    |
| destroy_ghost_object         |      100%  |     100.00% | clean   |
| load                         |     82.66% |      96.04% | wall    |
| get_overlapping_objects_count|      100%  |     100.00% | clean   |
| get_overlapping_objects      |     74.69% |     100.00% | now clean |
| contact_test (predicate)     |      100%  |     100.00% | clean   |
| contact_test ()              |      100%  |     100.00% | clean   |
| get_shapes_centers           |     93.33% |      93.33% | wall    |
| insert                       |      100%  |     100.00% | clean   |
| remove                       |      100%  |     100.00% | clean   |
| subscribe                    |      100%  |     100.00% | clean   |
| unsubscribe                  |     91.57% |      91.38% | wall    |
| set_transform                |      100%  |     100.00% | clean   |
| get_transform                |      100%  |     100.00% | clean   |

`get_overlapping_objects` was the orchestrator's "main candidate" at a stale 74.69%;
it is now a clean 100% (the ASSERT empty-stub call's COMDAT-fold misname -
`boost::function1<...>::dummy::nonnull` in base vs `fixed_size_allocator<...>::finalize_impl`
in target - is a reloc-level fold and is scored as matched by report.json's
fuzzy_match_percent; that misname appears identically in every function in the unit,
including the 100% ones).

## The five walls (all STRUCTURE MATCH, non-steerable)

Every non-100% function aligns 1:1 on statements (`quantity-diffs 0`). The residuals
are all whole-program LTCG/ICF artifacts, never a source-shape divergence:

### ctor (88.12%) - base-class ctor inline-vs-call
Target emits `call vostok::collision::game_object::game_object` (out-of-line, target
rva 0x9bbb0) for the base subobject; base INLINES it (writes the `game_object` vtable
pointer directly). `collision::game_object::game_object()` is present in the target
rich index (0x9bbb0) but ABSENT from the base index - it was inlined whole-program.
`collision` is an optimized / out-of-scope module, so we cannot steer its codegen.
This is the documented "base-class ctor: out-of-line call vs inlined trivial init"
pattern (decided by the base class's declaration, not the derived ctor's body). The
inline cascades the frame size (0x18 base vs 0x24 target) and all `[ebp-N]` slot
renames.

### dtor (99.74%) - frame-slot allocation + ASSERT misname
Two ASSERTs present (`[ebp-1]`, `[ebp-2]`), `destroy_ghost_object()` called, vector
member dtor + intrusive base dtor emitted. The only diffs are frame 0x3c (base) vs
0x38 (target) and the ASSERT empty-stub COMDAT-fold misname. Slot allocation is
documented allocation noise; the misname is a reloc fold. Non-steerable.

### load (96.04%) - LOG_WARNING internals + operator misnames
SIZE diffs on the `LOG_WARNING`, `create_compound_shape`, `create_ghost_object` and
`set_no_delete` statements. The byte sequences are the same instructions modulo:
(1) `operator[]` / `c_ptr` / `operator*` COMDAT-fold misnames; (2) the `LOG_WARNING`
macro's `boost::function<...>` ctor being inlined in a slightly different order
base-vs-target; (3) a `push 38h` (target) vs `push 2Dh` (base) literal - the length
of the generated `__FUNCSIG__`-style signature string baked into the LOG macro. All
whole-program LTCG/ICF, non-steerable.

### get_shapes_centers (93.33%) - arg reg-vs-stack
Sole SIZE on `non_compound_shapes_centers( centers_results )`: target pushes
`centers_results` on the stack, base passes it in a register. This is exactly the
LTCG call-boundary argument-passing choice that MATCHING.md permits stopping on.

### unsubscribe (91.38%) - vectora end() inline-vs-call
Sole SIZE on the `erase( find( begin, end, subscriber ) )` statement: target emits
`m_subscribers.end()` as an out-of-line accessor call (`add ecx,8; call ...`) while
base inlines it as a member read (`mov edx,[ecx+8]`). `begin()` is out-of-line on
both sides; only `end()` diverges. Same vectora/COMDAT inline-vs-call LTCG class as
the documented `vectora::size()` / `operator[]` entries. Non-steerable.

## Commands

```
nix develop -c python3 scripts/rebuild.py
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl --function <fn> --rva <target> \
  --view structure-diff --condensed --objdiff-base-dir binaries/objdiff/base \
  --objdiff-target-dir binaries/objdiff/target
```

rvas: ctor 0x6eed10, dtor 0x6ef180, load 0x6eeee0, get_overlapping_objects 0x6eecb0,
get_shapes_centers 0x6eee80, unsubscribe 0x6eebe0.
