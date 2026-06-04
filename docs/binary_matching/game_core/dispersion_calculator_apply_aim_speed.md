# game_core::dispersion_calculator::apply_aim_speed

`void survarium::dispersion_calculator::apply_aim_speed()` - target rva 0x5867a0
(carcass/base-build addrs 0x5967a9..; source lines 80-84). Mangled
`?apply_aim_speed@dispersion_calculator@survarium@@AAEXXZ` -> `AAE` = **private**,
`__thiscall`. PDB local: `const float speed_of_aiming`.

## Member layout (from headers, confirmed by offsets)
- dispersion_calculator: m_weapon_calculator @+0x00, m_character_calculator @+0x20,
  m_weapon @+0x40, m_shooting_skill_coeff @+0x44, m_aiming_speed_coeff @+0x48.
- weapon_core::get_dispersion_params() -> m_dispersion_params @+0x3e4 (inline).
  weapon_dispersion_params::speed_of_aiming @+0x0c -> 0x3e4+0x0c = +0x3F0.
- character_dispersion_calculator::m_aiming_speed @+0x14; within dispersion_calculator
  that is 0x20+0x14 = +0x34.
- weapon_dispersion_calculator::set_aiming_speed: out-of-line declared, real call.

## Target asm (pdb_fetch --view target, rva 0x5867a0)
```
0x00 push ebp; mov ebp,esp; sub esp,0Ch; mov [ebp-8],ecx          ; this @ [ebp-8]
0x09 mov eax,[ebp-8]; cmp dword[eax+40h],0; je .1                 ; if (m_weapon)
0x12 mov ecx,[ebp-8]; mov edx,[ecx+40h]; mov eax,[ebp-8]
0x1b movss xmm0,[edx+3F0h]                                        ; m_weapon->...speed_of_aiming
0x23 mulss xmm0,[eax+48h]                                         ; * m_aiming_speed_coeff
0x28 movss [ebp-0Ch],xmm0; jmp .2                                 ; ternary temp
.1:
0x2f movss xmm0,[offset]; movss [ebp-0Ch],xmm0                    ; else 0.0f
.2:
0x3c movss xmm0,[ebp-0Ch]; movss [ebp-4],xmm0                     ; speed_of_aiming = temp
0x46 mov ecx,[ebp-8]; movss xmm0,[ebp-4]; movss [ecx+34h],xmm0    ; m_character_calculator.set_aiming_speed (inlined)
0x53 push ecx; fld [ebp-4]; fstp [esp]; mov ecx,[ebp-8]
0x5d call weapon_dispersion_calculator::set_aiming_speed          ; m_weapon_calculator.set_aiming_speed
0x62 mov esp,ebp; pop ebp; ret
```
Statements (from index.jsonl): L80 sz9, L81 sz61(0x3d), L82 sz13(0xd), L83 sz15(0xf),
L84 sz4 - matches carcass deltas +0x3d/+0x0d/+0x0f exactly.

## The else-branch constant
The disassembler shows `movss xmm0,[offset]`. The instruction bytes at func+0x2f are
`f3 0f 10 05 44 40 97 00` = `movss xmm0,[0x00974044]` (absolute VA). RVA = 0x974044 -
0x10000(image_base) = 0x964044, in .rdata, raw dword = 0x00000000 = **0.0f**. So the
no-weapon default is `0.0f`. (Read directly out of survarium.exe with a small PE parser;
the function prologue at file offset for rva 0x5867a0 matched `55 8bec 83ec0c 894df8 ...`,
confirming rich-query rva is a true RVA.)

## Reconstructed source
```cpp
void dispersion_calculator::apply_aim_speed( )
{
    float const speed_of_aiming = m_weapon
        ? m_weapon->get_dispersion_params( ).speed_of_aiming * m_aiming_speed_coeff
        : 0.0f;
    m_character_calculator.set_aiming_speed( speed_of_aiming );
    m_weapon_calculator.set_aiming_speed( speed_of_aiming );
}
```

## Shape decisions made before build
- Header: moved `apply_aim_speed` from public -> private (mangled AAE).
- character_dispersion_calculator::set_aiming_speed given a real body
  `{ m_aiming_speed = aiming_speed; }` so L82 inlines to `[this+0x34]` store (was empty stub).
- weapon_dispersion_calculator::set_aiming_speed left as out-of-line call (target call at 0x5d).
- Link-reachable already via set_weapon (matched, anchored) + set_aiming_speed_coeff; no new anchor.

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep apply_aim_speed
  -> 0x5867a0
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x5867a0 --view target
- nix develop --command pdb_fetch ... --view structure  -> 5 statements, 0x66 bytes
- grep apply_aim_speed binaries/rich/target/index.jsonl  -> mangled AAE, statements, locals
- python3 PE parser on survarium.exe -> movss operand 0x00974044, .rdata, value 0.0f
- nix develop --command python3 scripts/rebuild.py   (full relink)

## Iterations
1. INPUT: body above; header apply_aim_speed -> private; character set_aiming_speed real body.
   BUILD (full relink): report.json fuzzy_match_percent = **100.0%** for
   ?apply_aim_speed@dispersion_calculator@survarium@@AAEXXZ (report-changes: 0.00 -> 100.00).
   The whole dispersion/weapon cluster unchanged or improved (ctor/set_weapon/set_aiming_speed_coeff/
   set_shooting_skill_coeff/weapon_dispersion_calculator setters all still 100%; get_dispersion still
   87.49%). 55 regressed / 56 improved in report-changes are unrelated delinker non-determinism
   (scalar deleting destructors, Scaleform, boost storage2, btXxx) - mirrored churn, NONE in the
   dispersion/weapon/character cluster. Matched first try.

## Outcome
STATE[100%|DONE]. Side edits required:
- dispersion_calculator.h: apply_aim_speed moved public -> private (mangled AAE).
- character_dispersion_calculator.h: set_aiming_speed given a real body
  `{ m_aiming_speed = aiming_speed; }` (was empty stub) so L82 inlines to the `[this+0x34]` store.
No new temp_include_all anchor (already reachable via the matched set_weapon).
