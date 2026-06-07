# weapon_core batch 7 - predicates / accessors

Resumed from a crashed session. Unit = a coherent batch of weapon_core
predicates + accessors. Branch `match/game_core-weapon_core-batch7` off
`origin/int/game_core`.

## Functions in the unit

| function | mangled access | final % | state |
|---|---|---|---|
| `is_sprinting()` | EBE (priv virtual const) | 100.00 | DONE |
| `instant_idle_predicate()` | ABE (priv const) | 100.00 | DONE |
| `get_ammo_slot(ammo_id_enum)` | QAE (pub) | 99.77 | DONE (LTCG-arg) |
| `can_and_must_reload_predicate()` | ABE (priv const) | 93.32 | PARTIAL |
| `can_and_must_reload_and_animation_ended_predicate()` | ABE (priv const) | 86.17 | PARTIAL (wall) |
| `could_be_used(base_player const&)` | QBE (pub const) | 73.90 | PARTIAL (wall) |
| `could_be_aimed(base_player const&)` | QBE (pub const) | 75.88 | PARTIAL (wall) |
| `ready_to_reload()` | QBE (pub const) | STUB | out-of-lined support |
| `damage_model::broken_hands_count()` (header inline) | - | - | support |

`is_sprinting`, `instant_idle_predicate`, `get_ammo_slot` and the
could_be_used/could_be_aimed first-approximations were authored by the crashed
session; this session finished get_ammo_slot's diagnosis, switched could_be_* to
the operator* form, matched both reload predicates, and out-lined ready_to_reload.

## Commands

```
git -C /home/sheep/Projects/surv/vostok_7 diff
nix develop -c python3 scripts/rebuild.py            # no module arg
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl --function <name> --view diff \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --function <name> --view target
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function <name> --list
```

## get_ammo_slot - 99.77% DONE

`static profile_slot_enum const weapon_ammo_slots[2][2]` 2x2 table, switch on
`profile_slot_id()` with `case weapon1_slot / weapon2_slot / default invalid_slot`.
Jump-table-free dispatch (two `cmp/je`). Structure byte-identical. Sole residual:
target loads `this` for the out-of-line `profile_slot_id()` call into `eax`
(`mov eax,[ebp-4]; call`), ours into `ecx`. Call-boundary register on the callee's
`this` -> LTCG custom calling convention on profile_slot_id, not steerable from
this function. The two other `~` rows are identical-instruction reloc diffs on the
static table address. DONE-class (LTCG-arg only).

## could_be_used / could_be_aimed - 73.90% / 75.88% PARTIAL (wall)

Target asm (both): `call damage_model()` (vtbl[+0Ch]) -> then
`call intrusive_ptr<...>::operator*` (an OUT-OF-LINE call, the COMDAT-folded
template body) -> reads `[+33Ah]`+`[+33Bh]` directly (broken_hands_count() inlined)
-> `cmp 2` etc.

So the target source is `(*user.damage_model()).broken_hands_count()` with
`broken_hands_count()` inlined but `operator*` out-of-lined. Switched our source
from `->` to `(*ptr).method()` so we invoke operator* (the symbol the target uses).
No score change: our build STILL inlines operator*, bringing the compiled-out
`ASSERT(m_object)` stub (`byte=0; lea; call dummy`) + the deref inline, plus a temp
copy.

Root cause (verified): base rsp flags are `/Od /Ob2 /GL /Oi`. Under `/Ob2` MSVC
inlines `inline`-marked functions even at `/Od`, but respects a size heuristic.
Our compiled-out `ASSERT` expands to a TINY stub, so our `operator*` body fits the
inline budget and gets inlined. In the original, `ASSERT(m_object)` expanded to a
larger live body, pushing `operator*` over the budget -> out-of-lined. The
inline/out-line decision lives in the shared `intrusive_ptr_inline.h`; cannot be
steered from these functions without affecting every other operator* call site.
Established inline-decision wall (same class as is_trying_to_aim residuals).

could_be_used: `return !( broken_hands_count == 2 && is_double_handed() );`
(is_double_handed reads `[+48Ah]` inline - matches).
could_be_aimed: `return broken_hands_count != 2;`

## can_and_must_reload_predicate - 93.32% PARTIAL

Out-lined `ready_to_reload()` (was a header inline stub; target has an out-of-line
symbol @0x0ac370 with "A LOT OF LOGIC", not matched). Declared it in the header,
defined a `return true;` stub in the .cpp, anchored it. Now the predicate emits
`call ready_to_reload` as the target does.

Source: `return ready_to_reload() && m_ammo_in_magazine == 0 && !m_is_round_chambered;`
- members read directly: `[+47Ah]` = m_ammo_in_magazine (u16), `[+48Eh]` =
  m_is_round_chambered. `&&` short-circuit produces the `mov 1 / mov 0` result temp.
Structure byte-identical (every branch + member read + result temp matches).

Sole residual: target reserves a 0x30 frame and zeroes a DEAD dword `[ebp-10h]` in
the prologue (never read); ours has a 0x08 frame and no such slot. Hidden local
with no PDB LOCALS record and no read - not source-pinnable. 93.32%.

## can_and_must_reload_and_animation_ended_predicate - 86.17% PARTIAL (wall)

Source:
`return current_base_state().has_animation_ended() && can_and_must_reload_predicate();`
- `current_base_state()` = `*static_cast<weapon_core_base_state*>(m_logic->current_state())`
  reads `[+414h]` (m_logic, ai::fsm*) -> `[+10h]` (fsm::m_current_state). The 3-temp
  pass-through chain matches the inlined static_cast.
- `can_and_must_reload_predicate()` is a `call` (matches).

Residual: target out-of-lines `weapon_core_base_state::has_animation_ended()`
(`call has_animation_ended`); our `/Od /Ob2` inlines the `[+135h]` read. Same
documented inline-decision wall as `target_and_animation_ended_predicate` (85.68%
sibling) and `must_chamber_a_round_and_animation_ended_predicate`. Out-lining the
shared getter would regress other inlined call sites - rejected. 86.17%.

## Build / regressions

Final `rebuild.py`: 0 regressed, 0 removed. The reload-predicate build improved
both predicates (26.56->93.32, 22.90->86.17) with 0 regressed. Out-lining
ready_to_reload caused no regression (no other reachable caller). Overall match
27.46% / 32.42% unchanged at the rollup (these are small functions).
