<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Breath-state and calculator inline review

Scope: 40 declared inline bodies: seven on `breath_vibration_calculator`, six
on `weapon_recoil_calculator`, three on `character_dispersion_calculator`,
17 on the active breath-state family, and seven declarations in its duplicate
public header. The implicit active base destructor is checked additionally,
not counted as a newly reconstructed declaration. Outlined constructors and
callers supply the evidence; no build or ledger refresh ran.

## Seven real constant mismatches, including disabled vibration

The retail disassembler calls a pooled 1.0f constant `clear_value`. The name
is not its value. Decode each instruction's absolute operand, translate it
through its own PE image base, and read the four bytes:

| Operation | Target instruction RVA | Frozen base instruction RVA | Target/base float |
|---|---:|---:|---|
| Normal-state multiplier at +0x24 | 0x58384b | 0x56f96b | 1 / 0 |
| Short-breath state restoring speed at +0x28 | 0x583947 | 0x56fa67 | 1 / 0 |
| Calculator current multiplier at +0x20 | 0x583bd8 | 0x56fcf8 | 1 / 0 |
| Calculator target multiplier at +0x24 | 0x583be8 | 0x56fd08 | 1 / 0 |
| Calculator character multiplier at +0x30 | 0x583c18 | 0x56fd38 | 1 / 0 |
| Calculator initial reserve at +0x34 | 0x583c28 | 0x56fd48 | 1 / 0 |
| Weapon update's character-multiplier setter | 0x593a0d | 0x58bb1d | 1 / 0 |

All seven target operands address VA 0xa7b6c4, bytes `00 00 80 3f`.
The corresponding frozen candidate operands address VA 0x804808, bytes
`00 00 00 00`. Target/base methods were fetched independently to avoid the
documented ICF alias-selection problem. Restore all seven source constants.
The existing horizontal/vertical calculator outputs start at **zero** in both
images; the holding lower bound and insufficient-reserve threshold are also
raw-verified zero. Do not replace every floating zero mechanically.

The update setter is a direct behavioral bug: `tick` multiplies both vibration
outputs by `m_character_multiplier`. Setting it to zero on every update
suppressed that output regardless of the state machine. The normal-state
multiplier also matters after parameter setup: the inherited setter only
stores the parameter pointer; the calculator then copies the normal state's
multiplier into its current/target values. Short-breath restoring speed is
subsequently replaced by max-time/repair-time on non-null parameter setup;
its initial 1.0 is still part of the retail constructor contract.

The frozen `initialize_logic` target and base already have the same 11
statements and 0x3bd-byte extent, including the same frame. Remove its stale
claim that an unavoidable frame difference explained the remaining mismatch.
Equal statement shapes do not establish equal referenced data. New codegen
and data agreement await the deferred build, not an old score claim.

## One breath-state owner, not a second implementation

Only `sources/breath_holding_states.h` and its inline companion are included
by the current calculator implementation. The public `breath_state.h` had no
source includers and independently redeclared the same class with incorrect
private fields/public constructor and five literal stubs. Replace that
duplicate with includes of the existing canonical declarations and bodies.
No class is removed from the public include path and no second class is added.

The active base constructor remains protected; its shared reserve, parameters
and multiplier remain protected for the three derived states. The target base
constructor calls the two base constructors, writes its vfptr, then binds the
reserve reference at +0x1c. It does **not** initialize the two other fields.
Do not add blanket initialization or NOVTABLE here. The implicit destructor
already supplies the inherited virtual interface; target scalar-deleting
destruction at RVA 0xbb3c0 performs base cleanup and conditional delete without
new member ownership. Do not add a hand-written destructor just because a
generated PDB declaration exists.

Retail base initialize/execute/finalize share the framed empty return at RVA
0x1a800, not the one-byte non-returning trap discussed in the inventory review.
Keep them genuinely empty. Put execute/finalize definitions beside initialize
in the existing inline owner, matching their target file attribution.

Raw class comparison finds identical active base, weapon-recoil and
breath-calculator variants. Character dispersion has one identical semantic
variant with target two/base one duplicate-equal records. This is not a
field/access discrepancy. The generated header's boilerplate inheritance
spelling is not substituted for the raw comparison.

## Every calculator inline (16)

