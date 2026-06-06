# game_core::hand_to_weapon_ik_processor (batch unit)

One unit matching the IK-processor functions in `hand_to_weapon_ik_processor.cpp`
(+ its header, `ik_utils.h`, and the `temp_include_all.cpp` anchor).

Branch: `match/game_core-hand_to_weapon_ik_processor` off `origin/int/game_core`.

## Final results (report.json top-level fuzzy_match_percent)
- `hand::hand()`                         100%  DONE  (header member-init ctor)
- `hand_to_weapon_ik_processor::hand_to_weapon_ik_processor()`  100% DONE
- `activate`                             100%  DONE
- `activate_hand`                        100%  DONE
- `hand_need_correction`                 100%  DONE
- `hand_need_interpolation`              100%  DONE
- `get_hand_new_start_transition_time`   100%  DONE
- `get_hand_coefficient`                 99.90% PARTIAL (was 88.54; single-statement coeff + named return local - RE-MATCH below)
- `process`                              81.51% PARTIAL (mix_transformations wall - genuine, slerp_optimized has no source)
- `process_hand`                         90.37% PARTIAL (was 89.69; .c.xyz() reference hoist restored L134 - RE-MATCH below)
- `serialize` / `deserialize`            BLOCKED (udp_match_packet/packet_reader cluster)
- `s_ik_hands_debug_draw_cc` init/atexit None (pairing artifact, same as legs)
- ik_utils.h: `get_angle` 72.58% PARTIAL (inline-vs-call of vostok::math::acos, LTCG class);
  `mix_transformations(3-arg)` BLOCKED (forwards to the 4-arg STUB; None in report.json).

## REVIEW (audit) corrections - 2026-06-04
- get_angle was marked `100%|DONE` in ik_utils.h but report.json scores it 72.58%. The
  residual is a real divergence: target keeps `call vostok::math::acos` (out-of-line, frame
  sub esp,14h); our /GL LTCG inlines it to `_CIacos` + extra movss/fld slot round-trips
  (sub esp,1Ch). acos has a standalone symbol in BOTH indexes (target 0x27c80 / base 0x37930)
  -> documented per-call-site inline-vs-call LTCG class, not steerable from this body. Source
  correct, but it is NOT a clean DONE -> re-tagged PARTIAL 72.58%, carcass restored.
- mix_transformations(3-arg) was marked `100%|DONE` but report.json scores it None (no base
  bytes: both overloads exist out-of-line in TARGET, neither survives in BASE - DSE'd because
  the 4-arg is a STUB). Re-tagged BLOCKED, carcass restored.
- get_hand_coefficient / process_hand: the matcher banked both as PARTIAL with a "MSVC /Od
  slot placement / slot idiom, same instructions" rationale. That is WRONG - both are
  source-steerable STRUCTURE divergences (a high % hiding the wrong shape), re-tagged
  INPROGRESS with the concrete restructure (below). Logic/compiled bytes NOT changed in this
  review (no rebuild) - flagged for a faster machine to re-match.

## RE-MATCH applying the review diagnoses - 2026-06-06 (both reviewer fixes confirmed)

### get_hand_coefficient: 88.54% -> 99.90%
Two source edits, each verified by rebuild:
1. Collapse `(current-start)/1000.0f` to ONE statement (drop the named hand_transition_time
   local). The reviewer was right: the split emitted an extra `mov [ebp-4],edx; mov eax,[ebp-4]`
   round-trip through a slot the target never allocates. 88.54 -> 95.54.
2. Bind the ternary result to a NAMED return local and `return` it:
   `float const hand_coefficient = is_active ? ... ; return hand_coefficient;`
   The target materializes the return through `movss xmm0,[result]; movss [ebp-8],xmm0;
   fld [ebp-8]` (an extra slot for the return value); a bare `return <ternary>` gives only a
   plain `fld`. The named local reproduces the round-trip. 95.54 -> 99.90.
Residual 0.10%: ONE /Od stack-slot offset - target gives the return local its own fresh slot
[ebp-8]; base reuses the now-dead interpolation_coeff slot [ebp-4] for it (one mov/fld operand).
Frame (sub esp,1Ch), instruction stream and statement structure are otherwise identical. Tried
const vs non-const on the result local: no effect on the slot. Genuine /Od slot-numbering noise.

