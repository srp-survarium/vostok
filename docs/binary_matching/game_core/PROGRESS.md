# game_core matching progress (orchestrator ledger)

Module is large: 529 STUB markers across ~90 files at run start, plus 73 PARTIAL
and 55 SKIPPED. Workers dispatched one-per-function, leaf/small-first.

## Ledger
One line per matched function, added in that function's OWN commit (not here on the
infra base): `module::function -> STATE -> PR (regressions)`.

<!-- Each match commit appends its line below. -->
- game_core::game_core_initialize -> STATE[100%|DONE] -> PR #104 (regressions: none)
  - empty body == target's 5 bytes; it DOES get its own symbol (visible in objdiff), not ICF-unscorable.
- game_core::client_player_update::serialize -> STATE[BLOCKED] -> PR #105 (regressions: none)
  - body matched 1:1 from asm but parked: udp_match_packet.h cluster is never-compiled stubs.
- game_core::inventory_item::inventory_item(action_behaviour_type) -> STATE[78.8%|PARTIAL] -> PR #106 (regressions: none)
- game_core::scheduler::on_frame(u32,u32) -> STATE[46.39%|PARTIAL] -> PR #108 (regressions: none)
  - NOT a "bug fix": the target asm reads m_active_objects (off 0x10), so the source must too (the STUB's
    m_inactive_objects was wrong vs target). Matching the target, not correcting logic. Residual is
    unsteerable LTCG (target out-of-lines vectora::size/operator[], base inlines).
- game_core::dispersion_calculator::get_dispersion() const -> STATE[87.49%|PARTIAL] -> PR #109 (regressions: none)
  - body matches instruction-for-instruction; residual entirely LTCG (frame/slots, is_aimed inlined,
    safe-bool extra slot). Effectively done as source allows. Getter strategy works well.
- game_core::character_dispersion_calculator::get_target_koef(...) const -> STATE[95.74%|INPROGRESS] -> PR #110 (regressions: none)
  - switch case bodies + m_params reads match; recovered the empty_stub ASSERT (88% -> 95.74%).
    OPEN (faster machine): OUR base emits an extra `cmp 3; ja default` bounds check the target lacks
    (target has a contiguous jump table). Fix: `case type_preview: return 1.0f;` + `default: NODEFAULT();`.
    NOT LTCG - a source-structure problem. (NOTE: #110 was merged into feature-2 before review.)
- game_core::character_dispersion_calculator::get_broken_hands_penalty(u8,bool) const -> STATE[93.33%|PARTIAL] -> PR #111 (regressions: none)
  - switch body + m_params reads match instruction-for-instruction; recovered the empty_stub ASSERT
    (82.89% -> 93.33%). Residual not re-diffed after recovery; not LTCG. See the per-function .md.
  - PR CHAIN: #111 branched off #110 (shares the class scaffolding: private-getter mangling,
    params include, tick-stub anchor). Same-file functions chain like this.
- game_core::weapon_dispersion_calculator::get_value() const -> STATE[100%|DONE] -> PR #112 (regressions: none)
  - first non-trivial 100%: clean getter, no empty_stub prologue to cap it. Independent off feature.
- game_core::weapon_dispersion_calculator::{set_reload_dispersion_amount,set_one_shoot_dispersion_amount,set_aiming_speed} -> STATE[100%|DONE] -> PR #113 (regressions: none)
  - GROUPED unit (first use of accessor-grouping): all 3 setters 100% in one rebuild.
  - key: anchor must ESCAPE &calc/&table through an opaque sink so LTCG doesn't DSE the stores (see loop_performance).
- game_core::player_stealth::{player_stealth(const&),operator=} -> STATE[100%|DONE] -> PR #114 (regressions: none)
  - GROUPED unit on a fresh independent class (no chain): copy ctor + assignment, both 100% in one rebuild.
