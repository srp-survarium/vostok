# game_core::weapon_core_base_state::{weapon_core_base_state(weapon_core&,bool), deserializing() const}

Group of two non-blocked members of `survarium::weapon_core_base_state`, matched as one unit.
(serialize/deserialize stay BLOCKED in the udp_match_packet/packet_reader cluster.)

## Target asm (verbatim)

### ctor @ target rva 0x6ecf90  (mangled `??0weapon_core_base_state@survarium@@IAE@AAVweapon_core@1@_N@Z` -> IAE = PROTECTED)
```
push ebp; mov ebp,esp; sub esp,0Ch; mov [ebp-0Ch],ecx
mov ecx,[ebp-0Ch]; call fsm_state::fsm_state          ; base @0x00 (no add)
push 1; mov ecx,[ebp-0Ch]; add ecx,18h; call unmanaged_resource::unmanaged_resource(u32=1)  ; base @0x18
mov ecx,[ebp-0Ch]; add ecx,120h; call <float4()>      ; delinker MISNAME: m_animation_playback_state default ctor @0x120
mov eax,[ebp-0Ch]; mov [eax], vtbl_fsm_state          ; vtable @0x00
mov ecx,[ebp-0Ch]; mov [ecx+18h], vtbl_unmanaged_resource ; vtable @0x18
mov edx,[ebp-0Ch]; add edx,120h; mov [ebp-8],edx      ; &m_animation_playback_state -> slot
mov eax,[ebp-8];  mov dword [eax],0                   ; m_animation_playback_state.interval_id = 0
mov ecx,[ebp-8];  movss xmm0,[rdata+0x14=0.0f]; movss [ecx+4],xmm0  ; .interval_time = 0.0f
mov edx,[ebp-0Ch]; mov eax,[ebp+8]; mov [edx+128h],eax ; m_weapon = weapon (init list)
mov ecx,[ebp-0Ch]; mov dword [ecx+12Ch],0             ; m_is_firing_ptr = NULL
mov edx,[ebp-0Ch]; mov dword [edx+130h],0FFFFFFFFh    ; m_body_part_mask_for_user = -1 (body_part_whole_body)
mov eax,[ebp-0Ch]; mov byte  [eax+134h],0             ; m_is_ready_to_be_deactivated = false
mov ecx,[ebp-0Ch]; mov byte  [ecx+135h],0             ; m_animation_has_been_ended = false
mov edx,[ebp-0Ch]; mov al,[ebp+0Ch]; mov [edx+136h],al ; m_serialize_animation_state = arg
mov eax,[ebp-0Ch]; mov esp,ebp; pop ebp; ret 8
```

### deserializing() @ target rva 0x6ece60
```
push ebp; mov ebp,esp; sub esp,8; mov [ebp-8],ecx
mov eax,[ebp-8]; mov ecx,[eax+128h]    ; this->m_weapon (ref @0x128)
mov dl,[ecx+493h]                      ; weapon_core member @0x493 = m_deserializing
mov [ebp-1],dl; mov al,[ebp-1]
ret
```
=> `return m_weapon.deserializing();` where `weapon_core::deserializing()` inlines `return m_deserializing;` (m_deserializing @0x493).

## Key facts read up-front (zero rebuilds)
- ctor mangled name is **IAE** (protected) -> moved ctor decl to `protected:` in weapon_core_base_state.h (else objdiff pairs as None).
- COFF parse of target obj `sources/weapon_core_base_state.cpp.obj`: ctor sym off 0x114; the
  3rd "ctor" call reloc is `??0float4@math@vostok@@QAE@XZ` (= delinker misname of the empty
  m_animation_playback_state default ctor); the movss reloc points at rdata+0x14 = 0.0f.
- m_deserializing is private in weapon_core (0x493); base_state reaches it via the inline
  `weapon_core::deserializing()` getter (was `{ /* no source */ }`), filled to `return m_deserializing;`.
- animation_playback_state default ctor was empty `{ /* no source */ }`; target shows interval_id=0,
  interval_time=0.0f -> set ctor to `: interval_id(0), interval_time(0.0f) {}`.

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_base_state --list
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function deserializing --list
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function weapon_core_base_state --view target --rva 0x6ecf90
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function deserializing --view target --rva 0x6ece60
- python3 COFF parser over binaries/objdiff/target/vostok/game_core/sources/weapon_core_base_state.cpp.obj (sections/relocs/rdata)
- python3 scripts/rebuild.py    # NO module arg

