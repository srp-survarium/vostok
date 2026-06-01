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
  - low % is expected: constant-only default ctor, LTCG dead-store-elim'd until callers matched (see README).
- game_core::scheduler::on_frame(u32,u32) -> STATE[46.39%|PARTIAL] -> PR #108 (regressions: none)
  - fixed a real source bug (m_inactive_objects -> m_active_objects); residual is unsteerable LTCG (target
    out-of-lines vectora::size/operator[], base inlines). % chose correctness over the metric.
- game_core::dispersion_calculator::get_dispersion() const -> STATE[87.49%|PARTIAL] -> PR #109 (regressions: none)
  - body matches instruction-for-instruction; residual entirely LTCG (frame/slots, is_aimed inlined,
    safe-bool extra slot). Effectively done as source allows. Getter strategy works well.
- game_core::character_dispersion_calculator::get_target_koef(...) const -> STATE[88%|PARTIAL] -> PR #110 (regressions: none)
  - switch body + m_params reads match byte-for-byte in shape; capped by LTCG empty_stub prologue artifact.
- game_core::character_dispersion_calculator::get_broken_hands_penalty(u8,bool) const -> STATE[82.89%|PARTIAL] -> PR #111 (regressions: none)
  - switch body + m_params reads match instruction-for-instruction; same empty_stub LTCG cap as #110.
  - PR CHAIN: #111 is based on #110's branch (shares the class scaffolding: private-getter mangling,
    params include, tick-stub anchor). Merge #110 before #111. Same-file functions chain like this.

