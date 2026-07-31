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

## T3 - `single_threading_policy::increment/decrement` ASSERT blocks inlining engine-wide

Raised by batch B3. The `ASSERT` inside
`single_threading_policy::increment/decrement` (`sources/vostok/threading_policies.h`)
emits `call empty_stub` in MASTER_GOLD, inflating the instantiation to 38
bytes, so MSVC calls it out-of-line at **every** `intrusive_ptr` assignment.
Target evidence: its surviving `increment<long volatile>` is **9 bytes** - too
small to hold a `call empty_stub` (~0xc) - and it ships **no** out-of-line
`increment<unsigned int>` at all. So the shipped source's increment/decrement
carry no assert.

This is the whole residual of `res_xs<T>::res_xs` (30-40%),
`signature_layout_pair` (36%), `res_xs_hw<vs_data>::create_hw_shader` (53%),
and a large share of `end_pass` / `set_texture`. Write-up:
`patterns/refcount-policy-assert-blocks-inlining.md`.

Blast radius is every `intrusive_ptr` in the engine, so it belongs in the same
deliberate core pass as T1/T2 - but its evidence is the strongest of the three,
and it is the only one of the three that is a pure codegen change (no
semantics move). Note it cuts AGAINST the usual repo rule that MASTER_GOLD
asserts must be preserved as `call empty_stub` sites: here the target proves
these two specific functions have none.

## T4 - 247 render paired functions are UNMEASURED (fuzzy_pct IS NULL)

Not a source defect - a scoring blind spot found while checking B3's
"stripped functions" claim. B3 reported ~8 `resource_manager` create_* fns as
unpaired/stripped; they are in fact **paired by address** (both `base_rva` and
`target_rva` present) but carry **NULL `fuzzy_pct`** - objdiff never produced a
measurement for them.

Scale (per `match.db`, at batch B3):

| module | paired | unmeasured | share |
|---|---|---|---|
| render | 1,295 | 247 | 19.1% |
| scaleform | 433 | 73 | 16.9% |
| vostok | 1,007 | 155 | 15.4% |
| stlport | 1,209 | 161 | 13.3% |
| boost | 2,453 | 278 | 11.3% |
| core | 1,213 | 18 | 1.5% |

So it is **systemic, not render-specific** - but render is on the high end
because it is header-inline heavy. The unmeasured rows cluster exactly there:
`effect_manager_inline.h` 40/40, `res_effect.h` 10/10, `state_cache_inline.h`
8/8, `backend_inline.h` 8/8, `res_xs_hw_impl.h` 5/5, plus 69 with no unit
attribution at all. Header-defined functions land in COMDATs whose unit maps to
a header path, and there is no object pair to diff.

**Implication for the "render to 100% max" goal:** these 247 functions cannot
reach 100% by matching - they are not scored at all, yet they sit in the
2,647-function denominator the README reports. That is a ~9% hard ceiling on
render's headline exact %, independent of match quality. Closing it is a
delinker/objdiff attribution fix (map header COMDATs to a diffable object
pair), i.e. tooling work, not matcher work. Do NOT let matchers chase these.

## Handling

Run these as ONE deliberate core-structure pass with a full before/after
`match_db.py diff` across ALL modules (not just render), once the render root
lanes have drained the work that does not depend on them. Both tickets were
raised from render but neither is render-owned.
