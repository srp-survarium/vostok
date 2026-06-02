# game_core::pistol_weapon_core_aimed_idle_state + double_barreled_weapon_core_aimed_idle_state (batched unit)

Aimed-idle counterparts of the idle-state classes matched in PR #151 (weapon_core_idle_state)
and PR #153 (pistol_/double_barreled_weapon_core_idle_state). Matched as ONE unit (one
branch/commit/PR). 8 functions total (4 each):
- ctor `(weapon_core&, managed_resource_ptr const*, u32)`
- `weapon_and_hands_expression(...) const`
- `get_weapon_lexeme_pair(...) const`
- `weapon_core_state_cook_template<T>::new_object(...)`

Branch off stack tip `match/game_core-pistol-double-barreled-idle-state` (PR #153).

## Target RVAs (from pdb_rich_query --list)
pistol: ctor 0x79b7a0  new_object 0x79b8b0  get_weapon_lexeme_pair 0x79b910  weapon_and_hands_expression 0x79b9d0
dbl:    ctor 0x79c910  new_object 0x79ca30  get_weapon_lexeme_pair 0x79ca90  weapon_and_hands_expression 0x79cb60

(The carcass `// FUNCTION BODY` addresses in the original stub cpp were stale 0x7ab.../0x7ac...;
the real RVAs above are the ground truth. Structure/deltas still match the carcass.)

## Access chars (from target mangled names in binaries/rich/target/index.jsonl) - all PRIVATE
- ctor                  `??0...@@AAE@`  -> private, non-const
- weapon_and_hands      `...@@EBE...`   -> private virtual, const
- get_weapon_lexeme_pair `...@@ABE...`  -> private, const
- cook new_object       `...@@AAE...`   -> private, non-const
The stub headers declared these PUBLIC; changed to PRIVATE + befriended the two new
temp_include_all anchors + the cook template (mirror #153). Otherwise objdiff scores None.

## Differences from the idle siblings (#153)
- captions per class (read straight from the target asm string literals):
  - pistol: {"pistol-aimed_idle", "pistol-empty_aimed_idle"}; index = (ammo_in_magazine()==0)
    (asm 0x25: movzx ax; neg;sbb;add 1 -> 0 if ammo!=0, 1 if ammo==0).
  - dbl: {"aimed_both_barrels_empty","aimed_one_barrel_loaded","aimed_two_barrels_loaded"};
    index = ammo_in_magazine() directly (asm 0x38: movzx; store).
- base class is weapon_core_aimed_state_base (0x138), not _idle_state_base; m_weapon_animations
  at +0x138. pistol [2][2][2] size 0x158; dbl [2][2][3] size 0x168 (same as idle siblings).
- inner stride: pistol view*16 (shl 4) user_state*8 ammo*4; dbl view*0x18 (imul 18h)
  user_state*0x0C (imul 0Ch) ammo*4. (identical to idle siblings)

## ASSERT eater layout (read from target asm - each is a `mov byte,0; lea; call empty_stub`)
- pistol ctor 0x79b7a0: ASSERT_CMP_U(count,==,8) at 0x3f (the _U form: movzx;test;je;push 0;push 8;
  push [ebp+10h]=count;call;add esp,0Ch), trailing plain ASSERT at 0xf1. NO leading plain assert.
- dbl ctor 0x79c910: leading plain ASSERT at 0x3f (no branch), ASSERT_CMP_U(count,==,12) at 0x4b
  (_U form pushes 0x0Ch), trailing plain ASSERT at 0x102.
- pistol getter 0x79b910: NO leading eater; trailing plain ASSERT (call empty_stub at 0xaf, after
  add esp,28h, before the sret return).
- dbl getter 0x79ca90: leading plain ASSERT at 0x09 (before captions); trailing plain ASSERT at 0xbd.
- both weapon_and_hands 0x79b9d0/0x79cb60: ASSERT_U(weight_driving_animation) at 0x11 (the _U eater
  with movzx;test;je + rep movsd of the lexeme), then get_weapon_lexeme_pair, then
  return expression(main+offset) where the TARGET INLINES the addition_lexeme ctor + cloned_in_buffer
  (the operator+ inline) -> expected PARTIAL ~85.65%, documented LTCG cause, do NOT chase.

## Commands run
- pdb_rich_query --index binaries/rich/target/index.jsonl --list   (RVAs + mangled chars)
- pdb_fetch --target-index ... --rva <rva> --view target            (per-fn asm, all 8)
- python3 scripts/rebuild.py   (NO module arg; relinks EXE ~20 min)

## INCIDENT: first rebuild wasted - cpp written to wrong dir
The cpp lives at `sources/vostok/game_core/SOURCES/<file>.cpp` (note the `/sources/`); the .h
lives at `sources/vostok/game_core/<file>.h`. First Write dropped the `/sources` segment and
created the cpp next to the header, so ninja compiled the UNTOUCHED stub cpps and the first
~20-min relink was against stubs. Killed it, `mv`'d the files into `sources/`, restarted.
LESSON: after writing a NEW .cpp, `git status` it - a `??` under the header dir (not under
`.../sources/`) means it is misplaced; the build silently uses the old stub. Also: the trailing
getter `call empty_stub` is the existing get_weapon_lexeme_pair_impl temporary cleanup (present
in the #153 idle getters that matched 99.92% with NO trailing ASSERT) - do NOT add a trailing
ASSERT to the getters; only the ctors have the `mov byte,0; lea; call` eater shape.

## Build #1 (real, corrected sources) results - FINAL, no regressions
report.json fuzzy_match_percent:
- pistol::ctor 100% DONE, pistol::new_object 100% DONE
- pistol::get_weapon_lexeme_pair 99.92% DONE (ammo-this reg + 2 benign relocs, same as #151/#153)
- pistol::weapon_and_hands_expression 85.65% PARTIAL (operator+ LTCG inline, documented)
- dbl::ctor 100% DONE, dbl::new_object 100% DONE
- dbl::get_weapon_lexeme_pair 99.92% DONE
- dbl::weapon_and_hands_expression 85.65% PARTIAL

First (corrected) build hit every target score on the first try - the source is a faithful
mirror of the idle siblings (#153) with the per-class captions / index / ASSERT-eater layout
read straight from the target asm.

### report-changes.json regressions = relink ICF churn only (accepted, same as #153)
44 "regressed 100->0" entries, all COMDAT-folded boilerplate: `vector deleting destructor`s,
plain dtors (~btHashMap, ~speedtree_data), boost `storageN` ctors, `resource_ptr` copy ctors,
`interlocked_decrement`, `float3::float3()`, `medkit::tick`, `empty_stub`, bullet/Scaleform
helpers. 56 matching "improved 0->100" entries (the ICF flip-back). NO deliberately-matched
source function regressed; the watched neighbors (idle siblings, weapon_core, ammo_in_magazine,
get_weapon_lexeme_pair_impl) are all clean. This is the documented relink-churn the
`game_core_initialize` ICF note describes - not a source regression.

## Reusable learnings
1. The aimed-idle classes are byte-for-byte structural twins of the idle classes (#153): same
   ASSERT layout per function-kind, same getter-impl call shape, same operator+ PARTIAL. Only
   the string captions, the ammo-index expression (pistol `==0`, dbl direct), the loop bound
   (weapon_state 2 vs 3) and the class size (0x158 vs 0x168) differ. Read those four deltas off
   the target asm and the rest copies verbatim.
2. The trailing getter `call empty_stub` (after `add esp,28h`, no `mov byte,0; lea` setup) is the
   get_weapon_lexeme_pair_impl temporary cleanup, NOT an ASSERT. Confirmed by the #153 idle
   getters that matched 99.92% with NO trailing ASSERT in source. Adding a trailing ASSERT would
   inject an EXTRA `mov byte,0; lea; call` the target does not have. Only the ctors have a real
   trailing-ASSERT eater (the `mov byte,0; lea; call` shape).
3. (time-saver) After Write-ing a NEW .cpp, `git status` it before kicking off the ~20-min
   rebuild: a cpp belongs under `.../game_core/sources/`, the header under `.../game_core/`. A
   `??` entry directly under the header dir means the path dropped its `/sources` segment and the
   build will silently compile the OLD stub - a wasted full relink. (Cost me one here.)

