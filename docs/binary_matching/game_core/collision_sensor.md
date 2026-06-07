# collision_sensor.cpp - structure verification (v2)

Branch: chain/game_core-sv-collision_sensor. Verified each matched-but-<100% function
with `pdb_fetch --view structure-diff --condensed` (target vs base) and `--view diff`.
All percentages below are read from `binaries/objdiff/report.json` (the live score),
NOT the STATE markers (several were stale).

## Summary

| function | report % | STATE was | now | structure | cause |
|---|---|---|---|---|---|
| collision_sensor() ctor | 82.21 | 82.21 PARTIAL | 82.21 PARTIAL | MATCH (0/0 stmts) | implicit link_resolver base ctor inline-vs-call |
| load | 100.0 | 94.15 DONE | 100 DONE | MATCH (5/5) | already perfect; carcass deleted, bare DONE |
| resolve_links | 100.0 | 98.25 DONE | 100 DONE | MATCH (5/5) | already perfect; carcass deleted, bare DONE |
| tick | 97.60 | 97.60 PARTIAL | 97.60 PARTIAL | MATCH (36/36) | 2x buffer_vector(ALLOCA,count) ctor inline |
| left_objects_predicate::operator() | 97.67 | 97.67 PARTIAL | 97.67 PARTIAL | MATCH (6/6) | /Od `xor ecx,ecx` reg-init before sete |
| notify_and_add_incoming_objects | 92.62 | 92.62 PARTIAL | 92.62 PARTIAL | MATCH (15/15) | buffer_vector(ALLOCA,count) ctor inline |
| notify_objects_inside | 89.29 | 89.29 PARTIAL | 89.29 PARTIAL | MATCH (3/3) | buffer_vector(ALLOCA,count) ctor inline |
| remove | 93.08 | 94.32 DONE | 93.08 PARTIAL | MATCH (15/15) | empty() inline-vs-call (target inlines) |

All eight have matching statement structure (quantity-diffs 0). Every residual is a
SIZE-only divergence with a non-steerable LTCG/ICF cause. No source restructure is
warranted on any. The dtor (100% DONE) carried a stale carcass; deleted.

## ctor (0x58af90, base 0x450fa0) - 82.21%

`--view structure-diff`: target 0 / base 0 stmts (member-init list, no addressed
statements), aligned 0, size-diffs 0, quantity-diffs 0. The init-list shape is correct.

`--view diff`: the divergence is the link_resolver subobject construction.
- TARGET: `add eax,4; call survarium::link_resolver::link_resolver` (one out-of-line
  call to the implicit base ctor that stores the link_resolver vtable @ this+4).
- BASE: `add edx,4; mov [ebp-8],edx; mov [eax],??_7link_resolver@...` - inlines the
  vtable-only ctor at the store site.

`link_resolver` (link_resolver.h) declares only a pure virtual `resolve_links` and NO
user-declared ctor, so the compiler-generated implicit ctor just stores the vtable.
Per assembly_patterns "base-class ctor: out-of-line call vs inlined trivial init" and
its COROLLARY: whether the implicit base ctor is emitted as a folded out-of-line `call`
or inlined is decided by the base class declaration whole-program, NOT the derived body.
`pdb_rich_query` finds NO standalone `link_resolver::link_resolver` symbol in EITHER
index (it is an ICF fold representative). Same class as the `fsm_state::fsm_state`
corollary - except there both binaries called it; here our /GL build chose to inline it.
Not steerable from the derived ctor's source.

Also note: TARGET 0x4d `call vostok::ai::brain_unit_cook_params::brain_unit_cook_params`
vs BASE `call _Impl_vector<void*,...>::_Impl_vector` - both are the `m_old_objects`
vector base ctor, differing only by ICF fold-representative name (not a divergence).

## load (0x58b010), resolve_links (0x58a8a0) - now 100%

structure-diff: 5/5 stmts, size-diffs 0, quantity-diffs 0 for both. report.json reports
100.0 for both - they were brought to 100% by an earlier rebase (the operator[] residual
the old STATE notes mentioned is gone). Deleted carcasses, set bare `// STATE[100%|DONE]`.

## tick (0x58ab80) - 97.60%

structure-diff: 36/36 stmts, two SIZE diffs, both on the
`buffer_vector<...>(ALLOCA(...), count)` ctors (all_sensed_objects @0x1c7, sensed_objects
@0x247). Same cause as notify_objects_inside below.

## left_objects_predicate::operator() (0xb6610) - 97.67%

structure-diff: 6/6 stmts, one SIZE on `bool result = std::find(...) == ...end();`
(target 0x43, base 0x45 - 2 bytes). `--view diff`: BASE emits `xor ecx,ecx` before
`cmp eax,[ebp-14h]; sete cl`; TARGET omits it (clean register). A /Od register-init
artifact, not source-steerable (`sete cl` only writes the low byte; the zeroing is
redundant). The `push_back` symbol name diff is an ICF fold misname. This is sushi's
"target didn't generate xor after std::find" note, confirmed non-steerable.

## notify_and_add_incoming_objects (0x58aa00) - 92.62%, notify_objects_inside (0x58a240) - 89.29%

Each: structure-diff matched stmt count (15/15, 3/3), one SIZE on the
`buffer_vector<...>(ALLOCA(...), count)` ctor. `--view diff` shows BASE emits an EXTRA
`size()` call (the inlined `fill_uninitialized`/ctor body re-evaluating the count) where
TARGET folds the ctor out-of-line. The `size()` symbol itself is an ICF fold rep
(`buffer_vector<resource_ptr<...>>::size` vs `_Impl_vector<void*,...>::size`). Sushi's
notes already documented this; confirmed it is the buffer_vector(ALLOCA,count) ctor
inline-vs-call LTCG wall, not steerable from this function's source.

## remove (0x58a3e0) - 93.08% (was mislabeled 94.32 DONE)

structure-diff: 15/15 stmts, one SIZE on `if ( m_old_objects.empty( ) )` (target 0x23,
base 0x15). `--view diff`: this time the TARGET INLINES `empty()` (the `[begin]==[end]`
compare: `mov [edx+4]; cmp; sete cl`, plus an extra `push esi`), while our BASE keeps it
an out-of-line `call` (delinker misnames it `path_string_impl::operator!`). The
inline-vs-call goes the opposite direction from the other functions but is the same
non-steerable LTCG class. Downgraded the mislabeled `94.32|DONE` to `93.08|PARTIAL`
(report.json = 93.08; the empty() residual is not an arg-passing-only diff).
