<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Key-binder inline and consumer recovery

Source-only continuation above `82981c081` in PR 571. **28 additional entries**:
all five missing inlines, three generated methods, fourteen retained methods
and six consumer anchors. No build, regenerated ledger or new score claim.
Addresses below are retail RVAs unless explicitly labeled frozen base.

The evidence distinguishes observed operations from original source boundaries:
retail mouse consumers read the existing sensitivity/inversion globals, but
that alone cannot prove the original getter calls. Restore the declared getter
seams and record that inference. Likewise, two private helpers have concrete
older implementations but no established retail consumer. They are models,
not recovered standalone procedures or proven-unused functions.

## Individual register

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `key_binder::GetActionAllBinding` | Open formatting contract. Older commented sketch selects the named action, copies both localized key names, and sketches joined output/unbound localization. No live destination-buffer consumer found. Preserve exact PDB capitalization; do not turn an unfinished localization/buffer policy into a verified no-op. |
| 2 | `mouse_sensitivity() const` | Replace zero with existing `g_mouse_sensitivity`. Retail player mouse, first-person keyboard-look and lobby mouse operations read this console-controlled global. Restore three caller seams; original named boundaries remain indexed. |
| 3 | `mouse_invertion() const` | Replace false with existing `g_mouse_invert`; restore two player-input caller seams. Retail tests the flag and negates vertical sensitivity. Do not add inversion to lobby camera, which lacks that operation. |
| 4 | Private `keyname_to_dik(pcstr)` | Restore older unchecked `keyname_to_ptr(name)->dik` model with a named local. Original invocation and valid-name precondition remain open; no invented null-to-zero policy or new call site. |
| 5 | Private `is_binded(action,dik)` | Restore older two-slot null-guarded DIK comparison, adapted from global bindings to retail's member array. Original nonconst private consumer remains open; const `get_binded_action` cannot directly invoke it. |
| 6 | Implicit copy constructor | Raw `0x15123/0x103`, constructor list `0x15125`; preserve implicit declaration through private noncopyable base, not a custom copied binding system. |
| 7 | Implicit assignment | Raw `0x15143/0x103`; keep generated, no invented game-reference rebinding. |
| 8 | Implicit destructor | Raw `0x15128/0x103`; retain implicit nonvirtual destruction. Do not delete borrowed descriptors, game, or function-static console commands here. |
| 9 | Explicit constructor | Retained `0x5cb6e0`: store game reference, clear 64 binding records, populate action pointers, initialize four function-static commands with guards/atexit, apply defaults. Preserve first-construction command capture and generated ownership. |
| 10 | `bind_key(args,bind_number)` | Retained `0x5cb4a0`; existing parse/remap/action/key lookup, slot assignment and conflicting descriptor-pointer clearing remain. The conflict loop compares descriptor identity and group overlap, not `is_binded`'s DIK predicate. |
| 11 | `unbind_key(args,bind_number)` | Retained `0x5cb450`; name-to-ID followed by selected binding-slot clear. No guessed validation or helper call added to change retained invalid-input behavior. |
| 12 | `set_default_controls()` | Retained `0x5cb670`; walks action defaults and calls existing bind implementation. Preserve current real command path. |
| 13 | `dik_to_ptr(dik,bSafe)` | Retained `0x5cb1b0`; descriptor-table lookup and null on miss. Surviving code does not use `bSafe`; no new logging/assertion policy introduced. |
| 14 | `get_binded_action(dik,mask,group) const` | Verified `0x5caf60`: 64-record walk, action/group guards, two DIK checks, writes action mask then returns ID; miss `0x41`. Keep separate slot branches and const qualification. Similar checks do not prove a call to nonconst `is_binded`. |
| 15 | `id_to_action_name(id) const` | Retained `0x5cb350`; current table search, diagnostic and null fallback preserved. No fabricated default action name. |
| 16 | `dik_to_keyname(dik)` | Verified `0x5cb1f0`: call `dik_to_ptr`, return descriptor's name or null. Preserve safe name lookup, distinct from the older unchecked inverse model. |
| 17 | `get_binding_group(id)` | Retained `0x5caf50`; existing action descriptor group getter retained, no new bounds/null policy. |
| 18 | `get_action_dik(action,idx)` | Restore older indexed/fallback model. Both retail consumers use primary then secondary then zero; use `-1` at both existing callers to select that older fallback contract. Original selector spelling/general unobserved contract remains inferred, not proven by argument elision. |
| 19 | Private `keyname_to_ptr(name)` | Retained `0x5cb080`; case-insensitive descriptor lookup, diagnostic/null on miss. This actual null result makes the unchecked inverse model's precondition significant. |
| 20 | Private `action_name_to_id(name)` | Verified `0x5cb330`: descriptor lookup, ID at `+4` or `kNOTBINDED` (`0x41`). Do not conflate missing action with ID zero. |
| 21 | Private `action_name_to_ptr(name)` | Retained `0x5cb210`; current case-insensitive action-table search and null/diagnostic path retained. |
| 22 | Private `remap_keys()` | Retained `0x5caff0`; localized keyboard-name producer writes each descriptor's 128-byte buffer, falling back to canonical name. Does not establish the missing combined-output helper's formatting contract. |
| 23 | `player_input_handler::on_mouse_move` | `0x5bfaf0 +0x1e/+0x51`: sensitivity load and inversion test. Restore both key-binder getter calls without changing FOV, aspect, multipliers, signs or scroll handling. Frozen structure previously matches six statements/`0x104` bytes; not a post-edit verdict. |
| 24 | `player_input_handler::process_first_person_mode` | `0x5bff10 +0xb3/+0xe6`: same two global operations for keyboard-driven look. Restore both getter seams; do not enable previously absent mouse movement or change action predicates. |
| 25 | `lobby_camera::on_mouse_move` | `0x5b3d40 +0x57`: sensitivity global only. Restore sensitivity seam; no inversion getter, extra FOV factor or private world access added. |
| 26 | `console_command_bind::save_to` | `0x81a40 +0x10`: accesses binder's selected keyboard slot in public array, then name guard and serialization. Remove unnecessary reconstructed binder friendship; method does not need private game/lookup access. |
| 27 | `game_options::reset_bindings` | `0x6f5be0 +0x29..+0x58`: inlined primary/secondary/zero selection, then DIK-to-name search. Caller now selects legacy fallback with `-1`, preserving observed behavior instead of requesting slot zero. |
| 28 | `game_world_ui::create_slot_value` | `0x5c1e20 +0xac`: calls retained `get_action_dik`, whose body contains primary/secondary/zero selection, then `dik_to_ptr`. Caller now uses the same legacy fallback selector. |

