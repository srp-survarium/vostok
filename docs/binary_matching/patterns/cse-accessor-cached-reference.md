# Repeated accessor reloaded in base, CSE'd to ONE held pointer in target — cache the reference

## Tags
cpp:reference cpp:accessor cpp:local | asm:mov asm:reg-alloc | topic:codegen-idiom topic:cse topic:pdb-locals

## Symptom

A function calls the SAME accessor several times — e.g. `m_game.lobby_menu()`
(an inline `return *m_lobby_menu;`) — to read/write members of the returned
object. The TARGET loads the object pointer ONCE into a callee-saved register and
reuses it across the whole body (even ACROSS an intervening member call):

    mov   eax, [ecx+18h]     ; m_game  (a reference member)
    mov   esi, [eax+374h]    ; m_lobby_menu  -> held in esi
    call  lobby_menu::query_lobby_info       ; this = esi
    cmp   byte ptr [esi+0F6h], 0             ; reuses esi after the call
    ...
    mov   byte ptr [esi+0F6h], 1
    push  esi                                ; this for the next call

The BASE re-derives the accessor at every use (`mov ecx,[..+18h]; mov edx,[..+374h]`
repeated), inflating the function and scoring low. The target records **0 named
locals** for the function.

## Cause

The original source captured the result of the accessor into a **reference local**
once and used that name throughout:

    lobby_menu& menu = m_game.lobby_menu( );
    menu.query_lobby_info( );
    if ( !menu.m_is_connected_to_lobby ) { menu.m_is_connected_to_lobby = true; ... }

Under LTCG/optimization the reference is a fixed pointer captured at one point, so
the compiler holds it in a callee-saved register and reuses it across the member
call (the call can't change the *reference*, only the object's fields). Writing the
accessor inline at each site instead lets the base optimizer (less aggressive here)
re-load it each time.

## Fix

Cache the accessor into a `T& name = ...;` reference local and use it everywhere.
This is the FAITHFUL form, not a hack:

- The reference **enregisters with NO PDB local** (`--view info` still shows
  `locals (0)`), so it matches a 0-named-local target — it is NOT a phantom local.
- It reproduces the held-pointer CSE the target emitted, collapsing the redundant
  reloads.

Precedent: `network_client::on_connected_to_lobby` / `on_disconnected_from_lobby`
(network_client_lobby.cpp). A stale note had rejected the cached-reference form as
"adds a phantom local AND scores lower"; after surrounding TUs landed, re-testing
showed the cached `lobby_menu& menu` keeps `locals (0)` and lifts
`on_disconnected_from_lobby` 19% -> 70% (residual then is purely a cross-TU inline
of a scaleform-stubbed callee). Lesson: re-test "scores lower" local-vs-inline
verdicts after the neighbourhood changes — and verify with `--view info`, not the
guess that any reference local must be phantom.