## Iterations
1. INPUT: ctor body (member-init m_weapon + 5 body assigns), deserializing -> m_weapon.deserializing();
   animation_playback_state ctor interval_id(0)/interval_time(0.0f); weapon_core::deserializing()
   -> return m_deserializing; ctor decl moved to protected:; anchor concrete_state derived stub.
   BUILD #1: LINK FAILED - unresolved ??1fsm_state@ai@vostok@@UAE@XZ. The concrete derived stub
     forces fsm_state's pure-virtual dtor (= 0, no body in our sources) into the vtable.
   FIX: define `vostok::ai::fsm_state::~fsm_state(){}` in temp_include_all.cpp (a NON-target TU,
     so it cannot regress a matched obj). Checked origin/sushi/v0.100b/match-fsm: it keeps the
     dtor pure (= 0) and provides no body, so nobody has matched fsm_state itself.
   BUILD #2: ctor 100.0%; deserializing None.
   DIFF: deserializing mangled QBE (public) in base vs IBE (protected,const) in target -> objdiff
     can't pair -> None. Same access-specifier class as assembly_patterns.md.
2. INPUT: move deserializing() to protected: in weapon_core_base_state.h; expose via concrete_state
   wrapper call_deserializing() (derived can call protected base member); anchor calls it + escapes.
   BUILD #3: ctor 100.0%, deserializing 100.0%. report-changes: 0 regressed, 1 improved
     (deserializing 0->100), 0 removed/added.

## Post-merge correction (#121 follow-up): ctor STRUCTURE does not match
report.json scores the ctor at 100% (instruction + relocation level), so it was banked `100%|DONE`,
but the SOURCE STRUCTURE diverges - the classic "high-% over the wrong structure" (reviewer check #5):

    pdb_fetch --view structure, target rva 0x6ecf90:   2 statements   (0x00 <0xaa> L23 ; 0xaa <0x9> L24)
    pdb_fetch --view structure, base   rva 0x4594d0:   7 statements   ({ ; 5 member assigns ; })

The target attributes the entire init region (0x00-0xaa) to a SINGLE source line (L23) = a full
member-initializer list, with an empty body on L24. Our base initializes only `m_weapon` in the list
and writes the other 5 members as body assignments, producing a `{`-statement + 5 assignment lines + `}`
(7 statements). The bytes coincide (member-init vs body-assign of POD members emit the same stores in
declaration order), which is why report.json reads 100% and the divergence stayed hidden.

Concrete fix (needs a rebuild - a matcher's job, not done here): move all 5 assignments into the
member-initializer list so the source collapses to the target's 2-statement layout, e.g.
`: m_weapon(weapon), m_is_firing_ptr(NULL), m_body_part_mask_for_user(animation::body_part_whole_body),
m_is_ready_to_be_deactivated(false), m_animation_has_been_ended(false),
m_serialize_animation_state(serialize_animation_state) {}`. Verify report.json stays 100% AND
`--view structure` base becomes 2 statements. STATE downgraded ctor -> INPROGRESS; deserializing stays DONE.

## Outcome
STATE: ctor INPROGRESS (bytes 100% but structure mismatch, see correction above), deserializing DONE 100%. Regressions caused: none.
Files: sources/.../weapon_core_base_state.cpp (bodies + STATE), weapon_core_base_state.h (ctor +
deserializing -> protected to match IAE/IBE mangling), weapon_core.h (deserializing() inline ->
return m_deserializing), animation/type_definitions.h (animation_playback_state ctor interval_id(0)/
interval_time(0.0f)), temp_include_all.cpp (anchor + local fsm_state::~fsm_state body).
Inlining: ctor's 3rd sub-object ctor call is the delinker-misnamed empty animation_playback_state
default ctor (shows as float4()); interval_id/interval_time stored explicitly after = the member-init
of that struct's ctor. No residual inlining diff (both 100%).
Foundational gap flagged to orchestrator: fsm_state has a pure-virtual dtor with no body anywhere in
sources (target def @0x3f210). Any concrete fsm_state-derived ANCHOR can't link without a dtor body;
I supplied one locally in the non-target temp_include_all TU. fsm_state should be matched first.
