# Repeated free/delete temps: ONE reused `[ebp-4]` slot = a single reassigned local, N slots = N scoped blocks
tags: cpp:local cpp:dtor | asm:mov asm:call | topic:pdb-locals topic:structure-shape
symptoms: same [ebp-4] slot reused across frees, N same-named locals in --view info, pstr temp, VOSTOK_FREE_IMPL frame delta
confidence: 9/10
variants: sibling-braced-scopes.md, free-vs-delete-impl.md

A dtor freeing N pointers shows N near-identical `temp = m_x; VOSTOK_FREE_IMPL(...)`
pairs; the source could be N disjoint `{ pstr temp = ...; FREE; }` blocks OR one local
reassigned. /Od gives every DISTINCT local its own slot even across disjoint sibling
scopes, so every store hitting the SAME slot (plus exactly ONE recorded local in
`--view info`) = one local reassigned, no inner braces (blank lines between the pairs,
no `}` jmps); N recorded same-named locals on distinct slots (-4/-8/-0xC, bigger frame)
= N disjoint braced scopes.

```cpp
pstr temp        = m_string0;
VOSTOK_FREE_IMPL ( m_allocator, temp );

temp             = m_string1;     // reassign -> same [ebp-4] slot
VOSTOK_FREE_IMPL ( m_allocator, temp );
```
```asm
mov edx,[ecx+68h]; mov [ebp-4],edx    ; temp = m_string0
; ...call free_helper...
mov edx,[ecx+6Ch]; mov [ebp-4],edx    ; SAME slot -> same local
```
Different TYPES force distinct locals = genuine scoped blocks (~connect_order: `char* temp` at [ebp-4] + `udp_match_packet* temp` at [ebp-8]).
Evidence: ~string_order single `char* temp`, three [ebp-4] stores = decl-init + two reassigns, 100%; reshaping ~string_response from three scoped blocks to one reused temp 88.87% -> byte-equal statements.
