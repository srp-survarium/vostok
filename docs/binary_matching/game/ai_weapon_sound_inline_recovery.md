<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# AI weapon objects and sound-player inline recovery

Source-only continuation above `19a50a0d4` in PR 571. No build or regenerated
scores. **33 additional entries**: 17 weapon-object operations (all 11 in-class
bodies, four retained constructor/destructor/spatial methods and two implicit
methods), plus 16 sound-player inline/generated/consumer operations.

All addresses below are retail RVAs. `Verified` identifies observed retail
operations; `model` identifies an older-source reconstruction whose original
inline boundary/invocation remains unverified. Missing direct xrefs do not
prove a helper unused. No new invocation is introduced for the private sound
serialization helpers.

## Individual register

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `object_weapon::cast_game_object() const` | Verified `0x9beb0`: null-preserving conversion from weapon interface to AI-game-object base at `+4`. Existing `return this` is retained. |
| 2 | `get_type() const` | Verified `0x2c580`: load `[this+0x10]` from primary weapon interface. |
| 3 | `is_loaded() const` | Verified `0x9bec0`: unsigned ammo count at `+0x1c` greater than zero, expressed by cmp/sbb/neg. Keep predicate, not a hardcoded true despite the observed constructor default. |
| 4 | `get_name() const` | Same folded `0x2c580` as type getter, but AI-game-object `this` starts at `+4`, so this loads complete-object name `+0x14`. No type/name field confusion. |
| 5 | `get_id() const` | Verified `0x9bed0`: adjusted-base `[this+0x14]`, complete-object ID `+0x18`. |
| 6 | `cast_npc()` | Verified `0x327c0`: null; installed AI-game-object slot `+8`. |
| 7 | `cast_npc() const` | Same null body, separately installed at slot `+0xc`. |
| 8 | `cast_weapon()` | Verified `0x9bb00`: AI-game-object `this -4` returns primary weapon base. |
| 9 | `cast_weapon() const` | Same adjusted leaf at its distinct const virtual slot. |
| 10 | `get_velocity() const` | Verified `0x9b8f0`: `fldz; ret`; zero is real. |
| 11 | `get_luminosity() const` | Verified `0x9bee0`: pool bytes `a6 9b 44 3b` at `0x975fc4`, exactly float `0.003f`. Existing unmarked body retained. |
| 12 | Four-argument weapon constructor | Verified `0x5ae3c0`: initialize loose-pointer control block, zero public list link, copy type/name/ID, store ammo 32 and install both vtables. Keep source parameter and default 32, not a hardcoded body constant: retail specializes away the defaulted fourth argument at this call boundary. |
| 13 | Explicit virtual weapon destructor | Verified `0x5ae370`: derived vptr stores and inherited loose-control-block release/invalidation. Empty source body correctly retains base cleanup. Raw destructor attributes `0x007` are explicit, not generated; no new novtable annotation. |
| 14 | `get_random_surface_point(u32 const) const` | Retained `0x5ae360` is `int3`, not an absent inline. Preserve existing non-returning `NOT_IMPLEMENTED`/`UNREACHABLE_CODE`, not a fabricated zero position. |
| 15 | `local_to_cell(float3 const&) const` | Retained `0x5ae350` is also `int3`; preserve non-returning form, not a fabricated transform. |
| 16 | Implicit weapon copy constructor | Raw method `0xa362`, constructor list `0xa365`, attributes `0x103`. Keep implicit; do not add custom list-link, name ownership or ammo policy. |
| 17 | Implicit weapon assignment | Raw method `0xa378`, attributes `0x103`. Keep generated; no invented cloning/ownership behavior. |
| 18 | `ai_sound_player::sounds() const` | Verified in `find` at `0x5adc80`: start `this+0x1a8`, stride `0x10`, count `+0x1a4`. Cook allocates/constructs that trailing array; destructor walks it. Existing unmarked getter retained. |
| 19 | `serialize()` | Legacy model restored: heap writer, serialize each emitter in array order, bind existing two-writer callback, forward to active sound's asynchronous serializer. Original activation, active-sound precondition and async owner lifetime remain open. |
| 20 | `deserialize()` | Legacy file-read/emitter-replay model restored, then NPC-pain emitter selection and call to existing sound-level decoder. Original activation, file/format assumptions and reader/buffer lifetime remain open. Sound-level decoder demonstrably returns null; this is not completed playback reconstruction. |
| 21 | Implicit sound-player copy constructor | Raw method `0x435da`, attributes `0x103`; keep implicit, not a manual copy through noncopyable base/references. Record presence does not establish usable copying. |
| 22 | Implicit sound-player assignment | Raw `0x435df`, attributes `0x103`; keep implicit. Reference/const members do not justify an invented assignment policy. |
| 23 | `sounds_collection_type(parent,type,emitter,priority)` | Verified `0x5ae300`: copy type/priority, initialize null child pointer, establish parent link, release by-value emitter argument. Keep explicit constructor and value parameter. |
| 24 | Implicit collection copy constructor | Raw `0x435e5`, attributes `0x103`; keep generated and let the child-resource type determine copying. No raw-memory replacement. |
| 25 | Implicit collection assignment | Raw `0x435e9`, attributes `0x103`; keep generated. |
| 26 | Implicit collection destructor | Raw `0x435e7`, attributes `0x103`; parent-unlink and resource release are visible in the player's destruction loop. Do not add a manual destructor or free the trailing element storage separately. |
| 27 | Sound-player constructor | Verified `0x5ae090`: initialize base/resource state and null active sound; store scene-handle and world-user references, producer/receiver pointers and count. Scene remains a borrowed reference, not an owning resource copy. |
| 28 | Sound-player destructor | Verified `0x5ae0f0`: release active proxy first, explicitly destruct trailing collection entries, then compiler member/base cleanup. Preserves child-parent unlink before resource release. |
| 29 | `find(sound_type) const` | Verified `0x5adc80`: linear first-match search over the tail, null when absent. Frozen structure matches four statements/`0x23` bytes; no sorted/indexed table substitution. |
| 30 | `on_active_sound_serialized(writer*,writer*)` | Verified `0x5adf50`: append sound-thread bytes to current-thread writer, save fixed file, log failure, delete only current-thread writer. Frozen base has no selected procedure: obsolete in-source claim of current structure match removed. |
| 31 | `on_active_sound_deserialized(reader*,void*)` | Verified `0x5adc60`: `ret 8`, no reader/buffer cleanup. Preserve existing unreferenced-parameter body; do not infer deletion from callback name. |
| 32 | `tick()` | Verified `0x5adc70`: `ret`. Do not revive older debug-key bindings here to make serialization helpers reachable. |
| 33 | `on_finish_playing()` | Verified `0x5adc50`: return zero (`command_result_executed`); it does not clear/release active sound. Preserve that distinction from explicit `clear_resources`. |

