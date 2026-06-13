# Cast-forward to current_state(): the chained one-liner is the 0-local form (named locals are phantom)
tags: cpp:cast cpp:local cpp:return | asm:mov asm:call | topic:source-fidelity topic:codegen-idiom
symptoms: base `locals (2) state/current` but target `--view structure` has NO locals block, SIZE -0x6 on a 1-statement cast-forward, static_cast<DerivedState*>(current_state())->method()
confidence: 7/10
variants: same-line-statement-merges.md, accessor-call-result-temp.md

An fsm-driver method forwarding to its concrete current state. Write it as ONE chained
expression to match the target's source structure - 1 statement and ZERO named locals
(check `--view structure`: the target shows no `locals` block). The byte-100% temptation is
to split into `T* state = current_state(); U* current = static_cast<U*>(state); return
current->m(...)` collapsed onto one line - that reproduces the target's 2 materialized stack
temps (byte-exact) but FABRICATES two `locals` records the target never had. Local names live
in `.debug$S`, not `.text`, so objdiff still scores it 100% - the divergence is source-fidelity,
invisible to the byte/statement verdict (only `--view structure` shows it). The chained form may
run a few bytes SHORT (here -0x6, ~90%) where the target keeps an extra temp; that residual is
non-steerable and preferred over inventing locals.

```cpp
return static_cast< DerivedState* >( m_logic->current_state( ) )->method( args );
```
Steerable (source-shape). ALWAYS check `--view structure` for the target's local count before
collapsing statements onto a line to win bytes. Evidence: weapon_core_shotgun_reload_state::weapon_and_hands_expression (#369, chained 0-local); cf. jump_logic::is_jump_finished (chained, clean).
