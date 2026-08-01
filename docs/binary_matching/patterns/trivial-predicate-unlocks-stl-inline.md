# `call __lower_bound<..., bool (__cdecl*)(T const&,T const&), ...>` where the target has an inlined binary search = OUR predicate is too big

- **confidence**: 9/10
- **tags**: `cpp:template cpp:function-pointer cpp:stl | asm:call asm:cmp asm:sar | topic:inline-vs-call topic:stlport topic:structure-shape`
- **symptoms**: base emits `call stlp_std::priv::__lower_bound<T*,T,bool (__cdecl*)(...),...>`;
  the target has no such call and instead shows an open-coded halving loop
  (`mov ecx,eax / sar ecx,1 / cmp [edx+ecx*8+K], reg / jae`) with the COMPARISON
  itself inlined; sema verdict `BRANCH-COUNT` with a large block deficit
  (22 base vs 37 target).

## What is going on

The comparator is a **free function passed by pointer**, and its symbol exists
on both sides - so the difference is not "functor vs function pointer". MSVC
constant-folds the function pointer and inlines the whole algorithm *only if the
comparator body is small enough*. A two-clause predicate is over the threshold;
a one-clause predicate is under it, and then `lower_bound` (and the following
`vector::insert`) collapse into the caller.

So the useful reading is inverted from the usual one: **the un-inlined STL call
is a symptom of OUR PREDICATE being wrong**, not of an inliner difference.
Fetch the predicate's own target body before touching the caller.

## Worked example

`vostok::render::constant_data_predicate` - target body, 19 bytes:

```
mov eax, [esp+4]
mov ecx, [eax+4]
mov edx, [esp+8]
cmp ecx, [edx+4]
sbb eax, eax
neg eax
ret
```

i.e. exactly `return left.class_id < right.class_id;`. Ours carried a second
clause (`|| (class_id == && data_ptr <)`), which both diverged from the target
and blocked the inlining of `std::lower_bound` at all four
`effect_constant_storage::store_constant<T>` instantiations.

Removing the second clause: predicate 100%, and the four `store_constant<T>`
rows went 50.5/51.0/54.6/54.8% -> 73.9/75.9/71.7/71.2% with the first
binary-search loop becoming block-for-block identical (batch B7).

## Corollary - check the LOOP condition too

The same function's scan loop showed the mirror image: the target's
`for ( ; found != end( ); ++found )` had **no** `&& found->class_id == class_id`
guard (target loop tail is a bare `add esi,8 / cmp esi,[edi+4] / jne`). A
comparison term that only exists in our source shows up as one extra branch per
iteration - read the loop tail, not just the loop head.
