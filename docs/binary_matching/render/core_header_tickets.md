# Shared-core-header tickets raised by the render campaign

Findings that cap render functions but live OUTSIDE `sources/vostok/render/`
(in `sources/vostok/` core headers). They are recorded here rather than fixed
in-flight for one reason: **a shared-header edit changes the effective-source
fingerprint of every function that sees it, which RESETS their banked `max` %**
repo-wide. Under the campaign's MAX-% goal that is a real cost, so these get a
deliberate, measured core-structure pass - not a drive-by edit between matcher
batches.

Each ticket states the evidence, the correct diagnosis, and what a fix must
prove before it is kept.

## T1 - `math_functions_inline.h`: `min` family is `__forceinline`, `max` family is not

Raised by batch B2 as "the target has non-template `math::max(u32,u32)`/`min`
overloads we don't declare". **That diagnosis is wrong** - we do declare them
(`math_functions_inline.h:94-107` for min, `:134-178` for max). The real
asymmetry:

- min overloads (u8/u16/u32/u64) are `__forceinline`
- max overloads (float/s8..s64/u8..u64) are plain `inline`

Target evidence (`match.db` symbols):

| symbol | our status |
|---|---|
| `unsigned int vostok::math::max(unsigned int, unsigned int)` | PAIRED 45.4% |
| `float vostok::math::max(float, float)` | PAIRED 100% |
| `unsigned int vostok::math::min(unsigned int, unsigned int)` | **TARGET_ONLY** |
| `int vostok::math::max(int, int)` | TARGET_ONLY |

The target ships an out-of-line `min(u32,u32)` **function with an RVA**. A
`__forceinline` body this trivial (`min_integral`) would never be emitted
out-of-line, so the original source most likely spelled it plain `inline`.
The target ALSO ships the template instantiation
`math::max<unsigned int>(unsigned int const&, unsigned int const&)` alongside
the non-template overloads, so both forms coexist there.

Caps observed: `backend_handlers` textures/samplers `assign` + `set_overwrite`
(our template inlines where the target calls).

**Fix to test:** drop `__forceinline` to `inline` on the min family only.
**Keep criterion:** builds + the target-only `min(u32,u32)` starts pairing.
Accept a current-% dip elsewhere (faithful-change rule) but measure the
fingerprint-reset cost across modules before committing.

## T2 - `math_color.h`: `color` union byte order contradicts `color_rgba` packing

`class color` (`math_color.h:115-160`) contains a genuine internal
contradiction, independent of any target question:

- `color_rgba(r,g,b,a)` packs `(a<<24)|(r<<16)|(g<<8)|b` -> little-endian
  bytes are **[0]=b [1]=g [2]=r [3]=a**
- the union declares **[0]=r [1]=g [2]=b [3]=a**

So `set_R()` (writes union `r`, byte 0) and `get_R()` (`color_get_R` = `>>16`,
byte 2) touch DIFFERENT channels. One of the two spellings is wrong in our
source today; the accessors and the packer cannot both be faithful.

Batch B2 reports the target "reads channels in union order" and that this caps
both `backend::clear_render_targets(color*)` overloads at ~81%.

**This one is a SEMANTIC change** (it swaps R/B at every call site in the
engine), not just a codegen change, so it needs target disassembly of a
specific channel read (a `clear_render_targets` colour unpack, or any
`get_Rf`-consuming site) proving the shift amount - not inference from the
union alone. Do not "fix" it by making the two sides agree arbitrarily; fix it
to whatever the target's shifts say, then re-measure.

## Handling

Run these as ONE deliberate core-structure pass with a full before/after
`match_db.py diff` across ALL modules (not just render), once the render root
lanes have drained the work that does not depend on them. Both tickets were
raised from render but neither is render-owned.
