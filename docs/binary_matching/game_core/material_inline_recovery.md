<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Game-material and pair inlines

Scope: three manager placeholders, eleven material-pair inlines, eight
material accessors and its implicit destructor: **23 declarations**. Raw
target/frozen-base class topology is identical for all three classes. No
builds or score refresh. This batch supplies lookup models and records every
existing accessor; it does not claim a newly verified runtime bug.

## Manager lookup models, not recovered policy

The current source/reference search and retained index contain no consumer or
procedure for any of the three missing name/pair lookup helpers. Numeric
membership and numeric fallback retrieval are retained and establish the
storage, but not every contract of the missing helpers:

- `material_exist(name, id*)`: scan the material map in key order, compare
  each existing material name with `strings::equal`, optionally write the
  matching key, return true; on miss return false without touching the output.
  Case sensitivity, null-name handling, duplicate-name precedence, output
  optionality and miss-output policy are hypotheses. The cook already uses
  `strings::equal` to identify the material named "default", which is a
  relevant source prior, not proof of this helper's original implementation.
- `get_material_id(name)`: delegate to that membership helper and use the
  loaded default ID on miss. Retained numeric `get_material` (`0x72e660`)
  falls back through `m_default_material_id`, supporting this model. An
  assertion, invalid-ID sentinel or different unknown-name contract remains
  possible. The manager must already have loaded its default material; no
  arbitrary constructor initialization is added to make an unloaded lookup
  appear valid.
- `pair_exist(first, second)`: exact nested-map membership, without inserting
  keys, reversing the order, or invoking fallback retrieval. Retained
  `add_pair` (`0x72e910`) stores ordered first/second keys. `get_pair`
  (`0x72e4c0`) separately substitutes default material IDs and default rows/
  entries. Do not confuse a fallback result with exact pair existence.
  Whether the original membership helper also supported reverse/default
  substitution remains explicitly open.

No model is forced into retained numeric retrieval or cook code. The manager
header includes the existing material declaration for the name call; no new
predicate class, symbol, map or cache is fabricated.

The cook's current friendship remains an open source relationship: it calls
private insertion methods and writes the default ID directly after comparing
the material name. PDB validates access, not which friend declaration existed.
There is no recovered declared setter that justifies replacing this direct
store, and inventing one would not recover an original inline seam.

## Pair getters and resource setters

Retained leaves independently verify `decal1` (+0x14 reference, `0x86da0`),
`decal1_size` (+0x18 float, `0x86db0`), and `has_particle` (begin/end inequality,
`0x86dc0`). Bullet front-face collision really calls them. The existing three
`__declspec(noinline)` annotations are a separate question: surviving calls
do not establish original source attributes. They are left unchanged in this
no-build pass and explicitly queued for the deferred measured review, not
silently treated as verified original declarations.

`add_pair` statements 1/2 load pair +0x24/+0x28, then material IDs at +0x5c:
both existing material-pointer accessors and the ID getter agree with these
expanded operations.

The three resource setters already have real consumers in
`game_material_manager_cook::on_decals_loaded`. Retained offsets +0x178,
+0x1e3 and +0x24b obtain a resource value, assign it into pair +0x14, +0x1c or
+0x10 respectively, then release the temporary. Preserve their **by-value**
resource-pointer parameters and assignments; do not change them to references
or raw pointers to remove the observed ownership operations.

`sound`, `decal2` and `decal2_size` have existing simple field bodies, supported
by the setter/configuration layout, but no original getter consumer identified
in this pass. Keep these as existing field models, not verified expansions.
Do not repurpose the primary-decal consumer to force them into emitted code.

## Material accessors and generated destruction

The ID expansion above reads +0x5c. Bullet `try_reflect` reads ricochet +0x58
and reflection slow-down +0x50; front-face collision reads resistance +0x4c.
Trap placement reads mine-place +0x5e and mine-stick +0x5f as bools, preserving
their distinct tests. Those existing accessors match actual consumer fields.
The name getter supplies the cook's existing default-material comparison.

Width at +0x54 has an established load/layout field but no original getter
consumer found here. Keep the field model; do not invent a penetration-width
calculation simply because the field has an evocative name.

The raw PDB also declares `~game_material` inline. Leave source destruction
implicit: the class owns a fixed string and empty noncopyable base, not a
resource pointer requiring manually added release code. Retained
`delete_materials` already passes the map's material pointer through the
ordinary destructor/delete helper. A deleting-helper ICF alias may display
another class name; it is not evidence to invent that class's destructor body
here. Exact original explicit-empty versus generated source is not separately
recoverable from this evidence.

## Every declaration

| # | Declaration | Disposition |
|---:|---|---|
| 1 | `game_material_manager::get_material_id` | New default-ID fallback model; original miss policy/consumer open. |
| 2 | `material_exist(pcstr,u16*)` | New name-scan/output model; individual name/output contracts open. |
| 3 | `pair_exist(u16,u16)` | New exact ordered membership model; symmetry/default policy open. |
| 4 | `material_pair::first_material` | Existing +0x24 pointer getter verified in add_pair. |
| 5 | `second_material` | Existing +0x28 pointer getter verified in add_pair. |
| 6 | `sound` | Existing +0x10 reference field model; original getter consumer open. |
| 7 | `decal1` | Retained +0x14 reference leaf and bullet calls verified; noinline annotation separately open. |
| 8 | `decal1_size` | Retained +0x18 float leaf and bullet calls verified; noinline annotation separately open. |
| 9 | `decal2` | Existing +0x1c reference field model; original getter consumer open. |
| 10 | `decal2_size` | Existing +0x20 float field model; original getter consumer open. |
| 11 | `has_particle` | Retained vector nonempty test and bullet calls verified; noinline annotation separately open. |
| 12 | `set_decal1` | By-value resource assignment/release verified in cook. |
| 13 | `set_decal2` | Independent +0x1c resource assignment/release verified in cook. |
| 14 | `set_sound` | Independent +0x10 resource assignment/release verified in cook. |
| 15 | `game_material::id` | Existing +0x5c getter and ordered-pair insertion expansion verified. |
| 16 | `resistance` | Existing +0x4c getter agrees with bullet collision read. |
| 17 | `reflection_speed_down` | Existing +0x50 getter agrees with reflection reads. |
| 18 | `ricochet_koef` | Existing +0x58 getter agrees with reflection read. |
| 19 | `width` | Existing +0x54 field model; original consumer open. |
| 20 | `name` | Existing fixed-string getter and cook's name comparison reviewed. |
| 21 | `can_place_mine` | Existing +0x5e bool read verified in trap placement. |
| 22 | `can_stick_mine` | Independent +0x5f bool read verified in trap placement. |
| 23 | Implicit `~game_material` | Preserve generated member/base destruction; no new release policy/body. |

Deferred verification covers the new lookup models if original consumers are
recovered, ordinary header buildability, and the separately unresolved
noinline/friend relationships. Do not report models as byte-matched functions.
