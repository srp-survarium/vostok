# game_core::character_dispersion_calculator::get_broken_hands_penalty

Target RVA 0x585e50 (`vostok/game_core/sources/character_dispersion_calculator.cpp`).
Signature: `float survarium::character_dispersion_calculator::get_broken_hands_penalty(const unsigned char, const bool) const`.
Mangled: `?get_broken_hands_penalty@character_dispersion_calculator@survarium@@ABEME_N@Z` (ABE = private const __thiscall).

## Dependency / base branch
This function is the sibling of `get_target_koef` (PR #110, branch
`match/game_core-character_dispersion_calculator_get_target_koef`). PR #110 built
ALL the shared scaffolding this function needs and is still OPEN (not on
`feature/agentic-matching-loop`, which only has the forward-ported ledger/docs):
- moved both getters to `private:` (so base mangles `ABE`, matching the target;
  `public`/`QBE` makes objdiff score `None`),
- added `#include <vostok/game_core/character_dispersion_params.h>`,
- made `tick`'s STUB body call BOTH getters (the getters are private, so `tick` is
  their sole caller - this is what keeps them link-reachable),
- added `use_character_dispersion_calculator()` anchor in `temp_include_all.cpp`.

I first tried this on `feature/agentic-matching-loop` directly: the rebuild dropped
ALL FOUR functions from the base obj (278-byte obj, only the ctor) -> score `None`,
because that branch lacks the tick-anchor + temp_include_all anchor, so the private
getters were dead-stripped. Lesson: this unit cannot stand alone on the bare
branch; I rebased my work onto PR #110's branch (commit 40e514d0) so the
scaffolding exists, and add only the get_broken_hands_penalty body here.

## Target asm (pdb_fetch --view target)

```
prologue: push ebp; mov ebp,esp; sub esp,14h; mov [ebp-8],ecx(this)
0x09: mov byte ptr [ebp-1],0; lea eax,[ebp-1]; call <empty_stub/finalize_impl>
        ^ LTCG inlined/stripped no-op call (delinker-misnamed). NOT in source.
0x15: mov cl,[ebp+8]; mov [ebp-0Ch],cl        ; broken_hands_count -> local
      cmp [ebp-0Ch],0 / je .1
      cmp [ebp-0Ch],1 / je .2
      cmp [ebp-0Ch],2 / je .5
0x2d: .1: fld1; jmp end                        ; case 0 AND fall-through default -> 1.0f
0x31: .2: movzx edx,[ebp+0Ch]; test; je .3     ; case 1 ternary on using_double_handed_weapon
0x39:     [m_params]; movss xmm0,[ecx+30h]; movss [ebp-10h]; jmp .4
0x4a: .3: movss xmm0,[clear_value=1.0f]; movss [ebp-10h]
0x57: .4: fld [ebp-10h]; jmp end
0x5c: .5: movzx edx,[ebp+0Ch]; test; je .6     ; case 2 ternary
0x64:     [m_params]; movss xmm0,[ecx+30h]; movss [ebp-14h]; jmp .7
0x75: .6: [m_params]; movss xmm0,[eax+34h]; movss [ebp-14h]
0x84: .7: fld [ebp-14h]
0x87: end: mov esp,ebp; pop ebp; ret 8
```

character_dispersion_params: 0x30 = injury_penalty_for_double_handed,
0x34 = injury_penalty_for_one_handed.

## COFF reader findings (zero rebuilds)
target obj `binaries/objdiff/target/vostok/game_core/sources/character_dispersion_calculator.cpp.obj`:
- .data: `clear_value`@+0 = 1.0f (this fn's case-1 const arm), `clear_value`@+4 = 1.0f (get_target_koef's).
- .text relocs in this fn: empty_stub (prologue call, secnum 0 = undefined/stripped),
  clear_value (case-1 1.0f const).
