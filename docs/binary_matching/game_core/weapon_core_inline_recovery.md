<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Weapon core and animation-selector inline review

Scope: all 58 explicit/implicit `weapon_core` inlines and all seven selector
inlines, including previously unmarked bodies. This removes 19 literal
no-source markers across the two headers and replaces the unmarked zero-return
`ammo_in_weapon` placeholder. No C++ build or score refresh was run.

The frozen target/base raw class comparisons have the same single semantic
variant in each case: weapon size 0x498/205 declarations, selector size
0x50/52 declarations. Only duplicate-equal-record multiplicity differs (4:1
and 5:1 respectively). This supports the existing access/layout/interface,
not a post-edit build claim or proof that retail had no friend declaration.

## Recovered seams and caller evidence

- `ammo_in_weapon`: `weapon::set_ui_ammo` adds the normalized chamber flag at
  +0x48e to the magazine word at +0x47a with **16-bit** arithmetic. Retail
  `weapon_core::ready_to_reload` computes the same total and stores it in a
  word temporary. The PDB interface returns `u16`; no `m_ammo_in_weapon` field
  exists. Restore that total and use it in reload/UI consumers. The old
  32-bit-add versus 16-bit-add wall claim was premature: a typed inline return
  explains the narrowing, independently of argument-register allocation.
- `is_active`: target `tick` statement 4, `serialize` statement 12 and the
  analogous deserialize gate load `m_logic` at +0x414, then its current state
  at +0x10, materialize a non-null bool, and test it. Activation installs a
  state; deactivation calls `set_initial_state(NULL)`. It does **not** clear
  `m_user`, so `m_user != NULL` would be the wrong lifecycle model. Extract
  the existing predicate and propagate it to those three gates.
- `ready_to_fire` and `has_chamber_a_round_state`: `is_ready_to_shoot` reads
  +0x489 and +0x48d as separate terms. Preserve that distinction: the former
  getter is not a replacement for the complete ammunition/queue predicate.
- Queue helpers: `fire_queue_length` reads an index byte at +0x47e and then
  a byte from the +0x454 array. `weapon::set_next_fire_queue_type` has the same
  expansion. The type is an **index**, not the returned queue length. Target
  deserialize statement 9 calls `packet_reader::r<bool>` then stores AL at
  +0x47e, without an assert or clamp. Preserve that unusual reader type and
  route the store through the setter, and the array lookup through the getter.
- `set_user_animations`: cook `on_core_subresources_ready` statement 8 adds
  +0x278 and calls `weapon_user_animations_selector::set_animations`. Restore
  the weapon wrapper around that exact operation and use it at the cook.
- `backward_recoil_value`: the final statement of
  `computed_backward_recoil_time` calls `get_back_coeff`, clamps against the
  existing `epsilon` and `clear_value - epsilon` data, then multiplies by the
  animation length. `selected_animations` contains the same clamp. Extract
  the private inline next to those existing file-local constants, before its
  two consumers. Do not substitute header literals or invent external data
  merely to make its definition header-resident. Both uses are in this TU.
- `set_forced_not_to_sprint`: selector `tick` stores false/true at +0x4c in
  its two existing arms. The private helper now owns those stores; conditions
  and the following FSM tick are unchanged.

These are supported semantic extractions, not proof of the exact original
inline source boundaries. Target and candidate bodies were fetched separately;
the two-sided ICF alias-selection defect cannot establish a match here.

## Access enforcement

The old `friend class weapon` was explicitly introduced to permit open-coded
private-member reads. Direct loads in an optimized caller cannot establish
that friendship: they can equally be inlined public accessors. Remove that
invented permission and use the existing PDB-declared interface throughout
`game/sources/weapon.cpp`: ammo totals/magazine, queue length, ammo slot,
animation selector, view/aim flags, FOV/near-plane factors, weapon target,
double-handed status, and the **base** transform getter.

Preserve genuine distinctions: `weapon` has its own shadowing `m_transform`
and `m_last_tick_time_in_ms`; do not redirect those members to the base.
`m_user` is protected and remains available for the existing reference-based
binds. The nested death-state helper's `m_random` is its own member.
No field visibility, layout, virtual slot or function signature is broadened.
The separate cook friendship is not adjudicated by this read-access pass.

## Every weapon-core inline

“Retain” below means the existing body was checked against its declaration,
member ownership and uses. It is not a new byte-verification claim. Model
getters without a discriminating retail consumer remain explicitly identified.

