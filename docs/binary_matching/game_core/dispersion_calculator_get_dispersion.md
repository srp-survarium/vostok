# game_core::dispersion_calculator::get_dispersion

`float survarium::dispersion_calculator::get_dispersion() const` — target rva 0x586970
(carcass/base-build addr was 0x596970, source lines 27-40).

## Target asm (pdb_fetch --view target, rva 0x586970)

Frame: `this` @ [ebp-3Ch]. Members: m_weapon @+0x40, m_shooting_skill_coeff @+0x44,
m_aiming_speed_coeff @+0x48, m_weapon_calculator @+0x00, m_character_calculator @+0x20.

Logic trace:
- [ebp-18h]=0 (EH guard for a temp). `if (m_weapon==0) goto .1`.
- construct temp weapon_ammunition_ptr from &m_weapon->m_ammunition (offset 0x404)
  via intrusive_ptr::set(const&); `if (temp.get()==0) goto .1`  (= !m_weapon->ammunition()).
- `if (s_dispersion_enabled_value==0) goto .1`.
- fall-through: [ebp-40h]=0 (enabled); .1: [ebp-40h]=1 (disabled); store -> [ebp-5].
- destroy temp ammunition ptr (intrusive_ptr::dec).
- `.3: if ([ebp-5] != 0) { fldz; ret 0.0f }`   = the `return 0.0f` early-out.
- weapon_params = &m_weapon->m_dispersion_params (offset 0x3e4) -> [ebp-4].
- `m_weapon->is_aimed()` (real call, this in eax) ? weapon_params.aim_multiplier (+0x08)
  : weapon_params.from_the_hip_multiplier (+0x04)  -> [ebp-44h].
- middle block 0xcf-0x131: construct ANOTHER temp ammunition ptr (this one inlines
  interlocked_increment + finalize_impl) -> raw ptr in [ebp-14h].
- ammunition->dispersion() = [ptr+0x11c] (weapon_ammunition::m_dispersion) -> [ebp-30h].
- m_character_calculator.get_value() = [this+0x2C] (m_character_calculator.m_value @+0x0c) -> [ebp-34h].
- fld weapon_params.base_dispersion (+0x00); fmul [ebp-30h] (dispersion); fmul [ebp-44h] (aim/hip)
  -> store partial product [ebp-4Ch].
- call m_weapon_calculator.get_value() (this+0, st0); fadd [ebp-34h] (character value);
  fmul [this+0x44] (m_shooting_skill_coeff); fadd [ebp-4Ch] (partial product) -> [ebp-10h].
- destroy temp ammunition ptr; fld [ebp-10h]; ret.

Reconstructed source:
```
if ( !( m_weapon && m_weapon->ammunition() && s_dispersion_enabled_value ) )
    return 0.0f;
weapon_dispersion_params const& weapon_params = m_weapon->get_dispersion_params();
float aim_or_hip_multiplier = m_weapon->is_aimed() ? weapon_params.aim_multiplier
                                                   : weapon_params.from_the_hip_multiplier;
return weapon_params.base_dispersion * m_weapon->ammunition()->dispersion() * aim_or_hip_multiplier
     + ( m_weapon_calculator.get_value() + m_character_calculator.get_value() ) * m_shooting_skill_coeff;
```
PDB local: `weapon_dispersion_params const& weapon_params` (confirms the ref binding).

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function get_dispersion --list
  -> 0x586970 dispersion_calculator, 0x594760 weapon_core
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_dispersion --rva 0x586970 --view target
- nix develop --command pdb_fetch ... --view callees   -> intrusive_ptr::set/dec, weapon_core::is_aimed,
  interlocked_increment, fixed_size_allocator::finalize_impl, weapon_dispersion_calculator::get_value
- nix develop --command pdb_fetch ... --view info       -> local: weapon_dispersion_params const& weapon_params
- nix develop --command pdb_rich_query ... --function dispersion_enabled --list
  -> dynamic initializer 0x7cafa0 / atexit 0x7e0ca0 for s_dispersion_enabled_cc (compiler-generated, out of scope)

## Iterations
1. INPUT: reconstructed body (early-out `if (!(m_weapon && m_weapon->ammunition() &&
   s_dispersion_enabled_value)) return 0.0f;` then weapon_params ref, is_aimed?aim:hip,
   final expr `base*disp*mult + (weapon.get_value()+character.get_value())*shooting_skill`).
   Added static `s_dispersion_enabled_value` + `cc_bool s_dispersion_enabled_cc`, included
   weapon_core.h / weapon_ammunition.h / console_command.h, made
   `character_dispersion_calculator::get_value()` return `m_value` (was empty stub).
   Anchored via new `use_dispersion_calculator()` in temp_include_all.cpp.
   BUILD: 87.49% (report.json fuzzy_match_percent).  Regressions: 0 in game_core / dispersion /
   weapon cluster. The 62 report-changes "regressed" entries are all unrelated delinker
   non-determinism noise (scalar deleting destructors, thunks, buffer_string::operator=,
   medkit::on_player_model_added, btSphereShape etc.) - all 100->0 or x->0 churn, mirrored
   by ~55 "improved" of the same kind.
   DIFF (--view diff, objdiff fuzzy 81.29%): body matches instruction-for-instruction modulo:
     - frame layout: base `sub esp,58h` vs target `4Ch`; every [ebp-XX] slot shifted (LTCG).
     - `m_weapon->is_aimed()`: target real `call`, base inlines `mov al,[edx+488h]` (m_aimed).
     - the `m_weapon->ammunition()` safe-bool term: base lowers via an extra bool slot
       ([ebp-24h]) vs target's direct `cmp/sete`. LTCG materialization, not steerable.
   No source change can close these (all whole-program/linker decisions). Stopped here.

## Outcome
STATE[87.49%|PARTIAL]: body, member offsets, expression order and control flow all match.
Residual diff is LTCG only: (1) larger base frame -> [ebp-XX] slot reassignment, (2) is_aimed()
inlined in base / called in target, (3) safe-bool conversion of the by-value ammunition_ptr
test materialized through an extra bool slot in base. Regressions caused: none (game_core
cluster unchanged; the report-changes churn is delinker non-determinism on unrelated symbols).
Inlining: is_aimed() and the size()/safe-bool COMDAT lowering differ; no cluster members
needed - get_dispersion is one standalone unit. Side edits: character_dispersion_calculator.h
get_value() now returns m_value (required so [this+0x2C] read compiles); temp_include_all.cpp
gained use_dispersion_calculator anchor; dispersion_calculator.cpp gained the
s_dispersion_enabled_value/cc statics it reads.
