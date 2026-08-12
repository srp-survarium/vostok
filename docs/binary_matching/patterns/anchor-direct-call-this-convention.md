# Anchor a non-virtual method by a guarded DIRECT CALL (not an address-take) to win the LTCG this-in-eax/esi/edi convention
tags: cpp:member | asm:mov asm:jmp asm:ret | topic:anchoring topic:convention topic:inline-vs-call
symptoms: target reads `this` from eax/esi/edi (not ecx), no push/pop of the this-reg, `ret 4` on an arg-taking method, base reaches the fn through an `anchor_* &C::method` member-fn-ptr sink and scores 65-95% with a pure register-cascade diff
confidence: 9/10
variants: anchor-sole-caller-convention.md, eax-this-convention.md

A non-virtual member that is only ever called INTERNALLY in the shipped EXE gets a
whole-program LTCG-specialised calling convention: `this` arrives in eax / esi / edi
(whatever was free at the real call sites), the this-register is NOT saved (no
`push esi`/`pop esi`), and an arg-taking method may clean its own stack (`ret 4`). The
function's STRUCTURE is identical to ours - the per-fn diff is a pure register cascade
(`mov ecx,[ecx+3B8h]` vs `mov ecx,[eax+3B8h]` ...), scoring 65-95%.

If our base reaches that method ONLY through the `/OPT:REF` anchor, HOW the anchor
references it decides the convention of the standalone copy:

- **Address-take through a sink** (`void (C::*p)() = &C::m; s_sink=*(pcvoid const*)&p;`)
  forces a stock `__thiscall` copy: `this` in ECX, push/pop of the saved reg, `ret`. That
  DIVERGES from a target that kept the eax/esi/edi convention (the eax-this-convention.md
  CAVEAT). This is the WRONG anchor device for these functions.

- **A guarded DIRECT CALL** through a volatile `this`-pointer lets LTCG specialise the
  body's convention exactly as the target did - the anchor call participates in the
  LTCG call graph as a normal call, so the standalone copy gets the eax/esi/edi
  convention and pairs byte-exact:

```cpp
// C is abstract (can't construct it) -> source `this` from a never-deref'd volatile.
static C* volatile s_g     = 0;
static volatile bool s_run = false;
// source any RUNTIME arg through a volatile too, else the literal is constant-
// propagated into the body (the param read becomes `mov eax,4`; ret 4 collapses).
static E volatile s_arg    = some_value;
if ( s_run )            // never executes; pins the symbols + their convention
{
    s_g->respawn_local_player( );          // void   -> this in eax
    s_sink = &s_g->get_flash_factory( );   // accessor, capture the result
    s_g->switch_to_login( s_arg );         // arg via volatile -> param preserved, ret 4
}
```

Verified positive (game.cpp, anchor_game_world.cpp): nine survarium::game methods went
from a register-cascade 65-97% / unpaired to 100% - respawn_local_player, discard_current_match,
get_flash_factory, toggle_console, switch_to_lobby/login/main_menu/game_world, create_debug_window.
switch_to_login needed the volatile-arg fix too (a literal `login_menu_status_connected`
folded to `mov eax,4` and broke `ret 4`; the volatile restored both -> 100%).

WHEN this works vs the sink: use the direct call for a non-virtual, internally-only-called
method whose TARGET uses a non-ecx this-reg. Use the address-take sink (anchor-sole-caller-convention.md)
when the target keeps stock `__thiscall` (this in ecx) - e.g. a method whose address IS
taken somewhere in the real program (commit_suicide stayed 100% via the sink). VIRTUALS
always use `__thiscall`; leave them on the sink. Tell them apart by reading the target's
first instruction: `mov ecx,[ecx+off]` = sink; `mov ecx,[eax+off]` / `[esi+off]` = direct call.

Retire the sink when a real call site lands. An address-take remains a whole-program
constraint even when `xref --base` also shows the real caller: it can keep the standalone
copy on stock `__thiscall` and prevent LTCG from promoting the arguments to match retail.
For `breath_holding_params::load`, target used `cfg` in ESI and `this` on the stack, while
the stale sink forced base to save ESI and use ECX/stack in the normal convention. Deleting
only the sink kept the function alive through `player_cook::on_config_loaded`, changed
97.26% to 100%, improved the real caller too, and produced zero regressions. Therefore,
before accepting a convention wall, distinguish "anchor is the only caller" from "anchor
is now redundant" and remove the latter.

This supersedes the eax-this-convention.md "VERIFIED INEFFECTIVE" verdict for the
reachable-only-via-anchor case: the methods it tried (header split, __declspec(noinline),
the member-fn-ptr sink) all FORCE __thiscall; the guarded direct call is the device that
reproduces the eax/esi/edi convention instead.

INTERLOCK with access mangling: a method that mangles PRIVATE (AAE/EAE) but is declared
public in our header stays UNPAIRED (QAE vs AAE/EAE = different mangled name, objdiff can't
pair it) - the convention fix alone won't close it. Fix BOTH: (1) set the header access to
the target's mangled letter (often a `private:` block interleaved to preserve declaration
order); (2) the /OPT:REF anchor that address-takes / direct-calls those now-private methods
needs `friend void ::vostok::use_<module>();` on the class (codegen-neutral, no symbol/layout
impact - mirror the existing project_cooker_simple precedent). Verified: weapon::play_weapon_
fire_pfx (esi this-conv) was unpaired until BOTH the direct-call anchor AND making it private
(+friend) landed -> 91.6% STRUCTURE MATCH (residual = a separate non-steerable call-boundary
arg-eval register cascade). play_weapon_shell_pfx (esi, already QAE-public) went 79.6->91.6
on the convention fix alone.
evidence-basis: positive (vostok game, game.cpp/anchor_game_world.cpp 9 methods 65-97%->100%; weapon.cpp play_weapon_*_pfx 79.6/unpaired->91.6 STRUCTURE MATCH; game_core breath_holding_params::load 97.26%->100% by retiring a redundant sink)
