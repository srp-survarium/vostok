# If-condition materializing a bool (cmp/sete/movzx/test/je) = an INLINED bool helper, not `if(!x)`
tags: cpp:if cpp:bool cpp:inline | asm:sete asm:movzx asm:test | topic:structure-shape
symptoms: cmp [this+OFF] 0 sete dl movzx test je vs direct cmp jne
confidence: 7/10
variants: positive-getter-inline-sete.md

`if ( !m_member )` is a direct `cmp [this+OFF],0; jne`. A target emitting
cmp/sete/movzx/test/je is the shape of an inlined bool-RETURNING member in the condition
(bool produce + return-widen + the if's test). Find the declared-but-stubbed bool member
the PDB type info lists and give it the body the bytes spell.

```cpp
bool medkit::empty( ) const { return !m_activity_time_ms; }   // was a 'no source' stub
if ( empty( ) )
```
Evidence: medkit::active_tick tail, 53.50 -> 57.12.
