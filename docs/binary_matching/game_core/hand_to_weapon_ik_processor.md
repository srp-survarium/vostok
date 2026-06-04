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
- `get_hand_coefficient`                 88.54% PARTIAL (stack-slot idiom)
- `process`                              81.51% PARTIAL (mix_transformations wall)
- `process_hand`                         89.69% PARTIAL (stack-slot layout; opcode-identical)
- `serialize` / `deserialize`            BLOCKED (udp_match_packet/packet_reader cluster)
- `s_ik_hands_debug_draw_cc` init/atexit None (pairing artifact, same as legs)
- bonus, ik_utils.h: `get_angle` filled (now out-of-line, 72.58%);
  `mix_transformations(3-arg)` filled to forward to the 4-arg.

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
  Residual: target stores hand_transition_time directly into the fild qword-low slot and
  round-trips the float return through xmm; base keeps a separate slot. Same instructions.

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

## process_hand (89.69%) - stack-slot residual
Full 2-bone (arm->forearm->hand) IK reconstructed from asm. The disassembly **opcode
stream is byte-identical** to the target and the **distinct local-slot count matches
(57 == 57)**; the only residual is MSVC /Od absolute slot placement (base frame 0x4D4 vs
target 0x54C; a ~0x78 gap appears before `arm_obj_matrix` and at function end). Fixing the
forearm get_bone_matrix to be an inline temp (not the named `forearm_obj_matrix`) took it
84.97->86.38; filling `get_angle` (it was inlining the 0.0f stub and dropping the call)
took it 86.38->89.69. Closing the last 10% needs reproducing MSVC's exact slot allocation.

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
