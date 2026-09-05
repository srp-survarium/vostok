<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Player input, RNG seed and shotgun helper review

Scope: every `player_input` inline (10), every `normal_random` inline including
the out-of-class `rand_n` definition (17), and the shotgun substate inlines
(five on the base, one finish-state setter): 33 declarations. The start and
one-round substate headers have no inline bodies. Eight literal no-source
markers are resolved; eight random-range overloads remain explicitly open.

Frozen raw target/base class variants agree. `normal_random` and the shotgun
base are fully identical in the topology comparison; player input has one
semantic variant with duplicate-equal-record multiplicity target 1/base 2.
No class layout, declaration access or signatures are widened. No build ran.

## A live seed-restoration bug

`normal_random::set_seed` was an empty body despite its real use in
`weapon_core::deserialize`. Target deserialize statement 4 reads four bytes
and stores the result at weapon +0x458, the RNG seed. The independently fetched
candidate statement only performs the packet read; the store is missing.
Restore `m_seed = seed` and retain that existing call site. This changes actual
post-deserialization RNG state, not just an unused interface.

The existing default constructor is also supported by weapon construction:
target +0x17d addresses the +0x458 member and +0x189 stores seed 1. Restore the
seed-taking constructor as the corresponding supplied-seed initializer; that
overload is a model rather than a separately retained procedure.

`rand_n` is the one retained target RNG procedure (RVA 0xac230). Its expansion
confirms the 0x343fd/0x269ec3 state update, arithmetic shift by 16 and 0x7fff
mask, division by 32767.0, rejection loop using log/exp, and a further random
draw for sign selection. The existing body and zero-argument primitives stay
unchanged, including the possibility of drawing zero before `logf`. Do not
replace the generator, change its draw count or introduce endpoint clamps.

## Missing input helpers explain caller shape

Target `weapon_core::update_bones_matrices` statement 2 calls the virtual
`input()` **once**, then compares the raw masked values `(mask&1)` versus
`(mask&2)`, and `(mask&8)` versus `(mask&4)`. The reconstructed caller repeated
`input()` at four expression sites. Restore that expression in `is_moving`
and call `m_user->input().is_moving()` once. Preserve raw-value comparisons:
opposing keys held together still return true. Boolean-normalized cancellation
would change retail behavior.

All three target `computed_*_recoil_time` functions have zero recorded locals.
Their breath predicate calls `input()` once, tests aim bit 0x80, then breath
bit 0x8000000. Restore `is_aiming` and `is_holding_breath`, and use the latter
directly in all three callers. The extra named `input` reference is no longer
needed; remove its old claim of an unavoidable locals mismatch.

The input handler maps the named fire action to 0x20, aim to 0x80, crouch to
0x100, jump to 0x10, use to 0x10000000, sprint request to 0x200 and breath request
to 0x8000000. The existing effective-sprint body also agrees with its retained
target: sprint request, forward bit 1, and no bits in 0x16e. Its previously
measured compiler qualifiers from `bac8eee14` are left unchanged in this
no-build pass; no new forced-inline/noinline attributes are introduced.

`is_walking` remains a model: moving and not effectively sprinting. The input
mapping has no independent walk request here. Whether the original instead
excluded the sprint-request bit is still an open distinction.

## Private shotgun playback state needs no derived friends

Restore the protected `deserializing()` wrapper as `m_weapon.deserializing()`
and use it in start initialization and finish finalization. Target guards
load the weapon through substate +0x120 and read its +0x493 flag.

All three derived finalizers start with the same target operation as the
existing base `finalize`: assert-shaped dummy use, load playback pointer at
+0x148, then reset it. Replace their direct private-pointer accesses with
`super::finalize()`. This removes the only uses requiring the three derived
friend declarations, so remove those permissions rather than widening the
private field. The remaining protected weapon/state accesses are unchanged.

Base `initialize` is a confirmed framed empty default; `execute` resets the
two playback fields without the extra assert, whereas `finalize` retains the
assert. Do not merge those distinct contracts. The finish state's pointer
setter still binds the owning reload state's readiness flag; it is not itself
a readiness-value assignment.

