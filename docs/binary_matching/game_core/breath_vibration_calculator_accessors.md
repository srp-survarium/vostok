# breath_vibration_calculator — ctor / set_breath_holding_params / dtor

Unit of work: 3 same-class members of `survarium::breath_vibration_calculator`,
matched together as one branch/commit/PR.

- ctor   `breath_vibration_calculator()`                       target RVA 0x583ba0
- setter `set_breath_holding_params(breath_holding_params const*)` target RVA 0x5834d0
- dtor   `~breath_vibration_calculator()`                      target RVA 0x583590

Explicitly NOT in scope (separate units): `initialize_logic` (0x5837e0),
`tick` (0x5835f0), the `dynamic initializer for s_enable_breath_vibration_cc` stub.

## Commands run (verbatim)

```
nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function breath_vibration_calculator --list
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --rva 0x583ba0   # ctor
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --rva 0x583590   # dtor
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --rva 0x5834d0   # setter
```

COFF constant/reloc dump (zero rebuilds) on
`binaries/objdiff/target/vostok/game_core/sources/breath_vibration_calculator.cpp.obj`:
- .rdata floats: 0x0=0.0 (clear_value/`___`), 0x4/0x8=0.001 (epsilon_3),
  0xc=6.2831855 (pi_x2). 0x10/0x14/0x24/0x28 = 0.0 (used by tick/initialize_logic only).
- All ctor float member inits are `0.0f` (masked `[clear_value]` and `[offset]` both
  resolve to 0.0 constants).

## Layouts (from headers + structure/target)

`breath_vibration_calculator` (size 0x3C):
- m_logic (ai::fsm) @0x00 (size 0x14), m_user @0x14, m_params @0x18,
  m_last_time_in_ms @0x1c, m_current_multiplier @0x20, m_target_multiplier @0x24,
  m_vertical_value @0x28, m_horizontal_value @0x2c, m_character_multiplier @0x30,
  m_breath_holding_reserve @0x34, m_is_breath_holded @0x38.

`ai::fsm` (size 0x14): m_states (fsm_state_list) @0, m_current_state @0x10.
`fsm_state` (size 0x18): vtable @0, next @0x4, transitions @0x8.
`breath_state` (size 0x28): fsm_state @0, core::noncopyable @0x19,
  m_breath_holding_reserve (float&) @0x1c, m_params @0x20, m_multiplier @0x24.
  vtable adds set_breath_holding_params @+0x14, tick @+0x18.
`breath_holding_params` (size 0x28): max_breath_holding_time @0.

## Disassembly analysis

### ctor (0x583ba0)
Implicit member construction of m_logic (allocator<char>::allocator + ai::fsm::fsm),
then member-init-list of the scalar members in declaration order:
m_user=0, m_params=0, m_last_time_in_ms=0, then six 0.0f floats, then
m_is_breath_holded=false; finally `initialize_logic()` (the only line with a
carcass breakpoint, source line 29).  => constructor initializer list, NOT body
assignments.

### dtor (0x583590)
`while ((state = m_logic.pop_state()) != 0) DELETE(state);` then implicit
ai::fsm destructor (two trailing finalize_impl calls). DELETE uses
`?g_allocator@survarium@@...` => VOSTOK_DELETE_IMPL(g_allocator, state).
Carcass source lines 47 (while), 48 (delete), 49 (close).

### setter (0x5834d0)
```
m_params = params;                                                  // 34
for (fsm_state* it = m_logic.states().front(); it; it = it->next)   // 35
    ((breath_state*)it)->set_breath_holding_params(params);         // 36 (virtual @ vtbl+0x14)
if (m_params)                                                       // 37
{
    m_breath_holding_reserve = m_params->max_breath_holding_time;   // 38 (fld [m_params+0]; fstp [this+0x34])
    m_logic.set_initial_state(m_logic.states().front());           // 39
    m_target_multiplier =                                          // 40
        ((breath_state*)m_logic.current_state())->get_multiplier();// reads [+0x24], stores [this+0x24]
    m_current_multiplier = m_target_multiplier;                    // 41 (fld [this+0x24]; fstp [this+0x20])
}
```
states()/front() are emitted as calls at /Od; current_state()/get_multiplier()
appear inlined to direct field reads in the target.

## Iterations

### Rebuild 1 (full `python3 scripts/rebuild.py`, no module arg)

