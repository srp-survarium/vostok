# Chained-temporary parameters object = ONE statement (and ONE less local)
tags: cpp:ctor cpp:local | asm:call | topic:structure-shape topic:pdb-locals
symptoms: single big row ~0xa3 where base has params + setter chain + ctor rows, PDB lists NO local for the params
confidence: 8/10
variants: byvalue-temp-named-local-hoist.md, same-line-statement-merges.md

One big target row where the base has separate `params p(...)`, `p.set_a().set_b();`,
`T obj(p);` rows (and the target PDB lists no params local): construct the parameters
object as a TEMPORARY with the setter chain inline inside the consumer's declaration,
nested temporaries included.

```cpp
animation_lexeme override_lexeme(
	animation_lexeme_parameters( ... ).animated_object( .. ).playback_type( .. ) );
```
Caution: on reload the faithful shape COST % (33.11) because base inlines setters the target calls out-of-line - faithful structure kept over a higher-% wrong-quantity shape.
Evidence: double_barreled fire (9/9, 73.17 -> 77.13) and reload (8/8) get_user_hands_expression.
