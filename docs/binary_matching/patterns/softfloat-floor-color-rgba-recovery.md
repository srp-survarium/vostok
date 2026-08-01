# Recovering `math::color_rgba( r, g, b, a )` from the inlined soft-float `math::floor` chain

tags: cpp:call cpp:float asm:shr asm:sar asm:sbb | topic:color topic:inlining topic:constant-recovery

## Symptom
A ~0x90-per-component block of pure integer bit twiddling with no FPU/SSE
arithmetic in it, repeated four times, ending in a chain of
`shl 8 / or / and 0FF00h / and 0FF0000h / shl 18h`:

```
movss  [esp+48h], xmm1          ; <- the float goes to memory ...
mov    edx, [esp+48h]           ; ... and comes back as its BITS
lea    ecx, [edx-1] / not ecx / and ecx, 80000000h / not ecx / and edx, ecx
sar    eax, 17h / and eax, 0FFh          ; exponent
mov    ecx, 9Eh / sub ecx, eax
or     ebp, 0FF800000h / shl ebp, 8 / shr ebp, cl   ; mantissa
... / neg / sbb / sar 1Fh / inc ...
```

That is `vostok::math::floor( float )` (`math_functions_inline.h`) inlined.
Four of them feeding one packed u32 is `math::color_rgba( float, float, float,
float )` (`math_color.h`), i.e. a `math::color( r, g, b, a )` ctor.

## Reading the arguments back out
`color_rgba(r,g,b,a)` packs `(a<<24) | (r<<16) | (g<<8) | b`. MSVC lays the four
inlined `floor` expansions out **in evaluation order a, r, g, b** (the last
argument first, then the rest left to right), and each one's result is tagged by
the mask/shift it finally gets:

| lane | tell in the final `or` chain | argument |
|---|---|---|
| `shl 18h` | `shl ecx, 18h` then `or` | `a` |
| `and 0FF0000h` | `shl ..., 10h` + `and 0FF0000h` | `r` |
| `and 0FF00h` | `shl ..., 8` + `and 0FF00h` | `g` |
| `and 0FFh` | no shift, `and 0FFh`, `or`ed last | `b` |

Each component's float is the one stored to memory at the top of its block. The
literal is already multiplied by 255 by the compiler, so **divide the constant by
255 to get the source spelling**:

| constant | float | source |
|---|---|---|
| `__real@437f0000` = 255.0 | 1.0 | `1.f` |
| `__real@433f4000` = 191.25 | 0.75 | `0.75f` |
| `__real@42ff0000` = 127.5 | 0.5 | `0.5f` |
| `0.0` (`xorps`) | 0.0 | `0.f` |

Worked example (`renderer::draw_stages_stats`, target line 1480): the two arms of
the row-colour ternary pack `0xFF7FBFFF` and `0xFFBFFFBF`, i.e.

```cpp
const u32 char_color = string_index & 1 ?
    math::color_rgba( 0.75f, 1.f, 0.75f, 1.f ) :
    math::color_rgba( 0.5f, 0.75f, 1.f, 1.f );
```

## When `floor` is a `call` instead
The same source can compile to `fld <const>; fstp [esp]; call vostok::math::floor`
- LTCG out-lines `floor` at some sites and expands it at others **in the same
function** (`draw_stages_stats` calls it at line 1480 and expands it at 1483).
Don't read the call/inline split as a source difference; read only the constants
and the lane masks. Likewise `math::color_rgba` itself has a standalone symbol
(`?color_rgba@math@vostok@@YAIMMMM@Z`) that some sites call - when it is called,
the convention is **`r` in `xmm0`, `g` at `[esp]`, `b` at `[esp+4]`, `a` at
`[esp+8]`** (custom LTCG convention), which reads the arguments off directly.

## Do not over-read the call ORDER
The three `floor` calls in an out-lined arm can be scheduled out of a/r/g/b order
(MSVC treats `floor` as pure and reorders it, and CSEs equal arguments). The
packed value is unambiguous; the call order is not.

## Related
- `assert-eater-recover.md` - `floor`'s own `R_ASSERT` is what makes the inlined
  expansion start at +0xc rather than +0x0.
