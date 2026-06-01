# game_core::character_dispersion_calculator::get_target_koef

> **STATUS: INPROGRESS (verified; fix deferred to a faster machine).** The dispatch
> differs, and the divergence is in OUR base, NOT the target (verified 2026-06-01 by
> disassembling both - target rva 0x585ee0, base rva 0x44c040):
> - TARGET: `mov edx,[ebp-0Ch]; jmp [edx*4+table]` - a direct, contiguous 5-entry jump
>   table over values 0..4, **no bounds check**. `type_jump`(3) is a plain table entry;
>   `type_preview`(4) maps to the `fld1` (=1.0f) body.
> - BASE (ours): `cmp [ebp-0Ch],3; ja default; jmp [edx*4+table]` - an extra bounds
>   check + a 4-entry table, because our `switch` stops at `type_jump`(3) and uses
>   `default: return 1.0f;`, so MSVC bounds the table at 3.
>
> FIX (faster machine), two parts, then rebuild + re-diff:
> 1. **Dispatch:** add `case type_preview: return 1.0f;` and change
>    `default: return 1.0f;` -> `default: NODEFAULT();` (NODEFAULT = `__assume(0)` in
>    Master Gold, `sources/vostok/debug_macros.h`). `__assume(0)` tells MSVC the default
>    is unreachable, so it drops the `cmp 3; ja` and emits the contiguous [0..4] table.
> 2. **Braces:** the carcass shows a `+0x002` step (a 2-byte `jmp short` = a closing
>    `}`) at body offset 0x5d (src line 108) and 0xc0 (src line 114) - the `type_stand`
>    and `type_crouch` blocks were `{ }`-brace-scoped. Add the braces; a brace changes
>    the codegen/structure (MATCHING.md "switch"). The single-`return` cases
>    (`sprint`/`jump`) show no `+0x002` and stay brace-less.
>
> NOT an LTCG residual - a source-structure problem. The per-case float bodies + the
> recovered ASSERT already match; the dispatch + braces are what is open.

`float survarium::character_dispersion_calculator::get_target_koef(const survarium::weapon_user_state_enum, const bool, const bool) const`
Target rva: 0x585ee0 (obj `.text` offset 0xd4 in character_dispersion_calculator.cpp.obj).

## Commands run (verbatim, in order)
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function get_target_koef --list`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_target_koef --rva 0x585ee0 --view target`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_target_koef --rva 0x585ee0 --view callees`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_target_koef --rva 0x585ee0 --view info`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_broken_hands_penalty --view target`  (sibling, same prologue)
- COFF parser over `binaries/objdiff/target/vostok/game_core/sources/character_dispersion_calculator.cpp.obj`:
  read section headers, symbol table, `.text` relocations, and the 5 jump-table
  dwords stored inside `.text` (relocs at obj 0x1e4..0x1f4, addend = fn-relative target).
- `python3 scripts/rebuild.py`   # NO module arg

## Target asm summary
Prologue: `mov [ebp-8],ecx` (this); then `mov byte[ebp-1],0; lea eax,[ebp-1];
call empty_stub` (a stubbed/stripped call the delinker named `empty_stub`; also
present verbatim in `get_broken_hands_penalty`; not reproducible from source).
Then `switch (character_state)` via `jmp dword ptr [edx*4 + table]`.

Jump table (case -> fn-relative offset, from .text relocs):
- type_stand(0)  -> 0x25
- type_crouch(1) -> 0x8b
- type_sprint(2) -> 0xef
- type_jump(3)   -> 0xf9
- type_preview(4)-> 0x103 (default: fld1 = 1.0f)

Member offsets used (character_dispersion_params):
idle@0 idle_aim@4 walk@8 walk_aim@0xc run@0x10 jump@0x14 crouch@0x18
crouch_aim@0x1c crouch_walk@0x20 crouch_walk_aim@0x24.

Logic:
- stand:  is_moving ? (is_aiming?walk_aim:walk) : (is_aiming?idle_aim:idle)
- crouch: is_moving ? (is_aiming?crouch_walk_aim:crouch_walk) : (is_aiming?crouch_aim:crouch)
- sprint: run
- jump:   jump
- default:1.0f

## Iterations
1. INPUT: switch body as above; `get_target_koef` declared PUBLIC in header;
   anchor `use_character_dispersion_calculator()` calls it directly.
   BUILD: report.json `fuzzy_match_percent = None` for get_target_koef.
   DIFF:  objdiff could not pair base/target - mangled-name MISMATCH:
          base `?get_target_koef@...@@QBE...` (public Q) vs
          target `?get_target_koef@...@@ABE...` (private A).
          Verified via COFF symbol dump: target getters are PRIVATE.
2. INPUT: moved get_target_koef + get_broken_hands_penalty to `private:` in
   the header (`QBE`->`ABE`). Anchor can no longer call them directly, so:
   removed direct calls from the anchor; made the `tick` STUB call both getters
   (tick is the sole caller in the target, per --view callees) to keep them
   reachable transitively.
   BUILD: report.json fuzzy_match_percent = 88.39%.
          report-changes: 0 regressed, 3 improved (get_target_koef 0->88.39,
          tick 7.22->25.26, get_broken_hands_penalty 0->14.76 - all from the
          new stub references; net code +0.00, 0 regressed).
   DIFF:  `pdb_fetch --view diff` refuses ("needs both indexes") because the
          `get_target_koef` substring also matches `tick`'s callees field in the
          index -> ambiguous pair, backend bails. Diffed the COFF `.text` bytes
          of base vs target obj directly instead.
          - Leaf branches (movzx;test;je; mov this; mov [m_params]; movss [+off];
            movss [tmp]; jmp; fld [tmp]) are byte-IDENTICAL in shape; only differ
            by register choice / [ebp-N] slot / a 2-byte jmp displacement.
          - Divergence is the prologue: target has
            `mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub` (a
            stripped/inlined call the delinker names `empty_stub`, also in
            get_broken_hands_penalty), forcing `sub esp,1Ch` vs my `18h` and
            shifting every [ebp-N] slot by 4. Not reproducible from source.
          - Switch dispatch: target `jmp [edx*4+table]` direct; base
            `cmp [ebp-N],3; ja default; jmp [eax*4+table]` (extra range check).

## Outcome
STATE[95.74%|INPROGRESS]: switch case BODIES + all leaf m_params reads match
byte-for-byte; the recovered ASSERT fixed the prologue. The one remaining divergence
is the dispatch, and it is in OUR base (verified by disassembling both):
- ASSERT recovery (88% -> 95.74%): the prologue `mov byte[ebp-1],0;
  lea eax,[ebp-1]; call empty_stub` (delinker misnames it `finalize_impl`) is a
  COMPILED-OUT ASSERT - recovered with `ASSERT( UNKNOWN_EXPRESSION_T( m_params ) )`
  as the first statement, which emits the `+0x0c` bytes and resolves the frame-size
  18h->1Ch / [ebp-N] slot shift.
- Open (faster machine): BASE emits `cmp [ebp-0Ch],3; ja default` + a 4-entry table;
  TARGET has NO bounds check and a contiguous 5-entry table. Cause: our `switch` ends
  at `type_jump`(3) + `default: return 1.0f;`. Fix: add `case type_preview: return 1.0f;`
  and `default: NODEFAULT();` (`__assume(0)`, debug_macros.h) so MSVC drops the bounds
  check and emits the contiguous [0..4] table. NOT an LTCG residual - a source-structure
  problem. Cases stay brace-less (no per-case local/ASSERT).
Regressions caused: none.
