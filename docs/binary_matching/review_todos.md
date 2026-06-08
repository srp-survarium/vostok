# Review TODOs (`sushi@TODO:`)

Open matching questions sushi raised in PR review, recorded in-source as `sushi@TODO:`
markers and indexed here so a later agent can sweep them in one pass.

- **Live set** (source of truth): `grep -rn "sushi@TODO:" sources/` — the markers travel with
  the code, so this is always current even if the table below drifts.
- **This table** is the curated index: one row per marker, with the function and the PR it came
  from. Append a row whenever you add a `sushi@TODO:` to the source; tick `Done` (and drop the
  marker) when the matching question is resolved.

Append-only ledger (union-merged across branches, like `PROGRESS.md` / `unanswered_questions.md`).

| Status | Function | File:line | PR | TODO |
|--------|----------|-----------|----|------|
| not done | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` | #155 | Function is **not matched-done**: `STATE[None]` with no STRUCTURE DIFF — objdiff can't pair it (the LTCG inline-vs-call of the `animation_lexeme_parameters` setters shortens the body past the pairing threshold). Needs structure recovery before it earns a real % / embed. |
| open | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (`.bones_mask( 2 )`) | #155 | Is `2` a bare magic constant or a named value defined somewhere? |
| open | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (the `bool dummy` dead store) | #155 | Unlikely a `bool dummy` — if it were, `dummy` would appear in the locals (it does not). The lone 4-byte dead store needs further matching to recover the compiled-out structure (would let objdiff pair it: currently `STATE[None]`). |
| investigate | `legs_ik_processor::process` | `game_core/sources/legs_ik_processor.cpp` (the `process()` STRUCTURE DIFF, `ONLY base` rows) | #159 | The 3 `ONLY base` rows (recomputed `hip_obj_matrix` + two `process_leg` calls) are statements **we** emit that target doesn't align here. Labeling them an "aligner swap" is suspect: if a stmt is only in base we can't know the target stmt is really there — likely a real **structure** problem (an extra/duplicated branch), not a diff-aligner artifact. Investigate before trusting the %. |
| investigate | `s_ik_*_cc` dynamic initializers | `game_core/sources/legs_ik_processor.cpp` | #159 | Four compiler-generated dynamic-initializer (+ atexit destructor) thunks marked `STATE[STUB]`. Unscored (`None`, name-pairing artifact). Try to match them so a mis-written static init would surface. |
| audit | all `ASSERT_U` / `ASSERT_CMP_U` (27 across 11 files) | `game_core/sources/*.cpp`, `animation/sources/*.cpp` | #164 | **Re-audit: many of our `ASSERT_U(x)` are probably `VOSTOK_UNREFERENCED_PARAMETER(S)(x)`.** Both emit the same `if(identity(false)){ folded_empty(args) }` eater shape; distinguish by whether `x` is an *asserted expression* (ASSERT_U) or the function's own *otherwise-unused parameter* (unreferenced macro). Prime suspect: `ASSERT_U( weight_driving_animation )` recurs in 6 weapon-state `weapon_and_hands_expression` files where that param is used nowhere else. Per-function: pull the target asm, check what the eater pushes, swap mislabeled ones to `VOSTOK_UNREFERENCED_PARAMETER(S)`, rebuild and require `regressed: []`. See assembly_patterns.md (the ASSERT_U + unreferenced-parameter entries). |
| pairing-fixed | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` | #155 | ROOT CAUSE of the `None` found: the `playback_enum` param's enum was named `playing_type_enum` (with `playback_enum` a typedef alias) in `animation/mixing.h`, so the base symbol mangled to `...W4playing_type_enum@mixing...` while the target uses `...W4playback_enum@mixing...` — different names objdiff can't pair. The target NEVER emits `playing_type_enum` (0 occurrences vs 2 `playback_enum@mixing`); two base symbols carried the wrong tag (this fn + the `weapon_core_shotgun_reload_base_substate` ctor). FIX: renamed the enum tag to `playback_enum`, dropped the typedef (byte-neutral; every site already used the alias). Rebuild in flight to confirm the symbols now pair. Any residual after that is the body inline-vs-call class (setters out-of-line in target, inlined by our /GL), out of scope. |
| name-fixed, still None | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` | #155 | The `playback_enum` param's enum tag is now correct: `animation/mixing.h` tags it `playback_enum` (the target's real tag — the enum appears in exactly 2 target mangled names, this fn + the `weapon_core_shotgun_reload_base_substate` ctor, both `W4playback_enum@mixing`, ZERO `playing_type_enum`), making our base mangled name CHARACTER-IDENTICAL to the target's. This tag is **zero-cost**: clean builds of `playback_enum` vs the old `playing_type_enum`+alias on the SAME toolchain show 0 unit-level regressions (the "408 regressed" once blamed on it was June-7→June-8 delinker/tooling drift, not the rename — verified by rebuilding commit 5038c307: 7952 matched, identical). HOWEVER the symbol STILL does not pair (stays `None`): the blocker is the BODY-level whole-program inline-vs-call of the inline-in-class `animation_lexeme_parameters` setters (target out-of-line, our /GL inlines them), an out-of-scope LTCG class. No zero-cost lever pairs this fn; left `STATE[None]`. See `game_core/get_weapon_lexeme_pair_impl.md`. |
| done | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (`.bones_mask( 2 )`) | #155 | RESOLVED: `2` == `animation::body_part_hands_only` (= 0x0002 in `animation/type_definitions.h`; synonym `bone_mask_offset_only` also = 2). Chose `body_part_hands_only` to match the sibling offset_lexeme's `body_part_whole_body_but_hands`. Replaced the magic `2`. |
| done | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (the `bool dummy` dead store) | #155 | RESOLVED: the lone `<0x4>` `mov byte[ebp-5],0` at target 0x72 (NO following lea/call) sits between the last main_lexeme_parameters setter and main_lexeme's construction — an unused `bool` local dead-stored under /Od, NOT an ASSERT (which adds lea+call = `<0xc>`). The reviewer's "dummy not in locals" doubt is consistent with a never-read bool: MSVC's PDB local table can omit a variable whose sole use is a dead store, yet /Od still emits the slot store. `bool dummy = false;` reproduces this single statement's bytes exactly; only the whole-function pairing was blocked (the enum-tag row above), independent of this line. |
| done | `legs_ik_processor::process` | `game_core/sources/legs_ik_processor.cpp` (the `process()` STRUCTURE DIFF, `ONLY base` rows) | #159 | RESOLVED: NOT a structure bug. Pulled the target asm, decoded the full four-branch `s_ik_adjust_hip_position` ladder (`.2`=both legs / `.5`=`+=right_delta` when left is_on_ground / `.8`=`+=left_delta` when right is_on_ground / `.9`=`left.is_on_ground()?right.is_on_ground()`) — base/target control flow is one-to-one. The operand-aware byte diff (`pdb_fetch --view diff`) is 100% `~` register/slot renames with ZERO `<`/`>` (missing/extra) instructions, so the three ONLY-base rows are an LCS aligner mis-pairing of the four textually-identical `process_leg` blocks, not an extra branch. Residual = the m_hip_bone pointer spill (0x4 frame, slot-rename cascade). |
| done | `s_ik_*_cc` dynamic initializers | `game_core/sources/legs_ik_processor.cpp` | #159 | RESOLVED: pulled both base (`??__E...`) and target (`dynamic initializer for...`) init asm. cc_float (foot_capsule) is BYTE-IDENTICAL to target. The three cc_bool inits carry correct arg values (default filter `general`→target ecx=1 for debug_draw; `execution_filter_early`→ecx=0 for rot_axis/adjust_hip); they differ from target only in the register-vs-stack call convention (LTCG: target passes `this` in esi + regs eax=1/ecx=filter, base uses thiscall + stack pushes). No mis-written init. STUB markers promoted to DONE; report.json stays `None` (universal cc name-pairing artifact). |

## Provisional idiom candidates (NOT definitive — DO NOT promote to `assembly_patterns.md` yet)

Both were drafted from `game_core/get_weapon_lexeme_pair_impl`, which is **unverified** (`STATE[None]`
— it doesn't pair, so the whole-function match can't be confirmed). They differ in confidence:
**#1 is likely right** — its asm reproduces byte-for-byte at that single arg site, which is checkable
locally regardless of the function pairing — while **#2 (the `bool dummy`) is the one questioned in
review** above. Kept here (not in `assembly_patterns.md`) so the future matcher has both in hand;
promote #1 once any paired function exercises it, and resolve #2 with the further matching it needs.

**1. Conditional-pointer arg `val != u32(-1) ? &obj : NULL`**  *(likely right — reproduces byte-for-byte at the site)*
```
push 0                  ; (an unrelated arg pushed first)
mov  edx, [ebp+20h]     ; val (a u32)
sub  edx, 0FFFFFFFFh    ; edx - (-1) == edx + 1
neg  edx                ; CF set iff edx+1 != 0  (i.e. val != -1)
sbb  edx, edx           ; edx = (val != -1) ? 0xFFFFFFFF : 0
lea  eax, [ebp-0E0h]    ; &obj
and  edx, eax           ; (val != -1) ? &obj : NULL
push edx
```
Source: write the ternary `val != u32(-1) ? &obj : NULL` (a pointer-or-NULL select on an unsigned
`!= -1` test) — NOT hand-rolled asm. MSVC /Od lowers `!= u32(-1)` to `sub r,-1; neg; sbb r,r` (mask
= all-ones iff non-equal) and ANDs it with the address. Candidate site: the offset lexeme's
`time_driving_animation` arg = `time_synchronization_group != u32(-1) ? &main_lexeme : NULL`.

**2. Lone 4-byte `mov byte[ebp-N],0` (no lea/call) = unused `bool b = false;`, not an ASSERT**
A structure statement of size `<0x4>` whose only instruction is `mov byte ptr [ebp-N],0` with NO
following `lea eax,[ebp-N]; call <empty_stub>` is a plain unused `bool` local dead-stored under /Od.
DISTINGUISH from a compiled-out `ASSERT` (`<0xc>` = the byte-store PLUS `lea+call`). Writing
`ASSERT(UNKNOWN_EXPRESSION)` for a `<0x4>` slot over-produces the lea+call. The 4-vs-7-byte disp is
just small-vs-large `[ebp-N]` offset (allocation noise). Candidate site: `get_weapon_lexeme_pair_impl`
L40, target `<0x4>` `mov byte[ebp-5],0`. **Caveat:** only valid if the surrounding match is correct
— here it is not yet (see the `not done` row).
