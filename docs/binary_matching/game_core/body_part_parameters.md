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
- fill_new_stats_item              91.785  (PARTIAL: fixed_string<46>(char const*) inline-vs-call LTCG; reviewer fixed tag BLOCKED->PARTIAL)
- regenerate                       72.608  (PARTIAL: min(u32,u32) inline-vs-call LTCG, see below; reviewer fixed tag BLOCKED->PARTIAL)
- dump_state(boost::function)      55.035  (PARTIAL: boost::function callback operator() inline)
- dump_state(npc_statistics)       17.042  (BLOCKED: needs npc_statistics::body_state member @+0x2798, buffer_vector<statistics_item<46,16>>)
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

## PARTIAL: regenerate 72.61% - inline-vs-call of math::min(u32,u32) [reviewer: was BLOCKED]

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
Reviewer correction: math::min(u32,u32) is a real standalone symbol in BOTH rich indexes
(target @0x03fbb0, base @0x023490) - so this is the documented per-call-site inline-vs-call
LTCG class (PARTIAL), NOT a missing/un-emitted symbol. Nothing blocks it; tag corrected
STATE[72.61%|BLOCKED] -> STATE[72.61%|PARTIAL]. Carcass + asm rationale kept inline.

## BLOCKED/deferred: the rest

- **dump_state(boost::function)** 55%: bodies match up to `.3`; then target CALLs
  `boost::function4<...>::operator()` out-of-line while /Ob2 inlines the whole
  `callback(...)` (bad_function_call check + vtable dispatch). Same inline wall.
- **fill_new_stats_item** 91.79%: PARTIAL - per-ctor inline-vs-call of fixed_string<46>(char const*)
  (the ctor is standalone in BASE @0x030ca0, inlined in target). Reviewer fixed tag BLOCKED->PARTIAL.
- **dump_state(npc_statistics)** 17%: BLOCKED - needs `npc_statistics::body_state` member (a
  buffer_vector<statistics_item<46,16> > @ npc_statistics+0x2798) for the trailing
  `stats.body_state.push_back(new_stats_item)`. Decoded from target rva 0x587140. Cross-unit.
- **serialize / deserialize / serialize_affect / deserialize_affect** 0%: empty stubs.
  Real bodies are small (packet.append / packet_reader::r<T>) but depend on
  `network_core::packet<udp_match_packet>::append` and `packet_reader::r<T>` template
  instantiations - the header only forward-declares the network types. This is the
  cross-cutting network_core wall the task said to defer.

## Result

protect_affect_predicate::operator() 0% -> 100%. Net unit 20/32 -> 21/32 at 100%,
no regressions. Everything else is a proven inline-heuristic / cross-cutting wall.
