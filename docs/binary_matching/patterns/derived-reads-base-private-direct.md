# Derived class reads a base class's private members DIRECTLY = grant friend access

- **confidence:** 9/10
- **tags:** cpp:member cpp:inheritance | asm:mov asm:cmp | topic:inline-vs-call topic:structure-shape topic:buildability

## Symptom

A method on a DERIVED class (own TU) reads members that live in a BASE class
(another TU's header) and are declared `private:` there. The target asm reads
them with a **direct** `[this+off]` / `[reg+off]` load (or `cmp [reg+off], k`),
with **NO `call`** to the base's out-of-line accessor — even though such an
accessor exists (e.g. `ammo_in_magazine()`, `round_is_chambered()`,
`fire_queue_length()`, `get_target()`, `ammo_slot()` are all out-of-line and
would emit a `call`).

Your first instinct is to call the accessor, which gives a `call` the target
doesn't have, OR you can't compile at all because the member is private to the
base. The function pairs at a low % with the accessor `call` polluting the body.

## Cause

The original source read the base member **directly** (`m_ammo_in_magazine`,
`m_is_round_chambered`, `m_fire_queue_type`, ...). For a derived class to do that
against a `private:` base member, the base class **granted it access** — either
the members were `protected:` or (codebase-consistent here) the base declared
`friend class Derived;`. The direct read is the structural truth; the
out-of-line accessor is for *unrelated* classes that are NOT friends/derived
(e.g. a sibling state class still `call`s `ammo_in_magazine()`).

## Fix

Grant the derived class friend access in the base header — this is
**codegen-neutral** (no layout, no mangling, no change to any base function;
it only enables access):

```cpp
// in weapon_core.h, near the existing friends
class weapon;          // forward-decl (same namespace)
class weapon_core : public inventory_item {
    friend class weapon;   // weapon reads m_ammo_in_magazine / m_is_round_chambered / ... INLINE
    ...
};
```

Then spell the member directly in the derived method:

```cpp
void weapon::on_chamber_a_round( ) {
    if ( m_game_ui && m_inventory )                                       // m_inventory: protected base member, direct
        m_game_ui->set_ammo_in_magazine( ( m_is_round_chambered != 0 ) + m_ammo_in_magazine );  // both base-private, direct via friend
}
```

This was the single biggest lever in `game/sources/weapon.cpp`: it unblocked
`set_fire_bullet_transform` (-> 100%), `on_hand_correction_event` (-> 100%),
`set_next_fire_queue_type` (-> 89%), `set_target`, `on_chamber_a_round`,
`on_unload_chambered_round`, `on_after_fire`, `set_next_ammo_type`, `set_ui_ammo`
all in one shot — every one had been parked "private to base, no inline getter".

## Notes / caveats

- Don't grant friend just to chase a %: do it only when the asm **proves**
  direct member access (a `[reg+off]` load where the accessor would be a `call`).
  A *protected* base member needs no friend at all — a derived class reaches it
  directly already; reach for friend only for `private:` base data.
- The residual after the friend fix is usually the LTCG **call convention** of
  the consumer call (`set_ammo_in_magazine` register-arg + tail-`jmp` vs base
  push/call/ret, 16-bit `add ax,[off]` vs 32-bit) — that part is the non-steerable
  arg-boundary LTCG cap, not a structure problem.
- The mirror situation across NON-derived classes (a sibling state that is not a
  friend) legitimately keeps the `call` to the out-of-line accessor — see the
  `finish_reload_predicate` note in `weapon_core_shotgun_reload_state.cpp`.
