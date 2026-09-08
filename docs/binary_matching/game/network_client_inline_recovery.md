<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Network-client inline handlers, virtual defaults and connection state

PR 571 source-only review. This covers the thirteen remaining in-class bodies
in `network_client.h`; `match_options` was already counted in the game match-
client register. Eleven related generated methods/consumer anchors make 24
new register entries. Two literal placeholders gain bodies; two remain open.
No build or score refresh.

## Real non-returning hook discrepancy

The constructor at RVA 0x6f5000 installs network-client vtable RVA 0x95cc3c;
slot 1 points to `load` at 0x9ea00. Raw bytes there are:

```text
8b 44 24 08 8b 4c 24 04 50 51 e8 01 08 fa ff 83 c4 08 cc cc ...
```

Retail copies the two supplied arguments into an unguarded helper call, adjusts
the stack and reaches trap padding: **no returning epilogue**. The call target
is RVA 0x3f210, a bare return; this is not a hidden non-returning callee. An
independent exact-name frozen-base fetch has the same shape followed by
`ret 8`. Thus the current body incorrectly permits normal return.

Retain argument-use behavior and add the existing `UNREACHABLE_CODE()` idiom.
In the shipping macro branch this is `__assume(0)`. This is an explicit model
for the observed non-returning source boundary, not proof of the original
assertion spelling or debug-build behavior. Do not replace it with a useful
scene loader or a silently returning no-op. No post-edit bytes are claimed.

## Individual register: 24 declarations/consumer anchors

| # | Function | Disposition |
|---:|---|---|
| 1 | `network_client::load` | Preserve the two argument copies and model non-returning completion. Installed slot and raw extent prove the old returning behavior differs; original assertion form remains open. |
| 2 | `has_bandwidth` | Existing true body verified. Slot 5 points to 0x12700, `mov al,1; ret`. No speculative bandwidth calculation added. |
| 3 | `initiate_respawn_current_player` | Existing empty body verified. Slot 11 points to bare return 0x3f210; do not add an outbound request merely from the method name. |
| 4 | `login_client` | Existing reference accessor verified, slot 14/0x82190 returns this+0x20. |
| 5 | `lobby_client` | Existing reference accessor verified, slot 15/0x821a0 returns this+0x218. |
| 6 | `match_client` | Existing reference accessor verified, slot 16/0x821b0 returns this+0xa88. |
| 7 | `messaging_client` | Existing reference accessor verified, slot 17/0x821c0 returns this+0x2e28. |
| 8 | `on_player_hit_received` | Existing empty override verified, slot 12/0x12c50 is ret 4. Distinct from live packet `process_player_hit`; do not duplicate damage application. |
| 9 | `get_local_player` | Existing by-value pointer return retained. `game_world_ui::on_player_killed` reads client+0x3cb8 and increments the player's +0x1f0 reference count before comparing team, with later temporary cleanup. Preserve ownership; do not change to a borrowed raw/reference return. |
| 10 | `is_player_connected` | Restore array-slot flag getter and use it in the respawn consumer. Target reads this+0x3c1c+8*id, independently of whether the resource pointer is nonnull. Original named inline boundary remains an inference; no new bounds/fallback policy. |
| 11 | `on_player_death` | OPEN wire/dispatch hypothesis. Likely death notification decoding, but the current 0x83 handler is a retained process_player_kill with victim/killer/headshot/item fields. No consumer chooses this older hook's payload, UI/kill split or forwarding boundary. |
| 12 | `create_new_player` | OPEN per-player creation hypothesis. Current live profile path collects the complete profile set, then asynchronously loads all player resources. Original per-player payload, profile ownership, query callback, connection flag and acknowledgment policy are unknown. No invented duplicate asynchronous pipeline. |
| 13 | `destroy_player` | Restore byte-ID model forwarding to destroy_player_impl. Other live player events use byte IDs and the retained implementation takes u8. Original message binding/width and any outer connection-flag update remain explicitly open; no new switch case is assigned. |
| 14 | `player_desc` constructor | Existing pointer-null/flag-false initialization verified at 0x82180. Header body is not an inert constructor; both fields are initialized. |
| 15 | `player_desc` implicit destructor | Preserve generated resource-pointer destruction. Retained array destructor iterates 20 elements backward at 8-byte stride and releases each resource. No scene removal or connection-flag operation in this implicit destructor. |
| 16 | Network-client constructor, descriptor/vtable ownership | Installed vtable at +0x17; +0xa7..0xc9 invokes the descriptor constructor iterator for 20 elements of size 8 at +0x3c18; +0xd4 nulls local-player reference. No whole-array byte memset is substituted. |
| 17 | `process_player_respawn` | Keep deserialize(packet), restore is_player_connected(id) boundary around the verified flag test, then retain flag write/conditional attachment. Remove the false comment claiming the argument is the network client. |
| 18 | Player constructor/deserialize virtual identity | Player ctor installs primary vtable 0x95a2bc; slot 25 points to actual player::deserialize RVA 0x5d52c0. This independently names the respawn virtual and disproves the old placeholder allegation. |
| 19 | `process_sync_response` flag producer | Target 0x5b5a40 reads u32 connected mask, updates 20 flags with (mask & (1 << i)) != 0 at 8-byte stride. Resource existence is not the connection-state oracle. Signature-variant conflict is recorded below. |
| 20 | `process_player_profile` | Existing bulk-profile deserializer/count increment and query_players-on-completion retained. Dispatcher 0x92 calls this method, not the missing one-player creation hook. |
| 21 | `query_players` | Existing full-set request builder retained: profile-backed creation data, resource slots cleared and connection flags false before async load. This is a semantic lead, not proof of create_new_player's former wire format. |
| 22 | `on_match_packet_received` | Raw jump table verified for bytes 0x81..0x9e. Existing kill/respawn/profile dispatch preserved; holes 0x8d..0x90 are zero entries, not hidden handler implementations. |
| 23 | `process_player_kill` | Retained decoder reads u8 victim, u8 killer, bool headshot, u32 item; updates UI, then conditionally kills the loaded/inserted/alive player. Do not blindly alias the older on_player_death to this seven-byte schema. |
| 24 | `destroy_player_impl` | Retained cleanup gets a temporary owning player pointer, clears resource slot, removes inserted player, resets matching local/current pointers and hides quick slots as needed. It does not clear the connected flag; preserve that distinction in the model and record outer policy as unknown. |

