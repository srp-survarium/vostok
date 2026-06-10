# Anchoring an abstract state subclass: override EVERY still-pure virtual
tags: cpp:virtual | topic:anchoring
symptoms: C2259 cannot instantiate abstract class, fsm_state pures
confidence: 8/10
variants: addressof-virtual-anchoring-trap.md

A `concrete_state : survarium::<state>` anchor stub must override ALL still-pure virtuals
or it stays abstract (C2259). `ai::fsm_state`'s initialize/execute/finalize/
is_ready_for_transition are pure; a mid-hierarchy class often re-declares only SOME
(player_logic_base_state overrides only is_ready_for_transition and adds its own pure
selected_animations). Check the .h chain for which pures the class did NOT re-declare.

```cpp
struct concrete_state : survarium::player_logic_base_state {
	virtual void initialize( ) override { }   // + execute/finalize + the class's own pures
};
```
Ctor/setter reference args can be fabricated from `*reinterpret_cast<T*>(NULL)` (the anchor never runs) to avoid constructing a noncopyable owner.
Evidence: use_game_core_player_logic_base_state.
