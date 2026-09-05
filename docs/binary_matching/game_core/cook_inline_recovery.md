<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Cook helper ownership and reconstruction models

PR 571 source-only batch; no build or score refresh. Four literal placeholder
functions are reviewed individually, together with seven relevant consumers,
donors and cleanup methods. Two bodies are explicit models, not newly proven
retail expansions. Two remain open rather than acquiring invented pipelines.

## Individual register: 11 declarations/consumer anchors

| # | Function | Disposition |
|---:|---|---|
| 1 | `weapon_core_cook::register_cooks_for_logic_states` | OPEN registration-list hypothesis. No current source call or retained procedure found. The live game owner has a different static method and game-specific wrappers; its list cannot simply be copied into core. Original core-only state set, static lifetimes and overlap policy remain unknown. |
| 2 | `weapon_core_cook::on_skeleton_config_loaded` | OPEN callback hypothesis. Likely consumes some skeletal configuration and continues weapon loading, but no query binding establishes its input resource or continuation. The live callback below consumes a skeleton resource, not a skeleton config. Do not alias these callbacks by signature. |
| 3 | `weapon_core_cook::process_loading_weapon_core` | Retained consumer at RVA `0x590460`: load parameters, form skeleton path, request skeleton and user-animation resources, bind `on_core_subresources_ready`. The callback address is explicit in statement 6 at +0xb2. No extra skeleton-config stage is inserted. |
| 4 | `weapon_core_cook::on_core_subresources_ready` | Retained consumer at `0x590330`: extract/assert parent and success, acquire both resources, install them through setters, then call `query_weapon_states`. Repair the PDB local `query_result_for_cook* const parent`; no extra missing-helper call is justified between these operations. |
| 5 | `weapon_core_cook` implicit destructor | Keep compiler-generated. Exact-name target fetch gives only the `cook_base` vptr store and return; no owned resource member cleanup or registration removal is added. |
| 6 | `weapon_cook::register_cooks_for_logic_states` | Retained game owner at `0x5bd320`, 27 statements. Its static list mixes core templates with game sound-event wrappers and the distinct game shotgun cook. Existing `game_world` call remains here. No base/core registration call is fabricated. |
| 7 | `damage_model_cook::get_current_hit_type` | Restore model `m_hit_types_strings[g_current_hit_type].c_str()`. The console producer establishes sequential IDs and stable string storage, but no getter consumer establishes original lookup/guards. Define in the owning cpp to use its file-static selection without changing visibility/linkage. Original placement is open. |
| 8 | `damage_model_cook::on_hit_params_received` | Producer at `0x7511f0`: populate string/token vectors once; each token ID is loop index and its name points at the corresponding stored string; bind `g_current_hit_type` by reference to the `hit_type` console command. Keep initialization, token ownership and existing resource-cooking behavior. |
| 9 | `weapon_core_shotgun_reload_state_cook` constructor | Replace the old reuse-true/empty placeholder with the sibling-state-cook model: class ID, `reuse_false`, current creation/allocation threads, self-registration. Original core constructor expansion/registration policy remains open. No new instance or registration call site is introduced. |
| 10 | `weapon_core_shotgun_reload_state_cook` destructor | Existing empty body retained. Target `0x580150` reinstalls its vptr then performs unmanaged-cook base cleanup. The printed inplace-cook callee is an ICF alias, not a reason to change inheritance. |
| 11 | `shotgun_weapon_reload_state_cook` constructor | Game donor at `0x5ad6f0`: raw stores establish class ID 0x136, non-reuse, two current-thread values and registration. It is a separate unmanaged-cook subclass, **not derived from the core shotgun cook**; this proves only the donor, not row 9. |

## Getter access and lifetime

The `hit_type` command receives the cpp's `g_current_hit_type` by reference
(`on_hit_params_received`, statement 19, +0x20b). Its token vector is populated
with `{i, m_hit_types_strings.back().c_str()}` in the preceding loop. This makes
the indexed string a natural selection model after successful initialization.
The strings live in a static fixed-capacity vector rather than in a temporary
console-status buffer.

There is no public selected-ID/name accessor on `cc_token`: `find_name` and
its value reference are private. Do not invent a friend or expose fields just
to keep this formerly implicit-inline placeholder in the class definition.
The static member declaration stays public; its model definition belongs in
the cpp that can access the existing file-static ID. No external-linkage ID,
header copy of the ID, static initializer or new caller is introduced.

Selection before token initialization, an empty configured list, invalid IDs,
and string-vector lookup versus token-name lookup are not settled by a retained
getter expansion. The model assumes a populated list and a valid selected ID;
it does not invent a null/empty-string fallback. No standalone rich getter
record and no current source consumer were found. This is not a claim that
the original helper was unused or that its original source was out-of-line.

## Constructor evidence limits

Both the existing core state-cook template and the retained game shotgun cook
use non-reuse, current-thread creation/allocation and self-registration. They
provide a much stronger source prior than the core placeholder's unsupported
reuse-true value. The core cook creates mutable per-weapon reload states, but
that alone is not proof of its original constructor flags.

The fourth unmanaged-cook argument already defaults to current-thread in this
tree; writing it explicitly is not a newly discovered thread-behavior fix.
Changing reuse and adding registration **are** model behavior changes. The
core constructor has no retained rich procedure, and the live game list
instantiates the different game cook. Do not register both for the same class
ID to make this model reachable. Its original core registration owner remains
the question in row 1, not a reason to alter the game list.

## Structure and deferred checks

Frozen target/base topology reports one identical semantic variant for each
of `weapon_core_cook`, `damage_model_cook`, and
`weapon_core_shotgun_reload_state_cook`. Existing public/protected/private
method boundaries and fields are preserved; no friendship or virtual member
is introduced. This frozen comparison is not a measurement of the new source.

Review the getter placement/consumer and core constructor against any new
consumer evidence. At the later authorized build, check the const local,
constructor emission, generated cleanup and all changed-header dependents.
No byte-match or successful compilation is claimed for this source-only batch.