## Every input inline

| Function | Disposition |
|---|---|
| `is_moving` | Restore target raw-mask comparison and the single-input-call consumer. |
| `is_trying_to_sprint` | Retain request-bit getter and existing compiler qualifier. |
| `is_sprinting` | Retain request/forward/exclusion-mask predicate and existing call-boundary qualifier. |
| `is_walking` | Moving/non-effective-sprint model; request-versus-effective distinction open. |
| `is_crouching` | Retain 0x100 getter. |
| `is_jumping` | Retain 0x10 getter. |
| `is_using` | Retain 0x10000000 getter. |
| `is_aiming` | Restore 0x80 input flag; distinct from weapon's achieved aiming state. |
| `is_shooting` | Restore 0x20 request flag; not weapon's firing-animation flag. |
| `is_holding_breath` | Restore aim-and-breath conjunction and all three recoil consumers. |

## Every normal-random inline

| Function | Disposition / candidate contract |
|---|---|
| Default constructor | Retain seed 1, confirmed by weapon construction. |
| Seed-taking constructor | Restore supplied seed initializer; no extra normalization model. |
| `set_seed` | Restore live deserialized seed store. |
| `seed` | Retain signed seed getter used by weapon serialization. |
| `maxI` | Retain 32767, consistent with the target's 15-bit output mask. |
| `randI()` | Retain exact LCG state update and output extraction observed inside `rand_n`. |
| `randI(s32)` | Open bounded-integer helper: modulo versus multiply/scale and closed/half-open endpoints are not established. |
| `randI(s32,s32)` | Open interval helper: parameter roles, endpoint inclusion and overflow/negative-range rules need a caller. |
| `randIs(s32)` | Open s-suffix contract: symmetric/signed output is plausible, but so is a scaling distinction. No distribution or draw count established. |
| `randIs(s32,s32)` | Open corresponding two-parameter contract; do not assume centre/radius versus min/max. |
| `maxF` | Retain 32767.0, directly identified by target constant bits 0x46fffe00. |
| `randF()` | Retain float conversion divided by maxF; both endpoints are possible. |
| `randF(float)` | Open scaled-uniform model candidate, e.g. base draw times bound; exact operation order, endpoints and negative bound policy unproved. |
| `randF(float,float)` | Open affine-interval model candidate; original parameter roles and arithmetic order remain unproved. |
| `randFs(float)` | Open signed/symmetric candidate; a remapped single draw and a difference of two draws have different distributions and seed advancement. |
| `randFs(float,float)` | Open two-parameter signed/scaled candidate; same unresolved distribution and parameter-role questions. |
| `rand_n(float)` | Retain inspected rejection sampler and sign draw; no algorithm cleanup. |

Only the zero-argument uniform primitives occur in the retained `rand_n`.
The wider source scan finds no consumers of the eight range/suffixed overloads.
Their hypotheses are individually recorded, but the literal no-source markers
remain live; no invented use or arbitrary RNG distribution is added.

## Every shotgun substate inline

| Function | Disposition |
|---|---|
| Base `initialize` | Target frame/this spill/return only; empty default retained. |
| Base `execute` | Retain pointer-targeted playback reset without assert. |
| Base `finalize` | Retain assert plus reset; now owns all three derived copies. |
| Base `set_animation_playback_state_ptr` | Retain playback pointer binding. |
| Base `deserializing` | Restore weapon-flag forwarding and both guard consumers. |
| Finish `set_owner_ready_for_transition` | Retain non-owning bool-pointer binding; owning state supplies its animation-ended flag. |

Twenty arithmetic-model checks cover the 16 movement masks and four aim/breath
combinations; no C++ was compiled. Patch/coverage checks pass. Deferred build
verification must check the restored seed store, one-call/zero-local input
seams and base-finalize expansion boundaries. All 33 methods are accounted for;
walking and eight RNG contracts remain open, not declared recovered.
