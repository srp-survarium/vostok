# weapon_recoil_params::weapon_recoil_params(binary_config_value const&)

Target rva `0x583190` (default ctor `0x5830c0`, matched #116). Public `QAE`.
Same family as `weapon_dispersion_params` config-ctor (#136) and
`character_dispersion_params::load` (#135): `value_exists`-guarded `(float)cfg["name"]`
reads in member order, prefixed by a partial member-init list.

## Commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_recoil_params --list`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x583190 --view target|structure|callees|info`

## Target structure (26 statements, 0x306 bytes)
- L39 (0x00, size 0x98): member-init list.
- L40-L41,L43-L44,L46-L47,L49-L50: first_shoot_side_recoil, shoot_side_recoil,
  first_shoot_back_recoil, shoot_back_recoil - each a plain `value_exists` guard + read.
- L52/L54/L55 (0x14f, size 0x28): shoot_recoil min/max angle block - NESTED guard:
  `if ( value_exists("shoot_recoil_min_angle") && value_exists("shoot_recoil_max_angle") )`
  then `min_angle = (float)cfg["...min_angle"];` and
  `angle_range = (float)cfg["...max_angle"] - min_angle;` (the `subss xmm0,[this+0x10]`).
  Braced (carcass `<1>` close marker after L55).
- L58-L59,L61-L62,L64-L65: additive_recoil_time, additive_side_recoil, additive_back_recoil.
- L68/L70/L71 (0x23d, size 0x28): additive_recoil min/max angle block, same nested
  shape as shoot_recoil, braced.
- L74-L75,L77-L78: side_compensation_speed, back_compensation_speed.

## Key finding: partial member-init list
The default ctor (#116) inits ALL members except additive_back_recoil (0x20).
The CONFIG ctor's init list stores only 9 members (offsets 0x00,0x04,0x10,0x14,
0x18,0x1C,0x24,0x28,0x2C) - i.e. it OMITS first_shoot_back_recoil (0x08),
shoot_back_recoil (0x0C), additive_back_recoil (0x20), back_compensation_speed (0x30).
Reproduced verbatim: the member-init list lists exactly those 9 in declaration order
(all 0.0f except additive_recoil_time = math::epsilon_3). The omitted members are
left uninitialized in the init list (the body assigns them conditionally).

## Anchor
`use_game_core_weapon_recoil_params` in temp_include_all.cpp - extended to also
construct `weapon_recoil_params config_params( cfg )` and escape `&config_params`
through `example_callback`, mirroring `use_game_core_weapon_dispersion_params`.

## Iterations
- v1: partial member-init list (9 members, 0.0f except epsilon_3) + 12 body reads in
  member order, with the two min/max-angle blocks braced as
  `if ( value_exists(min) && value_exists(max) ) { min = ...; range = (float)cfg[max] - min; }`.
  Anchor `use_game_core_weapon_recoil_params` extended to also construct + escape the
  config ctor instance.
- `python3 scripts/rebuild.py` (no module arg, full relink).
- report.json: config ctor `??0weapon_recoil_params@survarium@@QAE@ABVbinary_config_value@configs@vostok@@@Z`
  = **100.0%**; default ctor stayed 100.0%.
- base structure (rva 0x44c890) is identical to target: 26 statements, 0x306 bytes,
  every offset + per-statement delta matches. Clean 100% DONE; carcass deleted.
- report-changes.json regressions (55) are all `100->0` on unrelated COMDAT/template
  symbols (deleting destructors, btXxx, boost::asio, stlp_std, intrusive_ptr<...>) -
  the usual delinker relink churn (69+ improved 0->100 the same way); no game_core
  matched source function regressed.
