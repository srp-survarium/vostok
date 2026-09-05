<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Update, hit, relocation and match-option packet helpers

Source-only follow-up to PR 569, in PR 571. No build or score refresh.
The target and base observations below use the existing rich indexes and PDBs;
they are not measurements of these edits.

## Evidence and scope

All six structures have public data/methods in the target records. Frozen
two-PDB topology reports identical semantic records for `client_player_update`,
`weapon_state`, `hit_info`, `relocate_item_descr` and `match_options`.
`server_player_update` has one target record versus two equal base records,
but only one semantic variant on either side. Record multiplicity is not a
layout or access change. No visibility change is needed here.

The missing direction of each packet operation is reconstructed from the
retained opposite direction. This establishes a compatible wire model, not
the original missing body's assertions, guards, source partition or consumer.
Source searches found the consumers listed below, not consumers of all six
new bodies. Missing standalone symbols do not establish server/editor-only
ownership, and the new bodies are not claimed as verified inline expansions.

`packet<udp_match_packet>::append(pcstr)` writes one byte of length followed by
that many bytes, **without a NUL**. `r_string` adds the terminator locally.
Use `fixed_string::c_str()` and cast a raw character array to `pcstr`; do not
serialize the backing array or invent a different string protocol.

## Individual register: 22 declarations/retained consumers

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `client_player_update::deserialize` | Restore the inverse model: input reader, state reader, u32 time. Original receiving consumer remains open. |
| 2 | `client_player_update::serialize` | Existing body retained. Target RVA `0x771190`: three statements call input writer, state writer and four-byte append in that order. Live caller is `network_client::send_player_inputs`. |
| 3 | `client_player_update` implicit constructor | Keep generated. `send_local_player_input` RVA `0x5b4250`, statement 3, calls `player_input` construction then copies the default temporary into the vector. No extra state/time zeroing is observed. |
| 4 | `server_player_update::deserialize` | Existing body retained. Target RVA `0x7711d0`: three statements call input, state, weapon readers. Used by `network_client::process_player_action`. |
| 5 | `server_player_update::serialize` | Restore the inverse model by delegating to those three writers; original sending consumer remains open. |
| 6 | `server_player_update` implicit constructor | Keep generated. `process_player_action`, statement 3, calls `player_input` and `weapon_state` constructors at offsets 0 and `0x58`; do not add whole-object zeroing. |
| 7 | `server_player_update::operator=` | Keep generated. Retained RVA `0x862d0` copies 20 input bytes, 17 state dwords, calls `weapon_state::operator=` at `+0x58`, and returns this. Do not replace it with an aggregate memcpy. |
| 8 | `weapon_state::weapon_state()` | Existing empty body retained. RVA `0x590c40` has no field initialization; do not initialize slot/state bytes merely because it seems safer. |
| 9 | Extra `weapon_state` copy-constructor declaration | Remove the undefined handwritten declaration and allow implicit memberwise copying. No target declaration/procedure or source definition was found for it. Absence does not prove the original author relied on generation; that source-form question remains open. |
| 10 | `weapon_state::operator=` | Existing three byte assignments and return retained, without a self-assignment guard. Target RVA `0x590c00`. |
| 11 | `weapon_state::deserialize` | Replace three `r<bool>` spellings with `r<u8>` for the three unsigned-byte members; target stores AL at offsets 0, 1, 2. Shared helper aliases are discussed below. |
| 12 | `weapon_state::serialize` | Restore three one-byte appends in slot/ammo-slot/state order. Inverse model; original writer consumer remains open. |
| 13 | `hit_info::hit_info()` | Existing empty body retained. RVA `0x72ea50` constructs the two fixed strings, not the scalar/pointer fields. Called by `process_player_hit` before deserialization. |
| 14 | `hit_info::hit_info(arguments)` | Existing initializer list retained. RVA `0x72ea80` copies both strings, bullet pointer, amount, armor piercing and the two byte IDs. |
| 15 | `hit_info::deserialize` | Retained RVA `0x72eaf0`: two byte reads, separate 16-byte string locals and assignments, two floats, then `bullet = NULL`. Restore `r<u8>` for IDs; preserve local buffers and statement order. |
| 16 | `hit_info::serialize` | Restore byte IDs, two length-prefixed strings and two floats. Do not send the process-local bullet pointer. Inverse model; original sender remains open. |
| 17 | `relocate_item_descr::serialize` | Existing six appends retained. RVA `0x97680` proves five u32 writes followed by u16 amount. `lobby_client::move_item` iterates descriptions and sends them through this method. |
| 18 | `relocate_item_descr::deserialize` | Restore five u32 reads and a u16 amount. Inverse model; original receiving consumer remains open. |
| 19 | `match_options::match_options` | Existing initialization retained. RVA `0x97d40` constructs 20 profiles and initializes the listed options/sentinels and first name byte; `map_id` is not initialized. No blanket zeroing. |
| 20 | `match_options::deserialize` | Existing eight-statement reader at RVA `0x987c0` retained. `network_client` reads options here; profiles arrive separately. The reader resets match ID and received-player count instead of reading them. |
| 21 | `match_options::serialize` | Restore only map ID/name, byte mode, player count, victory-item count, respawn time and u16 match time. No profiles, local sentinels or wait/countdown settings. Inverse model; original sender remains open. |
| 22 | `network_client::send_local_player_input` | Correct the existing consumer's construction order: push a default update, take a reference to back(), then assign input/state/time there. Retail RVA `0x5b4250` copies the default temporary and advances vector end before assigning the last element. Frozen base instead assigns a local then pushes it. |

