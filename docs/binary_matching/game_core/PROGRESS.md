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
