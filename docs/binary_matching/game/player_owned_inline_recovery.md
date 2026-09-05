<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Player-owned records, profile cleanup and input predicate

PR 571 source-only audit. This follows the already-reviewed player and input
interfaces into their own records/local functor. No build or score refresh.
Three no-source markers are removed after consumer/assembly verification;
no additional runtime behavior is needed in this batch.

## Individual register: 13 declarations/consumer anchors

| # | Function | Disposition |
|---:|---|---|
| 1 | `client_player_history_item` constructor | Keep existing empty body. Exact-name retail fetch calls `player_input` construction at +0 and `weapon_state` construction at +0x58. The implicit `action` member construction performs both; no timestamp or full transform zeroing is emitted. |
| 2 | `client_player_history_item` destructor | Keep existing empty body. Exact-name target fetch is a bare return. The record has no owning resource reference; no history unlink or buffer free is added here. |
| 3 | `client_player_state` implicit constructor | Keep generated. The player constructor constructs both animation players and nulls their model references. No explicit state ctor is needed merely because a generated PDB header lists one; do not zero transform/controller/pitch fields without observed stores. |
| 4 | `client_player_state` implicit destructor | Keep generated. Target `0x7e850` releases model at +0x85c8, then resets/destroys the animation player/tree. The raw physics-controller pointer is not deleted here. |
| 5 | `player::serialize_current_state` | Verified history writer: reserve new history item, assign time at +0x5c, copy input at +0, target transform at +0x14, pitch at +0x54 and active slot at +0x58. The history constructor does not own those later assignments. |
| 6 | `player` constructor, state-member expansions | At +0x3e/+0x6e selects state objects at player +0x228/+0x87fc; +0x5f/+0x74 construct animation players, +0x64 and +0x81 null the two model references. Preserve generated member construction and later explicit initialization. |
| 7 | `profile_player_character` constructor | Retained `0x72f5c0`: null resource pointer at +0 and copy borrowed lobby reference at +4. Existing initializer list lets resource construction provide the null. |
| 8 | `profile_player_character` destructor | Remove stale marker, retain empty body. The lobby deletion expansion decrements the owned player reference and frees the 8-byte wrapper. It does not invoke `player::remove` or `clear_resources`. |
| 9 | `profile_player_character::clear_resources` | Retained `0x72faa0`: if player exists, remove it from the scene; then reset the owning reference. Distinct from destructor-only release; no extra clear call is put into the destructor. |
| 10 | `lobby_menu` destructor, profile deletion expansion | RVA `0x78b0d0`, statement 6: wrapper at +0xe8, member pointer decrement at +0xa7, wrapper free at +0xb8 and owner pointer null at +0xbd. No hidden scene-removal operation in this expansion. |
| 11 | `first_predicate<T>` constructor | Remove stale marker. The single enum field is copied into the by-value predicate argument consumed by retained `__find_if`. Preserve constructor's `T const&` interface and value-owned field. |
| 12 | `first_predicate<T>::operator()` | Remove stale marker. Retained `__find_if` at `0x86930` compares pair offset +0 with the captured enum at each 8-byte stride and selects equality. It does not test pair.second/action state. Generic `P` template spelling is an explicit source-form question. |
| 13 | `player_input_handler::process_first_person_mode`, predicate consumer | Retained `0x5bff10`; e.g. statement 3 pushes enum 0x0b (back) into `__find_if` and compares its result with end. Existing caller and frozen candidate have matching 82-statement/0x512-byte partitions; no extra action_present rewrite or caller reshaping is justified. |

## Why the empty bodies remain

An empty source body is not the same as an empty emitted function. History
construction must still construct its `server_player_update` member; profile
destruction must release its `player_ptr`; client-state destruction must tear
down model and animation members in reverse declaration order. Adding explicit
duplicate work would change these semantics. Conversely, the history destructor
really is a bare return, and neither record destructor owns a history buffer.

For profile deletion, `lobby_menu::~lobby_menu` supplies direct evidence even
though `profile_player_character::~profile_player_character` has no standalone
rich procedure. The retained explicit clear method has a `player::remove` call;
the deletion expansion has only reference release and allocation cleanup. That
rules out adding clear_resources to this destructor to make it look useful.
The borrowed lobby reference is not destroyed or freed.

The separate `lobby_menu::clear_resources` remains an existing out-of-line stub.
Its scene/project cleanup is not restored by this inline audit; do not report
whole lobby cleanup complete because wrapper destruction has been verified.

## Predicate and access limits

Raw target type data records the enum constructor and field but omits the
operator declaration. Assembly establishes equality on pair.first, while the
current generic `template<typename P>` signature is a compatible reconstruction
of the observed instantiation, not proof of its original generic spelling.
Keep that specific question instead of a misleading no-source body marker.

Frozen topology reports one identical semantic variant for history item,
client state and profile wrapper. The later
[raw-base audit](../pdb_base_access_rendering.md) confirms private inheritance
for history/profile in both actual PDBs. Their generated public spelling is a
renderer defect; those source questions are closed without access changes.

No new scalar initialization, resource owner, virtual function, friend, explicit
client-state special member or copy operation is introduced. Post-edit codegen
validation remains deferred under the no-build instruction.
