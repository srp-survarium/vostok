# survarium:: header triage - pass 1 (game_core coverage)

Queue headers removed because the type already lives in sources/vostok/game_core/
(basename = unique survarium:: type name).

## Removed (238)

- `affects_threshold.h` (exact)
- `affects_threshold_1.h` (variant-of-game_core-type)
- `animation_analysis_result.h` (exact)
- `animation_analysis_result_cook.h` (exact)
- `animation_analysis_result_cook_user_data.h` (exact)
- `animation_analyzer.h` (exact)
- `animation_analyzer__leg_cycle_sample.h` (nested-of-game_core-outer)
- `anomaly_state.h` (exact)
- `artefact_base.h` (exact)
- `artefact_container_core.h` (exact)
- `artefact_lifebone_core.h` (exact)
- `artefact_spores_core.h` (exact)
- `base_game_object.h` (exact)
- `base_player.h` (exact)
- `base_player_1.h` (variant-of-game_core-type)
- `base_player_creation_params.h` (exact)
- `base_project.h` (exact)
- `base_project__resolve_link_object.h` (nested-of-game_core-outer)
- `body_part_parameters.h` (exact)
- `body_part_parameters_1.h` (variant-of-game_core-type)
- `body_part_parameters_modifyer.h` (exact)
- `booby_trap_core.h` (exact)
- `booby_trap_core_cook.h` (exact)
- `booby_trap_set_cook_data.h` (exact)
- `booby_trap_set_core.h` (exact)
- `booby_trap_set_core__apply_damage.h` (nested-of-game_core-outer)
- `booby_trap_set_core__config_params.h` (nested-of-game_core-outer)
- `booby_trap_set_core_cook.h` (exact)
- `breath_holding_params.h` (exact)
- `breath_state.h` (exact)
- `breath_vibration_calculator.h` (exact)
- `bullet.h` (exact)
- `bullet_manager.h` (exact)
- `bullet_manager__bullet_functor.h` (nested-of-game_core-outer)
- `bullet_manager__bullet_functor_mt_allocator.h` (nested-of-game_core-outer)
- `bullet_manager_engine.h` (exact)
- `character_dispersion_calculator.h` (exact)
- `character_dispersion_calculator_1.h` (variant-of-game_core-type)
- `character_dispersion_params.h` (exact)
- `character_recoil_calculator.h` (exact)
- `character_recoil_calculator_1.h` (variant-of-game_core-type)
- `character_recoil_params.h` (exact)
- `client_player_update.h` (exact)
- `collision_geometry.h` (exact)
- `collision_geometry_subscriber.h` (exact)
- `collision_sensor.h` (exact)
- `collision_user.h` (exact)
- `damage_info_type.h` (exact)
- `damage_model.h` (exact)
- `damage_model_1.h` (variant-of-game_core-type)
- `damage_model_cook.h` (exact)
- `damage_protector.h` (exact)
- `damage_zone_core.h` (exact)
- `damage_zone_core__damage_zone_user_data.h` (nested-of-game_core-outer)
- `dictionary_item.h` (exact)
- `dispersion_calculator.h` (exact)
- `dispersion_calculator_1.h` (variant-of-game_core-type)
- `double_barreled_weapon_core_aimed_fire_state.h` (exact)
- `double_barreled_weapon_core_aimed_idle_state.h` (exact)
- `double_barreled_weapon_core_fire_state.h` (exact)
- `double_barreled_weapon_core_hide_state.h` (exact)
- `double_barreled_weapon_core_idle_state.h` (exact)
- `double_barreled_weapon_core_reload_state.h` (exact)
- `double_barreled_weapon_core_show_state.h` (exact)
- `engine.h` (exact)
- `game_material.h` (exact)
- `game_material_manager.h` (exact)
- `game_material_manager_cook.h` (exact)
- `game_material_manager_cook__query_ext_data.h` (nested-of-game_core-outer)
- `game_scene.h` (exact)
- `game_world_object.h` (exact)
- `generic_anomaly_core.h` (exact)
- `hand_to_weapon_ik_processor.h` (exact)
- `hand_to_weapon_ik_processor__hand.h` (nested-of-game_core-outer)
- `hit_info.h` (exact)
- `hit_initiator.h` (exact)
- `hit_receiver.h` (exact)
- `hit_type_parameters.h` (exact)
- `hit_type_parameters_modifyer.h` (exact)
- `hittable_object.h` (exact)
- `ik_processor.h` (exact)
- `interactive_object.h` (exact)
- `inventory.h` (exact)
- `inventory_cook.h` (exact)
- `inventory_cooker_data.h` (exact)
- `inventory_holder.h` (exact)
- `inventory_item.h` (exact)
- `inventory_item_instance.h` (exact)
- `inventory_item_props.h` (exact)
- `inventory_slot.h` (exact)
- `items_cook.h` (exact)
- `items_dictionary.h` (exact)
- `items_dictionary_cook.h` (exact)
- `jump_logic.h` (exact)
- `jump_logic_1.h` (variant-of-game_core-type)
- `jump_logic_base_state.h` (exact)
- `jump_logic_state_inactive.h` (exact)
- `jump_logic_state_landing.h` (exact)
- `jump_logic_state_landing_1.h` (variant-of-game_core-type)
- `jump_logic_state_start.h` (exact)
- `jump_logic_state_start_1.h` (variant-of-game_core-type)
- `ladder.h` (exact)
- `ladder__ladder_occluder.h` (nested-of-game_core-outer)
- `ladder_cook.h` (exact)
- `landing_point.h` (exact)
- `leg_info.h` (exact)
- `leg_key_times.h` (exact)
- `legs_ik_drawer.h` (exact)
- `legs_ik_processor.h` (exact)
- `legs_ik_processor__leg_params.h` (nested-of-game_core-outer)
- `legs_ik_processor__transition_time_calculator.h` (nested-of-game_core-outer)
- `link_resolver.h` (exact)
- `material_pair.h` (exact)
- `medkit.h` (exact)
- `medkit__affect.h` (nested-of-game_core-outer)
- `medkit__damage_protection.h` (nested-of-game_core-outer)
- `medkit__item_influence.h` (nested-of-game_core-outer)
- `missile_weapon_core.h` (exact)
- `normal_random.h` (exact)
- `oxygen_tank.h` (exact)
- `oxygen_tank__item_influence.h` (nested-of-game_core-outer)
- `pistol_weapon_core_aimed_fire_state.h` (exact)
- `pistol_weapon_core_aimed_idle_state.h` (exact)
- `pistol_weapon_core_fire_state.h` (exact)
- `pistol_weapon_core_hide_state.h` (exact)
- `pistol_weapon_core_idle_state.h` (exact)
- `pistol_weapon_core_reload_state.h` (exact)
- `pistol_weapon_core_show_state.h` (exact)
- `player_actions_subscriber.h` (exact)
- `player_death_subscriber.h` (exact)
- `player_initial_info.h` (exact)
- `player_input.h` (exact)
- `player_logic_base_state.h` (exact)
- `player_logic_base_state_1.h` (variant-of-game_core-type)
- `player_logic_crouch_state.h` (exact)
- `player_logic_crouch_state_1.h` (variant-of-game_core-type)
- `player_logic_jump_state.h` (exact)
- `player_logic_sprint_state.h` (exact)
- `player_logic_stand_state.h` (exact)
- `player_logic_stand_state_1.h` (variant-of-game_core-type)
- `player_parameters_modifyer.h` (exact)
- `player_parameters_modifyer_cook.h` (exact)
- `player_profile.h` (exact)
- `player_profile_1.h` (variant-of-game_core-type)
- `player_stamina.h` (exact)
- `player_stamina_subscriber.h` (exact)
- `player_state.h` (exact)
- `player_stealth.h` (exact)
- `profile_slot.h` (exact)
- `pseudo_random.h` (exact)
- `recoil_calculator.h` (exact)
- `recoil_calculator_1.h` (variant-of-game_core-type)
- `respawn_point_core.h` (exact)
- `scheduler.h` (exact)
- `scheduler__callback_record.h` (nested-of-game_core-outer)
- `scheduler__identifier.h` (nested-of-game_core-outer)
- `scheduler__record.h` (nested-of-game_core-outer)
- `scheduler__scheduler_record.h` (nested-of-game_core-outer)
- `server_player_update.h` (exact)
- `skill_booster.h` (exact)
- `static_collision.h` (exact)
- `usable_object.h` (exact)
- `usable_object_user_data.h` (exact)
- `victory_item_core.h` (exact)
- `victory_item_core_cook.h` (exact)
- `victory_items_container_core.h` (exact)
- `victory_items_container_core_1.h` (variant-of-game_core-type)
- `weapon_ammo_info.h` (exact)
- `weapon_ammunition.h` (exact)
- `weapon_ammunition_cook.h` (exact)
- `weapon_animation_parameters.h` (exact)
- `weapon_animation_parameters_1.h` (variant-of-game_core-type)
- `weapon_core.h` (exact)
- `weapon_core_1.h` (variant-of-game_core-type)
- `weapon_core_2.h` (variant-of-game_core-type)
- `weapon_core_3.h` (variant-of-game_core-type)
- `weapon_core_aimed_fire_state.h` (exact)
- `weapon_core_aimed_fire_state_base.h` (exact)
- `weapon_core_aimed_state.h` (exact)
- `weapon_core_aimed_state_base.h` (exact)
- `weapon_core_animation_end_aware_state.h` (exact)
- `weapon_core_base_state.h` (exact)
- `weapon_core_base_state_1.h` (variant-of-game_core-type)
- `weapon_core_chamber_a_round_aimed_state.h` (exact)
- `weapon_core_chamber_a_round_aimed_state_base.h` (exact)
- `weapon_core_chamber_a_round_state.h` (exact)
- `weapon_core_chamber_a_round_state_base.h` (exact)
- `weapon_core_cook.h` (exact)
- `weapon_core_fire_state.h` (exact)
- `weapon_core_fire_state_base.h` (exact)
- `weapon_core_hide_state.h` (exact)
- `weapon_core_hide_state_1.h` (variant-of-game_core-type)
- `weapon_core_hide_state_base.h` (exact)
- `weapon_core_idle_state.h` (exact)
- `weapon_core_idle_state_base.h` (exact)
- `weapon_core_inactive_state.h` (exact)
- `weapon_core_inactive_state_cook.h` (exact)
- `weapon_core_reload_state.h` (exact)
- `weapon_core_reload_state_base.h` (exact)
- `weapon_core_shotgun_reload_base_substate.h` (exact)
- `weapon_core_shotgun_reload_finish_substate.h` (exact)
- `weapon_core_shotgun_reload_one_round_substate.h` (exact)
- `weapon_core_shotgun_reload_start_substate.h` (exact)
- `weapon_core_shotgun_reload_state.h` (exact)
- `weapon_core_shotgun_reload_state_cook.h` (exact)
- `weapon_core_show_state.h` (exact)
- `weapon_core_show_state_1.h` (variant-of-game_core-type)
- `weapon_core_show_state_base.h` (exact)
- `weapon_dispersion_calculator.h` (exact)
- `weapon_dispersion_params.h` (exact)
- `weapon_lexeme_pair.h` (exact)
- `weapon_recoil_calculator.h` (exact)
- `weapon_recoil_params.h` (exact)
- `weapon_state.h` (exact)
- `weapon_state_creation_params.h` (exact)
- `weapon_user_animations_container.h` (exact)
- `weapon_user_animations_container_cook.h` (exact)
- `weapon_user_animations_selector.h` (exact)
- `weapon_user_animations_selector_1.h` (variant-of-game_core-type)
- `weapon_user_animations_selector_2.h` (variant-of-game_core-type)
- `weapon_user_animations_selector_3.h` (variant-of-game_core-type)
- `weapon_user_animations_selector_4.h` (variant-of-game_core-type)
- `zone_group.h` (exact)
- `zone_group__zone_wrapper.h` (nested-of-game_core-outer)
- `enums/affect_event_type_enum.h` (exact)
- `enums/affects_applying_type_enum.h` (exact)
- `enums/ammo_type_enum.h` (exact)
- `enums/collision_geometry__collision_geometry_mode.h` (nested-of-game_core-outer)
- `enums/game_material_manager_cook__res_type.h` (nested-of-game_core-outer)
- `enums/game_team_id.h` (exact)
- `enums/hand_to_weapon_ik_processor__hands_enum.h` (nested-of-game_core-outer)
- `enums/hit_affects_type_enum.h` (exact)
- `enums/inventory_item__action_behaviour_type.h` (nested-of-game_core-outer)
- `enums/missile_weapon_core__activation_type.h` (nested-of-game_core-outer)
- `enums/move_direction_enum.h` (exact)
- `enums/profile_slot_enum.h` (exact)
- `enums/weapon_targets.h` (exact)
- `enums/weapon_user_state_enum.h` (exact)

