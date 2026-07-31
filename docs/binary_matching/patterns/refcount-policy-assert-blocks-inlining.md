# `call single_threading_policy::increment<unsigned int>` where the target has `inc dword ptr [eax]`
tags: cpp:template cpp:assert cpp:inline | asm:call asm:inc asm:add | topic:inline-wall topic:codegen-idiom
symptoms: base emits `call vostok::threading::single_threading_policy::increment<unsigned int>` / `::decrement<unsigned int>` (plus `push`/`add esp,4`) at every `intrusive_ptr`/`resource_ptr` assignment, where the target has a bare `inc dword ptr [eax]` / `add dword ptr [eax], 0FFFFFFFFh`
confidence: 9/10

Every `intrusive_ptr` copy/assign/destroy funnels through
`threading::single_threading_policy::increment/decrement` (`sources/vostok/threading_policies.h`).
The target inlines them to a single `inc`/`add -1`; our base calls them out of line, so *every*
refcounted assignment costs ~0x18 extra bytes and the enclosing function's `SIZE` rows never close.

The cause is the `ASSERT` in the template body: under MASTER_GOLD an `ASSERT` still emits
`call empty_stub`, which inflates the instantiation past MSVC's inline budget.

```cpp
template <class IntType>
static inline IntType increment( IntType& value )
{
    ASSERT( value != boost::integer_traits<...>::const_max );   // <- the inline blocker
    return ++value;
}
```

PROOF that the target's copy has no assert - compare the out-of-line instantiations that DO
survive in each image:

```
target  0x8d8b0   9 bytes  single_threading_policy::increment<long volatile>(long&)
                           mov eax,[esp+4] / inc dword ptr [eax] / mov eax,[eax] / ret
base    0x43c20  38 bytes  single_threading_policy::increment<unsigned int>(unsigned int&)
```

9 bytes = no `empty_stub` call, and the target has **no** out-of-line
`increment<unsigned int>` at all (it is inlined everywhere); our base has both, at 38 bytes.

This is an ENGINE-WIDE lever, not a per-TU one: it caps every `res_*` ctor/assignment in
render (`res_xs<T>::res_xs` 29-40%, `signature_layout_pair` 36%, `res_xs_hw<vs_data>::create_hw_shader`
53%, `effect_compiler::end_pass`), and the same shape appears wherever `resource_ptr` is used.
Diagnose it with `--view diff`: a `STRUCTURE MATCH` whose every row is `SIZE +0xNN` and whose
base side shows the `push` / `call ...increment` / `add esp,4` triple is this wall, not a source
shape problem - do not "fix" the caller.
