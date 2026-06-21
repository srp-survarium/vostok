# Anchor a non-virtual method by a guarded DIRECT CALL (not an address-take) to win the LTCG this-in-eax/esi/edi convention
tags: cpp:member | asm:mov asm:jmp asm:ret | topic:anchoring topic:convention topic:inline-vs-call
symptoms: target reads `this` from eax/esi/edi (not ecx), no push/pop of the this-reg, `ret 4` on an arg-taking method, base reaches the fn only via an `anchor_* &C::method` member-fn-ptr sink and scores 65-95% with a pure register-cascade diff
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

This supersedes the eax-this-convention.md "VERIFIED INEFFECTIVE" verdict for the
reachable-only-via-anchor case: the methods it tried (header split, __declspec(noinline),
the member-fn-ptr sink) all FORCE __thiscall; the guarded direct call is the device that
reproduces the eax/esi/edi convention instead.
evidence-basis: positive (vostok game, game.cpp/anchor_game_world.cpp, 9 methods 65-97%/unpaired -> 100%)