- game_core::weapon_state::operator= -> STATE[100%|DONE] -> PR #115 (regressions: none)
  - 100%. BONUS: weapon_state::weapon_state() default ctor incidentally hit 100% via operator='s observed
    anchor -> proves a constant-only ctor matches when its instance is OBSERVED. RETRY #107 (weapon_recoil
    18%) with the opaque-sink escape anchor; README rule revised.
- game_core::weapon_recoil_params::weapon_recoil_params() [RETRY of #107] -> STATE[100%|DONE] -> PR #116 (regressions: none)
  - 18% -> 100% with body untouched, just the observed-escape anchor. Confirms the revised ctor rule.
- game_core::breath_vibration_calculator::{ctor,dtor,set_breath_holding_params} -> STATE[ctor 100% DONE, dtor 100% DONE, setter 76.8% INPROGRESS] -> PR #117 (regressions: none)
  - GROUPED. ctor+dtor 100% (observed-escape anchor). setter INPROGRESS (76.8%): control structure NOT
    verified - the FUNCTION BODY structure shows the post-loop stores ('38'-'41') as plain function-scope
    statements with NO block-open marker, but the source wraps them in `if ( m_params ) { ... }`; the `{`
    would show as a <n> line that is ABSENT -> likely an early `return` guard (no braces). Restructure +
    re-diff on a faster machine. Secondary (after bracing): fsm::states()/front() out-of-line in target
    @0x03f210/0x082cd0 but inlined in base -> blocked on the ai fsm type. Added breath_state.h for the cast.
- game_core::body_part_parameters::fill_new_stats_item<statistics_item<46,16>> -> STATE[91.78%|BLOCKED] -> PR #119 (regressions: none)
  - SALVAGED: worker #15 crashed mid-run; finisher (#16) resumed from the WIP branch (branch-handoff -
    agent-context reuse / SendMessage is not available in this harness). Body was already a structural match.
    The "fixed_string<46>("none") needs the 3-arg ctor" hypothesis was MOOT: the 1-arg fixed_string(const char*)
    ctor IS that 3-arg buffer_string call inlined; residual is the fixed_string inline-vs-call shape.
  - REVIEW (2nd pass): reclassified PARTIAL -> BLOCKED. The inline-vs-call is unsteerable from this
    function's source (a whole-program COMDAT decision on fixed_string<46>: target has no out-of-line
    char-const* ctor, base keeps one @0x030ae0); body/control-structure are otherwise an exact match.
    Like #117's fsm, this is BLOCKED on the fixed_string<46> type's emission, not a banked LTCG residual.
    Also refreshed the stale FUNCTION BODY carcass to the authoritative target --view structure (0x0ba3c0).
- game_core::{get_bone_matrix_in_object_space, get_bone_matrix_in_object_space_impl} -> STATE[100%|DONE] -> PR #120 (regressions: none)
  - free-function pair (wrapper+impl), float4x4 matrix math, both 100% in one rebuild. Fresh independent.
- game_core::weapon_core_base_state::{ctor,deserializing} -> STATE[ctor INPROGRESS (bytes 100% but structure mismatch: body-assigns vs member-init list), deserializing 100%|DONE] -> PR #121, structure fix folded into PR #122 (regressions: none)
  - GROUPED, 100%. Foundational state base. Surfaced: ai::fsm_state::~fsm_state has NO body in our sources
    (target rva 0x3f210); state classes need a local stopgap dtor to anchor (README). Consider matching it.
- game_core::player_logic_base_state::movement_animation_index(player_input const&) [static] -> STATE[100%|DONE] -> PR #122 (regressions: none)
  - static fn, real u32 logic, 100%. Notes: carcass <VA> = base build (not target --list rva); /Od slot
    ordering is NOT declaration-order (allocation noise) - both now in MATCHING.md.
