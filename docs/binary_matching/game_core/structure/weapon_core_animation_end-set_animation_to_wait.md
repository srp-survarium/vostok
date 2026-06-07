# Structure verification - weapon_core_animation_end_aware_state::set_animation_to_wait

Function: void survarium::weapon_core_animation_end_aware_state::set_animation_to_wait(
          resources::managed_resource_ptr const&) const
Unit: sources/vostok/game_core/sources/weapon_core_animation_end_aware_state.cpp
Indexes: /home/sheep/Projects/surv/vostok-review/binaries/rich/{target,base}/index.jsonl
This verification did NOT rebuild and changed NO compiled bytes (carcass/comment edit only).

## Resolved RVA

target rva 0x6ed0e0   base rva 0x44f9d0
report.json fuzzy_match_percent: 77.333336 (PR #127)
STATE: PARTIAL (kept - see verdict)

## Verdict

STRUCTURE MATCH (size) - target 3 / base 4 stmts; the single extra base "stmt" is an
EMPTY-only-base collapsed source-line gap (whitespace), NOT a control-structure
divergence. The one real divergence is a SIZE diff on the `if` guard, the documented
get_user() trivial-accessor inline-vs-call LTCG residual. No source/structure bug; the
77% is honest, not a high-%-over-wrong-structure trap.

## Condensed structure-diff (--view structure-diff --condensed)

```
target: 0x6ed0e0            base: 0x44f9d0
; ... set_animation_to_wait(... resource_ptr<...> const&) const ; target 3 stmts / base 4 stmts
.. same ..
0x009 <0x25> | 0x009 <0x2c> | if ( !m_weapon.get_user()->is_replaying_history() )   SIZE
.. same ..
--          | <0>         |    EMPTY only base
; aligned 2, size-diffs 1, quantity-diffs 1
```

## Divergence 1 (SIZE) - the `if` guard, L33

`if ( !m_weapon.get_user()->is_replaying_history() )` - target 0x25 vs base 0x2c
(base +0x7 larger). The statement is identical on both sides; only its byte size differs.

CAUSE (from --view diff): the trivial accessor `weapon_core::get_user()` inline-vs-call.
- TARGET keeps get_user() OUT-OF-LINE:
  `mov eax,[ebp-14h]; mov eax,[eax+128h]; call survarium::weapon_core::get_user;
   mov [ebp-18h],eax` (get_user has a standalone symbol @ target rva 0x9b330).
- BASE inlines get_user() whole-program (`/GL` LTCG):
  `mov edx,[ecx+128h]; mov eax,[edx+44Ch]; mov [ebp-1Ch],eax` - a direct read of
  m_user (@0x44C) off m_weapon (@0x128), plus an extra `[ebp-1Ch]` temp slot
  (frame grows 0x18 -> 0x1C).
get_user() has NO standalone symbol in the BASE rich index (inlined everywhere); it
exists out-of-line in TARGET. This is the documented inline-vs-call LTCG residual, not a
control-structure or member-selection bug.

SOURCE FIX: none available from this function's source - it is a whole-program inline
decision, not steerable from the caller (same class as the other game_core trivial-
accessor inline-vs-call residuals). Stays PARTIAL. No rebuild needed.

## Divergence 2 (QUANTITY) - EMPTY only base

The base carries one extra no-address empty-line run (`<0>`) that the target side does
not. Per the structure-diff format, `EMPTY only base` is a collapsed source-line gap on
one side - a whitespace/source-line-layout artifact (the trailing blank line in the
source body), NOT a `[n]` block-open, NOT a missing `return`, NOT a brace difference.
The control structure is identical: a brace-less single-statement `if` guard over one
assignment, no early-return-vs-if shape, no member-init-vs-body-assign issue. This
quantity-diff is benign and requires no source change.

## Audit result (did the structure-diff catch anything the % hid?)

No NEW structural defect. The diff CONFIRMS the prior whole-unit verdict
(docs/binary_matching/game_core/structure/weapon_core_animation_end_aware_state.md): the
two real statements (`if` guard + brace-less assignment) align 1:1, the only byte
residual is the get_user() inline-vs-call, and the lone quantity-diff is a whitespace
artifact, not control flow. STATE stays PARTIAL (correct). No DONE mislabel to downgrade,
no logic change, no rebuild.