### process_hand: 89.69% -> 90.37%
The reviewer's "missing L134 named temp" was correct in spirit but the FIX was subtler:
- A full-matrix hoist (`float4x4 forearm_obj_matrix_0 = get_bone_matrix_in_object_space(...)`)
  was WRONG and REGRESSED it (89.69 -> 89.54): the target does NOT keep the whole float4x4 -
  it materializes only the `.c.xyz()` of the temporary. Target L134 asm: `call get_bone_matrix;
  add eax,30h (= &matrix.c); call finalize_impl (.xyz ASSERT); mov [ebp-10h],eax`. So L134 binds
  a `float3 const&` to the `.c.xyz()` reference, not a matrix value.
- CORRECT fix:
  `float3 const& original_forearm_pos = get_bone_matrix_in_object_space( forearm_bone, *m_skeleton, matrices ).c.xyz( );`
  `float3 const& original_forearm_dir = math::normalize( arm_pos - original_forearm_pos );`
  This restored the statement (89.54 -> 90.37). Statement count now 37==37 (target == base),
  control flow identical (diff of the opcode stream shows ZERO jmp/je/cmp/call-target diffs -
  only push/mov/lea/add argument-marshalling differences remain).
- Tried free `math::length(arm_pos - ...)` for L190 (target uses out-of-line `vostok::math::length`):
  REGRESSED (90.37 -> 90.22) because our LTCG inlines the free `length(float3 const&)` overload to
  `float3_pod::length` PLUS a `mov [ebp-3FCh],eax; mov eax,...` temp round-trip - worse than the
  member `.length()` which inlines to a clean `call float3_pod::length`. Reverted to member.
Residual ~9.6%: per-call-site inline-vs-out-of-line ABI for the vector-math helpers (operator-,
normalize, free math::length). Target calls them OUT-OF-LINE with register-NRV (`mov esi/edi`) and
cdecl-by-ref; our /GL LTCG inlines them to push/push + member-length. This cascades the frame size
(target 0x54C vs base 0x4DC, 0x70 gap) and the operand slots. Same per-call-site inline-vs-call
LTCG class documented above for get_angle/acos in this TU - NOT source-steerable from this body.

## Key facts
- RVAs in the carcass comments are +0x10000 vs the real target index
  (carcass 0x593f70 == target 0x583f70). Query with `pdb_rich_query --list`.
- Access specifiers (mangled char drives pairing): `get_hand_coefficient`/`process_hand`
  are `ABE` = **private const**; the three statics (`hand_need_*`,
  `get_hand_new_start_transition_time`) are `C..` = **private static**. They were
  declared public on the base header -> objdiff scored them `None` until moved to
  `private:`.
- A function present in the compiled `.obj` but NOT reachable in the linked base EXE
  scores `None` (the base rich index is built from the EXE). `activate`/`process` were
  `None` until anchored: added `use_game_core_hand_to_weapon_ik_processor()` to
  `temp_include_all.cpp` (construct, call `activate`/`activate_hand`/`process`, escape
  `&processor`). process() then transitively keeps the private helpers + process_hand.
- ctor: `m_interpolator( s_aim_transition_time=0.3f )`, `m_current_transition_time( 0.0f )`,
  `m_active( true )`. Constant bytes read from the EXE: s_aim_transition_time @0x93d484
  = 0x3e99999a (0.3f); the `m_current_transition_time` const @0x974044 = 0x00000000 (0.0f,
  pooled separately so it loads via `movss`, not `xorps`). The leading `call` (delinker:
  finalize_impl/boost dummy::nonnull) is the `core::noncopyable` base subobject ctor and
  matches structurally.
- `hand` struct ctor: start_transition_time=0, hand_bone_index/hand_matrix_index/
  locator_matrix_index = u32(-1), is_active = true.
- `activate`: stores `&user_skeleton` to m_skeleton, `get_bone_index("Weapon")` ->
  m_weapon_bone_index, then per hand `get_bone_index("LeftHand"/"RightHand")` and
  `get_bone_index("left_hand_cont"/"right_hand_cont") - get_root_bones_count()`. Strings
  verified by mangled length + `strings` dump.
