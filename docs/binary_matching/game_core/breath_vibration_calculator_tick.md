# breath_vibration_calculator — tick / initialize_logic / dynamic initializer

Unit: the 3 remaining STUBs of `survarium::breath_vibration_calculator`
(the ctor/setter/dtor are a prior unit, see `_accessors.md`).

- `tick(u32, float)`                         target RVA 0x5835f0  (QAE, public)
- `initialize_logic()`                       target RVA 0x5837e0  (AAE, private)
- `` `dynamic initializer for 's_enable_breath_vibration_cc'` `` target RVA 0x7cb2f0

Stack tip branched off: `match/game_core-legs_ik_drawer` (HEAD b8f23795).

## Commands run (verbatim)

```
nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function breath_vibration_calculator --list
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --rva 0x7cb2f0   # dynamic initializer
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --rva 0x5837e0   # initialize_logic
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --rva 0x5835f0   # tick
```

## Constants (from delinker masks + math_constants.h)
- epsilon_3 = 0.001f, pi_x2 = 6.2831855  (both referenced by name in tick's asm)

## Member offsets (breath_vibration_calculator, size 0x3C)
m_logic@0, m_user@0x14, m_params@0x18, m_last_time_in_ms@0x1c,
m_current_multiplier@0x20, m_target_multiplier@0x24, m_vertical_value@0x28,
m_horizontal_value@0x2c, m_character_multiplier@0x30, m_breath_holding_reserve@0x34,
m_is_breath_holded@0x38.

breath_holding_params (m_params points to it): max_breath_holding_time@0,
shortbreathing_repair_time@4, breath_holding_multiplier@8, shortbreathing_multiplier@0xc,
multiplier_increase_speed@0x10, multiplier_decrease_speed@0x14, horizontal_amplitude@0x18,
vertical_amplitude@0x1c, horizontal_peroid@0x20, vertical_peroid@0x24.

breath_state vtable (over fsm_state: dtor@0,init@4,exec@8,final@c,is_ready@10):
set_breath_holding_params@0x14, tick@0x18; get_multiplier reads m_multiplier@0x24.

## tick (0x5835f0) — analysis

```
ASSERT( UNKNOWN_EXPRESSION_T( m_user ) );                              // 76 (byte=0 + finalize_impl)
if ( current_time_in_ms < m_last_time_in_ms ) return;                 // 77 (cmp [+1ch]; jae; jmp end)
float const dt = (current_time_in_ms - m_last_time_in_ms)*0.001f*time_scale; // 78 (fild;fmul eps3;fmul ts)
m_last_time_in_ms = current_time_in_ms;                               // 80
m_logic.tick();                                                       // 81 (fsm::tick)
breath_state* current_state = (breath_state*)m_logic.current_state(); // 82 ([+10h]=m_current_state)
current_state->tick(dt);                                             // 84 (call [vtbl+18h])
m_target_multiplier = current_state->get_multiplier();              // 85/86 ([+24h]->temp->[+24h])
m_current_multiplier = m_current_multiplier > m_target_multiplier    // 89 (comiss; jbe)
    ? math::max(m_current_multiplier - m_params->multiplier_decrease_speed*dt, m_target_multiplier)
    : math::min(m_current_multiplier + m_params->multiplier_increase_speed*dt, m_target_multiplier);
float const current_phase = m_user->local_time(current_time_in_ms)*0.001f*pi_x2*time_scale; // 91
m_horizontal_value = sin(current_phase/m_params->horizontal_peroid)*horizontal_amplitude*m_character_multiplier*m_current_multiplier; // 92
m_vertical_value   = sin(current_phase/m_params->vertical_peroid)  *vertical_amplitude  *m_character_multiplier*m_current_multiplier; // 93
if (!s_enable_breath_vibration_value)                                // 94 (movzx byte; test; jne end)
    m_horizontal_value = m_vertical_value = 0.0f;                    // 95 (store @28h then @2ch)
```
m_user->local_time is the virtual at [vtbl+0x68] taking u32 returning u32
(call-site info: `u32 <unknown>(const u32) const`). math::max/min/sin are real
out-of-line calls in the target.

## dynamic initializer (0x7cb2f0)
Static construction of `s_enable_breath_vibration_cc`:
```
static bool s_enable_breath_vibration_value = true;
static console_commands::cc_bool s_enable_breath_vibration_cc(
    "breath_vibration_enable", s_enable_breath_vibration_value, true,
    console_commands::command_type_user_specific );
```
asm: push 1 (serializable), value ref, name; command_type in eax=1 (user_specific),
execution_filter default in ecx=0. Mirrors dispersion_calculator's
s_dispersion_enabled_cc, whose dynamic initializer also scores objdiff None
(file-static cc_bool init/atexit thunks are LTCG/ICF folded).

## initialize_logic (0x5837e0) — analysis (LARGE), DEFERRED INPROGRESS

This is the canonical home for the reconstructed body + next steps (the cpp keeps
only a terse pointer here, per the lean-comment policy).

Builds the fsm: news three breath_state subclasses (normal/holding/shortbreathing)
via VOSTOK_NEW_IMPL(g_allocator, ...) (operator new + breath_state::breath_state each,
sets vtable + zeroes a float member), add_state x3, then **five** add_transition( from,
to, pred ) calls (verified against the 0x5837e0 asm: add_state @0x19c/0x1ab/0x1ba;
add_transition @0x234/0x2b6/0x328/0x368/0x3a8):

```
add_transition( normal,         holding,        boost::bind( &hold_button_state_equals_to, this, true  ) )  // cmf1, 0x234
add_transition( holding,        normal,         boost::bind( &hold_button_state_equals_to, this, false ) )  // cmf1, 0x2b6
add_transition( holding,        shortbreathing, boost::bind( &insufficient_breath,         this        ) )  // cmf0, 0x328
add_transition( shortbreathing, normal,         true_predicate )                                            // bool(*)(void), 0x368
add_transition( shortbreathing, holding,        true_predicate )                                            // bool(*)(void), 0x3a8
```

Each predicate is materialized through a `behaviour_cook_params` temp +
`boost::function0<bool>::assign_to<...>` then passed to `fsm::add_transition`, with a
`boost::function1<void,char const*>::clear` after each. ~0x3bc bytes.

NEXT STEP to match (concrete):
1. Switch this cpp's `#include` from `breath_state.h` to
   `breath_holding_states.{h,inline}` — the subclass ctors/vtables live there, and the
   duplicate `breath_state` definition conflict must be resolved first.
2. Make `initialize_logic` `private` in the header (target mangles AAE; currently public
   under `public:` -> scores None until moved).
3. Write the body above; anchor transitively (the ctor already calls initialize_logic).

Requires: breath_holding_states.{h,inline}, behaviour_cook_params, the boost::bind /
function0<bool> machinery, true_predicate.

## Iterations

### Rebuild 1 (full `python3 scripts/rebuild.py`, no module arg)

Source changes:
- `breath_vibration_calculator.cpp`: added `#include base_player.h` + `console_command.h`,
  the `s_enable_breath_vibration_value` bool + `s_enable_breath_vibration_cc` cc_bool static
  (= the dynamic initializer), and the full `tick` body. Marked `initialize_logic` INPROGRESS.
- `temp_include_all.cpp`: added `calc.tick( 0, 0.0f );` to the existing
  `use_game_core_breath_vibration_calculator` anchor so tick is reachable.

report.json:
- `?tick@breath_vibration_calculator@...QAEXIM@Z` -> **94.22857%**
- `` `dynamic initializer for 's_enable_breath_vibration_cc'` `` -> **None** (and the atexit
  destructor None). NO standalone symbol in `binaries/rich/base/index.jsonl` - the file-static
  cc_bool init/atexit thunks are LTCG/ICF folded, identical to dispersion_calculator's
  `s_dispersion_enabled_cc` (also None) and `game_core_initialize`. Body is byte-correct
  (mirrors the proven dispersion pattern); the gap is purely linker ICF visibility.

report-changes: tick 0->94.23. The 16 regressed / remaining improved rows are symmetric
ICF-fold-representative churn (thunks, ~dtors, empty_stub, fsm::tick 0->10, buffer_string
operator= 0->68) - none touch this unit's source (git diff only breath cpp + temp_include_all).

### tick diff (base 0x455860 vs target 0x5835f0, no rebuild)

Statements, control flow, member offsets (m_last@1c, m_current_mult@20, m_target_mult@24,
m_vertical@28, m_horizontal@2c, m_char_mult@30; m_params fields decrease@14/increase@10/
h_amp@18/v_amp@1c/h_per@20/v_per@24), the [vtbl+18h] tick vcall, [vtbl+68h] local_time vcall,
and all FPU/sse math line up 1:1. The ONLY residual: target frames `sub esp,38h` vs base
`30h` (8 bytes / 2 extra temp slots), so the saved `this` slot is [ebp-24h] (target) vs
[ebp-1Ch] (base) and the two m_user loads at the local_time vcall site swap which register
holds the vtable. Pure /Od frame-slot register-allocation churn - no missing local, brace,
ASSERT or statement. Not source-steerable => tick PARTIAL 94.23%.

### initialize_logic - LEFT INPROGRESS (not attempted this unit)
Large (~0x3bc), entangled: needs the breath_state SUBCLASSES (breath_state_normal/holding/
shortbreathing) from breath_holding_states.{h,inline} - which redefine `breath_state` and
conflict with the breath_state.h this cpp currently includes - plus the boost::bind /
function0<bool> machinery and VOSTOK_NEW_IMPL allocations, and the header must move
initialize_logic to private (target AAE). Full reconstructed body is in the cpp's STATE
comment. Deferred to avoid risking the build / burning the rebuild budget on a hard member.
