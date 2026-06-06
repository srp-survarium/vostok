# weapon_core batch6: instant forwarders + idle predicate + reset_fire_queue

All in `sources/vostok/game_core/sources/weapon_core.cpp`, class `survarium::weapon_core`.
Branch `match/game_core-weapon_core-batch6` off `origin/int/game_core`, worktree
`/home/sheep/Projects/surv/vostok_7`.

## Targets (pdb_rich_query --index binaries/rich/target/index.jsonl --list)
- reset_fire_queue          0x592f40  QAE? -> actually private? checked below
- instant_idle_start        0x593160  QAE (public)
- instant_chamber_a_round   0x593350  QAE (public)
- instant_reload            0x595840  QAE (public)
- instant_idle_predicate    0x594e40  ABE (private const) -> moved decl into private section of weapon_core.h

Member offsets (weapon_core.h):
0x278 m_user_animations_selector, 0x2c8 m_recoil_calculator, 0x328 m_dispersion_calculator,
0x44c m_user, 0x47a m_ammo_in_magazine, 0x47c m_bullets_in_queue, 0x47e m_fire_queue_type,
0x488 m_aimed, 0x48e m_is_round_chambered, 0x492 m_is_idle.

vtable slots (on_* virtuals declared on_before_fire, on_after_fire, on_reload, on_chamber_a_round,
on_show, on_hide, on_unload_chambered_round): on_reload=+0xA8, on_chamber_a_round=+0xAC,
on_show=+0xB0, on_hide=+0xB4. Target confirms instant_reload calls +0xA8, instant_chamber_a_round +0xAC.

## Bodies (from --view target)
- instant_idle_predicate: `return m_user_animations_selector.sprint_predicate() || m_user_animations_selector.is_in_jump();`
  asm: sprint_predicate (this+0x278); if !true then is_in_jump; bool dword temp normalize. Single return.
- instant_chamber_a_round: 2 ASSERTs (finalize_impl on byte temps) on m_ammo_in_magazine;
  `m_aimed=false; chamber_a_round(); on_chamber_a_round(); m_recoil_calculator.chamber_a_round();`
- instant_reload: `m_aimed=false; load_magazine(); on_reload(); m_recoil_calculator.reload();
  m_dispersion_calculator.reload(); reset_fire_queue();`
- instant_idle_start: `m_is_idle=true; if (!(m_user->input().actions_mask & 0x20)) reset_fire_queue();`
  (m_user->input() = base_player vtable+0x24, actions_mask at player_input+0x10)
- reset_fire_queue:
  ```
  if (fire_queue_length() == 0xff) {
      m_bullets_in_queue = m_ammo_in_magazine;
      if (m_is_round_chambered) ++m_bullets_in_queue;
  } else
      m_bullets_in_queue = math::min( fire_queue_length(), u16(m_ammo_in_magazine + m_is_round_chambered) );
  ```
  else branch min uses min_integral shape `right + ((left-right)&-(left<right))` -> math::min(u16,u16) overload
  (no R_ASSERT, unlike the template min). left=fire_queue_length(), right=desired. MSVC right-to-left arg
  eval => desired computed first (matches target which builds desired before the fql call). chambered added
  arithmetically via neg;sbb;neg (bool->0/1).

## Anchor
Added wc.instant_reload(); wc.instant_chamber_a_round(); wc.instant_idle_start(); to
use_game_core_weapon_core_small_setters() in temp_include_all.cpp. reset_fire_queue (called by
instant_reload/instant_idle_start) and instant_idle_predicate (FSM predicate via initialize_weapon_logic)
reachable transitively.

## fire_queue_length out-lined (header inline -> .cpp definition)
reset_fire_queue at first build (inline fire_queue_length in header) -> 66.5%: the `if`-condition
and the min arg both inlined `m_weapon_fire_queue_types[m_fire_queue_type]` (movzx [47E]; mov [454];
movzx) instead of the target's `call fire_queue_length`. fire_queue_length is a real standalone
target symbol @0x09b290 (QBE public const), so I moved its body out of the header into the .cpp
(like the existing ammo_in_magazine/get_magazine_capacity NOTE pattern). reset_fire_queue then
emitted both calls -> 87.85%.

## reset_fire_queue else-branch min iteration
- 87.85% diff: target builds `desired` into its own slot [ebp-2] then copies to the min by-value
  arg slot [ebp-6], fql to [ebp-4] (3 temps, frame 0xC). My inline-rvalue `math::min(fql, u16(...))`
  built only 2 slots. HOISTED desired into a NAMED local declared above the call -> 94.09%.
- 94.09% diff: my base computed `m_ammo + m_is_round_chambered` as a DIRECT byte add (no normalize);
  target does neg;sbb;neg (the `(x!=0)` bool->int idiom) before the add. Changed the named local to
  `m_ammo_in_magazine + ( m_is_round_chambered != 0 )` -> 99.65%.
- 99.65% residual: register choice (eax vs ecx for `this` at the if-condition fql call) + stack-slot
  NUMBERING ([ebp-2]/[ebp-6]/[ebp-4] target vs [ebp-4]/[ebp-8]/[ebp-6] base). Same 3-slot structure,
  same instruction sequence/order. Pure allocator nondeterminism; not source-steerable. Stopped here.

## Results (report.json top-level fuzzy_match_percent)
- instant_idle_predicate     -> 100.00% DONE
- instant_chamber_a_round    -> 100.00% DONE
- instant_reload             -> 100.00% DONE
- instant_idle_start         -> 100.00% DONE
- fire_queue_length          -> 100.00% DONE (out-lined; enabled the two reset_fire_queue calls)
- reset_fire_queue           ->  99.65% PARTIAL (register/stack-slot numbering residual)

report-changes.json: regressed 0.
