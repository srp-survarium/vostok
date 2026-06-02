# legs_ik_processor::leg_params setters (grouped unit)

Four trivial setters of the nested struct `survarium::legs_ik_processor::leg_params`,
matched as one unit / branch / commit / PR (accessor-grouping rule).

Branch: `match/game_core-legs_ik_processor-leg_params-setters`
Base (stack tip): `match/game_core-weapon_core_animation_end_aware_state` (#127).

## Functions
- `set_heel_transition_time(float)`  target rva 0x6ea930
- `set_toe_transition_time(float)`   target rva 0x6ea900
- `set_heel_on_ground(bool)`         target rva 0x6ea890
- `set_toe_on_ground(bool)`          target rva 0x6ea820

## Commands run
```
nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --list \
  | grep -iE 'leg_params.*set_(heel|toe)'
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6ea930 --view callees
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6ea890 --view structure
```

## Target asm (decoded against leg_params layout)
leg_params members: heel_transition_time @0x14, toe_transition_time @0x18,
m_time_since_stance @0x28, m_heel_on_ground @0x2c, m_toe_on_ground @0x2d.

### set_heel_transition_time(float tr_time)  (rva 0x6ea930)
```
movss xmm0, [ebp+8]      ; tr_time
movss xmm1, [eax+14h]    ; heel_transition_time
call  vostok::math::min  ; float min(const float, const float)
movss [ecx+14h], xmm0    ; heel_transition_time = result
```
=> `heel_transition_time = math::min( tr_time, heel_transition_time );`
callees: `float vostok::math::min(const float, const float)` (out-of-line, rva 0x239e0).

### set_toe_transition_time(float tr_time)  (rva 0x6ea900)
Same shape, member @0x18 => `toe_transition_time = math::min( tr_time, toe_transition_time );`

### set_heel_on_ground(bool value)  (rva 0x6ea890), public (QAE)
3 statements (structure: L81/L82/L83):
```
[2Ch] = value                                  ; m_heel_on_ground = value
if ( [2Ch] && [2Dh] )  -> bool temp [ebp-0Ch]  ; m_heel_on_ground && m_toe_on_ground
    if (temp) [28h] = 0.0f                      ; m_time_since_stance = 0.0f
```
No `+0x002` brace jmp on L83 -> `if` body is brace-less. The `jmp short .2`
at 0x3b is the `&&` short-circuit materialization, not a source brace.
Constant @[eax+28h] read from target obj .rdata via COFF parser: 0.0f.

### set_toe_on_ground(bool value)  (rva 0x6ea820)
Same as heel but the value is stored to [2Dh] (m_toe_on_ground); condition is the
SAME order (`m_heel_on_ground && m_toe_on_ground`, reads [2Ch] then [2Dh]) and the
same `m_time_since_stance = 0.0f` store. Reproduce verbatim.

## Anchor
`use_game_core_legs_ik_processor_leg_params()` in temp_include_all.cpp:
member-fn-pointer tables for both float and bool setters escaped through
`example_callback` (keep standalone bodies un-inlined), then construct a
`leg_params`, call all four setters, escape `&params` (observe the stores).
Included `<vostok/game_core/legs_ik_processor.h>`; called from `IncludeAll::IncludeAll`.

## Iterations
- v1 (bodies above + anchor): first full rebuild.
  - set_heel_transition_time / set_toe_transition_time: 83.69%
  - set_heel_on_ground / set_toe_on_ground: 59.90%
  - transition_time diff (byte-level, frame identical `sub esp,8`): ONLY the two
    `movss` operands swap which arg goes to xmm0 vs xmm1.
    target: `movss xmm0,[ebp+8] (tr_time); mov eax,[ebp-8]; movss xmm1,[eax+14h] (heel)`
    base:   `mov eax,[ebp-8]; movss xmm0,[eax+14h] (heel); movss xmm1,[ebp+8] (tr_time)`
    Source `math::min( tr_time, heel_transition_time )` is correct (matches target's
    `min(left=tr_time, right=heel)`); the swap is the LTCG custom register convention
    assigning args xmm0/xmm1 at link time - the permitted call-boundary arg-passing
    class. Not source-steerable.
  - bool setters diff (byte-level): target frame `sub esp,0Ch` with three temps
    `[ebp-1]`,`[ebp-2]`,`[ebp-0Ch]`; my direct `m_heel_on_ground && m_toe_on_ground`
    compiled to a leaner `sub esp,4`, `movzx;test` directly off members (no temp
    round-trips). The per-member `mov al,[edx+2Ch]; mov [ebp-1],al; movzx ecx,[ebp-1]`
    round-trip is the inlined `bool is_heel_on_ground() const { return m_heel_on_ground; }`
    return-value temp. FIX: give is_heel_on_ground/is_toe_on_ground real bodies
    (return the member) and write the condition as
    `if ( is_heel_on_ground() && is_toe_on_ground() )`.
- v2 (getter bodies + `if ( is_heel_on_ground() && is_toe_on_ground() )`): rebuild.
  - bool setters: 59.90% -> 78.19%. transition setters unchanged 83.69%.
  - byte-diff set_heel_on_ground: first half now matches (frame `sub esp,0Ch` in
    target vs `sub esp,08` in base) - target uses a THIRD temp `[ebp-0Ch]` for the
    `&&` RESULT, materialized then re-tested by the `if`; my `&&`-directly-in-`if`
    short-circuited straight to the body (no intermediate bool). That extra
    materialize-then-test is the inlined `is_full_on_ground()` whose body is
    `return is_heel_on_ground() && is_toe_on_ground();` returning into a bool temp.
    FIX: condition is `if ( is_full_on_ground() )` + give is_full_on_ground that body.
- v3 (`if ( is_full_on_ground() )` + is_full_on_ground body): rebuild.
  - bool setters: 78.19% -> 100.00%. transition setters unchanged 83.69%.
  - The third bool temp [ebp-0Ch] (the && RESULT) is the inlined is_full_on_ground()
    return-value slot; testing IT in the `if` reproduces the target frame `sub esp,0Ch`.

## Final
- set_heel_on_ground   100% DONE
- set_toe_on_ground    100% DONE
- set_heel_transition_time  83.69% DONE (LTCG arg passing - xmm0/xmm1 register
  assignment for the min call, link-time custom convention; source correct)
- set_toe_transition_time   83.69% DONE (same)
- regressions: none (the 59 report-changes entries are dtor/thunk/empty_stub ICF
  churn in optimized modules; none touch this unit).

## Verification commands
```
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target \
  --rva 0x6ea930 --view diff
# byte-diff via COFF parser on binaries/objdiff/{target,base}/.../legs_ik_processor.cpp.obj
# scores read from binaries/objdiff/report.json (fuzzy_match_percent per mangled symbol).
```

## Deep pass (anchor-removal, match/game_core-legs_ik_processor-deep)
The entire `use_game_core_legs_ik_processor_leg_params` direct anchor was removed (it
escaped a fake live leg_params and took the setters' addresses). Every leg_params member is
reached transitively through the real `processor` instance in `use_game_core_legs_ik_processor`:
- ctor                         <- legs_ik_processor ctor
- set_*_transition_time        <- private set_*_on_ground(leg_params&,bool) helpers <- public processor setters
- leg_params::set_heel/toe_on_ground(bool) <- same private helpers
- leg_params::activate         <- legs_ik_processor::activate
- leg_params::tick             <- legs_ik_processor::tick
- COMMAND: python3 scripts/rebuild.py; python3 scripts/legs_scores.py
- VERIFIED: every leg_params fn still SCORES in report.json (none dead-stripped):
  ctor 100, set_heel/toe_on_ground(bool) 100, set_*_transition_time 83.69, activate 100,
  tick 97.42 - all UNCHANGED vs PR #159. So the prior fake-observation anchor was redundant
  and was NOT the cap; the set_*_transition_time 83.69 residual stays the call-boundary LTCG
  arg-passing already documented for these.
