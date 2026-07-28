# A `return *(T*)NULL;` "buildability return" accessor stub collapses every inlined caller chain

## Tags
cpp:inline cpp:accessor | asm:mov-absolute | topic:ltcg topic:stub topic:reachability

## Confidence
9/10 — observed walling object_light/decal/wire insert+load+remove on a single
unmatched accessor stub; fixing the stub took insert/remove 62% -> 100%.

## Symptom
A function that should chain through a getter (`a.renderer().scene().add_x(...)`)
diffs with the *base* doing a bizarre absolute load such as `mov ecx, [10h]` (read
from a near-zero absolute address) or skipping an indirection level entirely, while
the *target* chains real member offsets (`mov eax,[esi+0A8h]; mov ecx,[eax+94h];
mov edx,[ecx+10h]`). The chain looks "collapsed" to a constant on the base side.

## Cause
One of the inlined accessors in the chain is still a placeholder STUB whose body is
`return *( T* )NULL; // buildability return`. Under the MASTER_GOLD LTCG build the
accessor is inlined into every caller, and the compiler **const-propagates the NULL
return** down the chain: `getter()->scene()` becomes `((scene*)NULL)->...` =
`*(T*)(0 + offset)`, i.e. a read from a tiny absolute address. The caller's own
source is correct — the residual is entirely the upstream stub.

## Fix
Replace the stub with its real one-line body (read the target asm for the offsets:
`mov ecx,[this+0xA8]; mov eax,[ecx+0x94]; ret` == `return m_game.renderer();`).
This is making a stub *correct*, not changing another unit's structure, and it is
required for the dependent functions to inline the way the target did. The accessor
itself usually jumps to 100% as a bonus, and every downstream chained caller picks
up its inlined form. Confirm the offsets against the target body
(`pdb_fetch --rva <accessor> --view target`) before writing.

## Example
`survarium::base_game_scene::renderer()` shipped as `return *(render::game::renderer*)NULL;`.
`object_light::insert`/`remove` chained `get_game_scene().renderer().scene().add_light(...)`
and diffed with `mov ecx,[10h]`. Restoring `return m_game.renderer();` (m_game at
0xA8, game::renderer() returns m_renderer at 0x94) fixed the accessor to 100% and
took insert/remove to a clean STRUCTURE MATCH at 100%.

## See also
- `anchor-sole-caller-convention.md` — the dual case where a *constant arg* (not a
  stub return) is the thing LTCG propagates; source the anchor's placeholders through
  `static volatile` pointers to stop it.
