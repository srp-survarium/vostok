# weapon_dispersion_params {ctor, config-ctor}

Unit: the small struct `survarium::weapon_dispersion_params` (game_core), two
ctors matched together (same pattern as character_dispersion_params #135):
- `weapon_dispersion_params::weapon_dispersion_params()` (default ctor)  target rva 0x582de0
- `weapon_dispersion_params::weapon_dispersion_params(configs::binary_config_value const&)` (config ctor)  target rva 0x582e70

Branch off `match/game_core-character_dispersion_params` (#135).

## Member layout (8 floats, struct size 0x20)
base_dispersion@0 from_the_hip_multiplier@4 aim_multiplier@8 speed_of_aiming@0xc
one_shoot_dispersion_amount@0x10 reload_dispersion_amount@0x14 growth_speed@0x18
max_dispersion@0x1c (matches the active header).

## Commands run
- `pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep weapon_dispersion_params`
  -> ctor 0x582de0, config ctor 0x582e70.
- `pdb_fetch --target-index ... --rva 0x582de0 --view target` / `--view structure`
- `pdb_fetch --target-index ... --rva 0x582e70 --view target` / `--view structure`
- hand-parsed the target COFF `binaries/objdiff/target/.../weapon_dispersion_params.cpp.obj`
  (`.data`/`.rdata` floats + `.text` relocations) in python to decode every
  constant store.

## Mangled names / visibility
- `??0weapon_dispersion_params@survarium@@QAE@XZ` -> default ctor, `QAE` = public.
- `??0weapon_dispersion_params@survarium@@QAE@ABVbinary_config_value@configs@vostok@@@Z`
  -> config ctor, `QAE` = public.
Active header is a `struct` (members public by default), both ctors declared
before the `public:` so default access = public = matches `QAE`. No change.

## Default ctor asm (2 statements: init-list, `}`)
8 constant stores, offsets 0x00..0x1c. Decoded each `movss` reloc to its
`.data`/`.rdata` slot:
- [0x00] base_dispersion        <- .rdata[+0x4]  = 0.0f
- [0x04] from_the_hip_multiplier <- .data[+0x0]  = 1.0f
- [0x08] aim_multiplier          <- .data[+0x4]  = 1.0f
- [0x0c] speed_of_aiming         <- .data[+0x8]  = 1.0f
- [0x10] one_shoot_dispersion_amount <- .data[+0xc]  = 1.0f
- [0x14] reload_dispersion_amount <- .data[+0x10] = 1.0f
- [0x18] growth_speed            <- .data[+0x14] = 1.0f
- [0x1c] max_dispersion          <- .rdata[+0x8] = 2.0f
=> member-init list: base_dispersion(0.0f), from_the_hip..growth_speed(1.0f x6),
max_dispersion(2.0f). (.data slots all 1.0f are the delinker's `clear_value`
pool; the two non-pool constants 0.0f/2.0f live in `.rdata`.)

## Config ctor asm (19 statements)
- L33 (0x00): member-init list = identical 8 constant stores as the default ctor
  (decoded the prologue relocs: base_dispersion <- .rdata[+0xc]=0.0f,
  six 1.0f from .data[+0x18..+0x2c], max_dispersion <- .rdata[+0x10]=2.0f).
- L34-L49 (0xa6..0x210): 8 brace-less `if(value_exists("name")) name=(float)cfg["name"];`
  blocks in member order (each block = `<0x14>` if + `<0x1c>`/`<0x1d>` assignment;
  no `+0x002` brace jump => brace-less). Strings in order: base_dispersion,
  from_the_hip_multiplier, aim_multiplier, speed_of_aiming,
  one_shoot_dispersion_amount, reload_dispersion_amount, growth_speed,
  max_dispersion.
- L52 (0x22d, `<0x13>`): TRAILING constant assignment
  `one_shoot_dispersion_amount = 0.0f;` (movss xmm0,[.rdata +0x154 = 0.0f];
  movss [eax+0x10],xmm0). This is reproduced verbatim - matching, not fixing:
  the target unconditionally clears one_shoot_dispersion_amount AFTER the if
  blocks, overwriting whatever cfg set.
- L53 (0x240): `}` epilogue (ret 4).

## Anchor (temp_include_all.cpp)
Added `use_game_core_weapon_dispersion_params()`: construct a default-ctor obj
and a config-ctor obj (`weapon_dispersion_params config_params(cfg)`), then
`example_callback(&...)` each so LTCG observes the constant stores (else DSE
empties the ctors - the #107 trap). Registered in IncludeAll after
`use_game_core_character_dispersion_params`. Added the header include.

## Results
`python3 scripts/rebuild.py` (no module arg, full relink). report.json:
- `??0weapon_dispersion_params@survarium@@QAE@XZ`  -> 100.00%
- `??0weapon_dispersion_params@survarium@@QAE@ABVbinary_config_value@configs@vostok@@@Z` -> 100.00%

`pdb_fetch --view diff` for both: no `-`/`+` divergence lines (clean byte match).
Both ctors matched first try; no iteration needed.

report-changes: ctor 25%->100%, config ctor 0%->100% (both in `improved`).
61 regressed / 69 improved - ALL of them `scalar/vector deleting destructor`,
`vcall` thunk, `empty_stub`, boost `_bi::storage/value/list` ICF storage, math
default ctors (float3/float4/float4x4), `noncopyable`, `interlocked_increment`,
`multi_thread` etc. in optimized modules (bullet/boost/scaleform/stlp/particle/
engine/render/sound/ai) that flip 100%<->0% on every relink. None touch
weapon_dispersion_params or any deliberately-matched game_core function
(checked: no dispersion/recoil/stealth/weapon_state/legs_ik fn regressed). Not a
real regression (symmetric improved list is the same churn classes).

STATE: ctor 100% DONE, config ctor 100% DONE. Carcass deleted (clean 100%).
