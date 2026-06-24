# Array-walk loop with 0 target locals = std::for_each (the iterator lives in the inlined helper)
tags: cpp:loop cpp:local cpp:template | asm:cmp asm:jmp asm:add | topic:inline-helper topic:source-fidelity
symptoms: target --view structure has 0 named locals, asm is init-ptr / cmp-end / body / add ptr,sizeof / jmp loop, a raw for-loop gives the bytes but +1 named local
confidence: 9/10
variants: current-state-cast-forward-materialized-locals.md

A function whose asm is a pointer walk over a contiguous array - `it = &arr[0]; loop: if it
== end goto done; body(it); it += sizeof; goto loop` - whose `--view structure` records ZERO
named locals. A raw `for ( T* it = arr; it != arr + N; ++it ) body( *it );` reproduces the
bytes but adds ONE named local (`it`) the target lacks. Names are NOT elided: the iterator
belongs to an INLINED `std::for_each`, not this scope, so the faithful source is the
algorithm call - byte-identical AND 0 named locals. Grep the SIBLING functions in the same
`.cpp`: they usually already iterate the same container that way.

```cpp
std::for_each( m_slots, m_slots + max_slots_count, call_item_remove );   // NOT a raw for-loop
```
```asm
mov eax,[this]; add eax,<off>; mov [ebp-4],eax    ; it = &arr[0]
.body: ... push [ebp-4]; call <fn> ...
.inc:  mov ecx,[ebp-4]; add ecx,<sizeof>; mov [ebp-4],ecx
.cond: cmp [ebp-4], <end>; jne .body
```
Steerable and CLEAN - 0 locals AND byte-identical, not a take-the-hit. Evidence:
`inventory::remove` 1->0 local at 100% (#372); its siblings `inventory::serialize`/
`deserialize` already `std::for_each` over the same `m_slots`.

## Variant: for_each over a std::vector's begin()/end() stays OUT-OF-LINE in our build

When the container is an STLport `std::vector` (e.g. `vector<T*>`) and you iterate
`std::for_each( v.begin(), v.end(), functor )`, the for_each form is STILL the correct
0-local structure, but our compile may NOT inline it: `begin()`/`end()` go out-of-line
(`call stlp_std::priv::_Impl_vector<...>::end`) and the whole loop folds into ONE `call
stlp_std::for_each<...>` statement, while the TARGET inlined for_each (its begin/end are
direct `_Myfirst`/`_Mylast` member reads `[this+off]`/`[this+off+4]`, and the loop body is
several separate statements attributed to the call-site source lines). Result: target N
statements, our base 1 - a `TRGT_ONLY` cascade in `structure-diff`, low fuzzy %, but the
SOURCE SHAPE (for_each functor, 0 iterator locals) is faithful. This is the STLport
header-template inline boundary, NOT source-steerable from the calling TU - bank it as the
inline-vs-call wall (unlike the raw fixed-array case above, which inlines cleanly because
its begin/end are raw pointer args). Evidence: `player::on_fire`/`jump`/`notify_actions_subscribers`
(player.cpp) - the functor captures the receiver `static_cast<hit_receiver const*>(this)`
and the per-call action; structurally correct, capped on the for_each non-inline.
