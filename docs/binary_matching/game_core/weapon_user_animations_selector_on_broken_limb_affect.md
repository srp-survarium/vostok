# weapon_user_animations_selector::on_broken_limb_affect (re-match)

RVA (target): 0x584960 / mangled
`?on_broken_limb_affect@weapon_user_animations_selector@survarium@@AAEXPBDW4hit_affects_type_enum@2@W4affect_event_type_enum@2@@Z`
(`AAE` = private; matches the `private:` declaration in weapon_user_animations_selector.h:138).

## Problem
Structure-verifier + reviewer agreed: BASE BODY WAS EMPTY (2 statements vs target's 5).
Prior STATE was 20.12% / PARTIAL. The carcass had decoded 3 source statements but the
L337 2-arg eater macro was unidentified ("needs the precise debug_macros.h form").

## Commands run
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function on_broken_limb_affect --list`
  -> two matches: damage_model (0x6ef660) and weapon_user_animations_selector (0x584960).
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x584960 --view target`
- `pdb_fetch ... --rva 0x584960 --view structure` -> `5 statements, 0x60 bytes`:
  L336 {, L337, L339, L341, L342 } (3 real statements: 337/339/341).

## Target asm (0x584960)
```
0x09: mov byte[ebp-1],0; lea eax,[ebp-1]; call eater          ; L337 guard
0x15: movzx;test;je .1
0x1c: push [ebp+10h](type); push [ebp+8](bodypart); call eater; add esp,8   ; eater(bodypart,type)
.1:
0x2c: mov byte[ebp-2],0; lea eax,[ebp-2]; call eater          ; L339 guard
0x38: movzx;test;je .2
0x3f: push 0; push 4; push [ebp+0Ch](affect); call eater; add esp,0Ch       ; eater(affect,4,0)
.2:
0x4f: mov edx,[ebp-8]; mov eax,[edx+44h]; call base_player::force_animation_selection  ; L341
0x5a: epilogue
```

## Statement decode
- L337: eater receives exactly 2 runtime args (bodypart, type), NO `push 0` for assert_untyped.
  Macro expansions (debug_macros.h, MASTER_GOLD/non-DEBUG branch):
  - `ASSERT_U(expr)` -> `expression_eater(assert_untyped, expr)` -> would add a `push 0`. NO.
  - `ASSERT_CMP_U(v1,op,v2)` -> `expression_eater(v1, v2, assert_untyped)` -> 3 pushes incl `push 0`. NO.
  - `ASSERT_T_U(assert_type, expression)` = `VOSTOK_EMPTY_EXPRESSION_U_VA_ARGS` ->
    `expression_eater(assert_type, expression)` = exactly 2 pushes, NO assert_untyped push.
  So the original used the raw typed-untyped form `ASSERT_T_U( bodypart, type )` (the assert_type
  slot holds the runtime `bodypart`, expression slot holds `type`). Push order eater(bodypart,type)
  -> push type first (rightmost), push bodypart last = matches `push [ebp+10h]; push [ebp+8]`.
- L339: `ASSERT_CMP_U( affect, ==, 4 )` -> eater(affect, 4, assert_untyped) -> `push 0; push 4; push affect`. Matches.
- L341: `m_user->force_animation_selection( )` (m_user @ +0x44, no-arg member).

## Result
Wrote the three statements. Single rebuild (`nix develop -c python3 scripts/rebuild.py`, no module arg).
report.json `fuzzy_match_percent`: **20.12% -> 86.38%**. Structure now `5 statements` (matches target).

First-try ASSERT_T_U guess was correct. Base 0x00-0x4f are byte-identical to target (both ASSERTs
matched perfectly).

## Residual (the 13.6%)
L341 only: `force_animation_selection()` is declared inline in base_player.h
(`void force_animation_selection() { m_force_animation_selection = true; }`), so OUR build inlines it:
```
base:   mov edx,[ebp-0Ch]; mov eax,[edx+44h]; mov [ebp-8],eax; mov ecx,[ebp-8]; mov byte[ecx+118h],1
target: mov edx,[ebp-8];  mov eax,[edx+44h]; call base_player::force_animation_selection
```
The target kept the call out-of-line (whole-program LTCG inline decision). This is the SAME
inline-vs-call wall documented for player_logic_sprint_state (47.50%). base_player.h is owned by
another unit, so out-lining it there to close this is out of scope for this unit.

## Regressions
report-changes.json shows 65 improved / 19 regressed, but those are delinker symbol-assignment
churn from relinking a freshly-checked-out worktree (empty_stub, network vcall thunks,
body_part_parameters dtor going 100->0 etc. - unrelated to a single one-function body edit). My
edit touched one function body only; the only true delta is on_broken_limb_affect 20.12->86.38.
