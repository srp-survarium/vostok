# pistol_weapon_core_aimed_fire_state

Mirror of `pistol_weapon_core_fire_state` (PR #174, branch
`match/game_core-pistol_weapon_core_fire_state`, commit 49b53894). The aimed
variant differs only in:
- base class `weapon_core_aimed_fire_state_base` (not `weapon_core_fire_state_base`)
- captions `"pistol-aimed_shot"` / `"pistol-aimed_last_shot"`
- the expression/lexeme machinery (weapon_and_hands_expression / get_user_hands_expression)
  remains INPROGRESS exactly as in the fire-state sibling.

## RVAs (target index)
- ctor                       0x79abc0
- new_object                 0x79ad60
- get_user_hands_expression  0x79add0
- get_weapon_lexeme_pair     0x79af30
- weapon_and_hands_expression 0x79b000
- initialize                 0x79b0e0

## Commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function "pistol_weapon_core_aimed_fire_state" --list`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x... --view target`

## Per-function notes

### ctor (0x79abc0)
Identical shape to fire-state ctor (100%): ASSERT_CMP_U(animations_count,==,12),
two triple/double nested loops copying animations, trailing ASSERT(UNKNOWN_EXPRESSION).
Base class init `weapon_core_aimed_fire_state_base( weapon, animation_time_scale )`.
0x6b mov byte/call empty_stub = ASSERT_CMP_U; 0x182 mov byte/call empty_stub = trailing ASSERT.

