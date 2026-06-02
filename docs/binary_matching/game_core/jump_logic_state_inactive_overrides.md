# jump_logic_state_inactive virtual overrides (initialize, is_ready_for_transition)

Unit: two trivial same-class header inline overrides of
`survarium::jump_logic_state_inactive` (derives from `jump_logic_base_state` ->
`ai::fsm_state`). Bundled in one branch/commit/PR.
NOT in this unit: `selected_animations` (separate non-trivial .cpp STUB).

## Target ground truth

The header carried `<0x2a800>` / `<0xe23e0>` (image notation, 0x10000 above the
rich-index RVA). Verified against target disassembly, not assumed.

Commands:

    nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function "jump_logic_state_inactive" --list
    # 0x01a800 ... initialize() (also finalize()/execute() fold to the same RVA)
    # 0x0d23e0 ... is_ready_for_transition() const

Exact bodies (rich index target, by exact name):

`void survarium::jump_logic_state_inactive::initialize()` rva 0x1a800 size 11
(mangled representative after /OPT:ICF fold:
`?nonnull@dummy@?$function1@...@boost@@QAEXXZ`):

    push  ebp
    mov   ebp, esp
    push  ecx
    mov   [ebp-4], ecx
    mov   esp, ebp
    pop   ebp
    ret

-> empty `__thiscall void` body: `{ }` (the 7-byte /Od this-spill empty-body
frame; assembly_patterns "empty function body", thiscall variant). CONFIRMED this
is the shared empty-function fold the dispatch note warned about: `initialize`,
`finalize`, `execute` all alias rva 0x1a800.

`bool survarium::jump_logic_state_inactive::is_ready_for_transition() const`
rva 0xd23e0 size 13 (mangled representative after fold:
`?is_update_modifier@particle_action_orbit@particle@vostok@@UBE_NXZ`):

    push  ebp
    mov   ebp, esp
    push  ecx
    mov   [ebp-4], ecx
    mov   al, 1          ; <- return true
    mov   esp, ebp
    pop   ebp
    ret

-> `{ return true; }`.

Visibility: both `public` virtual overrides (representative mangling `U..E`,
non-const `UAE` / const `UBE`); header already has them under `public:`.

## Base side before change

`binaries/rich/base/index.jsonl` had NO `jump_logic_state_inactive` functions at
all -> the class was never anchored, so the header inline overrides were never
emitted; `report.json` showed the `.h` unit `fuzzy: None` with only stray
COMDAT-folded `breath_state*` symbols mapped onto it.

## Source changes

1. `jump_logic_state_inactive.h`: `initialize() { }`, `is_ready_for_transition()
   const { return true; }`; STATE markers -> `100%|DONE`.
2. `temp_include_all.cpp`: `#include "jump_logic_state_inactive.h"` + new
   `use_game_core_jump_logic_state_inactive()` anchor (member-fn address-of ONLY:
   `&...::initialize` / `&...::is_ready_for_transition`, escaped via `example_callback`,
   to force a standalone out-of-line body for each). It deliberately does NOT construct
   an instance - see "Anchor (final)" below for why (an earlier construct-an-instance
   variant failed with C4716/LNK1257). Wired into `IncludeAll::IncludeAll()`.

## Rebuild / score

    git checkout -b match/game_core-jump_logic_state_inactive-overrides   # off tip match/game_core-weapon_recoil_params-config-ctor (PR #147)
    nix develop --command python3 scripts/rebuild.py

Rebuild 1 (full, no-arg): ninja FAILED to compile temp_include_all.cpp.
  .\temp_include_all.cpp(876) : error C2653: 'jump_logic_state_inactive' :
  is not a class or namespace name
Cause: the anchor sits inside `namespace vostok { ... }`, so unqualified
`jump_logic_state_inactive` resolved to `vostok::` (the class is in `survarium::`).
Fix: fully-qualify everything in the anchor as `survarium::jump_logic_state_inactive`
/ `survarium::jump_logic`. (Caught early with a fast `rebuild.py game_core` module
build - rather than burning the full relink on the typo.)

Rebuild 2 (full, no-arg): ninja compiled temp_include_all but LINK FAILED:
  jump_logic_state_inactive.cpp(28): error C4716:
  'survarium::jump_logic_state_inactive::selected_animations' : must return a value
  LINK : fatal error LNK1257: code generation failed
Cause: the anchor CONSTRUCTED a jump_logic_state_inactive instance, which emits
the class vtable and forces codegen of the still-STUB `selected_animations` (a
non-void body with no `return` -> C4716 -> LNK1257). selected_animations is a
SEPARATE unit, not mine to fill. (Documented trap: loop_performance.md "every
inline virtual in the vtable with a non-void return must actually return".)
Fix: drop the instance construction + qualified calls from the anchor; keep ONLY
the two member-fn address-of escapes. Address-of ODR-uses just initialize /
is_ready_for_transition (forces their standalone bodies) without emitting the
vtable, so selected_animations is never codegen'd.

Rebuild 3 (full, no-arg): SUCCEEDED, base diff inputs refreshed (FULL2_EXIT=0).

## Result: bytes match, but ICF-unscorable

`report.json` -> `jump_logic_state_inactive.h` unit fuzzy: None, and its listed
functions are stray `breath_state*` symbols (other classes' identical inline
overrides that fold to the same addresses). The base `binaries/rich/base/index.jsonl`
shows NO `jump_logic_state_inactive::initialize` / `::is_ready_for_transition`
standalone symbol - only the anchor `use_game_core_jump_logic_state_inactive`. No
base `jump_logic_state_inactive.h.obj` is delinked (only a target one exists).

This is NOT a source mismatch - it is the `/OPT:ICF` whole-program fold + delinker
symbol-pairing limitation (same class as the documented `game_core_initialize`
empty-function gap, temp_include_all.cpp lines 127-133). Proof the BYTES are exact:

    # base funcs whose .text is byte-identical to the target's two folds:
    #  100 funcs == empty __thiscall body  (push ebp; mov ebp,esp; push ecx;
    #               mov [ebp-4],ecx; mov esp,ebp; pop ebp; ret)  -> the initialize fold
    #   30 funcs == return-true body        (... mov al,1 ...)    -> the is_ready fold
    # e.g. boost::function1<...>::dummy::nonnull (the very symbol the TARGET folds
    # jump_logic_state_inactive::initialize into) and
    # weapon_core_base_state::is_ready_for_transition (a return-true sibling).

The target itself folds these two functions to `?nonnull@dummy@...` (0x1a800) and
`particle_action_orbit::is_update_modifier` (0xd23e0); neither side keeps a
`jump_logic_state_inactive::*` symbol, so objdiff has nothing to pair and reports
None. The emitted bytes are byte-identical to the target. Marked 100%|DONE.

report-changes.json: 50 regressed / 53 improved, ALL whole-program trivial folds
(scalar/vector deleting dtors, vcall thunks, refcount inc, intrusive_ptr ctors,
empty_stub, ...) flipping 100<->0 as the ICF fold-representative reshuffled when two
new identical trivial bodies were added. None are functions this PR's source touches;
net is roughly neutral. These are fold-representative churn, not real regressions.

## Anchor (final)

`use_game_core_jump_logic_state_inactive()` takes ONLY the two member-fn addresses
(`&survarium::jump_logic_state_inactive::initialize` / `::is_ready_for_transition`)
and escapes them via `example_callback`. It deliberately does NOT construct an
instance: instantiating the class emits its vtable -> forces codegen of the still-STUB
`selected_animations` (no return -> C4716/LNK1257). Address-of ODR-uses only the two
members this unit owns.