The implicit client/server constructors have no standalone rich procedure;
their recorded class methods and the concrete construction sites supply the
evidence. Other implicit trivial operations are left to the compiler, not
fabricated as additional target-named functions. `player_input` was already
registered in the input/RNG bundle and is not counted again here.

## Correct types and actual consumer order

Separate rich-index queries for `packet_reader::r<unsigned char>` and
`packet_reader::r<bool>` both return RVA `0x7e950`. Therefore a call printed as
`r<bool>` cannot establish that the original source requested a boolean.
The enclosing structures record `u8` members, the reads advance by one byte,
and consumers use their values as slot/player IDs. Restore unsigned-byte
spelling without claiming that this compiler necessarily normalized every
old bool value to 0/1 or that the edit has already changed machine bytes.
Old comments asserting that the COMDAT's first alias proved bool are removed.

The update-queue mismatch was found by following the generated constructor
into its actual owner, not by searching only for standalone inline methods.
Target statement 3 (`+0x50`) constructs and copies a default record into the
queue, then increments the end at `+0xa7`; statement 4 loads the new end, and
the subsequent stores address the last element. The frozen base's statement 3
only constructs a local. The two-sided structure diff has TRGT_ONLY and
BASE_ONLY rows despite both sides having seven statements. Correct the
source sequence; no score or fresh partition claim is made without rebuilding.

## Wire extents are not object extents

| Packet payload | Wire bytes | Object bytes | Reason |
|---|---:|---:|---|
| Client update | 44 | `0x5c` | input 20 + state position/yaw/pitch 20 + time 4 |
| Server update | 43 | `0x5c` | input 20 + state 20 + weapon state 3 |
| Weapon state | 3 | 3 | Three byte IDs/state values |
| Hit info | 12 + both string lengths | `0x48` | Two IDs, two length bytes, two floats; pointer/padding excluded |
| Relocate description | 22 | `0x18` | Five u32 + one u16; tail padding excluded |
| Match options | 8 + map-name length | `0x229c` | Only the fields read by the retained options reader |

Small Python `struct` model checks cover IDs above 1/127, amount values across
the 8/16-bit boundary, update sizes and empty/31-character map names. They
confirm the proposed wire arithmetic only, **not C++ execution or retail
round trips**. No compiler, linker, delinker or score derivation was run.

The adjacent player/profile/item serializers are a separate remaining bundle.
The unresolved `player_templates` table is not inferred from packet contents
or filled with fabricated values in this change.
