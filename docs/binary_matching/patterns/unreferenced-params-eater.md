# Unused-parameter eater: VOSTOK_UNREFERENCED_PARAMETER(S)
tags: cpp:macro | asm:lea asm:push asm:call | topic:assert-eater
symptoms: unreferenced_parameter_helper, (void)(&p), lone lea of a param slot, identity(false), rep movsd
confidence: 9/10
variants: assert-u-eater.md, assert-eater-recover.md

A parameter taken but never read almost always had one of these macros
(macro_unreferenced_parameter.h; 244 uses across game_core/animation - under-using them
is a red flag). Singular = `(void)(&p)` = zero bytes, zero rows. Plural = the same
guarded-eater shape as ASSERT_U but pushing the function's own parameters - a REAL
statement row (~0x25+ bytes) at /Od.

```cpp
VOSTOK_UNREFERENCED_PARAMETER( p );      // (void)(&p) - row-free; default choice
VOSTOK_UNREFERENCED_PARAMETERS( a, b );  // if(identity(false)){ unreferenced_parameter_helper(a,b); }
```
```asm
; singular: at most a lone discarded  lea reg,[ebp+p]  - no mov-byte, no call
; plural:   mov byte[ebp-N],0; lea; call <fold>; movzx;test;je
;           push b; push a; call <fold>; add esp    ; NO push 0 (assert_untyped) on top
; a reference param is evaluated BY VALUE: rep movsd struct copy (0x258 for query_result_for_cook)
```
Prefer singular to stay row-free (plural added a 16th row + 0x25 bytes in udp_network_flow_emulator::tick); plural is right when the target row carries the full eater.

UNGUARDED variant (stubs that must never be called): a target whose body is `mov byte;lea;call
<eater>` followed DIRECTLY by `push args; call <folded-empty>; add esp,N` - no movzx/test/je - is
the macro followed by `VOSTOK_UNREACHABLE_CODE( )`. The `__assume(0)` lets MSVC drop the
`identity(false)` test even at /Od (Wine probe, 2026-09-12), so the never-taken block is emitted
unconditionally. A reference to a class the retail header only forward-declares is handed over as
a pointer (`&user, &engine`, `&weight_driving_animation`) - by value the eater would copy the
object (0x11C `rep movsd` for base_player) and would not compile for an incomplete type; a small
complete type (`resources::managed_resource_ptr`, `mutable_buffer`) goes by value as usual.
```cpp
virtual void activate( base_player& user, engine& engine ) override { VOSTOK_UNREFERENCED_PARAMETERS( &user, &engine ); VOSTOK_UNREACHABLE_CODE( ); }
```
Evidence: weapon_ammunition / medkit / oxygen_tank / artefact_lifebone_core activate (43) and
selected_animations (50), weapon_core_inactive_state on_animation_end / on_specific_event (45) and
weapon_and_hands_expression (58) - all byte-exact in build 26; network_client::load (the same
shape in an /Ox TU, `&director`).
