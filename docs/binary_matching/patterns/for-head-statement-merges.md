# For-head merges: increment list, comma-operator counters, and both inits declared IN the for
tags: cpp:for | asm:jmp asm:cmp | topic:structure-shape topic:pdb-locals
symptoms: one loop-head row = sum of while+increment+backjump rows, jmp short .test entry, BASE_ONLY ++counter row
confidence: 9/10
variants: same-line-statement-merges.md, closing-brace-jmp-rows.md, return-row-inlined-dtor.md

The for line owns its condition AND increment block, so row sizes recover the original
loop spelling (size-sum tells):
- `for ( ; cond ; --count )` = ONE head row of 0x13 where `while (count != 0) { ...;
  --count; }` shows three rows (0x8 + 0x9 + 0x2); the /Od for-shape is
  `jmp short .test; .inc: <decrement>; .test: <cond>; je exit`.
- a comma-operator increment list `for ( ...; i; i = i->next, ++state_id )` places the
  counter bump IN the .inc block (before the test at loop top) - a BASE_ONLY `++state_id;`
  row at body-end means the original used the comma form (instruction PLACEMENT moves,
  not just attribution).
- ONE for row containing two slot inits + control = a single-declaration header
  `for ( pair* i = v.begin( ), * e = v.end( ); i != e; ++i )`; separate small rows before
  the for = separate declarations.

Evidence: body_part_parameters::deserialize (9/9, quantity 4 -> 0, 17.6 -> 29.3); weapon_core_shotgun_reload_state::serialize 66.47 -> 77.95 (11 -> 10 stmts); udp_network_flow_emulator::tick (15/15).
