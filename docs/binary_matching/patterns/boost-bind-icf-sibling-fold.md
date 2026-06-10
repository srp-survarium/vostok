# `boost::bind(&Derived::virtual_method, ...)` ICF-folds onto a SIBLING class's bind<> rep
tags: cpp:template cpp:virtual | asm:call | topic:fold-icf
symptoms: bind<> call named with a DIFFERENT class than the bound method, vcall'{36}' thunk, assign_to<bind_t<...>>
confidence: 8/10
variants: icf-fold-names-noise.md, boost-bind-cref.md

The `boost::bind<>` helper packs only {member-fn-ptr, this, arg} and is byte-identical
across sibling state classes, so /OPT:ICF folds them - the delinker prints whichever fold
representative it picked. The TRUE class shows on the un-folded
`assign_to<bind_t<...>>` and the `Derived::vcall'{N}'` member-pointer (a vcall thunk
because the method is virtual).

```cpp
set_animation_callback( "ch", this, boost::bind( &weapon_core_fire_state_base::on_shot_event, this, _1 ) );
```
Evidence: weapon_core_fire_state_base::initialize (99.71% - the bind call delinker-named ...weapon_core_animation_end_aware_state...).
