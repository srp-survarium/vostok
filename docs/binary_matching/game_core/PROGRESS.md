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
- game_core::weapon_core_base_state::{ctor,deserializing} -> STATE[100%|DONE] -> PR #121 (regressions: none)
  - GROUPED, 100%. Foundational state base. Surfaced: ai::fsm_state::~fsm_state has NO body in our sources
    (target rva 0x3f210); state classes need a local stopgap dtor to anchor (README). Consider matching it.
- game_core::player_logic_base_state::movement_animation_index(player_input const&) [static] -> STATE[100%|DONE] -> PR #122 (regressions: none)
  - static fn, real u32 logic, 100%. Notes: carcass <VA> = base build (not target --list rva); /Od slot
    ordering is NOT declaration-order (allocation noise) - both now in MATCHING.md.
- game_core::weapon_core_idle_state_base::{ctor,initialize,finalize} -> STATE[100%|DONE] -> PR #123 (regressions: none)
  - GROUPED, all 3 at 100%. FIRST stacked PR: stacked on #121 (needed weapon_core_base_state scaffolding +
    fsm_state stopgap). Finding (on stack): mangled access codes U=public/M=protected/E=private virtual
    (initialize/finalize were private-virtual EAEXXZ, ctor protected IAE) - read them from target COFF up front.
