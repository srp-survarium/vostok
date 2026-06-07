# body_part_parameters - matching log

Unit: `sources/vostok/game_core/sources/body_part_parameters.cpp` (+ header).
Already enabled (anchored in `temp_include_all.cpp` as `bpp`) and heavily matched
on arrival: 20/32 functions at 100%. This pass surveyed the unit, drove the one
definitively-winnable function to 100%, and classified the remaining residuals.

## Survey

```
git checkout -B match/game_core-body_part_parameters origin/int/game_core
nix develop -c python3 scripts/rebuild.py
# report.json units[].functions[].fuzzy_match_percent for this unit:
```

Sub-100 on arrival:
- ctor                              99.724  (stack-slot residual)
- get_hit_parameters               99.850  (stack-slot residual)
- hit_by_type                      99.833  (stack-slot residual)
- fill_new_stats_item              91.785  (BLOCKED: fixed_string<46> inline)
- regenerate                       72.608  (BLOCKED: min(u32,u32) inline, see below)
- dump_state(boost::function)      55.035  (BLOCKED: boost::function4::operator() inline)
- dump_state(npc_statistics)       17.042  (INPROGRESS: needs npc_statistics member)
- protect_affect_predicate::operator()  0.000  (FIXED -> 100%, see below)
- serialize / deserialize / serialize_affect / deserialize_affect  0.000 (network_core wall)

## WON: protect_affect_predicate::operator()  0% -> 100%

The base and target asm streams were **byte-identical** (verified with
`pdb_fetch --view target` / `--view base`). objdiff scored it 0% because it could
not *pair* the two symbols: the mangled names differed.

- target: `??Rprotect_affect_predicate@survarium@@QAEXQAUdamage_protector@1@@Z`  (`QAU` = `* const`)
- base  : `...PAU...`  (`PAU` = `*`)

MSVC encodes the top-level `const` on a pointer parameter in this position of the
mangled name. The source had `operator()( damage_protector* protector )`; target
is `damage_protector* const`. Added `const`:

```cpp
inline void operator()( damage_protector* const protector ) { ... }
```

Rebuild -> report.json 100.000, report-changes.json: improved 0.0 -> 100.0, no
regressions. (Sibling `protect_damage_predicate::operator()` already had `* const`
and was already 100%.)

Commands:
```
pdb_rich_query --index binaries/rich/target/index.jsonl --function "protect_affect_predicate::operator" --list
pdb_fetch --target-index binaries/rich/target/index.jsonl --function "protect_affect_predicate::operator" --view target
pdb_fetch --base-index   binaries/rich/base/index.jsonl   --function "protect_affect_predicate::operator" --view base
```

## BLOCKED: regenerate 72.61% - /Ob2 inlines math::min(u32,u32)

`pdb_fetch --rva 0x587860 --view target` vs base @0x46a390. Bodies match
statement-for-statement EXCEPT line 241 `regenerate_delta = math::min(regen_allowed, time_delta_ms)`:

- target (structure L177, +0xe bytes): `mov edx,[ebp+8]; mov ecx,[ebp-0Ch]; call vostok::math::min` - OUT OF LINE.
  callees confirm an out-of-line `unsigned int vostok::math::min(unsigned int,unsigned int)` (@0x03fbb0).
- base (+0x26 bytes): inlines `min` -> `min_integral` = `sbb ecx,ecx; neg; neg; and; add` directly. No call.

Root cause: the build rsp (`binaries/ninja/rsp/game_core_cl_0.rsp`) carries **/Ob2**
(full inline expansion). `math::min(u32,u32)` is `inline` in `math_functions_inline.h`,
so /Ob2 inlines it at our call site. The target build kept it out-of-line - a
whole-program inline-heuristic / header-visibility decision on that overload, not
steerable from `regenerate`'s own source. Same family as fill_new_stats_item's
fixed_string<46> and dump_state's boost::function4 inline. The +0x8 frame delta
(0x28 vs 0x20) and all `[ebp-20h]`-vs-`[ebp-18h]` slot renames cascade from that one inline.
Marked STATE[72.61%|BLOCKED], carcass + asm rationale kept inline.

## BLOCKED/deferred: the rest

- **dump_state(boost::function)** 55%: bodies match up to `.3`; then target CALLs
  `boost::function4<...>::operator()` out-of-line while /Ob2 inlines the whole
  `callback(...)` (bad_function_call check + vtable dispatch). Same inline wall.
- **fill_new_stats_item** 91.78%: pre-existing BLOCKED on fixed_string<46>(char const*)
  inline (whole-program COMDAT). Not touched.
- **dump_state(npc_statistics)** 17%: INPROGRESS - needs `npc_statistics`'s body-state
  member for the trailing `stats.<member>.push_back(new_stats_item)`. Cross-cutting.
- **serialize / deserialize / serialize_affect / deserialize_affect** 0%: empty stubs.
  Real bodies are small (packet.append / packet_reader::r<T>) but depend on
  `network_core::packet<udp_match_packet>::append` and `packet_reader::r<T>` template
  instantiations - the header only forward-declares the network types. This is the
  cross-cutting network_core wall the task said to defer.

## Result

protect_affect_predicate::operator() 0% -> 100%. Net unit 20/32 -> 21/32 at 100%,
no regressions. Everything else is a proven inline-heuristic / cross-cutting wall.
