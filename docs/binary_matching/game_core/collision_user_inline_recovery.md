<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Collision users, hit initiators and death subscribers

Scope: both death-subscriber constructors; user-data constructor; all twelve
explicit collision-user inlines plus its implicit constructor; usable-object's
cast override; and all three hit-initiator inlines: **20 declarations**.
The six existing pure virtual contracts in collision_user/usable_object need
no fabricated base bodies and are not counted as recovered implementations.
No builds or score refresh. Target/frozen-base topology is identical for all
five types (`player_death_subscriber`, `usable_object_user_data`,
`collision_user`, `usable_object`, `hit_initiator`).

## Real missing initialization in the live death-subscriber constructor

Retail `booby_trap_set::booby_trap_set`, RVA `0x730840`, constructs a bound
death callback, copies it into subscriber +0x148, then writes zero to
subscriber **next +0x20** at +0x68. The independent frozen-base body copies
the callback but omits this store. Its source constructor only initialized
`subscription_callback`. Restore `next(NULL)` in that supplied-callback
constructor; do not zero the type's trailing padding or change callback
ownership to force a byte pattern.

The chain is real: trap-set insert subscribes this member, remove unsubscribes
it, and base-player death dispatch traverses the +0x88 intrusive list with
`call_player_death_subscriber_callback`. That retained callback helper invokes
the stored boost function directly; it is not an unused server-only contract.

The default constructor is a different evidence case. No current default-
construction consumer or retained procedure was found. Replace the literal
placeholder with an explicit **model** of default-empty callback and null
link, guided by the live sibling. The original empty-callback constructor
form and any additional default policy remain open. This is not a claim that
all subscriber constructors everywhere clear their next member: earlier
booster evidence specifically required preserving an uninitialized link.

## Restore existing collision-user factor seams in their consumers

Both factor setters and the engineer getter already had bodies, but consumers
accessed the user-data internals through another accessor instead:

- `player_parameters_modifyer::apply`, RVA `0x59bc10`, statements 44 and 50,
  computes `1 + correction/100` and stores to base-player +0x24 and +0x28.
  These are collision-user +0x18/+0x1c, its two user-data factors. Route the
  existing expressions through `set_artcontainer_time_factor` and
  `set_engineer_use_time_factor` without introducing temporaries or arithmetic.
- Trap `use_execute`, RVA `0x58b460`, statement 6 loads the supplied user's
  owner and reads owner +0x1c. Use `get_engineer_use_time_factor()` on that
  **owner**, not the supplied user's own field. Its PDB also requires const
  `passed_ms`, `engineer_factor`, and `defuse_time_ms`; keep
  `config_defuse_time` mutable and restore just those three qualifiers.

The frozen defuse structure has 13 statements on both sides and only a
five-byte residual at that engineer-factor read (retail 0xf versus base
0x14 bytes). This supports revisiting the accessor boundary rather than
calling it an unconditional linker wall, but it is not a new measurement of
the edited source.

The field operations are verified; the choice between those declared inline
seams and the previous direct user-data spelling remains a source-boundary
inference. Record it rather than calling the refactor a proven byte fix.
Do not force the artcontainer getter into artefact-container's separate
consumer: that method reads the **supplied user-data record**, not an owner's
record, and equality of the two records is not established.

## Existing constructors, virtual defaults and accessors

User-data constructor `0xaaa80` is retained in `collision_user.h`: four zero
owner/time/object words, -1 progress, two 1.0 factors and a null next link.
Both float operand addresses independently decode from executable bytes to
VA `0xa7b6c4`, bytes `00 00 80 3f` (**1.0f**), despite the reader labeling them
`clear_value`. The existing body is correct and stays in its current owner
header. No synthetic owner or constructor is added.

Collision-user construction expands inside base-player construction: base at
+0xc, base vptr store at +0x72, and the user-data ctor call at +0x7e. Keep the
constructor implicit. Its deleting destructor at `0xaaaf0` restores that
vptr, invokes the empty noncopyable base destructor and conditionally frees;
there is no missing subscriber/resource cleanup. These observed base stores
must not be removed with novtable.

Independently resolve all four collision-user cast overloads. Const
base-player cast is the bare null return at `0x327c0`; the other three are
framed null returns at `0x10c0d0`. Both victory-item use defaults are `ret 4`
at `0x12c50`. Preserve these real defaults and derived virtual dispatch.
`usable_object::cast_to_usable` instead returns this; the player typed-query
path invokes that virtual interface. Empty/default results are not guessed
from a shared address or applied across unrelated overrides.

Collision-user's data accessor returns its +4 member. Retail player
`detect_usable_objects` stores owner at player +0x10, current time at +0x1c,
and passes the +0x10 record onward. The existing field-accessor body is
consistent with those real consumers; both factor getters retain their
existing field contracts, with the artcontainer owner-getter's original
consumer still unidentified.

Hit-initiator construction expands in base-player construction at +0x30:
install its vptr, copy supplied id to +4 and supplied local flag to +5. These
remain const members. Its deleting destructor `0xaa9f0` restores the vptr,
destroys the empty base and conditionally frees; the existing destructor
body is appropriately empty. `on_fire` is a retained bare return at
`0x3f210`, but weapon `instant_fire` at `0x595660 + 0x1b7` loads the initiator
from +0x40c and calls virtual slot +4. Keep the empty **base** default and the
real dispatch; do not replace it by an unconditional empty direct call.

## Every declaration

| # | Declaration | Disposition |
|---:|---|---|
| 1 | `player_death_subscriber(callback const&)` | Restore verified missing null-link initialization in live trap-set constructor. |
| 2 | `player_death_subscriber()` | New default-empty callback/null-link model; original consumer/form open. |
| 3 | `usable_object_user_data()` | Existing full initializer list verified, including both raw 1.0 floats and -1 progress. |
| 4 | Implicit `collision_user()` | Keep generated base/member initialization; real vptr and user-data construction observed. |
| 5 | `~collision_user()` | Existing empty body; generated base destruction verified. |
| 6 | `use_victory_item` | Verified empty virtual default. |
| 7 | `use_victory_items_container` | Independently verified empty virtual default. |
| 8 | `usable_object_user_data()` accessor | Existing +4 member address and player use path checked. |
| 9 | Nonconst `cast_to_inventory_holder` | Verified framed null default. |
| 10 | Const `cast_to_inventory_holder` | Independently verified framed null default. |
| 11 | Nonconst `cast_to_base_player` | Independently verified framed null default. |
| 12 | Const `cast_to_base_player` | Independently verified bare null default, a different retained body. |
| 13 | `set_artcontainer_time_factor` | Existing field setter propagated at matching modifier store; source boundary inferred. |
| 14 | `get_artcontainer_time_factor` | Existing owner-record getter retained; original getter consumer unidentified. |
| 15 | `set_engineer_use_time_factor` | Existing field setter propagated at matching modifier store; source boundary inferred. |
| 16 | `get_engineer_use_time_factor` | Existing getter propagated through defuse user's owner; source boundary inferred. |
| 17 | `usable_object::cast_to_usable` | Verified return-this override with live typed-query dispatch. |
| 18 | `hit_initiator(u8,bool)` | Existing const-member initialization and vptr verified in base-player constructor. |
| 19 | `~hit_initiator()` | Existing empty body and generated deleting-dtor operations verified. |
| 20 | `hit_initiator::on_fire()` | Verified empty base default; weapon still invokes virtually. |

Deferred verification covers the trap-set initialization store, the three
propagated accessor consumers, const locals and their original statement
partitions. No new measurement is inferred from unchanged frozen artifacts.
