# character_dispersion_params {ctor, load}

Unit: the small struct `survarium::character_dispersion_params` (game_core), two
functions matched together:
- `character_dispersion_params::character_dispersion_params()` (default ctor)  target rva 0x590c90
- `void character_dispersion_params::load(configs::binary_config_value const&)`  target rva 0x590d80

Branch off `match/game_core-legs_ik_processor-leg_params-setters` (#134).

## Member layout (14 floats, struct size 0x38)
idle@0 idle_aim@4 walk@8 walk_aim@0xc run@0x10 jump@0x14 crouch@0x18
crouch_aim@0x1c crouch_walk@0x20 crouch_walk_aim@0x24 prone@0x28 prone_aim@0x2c
injury_penalty_for_double_handed@0x30 injury_penalty_for_one_handed@0x34
(matches header; calculator uses 0..0x24 + 0x30/0x34).

## Commands run
- `pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep character_dispersion`
  -> ctor 0x590c90, load 0x590d80 (both in character_dispersion_params.cpp).
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x590c90 --view target`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x590d80 --view target`
- `pdb_fetch ... --rva 0x590c90 --view structure` -> 1 statement (L28 = the `{`).

## Mangled names / visibility
- `??0character_dispersion_params@survarium@@QAE@XZ` -> ctor, `QAE` = public.
- `?load@...@@QAEXABVbinary_config_value@configs@vostok@@@Z` -> public.
Header already had both `public:` (ctor + load). No access change needed.

## ctor asm
14 identical `mov reg,[ebp-4]; movss xmm0,[clear_value]; movss [reg+off],xmm0`,
offsets 0x00..0x34. `clear_value` is the delinker's name for the `.data` block.

Decoded the target `.obj` (objdump can't read this COFF; parsed COFF by hand in
python): `.data` holds 14 floats, all `0x3f800000` = `1.0f`. The 14 `movss`
relocations target `clear_value+0, +4, ... +52` (one .data slot per member).
This is exactly what a member-init list of `member( 1.0f )` produces under /Od:
verified against the already-laid-out (target) `character_recoil_params.cpp.obj`,
whose 4-member `member(1.0f)` init list compiled to 4 separate `1.0f` in `.data`
(rawsize 16, 4x 0x3f800000). So ctor = member-init list, all members `1.0f`.
(NOT a single reused .rdata literal.)

## load asm
14 identical blocks, brace-less `if` (each block has a `je short` over the body,
no `+0x002` brace jump in the carcass):
```
push "<member_name>"; mov ecx,[ebp+8]; call value_exists; test al; je .next
push "<member_name>"; mov ecx,[ebp+8]; call operator[]; call operator float;
mov reg,[ebp-14h]; movss [reg+off], xmm0
```
=> `if ( cfg.value_exists("name") ) name = (float)cfg["name"];` per member, in
member order. Identical shape to the already-written `character_recoil_params::load`
(reference). Member order matches the strings in the asm.

## Anchor (temp_include_all.cpp)
Added `use_game_core_character_dispersion_params()`:
- construct `character_dispersion_params params;` (ctor),
- `configs::binary_config_value cfg; params.load( cfg );` (load),
- `example_callback( reinterpret_cast<pcstr>(&params) );` to OBSERVE the object so
  LTCG does not DSE the constant ctor stores (the #107 18% trap; README rule).
Registered in the `IncludeAll` driver after `use_game_core_weapon_recoil_params`.
Added `#include <vostok/game_core/character_dispersion_params.h>`.

## Results
`python3 scripts/rebuild.py` (no module arg, full relink). report.json:
- `??0character_dispersion_params@survarium@@QAE@XZ`  -> 100.00%
- `?load@character_dispersion_params@survarium@@QAEXABVbinary_config_value...@Z` -> 100.00%

`pdb_fetch --view diff` for both: no `-`/`+` divergence lines (clean byte match).
Both functions matched first try; no iteration needed.

report-changes: code 27.19% -> 27.20% (+0.01), functions +11 net. 52 regressed /
49 improved - ALL of them `scalar deleting destructor` / `vcall` thunk /
`empty_stub` / boost storage ICF churn in optimized modules (bullet, scaleform,
stlp, sound, ai, particle, ui, physics) that flip 100%<->0% on every relink. None
touch character_dispersion_params or any function in this unit. Not a real
regression (the symmetric improved list is the same churn classes).

STATE: ctor 100% DONE, load 100% DONE. Carcass deleted (clean 100%).
