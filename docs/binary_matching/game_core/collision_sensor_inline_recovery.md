<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Collision sensor, geometry and subscriber inlines

Scope: seven sensor inlines, four implementation-file predicate methods, four
geometry inlines, three explicit subscriber inlines and its implicit default
constructor: **19 declarations**. No compilation or score refresh. Raw target
and frozen-base topology is identical for all five classes/predicate types.

## Real bug: the removal predicate had reversed polarity

`collision_sensor::filter_sensed_objects`, retail RVA `0x58a310`, passes the
named `objects_filter_predicate` to `std::remove_if`, then erases its returned
range before sorting/deduplicating. The retained predicate expansion in
`std::priv::__find_if<..., objects_filter_predicate>`, RVA `0xb6f20`, statement
3, calls sensor virtual slot +0xc (`is_filter_passed`) and **negates** its bool
result before testing whether this is an object to remove:

```
call virtual_is_filter_passed
movzx eax, al
neg eax
sbb eax, eax
add eax, 1
```

An independent, base-only read at `0xcd640`, statement 3, instead immediately
tests the returned bool, with no negation. Our existing unmarked inline was
`return m_sensor->is_filter_passed(obj);`: it removed objects that **passed**.
Restore `!`. Do not invert `is_filter_passed` itself: the default tests nonnull,
and damage-zone's override tests collision-group bit 0x40. Preserve dispatch.
This is a concrete behavior correction, not an inferred helper-boundary change.
The predicate constructor's captured sensor at +0 remains unchanged.

The neighboring `left_objects_predicate::operator()` is different and correct.
Retail `0xb6610` computes a const bool from `find(current, obj) == end`, returns
false for an object still present, otherwise appends it to the deletion buffer
and returns true. Its parent at `0x58a690`, statement 2, constructs the two
captured pointers from the sensed buffer and local deletion buffer before
passing it to remove_if. No extra inversion or notification belongs there.

## Empty defaults do not imply empty virtual calls

All four notification defaults are retained: inside, leave and loosed are
`ret 4` at `0x12c50`; enter is a framed this spill followed by `ret 4` at
`0xd2070`. Keep their protected access and empty bodies. Inside notification's
retained parent uses vtable slot +0x18; damage-zone overrides are the real
consumers. Remove the old debugger questions, not the virtual invocations.

Both subscriber casts independently resolve to `xor eax,eax; ret` at
`0x327c0`. The sensor PDB does not declare a `cast_to_sensor` override: inherit
the null default instead of inventing one. Its raw layout agrees with base.
The subscriber destructor has no extra state cleanup: sensor destruction
restores the subscriber vptr at +0x67 after destroying the vector. Sensor
construction likewise writes the subscriber vptr at +0x1d before its own.
Leave the subscriber default constructor implicit and do **not** apply
novtable to remove these observed stores.

## Bounded models and existing forwarding evidence

- `is_active`: return +0x1c. Sensor construction clears it; insert sets it and
  remove clears it. These stores establish the field, not a particular
  original inline consumer or absence of more elaborate getter policy.
- Sensor debug drawing: loop over the existing geometry array/count, forwarding
  color. Geometry debug drawing: forward its ghost, stored physics world and
  color to the existing `bt_ghost_object::dbg_render(world*, color const&)`.
  Neither wrapper has a retained procedure or a current source call found in
  the game/game-core search. This is **not** proof they were unused or
  editor-only. The lower physics source has a real shape/world draw body, but
  no retained procedure was found for that helper either: it is a source prior,
  not independent retail confirmation. World is null before insertion and
  after removal. Original guards/assertions and lifecycle consumers remain
  open; no new live debug invocation is added. Complete declarations come from
  the existing geometry and physics headers, without invented bridge symbols.
- `query_objects_by_type<usable_object>`, retained at `0x7f120`, iterates
  subscribers, applies the PMF's this adjustment, calls it, tests a named const
  result and appends nonnull results. Its real player consumer passes
  `collision_geometry_subscriber::cast_to_usable`. Preserve the existing PMF
  call, index loop and const result; do not replace it with a private-field
  type test. Geometry's cast returns the unadjusted this pointer at `0x17600`.
- Ghost getter is the existing +0x124 load used through the first geometry in
  `booby_trap_set_core::get_visible_place_transform`. Count is the existing +0x18 getter; source
  lifecycle/iteration uses the same field, with no separate getter consumer
  identified. Neither requires adding a new accessor or bypassing visibility.

## Every declaration

| # | Declaration | Disposition |
|---:|---|---|
| 1 | `collision_sensor::is_active` | New flag-getter model; original consumer/policy open. |
| 2 | `collision_sensor::dbg_render` | New geometry-forwarding model; guard/lifetime/consumer open. |
| 3 | `collision_geometries_count` | Existing count getter retained; observed field contract. |
| 4 | `on_inside` | Verified empty protected default; callers remain virtual. |
| 5 | `on_leave` | Verified empty protected default; derived notifications retained. |
| 6 | `on_enter` | Verified empty framed protected default, independently checked. |
| 7 | `on_objetcs_loosed` | Verified empty protected default; preserve PDB spelling. |
| 8 | `objects_filter_predicate::objects_filter_predicate` | Existing sensor-pointer capture confirmed. |
| 9 | `objects_filter_predicate::operator()` | Real polarity fix, independently confirmed against frozen candidate. |
| 10 | `left_objects_predicate::left_objects_predicate` | Existing two-buffer capture confirmed in parent. |
| 11 | `left_objects_predicate::operator()` | Existing find/append/return sequence confirmed in retained body. |
| 12 | `collision_geometry::cast_to_collision_geometry` | Existing return-this override confirmed. |
| 13 | `query_objects_by_type<T>` | Existing retained usable-object specialization and real player consumer checked. |
| 14 | `collision_geometry::dbg_render` | New physics-forwarding model; original guards and lower consumer open. |
| 15 | `ghost_object` | Existing pointer getter and trap consumer retained. |
| 16 | `collision_geometry_subscriber::~collision_geometry_subscriber` | Existing empty destructor; base vptr restoration observed. |
| 17 | `cast_to_usable` | Verified null base default, with live typed-query dispatch. |
| 18 | `cast_to_sensor` | Verified null base default, inherited unchanged by sensor. |
| 19 | Implicit `collision_geometry_subscriber()` | Keep compiler-generated constructor; subscriber vptr initialization observed. |

Deferred verification: rebuild once the wider reconstruction batch is ready;
check corrected predicate and filter caller partitions plus header dependents.
Do not report new byte percentages from the frozen artifacts inspected here.
