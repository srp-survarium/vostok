<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Camera and player-input inline review

Scope: all five inline declarations in `player_input_handler` and all nine in
`game_camera`. Eleven still carried no-source markers; three setters were
already unmarked. Raw PDB topology reports identical target/base semantic
variants for both classes, including size, declaration order and access.

## Real priority mismatch

Retail `player_input_handler::input_priority`, RVA `0x868f0`, is:

```asm
mov eax, 0Ah
ret
```

The pre-fix source returned zero. A **base-only** fetch confirms the actual
candidate at RVA `0xbc470` is `xor eax,eax; ret`. The source now returns 10.
This is observable through `input_world::add_handler`, whose lower-bound
comparator sorts handlers by their virtual `input_priority()` value.

The two-sided fetch/diff instead chose `vostok::assert_log_times_in_gold` as
the base body and reported 2/2 equal instructions, hiding the real mismatch.
The [alias-selection repro](../pdb_fetch_alias_selection.md)
records both paths. This is a tool defect, not an LTCG justification for keeping
the wrong source constant. The source fix is established; the parser fix remains
open and is not implemented in this source-only batch.

## Every inline declaration

| Function | Disposition and evidence |
|---|---|
| `player_input_handler::~player_input_handler` | Existing empty explicit body retained. Retail scalar-deleting destructor resets both vfptrs, destroys the fixed-vector member (end := begin), and conditionally deletes storage; no extra handwritten teardown is missing. |
| `player_input_handler::set_key_binder_context` | Existing context-field setter retained. |
| `player_input_handler::get_input` | Existing const input reference; consumed by `player::local_input`. |
| `player_input_handler::input_mode` | Existing enum getter; consumed by `player::update_camera`. |
| `player_input_handler::input_priority` | Corrected 0 to 10 from direct retail assembly and independently fetched actual base body. |
| `game_camera::get_inverted_view_matrix` | Existing matrix reference retained. |
| `game_camera::set_fov_factor` | Existing FOV-field setter; current/target FOV updates in `player::update_camera` already use this seam. |
| `game_camera::get_fov_factor` | Existing field getter retained. |
| `game_camera::set_near_plane` | Existing near-plane-field setter; target store through the input-controller subobject is at +0x4c. |
| `game_camera::get_near_plane` | Existing near-plane getter retained, not confused with far-plane field. |
| `game_camera::get_far_plane` | Existing far-plane getter retained. |
| `game_camera::on_deactivate` | Retail procedure is `ret`; empty hook is real. |
| `game_camera::on_focus` | Retail procedure is `ret 4`; empty hook consuming its bool argument is real. |
| `game_camera::get_game_scene` | Existing scene reference retained. |

All fourteen are accounted for: **one real source fix, ten stale markers
removed, three unmarked setters checked**. No missing body remains in these
two classes. No C++ build or ledger refresh was run, so this does not claim a
new measured match percentage. The alias-selection defect must be fixed or
worked around during the eventual comparison pass.