- target sym `...@@ABEME_N@Z` (private const). Confirmed both getters are ABE.

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function get_broken_hands_penalty --list   -> rva 0x585e50
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_broken_hands_penalty --rva 0x585e50 --view target
- nix develop --command pdb_fetch ... --view callees   -> empty_stub (delinker: finalize_impl)
- nix develop --command pdb_fetch ... --view info       -> 0 locals
- COFF parser over target + base objs (sections / symbols / .data floats / .text relocs)
- python3 scripts/rebuild.py   # NO module arg (full EXE relink) - on feature/agentic-matching-loop: all 4 fns dropped to None (no anchor)
- git checkout -b match/...get_broken_hands_penalty match/...get_target_koef   # rebase onto PR #110 scaffolding
- python3 scripts/rebuild.py   # second full rebuild on PR #110 base

## Iterations
1. INPUT (on bare feature/agentic-matching-loop): switch body + private move + include params.h.
   BUILD: None (all 4 fns dead-stripped - 278-byte obj, only ctor; no tick/anchor wiring).
   DIFF:  N/A (unpaired). Root cause: missing PR #110 scaffolding.
2. INPUT (rebased onto PR #110 branch 40e514d0): `case 0: break;` + tail `return 1.0f;`.
   BUILD: None (fuzzy_match_percent absent). Base obj HAS the symbol (ABE, text 0x0,
   0x80 bytes) - so it is not a mangling/strip issue.
   DIFF (COFF byte compare, base vs target): base lost the `cmp 0` - MSVC folded
   `case 0: break;` into the default path, emitting only `cmp 1 / cmp 2` then two
   jmps to the shared `return 1.0f`. Target keeps an explicit `cmp 0 / je <fld1>`.
   Fix: use `case 0: return 1.0f;` (distinct labeled block) + tail `return 1.0f;`;
   MSVC folds the two identical `fld1` returns into one reached by both the je-0 and
   the fall-through default - matching the target's single `fld1`.
   base bytes: 558bec 83ec10 894dfc 8a4508 8845f8 807df801.. (cmp1/cmp2 only)
   tgt  bytes: 558bec 83ec14 894df8 c645ff00 8d45ff e8.. 807df400 807df401 807df402 d9e8 (cmp0/1/2 + fld1)
3. INPUT: `case 0: return 1.0f;` + tail `return 1.0f;` (distinct case-0 block).
   BUILD: 82.89% (report-changes: 0 regressed, 1 improved 0.00 -> 82.89, 0 removed/added).
   DIFF (COFF byte compare): switch body now byte-exact in shape -
     base : 558bec 83ec10 894dfc 8a4508 8845f8 807df800 740e 807df801 740c 807df802 7431 eb5c d9e8 ...
     tgt  : 558bec 83ec14 894df8 c645ff00 8d45ff e8.. 8a4d08 884df4 807df400 740c 807df401 740a 807df402 742f d9e8 ...
   Both: cmp0/cmp1/cmp2 + single fld1, two ternaries reading [m_params+0x30]/[+0x34],
   case-1 1.0f const load `f30f1005...`. Residual = the empty_stub prologue + frame
   10h vs 14h + 4-byte [ebp-N] slot shift only. Same LTCG cap as get_target_koef (88%).

## Outcome
STATE[82.89%|PARTIAL]: observable switch body matches instruction-for-instruction.
CORRECTION (new guidelines): the prologue `mov byte[ebp-1],0; lea eax,[ebp-1];
call empty_stub` (delinker misnames it `finalize_impl`) is a COMPILED-OUT ASSERT,
not an LTCG artifact - recovered with `ASSERT( UNKNOWN_EXPRESSION_T( m_params ) )`
as the first statement (the `+0x0c` slot at body offset 0x009), which resolves the
frame 10h->14h / [ebp-N] slot shift. Same prologue/fix as get_target_koef. Cases
stay brace-less (ternary result temps are anonymous, no source local/ASSERT in any
case).
Regressions caused: none (final rebuild: 0 regressed, 1 improved, 0 removed/added).
Key learning: `case 0: break;` (folds case 0 into default, drops `cmp 0`) scored
None; `case 0: return 1.0f;` (distinct labeled block, two identical returns folded
by MSVC) scored 82.89%. The `None` was NOT a mangling/strip issue - the symbol was
present with the right ABE name; objdiff just omits fuzzy_match_percent when the
base body diverges too much structurally (missing the whole `cmp 0` block).
</content>
