# A carcass-anchor's SOLE call lets LTCG specialise the matched body's convention/args
tags: cpp:member cpp:param | asm:ret asm:cmp asm:mov | topic:anchoring topic:convention
symptoms: out-of-line body has `ret` not `ret 4`, a runtime param read as a literal (`cmp [m],0` where source says `== id`), param in a reg not `[esp+4]`, this in ecx vs eax
confidence: 7/10
variants: eax-this-convention.md, ltcg-dse-empty-ctor.md, dced-static-helper-anchor.md

When a network/game-client (or any) carcass function is reachable ONLY through the
`/OPT:REF` reachability anchor (`anchor_*` calls every public method once so the symbol
is kept), the anchor is the body's SOLE caller. Whole-program LTCG then specialises the
out-of-line body to that one call site, which DIVERGES from the target (whose real
callers force the standard ABI). Two distinct symptoms, both steerable from the anchor:

1. **Constant-propagated argument.** Anchor calls `base.is_player_current( 0 )` with a
   literal -> LTCG folds `id == 0` into the body, so `m_current_player->id == id` compiles
   to `cmp byte [eax+34h], 0` and the stack param vanishes (`ret`, not `ret 4`). FIX: pass
   a value the compiler can't fold - read a `volatile` flag the anchor already has
   (`const u8 runtime_id = (u8)s_run;`) and pass `runtime_id`. Restores the param.

2. **Custom register calling convention.** With one caller LTCG may pass the param in a
   register and drop the stack slot (param in `dl`/`edx`, `ret` not `ret 4`). FIX: take the
   member-function ADDRESS through a `volatile` sink in the anchor
   (`bool (C::* const p)(const u8) const = &C::is_player_current; s_sink = *(pcvoid const*)&p;`)
   - the pointer must be callable via the normal ABI, pinning standard `__thiscall` (stack
   param + `ret 4`). On `is_player_current` this lifted 82.6% -> 98.7%, and fixing the
   compare ORDER to the source's (`m_current_player->id == id`, member loaded first) -> 99.6%.

CAVEAT (see eax-this-convention.md): the sink forces `__thiscall` with `this` in ECX. If
the TARGET keeps `this` in EAX (its real caller's convention), the sink will MATCH the param
side but DIVERGE the this-register (`mov eax,[ecx+8]` vs target `[eax+8]`). Net is usually
still a large win when the param/`ret 4` gap dominates; the residual this-in-eax is
irreducible without the real caller. Weigh per function: param-convention gap -> sink helps;
pure this-in-eax frameless leaf -> sink hurts (do NOT escape the address there).
evidence-basis: positive (vostok game core-client, base_network_client::is_player_current 82.6->99.6%); anchor_game_clients.cpp runtime_id + member-fn-ptr sink
