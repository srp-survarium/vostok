# game_core::dispersion_calculator::set_weapon(weapon_core*)

Target rva: `0x5868c0`
Mangled: `?set_weapon@dispersion_calculator@survarium@@QAEXPAVweapon_core@2@@Z`
(`QAE` = public, __thiscall) -> matches header `public: void set_weapon(weapon_core*)`.
Source: `sources/vostok/game_core/sources/dispersion_calculator.cpp` (~line 94)
Unit: set_weapon only. apply_aim_speed / set_one_shoot_/set_reload_dispersion_amount
are out-of-line `call`s (NOT inlined here) - separate units, untouched.

## Target asm (`pdb_fetch --rva 0x5868c0 --view target`)
```
0x00: push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx        ; L87 prologue
0x07: mov eax,[ebp-4]; mov ecx,[ebp+8]; mov [eax+40h],ecx     ; L88 m_weapon = weapon
0x10: mov edx,[ebp-4]; cmp dword[edx+40h],0; je .1            ; L89 if ( m_weapon )
0x19: mov eax,[ebp-4]; mov ecx,[eax+40h]; push ecx;
      fld [ecx+3F4h]; fstp [esp]; mov ecx,[ebp-4];
      call weapon_dispersion_calculator::set_one_shoot_dispersion_amount  ; L91
0x31: mov edx,[ebp-4]; mov eax,[edx+40h]; push ecx;
      fld [eax+3F8h]; fstp [esp]; mov ecx,[ebp-4];
      call weapon_dispersion_calculator::set_reload_dispersion_amount     ; L92
.1:
0x49: mov ecx,[ebp-4]; call dispersion_calculator::apply_aim_speed        ; L94
0x51: mov esp,ebp; pop ebp; ret 4                                         ; L95 }
```
Structure (`--view structure`): 7 statements, 0x57 bytes.
L87(0x7) L88(0x9) L89(0x9) L91(0x18) L92(0x18) L94(0x8) L95(0x6).

## Decode
- `[this+40h]` = m_weapon. `mov [eax+40h],ecx` with ecx=[ebp+8] = `m_weapon = weapon`.
- `cmp [edx+40h],0; je` = `if ( m_weapon )` - braced block (two `<0>` carcass
  markers bracket L91/L92; the `{`/`}` are L90/L93, no addressed line).
- callees: set_one_shoot_dispersion_amount(const float), set_reload_dispersion_amount(const float).
  Both arg from `m_weapon->[3F4h]` / `[3F8h]`.
- weapon_core::m_dispersion_params @ 0x3e4 (weapon_dispersion_params, 0x20).
  0x3e4+0x10=0x3F4 = one_shoot_dispersion_amount; 0x3e4+0x14=0x3F8 = reload_dispersion_amount.
  => arg is `m_weapon->get_dispersion_params().one_shoot_dispersion_amount` / `.reload_dispersion_amount`.
- L94 `apply_aim_speed()`.

## Source written
```cpp
m_weapon = weapon;
if ( m_weapon )
{
    m_weapon_calculator.set_one_shoot_dispersion_amount( m_weapon->get_dispersion_params( ).one_shoot_dispersion_amount );
    m_weapon_calculator.set_reload_dispersion_amount( m_weapon->get_dispersion_params( ).reload_dispersion_amount );
}
apply_aim_speed( );
```

## Anchor
`use_dispersion_calculator()` in temp_include_all.cpp: added `calc.set_weapon( NULL );`
so the linker keeps set_weapon alive. Body has observable stores/calls so referencing
is enough.

## Commands run (verbatim, in order)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function "dispersion_calculator::set_weapon" --list  -> 0x5868c0
- pdb_fetch --target-index ... --rva 0x5868c0 --view target / structure / callees / info
- grep weapon_core.h / weapon_dispersion_params.h / weapon_dispersion_calculator.h for offsets/setters
- python3 scripts/rebuild.py        # NO module arg (full EXE relink)
- pdb_fetch ... --view diff ...

## Iterations
1. INPUT: body as written above; anchor `calc.set_weapon( NULL )` added to
   use_dispersion_calculator().
   BUILD: report.json fuzzy = **100.0**  (report-changes: 0 regressed, 1 improved -
   set_weapon 0.00% -> 100.00%).
   DIFF: `--view diff` header reads 97.16% but every instruction row is equal except
   one `~` at 0x4c: `call 0000009Dh -> call 00000161h` - that is purely the relative
   call-target displacement to apply_aim_speed (a base-vs-target relocation address,
   not a body diff). report.json (the number of record) is 100.0. Structure identical:
   7 statements, same `<0x18>` deltas on L91/L92, braced `if`, `ret 4`.

## Outcome
STATE[100%|DONE]. First-try byte match. Carcass deleted (clean 100% keeps only the
STATE line). Regressions caused: none. Inlining: none - apply_aim_speed,
set_one_shoot_dispersion_amount, set_reload_dispersion_amount are all out-of-line
`call`s and remain separate units (untouched). get_dispersion_params() is the inline
accessor returning `&m_dispersion_params` (this+0x3e4); the compiler folds the field
offsets +0x10/+0x14 into the constants 0x3F4/0x3F8.