New/changed source:
- `sources/vostok/game_core/breath_state.h` (NEW): minimal `breath_state` class
  matching structure/target layout (size 0x28), so the setter's
  `static_cast<breath_state*>` reaches the virtual `set_breath_holding_params` at
  vtable slot +0x14 and the `m_multiplier` field at +0x24.
- `breath_vibration_calculator.cpp`: ctor (member-init-list), setter, dtor as analysed.
- `temp_include_all.cpp`: include breath headers + `use_game_core_breath_vibration_calculator`
  (construct calc, call setter, escape `&calc` through `example_callback`) + call from `IncludeAll`.

report.json (`binaries/objdiff/report.json`):
- `??0breath_vibration_calculator@survarium@@QAE@XZ`  -> **100.0%**  (ctor)
- `??1breath_vibration_calculator@survarium@@QAE@XZ`  -> **100.0%**  (dtor)
- `?set_breath_holding_params@breath_vibration_calculator@...` -> **76.80%** (setter)

report-changes.json: all three breath functions are in `improved` (0 -> 100/76.8).
The 68 `regressed` entries (sound_target_selector, btSoftRigidDynamicsWorld, medkit::
on_player_model_added, btHashMap, float3::float3, Scaleform, ... scalar deleting dtors)
are unrelated to my git diff (which only touches breath_vibration_calculator.cpp +
temp_include_all.cpp + new breath_state.h) — stale-baseline artifacts per
loop_performance.md (prior report.json was generated on a different branch's tree).

### Setter diff (`pdb_fetch --view diff --rva 0x5834d0`, no rebuild)

`24/92 instructions equal (26.1%)` per the diff header (report.json fuzzy = 76.80%).
The ONLY inline-vs-call divergence is `fsm::states()` and `intrusive_list::front()`
(x2): the TARGET emits real out-of-line CALLs to them - delinker misnames them
`...finalize_impl` / `boost::_bi::list3<...>::operator[]` at target 0x15/0x1a and
0x66/0x6b - while our /Od+/GL base INLINES them to a direct `mov eax,[edx+8]` read.

CORRECTION (reviewer, 2026-06-01): `fsm::current_state()` and
`breath_state::get_multiplier()` are NOT a target-call divergence - the
`pdb_fetch --view target` asm shows them INLINED on the target too
(`mov eax,[edx+10h]` reads m_current_state @0x10 directly; `movss xmm0,[eax+24h]`
reads m_multiplier @0x24 directly), and the base inlines them identically. The
earlier "the target emits real CALLs to ... current_state()/get_multiplier()"
wording was a divergence misattribution and has been removed. The residual in that
tail (target 0x79-0xb1 vs base 0x6b-0x97) is the cascading EXTRA-temp/slot
materialization that follows from the `sub esp,1Ch` vs `14h` frame the two real
`states()/front()` calls forced (the target bounces the inlined value through
`[ebp-0Ch]->[ebp-8]->[ebp-10h]` extra slots), not a separate call.

Consequence: the member set, control flow, virtual dispatch (`[vtbl+0x14]`),
`m_breath_holding_reserve = m_params->max_breath_holding_time` (`fld [eax]; fstp
[+34h]`), the `set_initial_state` call, and the `m_target_multiplier`/
`m_current_multiplier` stores appear to line up. BUT the control structure is NOT
verified (see the OPEN note below) - the post-loop stores may be an early-return guard,
not a braced `if`. => setter **INPROGRESS** at 76.80%; ctor/dtor DONE 100%.

### OPEN (INPROGRESS) - control structure not verified vs the target
The FUNCTION BODY structure shows the post-loop stores ('38'-'41') as four plain
function-scope statements with NO block-open marker, yet this source wraps them in
`if ( m_params ) { ... }`. That `{` would show as a `<n>` (no-address) line in the
structure - it is ABSENT - so the braces are likely wrong; the target is probably an
early `return` guard (`if ( !m_params ) return;`, no braces). NOT verified exact;
restructure + re-diff on a faster machine. (Lesson: compare the base vs target STRUCTURE,
not just the %; see MATCHING.md "Match the base/target structure".)

Secondary residual (only after the bracing is settled): `fsm::states()` +
`intrusive_list::front()` are out-of-line in the TARGET (`pdb_rich_query` target:
`fsm::states()` 0x03f210, `front()` 0x082cd0) but inlined in our base (absent in the base
index) - that part is blocked on the `ai` fsm type being matched, not an unsteerable LTCG
residual. This review did NOT rebuild; the body shape is unchanged.
