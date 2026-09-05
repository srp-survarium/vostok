<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Weapon inlines, finger correction and sound callbacks

PR 571 source-only audit covers **41 declaration/consumer anchors**: ten
weapon inlines and three generated weapon operations, twelve corrector
operations, its nested hand constructor, eight sound-effect operations,
six nested sounds operations, and the sound-finished callback. Existing
bodies, compiler-generated operations and explicit models count as reviewed,
not as newly implemented functions. Supporting weapon/cook procedures are
not counted again. No build, new score or post-edit structure claim.

The concrete source bug is the missing false initializer for the corrector's
first-person flag. Restore three named caller seams (locator setter,
current-player predicate and single-element erasure), with the original
boundaries explicitly open. Two particle setters receive bounded models;
their original result-list and ownership contracts remain unresolved.

## Individual register

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `weapon::first_view_death_animations_count()` | Preserve public const getter of const u32 at +0xfdc. Cook size calculation and load_weapon consume the count, not a fixed capacity. |
| 2 | `weapon::third_view_death_animations_count()` | Preserve corresponding +0xfe0 getter and its cook/animation consumers. |
| 3 | `weapon::preview_animations_count()` | Preserve +0xfe4 getter. Do not erase the separate, previously recorded preview allocation/split question. |
| 4 | `weapon::is_in_scene()` | Preserve private const getter at +0xfe9. Constructor clears the flag, on_show sets it and on_hide clears it. No observed named consumer; do not add a render guard merely to exercise the helper. |
| 5 | `weapon::set_fire_pfx(queries_result&)` | Replace empty body with explicit dedicated-result/preallocated-array model: copy one unmanaged handle per fire count from result index zero. Original caller, result offset/count, allocation and failure contracts remain open; not called on the cook's combined result batch. |
| 6 | `weapon::set_shells_pfx(queries_result&)` | Separate corresponding shells model, with the same unresolved contract. Do not exchange the two arrays or change the cook's existing count/request ordering. |
| 7 | `weapon::assign_game_ui(game_world_ui*)` | Preserve private virtual direct store; retained RVA 0x9b370 writes argument to +0xfb8. This is not a current-player predicate. |
| 8 | `weapon::is_player_current()` | Replace false body with scene → game → network-client identity comparison against hit-initiator ID. Restore the private call in on_sound_event. Retail's fresh weapon/scene reads support the helper seam; original named boundary/friend spelling remains a model. |
| 9 | `weapon::user()` | Preserve private const method returning mutable player reference. Replace incomplete-type C cast with static_cast through the public offset-zero base_player base. No new null check, RTTI or ownership transfer. Existing aim/input/FOV/footstep consumers remain. |
| 10 | `weapon::get_game_scene()` | Preserve public nonconst pointer getter at +0xfc0. Do not change its PDB signature to make the private const predicate call it; that predicate accesses its own field. |
| 11 | `weapon` generated copy constructor | Raw generated 0x0103 record; leave implicit. Do not manufacture a source body or change base/member copying restrictions. |
| 12 | `weapon` generated assignment | Raw generated 0x0103 record; leave compiler-owned, including const-member restrictions. |
| 13 | `weapon::__vecDelDtor` | Generated introducing-virtual 0x0113 record, not a source declaration. Preserve the explicit virtual destructor and its ownership behavior. |
| 14 | `fingers_to_weapon_corrector()` | Restore m_first_person_view(false). Retail stores zero to +0x814; frozen base omits it. Preserve two hand constructors, 0.1f interpolator and implicit null model-handle construction. |
| 15 | `fingers_to_weapon_corrector::activate(...)` | Verify two initializer calls and destruction of by-value weapon_model argument. Retail does not assign the argument to m_weapon_model; do not add that tempting cache store. |
| 16 | `fingers_to_weapon_corrector::process(...) const` | Verify two hands, unsigned start-time + 100 threshold, active-only final copying and transition interpolation. Existing full body is live, not a STUB. |
| 17 | `fingers_to_weapon_corrector::activate_hand(...)` | Verify comparison with old active flag, then flag/time stores only when it changes. Repeated activation does not restart the transition. |
| 18 | `fingers_to_weapon_corrector::get_hand_coefficient(...) const` | Preserve active ? 1 − interpolated_value : interpolated_value, supported by both retained method and process expansion. Frozen standalone line projection still differs; not reported structurally closed. |
| 19 | `fingers_to_weapon_corrector::initialize_bones_indices(...)` | Verify two hands × fifteen names and subtraction of root-bone count. No index sentinel/default initialization added. |
| 20 | `fingers_to_weapon_corrector::initialize_locators(...)` | Verify view-flag store, sixteen locator matrices per hand, inversion, fifteen parent-relative products and 0x3c0-byte copies. Route first store through existing public setter; original named boundary is inferred. |
| 21 | `fingers_to_weapon_corrector::set_first_person_view(bool)` | Replace empty body with pure flag store, supported by initialize_locators +0xa. No view-dependent locator-name or matrix changes are inferred from the name. |
| 22 | Corrector generated copy constructor | Raw 0x0103; retain implicit generation and private noncopyable-base restriction. |
| 23 | Corrector generated destructor | Preserve destruction of model handle, interpolator and hand subobjects. No invented explicit cleanup or rendering removal. |
| 24 | Corrector generated assignment | Raw 0x0103; do not author a copy operation around the noncopyable base. |
| 25 | Corrector `__vecDelDtor` | Generated nonvirtual deletion machinery; leave implicit. |
| 26 | `fingers_to_weapon_corrector::hand::hand()` | Retail RVA 0x9cce0 writes time zero at +0x3fc and active true at +0x400. Preserve existing initializer list; matrices and bone indices are not zeroed. No other generated-method records are added to this count. |
| 27 | `weapon_sound_effect(...)` | Verify borrowed weapon reference, both supplied buffer/count groups, stop flag, counter zero and queue size. No extra allocation or copied backing arrays. |
| 28 | `weapon_sound_effect::initialize()` | Retail RVA 0x5ad2f0 stores 0xff at +0x25; preserve reset-to-sentinel, not zero. |
| 29 | `weapon_sound_effect::finalize()` | Verify stop flag then two clear_instances calls. Clearing releases intrusive references and resets ends; it does not directly call sound stop or free backing storage. |
| 30 | `weapon_sound_effect::on_sound_event(...)` | Restore private current-player call, two null-instance early returns and single-element queue erase calls. Preserve counter wrap/modulo, equality-only queue check, third-view-only position and callback/push/play order. Original statement/caller seams remain open pending a later build. |
| 31 | Sound-effect generated copy constructor | Raw 0x0103; preserve private noncopyable base, references and nested ownership; no hand-written copy. |
| 32 | Sound-effect generated destructor | Preserve nested buffer-vector destruction. Do not replace it with finalize, whose conditional policy differs from unconditional member destruction. |
| 33 | Sound-effect generated assignment | Raw 0x0103; retain compiler restrictions from base/reference members. |
| 34 | Sound-effect `__vecDelDtor` | Generated nonvirtual helper, not a new virtual destructor. |
| 35 | `weapon_sound_effect::sounds::sounds(...)` | Remove stale placeholder from already-correct initializer-only body. Emitter buffer is initially full; instance buffer initially empty. Keep all four arguments, including capacities unused in gold layout. |
| 36 | `weapon_sound_effect::sounds::clear_instances()` | Preserve sounds_instances.clear(), used twice by finalize. Do not clear the const emitter list or release storage. |
| 37 | `sounds` generated copy constructor | Raw 0x0103; leave implicit and retain private noncopyable base. No invented consumer or buffer aliasing policy. |
| 38 | `sounds` generated destructor | Preserve both member destructors; buffer_vector destroys elements but does not own/free supplied storage. |
| 39 | `sounds` generated assignment | Raw 0x0103; const emitter vector/noncopyable constraints are not permission to invent an assignment body. |
| 40 | `sounds::__vecDelDtor` | Generated nonvirtual deletion machinery; leave implicit. |
| 41 | `on_sound_finished(instances&, instance const&)` | Retail RVA 0x5ad300 finds the raw pointer then erases one element. Preserve existing no-not-found-guard behavior; the displayed ui::window find callee is an ICF representative, not evidence for UI data. |

