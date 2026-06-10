# Guarded eater WITH arg pushes = ASSERT_U / ASSERT_CMP_U / ASSERT_T_U
tags: cpp:assert cpp:macro | asm:push asm:call asm:test asm:jcc | topic:assert-eater
symptoms: expression_eater, identity(false), push 0 before eater call, movzx test je, add esp, rep movsd
confidence: 9/10
variants: assert-eater-recover.md, unreferenced-params-eater.md

The `_U` asserts compile to `if(::vostok::identity(false)){ expression_eater(args); }`
(debug_macros.h): the guard eater plus a never-taken block pushing the discarded args
C-style (right-to-left) into a folded-empty varargs call. Recover the macro from the
pushes; the `push 0` (`assert_untyped`) is the tell between forms.

```cpp
ASSERT_CMP_U( v1, ==, 4 );  // expression_eater(v1, 4, assert_untyped) -> push 0; push 4; push v1
ASSERT_U( expr );           // pushes expr then push 0
ASSERT_T_U( a, b );         // expression_eater(a, b) -> push b; push a - NO push 0
```
```asm
mov byte [ebp-N],0; lea eax,[ebp-N]; call <folded-empty>   ; if ( identity(false) )
movzx eax, byte [eax]; test eax,eax; je .skip
push <v2>; push <v1>; call <folded-empty>; add esp, M      ; expression_eater( ... )
; a class-typed arg is copied by value: sub esp,0x84; rep movsd (e.g. animation_lexeme)
```
If the pushed args are the function's OWN otherwise-unused parameters, it is
VOSTOK_UNREFERENCED_PARAMETERS, not an assert (identical shape; see variants).
Evidence: weapon_user_animations_selector::on_broken_limb_affect L337 ASSERT_T_U byte-perfect; weapon_core_idle_state ctor ASSERT_CMP_U(count,==,4) 100%; weapon_and_hands_expression L32 ASSERT_U; ASSERT_U->UNREFERENCED swap (push 0 gone) took idle/aimed weapon_and_hands_expression 85.65->87.52 each.