| Function | Disposition and evidence |
|---|---|
| Breath `set_max_breath_delay_time` | Open. Setting current reserve, clamping it, and changing maximum duration are different contracts. There is no separate max-delay field; params is const. No caller or retained procedure found. Do not const-cast configuration or declare the no-op verified. |
| Breath `set_user` | Retain non-owning +0x14 pointer binding used by weapon activation/deactivation; it does not reset the FSM. |
| Breath `get_horizontal_value` | Retain +0x2c load; weapon horizontal recoil reads weapon +0x3a0. |
| Breath `get_vertical_value` | Retain +0x28 load; weapon vertical recoil reads weapon +0x39c. |
| Breath `set_character_multiplier` | Retain +0x30 assignment; fix its live weapon caller to supply retail 1.0. |
| Breath `hold_button_state_equals_to` | Retain +0x38 bool comparison; target RVA 0xbb300 and both bound transition predicates confirm it. |
| Breath `insufficient_breath` | Retain reserve <= 0.0; target RVA 0xbb320 and raw zero constant confirm it. |
| Weapon recoil `set_interpolation_time` | Model: assign `animation::linear_interpolator(time)` to the owned interpolator. Its duration is private; no new friendship/setter is invented in animation. No consumer establishes assignment versus reconstruction, assertions or elapsed-timer reset policy. |
| Weapon recoil `set_character_multiplier` | Retain +0x14 store. Recoil tick calls character tick, weapon tick, then copies character +0x8 (outer +0x4c) through this setter. |
| Weapon recoil `set_player_compensation_multiplier` | Retain direct +0x18 field model consumed by `process_compensation`; no current setter call found. No validation/clamping evidence. |
| Weapon recoil `get_vertical_koef` | Retain +0x28 getter; emitted enclosing vertical getter contains that load. |
| Weapon recoil `get_horizontal_koef` | Retain +0x2c getter; emitted enclosing horizontal getter contains that load. |
| Weapon recoil `get_back_koef` | Retain +0x30 getter; emitted enclosing back getter contains that load. |
| Character dispersion `reset` | Open. Constructor-zero state, configured idle-value reset, and clock-only reset are distinguishable models. Parameter setter writes all three values from idle multiplier, while first tick gates on a zero clock. No caller selects the intended values/clock/jump policy. |
| Character dispersion `get_value` | Retain +0x0c value getter used by enclosing dispersion calculation; no target/current substitution. |
| Character dispersion `set_aiming_speed` | Retain +0x14 store; target `apply_aim_speed` writes it at outer +0x34 before setting the weapon calculator's speed. |

## Every active breath-state inline (17)

| Function | Disposition and evidence |
|---|---|
| Base `set_breath_holding_params` | Retain pointer store at +0x20; target RVA 0xbb360. It does not alter the normal multiplier. |
| Base `get_multiplier` | Retain +0x24 getter; consumed by calculator setup and tick. |
| Base `initialize` | Confirmed empty framed default, RVA 0x1a800. |
| Base `execute` | Same confirmed empty default; clear stale marker and use inline owner. |
| Base `finalize` | Same confirmed empty default; clear stale marker and use inline owner. |
| Base constructor | Retain shared-reference binding only after base construction; target RVA 0xbb380. |
| Holding constructor | Retain base construction only; target initialize_logic +0xd3 through +0xee. No multiplier store here. |
| Holding `is_ready_for_transition` | Retain true; explicitly fetched target return at RVA 0xd23e0. |
| Holding `set_breath_holding_params` | Retain pointer store, null guard, supplied params +0x8 multiplier; target RVA 0xbb450, three statements. |
| Holding `tick` | Retain assert-shaped use and max(reserve - dt, 0); target RVA 0xbb480. |
| Normal constructor | Fix inherited multiplier assignment to 1.0, preserving body assignment to the protected base field. |
| Normal `is_ready_for_transition` | Retain true; shares the normal/holding target return. |
| Normal `tick` | Retain assert-shaped use and min(reserve + dt, configured max); target RVA 0xbb400. |
| Short-breath constructor | Fix own restoring-speed initializer to 1.0, retaining base construction. |
| Short-breath `set_breath_holding_params` | Retain pointer store, null guard, multiplier at params +0xc, and max-time/repair-time quotient; target RVA 0xbb4d0, four statements. |
| Short-breath `tick` | Retain assert-shaped use and min(reserve + restoring_speed * dt, max); target RVA 0xbb520. |
| Short-breath `is_ready_for_transition` | Retain assert and exact reserve == max comparison, not >= or epsilon test; target RVA 0xbb570. |

Pure virtual `tick` has no body to reconstruct. The base destructor contract
is covered above, not an instruction to introduce a new source declaration.

## Every duplicate-header inline (7)

| Removed duplicate declaration | Canonical owner reached by the same public include |
|---|---|
| `set_breath_holding_params` | Existing inline pointer setter, not a second empty setter. |
| `get_multiplier` | Existing inline getter. |
| `initialize` | Existing empty inline default. |
| `execute` | Confirmed empty inline default. |
| `finalize` | Confirmed empty inline default. |
| Constructor | Existing protected constructor with reference binding. |
| Destructor | Existing compiler-generated virtual destructor, no new ownership. |

## Verification and remaining questions

Read target procedures, constructor expansions, source callers and raw PE
constants; check the active raw class variants. Clangd's reference probe did
not return a useful method-use set here, so include/source scans plus the
actual calculator call sequence establish ownership rather than that probe's
absence. Patch checks pass. No C++ or tool builds ran and no existing score
was refreshed or presented as verification of these edits.

The two literal open bodies are max-breath-delay and character-dispersion
reset. Recoil interpolation has an explicit model and a remaining timer/
source-form question. These are individually registered, not falsely declared
unused/server-only or completed merely because no out-of-line call survives.
