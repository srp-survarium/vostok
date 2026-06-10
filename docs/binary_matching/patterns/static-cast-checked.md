# static_cast_checked is the project-wide cast idiom: extra temps / a `mov eax,[eax]; ret` call
tags: cpp:cast cpp:template | asm:mov asm:call | topic:codegen-idiom topic:fold-icf
symptoms: cast statement 6+ bytes short, t1->t2->t3 temp chain, call bogus operator* that is mov eax [eax] ret, +0xc spill before strings::copy
confidence: 9/10
variants: config-value-operator-cast.md, static-cast-resource-ptr-copy.md

The original spells casts `static_cast_checked< T >( expr )` (debug_static_cast_checked.h);
in MASTER_GOLD its body is an inline `T f( S const& s ){ return static_cast<T>(s); }` - at
/Od+LTCG the cref-bind materializes a temp for an rvalue arg and the return materializes
another. When LTCG keeps the instantiation OUT-OF-LINE the call is the instantiation
itself, ICF-folded onto any `mov eax,[eax]; ret` symbol (bogus intrusive_ptr<X>::operator*
/ vec_begin names). Check static_cast_checked FIRST when a cast statement is 6+ bytes short.

```cpp
static_cast_checked< pcstr >( value[ i ]["key"] );   // also the config-string arg variant
```
```asm
; (a) extra stack-temp copies the plain static_cast collapses: [this+10h]->t1->t2->t3->read->t4
; (b) lea eax,&temp; call <folded instantiation>      ; callee = mov eax,[eax]; ret
; config-string variant: target spills eax to a deep temp and reloads (+0xc) before the copy call;
;   downstream rel8->rel32 rows (+-0x3..0x6) close themselves
```
Evidence: weapon_user_animations_selector::get_current_state_id 71.5->100, current_state 70.26->80.2; serialize/deserialize +0x6 temps -> byte-match; medkit::load all 6 rows closed (0x55c both sides); sushi@TODO in game_material_manager_cook.cpp suspected it.
