# weapon_core batch2 - small setters + recoil-time-calculator getters

Unit (one PR), all in `sources/vostok/game_core/sources/weapon_core.cpp`:

- `instant_idle_end`              -> 100% DONE
- `instant_toggle_start`         -> 100% DONE
- `instant_toggle_end`           -> 100% DONE
- `backward_recoil_time_calculator`   -> 100% DONE
- `horizontal_recoil_time_calculator` -> 100% DONE
- `vertical_recoil_time_calculator`   -> 100% DONE

Branched off fc3aadf9 (PR #175's anchors absent -> the report-changes "100->0"
entries are the clean-base accounting artifact, not regressions).

## Commands

```
nix develop -c true
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "recoil_time_calculator" --list
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_core::instant_" --list
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x593f20 --view target  # vertical
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x593f70 --view target  # horizontal
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x593fc0 --view target  # backward
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x592a10 --view target  # instant_toggle_end
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x592a30 --view target  # instant_toggle_start
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x592a50 --view target  # instant_idle_end
nix develop -c python3 scripts/rebuild.py
```

## instant_* setters

Target asm (instant_toggle_end):
```
mov  eax, [ebp-4]
mov  byte ptr [eax+494h], 0
```
Member offsets from `binaries/structure/target/headers/survarium/weapon_core.h`:
- `0x0492` = `m_is_idle`
- `0x0494` = `m_is_toggling`

So:
- `instant_idle_end`      -> `m_is_idle = false;`
- `instant_toggle_start`  -> `m_is_toggling = true;`
- `instant_toggle_end`    -> `m_is_toggling = false;`

These are reached via the `weapon_core` vtable anchor already present
(`use_game_core_weapon_core_base_state`'s `weapon_core weapon;`), so no extra
anchor needed. First rebuild -> all three 100%.

## recoil-time-calculator getters

All three target bodies identical shape (sret FastDelegate6), e.g. vertical:
```
mov  dword ptr [ebp-8], survarium::weapon_core::computed_vertical_recoil_time
mov  dword ptr [ebp-4], 0
... call (FastDelegate bindmemfunc, delinker-misnamed simple_lock) ...
[edx+4] = func_ptr ; [edx] = this + 0  ; return ptr
```
This is a single return statement constructing the delegate from `this` + a
member function pointer:
```
return calculator_functor( this, &weapon_core::computed_vertical_recoil_time );
```
`calculator_functor` is the `typedef FastDelegate<float(float,float,u32,u32,u32,float)>`
already on the class. The `computed_*_recoil_time` members are non-const (matches
the `Y* pthis` non-const FastDelegate6 ctor).

These getters are non-virtual and were NOT being kept by any anchor (absent from
`binaries/rich/base/index.jsonl`, 0% with symbol present in report). Added an
anchor in `temp_include_all.cpp` inside `use_game_core_weapon_core_base_state`:
call each getter on the existing `weapon` instance and escape the result via
`example_callback`. Second rebuild -> all three 100%.

## Result

All six 100% DONE. No real regressions (report-changes regressed list empty after
excluding 100->0 clean-base artifacts).
