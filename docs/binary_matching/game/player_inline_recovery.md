<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Player inline review

Scope: all **33 inline declarations** in `game/sources/player.h`, including
its inline virtual overrides. Twenty-three carried no-source markers at the
`855dc1283` baseline; thirteen already had field/forwarding bodies, ten were
inert placeholders. The retained out-of-line player methods remain covered by
the match ledger and were inspected here where they supply consumer evidence.

The raw target/base class comparison finds the same 0x10f88 layout and the
same 153-declaration semantic variant. The only difference is duplicate equal
record multiplicity (three target records versus two base records). No access
qualifier, friend, member or method order is changed in this bundle.

## New reconstruction

`player::set_near_plane_factor`, target RVA `0x5d2590`, checks the input
controller and writes `factor * 0.05f` to `[controller + 0x4c]`. That is the
camera's near-plane field through the controller's base subobject. The
existing camera setter already implements the store; its alleged missing-body
wall in the old source comment was stale.

The declared private `player::set_near_plane(float)` now forwards the supplied
distance to the controller. Its factor wrapper keeps the null-controller guard
and calls this helper. This preserves the current behavior and restores the
named seam. The original helper's guard placement is a source hypothesis;
batch-check the caller partition after compilation is permitted. The existing
pre-edit report is one statement, 0x18 bytes on both sides, STRUCTURE MATCH.

`get_player_max_carried_weight` now returns
`m_stamina.get_max_carried_weight()`, replacing the inert zero return. This is
a model supported by the stamina configuration loader and
`player_parameters_cook`, which modifies the same maximum to incorporate
additional weight. No client consumer was found for this private accessor, so
this is not a proven runtime bug fix or a new byte-match claim.

## Every inline declaration

| Function | Disposition |
|---|---|
| `transform` | Existing root/render transform reference, not the physical current transform. Preserve field identity. |
| `get_target` | Existing mutable target-state reference. |
| `get_current` | Existing const current-state reference. |
| `is_visible` | Existing visibility field return; `hide`/`show` own updates. |
| `team` | Existing team ID override; not a missing body. |
| `get_profile_name` | Existing profile-name buffer return. |
| `damage_model` | Existing delegation to `base_player::damage_model`; preserve that base seam. |
| `get_input_handler` | Existing reference through the local-controller pointer; requires attachment. |
| `set_target_fov_factor(float)` | Open: not the retained two-argument overload. Default transition duration, reuse of current duration and immediate assignment differ. The optimized two-argument procedure's dropped constant argument does not prove the one-argument body. |
| `target_fov_factor` | Existing target FOV field getter, not the interpolated current value. |
| `history` | Existing const history-container reference. |
| `get_target_character_transform` | Existing target physics transform reference. |
| `get_player_max_carried_weight` | New stamina-backed model; no observed caller. |
| `get_transform` | Existing virtual current physics transform, intentionally distinct from `transform()`. |
| `input` | Existing current input reference. |
| `position` | Existing current-transform translation reference. |
| `get_look_pitch` | Existing current-state pitch. |
| `physics_controller` | Existing target-state controller reference. |
| `stamina` | Existing mutable stamina reference. |
| `is_replaying_history` | Existing replay flag return. |
| `local_time` | Existing identity mapping override; no invented clock adjustment. |
| `on_animation_event(5 args)` | Open: old callback signature has no source binding; constant call-again return is not a recovered event dispatcher. Need its event/channel consumer. |
| `on_animation_ik_interval(5 args)` | Open: the actual client binds `weapon_core::on_animation_ik_interval(animation_callback_params&)`. That method owns leg-IK updates; adapting the old signature needs a consumer and ownership contract. |
| `on_weapon_aim_started` | Open: active weapon code owns FOV/near-plane changes. No source call to this private hook establishes which effects the earlier player hook performed. |
| `on_weapon_aim_ended` | Open: same ownership issue for resetting aim/FOV state. Do not duplicate the weapon's effects in an unproven hook. |
| `set_near_plane` | New forwarding seam in the retained factor wrapper, with null guard kept at that caller. |
| `update_recoil(u32,u32)` | Open: base player holds immutable recoil parameters, while `weapon_core` owns the live calculator and target-time/time-scale path. The old two-integer interface does not establish a conversion or forwarding policy. |
| `update_dispersion(u32,u32)` | Open: live weapon path derives movement, aim state and broken-hand count before ticking its calculator. No player call identifies the roles of the two integers. |
| `update_breath_vibration(u32,u32)` | Open: weapon path owns holding-breath input, calculator state and time-scale update. A wrapper cannot be chosen from matching names alone. |
| `animation_player()` | Existing mutable current-state animation player, not target-state player. |
| `animation_player() const` | Same current-state owner with const return. |
| `is_demo_player` | Existing demo flag return. |
| `game_ui` | Existing UI pointer return. |

## Ownership checks and remaining work

The old player update/callback declarations are not labelled editor-only.
Current source binds the one-argument weapon IK callback to toe/heel channels;
it updates `m_legs_ik_processor` and returns call-again. The weapon also has
retained recoil, dispersion and breath-vibration implementations with different
parameter types. These are useful semantic leads, not proof that the obsolete
player signatures were empty or that unconditional delegation would be right.

Result: thirteen stale markers removed, two bodies supplied, **eight remaining
contracts explicitly open**. All 33 inline declarations reviewed. No C++ build,
README score update or ledger refresh accompanies this source-only batch.
