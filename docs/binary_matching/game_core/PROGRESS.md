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