## Raw PDB structure and access

Retail weapon is complete TPI 0x3fc89, field list 0x3fc88, size 0xff0.
weapon_core base at zero is public. Three count getters and get_game_scene
are public; other reviewed ordinary helpers are private. assign_game_ui is
private virtual (0x0005). The explicit destructor is public virtual 0x0007.
Generated copy/assignment are 0x0103, vector-deleting destructor is 0x0113.
Preserve all fields, access, declarations and virtual order.

Corrector is 0x3fc4c/0x3fc4b, size 0x818, private core::noncopyable base.
Hands start at zero, interpolator at +0x808, model at +0x810, view flag at
+0x814. The helper trio is private; setter/activate/process/activate_hand
are public. Its hand record is 0x3fc52/0x3fc51, size 0x404, with an explicit
public default constructor and public matrices/indices/time/active fields.

Sound effect is 0x44da0/0x44d9f, size 0x28, private boost::noncopyable base.
Both nested sounds at 0/+0x10, weapon reference +0x20, stop/counter/queue
bytes +0x24/+0x25/+0x26 are private. Ordinary methods are public; generated
operations are 0x0103. Nested sounds is 0x44daf/0x44dae, size 0x10, private
boost::noncopyable base, public const emitter vector and mutable instance
vector at 0/+8. No explicit copy/destructor declarations are introduced.

The complete retail player variants 0xaa26, 0x1df27 and 0x3fc8c all encode
public base_player (type 0x2dce) at offset zero. The new static_cast needs a
complete player definition, not a type-layout guess. Complete scene/game/
network-client/query-result types are included for the in-class bodies;
project-header traversal checks for a cycle back into weapon.h. This is a
source-availability check, not compiler validation.

