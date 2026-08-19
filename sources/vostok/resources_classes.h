////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_CLASSES_H_INCLUDED
#define VOSTOK_RESOURCES_CLASSES_H_INCLUDED

namespace vostok {
namespace resources {

enum class_id_enum
{
	unknown_data_class,
	fs_iterator_class,
	fs_iterator_recursive_class,
	raw_data_class,
	raw_data_class_no_reuse,// this class stands for managed resource that will not be associated with fat-it and reused therefore
	sub_fat_class,
	vfs_sub_fat_class,
	texture_wrapper_class,
	texture_class,
	texture_converter_class,
	texture_gpu_converter_class,                   // [+]
	texture_tga_to_argb_class,
	shader_binary_source_class,
	render_effect_class,

	tracer_model_instance_class,                   // [+]
	material_effects_instance_class,
	speedtree_class,
	speedtree_instance_class,
	speedtree_render_instance_class,

	static_model_instance_class,			// final class for user
	skeleton_model_instance_class,			// final class for user
	skeleton_combined_model_instance_class,        // [+]
	static_render_model_class,				// static render model class (shared between instances)
	skeleton_render_model_class,			// skeleton render model class (shared between instances)
	static_render_model_instance_class,		// static_render_model_instance
	skeleton_render_model_instance_class,	// skeleton_render_model_instance

	skeleton_combined_render_model_instance_class, // [+]

	skeleton_combined_model_class,                 // [+]
	user_mesh_class,						// user mesh instance (not shared).terrain cursor, and other user-defined geometry

	grass_render_model_class,				// grass render model class (shared)
	collision_geometry_class,

	collision_bt_shape_class_dynamic,	// bullet shape impl
	collision_bt_shape_class_static,	// bullet shape impl

	material_class,
	binary_config_class_impl,                      // [+]
	ltx_config_class,
	lua_config_class,

	test_resource_class,
	test_resource_class1,
	test_resource_class2,
	test_resource_class3,
	sound_scene_class,
	ogg_sound_wrapper_class,
	ogg_sound_class,
	sound_rms_class,
	sound_panning_lut_class,
	encoded_sound_interface_class,
	ogg_encoded_sound_interface_class,
	wav_encoded_sound_interface_class,
	ogg_file_contents_class,
	ogg_class,
	ogg_converter_class,
	ogg_options_converter_class,
	sound_spl_class,

	audibility_graph_class,                        // [+]
	sound_environment_class,                       // [+]
	skeleton_class,
	bi_spline_skeleton_animation_baked_class,      // [+]
	bi_spline_skeleton_animation_impl_class,       // [+]
	bi_spline_skeleton_animation_class,
	cubic_spline_skeleton_animation_class,
	animation_class,

	texture_options_lua_class,
	texture_options_binary_class,
	terrain_model_class,
	terrain_instance_class,
	terrain_mesh_class,
	dialog_resources_class,
	particle_world_class,
	particle_system_class,
	particle_system_binary_class,
	preview_particle_system_class,
	particle_system_instance_class,
	sound_class,
	ogg_raw_file,

	dialog_class,
	game_dialog_class,


	client_game_project_class,                     // [+]
	server_game_project_class,                     // [+]
	server_player_class,                           // [+]
	inventory_class,                               // [+]
	items_dictionary_class,                        // [+]

	brain_unit_class,
	human_npc_class,
	weapon_class,

	weapon_ammunition_class,                       // [+]
	missile_weapon_class,                          // [+]
	item_class,                                    // [+]
	booby_trap_set_class,                          // [+]
	booby_trap_class,                              // [+]
	rifle_scope_class,                             // [+]

	sound_player_class,                            // [+]

	unmanaged_allocation_class,
	behaviour_class,

	game_material_manager_class,                   // [+]
	damage_model_class,                            // [+]
	animation_space_graph_class,                   // [+]
	player_class,                                  // [+]
	player_skin_visual_class,                      // [+]
	player_parameters_class,                       // [+]
	victory_item_class,                            // [+]

	render_animated_model_instance_class,
	physics_animated_model_instance_class,
	game_animated_model_instance_class,
	editor_animated_model_instance_class,

	scene_class,
	scene_view_class,
	render_output_window_class,