## Raw class/access evidence

Frozen target/base `key_binder` topology agrees, size `0x304`, one complete
record and semantic variant each. Raw retail class `0x15146`, field list
`0x15145`, encodes private noncopyable inheritance (access 1, offset zero),
public `m_key_bindings[64]` at zero and private `m_game` at `+0x300`.
The generated header's public-base rendering is not the raw access verdict.

`keyname_to_dik`, `keyname_to_ptr`, `action_name_to_id`, `action_name_to_ptr`,
`remap_keys` and `is_binded` have private attributes `0x001`. Public getters,
including sensitivity/inversion, have `0x003`. Generated destructor/copy/assign
carry bit `0x100`; no user-written empty destructor is required.

Related data records are also checked: `keyboard_key_descr` (`0x1516c`, fields
`0x1516b`) has name/DIK/local-name at `0/4/8`, size `0x88`; `key_binding`
(`0x1516f`, fields `0x1516e`) has action plus two pointers, size `0xc`;
`game_action_descr` (`0x15171`, fields `0x15170`) has name/ID/group/action-mask/
default-key at `0/4/8/0xc/0x10`. No new constructor or hidden field invented.

The `console_command_bind` friend was introduced during reconstruction to
permit an already-public array access. Remove that friend and unused forward
declaration; keep all real visibility/order. This is not an inference from PDB
omitting friendship records alone.

## Globals and getter propagation

The existing definitions and console registrations remain in `key_binder.cpp`.
Move their duplicate consumer-side extern declarations to the owner header,
where the inline getters need them. No new global definition or duplicated
console state is introduced. The retail sensitivity operand points to RVA
`0x9b8950`, bytes `00 00 80 3f` (`1.0f`); inversion points to `0x4ba2d9d` in
the zero-filled virtual tail of `.data`, initially false. The getters must read
mutable state, not return these initial constants.

Current game accessors already expose `get_key_binder()`. Propagate that seam
at three sensitivity and two inversion uses. All observed arithmetic and
input-handler ordering remain unchanged. Since a global-backed getter need
not retain a load of its `this`, absent binder loads in optimized retail cannot
disprove this source form; they also do not prove its named original boundary.
Both getter questions are kept explicitly open for deferred comparison.

## Private models and the indexed-selection constraint

The older source at `885e1d4a4:temp/game_legacy/key_binder.cpp` supplies two
actual bodies: unchecked name-to-DIK at lines 197–201 and two-slot `is_binded`
at 215–225. Adapt only the latter's storage from old global bindings to the
PDB-proven member array. No live private call is invented. The const qualifier
on `get_binded_action` rules out directly replacing its loop with a call to the
nonconst predicate; bind's pointer-identity/group conflict checks are a different
contract as well.

The same older file has `get_action_dik(action,idx)` selecting a specific slot,
or trying both when `idx == -1`. The reconstructed body discarded `idx`, and
both reconstructed callers passed zero. Retail evidence is stronger than that
reconstruction: the standalone `0x5cafd0` and the options caller's inline region
both implement fallback. Restore the older general body and select fallback at
both callers. This retains unobserved indexed semantics as an explicit legacy
model instead of claiming the optimized omission proves an unused parameter.
Do not add range checks or change failure values absent evidence.

Direct xrefs find only the create-slot call; options is inlined. This is a
concrete example of why a direct-call census is incomplete. The frozen base
fetch for `get_action_dik` is only `0xe` bytes versus retail `0x1f`, with different
statement coverage; it is not evidence for this newly edited source. Recheck
the full caller/callee pair in the eventual build, including argument folding.

The missing `GetActionAllBinding` has only a commented older sketch: primary
and secondary localized names, a possible `" , "` separator, and an unbound
translation via an unavailable older string-table API. Neither the current
UI's single-key translation nor the remap producer establishes truncation,
terminator, invalid-action or both-unbound policy for this destination buffer.
It remains an individually managed open body, not called editor/server-only.

## Checkpoint

Four literal markers removed; one combined-output body remains open. Six new
source TODO anchors are indexed. Static review checked caller count, complete
descriptor includes, unchanged access/layout and source diff integrity. No
compilation, tool rebuild, score refresh or claim of complete key-binder byte/
structure matching. The four getter/private models and legacy selector's
original boundaries still need the deferred evidence checks above.