## Weapon identity, callers and PDB access

Target class `0xa37b`, field list `0xa37a`, has a public `LF_MEMBER m_next`
(attributes `0x0003`, offset `0xc`). Type/name/ID/ammo are private. The frozen
target/base class comparison is identical, size `0x20`, one variant and record
each. The older header at `885e1d4a4:temp/game_legacy/object_weapon.h` also has
a public link and no NPC friendship. Remove the reconstructed friend whose
comment claimed that already-public link was private; keep visibility/order.

Constructor-installed table pointers, decoded from the PE instruction bytes:

| Interface | Target table | Frozen base table | Relevant slots |
|---|---:|---:|---|
| `ai::weapon`, `+0` | `0x9633dc` | `0x956900` | cast, type, loaded, deleting destructor |
| `ai::game_object`, `+4` | `0x9633f0` | `0x956914` | name/ID, NPC/weapon casts, velocity/luminosity, two spatial traps, destructor thunk |

All 11 inline leaves were selected independently by full name in both rich
indexes; their frozen instruction streams agree. The equal type/name code at
`0x2c580` accesses different fields because the incoming base pointers differ.
The base luminosity pool at `0x9675ec` has the same four bytes as retail, not
merely an equal delinker symbol name. Constructor frozen structure is zero
statements/`0x5a` bytes on both sides; this is not a build of the cleanup edits.

