# weapon_core batch8: predicates / forwarders

Unit: a coherent batch of still-STUB `weapon_core` members matched off
`origin/int/game_core` (branch `match/game_core-weapon_core-batch8`).

## Functions

### is_sprinting() const  -> 100% (byte-identical; report.json 0% is an ICF artifact)
Target asm:
```
mov ecx,[ebp-4]; add ecx,278h; call weapon_user_animations_selector::is_sprinting
```
0x278 is `m_user_animations_selector`. Source:
```cpp
return m_user_animations_selector.is_sprinting( );
```
Rich-index diff (`pdb_fetch --view diff`, PDB-backed) = **10/10 instructions equal
(100.0%)**. report.json shows 0% only because this 10-byte virtual is ICF-folded with
another identical body in BOTH binaries, so neither delinked `.obj` retains a distinct
COMDAT for it -> the objdiff backend "not found in ...obj" -> can't pair -> 0%. This is
the documented COMDAT-fold 100<->0 artifact (ik_processor-ctor-activate.md). Source is
correct; nothing to fix.

Commands:
```
pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_core::is_sprinting" --list   # 0x5932c0
pdb_fetch --target-index ... --base-index ... --view diff --function "weapon_core::is_sprinting"          # 10/10 equal
pdb_fetch ... --objdiff-base-dir ... --objdiff-target-dir ... --function "weapon_core::is_sprinting"      # 'not found in ...obj' (folded)
```

### could_be_used(base_player const&) const -> 81.19% PARTIAL
### could_be_aimed(base_player const&) const -> 75.88% PARTIAL
Both compute `broken_hands_count = (*user.damage_model()).broken_hands_count()`:
- `user.damage_model()` = virtual at vtable+0Ch returning `resource_ptr<damage_model> const&`.
- `operator*` -> `damage_model&`; reads bytes [+33A]+[+33B] = `m_broken_hands_count[0..1]`.
  Filled the empty inline `damage_model::broken_hands_count()` body (modeled on
  `broken_legs_count`): `return m_broken_hands_count[0] + m_broken_hands_count[1];`

Logic:
```cpp
// could_be_aimed
return broken_hands_count != 2;
// could_be_used  (the !(A && B) temp-store-0/1 shape)
return broken_hands_count != 2 || !m_is_double_handed;   // m_is_double_handed @ +0x48A
```
Control flow + every member read is **byte-identical**. Sole residual: the smart-ptr
`operator*` shape on `(*user.damage_model())`. Target calls `operator*` out-of-line on the
returned `resource_ptr const&` (no copy):
```
call eax(damage_model); call operator*; mov [ebp-8],eax  (damage_model&)
```
Our /GL copies the resource_ptr into a temp then inlines operator*:
```
call eax; mov [ebp-8],eax; mov byte[ebp-2],0; lea eax,[ebp-2]; call <resource_ptr copy>; mov edx,[ebp-8]; mov eax,[edx]
```
That is an LTCG inline/copy decision at the operator* call boundary - source-identical,
not source-steerable here. Wall.

Header: `could_be_used`/`could_be_aimed` are public (QBE) - unchanged. Anchored via
pointer-to-member in temp_include_all `use_game_core_weapon_core_base_state()` so a
standalone COMDAT exists for objdiff to pair (they otherwise inline into is_trying_to_aim).

### can_and_must_reload_predicate() const -> 85.32% PARTIAL
```cpp
return ready_to_reload( ) && m_ammo_in_magazine == 0 && !m_is_round_chambered;
```
m_ammo_in_magazine @ +0x47A, m_is_round_chambered @ +0x48E. Members + &&-chain shape
byte-identical. Sole residual = LTCG inline-vs-call of the trivial `ready_to_reload()`
stub: target `call ready_to_reload` into a temp (frame 0x30); our /GL inlines its
`return true` to `mov eax,1` (frame 0x08). Out-lining ready_to_reload would risk the
other inline callers (must_chamber predicates). Same inline class as
must_chamber_a_round_and_animation_ended_predicate (87% in this file).

### can_and_must_reload_and_animation_ended_predicate() const -> 86.17% PARTIAL
```cpp
return current_base_state( ).has_animation_ended( ) && can_and_must_reload_predicate( );
```
m_logic @ +0x414 -> current_state() @ +0x10. Control flow byte-identical; residual = the
ready_to_reload inline (inherited) + has_animation_ended inline class.

Both `can_and_must_reload_*` and `horizontal/vertical_recoil_value` were in a `public:`
header section but mangled `ABE` (private) - moved their declarations into a `private:`
block so objdiff pairs them.

## Deferred (walls)
- `horizontal_recoil_value` / `vertical_recoil_value`: real bodies (is_aimed() call +
  recoil_calculator::get_X_coeff + math::clamp_r + statics c_anim_center/epsilon/NegFloat).
  is_aimed() is the same inline/out-line wall as update_recoil - PARTIAL at best.
- `get_ammo_slot`: switch over profile_slot_id() with two static jump tables
  (weapon_ammo_slots + table@9BE634h) whose contents must be reversed; profile_slot_id
  is also a call (currently inline). Tractable but a separate unit.
- `process_finger_correction` / `on_skeleton_matrices_changed` / `update_dispersion`:
  ASSERT (finalize_impl) + conditional forwarders into more empty/compiled-out stubs.
