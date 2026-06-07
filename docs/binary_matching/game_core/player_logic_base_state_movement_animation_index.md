# game_core::player_logic_base_state::movement_animation_index

Static member: `static unsigned int survarium::player_logic_base_state::movement_animation_index(survarium::player_input const&)`.
Target rva 0x58c100 (carcass addresses 0x59c... are the stale base build).

## Target asm (pdb_fetch --view target)
- Reads `[eax+0x10]` = `player_input::actions_mask` (u32) four times, masking 1,2,4,8.
- Each `and; neg; sbb; neg` is the /Od boolean-normalize for `(actions_mask & MASK) != 0`.
- Slot map (from the byte stores):
  - `[ebp-2]` = mask 0x1  (computed 1st, carcass line 34) -> move_fwd_pressed
  - `[ebp-4]` = mask 0x2  (computed 2nd, line 35)         -> move_bwd_pressed
  - `[ebp-1]` = mask 0x4  (computed 3rd, line 36)         -> move_left_pressed
  - `[ebp-3]` = mask 0x8  (computed 4th, line 37)         -> move_right_pressed
- Conflict block 1 (0x4c): `if ( fwd && bwd ) { fwd=0; bwd=0; }`
- Conflict block 2 (0x64): `if ( left && right ) { left=0; right=0; }`
- Decision tree (0x7c..0xf6) returns animation_type_enum:
  - fwd: left->0x18 move_fwd_left, right->6 move_fwd_right, else 3 move_fwd
  - bwd: left->0x12 move_bwd_left, right->0xC move_bwd_right, else 0xF move_bwd
  - left->0x15 move_left, right->9 move_right, else 0 idle

Slot order ([ebp-2],[ebp-4],[ebp-1],[ebp-3]) is /Od slot packing, not declaration
order. Declared fwd,bwd,left,right to match carcass source lines 34-37.

Mask->direction confirmation: player_input_inline.h `is_sprinting()` requires
`(actions_mask & 0x001)` => mask 1 is forward movement.

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function movement_animation_index --list
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function movement_animation_index --view target
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function movement_animation_index --view info
- python3 scripts/rebuild.py   # NO module arg

## Iterations
1. INPUT: four `bool = (actions_mask & MASK) != 0` (decl fwd,bwd,left,right), two
   conflict-cancel ifs, nested if/else decision tree returning enum values; anchored
   via new use_game_core_player_logic_base_state() (construct player_input, call
   static, escape returned u32 through example_callback).
   BUILD: FAILED - player_logic_base_state.cpp(38) C2027: use of undefined type
   'survarium::player_input'. The .cpp included only player_logic_base_state.h,
   which forward-declares `struct player_input;`. Accessing `.actions_mask` needs
   the full type. FIX: add `#include <vostok/game_core/player_input.h>` to the .cpp.
2. INPUT: same body + added the player_input.h include.
   BUILD: FAILED at LINK - LNK2001 unresolved `vostok::ai::fsm_state::~fsm_state`
   and `survarium::player_logic_base_state::~player_logic_base_state`. Anchoring the
   static fn keeps player_logic_base_state.obj, which carries the abstract class's
   vtable -> needs both dtors (the documented fsm_state foundational gap +
   ~player_logic_base_state one level down). Neither is defined in our sources.
   FIX: add empty `fsm_state::~fsm_state(){}` and `~player_logic_base_state(){}`
   link stopgaps in the non-target temp_include_all.cpp TU (game_core/README.md
   pattern - cannot regress a matched obj).
3. INPUT: same body + both dtor stopgaps in temp_include_all.cpp.
   BUILD: FAILED at LTCG codegen - C4716 'get_attachment_transform must return a
   value' -> LNK1257. Providing the dtor lets MSVC emit the abstract class's vtable,
   which instantiates the inline virtual `get_attachment_transform`'s `{ /* no source
   */ }` body that returns nothing. FIX: give it `{ return float4x4(); }` (stub, not
   matched).
4. INPUT: same body + get_attachment_transform returns float4x4().
   BUILD: FAILED again, SAME C4716 at link codegen - but ninja ran `[1/1]` (link
   only), it did NOT recompile any .obj because only the *header* changed and the
   obj depfiles don't list the header. The stale .obj IL still has the empty body.
   FIX: `touch` the TUs that emit the vtable (player_logic_base_state.cpp,
   temp_include_all.cpp, player_logic_sprint_state.cpp) to force recompile with the
   fixed header.
5. INPUT: touched the 3 cpp TUs; body unchanged.
   BUILD: SUCCEEDED (delinked). But report.json fuzzy_match_percent = None for the fn.
   COFF symbol dump showed the mangling diff: TARGET `?...@@KAI...` vs BASE `?...@@SAI...`.
   `K` = private static member, `S` = public static. The fn must be PRIVATE.
   The 79 "regressions" are stale-baseline artifacts (scalar deleting dtors / empty_stub
   100->0 from other branches' scaffolding not on this branch) - my change doesn't touch them.
   FIX: move the static decl into `private:`; befriend the anchor so it can still call it.
6. INPUT: moved movement_animation_index to private:, added friend decl for the anchor.
   BUILD: SUCCEEDED. Mangling now matches (`KAI`==`KAI`) but pct still None (objdiff
   "not found" -> text-diff fallback, 77/113 = 68.1% equal). DIFF: body byte-identical
   through the boolean-normalize + two conflict blocks; the ONLY divergence is the jump
   structure. My if/else-if/else chain emits a redundant join `jmp` per nesting level
   (`jmp .13; jmp .5` pairs); the target does a single `jmp .11` per leaf to one epilogue.
   FIX: flatten to early returns without `else` so each leaf jumps once.
7. INPUT: rewrote decision tree as flat early-returns (no else).
   BUILD: SUCCEEDED. report.json still None (objdiff "not found in obj" -> text-diff
   fallback shows 91/91 = 100.0% instructions equal - body is byte-identical now!).
   Root cause of None: COFF symbol access mismatch. Base obj symbol = `CAI` (private
   static), target = `KAI`. MSVC static-member access codes: C=private, K=protected,
   S=public. The target is PROTECTED static, not private. (My earlier `private:` gave
   `C`; report.json/rich showed `KAI` from a stale read before recompile.)
   FIX: move the static into `protected:` -> `K`.
8. INPUT: moved movement_animation_index to protected:.
   BUILD: 100.0% (report.json), symbol `KAI` pairs. report-changes: 0 regressed,
   1 improved (0->100), 0 removed/added. DONE.

## Outcome
STATE[100%|DONE]: byte-identical (91/91 instructions). No blocker. Regressions caused: none.
Scaffolding added in the same unit (all in non-target TUs / header, can't regress a
matched obj):
  - player_logic_base_state.cpp: `#include <player_input.h>` (full type for actions_mask).
  - temp_include_all.cpp: new anchor `use_game_core_player_logic_base_state` (escapes the
    returned u32 via example_callback); link stopgaps `vostok::ai::fsm_state::~fsm_state(){}`
    and `survarium::player_logic_base_state::~player_logic_base_state(){}`.
  - player_logic_base_state.h: get_attachment_transform stub returns float4x4() (was
    `{ /* no source */ }`, fails LTCG codegen once the vtable is kept); movement_animation_index
    moved to `protected:` for the `K` mangling; friend decl for the anchor.
Inlining: none relevant.