## Remaining in queue (490) - to triage one by one

## Pass 2: definition-grep over sources/vostok (32 more removed)

- `affect_event_predicate.h` (ELSEWHERE:game_core)
- `affect_subscriber.h` (ELSEWHERE:game_core)
- `application.h` (ELSEWHERE:survarium-pc (survarium_pc_application.h))
- `bodypart_health_regen_scale_predicate.h` (ELSEWHERE:game_core)
- `booster_damage_protector.h` (ELSEWHERE:game_core)
- `breath_state_holding.h` (ELSEWHERE:game_core)
- `breath_state_normal.h` (ELSEWHERE:game_core)
- `breath_state_shortbreathing.h` (ELSEWHERE:game_core)
- `dump_npc_body_part_state_predicate.h` (ELSEWHERE:game_core)
- `dz_bone_data_contact_test_predicate.h` (ELSEWHERE:game_core)
- `enums/apply_hit_type.h` (ELSEWHERE:game_core)
- `enums/booby_trap_state.h` (ELSEWHERE:game_core)
- `enums/game_mode_type.h` (ELSEWHERE:game_core)
- `enums/jump_animation_parts.h` (ELSEWHERE:game_core)
- `find_body_part_by_name_predicate.h` (ELSEWHERE:game_core)
- `find_by_damage_type_predicate.h` (ELSEWHERE:game_core)
- `find_hit_parameters_by_type_predicate.h` (ELSEWHERE:game_core)
- `game_module_proxy.h` (ELSEWHERE:survarium module)
- `hit_receiver_info.h` (ELSEWHERE:game_core)
- `left_objects_predicate.h` (ELSEWHERE:game_core)
- `match_options.h` (ELSEWHERE:game_core)
- `objects_filter_predicate.h` (ELSEWHERE:game_core)
- `player_parameters_cooker_data.h` (ELSEWHERE:game_core)
- `protect_affect_predicate.h` (ELSEWHERE:game_core)
- `protect_damage_predicate.h` (ELSEWHERE:game_core)
- `redundant_bullet_predicate.h` (ELSEWHERE:game_core)
- `regenerate_body_parts_predicate.h` (ELSEWHERE:game_core)
- `relocate_item_descr.h` (ELSEWHERE:game_core)
- `reset_predicate.h` (ELSEWHERE:game_core)
- `stamina_depletion_predicate.h` (ELSEWHERE:game_core)
- `weapon_sound_events_handler_state_cook_survarium__weapon_sound_events_handler_state_survarium__weapon_core_shotgun_reload_one_round_substate.h` (ELSEWHERE:game_core)
- `weapon_sound_events_handler_state_cook_survarium__weapon_sound_events_handler_state_survarium__weapon_core_shotgun_reload_one_round_substate_1.h` (ELSEWHERE:game_core)

