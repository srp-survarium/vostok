# game_core::player_stealth {default ctor, load}

Unit: the small class `survarium::player_stealth` (game_core), two functions:
- `player_stealth::player_stealth()` default ctor  target rva 0x59aad0
- `void player_stealth::load(configs::binary_config_value const&)`  target rva 0x59aae0

Copy ctor + operator= already matched (#114); member layout reused from the header.
Same pattern as `character_dispersion_params` (#135): constant default ctor + config-reading
`load`. Branch off `match/game_core-dispersion_calculator-apply_aim_speed` (#139).

## Member layout (11 floats, struct size 0x2C, header already has it)
m_default_value@0 m_default_sound_value@4 m_stand_factor@8 m_crouch_factor@0xc
m_crouch_sound_factor@0x10 m_walk_factor@0x14 m_walk_sound_factor@0x18
m_sprint_factor@0x1c m_sprint_sound_factor@0x20 m_detection_level@0x24
m_always_visible_distance@0x28

## Mangled names / visibility
- `??0player_stealth@survarium@@QAE@XZ`  -> default ctor, `QAE` = public.
- `?load@player_stealth@survarium@@QAEXABVbinary_config_value@configs@vostok@@@Z` -> public.
Header already had both in `public:`. No access change needed.

## Commands run (verbatim)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function player_stealth --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x59aad0 --view target   (ctor)
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x59aad0 --view info
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x59aae0 --view target   (load)
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x59aae0 --view structure
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x59aae0 --view info

## ctor asm (0x59aad0)
```
push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx
mov eax,[ebp-4]    ; load this
mov esp,ebp; pop ebp; ret
```
NO member stores at all - the default ctor body is EMPTY `{ }`. (Unlike
character_dispersion_params whose ctor stored 14x 1.0f; player_stealth's default ctor
leaves members uninitialized.) So NO DSE / anchoring concern for the ctor (nothing to
store). The empty body was already 100% in report.json from #114's anchoring.

## load asm (0x59aae0)
Frame `sub esp, 4Ch` (temporaries for the by-value operator[]/operator float calls).
11 identical blocks, NO value_exists guard (each member unconditionally assigned):
```
push "<member_name>"; mov ecx,[ebp+8]; call operator[]; call operator float;
mov reg,[ebp-4Ch]; movss [reg+off], xmm0
```
Strings in order: default_value, default_sound_value, stand_factor, crouch_factor,
crouch_sound_factor, walk_factor, walk_sound_factor, sprint_factor, sprint_sound_factor,
detection_level, always_visible_distance. Offsets 0x00..0x28 = the 11 members in decl order.
=> `m_x = (float)config["x"];` per member, member order. NO braces (structure: 13
statements L44 `{`, L45..L55 the 11 assignments, L56 `}`, no `+0x002` brace jumps, 0 locals).

This is SIMPLER than character_dispersion_params::load (which had `if(value_exists)`
guards) - player_stealth::load is unconditional assignment for all 11 members.

## Anchor (temp_include_all.cpp)
use_game_core_player_stealth() already existed (#114): default-construct stealth,
copy-construct stealth2, operator=, escape both via example_callback. Added a
`configs::binary_config_value cfg; stealth.load( cfg );` call so the load body is
reachable/scored (it was 0% before because no caller referenced it).

## Iterations / Results
- nix develop --command bash -c 'python3 scripts/rebuild.py'   # bare, full EXE relink
- report.json fuzzy_match_percent, BOTH 100% on the FIRST rebuild:
    ??0player_stealth@survarium@@QAE@XZ                                  = 100.0  (default ctor)
    ?load@player_stealth@survarium@@QAEXABVbinary_config_value@configs@vostok@@@Z = 100.0  (load)
  (copy ctor ??0...@ABV01@@Z and operator= ??4... stayed 100% from #114.)
- pdb_fetch --base-index binaries/rich/base/index.jsonl --target-index ... --rva 0x59aad0 --view diff
    ctor: "8/8 instructions equal (100.0%)", no -/+ lines.
- pdb_fetch ... --rva 0x59aae0 --view diff   load: no -/+ divergence lines (clean byte match).
Both matched first try; no source iteration needed. Carcass deleted (clean 100% DONE).

## report-changes triage (stale-baseline / relink-churn, NOT my regressions)
49 regressed / 58+ improved. My git diff touches ONLY player_stealth.cpp + the
temp_include_all anchor (added `stealth.load(cfg)`). All regressions are the standard
full-relink ICF/COMDAT churn that flips 100%<->0% every relink:
- scalar/vector deleting destructors, ~dtors, boost::_bi::storageN copy ctors,
  resource_ptr::operator=, btHashMap dtor, empty_stub, setOverlapFilterCallback,
  ui/sound/ai [thunk] vcall entries.
- 2 game_core-named hits are the same class: `collision_sensor::'scalar deleting
  destructor'` (vcall thunk, not the matched body) and `inventory_item::
  deserialize_game_world_object` (13.4%->0, BLOCKED packet-cluster stub unanchored on
  this branch). Neither is a function this unit touches.
=> no real regressions.

## Outcome
STATE[100%|DONE] for both default ctor and load. Matched byte-exact on first rebuild.
Inlining: none relevant. Regressions caused: none (relink ICF/vcall-thunk churn only).
