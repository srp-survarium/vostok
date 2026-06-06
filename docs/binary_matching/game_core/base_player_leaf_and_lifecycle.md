# base_player - player-death subscriber leaves + lifecycle

Unit: a batch of the `base_player` STUBs in
`sources/vostok/game_core/sources/base_player.cpp`.

Target symbols (carcass RVAs are +0x10000 of the real RVA; real RVAs below):

| fn | RVA | access | result |
|----|-----|--------|--------|
| `subscribe_on_player_death` | 0x72ebb0 | QAE (public) | 100% DONE |
| `unsubscribe_from_player_death` | 0x72eb80 | QAE (public) | 100% DONE |
| `call_player_death_subscriber_callback` (free) | 0x72eca0 | static | 100% DONE |
| `on_player_death` | 0x72ecc0 | IAE (protected) | 100% DONE |
| `~base_player` | 0x72ed70 | UAE | 72.19% PARTIAL |
| `tick_active_object` | 0x72ee40 | IAE (protected) | 3.38% INPROGRESS |
| `base_player::base_player` | 0x72f0b0 | QAE | INPROGRESS (unpaired) |

## Commands

```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep base_player
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x72ebb0 --view target
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --base-index binaries/rich/base/index.jsonl --rva 0x72ed70 --view diff
nix develop -c python3 scripts/rebuild.py
```

## The four 100% matches

### subscribe_on_player_death / unsubscribe_from_player_death
Both are a leading compiled-out `ASSERT` then one intrusive_list op:
- asm: `mov byte[ebp-1],0; lea eax,[ebp-1]; call finalize_impl` = a plain
  `ASSERT( UNKNOWN_EXPRESSION )` (assembly_patterns.md "lone assert eater").
- then `m_player_death_subscribers.push_back( subscriber )` (push_back's 2nd arg
  defaults to NULL; target `push 0` = that default) / `.erase( subscriber )`.
- `add ecx, 88h` = `m_player_death_subscribers` at offset 0x88.

### call_player_death_subscriber_callback
`subscriber->subscription_callback()` (boost::function0<void>::operator()). The
target symbol is INTERNAL-linkage (delinker shows only the demangled name, no
`?...@@` COMDAT), so it must be declared **`static`** - without `static` the base
build emits an external COMDAT whose name does not pair (objdiff scored 0% despite
the body being byte-identical via the RVA diff). Adding `static` fixed the pairing
to 100%.

### on_player_death
`m_player_death_subscribers.for_each( call_player_death_subscriber_callback )`. The
function name decays to a function lvalue, binding `for_each(Predicate&)` with
`Predicate = void(player_death_subscriber const*)`, matching the target's
`void_predicate_ref<void __cdecl(...)>`. The only diff is the trailing empty-fn fold
(`dummy::nonnull` vs `finalize_impl`, both 0x3f210) which is byte-identical at link.
Access is **protected** (`IAE`); set via a `protected:` label in the header.

## Anchoring
base_player is abstract, but the four leaves are non-virtual, so a qualified call on
a fabricated null ref ODR-uses each body. Added `use_game_core_base_player()` to
temp_include_all.cpp (registered in IncludeAll::IncludeAll). Because
`tick_active_object`/`on_player_death` are protected, the anchor is befriended:
`friend void ::vostok::use_game_core_base_player();` in base_player.h (with a forward
decl of the anchor in namespace vostok).

## ~base_player (72.19% PARTIAL)
Body is just `ASSERT( UNKNOWN_EXPRESSION )` (line 36 = finalize_impl @0x09); adding it
took 69.79% -> 72.19%. The rest is compiler-generated member/base destruction. The
residual divergence is structural and cross-class:
- our build INLINES the base-subobject destructors (`loose_ptr_base::~loose_ptr_base`,
  the hit_initiator/collision_user/inventory_holder dtors) and lays all 4 base vtables
  down at the TOP; the target calls e.g. `survarium::hit_receiver::~hit_receiver`
  OUT-OF-LINE and sets each base vtable right before its subobject dtor.
- the `dummy::nonnull` vs `finalize_impl` diffs are folded-empty-fn cosmetics.
NEXT: match the four base-class destructors out-of-line first; then this should close.

## base_player::base_player (INPROGRESS, unpaired)
Body empty `{ }`; all work is the member-init list. Decoded from @0x72f0b0:
recoil_params<-params+0x10 (0x10B), dispersion_params<-params+0x20 (0x38B),
breath_holding_params<-params+0x58 (0x28B), damage_model<-params+0x114,
movement_speed_factor<-clear_value, the 4 trailing bools = 0. Init list updated to the
verified member copies. Not yet anchored (abstract class needs a concrete derived stub
+ a constructed base_player_creation_params), so it reads unpaired. Like the dtor, the
full match is also gated on the base-class CTORS being out-of-line.

## tick_active_object (3.38% INPROGRESS)
Large (~616B) virtual-dispatch-heavy function on the active object + weapon_core +
inventory. Now PAIRED (was unpaired) after setting protected access + anchoring. The
active-object vtable offsets used (0x1c/0x20/0x28/0x2c/0x54/0x70/0x7c) exceed
interactive_object's own table, so the dynamic type has additional virtuals - mapping
those is the next step. Real engine logic; reconstruct statement-by-statement against
the preserved carcass.