The cook still requires its existing friendship to construct and fill
private arrays. The sound callback now uses a private declared predicate,
so the existing sound-effect friendship is no longer explained by invented
direct private-field access in that caller. Raw PDB method access constrains
the required access; it does not independently prove the original friend
declaration or the reconstructed inline boundary.

## Constructor bug and corrector consumers

Retail constructor 0x5aca00 has zero body statements and 0x3d bytes; frozen
base also has zero statements but 0x39 bytes. Both inline the interpolator.
Retail +0x34 stores AL to this+0x814 after xor eax,eax; base never writes
that flag. Thus zero statement count is not proof that initializer lists
match. The float pool operand at +0x12 resolves to RVA 0x8138b8, bytes
cd cc cc 3d (0.1f), independently checked in the original PE.

initialize_locators at 0x5ac6d0 begins with the view-flag store. The remainder
does not select different first/third-person locator names. activate at
0x5ac9b0 calls bone and locator initialization then releases its by-value
model parameter; it does not retain the model in +0x810. The hand constructor
at 0x9cce0 writes only time zero and active true. Keep these omissions.

Frozen structure checks: activate_hand 0x5ac670 and activate match; process
0x5ac8c0 has seven statements and 0xe9 bytes on both sides. Existing
initialize_bones_indices 0x5ac7f0 has three statements/0xc4 bytes on both
sides but a four-byte statement-size difference. get_hand_coefficient
0x5ac6a0 has one target return statement versus zero in frozen base, despite
both having 0x2c bytes. These are deferred structural checks, not proven
LTCG walls. Remove obsolete blanket closure/unroll comments rather than
claiming they explain the current frozen data.

## Sound callback evidence

Retail on_sound_event is RVA 0x5ad340, 26 statements/0x2ee bytes. Frozen base
has 24 statements/0x2e8 bytes. Target records params and instance locals;
the existing scene/user/first_view source locals are not all independently
certified by this record. No post-edit byte/statement result is available.

After obtaining the sound world, target +0x56 reloads the weapon, its
hit-initiator holder (+0x40c), its scene (+0xfc0), scene's game (+0xa8), game
network client (+0x3b8), current-player pointer (+8), and initiator ID (+4).
It checks the player pointer and compares that player's ID (+0x34).
The frozen direct caller used the earlier cached scene. This supports
restoring weapon::is_player_current(), not testing UI presence, ownership
pointer equality or first-person view mode.

Null handling is split into target statements at +0xdf (load/test) and
+0xe5 (exit jump), repeated at +0x1ea/+0x1f0 in the other branch. Both exit
to the common call-me-again return. Restore explicit early-return guards;
do not reinterpret the two statement rows as two separate null tests.

Queue erasure at +0x165 and +0x282 passes references to begin and begin+1
to the range overload. The existing one-iterator buffer_vector::erase
implements exactly that forwarding. The retained on_sound_finished uses
the one-element seam already and has matching one-statement/0x3c-byte
frozen structure. This supports the restored spelling, but cannot uniquely
distinguish it from an explicit one-element range in the original source.

Preserve retail quirks: modulo uses the first-view emitter count even when
selecting third-view audio; no zero-count guard is added; queue check is
equality, not >=; third-view alone sets position; callback is installed
before queue erasure, push_back and play. initialize writes counter 0xff,
distinct from constructor's zero. finalize at 0x5ad630 already has matching
two-statement/0x29-byte frozen structure, so its old extra-statement comment
was stale. buffer_vector::clear destroys its elements and resets the end;
neither it nor the vector destructor frees the supplied backing buffer.

## Particle setter models are not a recovered cook contract

The live request list contains optional scope, skeleton model, three
animation groups, shells and fire. Arrays are allocated by the cook.
Retained on_weapon_subresources_ready (0x5bd800, 22 statements/0x68b bytes)
copies the particle handles with an advancing resource_index, then calls
load_weapon; the scope member is not yet installed during these copies.
The two setters accept only queries_result&, with no explicit start index.
Calling their index-zero models on that mixed list would copy wrong data.
Do not infer the optional prefix from the still-unset m_rifle_scope.

Current models assume separate results and preallocated arrays, copy the
existing count and do not allocate, free, change counts or add failure
guards. Alternative original query/slicing/allocation contracts remain
open. No artificial query or consumer is introduced to emit the helpers.
The cook's delete_resource destroys weapon, deletes both particle arrays,
destroys trailing animation handles, and frees the object allocation;
weapon destruction removes playing particle instances, not these arrays.
cooked_object_size at 0x5bd300 is 0xff0 + four times the sum of all three
animation counts. These established owners remain untouched.

## Source-only checkpoint

Five literal markers disappear: the view setter, both particle setters,
current-player predicate and already-initialized sounds constructor. That
is not five fully verified original bodies. Five paired sushi@TODO entries
retain the setter/predicate/result/statement-boundary questions. No methods,
fields, virtual slots or forced-emission consumers are added. Static checks
cover include reachability, declarations, marker/register counts and the
source diff; they do not establish buildability or measured matching.
