# game_core::dispersion_calculator::dispersion_calculator() (default ctor)

Target rva: `0x586920`
Mangled: `??0dispersion_calculator@survarium@@QAE@XZ` (`QAE` = public, non-const, __thiscall)
Source: `sources/vostok/game_core/sources/dispersion_calculator.cpp` (~line 29)
Unit: ctor only (set_weapon / apply_aim_speed are separate units, not inlined here).

## Goal
Verify the drafted member-init list and anchor it so LTCG does not DSE the
constant stores (the same constant-ctor pattern that hit 100% on
character_dispersion_params / weapon_dispersion_params).

## Target asm (`pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x586920 --view target`)
```
0x00: push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx
0x07: mov ecx,[ebp-4]
0x0a: call survarium::weapon_dispersion_calculator::weapon_dispersion_calculator   ; m_weapon_calculator @0x00
0x0f: mov ecx,[ebp-4]; add ecx,20h
0x15: call survarium::character_dispersion_calculator::character_dispersion_calculator ; m_character_calculator @0x20
0x1a: mov eax,[ebp-4]; mov dword ptr [eax+40h],0          ; m_weapon = NULL
0x24: mov ecx,[ebp-4]; movss xmm0,[clear_value]; movss [ecx+44h],xmm0  ; m_shooting_skill_coeff
0x34: mov edx,[ebp-4]; movss xmm0,[clear_value]; movss [edx+48h],xmm0  ; m_aiming_speed_coeff
0x44: mov eax,[ebp-4]; mov esp,ebp; pop ebp; ret          ; return this
```
Structure: 2 statements - L22 (init list / `{`) 0x44 bytes, L23 (`}`) 0x7 bytes.

## Constant verification
The rich/delinker view masks the movss operand as `[clear_value]`, and the
delinker target obj's own `.rdata` slots (0x0, 0x4) read 0.0 - a delinker
placeholder, NOT the real literal. Read the REAL bytes from survarium.exe:
- ctor bytes: `...c74040000000008b4dfcf30f1005c4b6a700f30f1141448b55fcf30f1005c4b6a700f30f114248...`
- both `movss xmm0,[disp32]` use disp `0xa7b6c4` (rva 0xa6b6c4) => value **1.0**
- BOTH stores reference the SAME constant slot 0xa7b6c4 (MSVC pooled the two
  identical `1.0f` literals into one rdata slot).
So the drafted init list `m_weapon(NULL), m_shooting_skill_coeff(1.0f),
m_aiming_speed_coeff(1.0f)` is correct.

Recipe used (read EXE movss operand):
```python
# parse PE, rva->file offset, ctor at 0x586920 len 75
# movss F3 0F 10 05 <disp32> at func+39 and func+55; disp - image_base(0x10000) -> rva -> f32
```
Both -> 1.0.

## Visibility
Header declares `public: explicit dispersion_calculator()`. Target mangled `QAE`
= public => matches, no change.

## Anchor
`use_dispersion_calculator()` in temp_include_all.cpp constructed `calc` and
called `get_dispersion()`, but with m_weapon==NULL get_dispersion returns early
and never reads the coeffs, so LTCG DSEs the coeff stores. Added
`example_callback( reinterpret_cast<pcstr>( &calc ) );` to observe the stores
(loop_performance / assembly_patterns LTCG-DSE entry).

## Commands run
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function "dispersion_calculator::dispersion_calculator" --list` -> single match 0x586920
- `pdb_fetch --target-index ... --rva 0x586920 --view target` / `--view structure`
- read survarium.exe movss constants (above) -> 1.0 / 1.0, shared slot
- `python3 scripts/rebuild.py` (no module arg)

## Results
- After `python3 scripts/rebuild.py` (full relink): report.json
  `??0dispersion_calculator@survarium@@QAE@XZ` = **100.0**.
- report-changes.json: 0 regressed, 0 improved (anchor edit changed nothing
  elsewhere; ctor was already 100% in the prior report and stayed 100%).
- Base asm (rva 0x453f60) is byte-identical to target (rva 0x586920): same two
  sub-ctor calls, `mov [eax+40h],0`, two `movss [.+44h]/[.+48h]` from one pooled
  1.0f slot, identical 2-statement structure (L22 `{` 0x44 bytes, L23 `}` 0x7).
- NOTE: the prior `use_dispersion_calculator()` (construct + `get_dispersion()`,
  no escape) already kept the stores observed enough to match 100%. The added
  `example_callback( &calc )` escape is belt-and-suspenders (the documented
  constant-ctor anchor pattern) and caused no regression.

Final: STATE[100%|DONE]. Carcass stripped (clean 100% keeps only the STATE line).
set_weapon / apply_aim_speed are NOT inlined into the ctor (separate out-of-line
`call`s); left as separate units, untouched.