| Function | Disposition |
|---|---|
| `set_recoil_params` | Retain corresponding parameter-object assignment. |
| `get_recoil_params` | Retain const reference to the same object. |
| `set_dispersion_params` | Retain corresponding parameter-object assignment. |
| `get_dispersion_params` | Retain const reference to the same object. |
| `get_dispersion_calculator` nonconst | Retain member reference. |
| `get_dispersion_calculator` const | Retain const member reference; do not collapse overloads. |
| `get_magazine_capacity` | Retain u16 capacity getter. |
| `ammunition` | Retain resource pointer by value, including its reference-count lifetime. |
| `ammo_in_weapon` | Replace zero with magazine plus normalized chamber flag; propagate total consumers. |
| `ready_to_fire` | Restore `m_ready_for_fire`; used by the full shoot predicate. |
| `is_firing` | Existing field getter; remove stale marker. |
| `is_toggling` | Retain `m_is_toggling || !m_is_shown`, not merely the first flag. |
| `get_bullet_transform` | Retain const reference to fire-bullet transform, not the model transform. |
| `get_target` | Retain target enum getter; use in derived target comparisons. |
| `set_fire_queue_type` | Restore direct byte-index assignment; propagate deserialize store. |
| `get_fire_queue_type` | Restore index getter; used by `fire_queue_length`. |
| `get_bullets_in_queue` | Retain u16 remaining-queue getter. |
| `get_transform` | Retain base matrix by value; replace qualified private-base reads in derived code. |
| `transform` | Retail copies 0x40 bytes from +0x158; retain virtual matrix return. |
| `set_transform` | Retail copies 0x40 bytes to +0x158; retain assignment. |
| `hit_initiator_holder` | Retain const pointer result from initiator holder. |
| `get_skeleton` | Ownership-based model returns `m_skeleton` by value. Setter targets +0x408; no direct getter procedure/consumer found. |
| `on_ammo_empty` | Retail framed return with no work; retain empty default. |
| `user_animations_selector` nonconst | Retail `add eax,278h; ret`; retain member reference. |
| `user_animations_selector` const | Retain corresponding const reference; ownership-based existing body. |
| `can_hold_breath` | Retail reads +0x488; retain aimed flag return. |
| `is_idle` | Retain `m_is_idle || (m_aimed && !m_is_firing)`; visible in target tick. |
| `set_bullet_damage` | Retain compiled-out-assert shape plus damage store; assert predicate remains a recorded guess. |
| `get_bullet_damage` | Retain damage field getter. |
| `set_bullet_pierce` | Retain corresponding assert/store shape; no new predicate claim. |
| `get_bullet_pierce` | Retain pierce field getter. |
| `set_aim_fov_factor` | Retain factor assignment. |
| `aim_fov_factor` | Retain getter; use at derived aim transition. |
| `set_aim_near_plane_factor` | Retain factor assignment. |
| `aim_near_plane_factor` | Retain getter; use at derived aim transition. |
| `set_weapon_fire_queue_types` | Retain non-owning array pointer and count stores; no allocation/copy introduced. |
| `set_user_animations` | Restore selector forwarding wrapper and cook use. |
| `user_animations` | Ownership-based model delegates to selector `animations()`; no separate container member exists. Original consumer remains open. |
| `get_user` | Retain nullable pointer result; not an activity test. |
| `cast_weapon_core` nonconst | Retail identity return, same RVA 0x9b350 as const overload. |
| `cast_weapon_core` const | Retail identity return; preserve const interface. |
| `is_third_view` | Restore getter for the mutable flag set during animation selection; use in derived rendering. |
| `has_chamber_a_round_state` | Restore configuration flag getter; not the currently-chambered flag. |
| `chamber_a_round_on_reload` | Retain distinct reload-policy flag getter. |
| `load_ammo_on_next_activate` | Retain one-way flag setter; activation consumes/resets the flag. |
| `deserializing` | Retain transient deserialize flag getter. |
| `is_active` | Restore non-null FSM current-state predicate and propagate three gates. |
| `target_predicate` | Retail compares +0x46c against argument; retain private equality predicate. |
| `fake_callback` | Retail returns zero and pops callback argument; named call-me-again enum retained. |
| `on_before_fire` | Retail frame/this spill/epilogue only; empty default confirmed. |
| `on_after_fire` | Same independently fetched empty default. |
| `on_reload` | Same independently fetched empty default. |
| `on_chamber_a_round` | Same independently fetched empty default. |
| `on_show` | Same independently fetched empty default. |
| `on_hide` | Same independently fetched empty default. |
| `on_unload_chambered_round` | Same independently fetched empty default. |
| `current_base_state` | Retain checked downcast/dereference of current state; do not add a null-state fallback. |
| `backward_recoil_value` | Extract existing clamped coefficient; preserve file-local data and both consumers. |

## Every selector inline

| Function | Disposition |
|---|---|
| `set_animations` | Retain resource-pointer assignment; actual target cook calls this method. |
| `animations` | Retain dereference of owned container pointer. |
| `user` | Retain asserted dereference, unlike weapon's nullable pointer getter. |
| `logic` | Retain FSM reference, used to install derived death/preview states. |
| `is_right_leg_supporting` | Retain supporting-leg flag getter. |
| `set_player_logic_initial_state` | Retain non-owning state pointer assignment; derived activation sets the preview initial state. |
| `set_forced_not_to_sprint` | Restore false/true store helper and both tick uses. |

No no-source body remains in these two headers. Two ownership-based getter
models and the extracted caller boundaries still require evidence/verification;
the synchronized source TODOs and review index retain those questions. Deferred
verification must inspect both structure and selected identities, particularly
the u16 UI totals, by-value matrix getter expansions and private recoil helper.
