<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Ammunition and empty-hands inline review

Scope: all 22 `weapon_ammunition` inlines and all nine `empty_hands` inlines.
Both raw PDB class comparisons are topology-identical; no access, layout,
declaration order or friend changes are required. All procedure comparisons
below use independently fetched target and base bodies to avoid the documented
two-sided ICF alias-selection defect.

## Two different constant-return problems

`empty_hands::is_ready_to_be_deactivated` was an **unmarked** false return.
Retail is `mov al,1; ret`; the actual candidate is `xor al,al; ret`. The source
now returns true. `base_player::tick_active_object` tests this method before
deactivating the current object and activating the pending one. Returning false
for empty hands prevents that switching path, so this is a real behavioral
mismatch rather than a harmless unused accessor.

The ammunition class is different. Its two boolean hooks have framed retail
bodies that never assign AL/EAX. Candidate `is_ready_to_be_deactivated` added
`mov al,1`, and candidate `is_sprinting` added `xor al,al`. Neither constant is
supported by retail. The adjacent `transform` method has the same lack of a
result assignment and is already represented by an unreachable-code macro.
The boolean hooks now use `UNREACHABLE_CODE()`, which is `__assume(0)` in the
shipping configuration. This is a source-form inference, not proof of the
original macro spelling or a promise that those virtuals return a valid bool.
Do not replace unreachable target behavior with invented safe fallback values.

## Every empty-hands inline

| Function | Disposition |
|---|---|
| `deactivate` | Retail bare `ret`; empty override retained. |
| `transform` | Retail copies 0x40 bytes from member +0x108 into the return buffer; existing matrix return retained. |
| `tick` | Retail bare `ret`; empty override retained. |
| `is_ready_to_be_deactivated` | Corrected false to true from independent target/base assembly. |
| `on_player_model_added` | Retail bare `ret`; empty override retained. |
| `on_player_model_removed` | Retail bare `ret`; empty override retained. |
| `is_sprinting` | Retail `xor al,al; ret`; existing false return retained. |
| `serialize` | Existing direct argument-eater call retained; retail pushes both arguments and calls the shared no-op alias. No serialized state is emitted. |
| `deserialize` | Open boundary question: target rich record at RVA 0x112e20 spans one `int3` byte. Direct-call xref scan finds no callers, but that does not resolve indirect vtable use or establish a callable empty body. Keep source unchanged pending vtable/extent evidence; remove the earlier overconfident wall claim. |

## Every ammunition inline

| Function | Disposition |
|---|---|
| `distance` | Existing corresponding float-field getter retained. |
| `dispersion` | Existing corresponding float-field getter retained. |
| `ricochet_angle` | Existing corresponding float-field getter retained. |
| `damage` | Existing corresponding float-field getter retained. |
| `impulse` | Existing corresponding float-field getter retained. |
| `pierce` | Existing corresponding float-field getter retained. |
| `air_resistance` | Existing corresponding float-field getter retained. |
| `muzzle_speed` | Existing corresponding float-field getter retained. |
| `buck_shot` | Existing u16 return from the recorded signed 16-bit storage; preserve declared interface/storage distinction. |
| `game_material_id` | Same signed-storage/unsigned-return distinction; no type normalization. |
| `tracer` | Existing boolean field getter retained. |
| `activate` | Existing assert-shaped dummy use plus direct reference-pointer argument eater retained. Retail stack/argument sequence supports this shape, but the exact original assert expression remains unknown. |
| `deactivate` | Existing empty override retained. |
| `transform` | Existing unreachable representation retained; target writes no output matrix. |
| `tick` | Retail frame/this spill/epilogue only; empty body retained, not a new tick implementation. |
| `is_ready_to_be_deactivated` | Remove unsupported true result; use shipping unreachable form, with original spelling still open. |
| `selected_animations` | Existing argument-use/unreachable form retained. Retail consumes arguments but does not produce an expression result. |
| `on_player_model_added` | Retail frame/this spill/epilogue only; empty body retained. |
| `on_player_model_removed` | Same framed empty override. |
| `serialize` | Existing `inventory_item::serialize` delegation retained; no duplicate ammunition-specific serialization invented. |
| `deserialize` | Existing `inventory_item::deserialize` delegation retained. |
| `is_sprinting` | Remove unsupported false result; use shipping unreachable form, with original spelling still open. |

Seven literal no-source markers and two nonstandard `sushi@TODO no source`
comments are resolved into explicit dispositions. The audit also caught the
unmarked empty-hands behavior bug. All 31 inlines are accounted for, but the
three source-form/boundary questions above are not claimed closed. No C++ build
or ledger refresh was run; validate the changed bodies in the deferred batch.