## The respawn comment was wrong, not its deserialize call

`process_player_respawn`, RVA 0x5b5bf0, starts with EBP=network client from the
stack and ESI=packet reader from EAX. It advances that reader by one byte for
the ID, obtains the player via virtual +0x48, then calls player slot +0x64 at
+0x35 with **ESI** pushed. ESI is still the reader, not this. The player's raw
installed slot is exactly `player::deserialize(packet_reader&)`.

The old source comment called this an unidentified virtual and a fake stand-in
call. That allegation is removed. No invented base_network_client-taking player
virtual, override, cast or different deserialization stage is needed.

## Dispatcher and unresolved old packet helpers

The jump table lives at RVA 0x7b0620, inside the 0x7b0400 dispatch procedure.
The relevant observed entries are:

| Message | Destination RVA | Operation |
|---|---|---|
| 0x83 | 0x7b048f | call process_player_kill |
| 0x84 | 0x7b04a2 | pass reader and this to process_player_respawn |
| 0x8d..0x90 | literal zero entries | no handler |
| 0x92 | 0x7b042e | call process_player_profile |

No range/default safety behavior or replacement message IDs are invented here.
Absent standalone records alone would not settle inline use. This audit also
checks the actual dispatch entries and the retained kill/profile/respawn
operations, but it still does **not** prove the old helpers were globally
unused, server-only or editor-only. The two unbodied handlers retain individual
source TODOs with their precise wire/ownership gaps.

The destroy model reads one byte and calls the existing cleanup owner. It
does not broaden that owner by clearing a separate network flag or sending an
acknowledgment. These may have belonged to the original packet helper, but no
binding establishes them. The packet-reader header is included explicitly so
the new inline body does not depend on a transitive/PCH definition.

## Conflicting class variants: do not claim whole-class equality

Frozen topology reports four target records representing two semantic variants,
versus one base record. Raw TPI inspection isolates the difference:

- Target class TIs 0x1df92/0x47ea8 use a no-argument
  `process_sync_response` member type. The first field list is 0x1df91.
- Target class TIs 0x7164e/0x71650 use its reader-taking type. The first field
  list is 0x7164d; base matches this variant.
- The differing field-list word at offset 0x7a0 is method type 0x1df69 versus
  0x1df7c. Their LF_MFUNCTION parameter counts are 0 and 1, respectively;
  argument-list TIs are 0x1006 and 0x6431.

The retained procedure, its packet load, and the dispatcher establish the
reader-taking live interface. Keep it; do not fabricate a no-argument overload
or delete the packet read to satisfy both record variants. Why the other class
projection differs remains a source/producer question with a lean declaration
TODO. No whole-class 100% assertion or new linker-wall classification.

## Deferred validation

Check the load hook's non-returning extent and argument copies, the restored
connection getter/caller, byte-ID deletion model and all changed-header users
when builds are authorized. No successful C++ compilation or post-edit byte
match is claimed. The packet schema, const/argument and variant questions above
remain open; this pass does not complete the engine-wide reconstruction goal.
