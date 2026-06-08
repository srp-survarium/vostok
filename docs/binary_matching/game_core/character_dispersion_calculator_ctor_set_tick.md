# game_core::character_dispersion_calculator — ctor + set_character_dispersion_params + tick

Same-class STUB cluster (3 functions), one rebuild. Branch
`match/game_core-character_dispersion_calculator` off `build/ninja-stall-watchdog`
(HEAD 170b70b8). The other two functions in the file (get_target_koef,
get_broken_hands_penalty) are NOT in this unit (pre-existing INPROGRESS/PARTIAL).

Header `sources/vostok/game_core/character_dispersion_calculator.h`; member layout:
m_params@0x0, m_target_value@0x4, m_current_value@0x8, m_value@0xc,
m_value_smoothing_speed@0x10, m_aiming_speed@0x14, m_current_time@0x18 (u32),
m_jumped@0x1c (bool). Class derives `core::noncopyable`. Size 0x20.

## Commands run (verbatim)
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function "character_dispersion_calculator::character_dispersion_calculator" --list` -> 0x586010
- `... set_character_dispersion_params --list` -> 0x585e00
- `... tick --list` -> 0x586090
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x586010 --view target/structure`
- `... --rva 0x585e00 --view target/structure/info`
- `... --rva 0x586090 --view target/structure/info/callees`
- EXE-constant reader (PE parse, movss disp32 -> rdata f32) over the ctor bytes
- `python3 scripts/rebuild.py` (NO module arg, full EXE relink) — 701s, watchdog did NOT engage
- `pdb_fetch --base-index binaries/rich/base/index.jsonl --rva 0x454000 --view base` (base tick disasm for the residual)

## ctor (0x586010) -> 100% DONE
asm: prologue, then `mov eax,[ebp-4]; call ...finalize_impl` (the folded
`core::noncopyable()` base ctor — NO byte-local, so NOT an ASSERT; the rva-0x3f210
empty-fn fold delinker-misnamed `finalize_impl`, per assembly_patterns), then a
member-init store sequence in declaration order.

Constants read from survarium.exe (NOT the delinker placeholders — "offset"/
"clear_value" are masks; the real movss disp32 -> rdata f32):
| member                  | off  | value |
|-------------------------|------|-------|
| m_params                | 0x00 | NULL (mov dword 0) |
| m_target_value          | 0x04 | 0.0f  |
| m_current_value         | 0x08 | 0.0f  |
| m_value                 | 0x0c | 0.0f  |
| m_value_smoothing_speed | 0x10 | 5.0f  (0x40a00000) |
| m_aiming_speed          | 0x14 | 1.0f  (the "clear_value" placeholder really reads 1.0) |
| m_current_time          | 0x18 | 0     (mov dword 0) |
| m_jumped                | 0x1c | false (mov byte 0) |

Source = a member-init list in declaration order. Hit 100% in one rebuild (the
anchor `use_character_dispersion_calculator` escapes `&calc` through
`example_callback` so the constant stores are observed — the constant-ctor DSE
pattern). Carcass deleted (clean 100%).

## set_character_dispersion_params (0x585e00) -> 100% DONE
7 statements. `m_params = character_params;` then `if ( m_params )` (braced — the
carcass `<0>` markers bracket the block) storing the SAME value
(`m_params->idle_multiplier`, the +0x0 member of character_dispersion_params) into
m_target_value(+4), m_current_value(+8), m_value(+0xc). All three `fld [ecx]` read
`[m_params+0]`. 100% first try. Carcass deleted.

## tick (0x586090) -> 99.67% PARTIAL (frame-slot residual, DONE-quality)
16 statements, full real function (NOT a getter-keeper stub). Decode:
- L50 `ASSERT( UNKNOWN_EXPRESSION_T( m_params ) )` — byte-local + lea + call empty_stub.
- L51-53 `if ( m_current_time == 0 ) { m_current_time = current_time_in_ms; return; }`
  (`cmp [+18h],0; jne` — braced block, carcass `<0><1>`).
- L56-57 `if ( m_current_time >= current_time_in_ms ) return;`
  (`cmp [+18h],[ebp+1Ch]; jb continue` — brace-less, single return).
- L59 `float const dt = ( current_time_in_ms - m_current_time ) / 1000.0f;`
  — the u32 difference is stored low + zero high then `fild qword` (MSVC's u32->float
  widening), `fdiv [__real@447a0000]` (=1000.0). dt is the one PDB-recorded local.
- L60 `m_current_time = current_time_in_ms;`
- L63 `m_target_value = get_target_koef( character_state, is_moving, is_aiming ) *
  get_broken_hands_penalty( broken_hands_count, using_double_handed_weapon );`
  (target_koef result `fstp [tmp]`, then `fmul [tmp]` after the penalty call — a
  multiply, NOT the STUB's `+`). Arg push order confirms each callee's signature.
- L69 `m_current_value = math::max( m_target_value, m_current_value - m_aiming_speed * dt );`
- L73 `if ( m_value > m_current_value )` (`comiss; jbe else`)
- L75   `m_value = math::max( m_current_value, m_value - m_value_smoothing_speed * dt );`
- L78 `else if ( m_current_value > m_value )` (`comiss; jbe end`)
- L80   `m_value = math::min( m_current_value, m_value + m_value_smoothing_speed * dt );`
- math::max/min stay out-of-line (const float, const float overloads).

Residual: base `--view base` (rva 0x454000) is byte-for-byte IDENTICAL to the
target in EVERY instruction, member offset, call, and constant (the lone normalized
diff is the ICF-fold misname of the ASSERT empty-stub call — `finalize_impl` in
target vs `call_constructor_helper<..>::call` in base, same 0x3f210 fold). The SOLE
real diff:
- target: `sub esp, 1Ch`, `this` at `[ebp-10h]`, slots -10h/-14h/-18h/-1Ch
- base:   `sub esp, 18h`, `this` at `[ebp-0Ch]`, slots -0Ch/-10h/-14h/-18h

i.e. the target build reserved one extra unused 4-byte frame slot, shifting every
`[ebp-N]` by 4. Pure /Od stack-allocation noise — NOT a missing local/brace/ASSERT/
statement (the dt local maps, the 16-statement structure matches the carcass 1:1).
Same residual class as `breath_vibration_calculator::tick` (94%, frame-slot churn).
Stopped at PARTIAL; carcass kept (non-100%).

## Anchor (temp_include_all.cpp, use_character_dispersion_calculator)
Pre-existing anchor constructed `calc`, called set_character_dispersion_params(NULL)
+ tick(...). Added `example_callback( reinterpret_cast<pcstr>( &calc ) )` to escape
the instance so the ctor's constant stores are observed (constant-ctor DSE pattern).

## Result
ctor 100% DONE, set_character_dispersion_params 100% DONE, tick 99.67% PARTIAL.
report-changes: 0 regressed, 3 improved (30.32->100 / 22.86->100 / 25.26->99.67),
0 removed, 0 added. One rebuild, 701s, watchdog not engaged.
