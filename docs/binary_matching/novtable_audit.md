<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# `novtable` audit

This audit answers one narrow source-recovery question: which Vostok and
Survarium polymorphic classes carried MSVC's `__declspec(novtable)` in the
retail build? It is not valid to add the attribute merely because a class is
abstract. The attribute is established only by preserved source or by a
candidate-only base-vtable write that retail omits in the same construction or
destruction cone.

The 2026-09-04 pass covered all 136 abstract Vostok/Survarium class records in
the retail PDB, all 84 attributed declarations in the source tree, every
vtable reference in both rich indexes, 34,366 uniquely paired functions with
identical mangled names, and every method qualifier on the 136 class records.

## Result

- 60 of the 136 retail abstract classes are attributed. None has an own-base
  vtable reference in retail.
- 64 untagged abstract classes positively install their own vtable in retail:
  62 ordinary types and the `batched_geometry<T>` and `statistics_value<T>`
  specializations. `novtable` is forbidden for these classes.
- The five observable missing attributes were
  `survarium::base_animation_controller`, `survarium::base_network_client`,
  `survarium::base_player`, `survarium::engine`, and
  `vostok::physics::base_physics_object`. The pre-fix candidate emits their
  base-vtable writes while retail does not; the attributes were restored in
  the data-audit stack merged through `baaa14f65`.
- The only other untagged type whose vtable occurs only in the candidate is
  `vostok::testing::test_base`. Its reference comes from candidate-only test
  registration code, while the preserved original declaration is explicitly
  untagged. It is not a `novtable` case.
- No additional observable missing or incorrectly applied attribute remains.

A fresh worktree-scoped build verifies the five restored attributes directly:
all five classes now have zero own-base vtable references on both sides. The
same build finds zero retail own-vtable stores for every attributed class.

Of the 84 attributed source types, 77 preserve the attribute from the original
pre-reconstruction source tree. The other seven are the five classes above,
`vostok::ai::fsm_state`, and `survarium::scaleform_game_engine`; each was
recovered from constructor/destructor code generation. The Scaleform-facing
declaration uses the project macro, rather than spelling the equivalent
`__declspec` directly.

## Compiler-generated member audit

The method-qualifier pass initially found four abstract-class records where
retail marked a special member compiler-generated but the reconstruction
declared an empty body. Removing those invented declarations makes each whole
class record identical:

- `network::order::order()`;
- `network::response::response()`;
- `network_core::udp_match_packets_orderer::udp_match_packets_orderer()`; and
- `physics::base_physics_object::~base_physics_object()`.

The post-build qualifier diff is zero across all 136 abstract classes. In
particular, adding `VOSTOK_NOVTABLE` to `udp_match_packets_orderer` was tested
and rejected: it did not supply the missing evidence, while removing the
fabricated constructor closed the complete class topology without the
attribute.

## Untagged classes without an attribute verdict from vtable code generation

Eleven untagged abstract classes have no standalone own-base vtable reference
on either side. Absence alone is not proof of `novtable`: LTCG may remove a
base write that is immediately overwritten by a derived constructor.

| Class | Decision |
|---|---|
| `vostok::camera_effector` | keep untagged; preserved original source |
| `vostok::animation::camera_follower_interface` | keep untagged; preserved original source |
| `vostok::animation::i_editor_mixer` | keep untagged; preserved original source |
| `vostok::render::destroy_data_helper_base` | keep untagged; preserved original source |
| `vostok::render::skin` | keep untagged; preserved original source |
| `vostok::render::user_render_surface` | keep untagged; preserved original source |
| `survarium::animation_controller_parameters` | keep untagged; its earlier source ancestor is untagged and both links emit only the derived vtables |
| `survarium::game_scene` | keep untagged; its earlier source ancestor is untagged and both links emit only the derived vtables |
| `vostok::animation::animation_states_dumper` | leave unresolved; it is used by reference and has no candidate class emission |
| `vostok::network_core::udp_match_packets_orderer` | keep untagged; its complete PDB class topology matches without the attribute, and both links fold the derived construction identically |
| `vostok::render::batched_geometry_interface` | keep untagged; both links fold the base write from the two `batched_geometry<T>` constructions |

The generic `batched_geometry` and `statistics_value` declarations also lack a
standalone generic vtable reference. Their emitted specializations do install
their own vtables in retail, so they belong to the forbidden group rather than
this unresolved group.

## Residual vtable-reference check

Before #557, 17 engine-named functions across the 34,366 unique exact-name
pairs contained a candidate-only vtable reference. Nine were exactly the five
restored attributes and are now gone. The remaining eight are concrete-class
or call-boundary differences:

- generated deleting destructors for `network::functor_order`,
  `network::functor_response`, and `particle_action_gravity`;
- two `n_ary_tree_size_calculator` optimizer-hoisting differences;
- two Bullet `btSphereShape` inline-construction differences; and
- `render::animated_model_instance`, whose constructor is standalone in retail
  and inlined into the candidate cook.

Each of those classes has positive retail own-vtable writes elsewhere, so none
can be repaired with `novtable`.
