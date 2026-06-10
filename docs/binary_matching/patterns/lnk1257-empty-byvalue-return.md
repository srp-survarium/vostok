# Post-increment returning by value needs a real `return` or LTCG dies with LNK1257
tags: cpp:operator cpp:return | topic:tooling
symptoms: LNK1257 on an empty /* no source */ operator++(s32) body
confidence: 5/10

A by-value-returning `operator++(s32)` with an empty stub body crashes the LTCG linker
(LNK1257). Give it the real temp-copy body - this also unblocks every post-incrementing
caller.

```cpp
sequence_number operator++( s32 ) { sequence_number r( *this ); ++m_number; return r; }
```
