# game_core::weapon_core_aimed_state_base::{ctor,initialize,finalize}

Group of three members of `survarium::weapon_core_aimed_state_base`
(derives from `weapon_core_base_state` -> `ai::fsm_state` + `resources::unmanaged_resource`).
Mirror of `weapon_core_idle_state_base` (PR #123), stacked on
`match/game_core-weapon_core_idle_state_base`.

Target rvas: ctor 0x6ea600, initialize 0x6ea590, finalize 0x6ea5c0.
(The carcass addresses 0x6fa... are the stale BASE-build rvas.)

## Access chars from the target COFF (read up front, before first build)
`strings binaries/objdiff/target/vostok/game_core/sources/weapon_core_aimed_state_base.cpp.obj`:
- ctor:       `??0...@@IAE@AAVweapon_core@1@@Z`  -> I = protected
- initialize: `?initialize@...@@MAEXXZ`           -> M = PROTECTED virtual
- finalize:   `?finalize@...@@MAEXXZ`             -> M = PROTECTED virtual

DIFFERENCE FROM IDLE: idle's initialize/finalize were E (private, EAE). Aimed's are
M (protected, MAE). The handoff guessed private; the COFF says protected. Reading the
COFF up front set the header right the first try.

## Target asm

### ctor (0x6ea600)
Identical shape to idle: base-ctor delegation `weapon_core_base_state(weapon, false)`
+ two compiler-emitted vtable stores (fsm_state @0x00, unmanaged_resource @0x18).
Source body empty.

### initialize (0x6ea590)
```
mov eax,[ebp-4]; mov ecx,[eax+128h]      ; m_weapon (ref @ 0x128)
mov edx,[ebp-4]; mov eax,[edx+128h]
mov edx,[ecx]                            ; weapon_core vtable
mov ecx,eax                              ; this = weapon_core
mov eax,[edx+8Ch]; call eax              ; VIRTUAL call, vtable slot 0x8c
```
=> `m_weapon.instant_aim_start();`. instant_aim_start is VIRTUAL on weapon_core
(line 126), so the call dispatches via the vtable - unlike idle's non-virtual
instant_idle_start (direct call).

### finalize (0x6ea5c0)
```
mov eax,[ebp-4]; add eax,120h; call animation_playback_state::reset   ; m_animation_playback_state @ 0x120
mov eax,[ebp-4]; mov ecx,[eax+128h]
mov edx,[ebp-4]; mov eax,[edx+128h]
mov edx,[ecx]; mov ecx,eax; mov eax,[edx+90h]; call eax               ; VIRTUAL call, vtable slot 0x90
```
=> `m_animation_playback_state.reset(); m_weapon.instant_aim_end();`. instant_aim_end
is VIRTUAL (line 127). reset() is an out-of-line call (own function, not my unit).

## Commands run (verbatim, in order)
- git checkout match/game_core-weapon_core_idle_state_base; git checkout -b match/game_core-weapon_core_aimed_state_base
- pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_aimed_state_base --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function weapon_core_aimed_state_base --rva 0x6ea600 --view target
- pdb_fetch ... --function initialize --rva 0x6ea590 --view target  (and --view callees)
- pdb_fetch ... --function finalize  --rva 0x6ea5c0 --view target  (and --view callees -> animation_playback_state::reset)
- strings binaries/objdiff/target/vostok/game_core/sources/weapon_core_aimed_state_base.cpp.obj | grep aimed_state_base
- pdb_fetch ... --function reset --rva 0x087f60 --view target  (reset zeros interval_id + interval_time)
- python3 scripts/rebuild.py   # NO module arg
- pdb_fetch ... --view diff (per fn, as needed)

## Scaffolding inherited from PR #121/#123 branch
- temp_include_all.cpp: `vostok::ai::fsm_state::~fsm_state(){}` stopgap.
- weapon_core_base_state.cpp ctor + access specifiers; idle anchor scaffolding pattern.

## Added this unit
- weapon_core_aimed_state_base.h: ctor protected:, initialize/finalize protected: virtual,
  forward-decl + friend of use_game_core_weapon_core_aimed_state_base.
- weapon_core_aimed_state_base.cpp: ctor body empty; initialize = m_weapon.instant_aim_start();
  finalize = m_animation_playback_state.reset(); m_weapon.instant_aim_end();. Added
  #include <vostok/game_core/weapon_core.h>.
- temp_include_all.cpp: include aimed header; use_game_core_weapon_core_aimed_state_base
  anchor (concrete derived stub overriding pure weapon_and_hands_expression, construct,
  qualified non-virtual initialize()/finalize(), escape &state); call it in IncludeAll.

## Iterations
1. INPUT: ctor empty body; initialize = m_weapon.instant_aim_start(); finalize =
   m_animation_playback_state.reset(); m_weapon.instant_aim_end(). Header: ctor
   protected:, initialize/finalize protected: virtual + friend anchor. Anchor mirrors
   idle. `animation_playback_state::reset()` left as the header's empty `{}` body.
   BUILD: code 27.10% -> 27.08% (baseline churn; 48 regressed/70 improved are unrelated
          scalar-deleting-dtors/btHashMap from a stale prior-branch baseline - not mine).
          ctor 100%, initialize 100%, finalize 83.33%.
   DIFF (finalize): target has `add eax,120h; call animation_playback_state::reset`
          (3 instrs) that base lacks. Root cause: reset()'s header body is empty `{}`,
          so /Od ELIDES the no-op call entirely (target's reset zeros both members, so
          it is a real out-of-line call). 15/18 equal.
   FIX:   fill `animation_playback_state::reset()` body in animation/type_definitions.h
          with `interval_id = 0; interval_time = 0.0f;` (matches its own target asm
          xorps/mov 0/movss at 0x087f60) so the call site survives. reset() is reachable
          only via finalize (anchored), so no extra anchor needed. This is an inlining
          cluster: finalize cannot match without reset's real body.
2. INPUT: reset() body filled IN-CLASS in type_definitions.h.
   BUILD: finalize 83.33% -> 54.39% (WORSE). 2 regressed/2 improved (other side baseline churn).
   DIFF (finalize): the in-class body was now visible at the finalize call site and MSVC
          INLINED reset() into finalize (`mov [ecx],0; movss [edx+4],xmm0`) instead of
          calling it. The target keeps an out-of-line `call`. 12/29 equal.
   ROOT:  MSVC inlines a trivial in-class (implicitly-inline) member even at /Od when its
          body is visible. The target avoids this by DECLARING reset() in the struct and
          DEFINING it out-of-class in animation_playback_state.h - a header the finalize
          TU does not include, so finalize sees only a declaration -> emits a call.
   FIX:   (a) type_definitions.h: reset() -> declaration only `inline void reset();`.
          (b) NEW sources/vostok/animation/animation_playback_state.h: out-of-class
              `inline void animation_playback_state::reset(){ interval_id=0; interval_time=0.0f; }`.
          (c) temp_include_all.cpp: #include the new header so the COMDAT body is emitted +
              reachable. finalize.cpp does NOT include it -> declaration-only -> emits call.
3. INPUT: as (2c) - decl in type_definitions.h, out-of-class def in new header included
   by temp_include_all.cpp (but NOT called there).
   BUILD: LNK2001 unresolved external reset(). An `inline` fn is only emitted by a TU that
          ODR-USES (calls) it; temp_include_all.cpp included the def but never called reset,
          so no COMDAT was emitted; finalize.cpp saw only the decl -> unresolved.
   FIX:   add an actual `playback.reset();` call (on a local animation_playback_state) in the
          anchor + escape &playback, so temp_include_all.cpp odr-uses reset and emits the body.
4. INPUT: as (3) + the anchor calls reset() (odr-use to emit the COMDAT).
   BUILD: link OK. ctor 100%, initialize 100%, finalize STILL 54.39%.
   DIFF (finalize): base STILL inlines reset (same `mov [ecx],0; movss [edx+4],xmm0`).
   ROOT:  the decl/def split stops the per-TU /Od inline, but under /GL LTCG the linker
          has reset's COMDAT body whole-program and inlines it into finalize at LINK time -
          uncontrollable cross-module inlining (MATCHING.md). Confirmed: base has NO
          standalone reset (pdb_rich_query base -> "no function matched"); target keeps it
          out-of-line @ 0x087f60. So the target's out-of-line `call reset` cannot be
          reproduced from this function's source - LTCG always folds our reset in.
   DECISION: empty {} reset (iteration 1) gave the CLOSER finalize match (83.33%: the only
          diff is the 3 missing call instrs - a clean elision, frame is correct `push ecx`)
          vs the inlined real body (54.39%, wrong frame + inlined stores). Revert to empty {}.
5. INPUT: revert - reset() back to empty {} in-class; remove the new header + its include +
   the anchor reset() call. ctor/initialize/finalize bodies unchanged.
   BUILD: ctor 100%, initialize 100%, finalize 83.33%. Regressions: none (only baseline-
          artifact scalar-deleting-dtor churn from the rotating prior-branch baseline).

## Outcome
STATE: ctor 100% DONE, initialize 100% DONE, finalize 83.33% PARTIAL (LTCG inline-vs-call
of animation_playback_state::reset - target keeps it out-of-line @0x087f60 and emits
`call reset`; our /GL LTCG inlines/elides it, unsteerable from this function's source).
Regressions caused: none.

Key learnings (also added to loop_performance.md / assembly_patterns.md):
- Access chars from the target COFF: aimed's initialize/finalize are PROTECTED virtual
  (MAE), NOT private (EAE) like idle. Read the obj symbol prefix up front.
- initialize/finalize call instant_aim_start/instant_aim_end VIRTUALLY (vtable slots
  0x8c/0x90 on weapon_core), because those weapon_core methods ARE virtual - unlike idle's
  non-virtual instant_idle_start/end (direct calls). The source is identical in shape
  (m_weapon.instant_aim_*()); the asm differs only because of the virtual-ness.
- A trivial out-of-line callee that the target keeps standalone but our /GL LTCG inlines is
  the uncontrollable inline-vs-call class. The decl/def header split (which stops /Od per-TU
  inlining) does NOT stop LTCG whole-program inlining. Pick whichever callee body gives the
  closer caller match (here empty {} elides -> 83% beats real-body inline -> 54%).
</content>
</invoke>
