# legs_ik_processor on_ground setters (grouped unit)

Six functions of `survarium::legs_ik_processor`, matched as one unit / branch /
commit / PR. The four trivial public left/right setters delegate to the two
private `leg_params&` helpers; the helpers were sizeable (clamp, fermi rebuild,
two delegated transition-time setters), pulled into the same unit because they are
the delegation target of the four trivial setters.

Branch: `match/game_core-legs_ik_processor-on_ground-setters`
Base (stack tip): `match/game_core-ik_processor-ctor-activate` (#142).

## Functions and access chars (from target obj symbol mangling)
- `set_left_heel_on_ground(bool)`   rva 0x6fad50 / target index 0x... ; `QAE` public
- `set_left_toe_on_ground(bool)`    rva 0x6fad20 ; `QAE` public
- `set_right_heel_on_ground(bool)`  rva 0x6facf0 ; `QAE` public
- `set_right_toe_on_ground(bool)`   rva 0x6facc0 ; `QAE` public
- `set_heel_on_ground(leg_params&, bool)`  rva 0x6fab70 ; `AAE` PRIVATE
- `set_toe_on_ground(leg_params&, bool)`   rva 0x6faa70 ; `AAE` PRIVATE

Header change: added `private:` before the two `(leg_params&, bool)` helpers
(they were under the trailing `public:`); `get_foot_fixed_transform`/`process_leg`
that follow are also private (`ABE`/`AAE`), so the single `private:` covers them.
Also gave `transition_time_calculator::reset()` a body `{ m_value = 0.0f; }`
(the target stores 0.0f to m_value at L431/L441 - see below).

## Commands run (verbatim)
```
pdb_fetch --target-index binaries/rich/target/index.jsonl --function set_left_heel_on_ground --view target
pdb_fetch --target-index binaries/rich/target/index.jsonl --function set_left_toe_on_ground --view target
pdb_fetch --target-index binaries/rich/target/index.jsonl --function set_right_heel_on_ground --view target
pdb_fetch --target-index binaries/rich/target/index.jsonl --function set_right_toe_on_ground --view target
pdb_rich_query --index binaries/rich/target/index.jsonl --function set_heel_on_ground --list
pdb_rich_query --index binaries/rich/target/index.jsonl --function set_toe_on_ground --list
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6fab70 --view target   # heel helper
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6faa70 --view target   # toe helper
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6fab70 --view structure
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6faa70 --view structure
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6fab70 --view callees
# COFF parse of binaries/objdiff/target/.../legs_ik_processor.cpp.obj for consts:
#   L431/L441 movss source = 0.0f  (m_*_transition_time_calculator.reset -> m_value=0)
#   clamp consts: half=0.5, epsilon_3=0.001, fermi epsilon __real@3ba3d70a=0.005f
```

## Delegation (the four public setters)
All four are identical-shaped: `mov ecx,[ebp-4]; add ecx, <off>; push; call <helper>`:
- set_left_heel_on_ground  -> add 0x10 (m_left_leg_params),  set_heel_on_ground(params,value)
- set_left_toe_on_ground   -> add 0x10,                      set_toe_on_ground(params,value)
- set_right_heel_on_ground -> add 0x40 (m_right_leg_params), set_heel_on_ground(params,value)
- set_right_toe_on_ground  -> add 0x40,                      set_toe_on_ground(params,value)
m_left_leg_params @0x10, m_right_leg_params @0x40 (header). Verified, not assumed.

## Heel helper set_heel_on_ground(leg_params& params, bool value) (rva 0x6fab70)
this=[ebp-24h], params=[ebp+8], value=[ebp+0Ch]. 16 statements L423..L443.
```
L424  read params.m_heel_on_ground(0x2c) into temp, cmp value   -> if ( params.is_heel_on_ground() == value )
L425  jmp .3 (<0x5>)                                            ->     return;
L426  params.set_heel_on_ground(value)                          -> params.set_heel_on_ground( value );
L428  test value; je .2 (<0x8>)                                 -> if ( value )
L430  fld[this+0x94]; fstp[params+0x14]                         ->   params.heel_transition_time = m_heel_transition_time;
L431  movss [this+0x88], 0.0f                                   ->   m_heel_transition_time_calculator.reset();
L433  jmp .3 (<0x5>)                                            -> } (brace -> jmp)
L435  m_heel_transition_time(0x94) = [this+0x88]                -> else { m_heel_transition_time = m_heel_transition_time_calculator.get_value();
L436  clamp<float>(&[0x94], 0.001f/xmm1, 0.5f/stack)            ->   math::clamp( m_heel_transition_time, 0.001f, 0.5f );
L437  leg_params::set_heel_transition_time on [this+0x10]       ->   m_left_leg_params.set_heel_transition_time( m_heel_transition_time );
L438  leg_params::set_heel_transition_time on [this+0x40]       ->   m_right_leg_params.set_heel_transition_time( m_heel_transition_time );
L439  fermi_interpolator(m_heel_transition_time,0.005f) temp,
      copy +4/+8 into [this+0x70], finalize_impl(temp dtor)     ->   m_heel_interpolator = fermi_interpolator( m_heel_transition_time );
L440  fld[this+0x98]; fstp[params+0x18]                         ->   params.toe_transition_time = m_toe_transition_time;
L441  movss [this+0x8C], 0.0f                                   ->   m_toe_transition_time_calculator.reset(); }
```
NOTE: the else branch (value==false) touches BOTH heel (L435-439) AND toe (L440-441)
members. Odd but faithful (rule #1). The L439 fermi rebuild copies only the two
float fields (+4 m_total_transition_time, +8 m_epsilon), leaving the vtable at +0;
finalize_impl is the COMDAT-folded temp dtor (see #142 / leg_params PROGRESS).

## Toe helper set_toe_on_ground(leg_params& params, bool value) (rva 0x6faa70)
11 statements L446..L458. SAME prologue, but the value==true branch is EMPTY:
```
L447  if ( params.is_toe_on_ground() == value )
L448      return;
L449  params.set_toe_on_ground( value );
L450  test value; jne .2 (<0xc>)                                -> if ( !value )
L452  m_toe_transition_time(0x98) = [this+0x8C].get_value()     -> { m_toe_transition_time = m_toe_transition_time_calculator.get_value();
L453  clamp( m_toe_transition_time, 0.001f, 0.5f );
L454  m_left_leg_params.set_toe_transition_time( m_toe_transition_time );
L455  m_right_leg_params.set_toe_transition_time( m_toe_transition_time );
L456  m_toe_interpolator = fermi_interpolator( m_toe_transition_time ); }
```
Asymmetry vs heel: toe has no value==true body and no trailing heel/toe stores.

## Resolved: fermi assignment + shared header fix
`m_*_interpolator = fermi_interpolator(...)` initially failed to compile: the
working `sources/vostok/animation/fermi_interpolator.h` declared a private
`operator=` AND `float const` members, both of which make memberwise copy-assign
illegal (C2248 / deleted implicit op=). The PDB GROUND-TRUTH structure header
(`binaries/structure/target/headers/vostok/animation/fermi_interpolator.h`) shows
the members as plain non-const `float` and NO declared operator= - i.e. the real
build had the compiler-implicit public memberwise op= (copies +4/+8, skips vtable),
which is exactly the target's L439/L456 bytes. So the `float const` + private op=
in the working header was a reconstruction error. Fixed the working header to match
the PDB: non-const members, removed the private operator= decl. fermi_interpolator.cpp
still compiles (its ctor uses an init-list, fine for non-const). This is the only
fermi consumer that assigns; `game`/`animation` only construct it, so no codegen of
matched code changes. Also had to qualify as `animation::fermi_interpolator` in the
cpp (file is in namespace survarium).

Compile sequence (targeted obj, ~1min incremental each):
- v0: `fermi_interpolator(...)` unqualified -> C3861 identifier not found
- v1: `animation::fermi_interpolator(...)` -> C2248 cannot access private operator=
- v2: header non-const + drop private op= -> EXIT=0 clean compile

## Anchor
`use_game_core_legs_ik_processor()` in temp_include_all.cpp: construct a
`legs_ik_processor`, call all four public setters, escape &processor. The two
private helpers survive transitively (called by the public setters).

## Iterations
- v1 (bodies as written + header private:/reset()/non-const fermi + anchor):
  full `python3 scripts/rebuild.py`. report.json fuzzy_match_percent:
  - set_left_heel_on_ground   100.000
  - set_left_toe_on_ground    100.000
  - set_right_heel_on_ground  100.000
  - set_right_toe_on_ground   100.000
  - set_heel_on_ground(leg_params&,bool)   98.837
  - set_toe_on_ground(leg_params&,bool)    98.592
  report-changes: all 6 improved 0.00 -> final; 68 regressed are the usual
  symmetric COMDAT/dtor/CRT delinker re-slice churn (empty_stub, scalar-deleting-
  dtors, float3/4/4x4 ctors, boost _bi::value/storage3, bt*/Scaleform/ai/particle/
  render dtors, noncopyable, interlocked) - none touch a matched game_core unit,
  balanced by 49 improved. Identical pattern to #142/#134.

  DIFF (both helpers, only-divergence): every statement, the control structure
  (if(value)/else for heel; if(!value) for toe), the clamp consts (0.001/0.5), the
  reset() 0.0f stores, and both delegated set_*_transition_time calls match exactly.
  The ONLY structural delta is `+ lea ecx,[ebp-0Ch]` (target-only) immediately
  before the temp `~fermi_interpolator` dtor call, plus a trailing `+ nop`
  (delinker inter-function alignment padding, not codegen):
    target 0x10f: fstp[edx+8]; 0x112: lea ecx,[ebp-0Ch]; 0x115: call <dtor>
    base   0x10f: fstp[edx+8];                            0x112: call <dtor>
  The dtor symbol differs only by ICF representative: target's is named
  `...fixed_size_allocator<...>::finalize_impl`, base's `boost::function1<...>::
  dummy::nonnull` - both are the same COMDAT-folded trivial empty function. The
  target's call site sets `this`(ecx) for the folded __thiscall dtor; base's does
  not. This is the destructor-call `this`-argument setup at the call boundary - the
  permitted ICF/LTCG call-boundary arg-passing class, not source-steerable. The
  fermi assignment statement itself is correct (everything else in the block matches
  byte-for-byte). No further source change recovers the 3 bytes.

## Outcome
- set_left_heel_on_ground   100%   DONE
- set_left_toe_on_ground    100%   DONE
- set_right_heel_on_ground  100%   DONE
- set_right_toe_on_ground   100%   DONE
- set_heel_on_ground(leg_params&,bool)  98.84% DONE (ICF/LTCG dtor-this residual)
- set_toe_on_ground(leg_params&,bool)   98.59% DONE (same)
- Regressions caused: none (only symmetric COMDAT/dtor/CRT delinker churn in
  optimized/out-of-scope modules; no matched game_core unit regressed).
- Header changes: legs_ik_processor.h - `private:` before the two leg_params&
  helpers (AAE), transition_time_calculator::reset() body `{ m_value = 0.0f; }`;
  fermi_interpolator.h - non-const members + dropped private operator= (PDB match).

## Verification commands
```
python3 - <<'PY'  # read fuzzy_match_percent from binaries/objdiff/report.json
PY
pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target \
  --rva 0x6fab70 --view diff      # heel helper
  --rva 0x6faa70 --view diff      # toe helper
```
