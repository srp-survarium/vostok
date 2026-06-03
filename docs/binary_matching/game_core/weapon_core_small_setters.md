# weapon_core small setters/accessors batch

Unit: a batch of small `weapon_core` setters/accessors + the sibling
`weapon_user_animations_selector::get_current_state_id` (forced in by `update_recoil`).

Branch: `match/game_core-weapon_core-small-setters` off `fc3aadf9` (fixed base).

## Functions and final state

| function | final | notes |
|---|---|---|
| `set_magazine_capacity` | 100% DONE | `ASSERT(UNKNOWN_EXPRESSION_T(magazine_capacity)); m_magazine_capacity = magazine_capacity;` (leading `call finalize_impl` = compiled-out ASSERT) |
| `set_skeleton` | 100% DONE | `m_skeleton = skeleton;` (resource_ptr::operator= @ +0x408) |
| `instant_show` | 100% DONE | `m_aimed = false; on_show();` (virtual @ vtbl+0xB0) |
| `instant_hide` | 100% DONE | `on_hide();` (virtual @ vtbl+0xB4) |
| `update_breath_vibration` | 100% DONE | `hold_breath(is_holding_breath); set_character_multiplier(0.0f); tick(current_time, time_scale)` — required implementing inline `breath_vibration_calculator::set_character_multiplier` (store @ +0x30) |
| `update_recoil` | 72.04% INPROGRESS | see below (is_aimed inline/out-of-line conflict) |
| `get_body_part_mask_for_user` | 83.89% INPROGRESS | `current_base_state().get_body_part_mask_for_user()`; off by 1 /Od temp |
| `get_current_state_id` | 71.5% INPROGRESS | `current_state().id()` chain inlined; off by 1 /Od temp |

## Access-specifier fixes (mangled name pairing)

Target mangling forced moving three weapon_core methods from public to private:
- `update_recoil`  -> `AAE` (private)
- `update_breath_vibration` -> `AAE` (private)
- `get_body_part_mask_for_user` -> `ABE` (private const)

`is_aimed` / `get_current_state_id` are `QBE`/`QBE` (public const) — left public.

The new `temp_include_all` anchor `use_game_core_weapon_core_small_setters` is friended
in `weapon_core.h` so it can call the now-private methods.

### IMPORTANT loop note (ninja header-dep miss)
After moving the three methods to `private:` in `weapon_core.h`, `rebuild.py` reported
`+0.00 / 0 changed` and the base index still showed the OLD `QAE` mangling — ninja did
NOT recompile `weapon_core.cpp` on the header change. `touch`ing `weapon_core.cpp`
forced the recompile and the obj symbol flipped to `AAE`. If an access/visibility edit
in a header shows no score movement, `touch` the defining `.cpp` and rebuild.

## update_recoil — is_aimed inline/out-of-line conflict (the 72% wall)

Body: `m_recoil_calculator.tick( m_user_animations_selector.get_current_state_id(), is_aimed(), current_time_in_ms, time_scale );`
(arg order from the push order: state_id, is_aimed, current_time, time_scale — recoil_calculator::tick(character_state, is_aiming, current_time, time_scale)).

Target emits `call survarium::weapon_core::is_aimed` (a standalone symbol exists,
`?is_aimed@...@@QBE_NXZ`, FPO: `mov al,[eax+488h]; ret`). Under `/Od` our `inline
is_aimed() { return m_aimed; }` is always expanded inline (`mov [this+0x488]`), so the
call is missing — capping update_recoil at ~72%.

Making `is_aimed` out-of-line (declared in header, defined in .cpp) DID get update_recoil
to **99.78%**, BUT it FPO-broke is_aimed across all its inline call sites and net cost
**~26 exact functions** (overall matched_functions 8005 -> 7984). The target had it both
ways (standalone + inlined per call site) which `/Od` cannot reproduce. Reverted to inline;
update_recoil stays INPROGRESS at 72.04%. Net: keeping inline is_aimed is +1 overall.

(Only real source caller of `is_aimed` is `dispersion_calculator::get_dispersion`, which
slightly improved when is_aimed was out-of-line — but not worth the COMDAT collateral.)

## get_body_part_mask_for_user / get_current_state_id — the 1-temp /Od gap

Both are single-`return` accessors that inline a `current_state()`-style chain:
`[m_logic+0x10]` (current_state) then a member at a fixed offset (`+0x130`
body_part_mask, `+0x20` state id). Target materializes ONE more `/Od` stack temp than
our base (target 4 temps / `sub esp,14h`; base 3 / `sub esp,10h` after adding an
explicit `const&` local — was 2 before). The extra temp is a reference-materialization
that depends on the exact original expression nesting of `current_base_state()` /
`current_state()`. Adding `weapon_core_base_state const& state = current_base_state();`
moved get_body_part 73% -> 83.89%; the analogous local did not move get_current_state_id
off 71.5%. Left both INPROGRESS with carcass preserved.

## Commands
```
pdb_fetch --target-index binaries/rich/target/index.jsonl --function "weapon_core::set_magazine_capacity" --view target
pdb_fetch --base-index   binaries/rich/base/index.jsonl   --function "weapon_core::update_recoil"          --view base
python3 scripts/rebuild.py            # NO module arg (relinks EXE so score refreshes)
python3 scripts/match_score.py        # honest overall rollup
```