## CHECK individually (kept in queue; def-grep hit a non-game_core module, likely name collision)

- `flash_renderer.h` - RESOLVED (batch 4): name collision confirmed, NOT coverage.
  survarium::flash_renderer (ctor takes scaleform_render_command_queue*/ID3D11*,
  members m_output_*+HAL+R2D, impl TU vostok/scaleform/sources/renderer.cpp per
  the rich index) vs the legacy vostok::render::flash_renderer (default ctor,
  m_movies vector, no canonical dump exists for it). Reproduced in game.
- `render_visual.h`
- `scaleform_engine__xrSysAllocMalloc.h` - RESOLVED (batch 4): survarium::
  scaleform_engine is a NAMESPACE (initialize/destroy are YA-mangled free fns);
  xrSysAllocMalloc + both fns live in vostok/scaleform/sources/factory.cpp - a
  glue module absent from our tree. Reproduced as game/sources/scaleform_engine.h
  until that module is rebuilt.
- `enums/action_type.h`

## Pass 3: real-definition audit + template-instantiation noise (2026-06-12)

Audit: all pass-2 removals re-verified against `{`-bodied definitions - no
forward-declaration false positives. Cross-module name hits re-checked:
booby_trap[_set]_cook defs in game_core are TU-local anchor stand-ins (the
real cooks are game's), fingers_to_weapon_corrector__hand[s_enum] collide
with hand_to_weapon_ik_processor's nested types - all four KEPT in queue.

Removed 147 instantiation dumps of the engine USER_ALLOCATOR container
machinery (vostok/map.h, vector.h, std_allocator via std_containers.h -
game_memory.h already instantiates these into survarium::):

- `map_enum vostok__input__enum_keyboard,survarium__dik_to_swf_bind,stlp_std__less_enum vostok__input__enum_keyboard_ _.h`
- `map_unsigned int,survarium__base_point_stats,stlp_std__less_unsigned int_ _.h`
- `map_unsigned int,survarium__dictionary_item,stlp_std__less_unsigned int_ _.h`
- `map_unsigned int,survarium__respawn_point_core +,stlp_std__less_unsigned int_ _.h`
- `map_unsigned short,stlp_std__pair_unsigned short const ,survarium__material_pair const +_,stlp_std__less_unsigned short_ _.h`
- `map_unsigned short,survarium__game_material const +,stlp_std__less_unsigned short_ _.h`
- `map_unsigned short,survarium__map_unsigned short,survarium__material_pair const +,stlp_std__less_unsigned short_ _,stlp_std__less_unsigned s.h`
- `map_unsigned short,survarium__material_pair const +,stlp_std__less_unsigned short_ _.h`
- `map_vostok__fixed_string_16_,stlp_std__pair_vostok__fixed_string_16_ const ,survarium__hit_type_parameters_modifyer_,stlp_std__less_vostok__.h`
- `map_vostok__fixed_string_16_,survarium__body_part_parameters_modifyer,stlp_std__less_vostok__fixed_string_16_ _ _.h`
- `map_vostok__fixed_string_16_,survarium__hit_type_parameters_modifyer,stlp_std__less_vostok__fixed_string_16_ _ _.h`
- `map_vostok__fixed_string_260_,survarium__base_game_object +,stlp_std__less_vostok__fixed_string_260_ _ _.h`
- `std_allocator_enum survarium__game_action_id_.h`
- `std_allocator_enum survarium__game_action_id___rebind_enum survarium__game_action_id_.h`
- `std_allocator_enum survarium__profile_slot_enum_.h`
- `std_allocator_enum survarium__profile_slot_enum___rebind_enum survarium__profile_slot_enum_.h`
- `std_allocator_int_.h`
- `std_allocator_int___rebind_int_.h`
- `std_allocator_stlp_std__pair_enum vostok__input__enum_keyboard,survarium__dik_to_swf_bind_ _.h`
- `std_allocator_stlp_std__pair_enum vostok__input__enum_keyboard,survarium__dik_to_swf_bind_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_std_.h`
- `std_allocator_stlp_std__pair_unsigned int,survarium__base_point_stats_ _.h`
- `std_allocator_stlp_std__pair_unsigned int,survarium__base_point_stats_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned int c.h`
- `std_allocator_stlp_std__pair_unsigned int,survarium__dictionary_item_ _.h`
- `std_allocator_stlp_std__pair_unsigned int,survarium__dictionary_item_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned int co.h`
- `std_allocator_stlp_std__pair_unsigned int,survarium__respawn_point_core +_ _.h`
- `std_allocator_stlp_std__pair_unsigned int,survarium__respawn_point_core +_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned i.h`
- `std_allocator_stlp_std__pair_unsigned short,stlp_std__pair_unsigned short const ,survarium__material_pair const +_ _ _.h`
- `std_allocator_stlp_std__pair_unsigned short,stlp_std__pair_unsigned short const ,survarium__material_pair const +_ _ ___rebind_stlp_std__pri.h`
- `std_allocator_stlp_std__pair_unsigned short,survarium__game_material const +_ _.h`
- `std_allocator_stlp_std__pair_unsigned short,survarium__game_material const +_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigne.h`
- `std_allocator_stlp_std__pair_unsigned short,survarium__map_unsigned short,survarium__material_pair const +,stlp_std__less_unsigned short_ _ .h`
- `std_allocator_stlp_std__pair_unsigned short,survarium__map_unsigned short,survarium__material_pair const +,stlp_std__less_unsigned short_ _ _1.h`
- `std_allocator_stlp_std__pair_unsigned short,survarium__material_pair const +_ _.h`
- `std_allocator_stlp_std__pair_unsigned short,survarium__material_pair const +_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigne.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_16_,stlp_std__pair_vostok__fixed_string_16_ const ,survarium__hit_type_parameters_modifyer.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_16_,stlp_std__pair_vostok__fixed_string_16_ const ,survarium__hit_type_parameters_modifyer_1.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_16_,survarium__body_part_parameters_modifyer_ _.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_16_,survarium__body_part_parameters_modifyer_ ___rebind_stlp_std__priv___Rb_tree_node_stlp.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_16_,survarium__hit_type_parameters_modifyer_ _.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_16_,survarium__hit_type_parameters_modifyer_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_260_,survarium__base_game_object +_ _.h`
- `std_allocator_stlp_std__pair_vostok__fixed_string_260_,survarium__base_game_object +_ ___rebind_stlp_std__priv___Rb_tree_node_stlp_std__pair.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_enum vostok__input__enum_keyboard const ,survarium__dik_to_swf_bind_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned int const ,survarium__base_point_stats_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned int const ,survarium__dictionary_item_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned int const ,survarium__respawn_point_core +_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned short const ,stlp_std__pair_unsigned short const ,survarium__material_pa.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned short const ,survarium__game_material const +_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned short const ,survarium__map_unsigned short,survarium__material_pair cons.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_unsigned short const ,survarium__material_pair const +_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_vostok__fixed_string_16_ const ,stlp_std__pair_vostok__fixed_string_16_ const ,su.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_vostok__fixed_string_16_ const ,survarium__body_part_parameters_modifyer_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_vostok__fixed_string_16_ const ,survarium__hit_type_parameters_modifyer_ _ _.h`
- `std_allocator_stlp_std__priv___Rb_tree_node_stlp_std__pair_vostok__fixed_string_260_ const ,survarium__base_game_object +_ _ _.h`
- `std_allocator_survarium__anomaly_state +_.h`
- `std_allocator_survarium__anomaly_state +___rebind_survarium__anomaly_state +_.h`
- `std_allocator_survarium__anomaly_state +___rebind_void +_.h`
- `std_allocator_survarium__artefact_container_core +_.h`
- `std_allocator_survarium__artefact_container_core +___rebind_survarium__artefact_container_core +_.h`
- `std_allocator_survarium__artefact_container_core +___rebind_void +_.h`
- `std_allocator_survarium__base_project__resolve_link_object_.h`
- `std_allocator_survarium__base_project__resolve_link_object___rebind_survarium__base_project__resolve_link_object_.h`
- `std_allocator_survarium__collision_geometry +_.h`
- `std_allocator_survarium__collision_geometry +___rebind_survarium__collision_geometry +_.h`
- `std_allocator_survarium__collision_geometry +___rebind_void +_.h`
- `std_allocator_survarium__game_material_manager_cook__query_ext_data_.h`
- `std_allocator_survarium__game_material_manager_cook__query_ext_data___rebind_survarium__game_material_manager_cook__query_ext_data_.h`
- `std_allocator_survarium__game_object_ +_.h`
- `std_allocator_survarium__game_object_ +___rebind_survarium__game_object_ +_.h`
- `std_allocator_survarium__game_object_ +___rebind_void +_.h`
- `std_allocator_survarium__game_world__bullet_tracer_.h`
- `std_allocator_survarium__game_world__bullet_tracer___rebind_survarium__game_world__bullet_tracer_.h`
- `std_allocator_survarium__generic_anomaly_core +_.h`
- `std_allocator_survarium__generic_anomaly_core +___rebind_survarium__generic_anomaly_core +_.h`
- `std_allocator_survarium__generic_anomaly_core +___rebind_void +_.h`
- `std_allocator_survarium__hit_receiver_info_.h`
- `std_allocator_survarium__hit_receiver_info___rebind_survarium__hit_receiver_info_.h`
- `std_allocator_survarium__player_actions_subscriber +_.h`
- `std_allocator_survarium__player_actions_subscriber +___rebind_survarium__player_actions_subscriber +_.h`
- `std_allocator_survarium__player_actions_subscriber +___rebind_void +_.h`
- `std_allocator_survarium__relocate_item_descr_.h`
- `std_allocator_survarium__relocate_item_descr___rebind_survarium__relocate_item_descr_.h`
- `std_allocator_survarium__victory_items_container +_.h`
- `std_allocator_survarium__victory_items_container +___rebind_survarium__victory_items_container +_.h`
- `std_allocator_survarium__victory_items_container +___rebind_void +_.h`
- `std_allocator_survarium__zone_group +_.h`
- `std_allocator_survarium__zone_group +___rebind_survarium__zone_group +_.h`
- `std_allocator_survarium__zone_group +___rebind_void +_.h`
- `std_allocator_survarium__zone_group__zone_wrapper_.h`
- `std_allocator_survarium__zone_group__zone_wrapper___rebind_survarium__zone_group__zone_wrapper_.h`
- `std_allocator_unsigned int_.h`
- `std_allocator_unsigned int___rebind_unsigned int_.h`
- `std_allocator_void +_.h`
- `std_allocator_void const +_.h`
- `std_allocator_vostok__fixed_string_16_ _.h`
- `std_allocator_vostok__fixed_string_16_ ___rebind_vostok__fixed_string_16_ _.h`
- `std_allocator_vostok__math__float3_.h`
- `std_allocator_vostok__math__float3___rebind_vostok__math__float3_.h`
- `std_allocator_vostok__physics__base_physics_object +_.h`
- `std_allocator_vostok__physics__base_physics_object +___rebind_void +_.h`
- `std_allocator_vostok__physics__base_physics_object +___rebind_vostok__physics__base_physics_object +_.h`
- `std_allocator_vostok__render__light_props_.h`
- `std_allocator_vostok__render__light_props___rebind_vostok__render__light_props_.h`
- `std_allocator_vostok__resources__creation_request_.h`
- `std_allocator_vostok__resources__creation_request___rebind_vostok__resources__creation_request_.h`
- `std_allocator_vostok__resources__request_.h`
- `std_allocator_vostok__resources__request___rebind_vostok__resources__request_.h`
- `std_allocator_vostok__resources__resource_ptr_survarium__damage_zone,vostok__resources__unmanaged_intrusive_base_ _.h`
- `std_allocator_vostok__resources__resource_ptr_survarium__damage_zone,vostok__resources__unmanaged_intrusive_base_ ___rebind_vostok__resource.h`
- `std_allocator_vostok__resources__resource_ptr_survarium__ladder,vostok__resources__unmanaged_intrusive_base_ _.h`
- `std_allocator_vostok__resources__resource_ptr_survarium__ladder,vostok__resources__unmanaged_intrusive_base_ ___rebind_vostok__resources__re.h`
- `std_allocator_vostok__resources__resource_ptr_vostok__resources__unmanaged_resource,vostok__resources__unmanaged_intrusive_base_ _.h`
- `std_allocator_vostok__resources__resource_ptr_vostok__resources__unmanaged_resource,vostok__resources__unmanaged_intrusive_base_ ___rebind_v.h`
- `std_allocator_vostok__variant_32_ _.h`
- `std_allocator_vostok__variant_32_ ___rebind_vostok__variant_32_ _.h`
- `std_allocator_vostok__variant_32_ const +_.h`
- `std_allocator_vostok__variant_32_ const +___rebind_void const +_.h`
- `std_allocator_vostok__variant_32_ const +___rebind_vostok__variant_32_ const +_.h`
- `vector_enum survarium__game_action_id_.h`
- `vector_enum survarium__profile_slot_enum_.h`
- `vector_int_.h`
- `vector_survarium__anomaly_state +_.h`
- `vector_survarium__artefact_container_core +_.h`
- `vector_survarium__base_project__resolve_link_object_.h`
- `vector_survarium__collision_geometry +_.h`
- `vector_survarium__game_material_manager_cook__query_ext_data_.h`
- `vector_survarium__game_object_ +_.h`
- `vector_survarium__game_world__bullet_tracer_.h`
- `vector_survarium__generic_anomaly_core +_.h`
- `vector_survarium__hit_receiver_info_.h`
- `vector_survarium__player_actions_subscriber +_.h`
- `vector_survarium__relocate_item_descr_.h`
- `vector_survarium__victory_items_container +_.h`
- `vector_survarium__zone_group +_.h`
- `vector_survarium__zone_group__zone_wrapper_.h`
- `vector_unsigned int_.h`
- `vector_vostok__fixed_string_16_ _.h`
- `vector_vostok__math__float3_.h`
- `vector_vostok__physics__base_physics_object +_.h`
- `vector_vostok__render__light_props_.h`
- `vector_vostok__resources__creation_request_.h`
- `vector_vostok__resources__request_.h`
- `vector_vostok__resources__resource_ptr_survarium__damage_zone,vostok__resources__unmanaged_intrusive_base_ _.h`
- `vector_vostok__resources__resource_ptr_survarium__ladder,vostok__resources__unmanaged_intrusive_base_ _.h`
- `vector_vostok__resources__resource_ptr_vostok__resources__unmanaged_resource,vostok__resources__unmanaged_intrusive_base_ _.h`
- `vector_vostok__variant_32_ _.h`
- `vector_vostok__variant_32_ const +_.h`

Restored 2 files pass 2 removed by matching the template ARGUMENT type
(...shotgun_reload_one_round_substate) instead of the template: the
weapon_sound_events_handler_state_cook family is GAME's (its compilands
are under target game/sources/):

