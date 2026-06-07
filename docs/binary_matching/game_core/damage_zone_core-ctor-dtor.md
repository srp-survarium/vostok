# game_core::damage_zone_core ctor / dtor -- structure verification (v2)

Structure-verifier pass over the two real-bodied non-100% members of
`damage_zone_core.cpp`. No logic changed; both are confirmed STRUCTURE MATCH and
their `DONE` STATE markers are accurate against report.json.

## damage_zone_core::damage_zone_core() -- 99.63%

- Target RVA `0x589380`, base RVA `0x456b20`.
- structure-diff: `target 0 / base 0 stmts ; aligned 0, size-diffs 0,
  quantity-diffs 0`. All construction is attributed to the ctor decl line (base
  subobject ctor + member-init list), so there are no body statements to diverge.
- --view diff (fuzzy 93.90%): instruction shape is identical. The only real
  divergence is a frame-size delta -- target `sub esp, 24h`, base `sub esp, 30h`,
  which shifts every `[ebp-24h]` to `[ebp-30h]` (the `mov ecx,[ebp-..h]; this`
  slot). That is an LTCG frame-slot allocation artifact; the remaining `~` rows
  are member-ctor `call` targets resolving to different relocs. The member-init
  list shape (`hit_initiator( u8(-1), true ), m_physics_world( NULL ), ...`) is
  correct -- moving inits into the body would ADD statements and break the 0/0
  structure.
- VERDICT: STRUCTURE MATCH (member-init list, 0 body stmts). Non-steerable. STATE
  stays `99.39%|DONE`.

## damage_zone_core::~damage_zone_core() -- 100% (was marked 94.65%)

- Target RVA `0x5892d0`, base RVA `0x456a70`.
- structure-diff: `target 0 / base 0 stmts ; size-diffs 0, quantity-diffs 0`.
  All member/base dtor calls are attributed to the dtor close-brace line.
- --view diff reports fuzzy 100.00% -- every instruction (the member zero-stores
  and the chain of member-dtor `call`s) aligns one-to-one. The stale `94.65%`
  STATE marker predated a delink in which the member-dtor call relocs resolved;
  the fresh `report.json` scores this dtor at **100.0%**. Promoted the marker to a
  bare `// STATE[100%|DONE]` and stripped the embed (a clean 100% carries no
  structure-diff).
- VERDICT: STRUCTURE MATCH, byte-perfect. STATE now `100%|DONE`.

## Note
Both functions are empty-bodied: the only correct source shape is exactly what is
written (member-init list for the ctor, empty body for the dtor). There is no
restructure available; the residual is purely at the call/reloc boundary.
