# player_stamina ctors (default + copy) - structure verification

Verifier-matcher v2 pass on chain/game_core-sv-player_stamina.

## Targets
- default ctor: target rva 0x59aea0 / base 0x45be60
- copy ctor:    target rva 0x59ae50 / base 0x45be10

## Diagnosis (structure-diff)
Both are clean structurally:
- default: `target 0 stmts / base 0 stmts ; aligned 0, size-diffs 0, quantity-diffs 0`
- copy:    `target 1 stmts / base 1 stmts ; aligned 1, size-diffs 0, quantity-diffs 0`

The member-initializer list and the single `*this = other;` body statement both
reproduce the target shape exactly.

## Byte diff (--view diff) and cause
- default ctor objdiff fuzzy 96.60% (marker was a stale 94.32%). The only `~`
  rows are reloc-resolved member subobject ctor calls and a `movss xmm0,[0]`
  const load:
    `0x0a: call ...  -> call ...` (same mnemonic, different reloc target)
    `0x84: movss xmm0,[0] -> movss xmm0,[0]`
  These are the construction of `m_subscribers`, an `intrusive_list<...,
  threading::mutex, size_policy, no_debug_policy>` (see player_stamina_subscriber.h):
  the mutex/list subobject ctors resolve to a different relocation under our build.
- copy ctor objdiff fuzzy 96.71% (marker was a stale 90.87%). Single `~` row:
    `0x0a: call 00000223h -> call 00000223h`
  Same: the member subobject ctor (run before the body `*this = other;`).

## Verdict
STRUCTURE MATCH for both. Residual is non-steerable: reloc-resolved member-ctor
calls for the intrusive_list-with-mutex member. Not an argument-passing LTCG and
not a source-shape issue. Markers synced to objdiff (96.60% / 96.71%), carcasses
deleted, condensed structure-diff embedded.
