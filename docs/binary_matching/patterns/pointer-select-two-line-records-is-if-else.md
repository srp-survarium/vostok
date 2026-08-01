# Two statement records around one `jcc` = an `if/else` assigning a pointer, NOT `x = a; if (c) x = b;`

tags: cpp:if cpp:else cpp:local cpp:pointer | asm:lea asm:je | topic:structure-shape topic:condition-shape
symptoms: `lea reg,<A>` then `je short .1` then `lea reg,<B>` then `.1:`; `--view structure` lists TWO statements (the two `lea`s) on non-adjacent lines, none for the `if` itself

MSVC /O2 lowers a two-armed pointer select without any `jmp` over the else arm:
it computes the THEN value unconditionally, then lets the branch skip the ELSE
assignment.

```
0x85:  lea ecx, [esp+10h]      ; <- statement, line 48 (THEN arm)
0x89:  je  short .3
0x8b:  lea ecx, [esp+18h]      ; <- statement, line 50 (ELSE arm)
.3:
```

The condition's own `test`/`cmp` is scheduled far earlier (into the previous
statement's address range) and gets NO statement record, so the structure view
shows only the two arms - on lines separated by a gap of 2 (`if` / arm / `else` /
arm). That gap is the tell that an `else` keyword sits between them.

The naive source

```cpp
float const* jitter = jitter0;
if ( index & 1 )
	jitter = jitter1;
```

produces the same bytes but only ONE statement record (init + `if` merge), so
`--view structure-diff` reports a TRGT_ONLY row plus a `SIZE +N` on the merged
statement. Write the branch out instead - and note the polarity: the arm that
lands BEFORE the `jcc` is the THEN arm, so the condition is the one that keeps
it:

```cpp
float const* jitter;
if ( !( index & 1 ) )
	jitter = jitter0;
else
	jitter = jitter1;
```

Neither `jitter` nor the arms appear in the PDB local list (register-allocated),
which is normal for an optimized TU - `--view structure`'s `locals` block on a
release target lists only the STACK-resident locals, so a missing entry is not
evidence against the local existing in the source.
