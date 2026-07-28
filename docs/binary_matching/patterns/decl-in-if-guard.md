# `if ( T x = expr )` decl-in-if: ONE record covering decl + test, opening its own [n] block
tags: cpp:if cpp:local | asm:cmp asm:test asm:jcc | topic:pdb-locals topic:structure-shape
symptoms: record covering init AND cmp/je, local listed in a [n] block opening at that statement, [1][2][3] consecutive depths
confidence: 8/10
variants: nested-if-vs-multiline-and.md, sibling-braced-scopes.md

A record whose range covers a local's initialization AND the test consuming it, with the
PDB listing the local in a block `[n]` that OPENS at that statement, is a
condition-declaration guard; consecutive deepening guards are a brace-less chain.
A separate decl + separate if would emit TWO records.

```cpp
if ( T const x = e )
	if ( u8 const y = f )
		if ( !x->p( ) )
		{ ... }
```
Also seen: a function-tail braced `{ T const x = e; }` whose decl is a DEAD STORE (consumer compiled out of MASTER_GOLD) - reproduce the block, the bytes are real.
Evidence: base_player::tick_active_object 3.38 -> 41.70 (19/19).
