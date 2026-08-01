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

## T4 - unmeasured paired rows are an INLINE-HEADER SPLIT problem, not a tooling gap

**CORRECTED 2026-08-01 by batch B5. The original diagnosis below was wrong and
the "don't chase these" instruction it produced was actively costing us wins.**

**Actual cause.** objdiff units come from the delinked TARGET objects (one per
source file) and pairing is keyed on `unit + mangled symbol`. The original tree
split non-trivial inline bodies out of `<x>.h` into a sibling `<x>_inline.h`.
Our reconstructions kept those bodies in the main header, so a byte-identical
body lives in a DIFFERENT UNIT than the target's and can never pair. Those
target units appear in `binaries/objdiff/objdiff.json` with
`base_path: ./dummy.obj` - 319 such units at time of writing.

**It is fixable by matchers, cheaply, with zero code change.** B5 relocated
eight header groups and paired ~80 functions: `backend_inline.h`
`reset_render_targets` 100%, `shader_constant_slot_inline.h` ctor 100%,
`custom_config_value_inline.h` float3/float4 100%, `state_cache_inline.h` 8/8
avg 95%, `effect_manager_inline.h` 44 rows avg 86%, plus `res_pass::apply`
34.9 -> 77.9%. Render fuzzy moved 30.7% -> 32.0% on that batch alone; the
unmeasured render count fell 247 -> 208.

**Detection recipe (no build required):** `grep dummy.obj
binaries/objdiff/objdiff.json` lists target units with no base counterpart;
any `*_inline.h` there is a header group we have not split. Cross-check with
`sqlite3 docs/binary_matching/match.db "SELECT unit, COUNT(*) FROM paired
WHERE fuzzy_pct IS NULL GROUP BY unit ORDER BY 2 DESC"`.

Pattern file: `patterns/inline-header-split-pairing.md`.

**Status after batch B6: DONE for `render/core`.** B6 relocated
`res_xs_hw_impl.h` (9 rows), `sources/xs_descriptor.cpp` (7), `res_effect.h` (7,
folding in four of our headers), `effect_options_descriptor.h` (3),
`destroy_data_helper.h` and `shader_constant_buffer_inline.h` (1). Render fuzzy
32.0% -> 32.9%, unmeasured render rows 208 -> 143. Every remaining `render/core`
mismatch is engine-owned, include-cycle blocked, an ICF/neighbour artifact, or a
missing template instantiation rather than a misplaced body - see the B6 section of
`matching_campaign.md`. **`render/engine` and `render/facade` have NOT been swept.**

**Residual after the splits are done:** some rows will remain unmeasured for the
original reason (a header COMDAT with genuinely no diffable object pair). Only
THAT remainder is a tooling matter. Re-measure before assuming any given row is
in it.

*(Original, superseded diagnosis: "header-defined functions land in COMDATs
whose unit maps to a header path and there is no object pair to diff -
a delinker/objdiff attribution fix, i.e. tooling work, not matcher work. Do NOT
let matchers chase these." That reasoning mistook a symptom for the cause.)*

## Handling

Run these as ONE deliberate core-structure pass with a full before/after
`match_db.py diff` across ALL modules (not just render), once the render root
lanes have drained the work that does not depend on them. Both tickets were
raised from render but neither is render-owned.