- game_core::weapon_core_aimed_state_base::{ctor,initialize,finalize} -> STATE[ctor 100%, initialize 100%, finalize 83.33%|PARTIAL] -> PR #124 (regressions: none)
  - STACKED on #123. ctor+initialize 100%; finalize PARTIAL: residual is the inline-vs-call of
    animation_playback_state::reset (real out-of-line fn, not an ASSERT). New guidelines: re-diff, do NOT bank as LTCG.
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
- game_core::legs_ik_processor::leg_params::{set_heel_transition_time,set_toe_transition_time,set_heel_on_ground,set_toe_on_ground} -> STATE[set_heel_on_ground 100%, set_toe_on_ground 100%, set_heel_transition_time 83.69%, set_toe_transition_time 83.69%|DONE] -> PR #134 (regressions: none)
  - GROUPED unit (4 trivial nested-struct setters), STACKED on #127. bool setters 100%; transition setters
    83.69% DONE (LTCG arg passing). Findings (no banking as LTCG): (1) the bool setters' `if` materializes a
    THIRD bool temp [ebp-0Ch] for the && RESULT - it is the inlined `is_full_on_ground()` whose body is
    `return is_heel_on_ground() && is_toe_on_ground();`; filling those three inline getters + `if ( is_full_on_ground() )`
    closed 59.90 -> 78.19 -> 100. A bare `m_heel && m_toe` short-circuits directly (no temp) -> wrong frame.
    (2) transition setters: `member = math::min( tr_time, member )`; the ONLY residual is which xmm each min
    arg lands in (target arg-position order tr->xmm0/member->xmm1; base reversed) - the link-time custom
    register convention, the permitted call-boundary arg-passing class. Source order is correct.
    (3) the 59 report-changes "regressions" are all dtor/thunk/empty_stub ICF churn in optimized modules
    (bullet/boost/scaleform/engine/particle) - none touch legs_ik_processor or this unit.
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
- game_core::dispersion_calculator::apply_aim_speed() -> STATE[100%|DONE] -> PR #139 (regressions: none)
  - STACKED on #138. Body: `float const speed_of_aiming = m_weapon ? m_weapon->get_dispersion_params().
    speed_of_aiming * m_aiming_speed_coeff : 0.0f; m_character_calculator.set_aiming_speed(speed_of_aiming);
    m_weapon_calculator.set_aiming_speed(speed_of_aiming);`. PDB local `const float speed_of_aiming`.
    Member offsets: m_weapon @+0x40, m_aiming_speed_coeff @+0x48; weapon_core::get_dispersion_params()
    (inline) -> m_dispersion_params @0x3e4, .speed_of_aiming @+0x0c => [edx+3F0h];
    character_dispersion_calculator @0x20, m_aiming_speed @+0x14 => [this+0x34]. The else-branch const is
    `0.0f` read directly out of survarium.exe (movss operand abs VA 0x974044 -> rva 0x964044 in .rdata =
    0x00000000), NOT the disassembler's masked `[offset]`. SHAPE before build: (1) mangled `AAE` => moved
    apply_aim_speed public -> PRIVATE in dispersion_calculator.h; (2) character_dispersion_calculator::
    set_aiming_speed given a real body `{ m_aiming_speed = aiming_speed; }` (was empty stub) so L82 inlines
    to the [this+0x34] store; (3) weapon_dispersion_calculator::set_aiming_speed stays an out-of-line call
    (target call @0x5d). No new anchor (already reachable via matched set_weapon). First-try 100%;
    report-changes 0->100 for apply_aim_speed, cluster unchanged, the 55/56 regressed/improved churn is
    delinker non-determinism (none in the dispersion/weapon/character cluster).
- game_core::player_stealth::{player_stealth(), load} -> STATE[100%|DONE] -> PR #140 (regressions: none)
  - STACKED on #139. Small class player_stealth (copy ctor + operator= already matched #114, member
    layout reused). Default ctor (rva 0x59aad0): EMPTY body - target stores no members (unlike
    character_dispersion_params whose ctor stored 14x 1.0f); already 100% from #114's anchor. load
    (rva 0x59aae0): 11 unconditional `m_x = (float)config["x"];` in member/offset order, NO
    value_exists guard (simpler than character_dispersion_params::load #135). Strings/offsets:
    default_value@0 default_sound_value@4 stand_factor@8 crouch_factor@0xc crouch_sound_factor@0x10
    walk_factor@0x14 walk_sound_factor@0x18 sprint_factor@0x1c sprint_sound_factor@0x20
    detection_level@0x24 always_visible_distance@0x28. Both `QAE` (public), header unchanged.
    Anchor: added `stealth.load(cfg)` to existing use_game_core_player_stealth(). First-try 100%
    for both; --view diff clean (ctor 8/8 equal, load no -/+ lines). report-changes 49 reg / 58+ imp
    all relink ICF/vcall-thunk/boost-storage churn (none in this unit; the 2 game_core-named hits are
    a collision_sensor scalar-deleting-dtor thunk and a BLOCKED inventory_item packet stub).
- game_core::player_logic_base_state::{ctor, set_user} -> STATE[100%|DONE] -> PR #141 (regressions: none)
  - STACKED on #140. Class layout reused from movement_animation_index (#122). ctor (rva 0x58c220,
    `QAE` public): member-init list in offset order - m_owner(owner)@0x18, m_user(NULL)@0x1c,
    m_weapon_user_state_id@0x20, four bools @0x24..0x27 (m_is_weapon_weapon_visible/m_is_smoothing_needed/
    m_is_physics_transform_allowed/m_is_ready_to_be_deactivated) all `true`. set_user (rva 0x58c200,
    `UAE` public virtual): `m_user = &user;` (0x1c). Base-class `call vostok::ai::fsm_state::fsm_state`
    appears IDENTICALLY in target AND base (fsm_state has only a pure-virtual dtor; its implicit ctor is
    a folded out-of-line symbol absent from both rich indexes by name but called in both) - the
    orchestrator's inline-vs-call/BLOCKED risk resolved with NO source change => clean DONE, not BLOCKED.
    Anchor: use_game_core_player_logic_base_state got a `concrete_logic_state` stub overriding ALL still-
    pure inherited virtuals (fsm_state initialize/execute/finalize + own selected_animations - first build
    C2259'd until all four were overridden), delegating ctor + calling set_user, &state escaped via
    example_callback. Owner/user refs fabricated from NULL ptr casts (anchor never runs) to dodge
    constructing noncopyable weapon_user_animations_selector. report-changes: only symmetric COMDAT/dtor/
    CRT 100<->0 delinker re-slice churn (39 scalar-deleting-dtor, float3/float4/float4x4 ctors,
    empty_stub, boost/bt/resource_ptr); no matched unit regressed.
- game_core::ik_processor::{ctor, activate} -> STATE[ctor 100%, activate 100%|DONE] -> PR #142 (regressions: none)
  - STACKED on #141. GROUPED (same trivial class). ctor (rva 0x57eba0, `IAE` protected): member-init list
    m_skeleton(NULL)@0x0, m_last_time_in_ms(0)@0x4; the pre-store `call ...finalize_impl` is the COMDAT-folded
    core::noncopyable base ctor (noncopyable::noncopyable, finalize_impl AND fsm_state::~fsm_state all fold to
    rva 0x03f210) - NOT an ASSERT (an ASSERT would emit `mov byte[ebp-N],0; lea eax; call`). activate
    (rva 0x57eb80, `IAE` protected): `m_skeleton = &skeleton;`. Header: both public -> protected (IAE).
    Anchor: concrete_ik_processor : ik_processor subclass in use_game_core_ik_processor (construct -> protected
    ctor; do_activate -> protected activate; &proc escaped).
  - VERIFIED after /nix was freed (83G avail): full `python3 scripts/rebuild.py` relink, report.json
    fuzzy_match_percent = 100.0 for both `??0ik_processor@survarium@@IAE@XZ` and
    `?activate@ik_processor@survarium@@IAEXABVskeleton@animation@vostok@@@Z`; both show as improved
    (0.0 -> 100.0) in report-changes.json. The folded base-ctor call reproduced exactly (no ASSERT). Carcass
    deleted (clean 100% DONE). report-changes: 53 regressed are all symmetric COMDAT/dtor/CRT delinker
    re-slice churn (scalar-deleting-dtor, thunk/vcall, resource_ptr, intrusive_ptr/interlocked, buffer_string
    operator=), balanced by 65 improved; no matched game_core unit regressed.
- game_core::legs_ik_processor::{set_left_heel_on_ground, set_left_toe_on_ground, set_right_heel_on_ground,
  set_right_toe_on_ground, set_heel_on_ground(leg_params&,bool), set_toe_on_ground(leg_params&,bool)} ->
  STATE[4x 100%, helpers 98.84%/98.59% | DONE] -> PR #143 (regressions: none)
  - STACKED on #142. GROUPED: the 4 public left/right setters (QAE) are trivial one-liners that delegate
    to the 2 private (AAE) leg_params& helpers, pulled into the same unit. m_left_leg_params@0x10,
    m_right_leg_params@0x40. Helpers reuse the leg_params setters matched in #134 (set_heel_on_ground(bool)
    etc.) + math::clamp(t,0.001f,0.5f) + transition_time_calculator::reset()(=m_value=0.0f) + two
    set_*_transition_time delegations + `m_*_interpolator = fermi_interpolator(time)`. Header: added
    `private:` before the two helpers; gave transition_time_calculator::reset() a `{m_value=0.0f;}` body.
  - SHARED HEADER FIX (in this PR): fermi_interpolator.h had `float const` members + a private operator=,
    which makes `m_*_interpolator = fermi_interpolator(...)` illegal (C2248). The PDB ground-truth structure
    header shows plain non-const floats and NO operator= (compiler-implicit memberwise op=, copies +4/+8 and
    skips the vtable - exactly the target bytes). Fixed working header to match the PDB. Only fermi consumer
    that assigns; game/animation only construct it, no matched codegen changes.
  - Helpers' lone residual: `+ lea ecx,[ebp-0Ch]` (target-only) before the temp ~fermi_interpolator dtor
    call - the dtor's `this` arg at the ICF/LTCG-folded call boundary (target dtor symbol finalize_impl, base
    boost::function1::dummy::nonnull - same folded empty fn). Permitted call-boundary arg class; every
    statement, control structure, and const matches. New assembly_patterns.md entry: "member = T(args)".
  - Anchor: use_game_core_legs_ik_processor() constructs a legs_ik_processor, calls all four public setters
    (helpers survive transitively), escapes &processor. report-changes: 68 regressed all symmetric COMDAT/
    dtor/CRT delinker re-slice churn (empty_stub, scalar-deleting-dtors, float3/4/4x4 ctors, boost _bi,
    bt*/Scaleform/ai/particle/render dtors, noncopyable, interlocked), balanced by 49 improved (incl. all 6
    of this unit, 0.0 -> final); no matched game_core unit regressed.
- game_core::get_additional_length -> STATE[65.38%|PARTIAL] -> PR #144 (regressions: none)
  - STACKED on #143. Free function in namespace survarium (legs_ik_processor.cpp), target rva 0x0bb1f0.
    Body: `float const cos = upleg_dir | -leg_dir; return is_similar(cos,1.0f,epsilon_5) ? knee_len*0.5f :
    sqrt(sqr(knee_len)*0.5f/(1.0f-cos));` - a single ternary return (4 statements per structure). Constants
    read from the target obj .text relocs: clear_value=1.0f (.data, bare literal), half=0.5f, epsilon_5 =
    vostok::math::epsilon_5. operator- = unary float3 negate, operator| = dot product.
  - Residual (the ONLY diff): the `operator|` dot is INLINED in base but the target emits `call
    vostok::math::operator|`. pdb_rich_query shows operator| present out-of-line in BOTH indexes (target
    0x8160, base 0x371e0) => per-call-site whole-program LTCG inline-vs-call of a trivial COMDAT, not
    steerable from this function's source. The frame shift (sub esp,24h vs 20h) + [ebp-18h] temp + [ebp-24h]
    vs [ebp-20h] result slot all cascade from that one inline. Every other statement, all constants, and the
    ternary control flow are byte-exact (objdiff fuzzy 88.43%). New assembly_patterns.md entry added.
  - Anchor: use_game_core_get_additional_length(float3 const*, float3 const*) - calls the free fn (NULL
    args, never runs) and escapes the float result; forward-declared in the survarium decl block; called from
    IncludeAll. process_leg (its real caller) is still STUB. report-changes: only basic_streambuf::imbue
    100->0 (an unrelated CRT/STL streambuf method, build-ordering report flake; no game_core source touched).
- game_core::legs_ik_processor::get_foot_fixed_transform -> STATE[84.16%|PARTIAL] -> PR #145 (regressions: none)
  - STACKED on #144. Private const member (mangled ABE), target rva 0x6ebae0, ~0xb5a bytes, 64 statements
    (src L294-L400). Large float4x4 IK math; the full control structure is matched: 5 bone world matrices
    `matrices[<bone>_index - get_skeleton().get_root_bones_count()] * hip_world_matrix`, the
    `is_similar(foot.c.xyz,toe.c.xyz)||is_similar(foot.c.xyz,leg.c.xyz)` early-return-foot, 3 normalized dirs
    (foot_to_leg/foot_to_toe/left), an identity+i/j/k matrix built then `create_rotation(left_dir,deg2rad(30))
    *result`, 3 ASSERTs, result.c=foot pos then =transform_position(up_dir), capsule_size/colors, a 4-way
    ground-state if-chain (is_full_on_ground / is_heel_on_ground / is_toe_on_ground / else) computing
    start/finish + rotation_interpolation_koef (1-interpolated_value for heel branches, raw for toe),
    get_relative_matrix(foot_world,result), two `s_ik_legs_debug_draw && m_drawer` debug-draw guards,
    m_character_controller->adjust_foot_transform, foot_center_transform = foot_to_center_rel*result, three
    bone lengths (leg/up_leg/knee_len) + `delta_len = sum - up_leg_to_fixed_foot_dist`, the
    `sqr(dist)>orig_sqr && heel_transition_time!=0` blend, and `return foot_center_transform`.
  - BIGGEST FIX: un-hoisted root_count - the target RE-CALLS get_skeleton().get_root_bones_count() fresh at
    every matrix site (7x), the index helper inlined each time; caching it in one local dropped the repeated
    `mov eax,[m_skeleton]; call get_root_bones_count` (81.55 -> 84.16). New assembly_patterns.md entry.
    Also: the two up_leg distances use the cached `up_leg_world_matrix` ref (hip-multiplied), not fresh
    matrices[]. knee_world_matrix is a declared-but-unused local in BOTH binaries (C4189 matches target;
    kept). else-branch is `finish = start`.
  - Setup: get_relative_matrix (inline math helper, target rva 0xbb050) absent from our headers -> defined
    vostok::math::get_relative_matrix directly in the .cpp (a header edit was a no-op due to PCH staleness).
    #include physics/api.h before character_controller.h (VOSTOK_PHYSICS_API). Added protected inline
    ik_processor::get_skeleton() to read the private base m_skeleton. s_ik_legs_debug_draw_value(bool)/
    s_ik_foot_capsule_radius_value(float) declared as file statics (the cc machinery is STUB). Anchored via a
    friend free fn use_game_core_legs_ik_processor_get_foot_fixed_transform (friend decl = no bytes), called
    from IncludeAll, escapes the returned float4x4.
  - Residual: register/[ebp-N] slot renaming + LTCG arg-passing/temp-materialization at the many math
    operator/helper call boundaries (operator -+^*, normalize, create_rotation, transform_position,
    is_similar, length, get_root_bones_count, interpolated_value, adjust_foot_transform) + a few
    trivial-COMDAT inline-vs-call decisions. All statement byte sizes (`; <0xNN>`) agree with the carcass, so
    the divergence is sub-statement arg passing, not structure. One genuinely-unresolved statement (@TODO):
    the else-branch single-byte original_color write (mov byte[tmp],64h; mov [original_color],cl) - written as
    a full color ctor; exact source form (a channel setter?) unknown. report-changes vs prior build: only the
    single 81.55->84.16 self-improvement, no regressions.
- game_core::weapon_recoil_params::weapon_recoil_params(binary_config_value const&) -> STATE[100%|DONE] -> PR #147 (regressions: none)
  - config ctor, same family as weapon_dispersion_params #136 / character_dispersion_params::load #135:
    partial 9-member init list (0.0f except additive_recoil_time=epsilon_3; omits 0x08/0x0c/0x20/0x30) +
    value_exists-guarded (float)cfg["name"] reads in member order. The two min/max-angle members use a nested
    `&&` guard: `if ( value_exists(min) && value_exists(max) ) { min=...; range=(float)cfg[max]-min; }`.
    Base structure byte-identical to target (26 statements, 0x306 bytes). No unconditional override (unlike #136).
  - Anchor: extended use_game_core_weapon_recoil_params to construct + escape the config-ctor instance.
  - Stacked on #145 (get_foot_fixed_transform). 55 report-changes regressions are all unrelated COMDAT/template
    relink churn (deleting dtors, btXxx, boost::asio, stlp_std, intrusive_ptr<...>); no matched source regressed.
- game_core::{pistol_weapon_core_aimed_idle_state, double_barreled_weapon_core_aimed_idle_state}::{ctor,
  weapon_and_hands_expression, get_weapon_lexeme_pair, weapon_core_state_cook_template<T>::new_object} ->
  STATE[ctors+new_objects 100% DONE, get_weapon_lexeme_pair 99.92% DONE, weapon_and_hands_expression 85.65% PARTIAL]
  -> PR #154 (regressions: none)
  - STACKED on #153 (the idle-state siblings). GROUPED: 8 fns, aimed-idle counterparts and byte-for-byte structural
    twins of the idle classes (#153) - only the string captions, the ammo-index expression (pistol `==0`, dbl
    direct), the weapon_state loop bound (2 vs 3) and the class size (0x158 vs 0x168) differ; base class is
    weapon_core_aimed_state_base (m_weapon_animations @0x138). Verified vs report.json: pistol ctor/new_object 100.0,
    get_weapon_lexeme_pair 99.91803, weapon_and_hands 85.64815; dbl ctor/new_object 100.0, get_weapon_lexeme_pair
    99.92063, weapon_and_hands 85.64815. Both get_weapon_lexeme_pair DONE residual VERIFIED genuine LTCG arg passing:
    weapon_core::ammo_in_magazine is standalone in BOTH indexes (target @0x9b270 takes `this` in eax via a link-time
    custom calling convention), so the one residual instr is `mov eax,[..+128h]` (target) vs `mov ecx,[..+128h]`
    (base) at offset 0x1a - the permitted call-boundary arg-passing class, NOT register-allocation noise (confirmed
    by diffing both --view base/target: everything else byte-identical incl. the ICF-folded trailing empty-fn call).
    Both weapon_and_hands PARTIAL residual = whole-program inline-vs-call of operator+<animation_lexeme,animation_lexeme>
    (standalone in BOTH indexes: target 0xb42f0, base 0x8b900; base keeps the call, target inlines) - same class as #151/#153.
  - REVIEW: restored BOTH get_weapon_lexeme_pair carcasses (a non-100% DONE keeps the FUNCTION BODY per house style,
    mirroring the #153 review); reworded both getter STATE lines to name the verified eax-vs-ecx custom-calling-
    convention cause instead of bare "__thiscall register-allocation choice"; added this PROGRESS line (#154 shipped
    without one, like #148/#149/#151/#153). No logic change; report.json unchanged (no rebuild).
- game_core::survarium::get_weapon_lexeme_pair_impl (free fn, target rva 0x7a13e0) ->
  STATE[None|PARTIAL] -> PR (stacked on #154 / match/game_core-pistol-double-barreled-aimed-idle-state)
  (regressions: none)
  - The big idle/aimed-idle lexeme builder, until now a link-only STUB. Full 1:1 body reconstructed: builds
    MAIN animation_lexeme_parameters ([ebp-58h]) via resource_ptr ctor + setter chain (.animated_object,
    .playback_type, .bones_mask(2), .weight_synchronization_group_id(offset_only=0x80), .weight_interpolator,
    .time_scale, .time_synchronization_group_id), constructs the main animation_lexeme ([ebp-0E0h]); then the
    OFFSET animation_lexeme_parameters ([ebp-138h]) whose 4th (time_driving) arg is the target's
    `group != u32(-1) ? &main_lexeme : NULL` select (the `sub edx,-1; neg; sbb edx,edx; and edx,&main` idiom -
    reproduced from source, byte-identical), chain (.start_animation_interval_id/_time from playback_state,
    .bones_mask(body_part_whole_body_but_hands=-3), .weight_synchronization_group_id(all_but_offset=0x100)),
    a `if ( ! offset_params.time_driving_animation() ) { ASSERT; .time_scale(time_scale); }` guard, the offset
    animation_lexeme ([ebp-1C0h] local), and `return weapon_lexeme_pair( offset_lexeme, main_lexeme )` (copies
    offset->pair@0x00, main->pair@0x84 - verified from the two copy-ctor calls + reverse-order dtors).
  - The two named local enum constants (all_but_offset=256, offset_only=128) declared as a local `enum` and used
    where the 0x100/0x80 immediates appear. body_part_whole_body_but_hands = -3 for the offset bones_mask.
  - L40 is a lone 4-byte `mov byte[ebp-N],0` dead store (target <0x4>, NO lea/call) = an unused `bool`, NOT an
    ASSERT (an ASSERT emits lea+call = <0xc>; first build's ASSERT over-produced - swapped to `bool dummy=false`).
  - Residual is whole-program inline-vs-call of the trivial inline-in-class animation_lexeme_parameters setters:
    TARGET keeps animated_object/playback_type/bones_mask/weight_interpolator/start_animation_interval_id
    out-of-line (rvas 0x098d90/0x09b390/0x09ce50/0x09cc90/...); our /GL LTCG inlines ~6 of them, shifting the
    whole [ebp-XX] layout and shortening the body (base ~415 vs target 503 bytes), so objdiff cannot pair it ->
    None. Same unsteerable class as scheduler::on_frame / operator| / fixed_string; the setters live in the
    out-of-scope `animation` module headers as inline COMDATs, so the "move out-of-line" lever is engine-wide.
  - All callees (animation_lexeme_parameters::*, animation_lexeme::*) are in the already-compiled `animation`
    module - only decls needed (mixing_animation_lexeme_parameters.h). NO game_core STUB callee blocked it.
  - Anchored transitively (weapon_core_idle_state::get_weapon_lexeme_pair, already anchored, calls it).
  - 53 regressed / 58 improved in report-changes = the documented relink ICF/vcall-fold churn (vector-deleting
    dtors, thunks, btXxx, stlp_std); none are this fn or its callers.

- game_core::weapon_core::initialize_weapon_logic (target rva 0x597020) -> STATE[95.69%|INPROGRESS] -> PR #156
  (regressions: none)
  - STRESS-TEST unit (the "how hard can it go" experiment). The weapon-logic FSM builder: ~10 `m_logic->add_state`
    + 72 `m_logic->add_transition( from, to, boost::bind( &weapon_core::<predicate>, this[, weapon_target_enum] ) )`,
    plus the chamber-a-round states/transitions guarded by the conditional weapon flags. The single
    `fixed_size_allocator::finalize_impl` is the compiled-out ASSERT. Full structurally-faithful body written and
    it scores 95.69% on the first rebuild - a strong result for a 122-statement boost::bind-heavy function whose
    callees (`ai::fsm`, `boost::bind`/`boost::function`) are in-scope/template, so unlike #155 it DOES score.
  - Residual (the ~4.3%): the per-transition `boost::function` construction has two instantiation SHAPES in the
    target ("Form A" = named function0 local + assign_to vs "Form B" = converting temporary ctor); one source
    idiom emits only one form, so a block of transitions mismatches on the function0 ctor/assign shape. This is a
    template-instantiation-shape limit, NOT LTCG-arg-passing -> correctly INPROGRESS, not a banked DONE. Next step:
    identify the two original idioms and split the body into the two regions.
  - The matcher process died on a transient API ConnectionRefused right after the rebuild, before reading the %,
    restoring the carcass, or committing. The orchestrator finalized STATE/.md/this ledger line from report.json
    (no rebuild; body unchanged). Carcass restoration on this non-100% function flagged for the reviewer.
- game_core::legs_ik_processor::process_leg (target rva 0x6eafa0) -> STATE[78.82%|PARTIAL] -> PR #157
  (regressions: none)
  - Stacked on #156. Full two-bone IK math: all 58 statements / control flow / IK operands verified against asm
    (is_similar early-out, both debug-draw blocks, the alpha-angle chain, the get_relative_matrix writeback tail).
    get_rotation_matrix/change_matrix_orientation defined locally (out-of-line callees); anchored via friend
    use_game_core_legs_ik_processor_process_leg. Verified vs report.json: 78.822136.
  - REVIEW (claude, no rebuild): the matcher banked the dominant 447 [ebp-N] slot renames as "LTCG/slot class /
    declaration-reorder tail". That is wrong: the target carcass has THREE [1] braced IK-stage blocks (srclines
    205/231/245) whose locals are PDB-tagged <1>, while the base structure has ZERO [n] block-opens - the three
    stages were written FLAT. That missing bracing is the SOURCE cause of the slot renames (function-scope vs
    block-scope allocation) = MATCHING.md check-5 structure divergence, a recoverable matching problem, NOT LTCG.
    Reworded the STATE line + .md to name the missing-brace cause and the concrete next step (brace 205/231/245,
    re-diff); added this ledger line (#157 shipped without one). No logic change; report.json unchanged (no rebuild).
    FLAGGED for a faster machine: brace the three IK stages so the <1> locals are block-scoped.

- game_core::legs_ik_processor::process_leg (RESTRUCTURE) -> STATE[78.81% -> 80.96%|PARTIAL] -> PR (base match/game_core-legs_ik_processor-deep)
  - Stacked on match/game_core-legs_ik_processor-deep. Executed the flagged brace work: wrapped the three IK
    stages in `{ }` at srclines 205/231/245 so the <1> locals are block-scoped, moved the knee/leg recompute to
    be the FIRST statement INSIDE the next block, and reordered block 2 to knee,leg,orig_knee_dir,rot,change
    (the old flat source took original_knee_dir before the leg recompute, using a stale leg_obj). Base structure
    dump now shows exactly THREE [1] block-opens (was ZERO); the [ebp-N] slot-rename storm collapsed.
    rebuild.py (no module arg): report.json 78.81% -> 80.96%, `0 regressed, 1 improved`. No surrounding regression.
  - Final residual (NOT bracing, not pursued): (1) up_leg_obj dir-math reads a separate [ebp-150h] working slot
    the target keeps while change_matrix_orientation mutates [ebp-180h] - a compiler in/out lowering copy, not a
    named local (no second float4x4 in carcass LOCALS); (2) get_angle inline-vs-call (STUB vs out-of-line);
    (3) the get_skeleton()->get_root_bones_count temp-roundtrip sibling get_foot_fixed_transform (84%) also shows;
    (4) is_similar epsilon/ptr + operator*/-/^ call-boundary temps. Trail in process_leg.md (Restructure pass).

- game_core::legs_ik_processor (ALL remaining functions, one unit) -> see per-fn STATE below -> PR (base chore/remove-slow-machine-notes)
  - First pass implementing + anchoring every remaining STUB in legs_ik_processor.cpp in one branch/commit/PR.
  - s_ik_legs_debug_draw_cc / s_ik_foot_capsule_radius_cc / s_ik_legs_rot_axis_cc / s_ik_adjust_hip_position_cc
    (the last NOT in the dispatch list; found referenced by process()) -> console_commands::cc_bool/cc_float
    static initializers (console_command.h). report.json leaves all `dynamic initializer`/`atexit destructor`
    thunks UNSCORED (fuzzy_match_percent None, NOT 0% - the universal cc name-pairing artifact, same as the
    accepted dispersion/bullet cc inits); cc_float byte-identical to target, cc_bool differs only by
    register-vs-stack ctor arg passing (LTCG). Effectively DONE.
  - leg_params::leg_params -> 100%  | leg_params::activate -> 100%  | leg_params::tick -> 97.42% (1 extra LTCG frame slot)
  - legs_ik_processor::legs_ik_processor -> 100% (key: transition_time_calculator() default m_value=0.1f)
  - legs_ik_processor::activate -> 100%  | legs_ik_processor::tick -> 100%
  - ~legs_ik_processor -> 85.71% (ICF-folded member-dtor this-ptr setup; DELETE(m_drawer) is the only real source)
  - process -> 90% (full structure; residual = per-call get_skeleton() temp-spill 0xC frame shift + R_ASSERT report half)
  - get_additional_length -> 65.38% PARTIAL unchanged (documented operator| inline-vs-call LTCG; re-verified, stays)
  - ik_processor.h: m_last_time_in_ms private->protected (no bytes). legs_ik_processor.h: transition_time_calculator
    ctor/tick, leg_params::is_on_ground filled in. temp_include_all.cpp: anchored activate/process/tick (both classes).
  - FLAGGED for the deeper second pass: process()/dtor capped by anchor-observation + LTCG codegen (get_skeleton spill,
    ICF dtor fold); revisit once private-method temp_include anchors are removed so real callers observe the objects.
  - REVIEW (no logic change, no rebuild): verified every per-fn % against report.json (leg_params::tick 97.42308,
    legs_ik ctor/activate/tick 100, dtor 85.71429, process 90.00284, get_additional_length 65.375 - all confirmed).
    Corrected the "0%" wording for the cc dynamic-init/atexit thunks -> they are report.json `None` (unscored), the
    universal cc name-pairing artifact (every cc init in the codebase is None), not a 0% score (.cpp comment, this
    file, the per-fn .md). Trimmed lean-policy overruns on the two clean 100%|DONE fns (leg_params::activate,
    legs_ik_processor ctor) to a bare STATE line (rationale already in the .md). Checked §2a: process()'s slot
    "storm" is a UNIFORM 4-byte frame offset (3B0 vs 3AC) from the get_skeleton() spill, NOT a brace/structure
    divergence - the if/else-if ladder braces pair cleanly base-vs-target; the R_ASSERT(success) report half and
    the dtor's two `this`-ptr setups for ~fermi_interpolator (offsets 0x7C/0x70) are genuine call-boundary/ICF
    artifacts, correctly PARTIAL with carcass preserved. No fixable cap found for the second pass beyond what is
    flagged. No code logic touched.
- game_core::legs_ik_processor (DEEP second pass: anchor-removal) -> all %s UNCHANGED -> PR (base match/game_core-legs_ik_processor-rest)
  - GOAL: the second-pass flag above hypothesised that fake NULL-cast observation anchors distorted LTCG/DSE codegen
    for the private methods, capping them below 100%. Removed the redundant fake-observation anchors so the REAL
    internal call chain (process() anchored via a real `legs_ik_processor processor;`) keeps them alive transitively.
  - REMOVED from temp_include_all.cpp (+ their IncludeAll dispatch lines):
    use_game_core_legs_ik_processor_get_foot_fixed_transform, use_game_core_legs_ik_processor_process_leg,
    use_game_core_get_additional_length, use_game_core_legs_ik_processor_leg_params.
  - REACHABILITY (verified - every fn still SCORES in report.json, none dead-stripped):
    get_foot_fixed_transform/process_leg <- process(); get_additional_length <- process_leg; all leg_params members
    <- the real processor instance (ctor/activate/tick/setters via the public processor methods).
  - BEFORE -> AFTER (rebuild, no module arg; numbers from report.json):
    get_foot_fixed_transform  84.23 -> 84.23   (unchanged)
    process_leg               78.81 -> 78.81   (unchanged)
    get_additional_length     65.38 -> 65.38   (unchanged; --view diff byte-identical, still the one operator| inline)
    process                   90.00 -> 90.00   (unchanged)
    ~legs_ik_processor        85.71 -> 85.71   (unchanged)
    leg_params::tick          97.42 -> 97.42   (unchanged)
    set_*_transition_time     83.69 -> 83.69   (unchanged)
    set_heel/toe_on_ground(processor) 98.84/98.59 -> same; all 100% fns stayed 100%.
  - RESULT: VERIFIED NEGATIVE - fake observation was NOT the cap. The residuals are genuine: get_additional_length =
    per-call-site LTCG inline of operator| (diff byte-identical before/after); process_leg = the documented three-block
    bracing (srclines 205/231/245 written flat) PLUS call-boundary LTCG; get_foot_fixed/process/dtor = call-boundary/
    ICF artifacts. The removed anchors were redundant clutter (good to drop) but never distorted codegen.
  - Regressions: none. report-changes shows 11 regressed / 15 improved, all 100<->0 trivial ICF/COMDAT-fold churn
    (empty_stub, float3::float3(void), booby_trap thunks, trivial dtors) - the standard net-neutral relink shuffle;
    NO legs_ik_processor (or any matched game_core) function regressed.

- legs_ik_drawer: 5-function same-class cluster (thin debug-draw forwarders over
  render::debug::renderer). One rebuild (incremental, 1m32s compile+link / 5s delink).
    draw_cross          100%   DONE
    draw_line_capsule   100%   DONE
    draw_solid_capsule  79.43% PARTIAL  (LTCG call-boundary reg-vs-stack int-arg assignment)
    draw_leg            73.36% PARTIAL  (LTCG draw_origin float pass xmm0 vs fld/fstp x4)
    draw_origin         62.88% PARTIAL  (LTCG draw_origin float pass xmm0 vs fld/fstp)
  Source is correct & minimal for all five (the two 100% siblings use the identical
  forward pattern); the three partials' sole residual is the callee's whole-program
  float/int arg-passing convention, not steerable from the drawer. float4_pod::xyz() shows
  in target as a COMDAT-folded `...::finalize_impl` thunk (delinker misname) = matrix.c.xyz().
  Regressions: none (only unrelated delinker COMDAT-fold churn; no matched game_core fn moved).

- breath_vibration_calculator: 3 remaining STUBs (the ctor/setter/dtor were a prior unit).
  One rebuild.
    tick                          94.23% PARTIAL  (/Od frame-slot reg-alloc churn at the
                                                   m_user->local_time vcall; structure 1:1)
    `dynamic initializer for
     's_enable_breath_vibration_cc'` None  DONE    (file-static cc_bool init/atexit ICF-
                                                   folded, no standalone symbol; body correct,
                                                   mirrors dispersion's s_dispersion_enabled_cc)
    initialize_logic              STUB   INPROGRESS (large ~0x3bc; needs breath_state
                                                   SUBCLASSES + boost::bind/function machinery +
                                                   header move to private (target AAE) +
                                                   resolving the duplicate breath_state defn.
                                                   Full reconstructed body in the cpp STATE
                                                   comment + _tick.md.)
  Added the tick body (FPU vibration math, fsm tick, breath_state vtable dispatch, max/min
  clamp) + the s_enable_breath_vibration_cc static (the dynamic initializer) + a `calc.tick(...)`
  reachability call to the existing breath anchor. Regressions: none (only unrelated ICF-fold
  representative churn; no matched game_core fn regressed).

- game_core::oneliners-batch1 (18 trivial one-liners across 10 headers) -> PR #TBD (regressions: none)
  One rebuild (706s / ~11.8min wall, exit 0, no post-build stall). Results:
    damage_protector::damage_protector()           100%  DONE     (already matched; marker fixed)
    damage_protector::~damage_protector()          100%  DONE     (already matched; marker fixed)
    artefact_container_core::use_info               None  PARTIAL  (standalone in EXE, but TARGET
                                                                    is FRAMELESS `mov eax,lit;ret 4`
                                                                    vs our /Od framed body - build-
                                                                    flag frame omission for this
                                                                    `this`-unused leaf, not source-
                                                                    steerable; literal/ret correct)
    booby_trap_core::get_speed                      None  PARTIAL  (same frameless-vs-/Od-framed:
                                                                    target `fldz;ret`, ours framed)
    inventory_holder::inventory() [non-const]       None  PARTIAL  (trivial-accessor LTCG inline-
                                                                    vs-call; verified inlined into
                                                                    anchor as mov[this+8], correct)
    inventory_holder::inventory() const             None  DONE     (ICF-folds w/ non-const)
    inventory_holder::scheduler()                   None  PARTIAL  (LTCG inline-vs-call, mov[this+4])
    weapon_user_animations_selector::set_animations None  PARTIAL  (LTCG inline-vs-call setter)
    base_project::register_named_object             None  PARTIAL  (LTCG inline-vs-call; verified
                                                                    inlined as map::operator[]+store)
    base_project::register_object_to_resolve        None  PARTIAL  (LTCG inline-vs-call; inlined
                                                                    vector push_back fast-path)
    collision_geometry::cast_to_collision_geometry  None  DONE     (ICF-fold `return this`)
    player_logic_base_state::is_ready_for_transition None DONE     (ICF-fold `return true`)
    collision_sensor::on_inside                     None  DONE     (ICF-fold empty; moved to
                                                                    protected: to match `MAE`)
    collision_sensor::on_leave                      None  DONE     (ICF-fold empty; protected `MAE`)
    collision_sensor::on_enter                      None  DONE     (ICF-fold empty; protected `MAE`)
    collision_sensor::on_objetcs_loosed             None  DONE     (ICF-fold empty; protected `MAE`)
    interactive_object::assign_game_ui              None  DONE     (ICF-fold empty `ret 4`)
    interactive_object::cast_weapon_core (×2)       None  DONE     (ICF-fold `xor eax,eax;ret`)
  Fold families proven populated in base index (mov al,1;ret=25, ret 4=68, xor eax,eax;ret=16,
  empty this-frame=100), so the None|DONE bodies ARE emitted/byte-correct. Anchors added/extended
  in temp_include_all.cpp: use_game_core_{inventory_holder,weapon_user_animations_selector,
  base_project,booby_trap_core_get_speed}; use_info via a qualified call in use_artefact_container_core.
  collision_sensor's 4 overrides moved public->protected to match the `MAE` mangling.
  Regressions: none (15 report-changes "regressed" are all ICF fold-rep churn - boost storage,
  dtors, thunks, empty_stub; none touch this batch's 10 units).

- character_dispersion_calculator STUB cluster (3 fns, ONE rebuild, 701s, watchdog not engaged):
    character_dispersion_calculator::character_dispersion_calculator  100%  DONE     (member-init list; ctor consts read from EXE: smoothing=5.0, aiming=1.0, rest 0/NULL; core::noncopyable base ctor folded call)
    character_dispersion_calculator::set_character_dispersion_params  100%  DONE     (m_params=arg; if(m_params){3x = m_params->idle_multiplier})
    character_dispersion_calculator::tick                           99.67%  PARTIAL  (every instr/offset/call/const byte-identical; sole diff = target sub esp,1Ch vs base 18h - one unused /Od frame slot; frame-slot churn class, like breath_vibration_calculator::tick)
  Anchor: use_character_dispersion_calculator extended to escape &calc (constant-ctor DSE observe).
  report-changes: 0 regressed, 3 improved (30.32->100 / 22.86->100 / 25.26->99.67), 0 removed/added.

- weapon_core_reload_state_base + weapon_core_chamber_a_round_state_base (6 fns, ONE rebuild, 103s, watchdog not engaged):
    weapon_core_reload_state_base::weapon_core_reload_state_base           100%  DONE
    weapon_core_reload_state_base::initialize                              92%   PARTIAL  (LTCG inline-vs-call of trivial round_is_chambered(): target keeps it standalone @0x09b360 `call`, our /GL inlines `mov cl,[+48Eh]`; every other instr/offset/&&-short-circuit byte-identical; chamber_a_round_on_reload inlined on BOTH sides so it matches)
    weapon_core_reload_state_base::on_animation_end_impl                   100%  DONE     (m_weapon.instant_reload(); result=true)
    weapon_core_chamber_a_round_state_base::weapon_core_chamber_a_round_state_base  100%  DONE
    weapon_core_chamber_a_round_state_base::initialize                     100%  DONE     (base::initialize() only)
    weapon_core_chamber_a_round_state_base::on_animation_end_impl          100%  DONE     (m_weapon.instant_chamber_a_round(); result=true)
  Both ctors: : weapon_core_animation_end_aware_state(weapon,true) + body m_animation_timescale=arg (@0x140, BEFORE) then m_body_part_mask_for_user=body_part_whole_body_but_hands (-3, @0x130). Filled inline getter weapon_core::chamber_a_round_on_reload() = m_chamber_a_round_on_reload (@0x48F). Access fixes: ctor IAE->protected, initialize MAE->protected, serialize/deserialize/on_animation_end_impl EAE/EBE->private. Two new anchors (concrete_{reload,chamber}_state derived stubs overriding pure weapon_and_hands_expression) + friend decls + dispatcher calls in temp_include_all.cpp.
  Regressions: none (10 report-changes "regressed" are ICF fold-rep churn - boost storage, float3/size_policy ctors, scaleform dtor, interlocked_decrement, sun_cascade, vcall thunks; none touch this unit's source).

- weapon_core_fire_state_base (6 fns, ONE rebuild, 698s, watchdog not engaged):
    weapon_core_fire_state_base::weapon_core_fire_state_base   100%   DONE     (: weapon_core_animation_end_aware_state(weapon,true); body m_animation_timescale=arg @0x140, m_playback_type=play_cyclically @0x144; NO m_body_part_mask_for_user store - unlike reload/chamber)
    weapon_core_fire_state_base::initialize                    99.71% DONE     (every instr matches; sole diff = 4-byte /Od frame size base sub esp,5Ch vs target 58h cascading slot numbers - stack-slot packing. base::initialize(); set_animation_callback("shoot",this,bind(&on_shot_event,this,_1)); m_playback_type=playback_enum(get_bullets_in_queue()<=1); ASSERT; *m_is_firing_ptr=true)
    weapon_core_fire_state_base::execute                       80.91% PARTIAL  (LTCG inline-vs-call of EMPTY weapon_core_base_state::execute(): target `call` @0x087f80, base inlines empty body; both keep standalone so per-call-site decision; m_animation_has_been_ended=false matches)
    weapon_core_fire_state_base::finalize                      100%   DONE     (base::finalize(); remove_animation_callback("shoot",this); ASSERT; *m_is_firing_ptr=false)
    weapon_core_fire_state_base::on_animation_end_impl         100%   DONE     (animation_player_tick_result=true)
    weapon_core_fire_state_base::on_shot_event                 95.26% PARTIAL  (logging residual: every ASSERT/control-flow/instant_fire byte-identical; diff entirely inside LOG_ERROR - __LINE__ push 4Bh vs 52h, __FILE__/__FUNCSIG__ string relocs, boost::function log_callback ctor reorder. same class as animation_analysis_result_cook::translate_query)
  Access fixes (read from target mangling): ctor IAE->protected; initialize/execute/finalize/on_animation_end_impl/on_shot_event all MAE->protected (header had them public). Filled inline getter weapon_core::get_bullets_in_queue() = m_bullets_in_queue (@0x47C). New anchor use_game_core_weapon_core_fire_state_base (concrete_fire_state derived stub overriding pure weapon_and_hands_expression) + include + friend decl + dispatcher call in temp_include_all.cpp.
  on_shot_event boost::bind ICF-folds onto the weapon_core_animation_end_aware_state bind<> rep (delinker names that symbol) but assign_to<> + vcall'{36}' are on weapon_core_fire_state_base - source binds &weapon_core_fire_state_base::on_shot_event (virtual, hence vcall thunk).
  Regressions: none (21 report-changes "regressed"/21 "improved" are symmetric ICF fold-rep churn - empty_stub, dtors, thunks, resource_ptr/buffer_string/interlocked trivials; none touch this unit's source).

- weapon_core batch2 (6 fns, PR #180, base match/game_core-weapon_core_fire_state_base - NOT stacked):
    weapon_core::instant_idle_end                  100%  DONE  (m_is_idle = false;       store byte[+0x492],0)
    weapon_core::instant_toggle_start              100%  DONE  (m_is_toggling = true;    store byte[+0x494],1)
    weapon_core::instant_toggle_end                100%  DONE  (m_is_toggling = false;   store byte[+0x494],0)
    weapon_core::backward_recoil_time_calculator   100%  DONE  (return calculator_functor( this, &weapon_core::computed_backward_recoil_time ); FastDelegate6-from-member-fn)
    weapon_core::horizontal_recoil_time_calculator 100%  DONE  (return calculator_functor( this, &weapon_core::computed_horizontal_recoil_time ))
    weapon_core::vertical_recoil_time_calculator   100%  DONE  (return calculator_functor( this, &weapon_core::computed_vertical_recoil_time ))
  Member offsets read from target weapon_core.h: m_is_idle @0x492, m_is_toggling @0x494. instant_* setters reached via the existing weapon_core anchor (use_game_core_weapon_core_base_state's weapon_core weapon;) - no new anchor needed. The three non-virtual recoil getters were absent from base/index.jsonl (0% scored, symbol present) until anchored: added 3 getter calls on the existing `weapon` instance in use_game_core_weapon_core_base_state, escaping each result via example_callback so LTCG keeps the out-of-line bodies. All six 100.0 in report.json on the first/second rebuild.
  Regressions: none (12 report-changes "regressed" 100->0 are branch-from-clean-base accounting artifacts - boost storage, empty_stub, dtors, thunks, resource_ptr/speedtree; the 3 getters show in "improved" 0->100; instant_* setters unchanged; none touch this unit's source).
- weapon_user_animations_selector state accessors (5 fns) -> PR #176 (--base fire_state_base, NOT stacked; report-changes "100->0" are branch-from-clean-base accounting artifacts, not regressions):
    weapon_user_animations_selector::is_in_jump                 100%   DONE     (current_state().id() == type_jump)
    weapon_user_animations_selector::is_sprinting              100%   DONE     (current_state().id() == type_sprint)
    weapon_user_animations_selector::is_ready_to_be_deactivated 100%  DONE     (current_state().is_ready_to_be_deactivated())
    weapon_user_animations_selector::current_state             70.26% PARTIAL  (verified vs target asm rva 0x584a30: base sub esp,0Ch MISSING target's extra lea[ebp-8];call <operator*>(ICF->vec_begin);mov[ebp-4] @0x18 - unidentified deref idiom on the cast result. empty_stub ASSERT @L81 + [+10h] read + return *result all matched. NOT LTCG arg-passing -> stays PARTIAL.)
    weapon_user_animations_selector::get_current_state_id      71.5%  PARTIAL  (same root cause; target rva 0x584a00 chains through TWO extra temps frame 0x14 vs base 0x0C; +10h/+20h reads + control flow match. ALSO appears in PR #175 as an inlined dependency - dedup at land time.)
  Access fix: current_state moved public->private (target mangling ABE). Anchor use_game_core_weapon_user_animations_selector extended with member-fn-pointer addresses (&self::get_current_state_id etc.) so the accessors stay standalone COMDATs instead of LTCG-inlining into the anchor.
- weapon_core_shotgun_reload_state_cook (4 fns, PR #179, NON-stacked off fc3aadf9):
    weapon_core_shotgun_reload_state_cook::~weapon_core_shotgun_reload_state_cook  100%    DONE     (empty body {}; dtor calls COMDAT-folded inplace_unmanaged_cook::~ - keeping unmanaged_cook base matched it)
    weapon_core_shotgun_reload_state_cook::destroy_resource                        100%    DONE     (static_cast<weapon_core_shotgun_reload_state*>(resource) downcast -0x18; wpn_state->~weapon_core_shotgun_reload_state())
    weapon_core_shotgun_reload_state_cook::deallocate_resource                     55.64%  PARTIAL  (VOSTOK_FREE_IMPL(g_allocator, (resource_base*&)buffer); g_allocator pointer no-deref + free_helper<doug_lea_allocator,resource_base> overload all correct. RESIDUAL: target keeps free_helper wrapper OUT-OF-LINE `call`, base INLINES it - inlines the if(!pointer)return null check + calls free_helper_impl directly, spills allocator to [ebp-4]. inline-vs-call class, same as weapon_core_cook::delete_resource. NOT LTCG-banked-as-done)
    weapon_core_shotgun_reload_state_cook::allocate_resource                       41.38%  PARTIAL  (stmt36 return mutable_buffer(MALLOC(0x140),0x140) shape correct - base out-of-lines uint2::uint2 ctor vs target inlining [edx]=ptr;[ecx+4]=0x140. stmt35 ASSERT recovered as compiled-out half only: target je past an if(x){copy in_query 600 bytes by value + 2nd ASSERT_check} - a validator-call assertion whose UNKNOWN_EXPRESSION is unrecovered. NEXT STEP: sibling weapon_core_inactive_state_cook::allocate @target 0x58ef60 is byte-identical modulo malloc size 0x138)
  Access fix: on_subresources_ready public->private (AAE) per target mangling. Anchor use_weapon_core_shotgun_reload_state_cook (static + register_cook) in temp_include_all.cpp, mirrors use_victory_item_core; emits vtable -> anchors all virtuals.
  Verified vs report.json: dtor 100.0, destroy_resource 100.0, deallocate_resource 55.642857, allocate_resource 41.38095.
  Regressions: none (report-changes "100->0" for weapon_core_cook::*/empty_stub/interlocked_* are a branch-from-clean-base artifact - PR #173 anchors absent here, NOT regressions).
- jump_logic animation methods (4 fns 100% + 2 STUB blocked) -> PR #181 (regressions: none):
    jump_logic::selected_animations  100%  DONE     (return ((jump_logic_base_state*)m_logic->current_state())->selected_animations(buffer,is_third_view,m_owner.look_time_calculator(),weapon_parameters); virtual [vtable+0x18])
    jump_logic::get_animation        100%  DONE     (was 91.93%: inlined get_jump_animation was MISSING its bounds ASSERT - target asm @0x57df30 has a SECOND empty_stub call between get_jump_animation_index and the m_jump_animations index math; recovering ASSERT(UNKNOWN_EXPRESSION_T(index<100)) reproduced the byte sequence -> 100%. NOT LTCG - the reg drift was a consequence of the missing ASSERT.)
    jump_logic::activate             100%  DONE     (m_jumping_direction=get_move_direction(m_user->input()); m_is_jump_from_right_leg=!m_owner.is_right_leg_supporting() - neg/sbb NOT of [m_owner+0x4d])
    weapon_user_animations_selector::look_time_calculator  100%  DONE  (dependency of selected_animations; return look_time_functor(this,&...::look_time_factor_calculator))
    jump_logic::get_move_animation       STUB  BLOCKED  (body verified `return m_owner.animations().get_stand_animation(false,m_jumping_direction*3,is_third_view)` @0x57dee0; get_stand_animation has no definition in tree -> LNK1257 if anchored)
    jump_logic::get_move_look_animation  STUB  BLOCKED  (same, *3+2 @0x57de90; same get_stand_animation blocker)
  Header inline bodies filled to reproduce inlining: jump_logic.h owner/get_jump_direction/is_jump_from_right_leg; selector.h animations()/is_right_leg_supporting(); container.h get_jump_animation() (with bounds ASSERT). get_move_direction free fn given a placeholder `return move_direction_on_site;` (still STUB) so activate links. Anchor use_game_core_jump_logic (member-fn ptrs of selected_animations/get_animation/activate).
  Scope vs PR #176: DISJOINT - #176 fills set_animations()/current_state() access in selector.h/.cpp; this PR fills animations()/is_right_leg_supporting() (left `/* no source */` by #176) + look_time_calculator + container get_jump_animation. No redefinition/conflict; adjacent hunks merge cleanly at land time.
- weapon_core batch3 - predicates + small accessors/events (18 fns, one PR):
    weapon_core::on_reload_started                                  100%  DONE   (empty body; UAE public virtual)
    weapon_core::on_player_model_added                             100%  DONE   (instant_show(); EAE private virtual)
    weapon_core::on_player_model_removed                           100%  DONE   (instant_hide(); EAE private virtual)
    weapon_core::set_ammunition                                   100%  DONE   (m_ammunition = arg; QAE)
    weapon_core::is_ready_to_shoot                                100%  DONE   (ternary m_is_there_chamber_a_round_state?m_is_round_chambered:m_ammo_in_magazine>0 && m_bullets_in_queue!=0 && m_ready_for_fire; QBE)
    weapon_core::unload_chambered_round                          100%  DONE   (3x ASSERT; ++m_ammo_in_magazine; m_is_round_chambered=false; on_unload_chambered_round() vcall @0xB8; QAE)
    weapon_core::must_chamber_a_round_predicate                  100%  DONE   (ASSERT; !m_is_round_chambered && m_ammo_in_magazine!=0 && !m_user_animations_selector.is_in_jump(); ABE private)
    weapon_core::must_chamber_a_round_aimed_predicate            100%  DONE   (must_chamber_a_round_predicate() && is_trying_to_aim(); ABE)
    weapon_core::must_chamber_a_round_aimed_and_animation_ended_predicate 100% DONE (must_chamber_a_round_and_animation_ended_predicate() && is_trying_to_aim(); ABE)
    weapon_core::is_not_trying_to_aim_predicate                  100%  DONE   (!is_trying_to_aim(); ABE)
    weapon_user_animations_selector::is_ready_to_be_deactivated  100%  DONE   (current_state().is_ready_to_be_deactivated(); forced-live by weapon_core::is_ready_to_be_deactivated; filled inline player_logic_base_state getter @0x27)
    weapon_core::on_hand_ik_event                                96.55% PARTIAL (2x ASSERT; active=params.domain_data==9; activate_hand(hand,active,params.callback_time_in_ms); return call_me_again. Residual = extra `xor ecx,ecx` before `cmp eax,9;sete cl` register-zeroing artifact; AAE private)
    weapon_core::on_user_sprint                                  89.72% PARTIAL (left_hand_ik_is_active = is_double_handed() || !user_is_sprinting; activate_hand(left,...,m_last_tick_time_in_ms). Residual = LTCG inline-vs-call of trivial is_double_handed(); MAE protected virtual)
    weapon_core::maximum_ammo_in_weapon                          88.41% PARTIAL (bool chamber_a_round_but_not_on_reload = m_is_there_chamber_a_round_state && !m_chamber_a_round_on_reload; return m_magazine_capacity + that. Residual = bool->int: target byte temp + neg;sbb;neg normalize vs our dword add; QBE)
    weapon_core::must_chamber_a_round_and_animation_ended_predicate 87.47% PARTIAL (ASSERT; must_chamber_a_round_predicate() && current_base_state().has_animation_ended(). Residual = LTCG inline-vs-call of trivial has_animation_ended(); ABE)
    weapon_core::target_and_animation_ended_predicate            85.68% PARTIAL (m_target==target && current_base_state().has_animation_ended(); same has_animation_ended inline-vs-call residual; ABE)
    weapon_core::is_ready_to_be_deactivated                      84.77% PARTIAL (current_base_state().is_ready_to_be_deactivated() && m_user_animations_selector.is_ready_to_be_deactivated(); same getter inline-vs-call residual; UBE public virtual)
    weapon_core::is_trying_to_aim                                66.75% PARTIAL (input=m_user->input(); just_toggled=~m_old_actions_mask & input.actions_mask; return could_be_aimed(*get_user()) && (input.actions_mask&0x80) && !(input.is_sprinting() && (just_toggled&0x200)) && m_user_animations_selector.get_current_state_id()!=type_jump. Residual = LTCG inline-vs-call of player_input::is_sprinting(); ABE)
    weapon_user_animations_selector::get_current_state_id        72.67% PARTIAL (forced-live by is_trying_to_aim; current_state().id(); residual = LTCG inline-decision on current_state)
    weapon_user_animations_selector::current_state              INPROGRESS (forced-live helper; static_cast<player_logic_base_state*>(m_logic.current_state())+ASSERT+return; target adds 2 ref-copies + operator* fold-rep call this form omits)
  Filled inline getters: weapon_core::current_base_state() = *static_cast<weapon_core_base_state*>(m_logic->current_state());
  weapon_core_base_state::is_ready_to_be_deactivated() = m_is_ready_to_be_deactivated (@0x134).
  Includes added to weapon_core.cpp: base_player.h, player_input.h. Include added to weapon_user_animations_selector.cpp: player_logic_base_state.h.
  Access fixes (target mangling): must_chamber_* / is_trying_to_aim / is_not_trying_to_aim_predicate / target_and_animation_ended_predicate -> private (ABE); on_hand_ik_event -> private (AAE); on_player_model_added/removed -> private (EAE); on_user_sprint -> protected (MAE).
  Anchor: extended use_game_core_weapon_core_initialize_weapon_logic (friended) to call is_ready_to_shoot/maximum_ammo_in_weapon/set_ammunition/unload_chambered_round/on_hand_ik_event so they emit + score.
  FIVE of the seven weapon_core PARTIALs share ONE residual class: LTCG inline-vs-call of trivial header getters (is_sprinting/is_double_handed/has_animation_ended/is_ready_to_be_deactivated) - target /Od emits standalone calls (confirmed standalone in the target rich index, absent from base), our /GL inlines them. Same class as reload_state_base::initialize round_is_chambered. Not source-steerable without disabling LTCG: on_user_sprint, must_chamber_a_round_and_animation_ended_predicate, target_and_animation_ended_predicate, is_ready_to_be_deactivated, is_trying_to_aim.
  The OTHER TWO are NOT the inline-vs-call class - distinct micro-codegen residuals, source-shape (not LTCG, not arg-passing), not yet diffed to a clean cause: on_hand_ik_event 96.55% (base inserts an extra `xor ecx,ecx` before `cmp eax,9;sete cl`); maximum_ammo_in_weapon 88.41% (target uses a BYTE `&&` temp + `neg;sbb;neg` bool-normalize before `add`, base uses a DWORD temp + direct `add`). Both correctly tagged PARTIAL, not banked - each is an open source-shape residual a faster machine should re-diff.
  REVIEW (claude): the three weapon_user_animations_selector functions in this PR (is_ready_to_be_deactivated 100%, get_current_state_id 72.67%, current_state INPROGRESS) DUPLICATE PR #176 (match/game_core-weapon_user_animations_selector-state-accessors), which already matched exactly these in the same file -> a land-time CONFLICT. is_trying_to_aim only needs get_current_state_id->current_state reachable, which #176 provides; weapon_core's own is_ready_to_be_deactivated reaches the selector getter transitively. On re-match DROP all three selector edits from this PR (keep only weapon_core's own functions); the selector functions are #176's deliverable.
  Regressions: none (report-changes regressed=0 removed=0).
- jump_logic.cpp batch3 (8 fns, parallel non-stacked off fc3aadf9):
    survarium::get_move_direction                100%   DONE     (4 mask-bit bools fwd/bwd/left/right; two cancel-ifs; nested dispatch tree; objdiff-backend diff 99/99=100%. report.json shows 0.0 only because the free-fn symbol did not pair in the delinked .objs - a pairing quirk, not a regression; both diffs confirm 100%)
    survarium::get_jump_animation_index          100%   DONE     (RE-MATCH f83c820c+: const-qualified the params in the HEADER decl (defn already had them) so it PAIRS - report.json top-level fuzzy = 100.0%. CODE byte-identical, 12-stmt structure exact. The 55.0% diff-VIEW footer is a jump-table-data artifact and must NOT be banked; cross-checked against report.json. no-bounds table -> default:NODEFAULT())
    jump_logic::does_need_land_and_run           100%   DONE     (RE-MATCH f83c820c+: CODE 0x00-0x188 byte-identical, 13-stmt structure exact, kept default:NODEFAULT(). report.json top-level fuzzy = 100.0% and report-changes logged "97.56 -> 100.0 improved". The 63.7% diff-VIEW footer was a jump-table-data/label artifact - the reviewer's "extra trailing block / single-tail" reading came from that footer; a trailing return actually ADDS a bounds check (WORSE). Carcass deleted.)
    jump_logic::set_user                         83.61% PARTIAL  (RE-MATCH f83c820c+: applied the steerable cast-temp fix - bound static_cast<jump_logic_base_state*> to a named local `state` before the call, reproducing the target's "materialize cast into own slot, then push user" order. report.json top-level fuzzy = 83.61% (diff-view footer 50.0% under-counts). Remaining residual is the ai-fsm inline-vs-call WALL (states()/front() out-of-line in target, inlined base; one extra slot) - legit LTCG, not steerable.)
    jump_logic::deactivate                       45.13% PARTIAL  (RE-MATCH f83c820c+: report.json top-level fuzzy = 45.13% (the diff-view footer 7/26=26.9% under-counts; "deactivate" substring also mis-pairs to thread_pool::deactivate_if_oversubscribed at 89.5% - use the full sig). Single statement set_initial_state(states().front()) - no cast/loop to steer; entire residual is the ai-fsm out-of-line-vs-inline WALL (states()=finalize_impl + front()=operator[] out-of-line in target, all inlined base). Legit LTCG.)
    jump_logic::jump_logic (ctor)                BLOCKED          (body verified; anchoring constructs jump_logic -> initialize_logic builds the 3 state subclasses -> their STUB selected_animations force C4716/LNK1257; unblock via PR #181 selected_animations)
    jump_logic::~jump_logic (dtor)               BLOCKED          (body verified, same shape as breath ~; same C4716 trap as ctor)
    jump_logic::initialize_logic                 BLOCKED          (full body reconstructed + compiles; constructing jump_logic_state_* emits vtables -> STUB selected_animations C4716; unblock via PR #181)
  Anchor: new use_game_core_jump_logic in temp_include_all.cpp - free fns get_move_direction/
  get_jump_animation_index called; set_user/deactivate/does_need_land_and_run ODR-used via
  member-fn-ptr address-of (NO instance constructed, avoids the C4716 trap).
  Header: jump_logic.h gained free-fn decls get_move_direction + get_jump_animation_index.
  jump_logic.cpp gained includes jump_logic_base_state.h / base_player.h / ai/fsm.h.
  Regressions: none (report-changes only "improved" entries for this batch's symbols; 0 regressed).
  NOTE: not done in this PR per orchestrator (other PRs): tick, look_time_factor,
  is_jump_finished, landing_predicate, selected_animations, get_animation, activate.
  SKIPPED: get_move_animation, get_move_look_animation (container::get_stand_animation LNK1257).
  Deferred: get_animation_caption, get_move_look_caption (need caption global decls + STUB animations()).
- player_logic_jump_state (7 fns, PR #177):
    player_logic_jump_state::player_logic_jump_state           100%   DONE     (: player_logic_base_state(owner, type_jump=3), m_logic(owner); empty body)
    player_logic_jump_state::initialize                        100%   DONE     (m_logic.activate(); ASSERT(UNKNOWN_EXPRESSION) recovered from compiled-out empty_stub)
    player_logic_jump_state::execute                           100%   DONE     (m_logic.tick())
    player_logic_jump_state::finalize                          100%   DONE     (m_logic.deactivate())
    player_logic_jump_state::set_user                          100%   DONE     (player_logic_base_state::set_user(user); m_logic.set_user(user))
    player_logic_jump_state::is_ready_for_transition           79.48% PARTIAL  (control flow + broken_legs_count==2 byte-exact; residual = two upstream inline-vs-call decisions not steerable from this caller - target keeps intrusive_ptr::operator* AND base_player::is_alive out-of-line, our /GL inlines both; get_user inline-vs-call class)
    player_logic_jump_state::selected_animations               36.84% INPROGRESS (one-line delegation return m_logic.selected_animations(...); BLOCKED on jump_logic::selected_animations still a value-returning STUB - C4716/LNK1257 under LTCG. Placeholder VOSTOK_UNREACHABLE_CODE() keeps the override compilable; match jump_logic::selected_animations first)
  Access fixes (read from target mangling): all virtual overrides mangle E (private) - moved to private: section (ctor stays public, QAE). base_state m_owner/m_user moved private->protected so the derived override reads m_user directly (no byte change to base_state's own fns). Filled inline accessor damage_model::broken_legs_count() = m_broken_legs_count[0]+m_broken_legs_count[1] (reached only here). New anchor use_game_core_player_logic_jump_state in temp_include_all.cpp.
  Regressions: none (~14 report-changes "regressed"/~16 "improved" are COMDAT fold-rep churn from anchoring a new TU; non-stacked branch-from-clean-base accounting; no touched-file logic fn regressed).
- pistol_weapon_core_aimed_fire_state (PR #182, base = match/game_core-weapon_core_fire_state_base; reviewed):
    pistol_weapon_core_aimed_fire_state::ctor                     100%  DONE      (140/140 instrs == target @0x79abc0; ASSERT_CMP_U(count,==,12), 3x/2x nested loops copy animations, trailing ASSERT)
    pistol_weapon_core_aimed_fire_state::get_weapon_lexeme_pair   100%  DONE      (62/62 instrs == target @0x79af30; captions "pistol-aimed_shot"/"pistol-aimed_last_shot")
    weapon_core_state_cook_template<...aimed_fire_state>::new_object  92.08%  PARTIAL  (report.json; LTCG calling-conv of computed_shooting_animation_time_scale - arg in reg + ret xmm0 vs our STUB callee's cdecl push/st0)
    pistol_weapon_core_aimed_fire_state::initialize               99.76%  PARTIAL  (now pairs in report.json; `last_shot ? 1u : 0u` ternary reproduced the final-store boolize byte-for-byte; sole residual = LTCG this-reg at the ammo_in_magazine() call boundary)
    pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression  INPROGRESS  (stub return; addition_lexeme/operator+ machinery; access EBE applied, pairs at 21.43%)
    pistol_weapon_core_aimed_fire_state::get_user_hands_expression    INPROGRESS  (stub return; lexeme machinery; access ABE applied, pairs at 12.78%)
  ACCESS FIXES APPLIED (re-match update to PR #182): set the .h access specifiers to match the target mangled access chars - ctor IAE(protected), initialize MAE(protected-virtual), get_weapon_lexeme_pair ABE(private const), get_user_hands_expression ABE(private const), weapon_and_hands_expression EBE(private-virtual const). Added the sibling-idle anchor-reach pattern (fwd-decl `vostok::use_game_core_pistol_weapon_core_aimed_fire_state()` + `template friend weapon_core_state_cook_template` + `friend ::vostok::use_...`) so the temp_include_all anchor can still reach the now-non-public members. Rebuilt: ctor + get_weapon_lexeme_pair now pair at 100%, initialize at 99.76%; 0 regressions.
  SCAFFOLDING OVERLAP (land-time dedup): this PR also edited weapon_core_aimed_fire_state_base.h (m_animation_timescale/m_playback_type private->protected) and gave weapon_core_aimed_fire_state_base.cpp's on_shot_event/on_aiming_event minimal `return callback_return_type_call_me_again;` stub returns (kept STATE[STUB], carcass preserved) to clear a link-time C4716 - that .cpp belongs to another bucket; reconcile on merge.
- pistol_weapon_core_show_state (5 fns, ONE unit):
    pistol_weapon_core_show_state::ctor                        100%   DONE     (: weapon_core_show_state_base(weapon,is_shown); body m_time_scale=animation_timescale @0x178; ASSERT_CMP_U(count==12); two nested fill loops weapon[2][2][2]@0x148 + user[2][2]@0x168 (managed_resource_ptr=4 bytes); trailing ASSERT)
    weapon_core_state_cook_template<pistol_show>::new_object   100%   DONE     (weapon_anim_length=pinned(animations[0])->length_in_frames(); user_anim_length=pinned(animations[8]); time_scale=weapon/user; placement new(buffer.c_ptr()) ctor(params->weapon,time_scale,animations,count,params->shown). KEY: animations[8] not [1] - the first USER anim, 8 weapon+4 user; target add ecx,20h = 8*4)
    pistol_weapon_core_show_state::get_weapon_lexeme_pair      99.92% DONE     (captions {"pistol-show","pistol-show_empty"}; idx=ammo==0; selected=m_weapon_animations[view][user==crouch][idx]; set_animation_to_wait(selected); get_weapon_lexeme_pair_impl(buffer,id,selected,&m_weapon,m_animation_playback_state,6,m_time_scale,play_once_and_freeze_at_end,instant_interpolator()). Residual: ammo_in_magazine `this` eax vs ecx - call-boundary LTCG)
    pistol_weapon_core_show_state::weapon_and_hands_expression 83.52% PARTIAL  (lexeme_pair=get_weapon_lexeme_pair; hands=get_user_hands_expression(lexeme_pair.offset_lexeme,...); return hands+main+offset. Residual: target uses dedicated `expression operator+(expression&,animation_lexeme&)`/`operator+(expression&,expression&)` overloads NOT present in shared mixing_addition_lexeme_inline.h - base falls back to generic addition_lexeme& operator+. Out-of-file infra gap, same class as weapon_core_idle_state::weapon_and_hands #151 85%)
    pistol_weapon_core_show_state::get_user_hands_expression   72.12% PARTIAL  (if(user_state_id==type_sprint) return expression(weapon_lexeme); else animation_lexeme_parameters(buffer,captions[user==crouch],m_user_animations[view][idx],&weapon_lexeme,&weight_driving_animation).animated_object(m_weapon.get_user()).bones_mask(2).playback_type(play_once_and_freeze_at_end) -> animation_lexeme -> expression. Residual: whole-program inline decision - target keeps get_user + builder setters out-of-line, base inlines; `bones_mask` standalone in target index, absent in base. LTCG inline class)
  Access fixes (from target mangling): ctor IAE->protected; weapon_and_hands_expression EBE->private virtual const; get_weapon_lexeme_pair/get_user_hands_expression ABE->private const (header had all public). Added friend cook_template<T> + use_game_core_pistol_weapon_core_show_state to pistol header; added that anchor friend + fwd decl to weapon_core_state_cook_template.h; new anchor + include + dispatcher call in temp_include_all.cpp.
  Regressions: none (report-changes.json "regressed": []).
- jump_logic_state_{start,landing,inactive} subclasses (ONE unit; UNBLOCK the #148 trap):
    jump_logic_base_state::jump_logic_base_state                100%   DONE      (richer ctor: m_jump_logic(owner)/m_user(0)/m_animation()/m_interval_id_to_wait_for(0xFF)/m_is_jump_finished(false); the richer body stops MSVC from inlining it into derived ctors, so they CALL it as the target does @0xbd230. Mangle IAE -> protected; data members private->protected)
    jump_logic_base_state::set_user                            100%   DONE      (kept; pairs now that the class is anchored)
    jump_logic_state_inactive::selected_animations             100%   DONE      (empty UNREACHABLE frame, never touches the return ptr @0x57ce30; mangle EAE -> private virtual; anchor befriended for the qualified call)
    jump_logic_state_landing::jump_logic_state_landing (ctor)  100%   DONE      (base ctor + m_landing_type=jump_animations_part_land_run(4) @0x57cef0)
    jump_logic_state_landing::finalize                         100%   DONE      (empty body @0x57ce40)
    jump_logic_state_start::jump_logic_state_start (ctor)      100%   DONE      (base ctor + m_preface_interval_ended/m_jump_interval_ended cleared; m_physics_jumped(+0x2C) deliberately left uninitialised @0x6f8bd0)
    jump_logic_state_start::finalize                           100%   DONE      (m_jump_logic.owner().remove_animation_callback("jump",this) + remove_animation_callback(channel_id_on_animation_interval_end=2,this) @0x6f8a00)
    jump_logic_state_start::is_ready_for_transition            100%   DONE      (kept; return m_jump_interval_ended)
    jump_logic_state_{start,landing}::selected_animations      9.43%  PARTIAL   (UNREACHABLE stub that UNBLOCKS the class; real body make_pair(expression(main)+look, main) blocked on animation_lexeme-no-default-ctor + expression/operator+ addition_lexeme machinery, recorded inline)
    jump_logic_state_{start,landing}::get_main_lexeme          3.3/3.22%  PARTIAL (UNREACHABLE stub; large lexeme/interpolator machinery)
    jump_logic_state_{start,landing}::get_look_lexeme          7.07/10.04% PARTIAL (UNREACHABLE stub; large lexeme_parameters machinery)
    jump_logic_state_{start,landing}::initialize              5.83/8.38% PARTIAL  (empty stub; boost::bind + set_animation_callback callback wiring)
    jump_logic_state_{start,landing}::on_interval_end       11.77/12.81% PARTIAL  (UNREACHABLE stub; reads animation_callback_params layout)
    jump_logic_state_start::on_jump_event                     10.52% PARTIAL   (UNREACHABLE stub; player_stamina + animation_callback_params)
  UNBLOCK PAYOFF: jump_logic::{ctor,dtor,initialize_logic} are NO LONGER C4716-blocked (the subclass vtables emit cleanly now that selected_animations returns). They remain BLOCKED-on-other-grounds: restoring initialize_logic @0x57dae0 needs the boost::bind machinery + a not-yet-present survarium::true_predicate free fn (NEW(fsm); NEW each state; add_state x3; add_transition x2; set_initial_state(states().front())). Left for a follow-up boost::bind unit; the C4716 blocker itself is gone.
  KEY ACCESS FIND: every override mangles private (selected_animations/initialize/finalize EAE; get_main/get_look/on_interval/on_jump AAE; is_ready EBE) - only ctor (QAE) + execute (public fold) stay public; base ctor IAE protected + base data members protected. Anchors now CONSTRUCT the subclasses (fabricated owner, never run) to emit the vtable; befriended to call the private helpers.
  Regressions: none real (~14 regressed / ~22 improved report-changes are ICF fold-rep churn - boost storage thunks, vector-deleting dtors, empty_stub, resource_ptr ctors; no jump_logic logic fn regressed).
- weapon_core_shotgun_reload_base_substate + weapon_core_inactive_state_cook (9 fns, ONE unit; "pick up what was blocked" + sibling that frees PARTIALs):
    weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair        100%   DONE       (objdiff unscored - newly emitted, absent from delink unit list; base asm 0x449160 BYTE-IDENTICAL to target 0x798790, 0xb0 bytes incl trailing ICF empty-stub. m_animation_to_wait_for = m_weapon_animations[is_third_view!=0][user_state_id==type_crouch]; return get_weapon_lexeme_pair_impl(buffer,m_animation_id,m_animation_to_wait_for,&m_weapon,*m_animation_playback_state,m_time_synchronization_group,m_animation_timescale,m_playback_type,linear_interpolator(s_aim_transition_time)). KEY: pass the just-assigned member as the impl `animation` arg, NOT a separate selected_animation local)
    weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression   68.18% PARTIAL    (structure identical to target 6-stmt; lexeme_pair=get_weapon_lexeme_pair; user_state_index=user_state_id==type_crouch; override_lexeme=animation_lexeme_parameters(buffer,m_hand_animation_captions[idx],m_user_animations[view][idx],&lexeme_pair.offset_lexeme,&weight_driving_animation).animated_object(m_weapon.get_user()).bones_mask(2).playback_type(m_playback_type); return override_lexeme+lexeme_pair.main_lexeme+lexeme_pair.offset_lexeme. Residual = LTCG inline-vs-call of the trivial animation_lexeme_parameters setters + operator+ in shared animation headers; same class as pistol_show 72%)
    weapon_core_shotgun_reload_state::weapon_and_hands_expression           ~      INPROGRESS (UNBLOCKED - was VOSTOK_UNREACHABLE_CODE placeholder; objdiff unscored. return static_cast<base_substate*>(m_logic->current_state())->weapon_and_hands_expression(...). [reviewer correction] NOT byte-identical: target 0x589db0 materializes the downcast into a NAMED `current` local (two slots: mov [ebp-8],edx; mov eax,[ebp-8]; mov [ebp-4],eax; sub esp,0Ch) while the single-expression form emits one slot (mov [ebp-4],edx; sub esp,8). NEXT: introduce `weapon_core_shotgun_reload_base_substate* current = static_cast<...>(m_logic->current_state());` local and re-diff. The earlier "single /Od scratch temp, not steerable" framing was wrong.)
    weapon_core_inactive_state_cook::~weapon_core_inactive_state_cook       100%   DONE       (empty {}; inplace_unmanaged_cook::~ COMDAT fold)
    weapon_core_inactive_state_cook::destroy_resource                      100%   DONE       (static_cast<weapon_core_inactive_state*>(resource)->~weapon_core_inactive_state())
    weapon_core_inactive_state_cook::create_resource                       91.97% PARTIAL    (base asm 0x457500 instr-for-instr == target 0x58efe0: params=(weapon_state_creation_params const*)raw_file_data.c_ptr(); object=new(in_out_buffer.c_ptr()) weapon_core_inactive_state(params->weapon); set_unmanaged_resource(unmanaged_resource_ptr(object),memory_usage_type(nocache_memory,0x138)); finish_query(result_success). Residual = memory_usage_type/c_ptr COMDAT inline-fold)
    weapon_core_inactive_state_cook::weapon_core_inactive_state_cook        79.07% PARTIAL    (: unmanaged_cook(weapon_inactive_state_class,reuse_false,use_current_thread_id,use_current_thread_id) + register_cook(this). Residual (a) class id: target 0x12C vs our enum's weapon_inactive_state_class=0x103 (resources_classes.h missing ~41 entries; engine-wide); (b) LTCG register calling-convention of unmanaged_cook ctor)
    weapon_core_inactive_state_cook::allocate_resource                     41.38% PARTIAL    (byte-identical shape to shotgun cook allocate, only malloc size 0x138 vs 0x140; ASSERT(UNKNOWN_EXPRESSION) compiled-out validator-by-value not recovered + mutable_buffer ctor inline-fold)
    weapon_core_inactive_state_cook::deallocate_resource                   55.64% PARTIAL    (byte-identical to shotgun cook deallocate; VOSTOK_FREE_IMPL(g_allocator,(resource_base*&)buffer); free_helper out-of-line vs base-inlined wrapper)
  Header edits: weapon_core_shotgun_reload_base_substate.h m_animation_to_wait_for -> mutable (const method writes it). weapon_core_inactive_state.h ctor weapon_core_base_state(weapon)->(weapon,false), has_animation_ended() STUB /* no source */ -> return true; (placeholder so the now-anchored vtable does not trip C4716; out of unit scope, still STATE[STUB]). New anchors in temp_include_all.cpp: use_weapon_core_inactive_state_cook (construct+register), and updated use_game_core_weapon_core_shotgun_reload_state to escape a member-fn-ptr to weapon_and_hands_expression (keeps the whole reachable chain).
  Regressions: none from the final edit (report-changes 0/0/0/0). The first int-delta rebuild's ~16 100->0 are relink ICF/vcall-fold noise (float3/mutex/btHashMap/network thunks/empty_stub/game_console thunks/boost binders) - not this unit's fns or callers.

- PR #188 review re-match (FIX 1 + FIX 2), worktree vostok_3, branch match/game_core-shotgun_base_substate_and_inactive_cook:
    weapon_core_shotgun_reload_state::weapon_and_hands_expression          100%   DONE       (FIX 1: was INPROGRESS/single-slot. Target 0x589db0 uses TWO named locals; the SEPARATE `ai::fsm_state* state = m_logic->current_state();` local forces the extra [ebp-8] slot, then `substate* current = static_cast<...>(state);` is a no-op copy into [ebp-4]. base 0x44eba0 size 68 == target size 68, 26/26 instructions byte-identical. objdiff still `None` - no target COMDAT to pair; verified by rich-index instruction compare)
    weapon_core_inactive_state_cook::allocate_resource                     87.45% PARTIAL    (FIX 2: 41.38->87.45. The second half behind the first ASSERT's bool is a U-form ASSERT, not a dropped validator. `ASSERT_T_U( in_query, raw_file_data, file_exist )` -> (NDEBUG/MASTER_GOLD) `if(identity(false)){ expression_eater(in_query, raw_file_data, file_exist); }`: first identity(false) = the bool/test/je guard, body copies in_query BY VALUE (96h dwords) + raw_file_data + file_exist (add esp,264h). Body now byte-matches target. Reference pattern: ASSERT_U(weight_driving_animation) in weapon_core_aimed_state. RESIDUAL ~12.5% = stmt29 malloc+return: target inlines mutable_buffer(pvoid,u32) as field stores w/ extra [ebp-8] temp (sub esp,0Ch), base COMDAT-folds to out-of-line uint2::uint2 (sub esp,8, push 138h) - inline-depth wall, same as shotgun cook allocate)
  Build: 0 regressed/0 improved/0 removed/0 added vs the FIX2 build (which captured the 41.38->87.45 jump). No header edits in this re-match (cpp-only).

- weapon_user_animations_selector remaining hub + weapon_user_animations_container_cook batch
  (ONE unit, branch match/game_core-selector_remaining_and_container_cook off origin/int/game_core):
    weapon_user_animations_selector::selected_animations        100%   DONE     (return current_state().selected_animations(buffer, weapon_parameters, is_third_view) - virtual at player_logic_base_state vtable+0x18, sret)
    weapon_user_animations_selector::tick                       53.98% PARTIAL  (control flow + all field reads match; 2 unsteerable inline choices - target inlines `is_trying_to_sprint()` => (actions_mask&0x200)!=0 with bool materialize (is_trying_to_sprint is /* no source */ in player_input.h, out of unit files), and target out-of-lines input().is_sprinting() while base inlines it)
    weapon_user_animations_selector::deactivate                 34.91% PARTIAL  (RE-MATCH PR #187 review: L122 unsubscribe_animation_player vtable slot FIXED - reordered base_player.h's two overloaded-virtual decls (pcstr first, enum second) so the (reserved_channel_ids_enum,pcvoid) overload lands at +54h like the target, base now emits mov eax,[edx+54h];call eax. report.json top-level 34.88%->34.91%, 0 regressed/1 improved. Remaining is L124 operator* deref-idiom/arg-eval-order wall (target derefs intrusive_ptr<booby_trap_core>::operator* after pushing args, frame sub esp,8; base uses dummy::nonnull fold, sub esp,10h) - same wall as current_state(), out of scope. base_player.h edit is shared, minimal, reconciles at land.)
    weapon_user_animations_container_cook::ctor                 100%   DONE     (: translate_query_cook(animation_container_class, reuse_true, use_current_thread_id); matched once anchored+observed via use_*_container_cook)
    weapon_user_animations_container_cook::delete_resource      33.38% PARTIAL  (VOSTOK_DELETE_IMPL(g_allocator,resource); same delete_helper inline-shape LTCG wall as weapon_core_cook/victory_item_core_cook/weapon_ammunition_cook)
    create_requests_for_animations                             None   INPROGRESS (body written+verified: ASSERT; for i<requests_count; requests.push_back(create_request(cfg[i], animation_class)) animation_class==0x3d; reads None - .cpp-local free fn whose only caller on_config_loaded is still a stub, fn-pointer anchor does NOT force EXE emission under LTCG and caused ICF churn, so removed. NEXT: match on_config_loaded to keep it)
  cook on_config_loaded / on_animations_loaded / translate_query: left STUB/8.04% (large config + boost::bind + query_resource(s) bodies; on_* are private and only reachable via the still-stubbed translate_query/on_config_loaded chain - deferred).
  serialize/deserialize: stay BLOCKED (udp_match_packet/packet_reader never-compiled cluster).
  New anchor: use_game_core_weapon_user_animations_container_cook (static cook + resources::register_cook); extended use_game_core_weapon_user_animations_selector with member-fn-ptr anchors p4/p5/p6 for tick/deactivate/selected_animations. selector.cpp now #includes base_player.h + player_input.h (tick/deactivate need complete types).
  Regressions: none real - 6 net 100%->None vs clean baseline are all ICF fold-representative flips (btHashMap dtor, asio binder, artefact_lifebone thunks, boost storage2, interlocked_increment) - none authored here; offset by 96 net ->100% from the integration delta. (per the documented "ICF visibility, not source mismatch" convention.)

- jump_logic ctor/dtor/initialize_logic UNBLOCK follow-up
  (ONE unit, branch match/game_core-jump_logic_ctor_initialize off origin/int/game_core):
    jump_logic::true_predicate         (reused) DONE   (NOT redefined: target's external survarium::true_predicate lives in breath_vibration_calculator.cpp 0xbb5c0; initialize_logic @0x1d5 pushes that symbol. jump_logic.cpp forward-declares `bool true_predicate();` and references it - a second defn would be an ODR/LNK conflict. The shotgun true_predicate is a separate file-static.)
    jump_logic::jump_logic (ctor)      100%   DONE     (: m_owner(owner),m_user(0),m_logic(0),m_animated_object(0),m_jumping_direction(move_direction_on_site),m_is_jump_from_right_leg(true){initialize_logic();}. objdiff report.json top-level 100.0%. C4716 state-vtable trap gone on common ground -> anchored by constructing a jump_logic in use_game_core_jump_logic.)
    jump_logic::~jump_logic (dtor)     100%   DONE     (m_logic->clear_transitions(); while(ai::fsm_state* state=m_logic->pop_state()) VOSTOK_DELETE_IMPL(g_allocator,state); VOSTOK_DELETE_IMPL(g_allocator,m_logic). NOTE: game_core_memory.h NEW/DELETE macros are commented out -> call the *_IMPL macros directly. objdiff report.json top-level 100.0%.)
    jump_logic::initialize_logic       60.40% PARTIAL  (NEW(ai::fsm); inactive/start/landing=NEW(jump_logic_state_*)(*this) [decl order inactive,start,landing -> slots -4/-0Ch/-8 match]; add_state x3; add_transition(inactive,start, boost::bind<bool>(&true_predicate)); add_transition(start,landing, boost::bind(&jump_logic::landing_predicate,this)); set_initial_state(states().front()). report.json fuzzy=None - objdiff rejects the boost stored_vtable Absolute relocation and falls back to text-diff 60.4% (166/275). RESIDUAL = the documented ai-fsm out-of-line-vs-inline WALL (LTCG call-boundary, same as deactivate 45.13%/set_user 83.61%): target out-of-lines fsm::states()+front()/operator[] in the final set_initial_state, base inlines them -> base frame 0x18 larger (sub esp,0D4h vs 0BCh), no `push esi`, uses ecx not esi for each transition temp's boost::function::clear() -> all slots renumber. fsm::states() is `inline` in fsm.h; the inline decision is the call boundary - not source-steerable.)
  Anchor: use_game_core_jump_logic now constructs `survarium::jump_logic anchored_jump_logic(sel_owner)` (sel_owner is a fabricated NULL ref, anchor never runs) to ODR-use the ctor->initialize_logic and the dtor; the C4716 note in that anchor was replaced.
  Regressions: none real - 18 net 100%->0 vs the int-delta rebuild are all ICF fold-representative flips caused by the new boost::bind instantiations (empty_stub, vector-deleting-dtor thunks, boost _bi::storage ctors, btPairSet/enemy_target_selector dtors) - none authored here, none of this unit's fns or callers; offset by 40 net ->100% improved (incl. this unit's ctor/dtor + the matched state ctors/add_state/add_transition bodies).
- weapon_core instant/accessor batch4 (ONE unit, branch match/game_core-weapon_core-batch4
  off origin/int/game_core, worktree vostok_3) - all in weapon_core.cpp:
    weapon_core::instant_show         100%   DONE   (m_aimed=false; on_show();  on_show virtual vtable+0xB0)
    weapon_core::instant_hide         100%   DONE   (on_hide();  vtable+0xB4)
    weapon_core::instant_aim_end      100%   DONE   (m_aimed=false; m_aiming_state_transition=true;)
    weapon_core::instant_aim_start    100%   DONE   (if(!is_firing() && !(m_user->input().actions_mask & 0x20)) reset_fire_queue(); m_aimed=true; m_aiming_state_transition=true; -- key: is_firing() accessor inline spills bool to [ebp-1]; aiming bit tested RAW as integer & 0x20, NOT a bool-returning is_aiming() (no neg/sbb normalization). 60.33%->100% after that shape fix.)
    weapon_core::chamber_a_round      100%   DONE   (3x compiled-out ASSERT (finalize_impl) then --m_ammo_in_magazine; m_is_round_chambered=true;)
    weapon_core::get_dispersion       100%   DONE   (return m_dispersion_calculator.get_dispersion();)
  Anchor: appended instant_aim_start/instant_aim_end/chamber_a_round/get_dispersion to existing
  use_game_core_weapon_core_small_setters() in temp_include_all.cpp (instant_show/hide already there).
  No header edits (all access chars already correct: QAE/QAE/UAE/UAE/QAE/QBE). report-changes 0 regressed.
- weapon_core_reload_state (concrete WeaponCore reload-state subclass, ONE unit, 5 fns,
  branch match/game_core-weapon_core_reload_state off origin/int/game_core; resumed an
  interrupted matcher that had written the 5 bodies but the unit did not compile):
    weapon_core_reload_state::weapon_core_reload_state          100%   DONE     (ASSERT_CMP_U(count,==,8); two [view][user_state] double-loops sharing one animation_index counter, weapon then user; trailing ASSERT(UNKNOWN_EXPRESSION). ??0...@@IAE -> protected ctor)
    weapon_core_reload_state::get_weapon_lexeme_pair            100%   DONE     ("weapon-reload"; selected = m_weapon_animations[is_third_view!=0][user_state==type_crouch]; set_animation_to_wait(selected); return get_weapon_lexeme_pair_impl(buffer, id, selected, &m_weapon, m_animation_playback_state, 2, m_animation_timescale, play_once_and_freeze_at_end, linear_interpolator(s_reload_transition_time)). @@ABE private const)
    weapon_core_reload_state::new_object                        86.5%  PARTIAL  (placement new(buffer.c_ptr()) weapon_core_reload_state(params->weapon, computed_reload_animation_time_scale(animations[0], params->reload_time), animations, count). Sole residual: target emits `call computed_reload_animation_time_scale` (fld params+0Ch); base inlines that helper to const fldz because it is still an inline STUB(return 0.0f) in weapon_animations_timescale_inline.h. Bounded by that helper's own unit -> 100% once it is matched non-stub.)
    weapon_core_reload_state::weapon_and_hands_expression       83.52% PARTIAL  (get_weapon_lexeme_pair -> get_user_hands_expression(lexeme_pair.offset_lexeme,...) -> return hands + main + offset. First two statements byte-identical (+0x01f,+0x02a); return differs (target +0x07a vs base +0x053): target builds an extra expression(animation_lexeme) temp + 3 binary_tree intrusive_ptr dtors = different expression-template instantiation/inline set under /Ob2 /GL. @@EBE private virtual const)
      RE-MATCH 2026-06-03 (template-selection fix attempt, PR #192 follow-up commit): CONFIRMED WALL, not source-steerable. Dumped `mixing::operator+` from target index: target picks `operator+<animation_lexeme>(expression&, animation_lexeme&) -> expression` (a `template<T> operator+(expression&, T&)` overload) + `operator+(expression&, expression&) -> expression` (non-template), both returning `expression` by value and calling the REAL `expression::is_empty()`. On disk mixing_addition_lexeme_inline.h has ONLY `template<T1,T2> operator+ -> addition_lexeme&` and mixing_expression.h `is_empty()` is a `return false` STUB - those overloads do not exist to be selected. Reshape attempts (each full rebuild, report.json top-level): baseline 83.52; `+ expression(offset)` -> 77.48 (WORSE); `expression(hands+main) + offset` -> 56.65 (WORSE). The wraps make it worse precisely because the expression-returning overloads are absent. RESIDUAL = cross-unit header gap (the mixing operator+ overload family + is_empty body), to be matched as its own unit; once present the natural `a+b+c` shape should lift this + all sibling weapon_and_hands variants together. Pattern recorded in assembly_patterns.md. Reverted to baseline. STATE marker + .md updated.
    weapon_core_reload_state::get_user_hands_expression         73.6%  PARTIAL  (if(id==type_sprint) return weapon_lexeme; ASSERT(UNKNOWN_EXPRESSION); user_state_index=(id==type_crouch); captions{stand_reload,crouch_reload}; animation_lexeme override_lexeme(animation_lexeme_parameters(...).animated_object(m_weapon.get_user()).bones_mask(2).playback_type(play_once_and_freeze_at_end)); return override_lexeme. All 7 statements (srcline 62,63,65,67,69,82,83) match target carcass; residual = target keeps the lexeme_parameters setters / weapon_core::get_user / ~animation_lexeme_parameters OUT-OF-LINE while base inlines them (in-class header inlines under /Ob2 /GL whole-program inlining; base body 37 lines vs target 22) - not de-inlinable from this TU. @@ABE private const)
  Header/anchor plumbing: weapon_core_reload_state.h rewritten from all-public to protected ctor + private const members + friends (cook template + use_*_reload_state anchor), mirroring weapon_core_aimed_state.h. weapon_core_state_cook_template.h: added the use_game_core_weapon_core_reload_state fwd-decl + friend (every sibling anchor already befriended; needed for the anchor's qualified cook->new_object call). weapon_core_reload_state_base.h: m_animation_timescale private -> protected (read directly by derived; access-only, no layout/mangling/codegen impact on the matched base). temp_include_all anchor use_game_core_weapon_core_reload_state was already present on common ground.
  Regressions: none real. report-changes 0/0/0/0 between consecutive identical rebuilds (steady state). The ~22 net 100->0 / 100->19.3 seen on the first stub->body build (boost storage2, particle/ai dtors+thunks, empty_stub, weapon_core::instant_aim/chamber_a_round/get_dispersion/reset_fire_queue) are EXE-relink ICF/vcall-fold representative flips from the code-size change - not this unit's functions or callers (the documented "ICF visibility, not source mismatch" convention).
- weapon_core_fire_state (concrete WeaponCore fire-state subclass; 5 fns, ONE unit,
  branch match/game_core-weapon_core_fire_state off origin/int/game_core):
    weapon_core_fire_state::ctor                            100%   DONE     (base_fire_state(weapon,timescale); ASSERT_CMP_U(animations_count,==,8); two 2D fill loops [2][2] view/user_state for m_weapon_animations then m_user_animations sharing animation_index; trailing ASSERT(UNKNOWN_EXPRESSION). class layout: m_weapon_animations[2][2]+m_user_animations[2][2] @0x148/0x158, ctor mangles @@IAE -> protected)
    weapon_core_fire_state::get_weapon_lexeme_pair          100%   DONE     (animation_identifier="weapon-fire"; selected_animation=m_weapon_animations[is_third_view!=false][user_state_id==type_crouch]; set_animation_to_wait; get_weapon_lexeme_pair_impl(buffer,id,sel,&m_weapon,m_animation_playback_state,1,m_animation_timescale, m_weapon.get_bullets_in_queue()<=1?play_once_and_freeze_at_end:play_cyclically, linear_interpolator(s_aim_transition_time)))
    weapon_core_fire_state::weapon_and_hands_expression     82.52% PARTIAL  (was STUB 63.78->82.52. lexeme_pair=get_weapon_lexeme_pair(...); hands_expression=get_user_hands_expression(lexeme_pair.OFFSET_lexeme,...); return main + expression(offset) + hands_expression. Tree shape verified vs target @0x799df0; weight_driving_animation arg is UNREFERENCED in target. Residual = operator+ inline-vs-call LTCG, same class as weapon_core_idle_state 85.65%. Needed #include mixing_addition_lexeme.h to compile)
    weapon_core_fire_state::get_user_hands_expression       73.51% PARTIAL  (was STUB 13.77->73.51. if(state==type_sprint) return expression(weapon_lexeme); idx=(state==type_crouch); sel=m_user_animations[view][idx]; if(pinned_ptr_const<cubic_spline_skeleton_animation>(sel)->animation_type()!=animation_type_additive) return expression(weapon_lexeme); captions[2]={"stand_shoot","crouch_shoot"}; animation_lexeme_parameters(buffer,captions[idx],sel,&weapon_lexeme,NULL).animated_object(m_weapon.get_user()).playback_type(play_once_and_freeze_at_end).additivity_priority(1); hands_lexeme(params); return expression(hands_lexeme). `!=1` decoded as animation_type()!=animation_type_additive (enum full=0/additive=1); target delinked the getter as a COMDAT-folded inventory_holder::scheduler [+0x18]. Residual = lexeme_parameters setter inline-vs-call LTCG, same wall as get_weapon_lexeme_pair_impl)
    weapon_core_state_cook_template<weapon_core_fire_state>::new_object  86.5% PARTIAL  (placement-new weapon_core_fire_state(params->weapon, computed_shooting_animation_time_scale(*animations,params->rounds_per_second), animations, animations_count). Residual = LTCG reg-arg + xmm0-return calling convention of computed_shooting_animation_time_scale, identical wall to pistol new_object 92.08%)
  Header (weapon_core_fire_state.h): access -> protected (ctor @@IAE, weapon_and_hands_expression @@MBE virtual const, get_weapon_lexeme_pair/get_user_hands_expression @@IBE const); friend cook template + use_game_core_weapon_core_fire_state anchor. New anchor use_game_core_weapon_core_fire_state in temp_include_all.cpp; cook_template.h friend + fwd-decl. cpp #includes added: mixing_addition_lexeme.h, mixing_animation_lexeme_parameters.h, cubic_spline_skeleton_animation.h, resources_pinned_ptr.h.
  Regressions: none real - report-changes 18 regressed/18 improved are all (100->0)/(0->100) on thunks/vcalls/COMDAT-folded dtors (~mutex/~Cluster/empty_stub/vector-deleting-dtors) that float between objects on relink; none in this unit, not logic regressions.
- weapon_core_chamber_a_round_state (concrete subclass; 5 fns, ONE unit;
  branch match/game_core-weapon_core_chamber_a_round_state off origin/int/game_core,
  resumed cut-off worker - the unit's base .obj never built because temp_include_all.cpp
  failed to compile):
    weapon_core_chamber_a_round_state::ctor                          100%   DONE     (prior worker; ASSERT_CMP_U(count,==,8); two 2x2 copy loops weapon_animations then user_animations; trailing ASSERT(UNKNOWN_EXPRESSION))
    weapon_core_chamber_a_round_state::get_weapon_lexeme_pair        100%   DONE     (was marked DONE but NEVER compiled - had bugs. FIXED from target asm 0x79a4..: set_animation_to_wait(selected) then get_weapon_lexeme_pair_impl(buffer,"weapon-chamber_a_round",selected,&m_weapon,m_animation_playback_state, time_sync_group=8, m_animation_timescale, play_once_and_freeze_at_end, linear_interpolator(s_aim_transition_time)). Prior wrong guesses: sync_group 1 (->8), playback m_playback_type [undeclared] (->play_once_and_freeze_at_end))
    weapon_core_chamber_a_round_state::new_object (cook)             100%   DONE     (prior worker; new(buffer.c_ptr()) weapon_core_chamber_a_round_state(params->weapon,1.0f,animations,count))
    weapon_core_chamber_a_round_state::weapon_and_hands_expression   83.52% PARTIAL  (return hands_expression + main_lexeme + offset_lexeme. WALL: target uses expression-returning operator+ overloads [operator+<animation_lexeme>(expression&,animation_lexeme&)->expression 0x099150, operator+(expression&,expression&)->expression 0x0bce60] that are standalone in target but ABSENT from our mixing_addition_lexeme_inline.h [only the generic template<T1,T2>->addition_lexeme&]; base falls back to operator+<expression,animation_lexeme>+operator+<addition_lexeme,animation_lexeme>. Recovering = add the full expression-returning operator+ family to the shared mixing_addition_lexeme API - separate cross-cutting unit, out of scope)
    weapon_core_chamber_a_round_state::get_user_hands_expression     74.02% PARTIAL  (if(sprint) return weapon_lexeme; ASSERT; captions[3]; override_lexeme = animation_lexeme_parameters(buffer,captions[idx],m_user_animations[view][idx],&weapon_lexeme,&weight).animated_object(m_weapon.get_user()).bones_mask(2).playback_type(play_once_and_freeze_at_end); return override_lexeme. WALL: target keeps animated_object/bones_mask/playback_type/~animation_lexeme_parameters OUT-OF-LINE [standalone in target: bones_mask 0x09ce50, animated_object 0x098d90]; our /Od base inlines the trivial in-class setters + folds the dtor to its ASSERT. Whole-program inline decision, not steerable from this file. Same class as shotgun_base_substate/pistol_show weapon_and_hands)
  Header edits (necessary to compile the unit): weapon_core_state_cook_template.h - add fwd-decl + friend for use_game_core_weapon_core_chamber_a_round_state (mirrors every other state anchor; the cook's new_object is private). weapon_core_chamber_a_round_state_base.h - m_animation_timescale private->protected so the subclass reads it in get_weapon_lexeme_pair (data-member access is not byte-visible; only mangled members are; base unit score unchanged). New anchor use_game_core_weapon_core_chamber_a_round_state in temp_include_all.cpp.
  Regressions: none from this unit's logic. Enabling this TU relinks game_core and the delinker re-pairs still-STUB COMDATs (boost functors, thunks, empty_stub, float3 ctor, jump_logic ctor/dtor). jump_logic::~jump_logic base body is itself a 9-line empty ASSERT stub (never matched); its prior "100%" was a pairing artifact disturbed by the relink. All 5 chamber fns improved 0->100/100/100/83.52/74.02.
- weapon_core_show_state (concrete WeaponCore show-state subclass, 5 fns; ONE unit, branch
  match/game_core-weapon_core_show_state off origin/int/game_core):
    weapon_core_show_state::ctor                            100%   DONE     (m_time_scale=animation_timescale; ASSERT_CMP_U(animations_count,==,8); two 2x2 loops sharing animation_index into m_weapon_animations[2][2] then m_user_animations[2][2]; trailing ASSERT(UNKNOWN_EXPRESSION). Sibling of pistol_weapon_core_show_state ctor.)
    weapon_core_show_state::get_weapon_lexeme_pair          100%   DONE     ("weapon-show"; m_weapon_animations[is_third_view!=false][user_state_id==type_crouch] (2D); set_animation_to_wait; get_weapon_lexeme_pair_impl(group 6, m_time_scale, play_once_and_freeze_at_end, linear_interpolator(s_aim_transition_time)); trailing ASSERT eater.)
    weapon_core_show_state::new_object                      100%   DONE     (REVIEW FIX: was 99.98% - sole penalizing instr was the animation index. Target line 90 does `add ecx,10h` = animations[4] (first user anim; ctor puts weapon anims 0-3, user anims 4-7); base had animations[1] (add ecx,4). Changed animations[1] -> animations[4]; rebuilt to 100.00 (report-changes 99.98->100.00, 1 improved 0 regressed). The two ?end@buffer_string reloc diffs were benign ICF churn - pistol carries them at 100.0.)
    weapon_core_show_state::weapon_and_hands_expression     83.52% PARTIAL  (structure matches; operator+ template-selection wall - dedicated expression-returning operator+ overloads absent from shared mixing_addition_lexeme_inline.h. Identical to pistol_weapon_core_show_state (83.52%); fix under investigation on PR #192.)
    weapon_core_show_state::get_user_hands_expression       72.12% PARTIAL  (control flow + all reads match; whole-program inline wall - target out-of-lines weapon_core::get_user() + animation_lexeme_parameters setters, base inlines them. Identical to pistol_weapon_core_show_state (72.12%); shared inline-header decision, out of scope.)
  New anchors: use_game_core_weapon_core_show_state in temp_include_all.cpp (+ call in entrypoint, + include); friend decls in weapon_core_state_cook_template.h (fwd decl + friend) so the anchor can call the private new_object. Header: access specifiers set to mangled chars (ctor IAE protected, weapon_and_hands EBE private virtual, get_weapon_lexeme_pair/get_user_hands ABE private), added m_user_animations[2][2], cook-template/use friend anchors. Per-file static s_aim_transition_time=0.3f.
  Regressions: none real - report-changes 19 regressed (all ->0.0) / 23 improved / 0 added / **0 removed**. The 19 ->0.0 are COMDAT representative-ownership churn from adding this TU (thunks, boost _bi storage, interlocked_increment/decrement, weapon_lexeme_pair ctor shared with weapon_core_fire_state); functions still present at full % under another unit's representative (verified). Net +4.
- weapon_core_hide_state (5 fns, ONE unit; concrete WeaponCore hide-state subclass,
  branch match/game_core-weapon_core_hide_state off origin/int/game_core):
    weapon_core_hide_state::weapon_core_hide_state (ctor)               100%   DONE     (: weapon_core_hide_state_base(weapon,is_shown); body m_time_scale=animation_timescale@0x168; KEY redundant m_body_part_mask_for_user=body_part_whole_body_but_hands(-3)@0x130 re-set in derived ctor even though the base ctor already sets it (was the 95.35->100 gap); ASSERT_CMP_U(count==8); two nested 2x2 fill loops sharing ONE un-reset animation_index: weapon[2][2]@0x148 then user[2][2]@0x158; trailing ASSERT(UNKNOWN_EXPRESSION). mangle IAE->protected)
    weapon_core_state_cook_template<hide>::new_object                   100%   DONE     (weapon_anim_length=pinned(animations[0])->length_in_frames(); user_anim_length=pinned(animations[4]) - KEY animations[4]=first USER anim, 4 weapon+4 user, target add ecx,10h=4*4; time_scale=weapon/user; new(buffer.c_ptr()) ctor(params->weapon,time_scale,animations,count,params->shown). mangle AAE->private)
    weapon_core_hide_state::get_weapon_lexeme_pair                      100%   DONE     (id="weapon-hide"; selected=m_weapon_animations[view][user==crouch]; set_animation_to_wait(selected); get_weapon_lexeme_pair_impl(buffer,id,selected,&m_weapon,m_animation_playback_state,7,m_time_scale,play_once_and_freeze_at_end,linear_interpolator(s_aim_transition_time)). vs pistol_show: time_sync=7 not 6, linear not instant interpolator, single array index. mangle ABE->private const)
    weapon_core_hide_state::weapon_and_hands_expression                 83.52% PARTIAL  (lexeme_pair=get_weapon_lexeme_pair; hands=get_user_hands_expression(lexeme_pair.offset_lexeme,...); return hands+main+offset. Residual = shared operator+ overload wall, identical 83.52% to pistol_weapon_core_show_state - target uses dedicated expression operator+ overloads absent from mixing_addition_lexeme_inline.h. #192 class. mangle EBE->private virtual const)
    weapon_core_hide_state::get_user_hands_expression                   73.52% PARTIAL  (if(user_state_id==type_sprint) return weapon_lexeme; ASSERT(UNKNOWN_EXPRESSION) - bare ASSERT present here+reload, absent in pistol_show; user_animation_index=user==crouch; captions{"stand_hide","crouch_hide"}; animation_lexeme_parameters(buffer,captions[idx],m_user_animations[view][idx],&weapon_lexeme,&weight_driving_animation).animated_object(m_weapon.get_user()).bones_mask(2).playback_type(play_once_and_freeze_at_end)->animation_lexeme->expression. Residual = whole-program inline of get_user()/setters, same wall as pistol_show 72.12%. mangle ABE->private const)
  Header edits: weapon_core_hide_state.h access fixes (ctor IAE->protected; weapon_and_hands EBE->private virtual; get_weapon_lexeme_pair/get_user_hands ABE->private const) + sibling-anchor scaffolding (use_game_core_weapon_core_hide_state fwd-decl, template friend weapon_core_state_cook_template, friend use_*). weapon_core_state_cook_template.h: added use_game_core_weapon_core_hide_state to the friend list so the anchor reaches the private new_object. New anchor in temp_include_all.cpp (construct + exercise all 5 fns) + include + dispatcher call.
  Regressions: none (report-changes.json 0 regressed / 1 improved).
- player_logic_crouch_state (player_logic_base_state crouch subclass; 7 fns, ONE unit;
  branch match/game_core-player_logic_crouch_state off origin/int/game_core; shape mirrors
  player_logic_jump_state PR #177 + the per-subclass lexeme machinery shared with stand_state):
    player_logic_crouch_state::ctor                         100%   DONE     (player_logic_base_state(owner, type_crouch); type_crouch==1 (push 1). ctor mangles @@QAE -> public.)
    player_logic_crouch_state::initialize                   100%   DONE     (m_user->crouch(); virtual call [vtable+0x40] on m_user@+0x1c. @@EAE private virtual.)
    player_logic_crouch_state::finalize                     100%   DONE     (m_user->stand_up(); virtual call [vtable+0x44]. base_player declares jump/end_jump/crouch/stand_up consecutively -> crouch=+0x40/stand_up=+0x44. @@EAE private virtual. Added #include base_player.h.)
    player_logic_crouch_state::selected_animations          6.47%  PARTIAL  (UNREACHABLE stub. Body fully decoded from target @0x57f680 (in .md): broken_legs>1?0:movement_animation_index(m_user->input()) -> main_lexeme=movement_lexeme(...) -> make_pair(look_expression(...)+expression(main_lexeme), main_lexeme). BLOCKED C2678: target calls free operator+(expression&,expression const&)->expression, but only template operator+(T1&,T2&)->addition_lexeme& is declared (mixing_addition_lexeme.h). Shared lexeme/operator+ machinery wall (same as jump_logic_state_start::selected_animations), out of unit.)
    player_logic_crouch_state::movement_lexeme              None   PARTIAL  (UNREACHABLE stub; report.json unscored (paired, body too divergent), NOT 0%. Lexeme machinery @0x57efb0: weapon_user_animations_container::get_animation_impl<27,6>, crouch_animations_captions static, linear_interpolator, animation_lexeme_parameters builder chain. Same wall as jump_logic_state_start::get_main_lexeme. Not anchored (only the blocked selected_animations stub reaches it). Decoded asm in .md.)
    player_logic_crouch_state::look_expression             None   PARTIAL  (UNREACHABLE stub; report.json unscored (paired, body too divergent), NOT 0%. Look-lexeme machinery @0x57f1e0: get_animation_impl<27,6>, crouch_animations_captions, instant/linear interpolators, weapon_user_animations_selector::look_time_factor/look_time_calculator, animation_lexeme_parameters builder, 3 near-identical lexeme blocks summed via the same expression operator+ overload. Same machinery wall.)
    player_logic_crouch_state::get_recoil_animation_lexeme None   PARTIAL  (UNREACHABLE stub; report.json unscored (paired, body too divergent), NOT 0%. Recoil lexeme machinery @0x57ed50: animation_lexeme_parameters builder, managed_resource_ptr additive animation, interpolators. Same machinery wall.)
  Header (player_logic_crouch_state.h): moved all members except ctor into private: section (ctor @@QAE public; initialize/finalize @@EAE private virtual; selected_animations @@EBE private virtual const; movement_lexeme/get_recoil_animation_lexeme/look_expression @@ABE private const non-virtual). New anchor use_game_core_player_logic_crouch_state in temp_include_all.cpp (+include +entrypoint call) reaching initialize/execute/finalize via ai::fsm_state&. cpp #include base_player.h added.
  Regressions: none real - report-changes 0 regressed/0 improved between the (matched) easy-3 build and the reverted-stub steady state. The initial enable build's ~24 100->0 flips (pre_perceptors_filter dtor, empty_stub, Cluster/btHashMap/mutex dtors, resource_ptr::operator=, sound thunks) are EXE-relink ICF/COMDAT representative churn from the code-size change (offset by 55 improved); none in this unit or its callers. Overall README 47.92->48.46% (exact 7,994->8,172).
- weapon_core_aimed_fire_state (concrete WeaponCore aimed-fire-state subclass, 5 fns; ONE unit,
  branch match/game_core-weapon_core_aimed_fire_state off origin/int/game_core). Structural twin of
  weapon_core_fire_state (same ctor / get_weapon_lexeme_pair / get_user_hands_expression / new_object
  shapes, 8 animations) - reused that file as the template; differences are string literals + one
  extra branch in weapon_and_hands_expression.
    weapon_core_aimed_fire_state::ctor                            100%   DONE     (ASSERT_CMP_U(count,==,8); two 2x2 loops sharing animation_index into m_weapon_animations then m_user_animations; trailing ASSERT(UNKNOWN_EXPRESSION). Identical to fire_state ctor.)
    weapon_core_aimed_fire_state::get_weapon_lexeme_pair          100%   DONE     ("weapon-aimed_fire" (vs fire_state "weapon-fire"); m_weapon_animations[is_third_view!=false][user_state_id==type_crouch]; set_animation_to_wait; get_weapon_lexeme_pair_impl(group 1, m_animation_timescale, get_bullets_in_queue()<=1?play_once_and_freeze_at_end:play_cyclically, linear_interpolator(s_aim_transition_time)).)
    weapon_core_aimed_fire_state::get_user_hands_expression       73.51% PARTIAL  (identical structure to fire_state, captions {"aimed_shoot","crouch_aimed_shoot"}; same unsteerable lexeme_parameters setter inline-vs-call /GL wall + COMDAT-folded animation_type() getter (inventory_holder::scheduler) as fire_state 73.51%.)
    weapon_core_aimed_fire_state::new_object (cook)               86.5%  PARTIAL  (new(buffer.c_ptr()) ctor(params->weapon, computed_shooting_animation_time_scale(*animations,params->rounds_per_second), animations, count). Residual = LTCG reg-arg + xmm0-return calling convention of computed_shooting_animation_time_scale - call-boundary arg wall, identical to fire_state new_object 86.5%.)
    weapon_core_aimed_fire_state::weapon_and_hands_expression     78.53% PARTIAL  (was 65.94% before recovering the extra branch -> 78.53%. lexeme_pair=get_weapon_lexeme_pair(...); if(user_state_id==type_sprint||user_state_id==type_jump) return main_lexeme+offset_lexeme; hands=get_user_hands_expression(offset,...); return main + expression(offset) + hands. WALL: mixing operator+ inline-vs-out-of-line /GL decision flipping at BOTH operator+ sites - .1 branch target INLINES operator+<animation_lexeme,animation_lexeme>, base out-of-lines it; .2 branch target out-of-lines operator+<animation_lexeme>, base inlines addition_lexeme. Same mixing-unit wall as fire_state weapon_and_hands 82.52%; two diverging sites here vs one there.)
  Header (weapon_core_aimed_fire_state.h): access -> ctor protected (@@IAE), weapon_and_hands_expression private virtual const (@@EBE), get_weapon_lexeme_pair/get_user_hands_expression private const (@@ABE) (note fire_state had get_* protected, aimed has them private); added cook-template + use_game_core_weapon_core_aimed_fire_state friends. New anchor use_game_core_weapon_core_aimed_fire_state in temp_include_all.cpp (+ call + include); cook_template.h fwd-decl + friend (new_object is private). cpp #includes mirror fire_state (mixing_addition_lexeme.h, mixing_animation_lexeme_parameters.h, cubic_spline_skeleton_animation.h, linear_interpolator.h, resources_pinned_ptr.h, weapon_animations_timescale_inline.h); per-file static s_aim_transition_time=0.3f.
  Regressions: none real - report-changes showed 15-28 COMDAT symbols flipping 100<->0 (boost _bi, btSoftBody, btHashMap, threading mutex/event, empty_stub, ai selectors), but the SET CHANGED between two rebuilds whose only source diff was the weapon_and_hands `if` (which cannot touch those), proving non-deterministic delinker tie-break churn. weapon_core_hide_state ctor briefly appeared but is an unmatched STUB. Overall matched_code_percent 27.3498->27.3414 jitter.
- player_logic_stand_state (player_logic_base_state stand subclass; 5 STUB fns, ONE unit;
  branch match/game_core-player_logic_stand_state off origin/int/game_core; shape mirrors
  player_logic_jump_state PR #177 + player_logic_crouch_state PR #198):
    player_logic_stand_state::ctor                         100%   DONE     (player_logic_base_state(owner, type_stand); type_stand==0 (push 0). ctor mangles @@QAE -> public. Added #include base_player.h.)
    player_logic_stand_state::initialize/finalize/execute  None   DONE     (empty {} bodies, header /* no source */; ICF-folded onto shared push ebp;...ret stub @0x1a800. Unscorable None, byte-correct.)
    player_logic_stand_state::selected_animations          8.08%  PARTIAL  (UNREACHABLE stub. Body fully decoded from target @0x771070 (in .md): movement_animation_index=movement_animation_index(m_user->input()) [NO broken_legs branch, unlike crouch] -> main_lexeme=movement_lexeme(...) -> make_pair(look_expression(...)+expression(main_lexeme), main_lexeme). BLOCKED C2678: target calls free operator+(expression&,expression const&)->expression, but only template operator+(T1&,T2&)->addition_lexeme& is declared. Shared lexeme/operator+ machinery wall (same as crouch/jump_logic_state_start), out of unit.)
    player_logic_stand_state::movement_lexeme              None   PARTIAL  (UNREACHABLE stub; report.json paired but 0% (body too divergent), labelled None. Lexeme machinery @0x770960: is_firing branch picks index, finalize_impl (misnamed fixed_size_allocator/animation_lexeme construct), linear_interpolator(s_aim_transition_time), animation_lexeme_parameters builder reading m_user weapon offsets. Same wall as crouch movement_lexeme. Decoded asm in .md.)
    player_logic_stand_state::look_expression             None   PARTIAL  (UNREACHABLE stub; paired 0%, labelled None. Look-lexeme machinery @0x770bc0: get_animation_impl, stand_animations_captions, instant/linear interpolators, look_time_factor/look_time_calculator, 3 near-identical lexeme blocks summed via the expression operator+ overload. Same wall.)
    player_logic_stand_state::get_recoil_animation_lexeme None   PARTIAL  (UNREACHABLE stub; paired 0%, labelled None. Recoil lexeme machinery @0x770710: animation_lexeme_parameters builder, managed_resource_ptr additive animation, interpolators. Same wall.)
  Header (player_logic_stand_state.h): moved all members except ctor into private: section (ctor @@QAE public; initialize/finalize/execute empty @@EAE private virtual; selected_animations @@EBE private virtual const; movement_lexeme/get_recoil_animation_lexeme/look_expression @@ABE private const non-virtual). New anchor use_game_core_player_logic_stand_state in temp_include_all.cpp (+include +entrypoint call) reaching initialize/execute/finalize via ai::fsm_state&. cpp #include base_player.h added; 4 lexeme/expression stubs get UNREACHABLE_CODE() bodies.
  Regressions: none real - report-changes 18 regressed (all ->0.0) / 30 improved / 0 removed / 0 added. The 18 ->0.0 are EXE-relink ICF/COMDAT representative churn from the code-size change (empty_stub, boost _bi storage2/3, vcall thunks, particle/sound dtor folds); none in player_logic_* or any real game_core state function.
- player_logic_sprint_state (player_logic_base_state subclass with two boost::function
  callbacks + a player_stamina_subscriber; 7 fns, ONE unit; branch
  match/game_core-player_logic_sprint_state off origin/int/game_core):
    player_logic_sprint_state::set_callbacks         100%   DONE     (m_initialize_callback = initialize_callback; m_finalize_callback = finalize_callback; operator= on the two boost::function members @0x28/0x48)
    player_logic_sprint_state::initialize            100%   DONE     (m_user->stamina().subscribe_on_depletion(&m_stamina_subscriber); m_initialize_callback(); m_user @ base_state+0x1C, stamina() virtual call [edx+34h])
    player_logic_sprint_state::finalize              100%   DONE     (m_user->stamina().unsubscribe_from_depletion(&m_stamina_subscriber); m_finalize_callback();)
    player_logic_sprint_state::execute               None   DONE     (empty { }; 7/7 bytes equal in text diff, ICF folds the empty stub so objdiff pairs a different fold-rep -> reads None. Bytes correct.)
    player_logic_sprint_state::ctor                  66.70% PARTIAL  (base(owner,type_sprint=2); body-assign m_stamina_subscriber.subscription_callback = boost::bind(&on_stamina_depleted,this) [43.47 member-init -> 66.70 body-assign: target default-ctors the subscriber then assigns]. Residual = boost-internal codegen of the subscriber default ctor [function(clear_type)+next=0] and boost::function::operator= [assign_to/swap/clear], both in shared headers, out of unit scope.)
    player_logic_sprint_state::on_stamina_depleted   47.50% PARTIAL  (m_user->force_animation_selection(). WALL: base_player::force_animation_selection is inline in base_player.h so base inlines the store [mov byte[edx+118h],1] + grows frame [sub esp,8 vs target push ecx]; target keeps an out-of-line call. Inline-vs-outline, fixable only in base_player.h [other unit]. Same class as jump_state is_alive residual.)
    player_logic_sprint_state::selected_animations   3.32%  STUB     (body is a VOSTOK_UNREACHABLE_CODE() placeholder, NOT matched - lexeme/expression mixing wall; the placeholder keeps the pair-returning override compilable so the anchor can construct the state. report.json DOES pair/score it at 3.32%. Carcass preserved.)
  Reachability: unit was NOT in the base index (compiled but link-discarded) -> all read 0%. Added use_game_core_player_logic_sprint_state anchor in temp_include_all.cpp (+ include "player_logic_sprint_state.h" + entrypoint call), mirroring the jump anchor. .cpp #includes added: base_player.h, player_stamina.h (base_player was incomplete). Header: virtual overrides + on_stamina_depleted moved to private: (mangled E/A), ctor + set_callbacks stay public (QAE); execute inline empty.
  Regressions: none real - report-changes 15 regressed / 11 improved, all (100->0)/(0->100) COMDAT fold-representative churn on shared stubs (empty_stub, ~btHashMap, ~mutex, network::world vcall thunks, resource_ptr::operator=, ~enemy_target_selector) from relinking the EXE with the new TU; none in this unit or crouch/jump/stamina/base_player.
- pistol_weapon_core_reload_state (concrete Pistol WeaponCore reload-state subclass, 5 fns; ONE
  unit, branch match/game_core-pistol_weapon_core_reload_state off origin/int/game_core). Shape
  mirrors pistol_weapon_core_fire_state (#174) + weapon_core_reload_state (#192).
    pistol_weapon_core_reload_state::ctor                   100%   DONE     (ASSERT_CMP_U(count,==,16); TWO triple-nested [2][2][2] loops (8 weapon + 8 user = 16) sharing one animation_index; trailing ASSERT(UNKNOWN_EXPRESSION). Both arrays [2][2][2] unlike fire_state's [2][2][2]/[2][2]. mangle IAE->protected.)
    pistol_weapon_core_reload_state::get_weapon_lexeme_pair 99.92% DONE     (captions {"pistol-reload","pistol-reload_empty"}; weapon_state_index=(ammo_in_magazine()==0); selected=m_weapon_animations[view][user==crouch][weapon_state_index]; set_animation_to_wait; impl(group time_sync=2, m_animation_timescale, playback=play_once_and_freeze_at_end, linear_interpolator(s_aim_transition_time)). Sole residual: the `this` argument for the LTCG-folded ammo_in_magazine() call in ecx(base) vs eax(target) - link-time calling-convention register choice for the implicit arg (documented call-boundary exception) -> DONE. NOTE push@0x85=playback, push@0x94=time_sync (first attempt swapped them -> 99.89%). mangle ABE->private const.)
    pistol_weapon_core_reload_state::get_user_hands_expression 77.49% PARTIAL (NO early return. captions[2][2] stand/crouch x normal/empty ("..._reload_pistol"/"..._reload_empty_pistol"); user_state_index=(user==crouch); weapon_state_index=(ammo==0); interpolator local; override_lexeme(params(buffer,captions[us][ws],m_user_animations[v][us][ws],&weapon_lexeme,&weight_driving).animated_object(get_user()).bones_mask(2).playback_type(play_once_and_freeze_at_end)); return override_lexeme. Residual = known wall: target keeps params setters/get_user/~params OUT-OF-LINE, base inlines (/Ob2 /GL). Same class as weapon_core_reload_state::get_user_hands 73.6%. mangle ABE->private const.)
    pistol_weapon_core_reload_state::weapon_and_hands_expression 73.33% PARTIAL (lexeme_pair=get_weapon_lexeme_pair; if(user_state_id==type_sprint) return main+offset; ASSERT(UNKNOWN_EXPRESSION); hands=get_user_hands_expression(offset,...); return hands+main+offset. Control flow (sprint cmp-2 + ASSERT + both returns) matches byte-for-byte. Residual = mixing operator+ overload family: target builds addition_lexeme by value + cloned_in_buffer + ~addition_lexeme + expression::expression<addition_lexeme>; on-disk headers only provide inline operator+ -> addition_lexeme&. Cross-unit header gap (weapon_core_reload_state 83.52%; lower here from extra sprint branch + operator+<expression,lexeme>). mangle EBE->private virtual const.)
    pistol_weapon_core_reload_state::new_object (cook)      86.5%  PARTIAL  (new(buffer.c_ptr()) ctor(params->weapon, computed_reload_animation_time_scale(animations[0],params->reload_time), animations, count). placement-new + ctor call + arg order match. Residual = LTCG calling convention of computed_reload_animation_time_scale (ref in reg, ret xmm0->movss) vs base inline-STUB. Same as weapon_core_reload_state::new_object 86.5%; bounded by helper's own unit. mangle AAE->private.)
  Header (pistol_weapon_core_reload_state.h): access -> ctor protected (@@IAE); weapon_and_hands_expression private virtual const (@@EBE); get_weapon_lexeme_pair/get_user_hands_expression private const (@@ABE); both arrays [2][2][2]; added weapon_user_state_enum.h + weapon_lexeme_pair.h includes, cook-template + use_game_core_pistol_weapon_core_reload_state friend anchors. cook_template.h: fwd-decl + friend (new_object private -> C2248 without it). New anchor use_game_core_pistol_weapon_core_reload_state in temp_include_all.cpp (animations[16] + exercise all 5 + call). cpp #includes mirror weapon_core_reload_state (mixing_addition_lexeme.h, mixing_animation_lexeme_parameters.h, linear_interpolator.h, weapon_animations_timescale_inline.h); per-file static s_aim_transition_time=0.3f.
  Regressions: none real - report-changes 21 regressed (all ->0.0) / 21 improved / 0 added / 0 removed. The ->0.0 are folded-COMDAT representative churn from enabling this TU (sun_cascade/ai::selectors dtors, btSoftBody, mutex, buffer_string::operator=, sound/network thunks, empty_stub); several names appear in BOTH lists. Net-neutral; no source-caused regression.
- double_barreled_weapon_core_fire_state (Double-Barreled WeaponCore fire-state subclass, 6 fns;
  ONE unit, branch match/game_core-double_barreled_weapon_core_fire_state off origin/int/game_core).
  Shape mirrors pistol_weapon_core_fire_state (#174) + weapon_core_fire_state (#193); 12 animations
  (8 weapon[2][2][2] + 4 user[2][2]); m_weapon_animation_index.
    double_barreled_weapon_core_fire_state::ctor                       99.96% DONE     (: weapon_core_fire_state_base(weapon,animation_time_scale), m_weapon_animation_index(-1); ASSERT_CMP_U(weapon.get_magazine_capacity(),==,2); ASSERT_CMP_U(count,==,12); 2x2x2 loop into m_weapon_animations then 2x2 into m_user_animations sharing animation_index; trailing ASSERT(UNKNOWN_EXPRESSION). KEY: target makes a real `call get_magazine_capacity` (own symbol @0x09cc20) - moved get_magazine_capacity OUT-OF-LINE from weapon_core.h into weapon_core.cpp (mirrors ammo_in_magazine pattern; used only by this unit among matched code) to stop the inline `[+478h]` read; frame 44h->40h, 93.51->99.96. Residual = get_magazine_capacity() __thiscall this in eax vs ecx, LTCG arg-reg. mangle IAE->protected)
    double_barreled_weapon_core_fire_state::initialize                 99.76% DONE     (weapon_core_fire_state_base::initialize(); ASSERT(UNKNOWN_EXPRESSION); m_weapon_animation_index = m_weapon.ammo_in_magazine()!=2. Residual = ammo_in_magazine() this in eax vs ecx, LTCG arg-reg. mangle MAE->protected virtual)
    double_barreled_weapon_core_fire_state::get_weapon_lexeme_pair     100%   DONE     (captions {"shot_first_barrel","shot_second_barrel"}; selected=m_weapon_animations[is_third_view!=false][user==crouch][m_weapon_animation_index]; set_animation_to_wait; get_weapon_lexeme_pair_impl(group 1, m_animation_timescale, play_once_and_freeze_at_end, linear_interpolator(s_aim_transition_time)). KEY FIX: playback_type arg was m_playback_type, target passes LITERAL mixing::play_once_and_freeze_at_end (push 1); use mixing:: NOT animation:: (else C2664 vs animation::playback_enum). 95.23->100. mangle ABE->private const)
    double_barreled_weapon_core_fire_state::weapon_and_hands_expression 83.18% PARTIAL  (lexeme_pair=get_weapon_lexeme_pair(...); hands=get_user_hands_expression(lexeme_pair.offset_lexeme,buffer,is_third_view,user_state_id,weight_driving_animation); return main + expression(offset) + hands. Identical source to #193 modulo the extra 5th arg. Residual = operator+ template-selection inline-vs-call /GL wall on `main + expression(offset)` (target operator+<animation_lexeme> out-of-line; base operator+<addition_lexeme,expression> inline), same as #193's 82.52%. 21.43->83.18. mangle EBE->private virtual const)
    double_barreled_weapon_core_fire_state::get_user_hands_expression  73.17% PARTIAL  (if(user_state_id==type_sprint) return expression(weapon_lexeme); user_animation_index=user==crouch; selected=m_user_animations[view][idx]; if(pinned_ptr_const<cubic_spline_skeleton_animation>(selected)->animation_type()!=additive) return expression(weapon_lexeme); captions {"stand_shot_double_barrel","crouch_shot_double_barrel"}; animation_lexeme_parameters(buffer,captions[idx],selected,&weapon_lexeme,&weight_driving_animation).animated_object(m_weapon.get_user()).playback_type(play_once_and_freeze_at_end).additivity_priority(1) -> animation_lexeme override_lexeme -> expression. vs #193: 5-param sig + last ctor arg &weight_driving_animation (not NULL). Residual = lexeme_parameters setter inline-vs-call /GL wall + COMDAT-folded animation_type() getter, same as #193's 73.51%. 12.78->73.17. mangle ABE->private const)
    double_barreled_weapon_core_fire_state::new_object (cook)          86.5%  PARTIAL  (new(buffer.c_ptr()) ctor(params->weapon, computed_shooting_animation_time_scale(*animations,params->rounds_per_second), animations, count). Residual = STUB callee computed_shooting_animation_time_scale folded to fldz here vs target out-of-line xmm0-return LTCG calling convention (frame 48h vs 0Ch); call-boundary wall, identical to pistol new_object #174 86.5%.)
  Header (double_barreled_weapon_core_fire_state.h): access fixed (ctor IAE->protected; initialize MAE->protected virtual; weapon_and_hands EBE->private virtual const; get_weapon_lexeme_pair/get_user_hands ABE->private const) + sibling-anchor scaffolding (use_game_core_double_barreled_weapon_core_fire_state fwd-decl, template friend, friend use_*). weapon_core.h: get_magazine_capacity declaration-only (moved def to weapon_core.cpp). weapon_core_state_cook_template.h: added use_game_core_double_barreled_weapon_core_fire_state to friend list. New anchor in temp_include_all.cpp (construct + exercise all 6 fns) + include + dispatcher call. cpp #includes mirror #193 (mixing_addition_lexeme.h, mixing_animation_lexeme_parameters.h, cubic_spline_skeleton_animation.h, linear_interpolator.h, resources_pinned_ptr.h, weapon_animations_timescale_inline.h); per-file static s_aim_transition_time=0.3f.
  Regressions: none real - report-changes 16 regressed (all ->0.0) / 15 improved / 0 removed / 0 added, net code +0.01%. The ->0.0 set is COMDAT-fold/thunk/boost-template re-attribution churn (empty_stub, [thunk] vcalls, speedtree/btPairSet/network/boost::asio) disjoint from this unit; code coverage rose. get_magazine_capacity was the only cross-TU edit and is used only by this unit among matched code.
- pistol_weapon_core_hide_state (Pistol WeaponCore hide-state subclass; 5 STUB fns, ONE unit;
  branch match/game_core-pistol_weapon_core_hide_state off origin/int/game_core). Fusion of the
  matched pistol_weapon_core_show_state (#185 - m_weapon_animations[2][2][2]+m_user_animations[2][2]
  layout, count 12, ammo-based caption select) and weapon_core_hide_state (#197 -
  linear_interpolator(s_aim_transition_time), time-sync group 7, play_once_and_freeze_at_end).
    pistol_weapon_core_hide_state::ctor                       100%   DONE     (m_time_scale=animation_timescale; ASSERT_CMP_U(count,==,12); triple 2x2x2 loop into m_weapon_animations + double 2x2 loop into m_user_animations sharing animation_index; trailing ASSERT(UNKNOWN_EXPRESSION). NO m_body_part_mask_for_user store, unlike non-pistol hide_state. ctor @@IAE protected.)
    pistol_weapon_core_hide_state::new_object (cook)          100%   DONE     (animations[0]/animations[8] cubic_spline length_in_frames -> time_scale; new(buffer.c_ptr()) ctor(params->weapon, time_scale, animations, count, params->shown). count 12. @@AAE private.)
    pistol_weapon_core_hide_state::get_weapon_lexeme_pair     99.92% DONE     (captions {"pistol-hide","pistol-hide_empty"}; animation_index=ammo_in_magazine()==0; m_weapon_animations[is_third_view!=false][user_state_id==type_crouch][animation_index]; set_animation_to_wait; get_weapon_lexeme_pair_impl(group 7, m_time_scale, play_once_and_freeze_at_end, linear_interpolator(s_aim_transition_time)); trailing compiled-out ASSERT reproduced. Sole residual = m_weapon.ammo_in_magazine() __thiscall this in eax(target)/ecx(base) - permitted call-boundary arg-passing exception, same as pistol_weapon_core_show_state::get_weapon_lexeme_pair 99.92% DONE. @@ABE private const.)
    pistol_weapon_core_hide_state::weapon_and_hands_expression 83.52% PARTIAL  (lexeme_pair=get_weapon_lexeme_pair(...); hands=get_user_hands_expression(offset,...); return hands + main + offset. Residual = shared-header operator+ wall: target uses dedicated expression operator+(expression&,animation_lexeme&)/operator+(expression&,expression&) overloads absent from mixing_addition_lexeme_inline.h, base falls back to generic addition_lexeme& operator+. Identical residual/score to pistol_weapon_core_show_state::weapon_and_hands_expression and weapon_core_hide_state. Out of unit scope. @@EBE private virtual const.)
    pistol_weapon_core_hide_state::get_user_hands_expression  73.86% PARTIAL  (if(user_state_id==type_sprint) return weapon_lexeme; user_state_index=user_state_id==type_crouch; captions {"stand_hide","crouch_hide"}; linear_interpolator(s_aim_transition_time) local; animation_lexeme_parameters builder(.animated_object(get_user()).bones_mask(2).playback_type(play_once_and_freeze_at_end)); return expression(override_lexeme). Residual = shared-header whole-program inline wall: target keeps weapon_core::get_user() + the lexeme_parameters setters OUT-OF-LINE (frame 0x114), base inlines to direct member stores + extra compiled-out ASSERTs (frame 0x128). Same class as pistol_weapon_core_show_state::get_user_hands_expression 72.12% / weapon_core_hide_state 73.52%. Out of unit scope. @@ABE private const.)
  Header (pistol_weapon_core_hide_state.h): ACCESS-SPECIFIER FIRST was the only blocker - first build had all 4 methods at public (QAE/QBE/UBE) -> objdiff would not pair public-vs-target-private -> all 0%. Fixed: ctor -> protected: (@@IAE), the 3 methods -> private: (get_weapon_lexeme_pair/get_user_hands_expression @@ABE, weapon_and_hands_expression @@EBE virtual). Mirrors pistol_weapon_core_idle_state.h. Added template friend cook_template + friend void ::vostok::use_game_core_pistol_weapon_core_hide_state() (+ fwd decl). New anchor use_game_core_pistol_weapon_core_hide_state in temp_include_all.cpp (+ include + entrypoint call); cook_template.h fwd-decl + friend (new_object private). cpp #includes mirror weapon_core_hide_state (weapon_core.h, weapon_state_creation_params.h, cook_template.h, cubic_spline_skeleton_animation.h, linear_interpolator.h, mixing_addition_lexeme.h, mixing_animation_lexeme_parameters.h); per-file static s_aim_transition_time=0.3f. NOTE: header edit needed a touch of the .cpp for ninja to recompile (stale obj kept QAE).
  Regressions: none - report-changes 0 regressed / 0 improved / 0 removed / 0 added.
- double_barreled_weapon_core_reload_state (Double-Barreled WeaponCore reload-state subclass, 5 fns;
  ONE unit, branch match/game_core-double_barreled_weapon_core_reload_state off origin/int/game_core).
  Resumed crashed session: body source mostly written; two build blockers fixed first
  (temp_include_all anchor called private new_object -> C2248: added cook_template.h fwd-decl +
  friend; ALL members were public -> objdiff 0% no-pairing: fixed access specifiers, then had to
  `touch` the .cpp to force recompile - editing the .h alone left the stale obj at QAE).
  Shape mirrors double_barreled_weapon_core_fire_state; 16 animations (8 weapon[2][2][2] + 8
  user[2][2][2]); NO m_weapon_animation_index (reload picks weapon_state by ammo_in_magazine()!=1).
    double_barreled_weapon_core_reload_state::ctor                       99.97% DONE     (: weapon_core_reload_state_base(weapon,animation_timescale); ASSERT_CMP_U(weapon.get_magazine_capacity(),==,2); ASSERT_CMP_U(count,==,16); 2x2x2 braced loop into m_weapon_animations then 2x2x2 braced loop into m_user_animations sharing animation_index; trailing ASSERT(UNKNOWN_EXPRESSION). KEY: braced the 3 nested for-loops so each closing `}` emits its `+0x2` jmp (L31/L32/L33) - structure then byte-identical to target. Residual sub-byte reloc noise. mangle IAE->protected.)
    double_barreled_weapon_core_reload_state::new_object (cook)          86.5%  PARTIAL  (new(buffer.c_ptr()) ctor(params->weapon, computed_reload_animation_time_scale(animations[0],params->reload_time), animations, count). count 16. Residual = base FOLDS STUB computed_reload_animation_time_scale (constant) to `fldz` at call site; target keeps it out-of-line, LTCG returns float in xmm0 (`movss [esp],xmm0`), frame 0x28 vs base 0x0C. Call-boundary inline-vs-call; identical wall to fire_state sibling new_object 86.5%. mangle AAE->private.)
    double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair     99.93% DONE     (captions {"reload_first_barrel","reload_both_barrels"} split across two physical lines -> L58/L59 two-statement attribution; weapon_state_index=ammo_in_magazine()!=1; selected=m_weapon_animations[is_third_view!=false][user==type_crouch][weapon_state_index]; set_animation_to_wait; get_weapon_lexeme_pair_impl(group 2, m_animation_timescale, play_once_and_freeze_at_end, linear_interpolator(s_aim_transition_time)). Structure byte-identical; residual reloc-level. mangle ABE->private const.)
    double_barreled_weapon_core_reload_state::get_user_hands_expression  73.66% PARTIAL  (captions[2][2] stand/crouch x first_barrel/both_barrels ("..._reload_first_barrel"/"..._reload_both_barrels"); user_state_index=user==type_crouch; weapon_state_index=ammo_in_magazine()!=1; named interpolator local; override_lexeme(params(buffer,captions[us][ws],m_user_animations[v][us][ws],&weapon_lexeme,&weight_driving).animated_object(get_user()).weight_interpolator(interpolator).bones_mask(2).playback_type(play_once_and_freeze_at_end)); return override_lexeme. Tried inlined temporary interpolator -> regressed 33%; named local + .weight_interpolator(interpolator) best at 73.66 (+0.35 over unused-local baseline). Residual = lexeme_parameters setter chain + weapon_core::get_user kept OUT-OF-LINE in target, inlined in base. Identical wall to fire_state sibling get_user_hands_expression 73.17%. mangle ABE->private const.)
    double_barreled_weapon_core_reload_state::weapon_and_hands_expression 83.52% PARTIAL  (lexeme_pair=get_weapon_lexeme_pair(...); hands=get_user_hands_expression(lexeme_pair.offset_lexeme,buffer,is_third_view,user_state_id,weight_driving_animation); return hands + main + offset. Tried sibling's `main + expression(offset) + hands` form -> 83.18% (worse); kept natural `hands + main + offset` 83.52%. Residual = operator+ template-selection inline-vs-call /GL wall (target operator+<animation_lexeme> out-of-line; base operator+<expression,animation_lexeme>+operator+<addition_lexeme,animation_lexeme> inline). Identical wall to fire_state sibling 83.18%. mangle EBE->private virtual const.)
  Header (double_barreled_weapon_core_reload_state.h): access fixed (ctor IAE->protected; weapon_and_hands_expression EBE->private virtual const; get_weapon_lexeme_pair/get_user_hands_expression ABE->private const); added `namespace vostok { void use_game_core_double_barreled_weapon_core_reload_state(); }` fwd-decl + `template<typename T> friend class weapon_core_state_cook_template;` + `friend void ::vostok::use_..._reload_state();`. weapon_core_state_cook_template.h: added the anchor to the fwd-decl block + friend list (new_object private -> C2248 without it). Anchor body + include + dispatcher call already present in temp_include_all.cpp from the crashed session. cpp #includes mirror the fire sibling (weapon_core.h, weapon_state_creation_params.h, cook_template.h, weapon_animations_timescale_inline.h, linear_interpolator.h, mixing_addition_lexeme.h, mixing_animation_lexeme_parameters.h); per-file static s_aim_transition_time=0.3f; local fwd-decl of get_weapon_lexeme_pair_impl.
  Regressions: none - report-changes 0 regressed vs the first-session report. (10 deltas vs the 0603 baseline are pre-existing COMDAT-fold/thunk/empty_stub re-attribution churn from prior commits already on HEAD, disjoint from this unit's edits.)
