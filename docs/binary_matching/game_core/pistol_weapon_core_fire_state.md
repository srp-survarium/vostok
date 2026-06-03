# pistol_weapon_core_fire_state

Unit: STUB functions in `sources/vostok/game_core/sources/pistol_weapon_core_fire_state.cpp`.
Closest matched reference: `pistol_weapon_core_idle_state.cpp` (same shape: ctor with
animation-fill loops, get_weapon_lexeme_pair via `get_weapon_lexeme_pair_impl`, new_object).

## Symbols (target index binaries/rich/target/index.jsonl)
- 0x79b170 ctor(weapon_core&, float, managed_resource_ptr const*, u32)
- 0x79b710 initialize()
- 0x79b4e0 get_weapon_lexeme_pair(mutable_buffer&, bool, weapon_user_state_enum) const
- 0x79b5b0 weapon_and_hands_expression(...) const  (large, addition_lexeme machinery)
- 0x79b380 get_user_hands_expression(...) const      (large)
- 0x79b310 new_object(mutable_buffer, weapon_state_creation_params const*, ...)

## Batch chosen
ctor, initialize, get_weapon_lexeme_pair, new_object (4 tractable).
weapon_and_hands_expression + get_user_hands_expression left STUB (large lexeme
machinery, separate effort).

## Commands
- nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target
- nix develop -c python3 scripts/rebuild.py

## Findings

### ctor (0x79b170)
- Calls weapon_core_fire_state_base(weapon, animation_time_scale).
- ASSERT_CMP_U(animations_count, ==, 8) at the front? -> actually the front block at
  0x6b..0x8b is the leading ASSERT_CMP_U-style eater. Two finalize_impl calls (0x72 and
  0x189) are compiled-out asserts. Mirror idle ctor: `ASSERT_CMP_U(animations_count,==,8)`
  at top, `ASSERT(UNKNOWN_EXPRESSION)` near end.
- Two triple/double nested loops with a SHARED animation_index ([ebp-4], never reset):
  - loop1: m_weapon_animations[view][user_state][weapon_state] (2x2x2) = animations[idx++]
  - loop2: m_user_animations[view][user_state] (2x2) = animations[idx++]
- m_weapon_animation_index = -1 set in ctor body (0x61 mov [edx+178h],0FFFFFFFFh) — emitted
  by the vector-constructor-iterator/init? Actually 0x61 is explicit `m_weapon_animation_index = u32(-1)`.

### initialize (0x79b710)
- weapon_core_fire_state_base::initialize();
- last_shot = m_weapon.get_bullets_in_queue() ? (ammo_in_magazine()==1) : (ammo_in_magazine()==0)
- m_weapon_animation_index = last_shot;

### get_weapon_lexeme_pair (0x79b4e0)
- captions = { "pistol-shot", "pistol-shot_last" }
- animation_index = m_weapon_animation_index
- identifier = captions[animation_index]
- selected = m_weapon_animations[is_third_view!=false][user_state_id==type_crouch][animation_index]
- set_animation_to_wait(selected)
- return get_weapon_lexeme_pair_impl(buffer, identifier, selected, &m_weapon,
    m_animation_playback_state, 1, m_animation_timescale, m_playback_type,
    linear_interpolator(s_aim_transition_time))

## RESULTS (after rebuild)
- ctor                    100%   DONE  (added m_weapon_animation_index(u32(-1)) to init list)
- get_weapon_lexeme_pair  100%   DONE
- initialize              92.62% PARTIAL  (thiscall this eax-vs-ecx LTCG + bool-rvalue boolize)
- new_object              92.08% PARTIAL  (LTCG calling convention of computed_shooting_animation_time_scale)
- weapon_and_hands_expression  ~14% INPROGRESS (large addition_lexeme/operator+ machinery)
- get_user_hands_expression    ~13% INPROGRESS (large lexeme machinery)

## Scaffolding added
- weapon_core_fire_state_base.h: m_animation_timescale/m_playback_type private -> protected
  (read by derived get_weapon_lexeme_pair). No codegen change to the base.
- pistol_weapon_core_fire_state.h: access specifiers fixed to match mangling
  (ctor @@IAE protected; initialize @@MAE protected virtual; weapon_and_hands_expression
  @@EBE private virtual const; get_weapon_lexeme_pair/get_user_hands_expression @@ABE private const).
  Added cook_template + use_game_core_pistol_weapon_core_fire_state friends.
- weapon_core_state_cook_template.h: added use_game_core_pistol_weapon_core_fire_state friend + decl.
- temp_include_all.cpp: include + anchor use_game_core_pistol_weapon_core_fire_state + dispatch call.
- pistol_weapon_core_fire_state.cpp: included weapon_animations_timescale_inline.h to provide the
  computed_shooting_animation_time_scale definition (new_object is its only current caller).

### new_object (0x79b310)
- return new(buffer.c_ptr()) pistol_weapon_core_fire_state(params->weapon,
    computed_shooting_animation_time_scale(...), animations, animations_count)
- target @0x0c calls intrusive_ptr operator* on [ebp+8] (buffer), pushes result, operator new(0x180).
- @0x3d call survarium::computed_shooting_animation_time_scale(...) — float arg.
