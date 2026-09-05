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

UNGUARDED direct call variant: a target whose ENTIRE body is just `push arg2; push arg1; call
<folded-empty>; add esp,8` (NO `mov byte;lea;call;movzx;test;je` guard, NO `push 0`) is a
DIRECT `vostok::detail::unreferenced_parameter_helper( a, b )` call - NOT the macro. The macro's
`if(identity(false)){...}` guard DCE-collapses the whole never-taken block to a bare `ret N`
(the call never reaches codegen), so the macro produces the WRONG bytes here; only the unguarded
call survives DCE and emits the eater. A class-reference arg (incomplete type at the call site)
is passed by ADDRESS - `helper( map_name, &director )` - so it pushes the reference pointer
(matching `mov eax,[esp+8]; push eax`), not a by-value `rep movsd` copy.
Evidence: weapon_core_shotgun_reload_state_cook::allocate_resource 41.38->87.45 (the 0x3d row
byte-for-byte; ASSERT(UNKNOWN_EXPRESSION) had produced only the 0xc half); network_client::load
(unguarded direct helper call, &director pointer-push, structure-exact - VOSTOK_UNREFERENCED_PARAMETERS collapsed to bare `ret 8`).

Check the bytes after the helper call and stack cleanup before assuming a
normal return. `inventory_item::{serialize_game_world_object_header,
deserialize_game_world_object}` and `booby_trap_set_core::{activate,
selected_animations}` end immediately in `cc` padding in the raw target PE;
the missing epilogue is not just a rich-index extent cutoff. Their source
model is direct argument use followed by the shipping unreachable macro
(`__assume(0)`), not an ordinary returning no-op. The exact original macro
spelling still requires evidence. A parameterless one-byte trap alias is
weaker evidence: inspect vtable uses and section contributions before drawing
the same original-source conclusion for every method sharing that address.