### initialize (0x79b0e0)
Calls base initialize, computes last_shot from get_bullets_in_queue ? (ammo==1) : (ammo==0),
stores into m_weapon_animation_index. Now 99.76% in report.json (after access UAE->MAE fix).
RESOLVED: the final-store boolize residual - rewriting `m_weapon_animation_index = last_shot`
as `m_weapon_animation_index = last_shot ? 1u : 0u` reproduces the target's `movzx;neg;sbb;neg`
final store byte-for-byte (verified: the 0x6e..0x7b block matches on both sides in pdb_fetch
--view diff). The ternary was the correct source shape, not LTCG.
SOLE REMAINING residual: 0x30/0x50 target `mov eax,[ecx+128h]` vs base `mov ecx,...` before
the ammo_in_magazine() call = LTCG this-register placement at the call boundary (the STUB
callee uses default thiscall ecx; the target's LTCG passes the m_weapon pointer in eax).
This is argument passing at the call boundary, not source-steerable until ammo_in_magazine is
matched. STATE[99.76%|PARTIAL].

### get_weapon_lexeme_pair (0x79af30)
Identical to fire-state (100%) but captions "pistol-aimed_shot" / "pistol-aimed_last_shot"
(target strings ??_C@..pistol?9aimed_shot / pistol?9aimed_last_shot).

### new_object (0x79ad60)
Identical to fire-state (92.08% PARTIAL): placement-new + computed_shooting_animation_time_scale.
Residual is LTCG calling convention of computed_shooting_animation_time_scale (ref in reg,
ret xmm0). Same as sibling.

### weapon_and_hands_expression (0x79b000) / get_user_hands_expression (0x79add0)
INPROGRESS - large addition_lexeme/operator+ machinery, same as fire-state sibling
(both INPROGRESS there). Carry the stub + carcass forward.

## Verified results (re-match: rebuilt + read from binaries/objdiff/report.json)
- new_object              92.08%  PARTIAL  (pairs; LTCG calling-conv of
                                            computed_shooting_animation_time_scale, verified)
- ctor                    100%    DONE     (now pairs in report.json after access QAE->IAE fix)
- get_weapon_lexeme_pair  100%    DONE     (now pairs in report.json after access QBE->ABE fix)
- initialize              99.76%  PARTIAL  (now pairs after UAE->MAE; ternary fixed the boolize
                                            residual; sole residual = LTCG this-reg at the
                                            ammo_in_magazine() call boundary)
- weapon_and_hands_expression  INPROGRESS  (stub return; access UBE->EBE fix applied, pairs at 21.43%)
- get_user_hands_expression    INPROGRESS  (stub return; access QBE->ABE fix applied, pairs at 12.78%)

## ACCESS FIXES APPLIED (re-match, this PR update)
Verified each against the target mangled access char via pdb_rich_query/index grep, then set the
matching C++ access specifier in the .h:
- ctor                          target ??0...@@IAE  -> `protected:`        (was QAE)
- initialize                    target ...@@MAE     -> `protected:` virtual (was UAE)
- weapon_and_hands_expression   target ...@@EBE     -> `private:` virtual   (was UBE)
- get_weapon_lexeme_pair        target ...@@ABE     -> `private:` const     (was QBE)
- get_user_hands_expression     target ...@@ABE     -> `private:` const     (was QBE)
Because the ctor/initialize became protected and the lexeme virtuals private, the
temp_include_all.cpp anchor `use_game_core_pistol_weapon_core_aimed_fire_state()` could no longer
reach them; added the sibling-idle pattern to the .h: a `namespace vostok { void
use_game_core_pistol_weapon_core_aimed_fire_state(); }` forward-decl plus two friend declarations
(`template<typename T> friend class weapon_core_state_cook_template;` and
`friend void ::vostok::use_game_core_pistol_weapon_core_aimed_fire_state();`). After the header
edit, `touch`ed the .cpp and ran `rebuild.py` (no module arg). report-changes.json: 0 regressed,
5 improved (the two lexeme INPROGRESS fns went 0->21/12 because they now pair).

new_object (??...@@AAE) already paired and stays 92.08% - sole residual is the verified call-boundary
calling-convention of computed_shooting_animation_time_scale (arg in reg + ret xmm0 in target;
cdecl push + st0/fstp in our STUB callee) = legitimate LTCG.

new_object (??...@@AAE) already pairs (mangling matches) and scores 92.08% cleanly - its sole
residual is the verified call-boundary calling-convention of computed_shooting_animation_time_scale
(arg in reg + ret xmm0 in target; cdecl push + st0/fstp in our STUB callee) = legitimate LTCG.

## Scaffolding required by this unit (outside the .cpp's own functions)
- weapon_core_aimed_fire_state_base.h: m_animation_timescale / m_playback_type private -> protected
  (read by get_weapon_lexeme_pair; same change PR #174 made to weapon_core_fire_state_base.h).
- weapon_core_aimed_fire_state_base.cpp: on_shot_event / on_aiming_event were pure STUBs with NO
  return; the derived pistol_weapon_core_aimed_fire_state vtable forces these virtuals through
  LTCG codegen, which fails C4716 "must return a value". Added a minimal
  `return animation::callback_return_type_call_me_again;` to each (kept STATE[STUB]; body left as
  carcass for the weapon_core_aimed_fire_state_base matcher). This is the FIRST anchor to
  instantiate that base class, hence the first to surface the C4716.
- weapon_core_state_cook_template.h: friend + fwd-decl for use_game_core_pistol_weapon_core_aimed_fire_state.
- temp_include_all.cpp: include + use_game_core_pistol_weapon_core_aimed_fire_state anchor + IncludeAll call.

## Build/link gotchas hit (time-sink, recorded for the next worker)
- The C4716 in a *base class's* stub virtual surfaces only at LINK time (LTCG/`/GL` does codegen
  at link), citing the base .cpp. Symptom: link produces a 0-byte EXE and ninja/rebuild.py still
  exit 0 (ninja sees the 0-byte output as "created"), so report.json/index stay STALE and the
  base index has 0 entries for your unit. Capture it with a direct
  `wine link @<rsp> ... 2>&1` - the "Generating code" line is immediately followed by the C4716.
- Do NOT manually `rm` vc90.pdb / the PCH between builds: it breaks the PCH (C2859) and forces a
  full game_core recompile. If a build leaves a broken vc90.pdb, remove BOTH the .pch and vc90.pdb
  together so they regenerate as a pair, then run rebuild.py once.
- rebuild.py's index regen can race the watchdog: it regenerated from a stale EXE while ninja's
  link was still finishing. After a long/slow link, run rebuild.py a SECOND time (ninja up-to-date,
  no relink) so the index/report regenerate from the now-fresh PDB. Confirm with
  `grep -c pistol_weapon_core_aimed_fire binaries/rich/base/index.jsonl` (should be 9, not 0).