- game_core::weapon_core_idle_state_base::{ctor,initialize,finalize} -> STATE[100%|DONE] -> PR #123 (regressions: none)
  - GROUPED, all 3 at 100%. FIRST stacked PR: stacked on #121 (needed weapon_core_base_state scaffolding +
    fsm_state stopgap). Finding (on stack): mangled access codes U=public/M=protected/E=private virtual
    (initialize/finalize were private-virtual EAEXXZ, ctor protected IAE) - read them from target COFF up front.
- game_core::weapon_core_aimed_state_base::{ctor,initialize,finalize} -> STATE[ctor 100%, initialize 100%, finalize 54.39%|PARTIAL] -> PR #124 (regressions: none)
  - STACKED on #123. ctor+initialize 100%. finalize PARTIAL: HONEST inline-vs-call of
    animation_playback_state::reset() (NOT "LTCG-uncontrollable" - that mislabel is fixed). reset() is
    now IMPLEMENTED (real body `interval_id=0; interval_time=0.0f;`) in its own header
    animation_playback_state.h, mirroring the target TU layout. The target keeps reset out-of-line
    (`add eax,120h; call reset`, callee in an LTCG this-in-EAX frameless convention @0x087f60); our
    /Od /Ob2 /GL link INLINES reset's member-zeroing body into finalize -> 54.39%. Verified
    non-steerable: own-header decl/def split, __declspec(noinline), &reset address-escape, multiple
    real same-module callers ALL still inline (full log in the per-fn .md). The empty-stub elision
    that previously scored 83.33% was dropped: it required reset to be a FICTION (no-op stub) and the
    higher % was coincidental; faithful reset() is the correct choice. reset itself is ~0% (LTCG
    rewrote it to a frameless EAX function; source can only emit the /Od __thiscall frameful form).
    Correction (on stack): initialize/finalize are PROTECTED virtual (MAE), dispatch instant_aim_start/end via
    weapon_core vtable 0x8c/0x90 - unlike idle's non-virtual pair.
  - STACK TIP was match/game_core-weapon_core_aimed_state_base.
- game_core::weapon_core_show_state_base::{ctor,initialize,finalize,on_animation_end_impl} -> STATE[100%|DONE] -> PR #125 (regressions: none)
  - STACKED on #124. ALL FOUR at 100%. The earlier "initialize 75.13% / on_animation_end_impl 69.93% PARTIAL,
    unreproducible LTCG-folded empty call" diagnosis was WRONG: that empty out-of-line call was a compiled-out
    ASSERT. Recovering it (ASSERT( UNKNOWN_EXPRESSION )) closed both to 100% - see assembly_patterns empty_stub.
    ctor reproduces the inherited-member overwrite m_body_part_mask_for_user = body_part_whole_body_but_hands
    ([this+130h]=0xFFFFFFFD).
  - STACK TIP = match/game_core-weapon_core_show_state_base. New matches stack on this.
