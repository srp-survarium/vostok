# Hoisted `.size()` with NO named local = a comma-declared for-init bound (`for ( u32 i = 0, n = v.size(); i < n; ++i )`)
tags: cpp:for cpp:local | asm:cmp asm:sub asm:sar | topic:structure-shape topic:pdb-locals topic:cse
symptoms: target computes v.size() ONCE before the loop and re-uses a register (cmp i,reg), base re-evaluates v.size() inside the loop each iteration; target --view structure has NO i / NO count local; size eval lands on its OWN statement line, separate from the for-condition
confidence: 9/10
variants: for-head-statement-merges.md, array-walk-zero-locals-is-for-each.md

A counted loop whose TARGET hoists the bound out of the condition - `end-begin >>2`
computed ONCE before the loop, then `cmp index, <reg>` each iteration - while a plain
`for ( u32 i = 0; i < v.size(); ++i )` re-evaluates `v.size()` at the loop bottom every
iteration (the size sub/sar appears BOTH at init AND inside the loop). The target's
`--view structure` records NEITHER the counter `i` NOR a count local - only the container.

The faithful source is a SECOND for-init declaration holding the size:
```cpp
for ( u32 i = 0, n = v.size( ); i < n; ++i )   body;
```
`n` enregisters and gets NO PDB named-local entry (unlike a SEPARATE `u32 n = v.size();`
statement, which DOES record `n`). This both hoists the size AND keeps 0 named locals.

Line-split control: the size eval and the loop-condition often sit on DIFFERENT source
lines in the target (size alone on line A, `i=0; i<n; ++i` on line B). Reproduce by
breaking the for-init across two physical lines so the `.size()` lands on its own line:
```cpp
for ( u32 n = v.size( ),
    i = 0; i < n; ++i )   body;        // size on line A, init+cond on line B  -> 3 stmts
```
The declaration ORDER (`n` first vs `i` first) shifts which line the size/init attribute
to - read the target's line column and order to match (target `i=0` on the for-keyword
line wants `i` first; target size on its own earlier line wants `n` first).

Why /Od still hoists here but not for a member-`.size()` loop (`is_affect_applied`,
which matches the plain re-eval form): the comma-init `n` is a real local the source
declared, so the compiler computes it once - it is NOT a compiler CSE.

Evidence (damage_zone.cpp): stop_particles 44.6->73.7% STRUCTURE MATCH (3/3),
load 34.4->54.5% STRUCTURE MATCH (4/4), play_particles 49.4->60.1%; all three drop the
phantom count/i local. Residuals are downstream inline-vs-call (renderer facade,
vector::push_back element-copy) + reg-alloc, not the loop shape.
