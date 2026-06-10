# rel32 brace-exit jmp gets its own line record; rel8 folds into the previous statement
tags: cpp:if | asm:jmp | topic:structure-shape topic:scoring-artifact
symptoms: one TRGT_ONLY 0x5 row at an if-branch closing }, preceding base statement SIZE +0x5, 21-vs-20 stmts byte-identical
confidence: 8/10
variants: closing-brace-jmp-rows.md

The branch-exit jmp over the rest of an if/else-if chain is a 5-byte rel32 when the join
is >127 bytes away, and then gets a line record on the `}` line; a 2-byte rel8 exit folds
into the previous statement's record - on BOTH builds. When the two builds land on
different sides of the rel8/rel32 edge for the same jmp, a zero-byte quantity artifact
appears. Not source-steerable (same braced source both sides).

Evidence: game_material_manager_cook::on_decals_loaded (decal1 rel32 split in target only; decal2/sound rel8 folded both sides).