- `get_hand_new_start_transition_time` and `get_hand_coefficient`: the merge-to-temp
  (both branches write one slot, then load it for the single `ret`) is a **ternary**, not
  if/else - structure view showed the whole branch as ONE statement on one line.
  Ternary took get_hand_new 84->100; get_hand_coefficient 80->88.54 (residual below).
- `hand_need_interpolation`: `(current_time - h.start) < 300` (0x12C). `hand_need_correction`:
  `h.is_active || hand_need_interpolation(...)`.
- `get_hand_coefficient`: `(current_time-h.start)/1000.0f` (__real@447a0000 = 1000.0f),
  then `is_active ? 1 - interp.interpolated_value(c) : interp.interpolated_value(c)`.
  [REVIEW CORRECTION] residual is NOT a slot idiom. Target structure = 6 statements; L187 is
  ONE statement computing `(current-start)/1000.0f` straight into the fild qword-low slot. Our
  source splits it into two locals (hand_transition_time, interpolation_coeff), so the base
  emits an extra `mov [ebp-4],edx; mov eax,[ebp-4]` round-trip through a slot the target never
  allocates - 2 EXTRA instructions, a real structure diff. FIX: collapse to a single statement
  `float const interpolation_coeff = ( current_time_in_ms - h.start_transition_time_in_ms ) /
  1000.0f;` (drop the named hand_transition_time local), rebuild, re-diff. The trailing
  fld-via-[ebp-8] return round-trip may be a second residual; re-check after the merge.

## process (81.51%) - PROVEN wall
The target calls `mix_transformations` OUT-OF-LINE (NRV). Its 3-arg overload (ik_utils.h)
forwards to the 4-arg, whose real body is a quaternion slerp + `create_matrix` that calls
**`slerp_optimized`** - a function file-local to `math_quaternion.cpp` with NO source in
our tree. So the 4-arg stub returns a trivial default float4x4 and MSVC /Od collapses the
whole 3-arg call to an inline default-construct, which cannot be the target's out-of-line
call. Everything else (control flow, the `const&` bindings for weapon/locator/target/hand
transforms, the `if (hand_need_correction) { ... }` wrapping, the pointer-iterator loop)
matches; the residual is the stack-slot cascade caused by the missing NRV temp. Unblock =
build out `slerp_optimized` / the 4-arg mix_transformations (separate unit).

## process_hand (89.69%) - REVIEW: structure divergence, NOT slot placement
Full 2-bone (arm->forearm->hand) IK reconstructed from asm. [REVIEW CORRECTION] the matcher
banked this as "opcode stream byte-identical, slot count 57==57, residual = /Od absolute slot
placement". That mis-reads it: the TARGET structure has **37 statements**, our base has **36**.
The target keeps the inner `get_bone_matrix_in_object_space( forearm_bone, ... )` as its OWN
statement (L134, named temp, +0x2c) feeding `original_forearm_dir` at L135 (+0x2e); our source
inlines it into L135 as one fused statement (+0x5c), so we drop a statement the target had. The
frame/slot gap (base 0x4D4 vs target 0x54C) cascades from that missing temp - it is a real,
source-steerable structure diff, not allocation noise. FIX: hoist the forearm object-space
matrix to a named local on its own line before `original_forearm_dir`, rebuild, re-diff. Also
re-check the L119/L120 matrix_index statements for an embedded ASSERT (`call ...finalize_impl`
at 0xd2 in the target body). NOTE: the matcher's earlier "fix the forearm get_bone_matrix to be
an inline temp (84.97->86.38)" went the WRONG direction - the target wants it OUT as a named
statement. Filling `get_angle` (86.38->89.69) was correct.

Helpers: `get_rotation_matrix`/`change_matrix_orientation` are declared in this TU and
resolve to legs_ik_processor.cpp's external definitions; `get_relative_matrix` is defined
inline (COMDAT) here as in legs. All are called out-of-line so no scored bytes are touched.

## Commands (representative)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function <name> --list
- pdb_fetch --target-index ... --rva <rva> --view target/structure/callees
- pdb_fetch --base-index ... --rva <rva> --view base   (diff via opcode comparison)
- python3 scripts/rebuild.py   (full, no module arg)
- xxd on $SURVARIUM_BIN/survarium.exe to read float/string constants (RVA->file:
  .text RVA-0x1000+0x400; .rdata VMA-0x7f4000+0x7e3200; .data VMA-0x9aa000+0x999200).