	renderer_class,

	grass_world_class,                             // [+]
	portal_sector_structure_class,                 // [+]
	damage_zone_class,                             // [+]
	artefact_class,                                // [+]
	animation_analysis_result_class,               // [+]
	ladder_class,                                  // [+]
	victory_items_container_class,                 // [+]
	animation_container_class,                     // [+]
	empty_hands_class,                             // [+]

	//these class ids are exported to lua and therefore must have the same value during engine life
	single_sound_class	= 256,
	composite_sound_class,
	sound_collection_class,

	// Target weapon class IDs begin at 300.
	weapon_inactive_state_class = 300,             // [+]
	weapon_show_state_class,                       // [+]
	weapon_hide_state_class,                       // [+]
	weapon_idle_state_class,                       // [+]
	weapon_reload_state_class,                     // [+]
	weapon_chamber_a_round_state_class,            // [+]
	weapon_chamber_a_round_aimed_state_class,      // [+]
	weapon_fire_state_class,                       // [+]
	weapon_aimed_state_class,                      // [+]
	weapon_aimed_fire_state_class,                 // [+]
	weapon_shotgun_reload_state_class,             // [+]
	weapon_shotgun_reload_start_substate_class,    // [+]
	weapon_shotgun_reload_one_substate_class,      // [+]
	weapon_shotgun_reload_finish_substate_class,   // [+]
	double_barreled_weapon_show_state_class,       // [+]
	double_barreled_weapon_hide_state_class,       // [+]
	double_barreled_weapon_idle_state_class,       // [+]
	double_barreled_weapon_reload_state_class,     // [+]
	double_barreled_weapon_fire_state_class,       // [+]
	double_barreled_weapon_aimed_state_class,      // [+]
	double_barreled_weapon_aimed_fire_state_class, // [+]
	pistol_weapon_show_state_class,                // [+]
	pistol_weapon_hide_state_class,                // [+]
	pistol_weapon_idle_state_class,                // [+]
	pistol_weapon_reload_state_class,              // [+]
	pistol_weapon_fire_state_class,                // [+]
	pistol_weapon_aimed_state_class,               // [+]
	pistol_weapon_aimed_fire_state_class,          // [+]


	//these class ids are exported to lua and therefore must have the same value during engine life
	single_animation_class = 512,
	composite_animation_class,
	animation_collection_class,

	flash_movie_class,                             // [+]
	game_world_class,                              // [+]
	last_resource_class,

	// target aliases these onto earlier ids (the PDB records 0x3/0x3/0x22/0x23,
	// not the contiguous 0x206..0x209 our auto-increment would assign)
	platformed_raw_data_class	= raw_data_class,			// 0x003
	converted_model_class		= raw_data_class,			// 0x003
	binary_config_class			= binary_config_class_impl,	// 0x022
	inherits_binary_config_class	= ltx_config_class,		// 0x023

	// sushi@TODO: [values] divergence - these deleted/renamed classes are absent
	// from the shipped target's class_id_enum, but our tree still has resource cooks
	// that reference them (config_class -> configs_binary_config_cook.cpp; spl_class ->
	// spl_cook.cpp; sound_stream_class -> sound_*_cook*.cpp; animation_controller_class ->
	// rtp/editor). Removing them
	// to match the target requires parking/removing those cooks (several in skipped
	// sound/editor modules) - out of scope for the enum-only pass. Kept at the end so
	// they don't shift the matched ids above. Parked. Anchored at 0x20a (past the
	// matched range) so aliasing the four entries above onto earlier ids does not
	// renumber the block down into a collision with the real class ids.
	config_class	= 0x20a,                       // [-]
	spl_class,                                     // [-]
	animation_controller_class,                    // [-]
	sound_stream_class,                            // [-]
	game_project_class,                            // [-]
	game_cell_class = 0x210,                       // [-]
	game_object_class,                             // [-]
	game_object_scene_class,                       // [-]
	sound_player_logic_class,                      // [-]
	sound_player_editor_class,                     // [-]

	// The retail simple-project cook registers the client-project id (0x4d).
	game_project_simple_class = client_game_project_class,
};


} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_RESOURCES_CLASSES_H_INCLUDED
