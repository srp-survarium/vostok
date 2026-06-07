# body_part_parameters::dump_state (both overloads) + predicate const-param pairing

Structure-verifier (v2) pass on body_part_parameters.cpp. Two `dump_state` overloads plus
a quick source-steerable fix on `protect_affect_predicate::operator()`.

## Commands (verbatim)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function dump_state --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --base-index binaries/rich/base/index.jsonl --function dump_state --rva 0x5872f0 --view structure-diff --condensed ...
- pdb_fetch ... --function dump_state --rva 0x5872f0 --view diff ...
- pdb_fetch ... --function dump_state --rva 0x587140 --view structure-diff --condensed ...
- pdb_fetch ... --function dump_state --rva 0x587140 --view diff ...
- pdb_fetch ... --function dump_state --rva 0x587140 --view callees ...
- pdb_fetch ... --function "protect_affect_predicate::operator()" --rva 0x0b9f80 --view target/structure ...
- nix develop -c python3 scripts/rebuild.py   # NO module arg

## protect_affect_predicate::operator()  (0.00% -> 100.00%)  [FIXED]
report.json had this at `?` (unpaired); structure-diff said "not found in BASE index".
Cause: the source declared `operator()( damage_protector* protector )` but the TARGET mangles
the parameter as `damage_protector* const` (sibling `protect_damage_predicate::operator()` already
uses `* const` and matched). objdiff pairs base<->target by mangled symbol, and a top-level
`const` on a pointer-BY-VALUE parameter IS part of the mangled name, so the differing const made
them unpairable -> reported 0/unpaired even though the body was identical. Added `const`:
`operator()( damage_protector* const protector )`. Rebuild: improved 0.00 -> 100.00. DONE.
(Note: top-level const on a by-value param is normally dropped for OVERLOAD resolution, but MSVC
still encodes it in the decorated name here, so it gates symbol pairing - see assembly_patterns.md.)

## dump_state(boost::function<...>, u32)  target 0x5872f0  -> 55.04% BLOCKED
structure-diff: target 5 / base 5 stmts, all `.. same ..` except the final `callback(...)` line
(SIZE: target 0x38 / base 0x9c). --view diff localizes it to the boost::function invocation:
- TARGET precomputes m_health (0xa2) and m_max_health (0xb8) into stack slots BEFORE calling
  affects_str.c_str(), then emits the out-of-line boost::function `empty()` guard
  (call; movzx; test; je) and finally the indirect functor dispatch through the vtable
  (mov edx,[ebp+8]; and edx,-2; mov eax,[edx+4]; call eax; add esp,14h).
- BASE inlines a shorter invoker: it computes m_health/m_max_health inline at the call
  (fld dword ptr [edx+8Ch]) and skips the separate empty() guard path.
This is arg-evaluation-order + boost::function::operator() inline-vs-call at the call boundary -
a whole-program COMDAT/inline decision on boost::function, not steerable from this function's
source. 5/5 statement structure matches. BLOCKED (non-steerable wall).

## dump_state(npc_statistics&, u32)  target 0x587140  -> 17.04% BLOCKED
report.json 22.25 -> 17.04 after the rebuild; the wobble is ICF/delink re-fold noise on its
callee set (statistics_item dtor / empty_stub flipping 100->0), NOT caused by the predicate edit.
The REAL block: the target has THREE statements; the third is
  stats.body_state.push_back( new_stats_item );
calling `buffer_vector<statistics_item<46,16>>::push_back` on a member at offset **0x2798**
(`mov ecx,[ebp+8]; add ecx,2798h; call push_back` in --view diff at 0x42-0x4b, confirmed via
--view callees showing `buffer_vector<statistics_item<46,16> >::push_back`). Our source has this
push_back COMMENTED OUT because `npc_statistics` (vostok/ai/npc_statistics.h) has no `body_state`
member.

### Why body_state is missing and why this is out of scope
Computed npc_statistics member offsets from our header (release: buffer_string=0xC, buffer_vector
=8, fixed_string<N>=0xC+align4(N), fixed_vector<T,C>=8+C*sizeof(T)):
- sensors_state    (fixed_vector<statistics_item<46,16>,5>, size 0x13CC) @ 0x0000
- selectors_state  (same, 0x13CC)                                       @ 0x13CC
- working_memory_state (statistics_item<32,128>, 0x163C)                @ 0x2798  <-- here
sensors_state + selectors_state END exactly at 0x2798. So the target's body_state is a THIRD
input_info_type-like member (`fixed_vector<statistics_item<46,16>, N>`) inserted right after
selectors_state at 0x2798, which shifts working_memory_state and every following member down.
Corroboration: ai_world::fill_npc_stats (0x568d90) indexes members at `add ecx,7378h` - far
beyond our header's total layout (~0x4470) - so our npc_statistics undermodels the real struct by
a large margin. Adding body_state (and correcting the layout) is a cross-module change in the ai
module that re-bases offsets for EVERY ai dump_state / fill_npc_stats / retrieve_statistics
consumer (blackboard/working_memory/sensors/selectors), risking broad regressions. That belongs
to an ai-module npc_statistics layout task, not this game_core structure-verifier pass.
Left the push_back commented with a claude@TODO and embedded the structure-diff + VERDICT. BLOCKED.

## Other functions in this unit (verified clean, STATE synced, carcasses deleted)
- ctor 0x586fd0: structure-diff 1/1 clean, 99.72% (was labeled 93.97). residual stack/reg noise. DONE.
- get_hit_parameters 0x5871b0: 2/2 clean, 99.85%. residual stack size. DONE.
- hit_by_type 0x587910: 29/29 clean, 99.83%. residual stack/reg noise. DONE.
- protect_damage_predicate::operator() 0xb9ef0: 2/2 clean, 100%. DONE (synced 99->100).
- fill_new_stats_item: 91.79% BLOCKED on fixed_string<46>("none") inline (pre-existing, unchanged).

## Rebuild result
Unit 81.45 -> 83.49 (protect_affect_predicate 0->100). report-changes "regressions" are the usual
100->0 ICF/COMDAT shuffle of destructors/thunks/empty_stub plus the npc dump_state 22->17 callee
re-fold noise; none caused by the source edits (only the predicate signature changed compiled bytes).