- game_core::weapon_core_hide_state_base::{ctor,initialize,finalize,on_animation_end_impl} -> STATE[100%|DONE] -> PR #126 (regressions: none)
  - STACKED on #125. ALL FOUR at 100%. Mirrors show_state_base; hide's on_animation_end_impl sets m_is_shown=false.
    The earlier "initialize 75.13% / on_animation_end_impl 69.93% PARTIAL, unreproducible LTCG" diagnosis was WRONG:
    the empty out-of-line call was a compiled-out ASSERT; recovering it (ASSERT( UNKNOWN_EXPRESSION )) closed both to
    100%. Also fixed a brace breakage in temp_include_all.cpp inherited from #125 (5 anchors missing `}`) -> #125
    alone may not compile; the stack tip does (flag when reviewing #125 standalone).
- game_core::weapon_core_animation_end_aware_state::{initialize,finalize,set_animation_to_wait,on_animation_end} -> STATE[initialize 100%, finalize 100%, set_animation_to_wait 77.33%, on_animation_end 90.92%|PARTIAL] -> PR #127 (regressions: none)
  - STACKED on #126. initialize/finalize 100%. set_animation_to_wait 77.33% PARTIAL: residual is the
    inline-vs-call of trivial weapon_core::get_user() (target out-of-lines @0x9b330; base inlines).
    on_animation_end recovered an empty_stub ASSERT (78.03% -> 90.92%); residual is the inline-vs-call frame
    shape. New guidelines: re-diff both residuals against source, do NOT bank as LTCG. Findings: a `const`
    method assigning a member needs the member `mutable` (mangles @@?BE); access-specifier must match target
    COFF mangling (members moved to protected to get MAE/IAE/IBE, not public UAE).
- game_core::legs_ik_processor::leg_params::{set_heel_transition_time,set_toe_transition_time,set_heel_on_ground,set_toe_on_ground} -> STATE[all 100%|DONE] -> PR #134 (regressions: none)
  - GROUPED unit (4 trivial nested-struct setters), STACKED on #127. All 100%. Findings: (1) the bool
    setters' `if` materializes a THIRD bool temp [ebp-0Ch] for the && RESULT - it is the inlined
    `is_full_on_ground()` whose body is `return is_heel_on_ground() && is_toe_on_ground();`; filling those
    three inline getters + `if ( is_full_on_ground() )` closed 59.90 -> 78.19 -> 100. A bare `m_heel && m_toe`
    short-circuits directly (no temp) -> wrong frame.
    (2) transition setters: `member = math::min( member, tr_time )`. Originally written `min( tr_time, member )`
    (arg first) and WRONGLY banked at 83.69% as "LTCG arg passing". That was a misdiagnosis: the xmm0/xmm1
    operand assignment FOLLOWS SOURCE OPERAND ORDER and is steerable. Target wanted arg->xmm0/member->xmm1
    (= member-first source, cf. clean `generic_anomaly_core::dec_energy` which is member-first and 100%);
    swapping to `math::min( member, tr_time )` took both 83.69 -> 100. min/max commutative, semantics intact.
    assembly_patterns.md note corrected (steerable operand order, never bank as LTCG).
    (3) report-changes.json: only improved entries (both setters 83.69 -> 100); no regressions.
- game_core::character_dispersion_params::{ctor,load} -> STATE[100%|DONE] -> PR #135 (regressions: none)
  - STACKED on #134. Small config-params struct, both functions 100% first try (no iteration).
    ctor: member-init list, all 14 multipliers = 1.0f (decoded target .obj .data = 14x 0x3f800000;
    delinker names the block `clear_value`. Verified vs target character_recoil_params.cpp.obj whose
    4-member `member(1.0f)` list compiled to 4x 1.0f in .data - same shape, not a reused .rdata literal).
    load: 14 brace-less `if ( cfg.value_exists("name") ) name = (float)cfg["name"];` in member order
    (each block has a `je short` over the body, no `+0x002` brace jmp) - identical shape to the
    character_recoil_params::load reference. Anchored via use_game_core_character_dispersion_params in
    temp_include_all.cpp: construct + load + escape &params through example_callback (observed-escape so
    LTCG does not DSE the constant ctor stores - the #107 18% trap). The 52 report-changes "regressions"
    are dtor/vcall-thunk/empty_stub/boost ICF churn in optimized modules (bullet/scaleform/stlp/sound/ai/
    particle/ui/physics) that flip 100%<->0% per relink (49 symmetric improved) - none touch this unit.
- game_core::weapon_dispersion_params::{ctor,config-ctor} -> STATE[100%|DONE] -> PR #136 (regressions: none)
  - STACKED on #135. Small weapon-params struct (8 floats, size 0x20), both ctors 100% first try.
    Decoded constants by hand-parsing the target .obj COFF (.data/.rdata floats + .text relocs):
    default ctor = member-init list base_dispersion(0.0f), from_the_hip..growth_speed(1.0f x6),
    max_dispersion(2.0f) - the six 1.0f live in the delinker `clear_value` .data pool, 0.0f/2.0f in .rdata.
    config ctor = the SAME init list, then 8 brace-less `if ( cfg.value_exists("name") ) name=(float)cfg["name"];`
    in member order, then a TRAILING unconditional `one_shoot_dispersion_amount = 0.0f;` (matched verbatim -
    the ctor clears it after the reads, discarding any config value; movss from .rdata 0.0f). Anchored via
    use_game_core_weapon_dispersion_params: construct both + escape &obj through example_callback (observed-
    escape so LTCG doesn't DSE the constant stores - the #107 trap). The 61 report-changes "regressions" are
    dtor/vcall-thunk/empty_stub/boost-storage/math-ctor ICF churn in optimized modules (69 symmetric improved)
    - none touch this unit or any matched game_core fn.
- game_core::dispersion_calculator::dispersion_calculator() (default ctor) -> STATE[100%|DONE] -> PR #137 (regressions: none)
  - STACKED on #136. ctor only (set_weapon/apply_aim_speed are separate units, NOT inlined - both are
    real out-of-line `call`s). Member-init list `m_weapon(NULL), m_shooting_skill_coeff(1.0f),
    m_aiming_speed_coeff(1.0f)` was already drafted; verified the constants against survarium.exe
    directly (movss operand disp -> rva -> f32): BOTH coeffs = 1.0f and BOTH movss reference the SAME
    pooled rdata slot 0xa7b6c4 (MSVC pooled the two identical 1.0f literals; the delinker's `clear_value`
    placeholder rdata read 0.0 and is misleading - read the real EXE bytes). Target asm = 2 sub-object
    ctor calls (weapon_dispersion_calculator @0x00, character_dispersion_calculator @0x20 via `add ecx,20h`),
    `mov [eax+40h],0`, two `movss [.+44h]/[.+48h]`. Structure: 2 statements (L22 `{` 0x44, L23 `}` 0x7).
    Mangled `QAE` (public) matches the header. Anchored via use_dispersion_calculator: construct +
    get_dispersion() + escape &calc through example_callback (observed-escape so LTCG cannot DSE the
    constant stores - the #107 trap). The ctor was in fact already 100% in the prior report (construct +
    get_dispersion was enough to keep stores observed); the &calc escape is belt-and-suspenders and caused
    0 regressed / 0 improved across all 1995 units.
- game_core::dispersion_calculator::set_weapon(weapon_core*) -> STATE[100%|DONE] -> PR #138 (regressions: none)
  - STACKED on #137. set_weapon only (apply_aim_speed / set_one_shoot_dispersion_amount /
    set_reload_dispersion_amount are out-of-line `call`s, NOT inlined - separate units, untouched).
    Body: `m_weapon = weapon; if ( m_weapon ) { m_weapon_calculator.set_one_shoot_dispersion_amount(
    m_weapon->get_dispersion_params().one_shoot_dispersion_amount ); m_weapon_calculator.
    set_reload_dispersion_amount( m_weapon->get_dispersion_params().reload_dispersion_amount ); }
    apply_aim_speed();`. weapon_core::m_dispersion_params @0x3e4; the inline get_dispersion_params()
    returns &m_dispersion_params so the compiler folds field offsets +0x10/+0x14 into the asm
    constants 0x3F4 (one_shoot) / 0x3F8 (reload). Braced `if` confirmed by the two `<0>` carcass
    markers bracketing L91/L92 (the `{`/`}` are L90/L93). Mangled `QAE` (public) matches header.
    Anchored via use_dispersion_calculator: added `calc.set_weapon( NULL )`. First-try 100% match;
    report-changes 0 regressed / 1 improved (set_weapon 0.00 -> 100.00). The `--view diff` 97.16%
    header is one `~` at 0x4c (relative call displacement to apply_aim_speed, a reloc address, not a
    body diff); report.json is 100.0.
