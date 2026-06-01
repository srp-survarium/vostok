# game_core matching progress (orchestrator ledger)

Module is large: 529 STUB markers across ~90 files at run start, plus 73 PARTIAL
and 55 SKIPPED. Workers dispatched one-per-function, leaf/small-first.

## Ledger
One line per dispatched function: `module::function -> STATE -> PR (regressions)`.

- game_core::game_core_initialize -> STATE[100%|DONE] -> PR #104 (regressions: none)
- game_core::client_player_update::serialize -> STATE[BLOCKED] -> PR #105 (regressions: none)
  - body matched 1:1 from asm but parked: udp_match_packet.h cluster is never-compiled stubs.
- game_core::inventory_item::inventory_item(action_behaviour_type) -> STATE[78.8%|PARTIAL] -> PR #106 (regressions: none)
- game_core::weapon_recoil_params::weapon_recoil_params() -> STATE[18.18%|PARTIAL] -> PR #107 (regressions: none)
  - SUPERSEDED by #116: the 18% was an inadequate anchor, not a hard cap. Close #107 in favor of #116.
- game_core::scheduler::on_frame(u32,u32) -> STATE[46.39%|PARTIAL] -> PR #108 (regressions: none)
  - NOT a "bug fix": the target asm reads m_active_objects (off 0x10), so the source must too (the STUB's
    m_inactive_objects was wrong vs target). Matching the target, not correcting logic. Residual is
    unsteerable LTCG (target out-of-lines vectora::size/operator[], base inlines).
- game_core::dispersion_calculator::get_dispersion() const -> STATE[87.49%|PARTIAL] -> PR #109 (regressions: none)
  - body matches instruction-for-instruction; residual entirely LTCG (frame/slots, is_aimed inlined,
    safe-bool extra slot). Effectively done as source allows. Getter strategy works well.
- game_core::character_dispersion_calculator::get_target_koef(...) const -> STATE[88%|PARTIAL] -> PR #110 (regressions: none)
  - switch body + m_params reads match byte-for-byte in shape; capped by LTCG empty_stub prologue artifact.
- game_core::character_dispersion_calculator::get_broken_hands_penalty(u8,bool) const -> STATE[82.89%|PARTIAL] -> PR #111 (regressions: none)
  - switch body + m_params reads match instruction-for-instruction; same empty_stub LTCG cap as #110.
  - PR CHAIN: #111 is based on #110's branch (shares the class scaffolding: private-getter mangling,
    params include, tick-stub anchor). Merge #110 before #111. Same-file functions chain like this.
- game_core::weapon_dispersion_calculator::get_value() const -> STATE[100%|DONE] -> PR #112 (regressions: none)
  - first non-trivial 100%: clean getter, no empty_stub prologue to cap it. Independent off feature.
- game_core::weapon_dispersion_calculator::{set_reload_dispersion_amount,set_one_shoot_dispersion_amount,set_aiming_speed} -> STATE[100%|DONE] -> PR #113 (regressions: none)
  - GROUPED unit (first use of accessor-grouping): all 3 setters 100% in one rebuild. Based on #112's branch.
  - key: anchor must ESCAPE &calc/&table through an opaque sink so LTCG doesn't DSE the stores (see loop_performance).

- game_core::player_stealth::{player_stealth(const&),operator=} -> STATE[100%|DONE] -> PR #114 (regressions: none)
  - GROUPED unit on a fresh independent class (no chain): copy ctor + assignment, both 100% in one rebuild.
- game_core::weapon_state::operator= -> STATE[100%|DONE] -> PR #115 (regressions: none)
  - 100%. BONUS: weapon_state::weapon_state() default ctor incidentally hit 100% via operator='s observed
    anchor -> proves a constant-only ctor matches when its instance is OBSERVED. RETRY #107 (weapon_recoil
    18%) with the opaque-sink escape anchor; README rule revised.

- game_core::weapon_recoil_params::weapon_recoil_params() [RETRY of #107] -> STATE[100%|DONE] -> PR #116 (regressions: none)
  - 18% -> 100% with body untouched, just the observed-escape anchor. Confirms the revised ctor rule.

- game_core::breath_vibration_calculator::{ctor,dtor,set_breath_holding_params} -> STATE[ctor 100% DONE, dtor 100% DONE, setter 76.8% PARTIAL] -> PR #117 (regressions: none)
  - GROUPED. ctor+dtor 100% (observed-escape anchor). setter PARTIAL: LTCG inlines trivial accessors
    (states()/front()/current_state()) that the target calls out-of-line. Added breath_state.h for the vtable cast.

- game_core::body_part_parameters::fill_new_stats_item<statistics_item<46,16>> -> STATE[UNVERIFIED] -> (no PR; WIP on branch match/game_core-fill_new_stats_item)
  - Worker #15 crashed (API socket error) before committing. Body is a byte-for-byte structural match per
    its trail; only source-steerable remaining diff = fixed_string<46>("none") must use the 3-arg
    buffer_string(char*,unsigned int const&,char const*) ctor (target) not the 1-arg one (base). WIP
    committed + pushed to that branch. RE-DISPATCH to apply the fix, rebuild/score, commit, open PR.

CONTRACT UPDATES (this run): (1) reproduce target EXACTLY, never fix bugs - exactness > correctness
(MATCHING.md rule #1); (2) trivial same-class accessors may be grouped into one unit/PR (matcher.md).