`game_world::on_npc_attributes_received` creates these objects from weapon
type/name/ID and relies on the default ammo argument, then inserts them into
the NPC attribute list. `human_npc::get_available_weapons` traverses that list
and hands out AI weapon pointers. Attribute assignment swaps lists; world
cleanup pops and deletes separately. No insertion/deletion policy moves into
the leaf getters or generated methods, and `m_name` remains borrowed `pcstr`.

## Sound serialization: a model, with its real limitations

Both private helpers have bodies in the older
`ce6adb983^:temp/game_legacy/ai_sound_player.cpp` (starting at lines 304/320).
The proposed S/D-key calls in that same file are **commented out**, inside its
non-gold debug control region. Retail's retained `tick` is empty. Neither the
rich-name/operand scan nor current independent source consumers establish an
active retail call to either helper. That evidence supports preserving their
private declaration and restoring a model, not saying they never mattered or
were editor/server-only.

Bodies are defined `inline` in the existing implementation file, beside their
private callback/resource dependencies; declarations remain inline/private in
the header. No new header identity or reachability binding is invented. This
placement follows the older implementation location but remains source-prior,
not a recovered retail inline line record.

Serialization constructs the current-thread writer and appends emitter state
before invoking the active proxy. The existing proxy serializer appends its
state and queues a copied callback/writer through the sound world. The sound
world creates its own writer and returns both via `on_propagators_serialized`.
The game callback appends those bytes, saves `Z:/test.sound_player`, and deletes
the current-thread writer. The response then queues `on_proxy_serialized` to
delete the sound-thread writer with its own allocator. A stack-local game
writer or eager deletion on return would violate that asynchronous lifetime.
The lifetime of the bound player and original invocation still need proof.

The persistence failure string was read directly from retail PE RVA
`0x963434`: `unable to write file [Z:/test.sound_player]`. This is an existing
debug-file format, not a new save-game format. The deserialize model preserves
the older file-size narrowing to `u32`, unchecked read, emitter iteration,
NPC-pain selection and callback bind; it does not add new error recovery.

Crucially, retail `sound_instance_proxy::deserialize` at `0x152b00` follows
its disabled unreferenced-argument branch with construction of a **null proxy**.
It does not deserialize a proxy or invoke the completion callback. The game
completion at `0x5adc60` also does no freeing. The restored legacy model can
therefore allocate a reader/buffer with no observed release path and replace
active sound with null if invoked against this decoder. This limitation stays
explicit: do not silently enable the commented decoder, add cleanup to the
retained empty callback, claim successful replay, or invent a caller.

## Tail storage, generated cleanup and scene lifetime

Sound-player target record `0x435e2`/field list `0x435e1` is identical to the
frozen base class, size `0x1a8`. Raw base access is public `ai::sound_player`,
private noncopyable. Its constructor list `0x435db` distinguishes the generated
copy (`0x103`) from explicit five-argument constructor (`0x003`). Collection
record `0x435ec`, field list `0x435eb`, size `0x10`, similarly distinguishes
explicit constructor from generated copy/assignment/destructor. Existing
source representations are preserved; the base destructor's actual vptr store
also gives no basis for adding `VOSTOK_NOVTABLE` to the AI sound-player base.

The cook at `0x758a90` allocates object plus `count * 0x10`, placement-constructs
the player and each collection record, and publishes the combined resource.
The player's destructor explicitly destroys the trailing entries because an
ordinary member destructor cannot own an allocation beyond `sizeof(*this)`.
The generated collection destructor handles child-parent unlink/refcounting;
outer allocation release remains the cook's responsibility.

There is a target lifetime hazard to preserve, not repair: the cook has a
named local `sound_scene`, passes its stack address at `+0x148`, and the
constructor stores that pointer at player `+0x194`. The cook releases the
local resource handle at `+0x292` and returns. The field really is a reference
to that handle, not a by-value resource copy. Source retains this shape;
changing it to fix dangling-handle risk would change retail semantics and the
recorded member type.

Static review checked filesystem open/read APIs, writer/reader constructors,
sound scene typedef identity, callback signatures, child-resource access and
allocator declarations. Two source questions are paired in `review_todos.md`.
Twelve literal markers are removed (ten already-bodied weapon leaves, two
sound helper models). Exact new code generation and inline placement remain
for the deferred build; the whole-engine reconstruction is not complete.
