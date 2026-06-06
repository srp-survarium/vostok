# game_core::player_logic_base_state {ctor, set_user}

Unit: the `player_logic_base_state` ctor and `set_user` setter (two trivial
same-class members sharing the class scaffolding + one anchor). The static
`movement_animation_index` is already matched (#122); the class layout is known.

Target rvas:
- ctor `??0player_logic_base_state@survarium@@QAE@AAVweapon_user_animations_selector@1@W4weapon_user_state_enum@1@@Z` @ 0x58c220 (public `Q`)
- set_user `?set_user@player_logic_base_state@survarium@@UAEXAAUbase_player@2@@Z` @ 0x58c200 (public virtual `U`)

## Target asm

ctor (0x58c220):
```
push ebp; mov ebp,esp; sub esp,8; mov [ebp-8],ecx   ; this
mov ecx,[ebp-8]; call vostok::ai::fsm_state::fsm_state   ; base-class ctor (out-of-line call)
mov eax,[ebp-8]; mov dword [eax], ??_7player_logic_base_state@@6B@   ; vtable @0x00
mov ecx,[ebp-8]; mov edx,[ebp+8];  mov [ecx+18h],edx   ; m_owner = owner            (0x18, ref)
mov eax,[ebp-8]; mov dword [eax+1Ch],0                 ; m_user = NULL              (0x1c)
mov ecx,[ebp-8]; mov edx,[ebp+0Ch]; mov [ecx+20h],edx  ; m_weapon_user_state_id     (0x20)
mov eax,[ebp-8]; mov byte [eax+24h],1                  ; m_is_weapon_weapon_visible (0x24) = true
mov ecx,[ebp-8]; mov byte [ecx+25h],1                  ; m_is_smoothing_needed      (0x25) = true
mov edx,[ebp-8]; mov byte [edx+26h],1                  ; m_is_physics_transform_allowed (0x26) = true
mov eax,[ebp-8]; mov byte [eax+27h],1                  ; m_is_ready_to_be_deactivated (0x27) = true
mov eax,[ebp-8]; mov esp,ebp; pop ebp; ret 8           ; return this
```
=> member-init list in declaration/offset order, all four bools = true, m_user = NULL.

set_user (0x58c200):
```
push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx
mov eax,[ebp-4]; mov ecx,[ebp+8]; mov [eax+1Ch],ecx    ; m_user = &user (0x1c)
mov esp,ebp; pop ebp; ret 4
```
=> `m_user = &user;`

## Base-class ctor: out-of-line `call` vs inline (the orchestrator's risk)
Target emits a real `call vostok::ai::fsm_state::fsm_state`. `ai::fsm_state` has NO
user-declared ctor (only a pure-virtual dtor `= 0`); `fsm_state::fsm_state` is not a
standalone symbol in EITHER rich index. So whether the base build emits a `call`
or inlines the implicit fsm_state ctor is a base-class/whole-program decision, not
steerable from this ctor (assembly_patterns.md "base-class ctor"). If it diverges
it is a matching problem possibly BLOCKED on the base type, NOT DONE-LTCG. To be
confirmed after the build.

## Commands run (verbatim, in order)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function player_logic_base_state --list
- pdb_fetch --target-index ... --function "player_logic_base_state::player_logic_base_state" --view target
- pdb_fetch --target-index ... --function "player_logic_base_state::set_user" --view target
- (COFF symbol dump of target obj for access chars: ctor `QAE`, set_user `UAE`)
- python3 scripts/rebuild.py        # NO module arg

## Iterations
1. INPUT: ctor member-init list (m_owner, m_user(NULL), m_weapon_user_state_id, 4 bools=true);
   set_user `m_user = &user;`. Anchor: concrete_logic_state derived stub overriding
   selected_animations, delegating ctor, calling set_user, escaping &state.
   BUILD: FAILED - C2259 at temp_include_all.cpp(799): concrete_logic_state still abstract.
   CAUSE: player_logic_base_state only overrides fsm_state's is_ready_for_transition;
   initialize/execute/finalize stay PURE. The stub must override those too.
2. INPUT: added `virtual void initialize/execute/finalize() override {}` to the stub.
   BUILD (game_core only, fast check): compiles clean (exit 0). Full relink:
   ctor 100.0%, set_user 100.0% (report.json). movement_animation_index unchanged 100.0%.
   DIFF:  set_user --view diff: 100.00%, byte-identical (m_user = &user @0x1c).
          ctor: --view diff rejects the pair (base mangles arg `weapon_user_state_enum`,
          target `const weapon_user_state_enum`), but report.json scored the symbol 100.0%
          and `pdb_fetch --view target` on the BASE index shows asm IDENTICAL to the target,
          INCLUDING `call vostok::ai::fsm_state::fsm_state` (base-class ctor came out as a
          real out-of-line call, not inlined - the inline-vs-call risk resolved correctly,
          so this is clean DONE, NOT BLOCKED on fsm_state).

## Base-class ctor resolution
The orchestrator flagged the `call fsm_state::fsm_state` as a possible BLOCKED-on-base-type
divergence. It resolved cleanly: the base build ALSO emits the out-of-line `call`, so the
inline-vs-call matched without any source change. fsm_state's implicit ctor stays a (folded)
out-of-line symbol in both binaries.

## Anchor note
player_logic_base_state is abstract: it leaves fsm_state's initialize/execute/finalize PURE
(it only overrides is_ready_for_transition) AND adds its own pure selected_animations. The
concrete_logic_state anchor stub must override ALL FOUR to be instantiable (first build failed
C2259 with only selected_animations overridden). References (owner, user) are fabricated from
NULL pointers - the anchor never runs - to avoid constructing the noncopyable
weapon_user_animations_selector.

## Outcome
STATE[100%|DONE] for BOTH ctor and set_user. Blocker: none. Regressions caused: none
(report-changes shows only COMDAT/dtor/CRT 100<->0 churn - 39 `scalar deleting destructor`
plus float3/float4/float4x4 default ctors, empty_stub, boost/bt/resource_ptr - the symmetric
delinker re-slice noise; no matched source unit regressed). Inlining: base-class fsm_state
ctor is a real out-of-line call in both, matched.
