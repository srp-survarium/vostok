# Link layout: how retail placed code and data, and what it does to matching

This note records how the retail `survarium.exe` laid out `.text`, `.rdata`,
`.data` and `.bss`, where our link differs, which build levers move the
layout, and which layout differences change function bytes. The evidence is
the DBI section-contribution substream and module list of both PDBs, the
`binaries/pdb/{target,base}/evidence.sqlite` function tables, and `/VERBOSE`
relinks of our own objects.

    python3 -m vostok tool link-layout runs --section 1     # .text runs, retail beside ours
    python3 -m vostok tool link-layout owners               # COMDATs kept from a different compiland
    python3 -m vostok tool link-layout owners --pass1 LOG   # ... classified with a /VERBOSE link log

`llvm-pdbutil` cannot open either PDB ("too many directory blocks"), so the
tool carries its own reader for the MSF directory, DBI modules, section
contributions and section headers. Measured on `layout/retail-order`
(PR #595's tree) against the v0.100b retail PDB: 2,396 retail / 2,383 base
modules, 65,703 / 65,532 contributions.

## Short answer

- Retail code and data follow **two independent orders**. Placement (where
  bytes go) follows the linker's pass-1 load order: library order, then symbol
  demand. The DBI module list (the "CIL" order the LTCG back end hands out)
  is a third order, unrelated to placement on either side.
- Our placement order is already close to retail: 2.9% of module pairs are
  inverted for LTCG COMDAT code, 2.5% for LTCG non-COMDAT code, 1.9–2.8% for
  `.rdata`/`.data`, 4.3% for `.bss`. Engine string literals, `__real@`
  constants, vtables and RTTI sit in the retail owner module 94–96% of the time.
- **Link-line levers do not move function bytes.** Relinking the same objects
  with a different invocation reshuffles the DBI order by ~10% of pairs and
  changes no function's normalized code across the exactness line. Spelling the
  archives as retail did (`c:\survarium\binaries\win32\libraries\...`) changes
  nothing at all.
- **What does move bytes is COMDAT copy selection.** Every TU that references
  an inline or template function carries an IL copy; the linker keeps the copy
  of the first module it *loads* in pass 1, and that module's `/Od` or `/Ox`
  flags decide the out-of-line body. 649 COMDAT functions are kept from a
  different compiland than in retail; 106 of those rows are not exact. 27 are
  `missing-reference` rows: the retail owner loads before ours in our own pass 1,
  so the retail owner's TU references the function and ours does not. That is
  a source correction, not a layout lever.

## Retail `.text`

Contribution characteristics split `.text` cleanly. Under VS2008 none of the
projects use `/Gy`, so each LTCG compiland emits **one** non-COMDAT `.text`
blob (`0x60502020`) holding all its ordinary functions, plus one COMDAT
contribution (`0x60503020`) per inline/template function, dynamic
initializer and atexit destructor.

| retail RVA | content | order |
|---|---|---|
| `0x1000`–`0x12a827` | LTCG COMDAT functions (10,040 contributions, 1.22 MB) | direct objects, then engine libraries in link order, with later "sweeps" of the same library (pass-1 load order) |
| `0x12a827`–`0x544fd0` | non-LTCG archives: sound (`/Od`, no `/GL`), vorbisfile, stlport, CRT, SpeedTree, OpenSSL, GFx, png/jpeg, pcre, expat, zlib, import thunks | pass-1 load order of those archives |
| `0x544fd0`–`0x7b0bc4` | one non-COMDAT blob per LTCG compiland (1,089 blobs, 2.52 MB) | waves that walk the libraries in reverse link order; inside a lib, blobs descend in DBI index with a stride of ~11–14 |
| `0x7b0bc4`–`0x7c3ff0` | `WinMain`, late CRT/OpenSSL/stlport/SpeedTree code | load order |
| `0x7c3ff0`–`0x7dd380` | ``dynamic initializer`` COMDATs | link order, engine first, then vendors |
| `0x7dd380`–end | ``dynamic atexit destructor`` COMDATs | link order |

The LTCG code generator appends its non-COMDAT output after every non-LTCG
object, while LTCG COMDATs keep the slot of the module that defined them
in pass 1. Inside a module, placement follows definition order (the
definition-order probe in `pdb_order_causal_attribution.md`). Our build has the same
regions: COMDAT order 2.9% inverted, non-COMDAT blob order 2.5% inverted
(8.3% on v0.10b before #595; moving data definitions to their PDB owners
also moved the LTCG emission order).

The visible library-level divergence is in the first region: retail extracts
`network` in two sweeps (159 COMDATs in the first slot, 728 after `particle`)
where we extract 700 then 342, and the vendor tail differs because we link
`libgfx_libpng`/`libgfx_libjpeg`/`zlibN` (already recorded as an
archive-identity difference in `build-flags.md`). Both come from symbol demand
and archive identity, not from link-line order.

## Retail `.rdata`, `.data`, `.bss`

- `.rdata` starts with the IAT (`0xc0303040`), then per-module read-only data.
  String literals (`??_C@`), `__real@` constants, vtables and RTTI are COMDATs
  placed in pass-1 load order, i.e. beside the module that first defined them;
  `.rdata` module order tracks the COMDAT code order (0–19% inverted against it
  per library, against 40–50% for DBI order). Shared unique identities between
  retail and ours:

  | symbol family | engine pairs inverted | same owner module |
  |---|---:|---:|
  | `__real@` | 3.5% | 95.6% |
  | `??_C@` | 2.8% | 93.8% |
  | `??_7` | 3.9% | 95.2% |
  | `??_R4` / `??_R0` | 4.3% / 4.7% | 94.9% / 95.5% |

  Vendor literal order is 32% inverted, entirely the GFx/png/jpeg archive split.
- `.data` puts non-LTCG data first (sound, stlport, CRT, ... GFx, zlib), then
  the LTCG engine data in link order from `0xbfcc8`; `.bss` follows the same
  shape. The only extra section is `STLPORT_` (stlport's own `init_seg`).
  Neither PDB shows any other `data_seg`/`const_seg`/`init_seg` section, so
  no project used one.

## The DBI (CIL) module order

The DBI module list interleaves every library (`core`, `core`,
`render_engine`, `render_engine`, `game`, `game_core`, `ui`, `ai`, ...). It is
unrelated to placement on both sides (45–50% of pairs inverted against the
COMDAT order inside most libraries) and to our pass-1 load order (45.8%).
Measured against retail it is 16.8% inverted overall, 53% inside `game_core`,
6% inside `render_engine`.

It is not an input-order lever:

| relink of the same objects | DBI vs ninja link | code vs ninja link |
|---|---:|---|
| manual relink, identical response file (twice) | 9.6% inverted; the two manual relinks are identical | 12 of 35,591 functions differ, none crossing exactness |
| exact replica of ninja's command line (`/ERRORREPORT:PROMPT`, same `/OUT`) | identical to the manual relink | as above |
| + 4 KB environment variable | identical to the manual relink | — |
| + `/VERBOSE` | 12.1% against the manual relink | — |
| engine archives spelled `c:\survarium\binaries\win32\libraries\X.lib` (retail spelling) | identical to the manual relink | — |

So the order depends on linker state outside the response file (the shared
`mspdbsrv` server is the remaining difference between a ninja link and a manual
one) and on diagnostics such as `/VERBOSE`, but the code generator's decisions
do not depend on it. Earlier probes already showed archive member order,
unresolved-root demand, compile chronology and librarian `/LTCG` indexes to be
neutral for this order under `/GL`
([`pdb_order_causal_attribution.md`](../todos/pdb_order_causal_attribution.md)).
Matching DBI order is therefore a PDB-cosmetic goal with no code yield.

## COMDAT copy selection is the layout effect that changes code

`link-layout owners` joins each function's RVA with the contribution that holds
it. On the PR #595 tree, 649 COMDAT functions are kept from a different
compiland than retail's:

| verdict (with our `/VERBOSE` pass-1 log) | exact | non-exact | of which owners differ in `/Od`↔`/Ox` class |
|---|---:|---:|---:|
| `missing-reference`: retail's owner loads first in our pass 1 | 43 | 27 | 20 |
| `load-order`: our owner loads first | 439 | 58 | 5 |
| `roster`: one owner is absent from a PDB | 49 | 20 | 19 |
| `owner-differs` (owner not in the log) | 12 | 1 | 0 |

The rule "first pass-1-loaded module whose IL holds the function wins" explains
577 of the 649 rows directly (our owner loads first on our side) and every
`missing-reference` row inversely: those retail owners load *before* ours, so
their TUs must carry the function in retail and do not in our source. It also
survives the DBI test above: the manual relink swapped the DBI order of 327
owner pairs (for example `game\weapon.obj` 731→411, `game_core\weapon_core.obj`
265→1119) and no owner moved.

When the two candidates differ in optimisation class the kept body differs:
retail `weapon_core::is_aimed` is the `/Ox` copy from `game\weapon.obj`
(`mov al,[eax+488h] / ret`, custom `this`-in-`eax` convention); ours is the
`/Od` copy from `game_core\weapon_core.obj` (framed, 20 bytes). The same holds
for `skeleton::get_bone` (retail `animation\skeleton.obj`, ours
`game_core\hand_to_weapon_ik_processor.obj`), the `behaviour_cook_params`
variant helpers (`game\game_world_npc.obj` vs `ai\brain_unit_cook.obj`), the
`string::find`/`search` pair (`render_engine_world_pc_dx11.obj` vs
`network_core\http_client.obj`) and `intrusive_list::push_back` (`game\object_sound.obj`
vs `sound\single_sound.obj`). Several of these rows were parked with notes
blaming the function's own source shape; the cause is a missing reference in
the retail owner's TU. The new pattern
[`comdat-copy-from-first-loaded-referencing-module.md`](patterns/comdat-copy-from-first-loaded-referencing-module.md)
records the rule.

`load-order` rows are the converse: our owner TU references the function where
retail's equivalent did not (or demand pulls it in earlier). `roster` rows are
mostly `Render_BufferGeneric` compiled inside our `vostok_scaleform` archive
where retail took it from `libgfx.lib`.

ICF follows the same first-loaded rule for its representative name. Of 1,814
retail fold groups, 1,505 have the same members and the same representative in
our build, 106 the same members and a different representative, and 203 differ
in membership (code differences). The ledger already pairs representatives
through fold aliases, so representative differences cost no score.

## Levers

| lever | retail had it? | effect |
|---|---|---|
| library order on the link line | yes; already reproduced (`RETAIL_LINK_LIBRARY_ORDER`) | moves placement and COMDAT owners; already retail |
| direct-object order (exe project files) | yes | already retail order (`finger_print`, `math_error_win`, `pch`, ...) |
| archive member order | yes | neutral under `/GL` (probe); `sound` is non-`/GL` and was normalised earlier |
| archive path spelling (`c:\survarium\binaries\win32\libraries`) | yes (lowercased by VS2008 "Link Library Dependencies") | no effect on DBI order, placement or code |
| linker diagnostics / invocation state | n/a | reshuffles DBI order, code-neutral |
| definition order within a TU | yes | moves placement inside a module (probe); no code effect seen |
| `/ORDER` | no: retail COMDATs sit in load order | not applicable |
| `#pragma data_seg`/`const_seg`/`init_seg` | only stlport's `STLPORT_` | not applicable |
| `static` vs `extern`, `selectany`, where a constant is defined | yes | decides COMDAT vs per-TU data and the owner of `__real@`/literals; PR #595 used this (94–96% owner agreement now) |
| **which TUs reference an inline function** | yes | **decides the kept COMDAT copy and its `/Od` or `/Ox` body** |

## Applied: `weapon_core` accessors

The clearest `missing-reference` group: six one-line accessors
(`ammo_in_magazine`, `round_is_chambered`, `fire_queue_length`, `ammo_slot`,
`is_aimed`, `is_double_handed`). Retail files them under `weapon_core.h`
(lines 132–232) and keeps the `/Ox` copy from `game\weapon.obj`, which our own
pass 1 also loads before `game_core\weapon_core.obj` (`/VERBOSE` lines 2470
vs 2644). The reconstruction had defined them out of line in `weapon_core.cpp`
and let `weapon` read the members through a friend declaration, so no IL copy
existed in `weapon.obj` and every game_core caller called a framed `/Od`
thiscall. Defining them inline in the header and calling them from
`weapon.cpp` restores the retail selection.

Full measured build against the parent's committed ledger (#595 head): exact
functions 10,680 → 10,720 and exact-max 11,287 → 11,316 of 13,014; 36 MAX
rises (34 `game_core`, 2 `game`), **no MAX falls**. The six accessors go from
unpaired to exact; 20 `game_core` callers that call them out of line
(`get_weapon_lexeme_pair`, `initialize`, `update_recoil`,
`vertical_recoil_value`, ...) reach 100 now that the call takes `this` in
`eax`; `weapon::set_next_ammo_type` (74.7 → 100) and `weapon::on_show`
(95.5 → 100) follow. Two `cur` dips with MAX held:
`weapon_core_shotgun_reload_state::finish_reload_predicate` 88.27 → 64.72 (the
`/Od` inliner now calls `intrusive_ptr::set` and inlines `get_target`, the
reverse of retail) and `dictionary_item::operator=` (fold churn, unpaired this
link).

## Still unexplained

- What fixes the DBI module order. It is deterministic for a fixed linker
  state but responds to `/VERBOSE` and to the PDB-server state, which points at
  pointer-keyed iteration inside `link.exe`/`c2.dll`; retail's order cannot be
  reproduced under Wine's heap and does not need to be for code.
- The stride pattern of the non-COMDAT blobs (reverse library waves, DBI index
  stepping by ~11–14) is reproduced by our link but not modelled.
- The 58 non-exact `load-order` rows need per-row reading: each one is either an
  extra reference in our owner's TU or a symbol-demand difference earlier in
  pass 1.
- The other non-exact `missing-reference` groups are open source work, each
  needing the retail TU's actual use of the function:

  | retail owner TU ← our owner TU | rows | example |
  |---|---:|---|
  | `vfs\virtual_file_system` ← `vfs\mount_physical_path`, `mount_archive` | 6 | `base_node<1>::sizeof_with_name`, `set_name` |
  | `game\game_world_npc` ← `ai\brain_unit_cook` | 4 | `variant<32>::set<behaviour_cook_params>` and its helpers |
  | `render_engine_world_pc_dx11` ← `network_core\http_client` | 2 | stlport `string::find`, `search` |
  | `scaleform\d3d1x_hal`, `movie` ← `libgfx` | 4 | `MatrixState`, `Matrix4x4<float>`, `Rect<float>` constructors |
  | `animation\skeleton` ← `game_core\hand_to_weapon_ik_processor` | 1 | `skeleton::get_bone` |
  | `game\object_sound` ← `sound\single_sound` | 1 | `intrusive_list<new_sound_propagator>::push_back` |
  | `vfs\saving_node`, `network\login_client`, `game\game` | 3 | `strings::ends_with`, `string_order` ctor, `intrusive_ptr::operator=` |
