# The `s32 compare( a, b )` ordering family: `>` for the second test, STLport `std::min` for the loop bound

tags: cpp:compare cpp:operator cpp:loop | asm:cmp asm:sbb asm:jbe asm:lea | topic:codegen-idiom topic:condition-shape

## Symptom
A reconstructed three-way `compare` (`-1` / `+1` / `0`) has the right STRUCTURE and even
the right byte SIZE, yet every comparison in the diff is operand-swapped:

```
- 0x1e: cmp   ecx, eax      ; base
+ 0x1e: cmp   eax, ecx      ; target
- 0x20: jbe   short .2
+ 0x20: jae   short .2
```

and a container `compare` that walks two sequences differs by a whole ~0x20-byte block
in the min-of-two-sizes prologue:

```
- cmp eax, ecx / sbb edx,edx / neg / neg / sub eax,ecx / and edx,eax / add edx,ecx   ; base
+ mov [esp+14h],eax / cmp eax,ecx / mov [esp+0Ch],ecx / lea eax,[esp+14h] / jb .1
+ lea eax,[esp+0Ch] / .1: mov eax,[eax]                                              ; target
```

## Cause + fix, part 1 - the second comparison is `>`, not a reversed `<`
The house three-way compare is

```cpp
if ( left.x( ) < right.x( ) )
    return -1;
if ( left.x( ) > right.x( ) )      // NOT `right.x( ) < left.x( )`
    return 1;
```

Both spellings are semantically identical and MSVC CSEs them into ONE `cmp` with two
conditional jumps - but the surviving `cmp`'s operand order follows the spelling. Writing
the second test as `right < left` makes the optimizer canonicalise on `cmp right, left`
and invert every predicate (`jbe`/`ja` where the target has `jae`/`jb`); writing it as
`left > right` keeps `cmp left, right` for both tests, which is what the target emits.
The tell is the SECOND comparison of a merged web reusing the FIRST one's operand order.
Applies to the trailing ternary too:
`return a < b ? -1 : a > b ? 1 : 0;` (not `... : b < a ? 1 : 0`).
`vostok::intrusive_ptr` defines the full `< <= > >=` set against both `self_type` and
`object_type const*`, so the `>` form is available for smart-pointer elements as well.

## Cause + fix, part 2 - the loop bound is STLport `std::min`, not `math::min`
`vostok::math::min` has `__forceinline u8/u16/u32/u64/s8.../min( T, T )` by-VALUE overloads
that forward to `min_integral` (`right + ((left - right) & -(left < right))`), so
`math::min( a.size( ), b.size( ) )` compiles to the branchless neg/sbb/and/add sequence.
STLport's `std::min` is `template <class T> const T& min( const T& a, const T& b )
{ return b < a ? b : a; }` - it returns a REFERENCE, so both operands must be spilled to
stack slots and the compiler selects an ADDRESS (`lea`/`lea`/`mov eax,[eax]`). That
address-select shape is the target's, i.e. the source says `std::min`, and the argument
order is `std::min( <this side>, <other side> )` (`b < a ? b : a` picks `b` when `b` is
smaller, so `jb` -> `&b` identifies `b` as the second argument).

## Cause + fix, part 3 - keep the min in a NAMED local, not in the `for` condition
`for ( u32 i = 0; i < std::min( a.size( ), b.size( ) ); ++i )` is NOT equivalent in
codegen: MSVC re-evaluates the whole min (both size divisions plus the address select)
on every iteration. The target hoists it once, which is what the plain
`u32 const size = std::min( ... ); for ( u32 i = 0; i < size; ++i )` gives you. Measured
on `res_texture_list::compare`: 59% with the named local, 31% with the min inlined into
the loop condition. A `xor <counter>,<counter>` scheduled BEFORE the min computation is
scheduling noise, not evidence for the in-condition form.

## Precedent
`vostok::render` compare family (batch B4): `compare(shader_constant const&, ...)`,
`res_texture_list::compare` x2, `res_sampler_list::compare` x2,
`shader_constant_table::compare`, `compare(res_xs<T> const&, ...)` - all six share the
skeleton
`u32 const size = std::min(this_n, other_n); for(...) { if (a<b) return -1; if (a>b) return 1; }`
`if (this_n < other_n) return -1; return other_n < this_n ? 1 : 0;`.

## Related
- `friend-in-class-template-nontemplate-mangling.md` (the free `compare(res_xs<T>&, ...)`
  is a friend defined inside the class template, not a function template).