- `weapon_sound_events_handler_state_cook_survarium__weapon_sound_events_handler_state_survarium__weapon_core_shotgun_reload_one_round_substate_1.h`
- `weapon_sound_events_handler_state_cook_survarium__weapon_sound_events_handler_state_survarium__weapon_core_shotgun_reload_one_round_substate.h`

## Pass 4: weapon_core_state_cook_template instantiations (2026-06-12)

Removed 23 monomorphised dumps of `weapon_core_state_cook_template< T >` -
the primary template is game_core's (`game_core/weapon_core_state_cook_template.h`,
already matched); the instantiations materialize from game_core's own use.
(Passes 1-2 missed them because the filename suffix is the template ARGUMENT,
not a `_N` variant.)

## TU-local helpers - owner mapping (was batch 2)

These queue headers are TU-local types (no own compiland); they are reproduced
INSIDE their owner `.cpp` when that file's batch runs, not as standalone headers:

- `console_command_bind.h` -> key_binder.cpp (batch 5)
- `dik_to_swf_bind.h` -> swf_input_translator.cpp (batch 4) - CORRECTED in
  batch 4: must live in swf_input_translator.h (the map member instantiates
  pair< enum_keyboard, dik_to_swf_bind > at class completion - C2079 otherwise)
- `relocate_item_func.h` -> lobby_menu_ui.cpp (batch 5)
- `hit_object.h` -> human_npc.cpp (batch 6)
- `max_angular_velocity_command.h` -> game.cpp (batch 11)
- `ray_query_predicate.h` -> game.cpp / game_unused.cpp (legacy prior; batch 11)
- `get_first_npc_in_camera_direction_predicate.h` -> game_unused.cpp lineage (batch 11)
- owner UNKNOWN (no carcass/legacy signal - placed when a consumer TU is
  matched; stay in queue for batch 12): check_health_predicate,
  compare_body_parts_predicate, dump_player_body_part_state_predicate,
  erase_null_ptrs, erase_old_receivers, find_closest_collision_predicate,
  remove_left_receivers_predicate, remove_vertex_from_hit_parameters_predicate,
  test_objects_in_shape_predicate
