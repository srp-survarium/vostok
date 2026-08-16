# game TU enablement (batch 14) — buildability recipe

Goal of this phase: make every enabled game `.cpp` **compile and link** as
buildability stubs — NOT byte-match. Byte-matching is the subsequent matcher
loop. Here we only clear compile/link errors with the minimal, canonical-shaped
change, preserving the addressed `// FUNCTION BODY[0xVA]` carcasses verbatim.

All game TUs are now un-`ExcludedFromBuild` for `Master Gold|Win32`. Build with
`nix develop --command python3 -m vostok.build.ninja game` (keep-going `-k 0` —
shows every TU's errors). Your TUs must produce their `.obj` with no `error C`.
Ignore errors in TUs outside your assigned set (other waves own them).

## The error catalogue (and the canonical fix)

- **C2758 / C2512** — reference/const member, or a base/member with no default
  ctor, not initialized. Add the init to the stub ctor's member-initializer
  list with a buildability placeholder (`0`/`NULL`/a default-constructed temp),
  and comment it `// buildability: matcher supplies real value`. Per the
  stub-body policy in [../library_carcass_rebuild.md](../library_carcass_rebuild.md).
- **C2027 / C2227 / C2440 (via `intrusive_ptr_inline.h`)** — a member/return is
  `intrusive_ptr<T>`/`resource_ptr<T>` where `T` is only forward-declared, and
  the stub body or dtor instantiates `->destroy()`. `#include` T's real header
  in the `.cpp` (not the header, to keep header cost down) so the type is
  complete at the instantiation point.
- **C2511 / C2061 / C2040 / C2039** — an out-of-line definition's signature
  doesn't match any declaration (param type drift, a name resolving to the
  wrong type, a missing forward decl). Reconcile to the **header's** declaration
  (the header was validated against the PDB); usually the `.cpp` carcass param
  type is stale — fix the `.cpp`.
- **C3668 (`override` doesn't override)** — the declared signature doesn't match
  the base virtual, so it isn't an override. Two causes seen: (a) a member NAME
  shadows a TYPE name in class scope (e.g. `game_world::bullet_tracer::bullet`
  member shadows type `bullet`) — fully-qualify the type in the signature
  (`::survarium::bullet`); (b) genuine interface drift where our engine header's
  base virtual differs from the shipped one (input::handler `on_before_processing`,
  engine_user::world `load`/`is_loading`/`on_fullscreen_alttab`) — drop the
  `override` keyword (keep `virtual`), leave a lean comment, do NOT edit the
  engine base header.
- **C2259 (cannot instantiate abstract class)** — a derived stub doesn't
  override all of a base's pure virtuals (often a downstream effect of a C3668
  on the same class). Fix the C3668s first; if a pure virtual is genuinely
  unoverridden, add the missing override as a stub.
- **C2248 (private member access) / C2923 (bad template arg)** — a stub uses a
  type/ctor it shouldn't (e.g. `vectora<T>` of an incomplete/private-ctor T).
  Reconcile to the canonical member type; a nested `player_skill` etc. may need
  qualifying or a different container.
- **C1075 (brace mismatch)** — a real stub bug; balance the braces.
- **C4100 / other warnings** — leave them (the engine builds with warnings).

## Rules

- Minimal, canonical-shaped changes only. Keep `// FUNCTION BODY[0xVA]`
  carcasses and offsets/STATIC_SIZE_ASSERTs. Lean comments.
- Shared headers (`game_world.h`, `human_npc.h`, `player.h`, `game.h`): wave A
  owns these. Later waves: if you must touch one, you inherit wave A's fixes —
  rebuild first; only add what your TU needs.
- Never weaken a STATIC_SIZE_ASSERT or change a member offset to dodge an error
  — that breaks the match. Fix the code shape instead.
- Verify your assigned TUs each produce a `.obj` with no `error C`
  (`ls "binaries/Win32/intermediates/Master Gold/game/<tu>.obj"`). Commit when
  your set is clean. Do NOT re-exclude a TU to dodge a hard error without
  flagging it in the report + a `sushi@TODO`.
