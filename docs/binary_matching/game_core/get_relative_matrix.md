# math::get_relative_matrix  (target rva 0xbb050)

Mangled: `?get_relative_matrix@math@vostok@@YA?AVfloat4x4@12@ABV312@0@Z`
Free `__cdecl` returning `float4x4` by value (sret). Real source file:
`sources/vostok/math_float4x4_inline.h` (after `remove_scale`). It is the inline
math helper called by `legs_ik_processor::get_foot_fixed_transform`.

Result: **90.2% PARTIAL -> 100% DONE** (40/40 instructions equal). No regressions.

## Build note (header in the PCH)
math_float4x4_inline.h is pulled into the game_core PCH via
extensions.h -> math_extensions.h -> math_float4x4.h, so a header edit alone does
NOT rebuild. Before EACH rebuild:
```
rm -f "binaries/Win32/intermediates/Master Gold/game_core/vostok_game_core-static-gold.pch"
touch sources/vostok/game_core/sources/pch.cpp
nix develop -c python3 scripts/rebuild_watchdog.py --stall 150
```
Score read from THIS worktree's `binaries/objdiff/report.json`
(`fuzzy_match_percent` for the mangled symbol).

## Commands
```
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
    --function get_relative_matrix --rva 0xbb050 --view target
nix develop -c pdb_fetch --base-index binaries/rich/base/index.jsonl \
    --function get_relative_matrix --rva 0x089090 --view base
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
    --base-index binaries/rich/base/index.jsonl --function get_relative_matrix \
    --rva 0xbb050 --view diff
```

## Target asm (carcass = 8 statements, 0x66 bytes)
```
0x0b lea ecx,[ebp-40h]; call allocator<char>::allocator   ; <0x8>  L408 float4x4 inverted_parent_matrix;  (default ctor, ICF-folded with empty allocator ctor)
0x13 lea eax,[ebp-40h]; push; mov eax,[ebp+10h]; call try_invert ; <0x13> L409 if (!...try_invert(parent_matrix))
0x24 jne .1                                                ; if try_invert succeeded -> else body (.1)
0x26 int3                                                  ; <0x1>  L411 DEBUG_BREAK( )
0x27 lea ecx,[ebp-80h]; call allocator; mov ecx,eax; call identity; rep movsd; mov eax,[ebp+8] ; <0x20> L412 return float4x4().identity();
0x45 jmp .2                                                ; the return's jmp to epilogue
0x47 jmp .2                                                ; <0x2>  L414 the if-block '}' jmp, OVER the else, to epilogue
.1:
0x49 lea edx,[ebp-40h]; push; ...; call operator*; ...; mov eax,[ebp+8] ; <0x17> L416 return original_matrix * inverted_parent_matrix;  (else body)
.2:
0x60 pop edi; pop esi; mov esp,ebp; pop ebp; ret          ; <0x6>  L418 '}'
```

## The two fixes
Initial body (90.2%):
```cpp
float4x4 inverted_parent_matrix;
if ( !inverted_parent_matrix.try_invert( parent_matrix ) ) {
    ASSERT( UNKNOWN_EXPRESSION );          // WRONG
    return float4x4( ).identity( );
}
return original_matrix * inverted_parent_matrix;   // WRONG: trailing return, not else
```

### Fix 1: L411 is `int3`, not an empty ASSERT  (90.2 -> 97.5)
The carcass said L411 = 0x1 byte. A normal compiled-out ASSERT-with-call is ~0xc
bytes (`mov byte; lea; call empty_stub`). But in **Master Gold** `ASSERT` expands
to `VOSTOK_EMPTY_EXPRESSION` (debug_macros.h:192) = `if(::vostok::identity(false)){}
else(void)0` = **zero bytes** - so `ASSERT(UNKNOWN_EXPRESSION)` here produced NO
int3 and collapsed L411. The 1-byte int3 is `DEBUG_BREAK( )` = `__debugbreak`
(debug_macros.h:26), which emits exactly `0xCC` (+0x001), matching the ladder_cook
precedent (`<...>|+0x001:'118' DEBUG_BREAK( )`). Replacing the ASSERT with
`DEBUG_BREAK( );` recovered the int3 -> 97.5%.

### Fix 2: if/else, not if + trailing return  (97.5 -> 100)
At 97.5 the only diff was a missing `jmp short .2` at target 0x47 - the if-block's
closing-brace jmp. Reading the target carefully: that `jmp .2` jumps to the EPILOGUE
(.2), skipping the multiply at 0x49. A plain `if (!x) { ...; return; } return mul;`
would have the `}` FALL THROUGH to `mul` (no jmp over it), and MSVC /Od folds the
return's jmp with the brace into one. For the if-`}` to jump OVER the multiply to the
epilogue, the multiply must be the **else body**. Rewrote as:
```cpp
if ( !inverted_parent_matrix.try_invert( parent_matrix ) ) {
    DEBUG_BREAK( );
    return float4x4( ).identity( );
} else {
    return original_matrix * inverted_parent_matrix;
}
```
The `jne .1` at the top is the cond branch to the else, the if-body returns identity,
the if-`}` jmps over the else to .2, and the else-`}` merges with the function `}`
(L418). -> 100%, 40/40 equal.

## Residual
None. Clean 100% DONE; carcass stripped per convention.

## get_foot_fixed_transform
The sole caller; unchanged at 84.158% (it inlines a debug-draw guard around this
call but matching the out-of-line body does not change its own score).
report-changes.json regressed: [] after the final clean relink.
