# `case X : break;` folds OUT of the cmp-chain but keeps its dead 2-byte jmp line record
tags: cpp:switch | asm:cmp asm:jmp | topic:structure-shape
symptoms: dispatch cmp-chain skips explicit case values, bare +0x002 jmp rows between dispatch and case bodies
confidence: 8/10
variants: switch-2value-cluster.md, switch-default-nodefault.md, jmp-to-next-kept-by-target.md

A /Od switch whose dispatch cmp-chain SKIPS some explicit case values (e.g. only
`cmp 1; cmp 3` for a 0..3 source switch) is NOT missing cases: a `case X : break;` whose
body equals the no-match path is folded out of the dispatch (no cmp emitted), but the
`break;` STATEMENT still emits its dead 2-byte `jmp <switch-end>` with its own line
record. A carcass showing dispatch, then a bare `+0x002` jmp, then real case bodies maps
to `case a : break; case b : <real>; ...` in source order.

```cpp
switch ( m_client_state ) {
case resolving :     break;        // folded out of dispatch, keeps the dead jmp
case signing_in :    break;
case unresolved :    return;       // real body -> real cmp
}
```
Corollary: a 5-byte near jmp can be the same folded break when the switch end is >127 bytes away.
Evidence: 19/19 statements on login_client_impl::~login_client_impl (do/switch over m_connection_state with inner switches over m_client_state).
