////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_WORLD_H_INCLUDED
#define GAME_WORLD_H_INCLUDED

/* INCLUDES */
enum const survarium::input_mode_type_enum;
class vostok::resources::unmanaged_resource;
class survarium::base_game_scene;
class survarium::bullet_manager;
class survarium::bullet_manager_engine;
class survarium::damage_model_stats;
class survarium::free_fly_camera;
class survarium::game_world_ui;
class survarium::npc_stats;
class survarium::player_input_handler;
class survarium::step_manager;
class survarium::vector<survarium::game_world::bullet_tracer>;
class survarium::vector<vostok::math::float3>;
struct vostok::ai::engine;
struct vostok::ai::navigation::engine;
struct vostok::ai::navigation::world;
struct vostok::ai::world;
struct vostok::input::handler;
enum survarium::input_mode_type_enum;
class vostok::ai::collision_object const* const;
class vostok::input::enum_gamepad_action;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::gamepad_button;
class vostok::input::mouse_button;
class vostok::math::color;
class survarium::player;
typedef vostok::intrusive_list<survarium::human_npc,vostok::resources::resource_ptr<survarium::human_npc,vostok::resources::unmanaged_intrusive_base>,320,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>
	survarium::human_npc_list;
typedef vostok::resources::resource_ptr<survarium::game_material_manager,vostok::resources::unmanaged_intrusive_base>
	survarium::game_material_manager_ptr;
typedef vostok::resources::resource_ptr<survarium::human_npc,vostok::resources::unmanaged_intrusive_base>
	survarium::human_npc_ptr;
typedef vostok::resources::resource_ptr<survarium::simple_game_project,vostok::resources::unmanaged_intrusive_base>
	survarium::simple_game_project_ptr;
typedef vostok::resources::resource_ptr<vostok::render::culling::portal_sector_structure,vostok::resources::unmanaged_intrusive_base>
	vostok::render::culling::portal_sector_structure_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
typedef vostok::vectora<vostok::resources::resource_ptr<survarium::victory_item,vostok::resources::unmanaged_intrusive_base> >
	vostok::vectora<survarium::victory_item_ptr >;
class vostok::render::culling::portal_sector_structure;
class survarium::game_material_manager;
class survarium::human_npc;
class survarium::simple_game_project;
class survarium::victory_item;

/* FORWARD REFS */
class boost::function<void __cdecl(vostok::ai::game_object const &)>;
class boost::function<void __cdecl(vostok::resources::queries_result &)>;
class vostok::ai::npc;
class vostok::collision::object;
class vostok::configs::binary_config_value;
class vostok::input::world;
class vostok::math::aabb;
class vostok::math::cuboid;
class vostok::resources::queries_result;
class vostok::resources::request;
class vostok::sound::world;
class vostok::variant<32>;
class vostok::vectora<vostok::ai::game_object const *>;
class vostok::vectora<vostok::ai::weapon *>;
class survarium::bullet;
class survarium::game;
class survarium::hit_info;
class survarium::human_npc::npc_game_attributes;

namespace survarium {

enum survarium::input_mode_type_enum
{
	first_person_mode		= 0x0,
	free_fly_mode			= 0x1,
	third_person_mode		= 0x2,
};

class game_world : public base_game_scene , public ai::engine , public ai::navigation::engine , public bullet_manager_engine , public resources::unmanaged_resource , public input::handler {
public:
			explicit							game_world						( game& game );
	virtual										~game_world						( );

	virtual	void								clear_resources					( ) override;

	virtual	void								tick							( const u32 frame_delta_ms, const u32 current_time_in_ms, const bool is_game_paused ) override;

	virtual	void								on_after_tick					( ) override;
	virtual	void								on_activate						( ) override;
	virtual	void								on_deactivate					( ) override;

	virtual	void								show_ui							( bool b_show ) override;

	virtual	void								play_sound						( resources::unmanaged_resource_ptr const& resource, float3 const& position ) override;

	virtual	void								add_decal						(
													resources::unmanaged_resource_ptr const&	decal,
													u32					id,
													float				size,
													float				depth,
													float3 const&		position,
													float3 const&		direction,
													float3 const&		normal,
													bool				is_front_face
												) override;

	virtual	void								play_particle					(
													resources::unmanaged_resource_ptr const&	particle,
													float3 const&		position,
													float3 const&		direction,
													float3 const&		normal
												) override;

	virtual	void								remove_decal					( u32 id ) override;

	virtual	bool								on_keyboard_action				(
													input::world*					input_world,
													input::enum_keyboard			key,
													input::enum_keyboard_action		action
												) override;
	virtual	bool								on_gamepad_action				(
													input::world*					input_world,
													input::gamepad_button			button,
													input::enum_gamepad_action		action
												) override;
	virtual	bool								on_mouse_key_action				(
													input::world*					input_world,
													input::mouse_button				button,
													input::enum_mouse_key_action	action
												) override;
	virtual	bool								on_mouse_move					(
													input::world*		input_world,
													s32					x,
													s32					y,
													s32					z
												) override;

	virtual	s32									input_priority					( ) override { /* no source */ }

	virtual	bool								attach_tracer					( bullet* bullet ) override;
	virtual	bool								detach_tracer					( bullet* bullet ) override;
	virtual	void								update_tracer					(
													const u16			tracer_idx,
													float3 const&		position,
													float3 const&		direction,
													const float			length
												) override;

	virtual	void								get_colliding_objects			( math::aabb const& query_aabb, vectora< ai::game_object const* >& results ) override;
	virtual	void								get_visible_objects				(
													math::cuboid const&		cuboid,
													boost::function< void( ai::game_object const& ) > const&	update_callback
												) override;

	virtual	bool								ray_query						(
													ai::collision_object const* const		object_to_pick,
													ai::collision_object const* const		object_to_ignore,
													float3 const&							origin,
													float3 const&							direction,
													const float								max_distance,
													const float								transparency_threshold,
													float&									visibility_value
												) const override;

	virtual	void								draw_frustum					(
													float				fov_in_radians,
													float				far_plane_distance,
													float				aspect_ratio,
													float3 const&		position,
													float3 const&		direction,
													math::color			color
												) const override;
	virtual	void								draw_ray						( float3 const& start_point, float3 const& end_point, bool sees_something ) const override;

	virtual	u32									get_node_by_name				( pcstr node_name ) const override;
	virtual	void								get_available_weapons			( ai::npc* owner, vectora< ai::weapon* >& list_to_be_filled ) const override;

			void								load							(
													pcstr						project_resource_name,
													resources::request*			requests_begin,
													resources::request*			requests_end,
													variant< 32 > const**		user_datas_begin,
													boost::function< void( resources::queries_result& ) > const&	callback
												);

			void								unload							( );

			bool								empty							( );

			void								switch_camera_mode				(
													const input_mode_type_enum		arg_0 /* input_mode_type_enum input_mode */
												);

			sound::world&						get_sound_world					( ) const;
	inline	ai::world&							get_ai_world					( ) const { /* no source */ }
	inline	ai::navigation::world&				get_ai_navigation_world			( ) const { /* no source */ }

	inline	bool								is_loading						( ) const { /* no source */ }
	inline	bool								is_loading_or_unloading			( ) const { /* no source */ }

	inline	human_npc_ptr const&				selected_npc					( ) const { /* no source */ }

	inline	input_mode_type_enum				get_current_input_mode			( ) const { /* no source */ }
	inline	free_fly_camera*					get_free_fly_camera				( ) const { /* no source */ }
			void								set_local_player_camera			( player_input_handler* camera );

	virtual	u32									get_current_time_in_ms			( ) const override;
	inline	const simple_game_project_ptr		get_project						( ) const { /* no source */ }
	virtual	bullet_manager&						get_bullet_manager				( ) const override { /* no source */ }
	inline	game_material_manager const&		get_game_material_manager		( ) const { /* no source */ }
	inline	step_manager const&					get_step_manager				( ) const { /* no source */ }

	inline	void								check_selected_npc				( ) { /* no source */ }
	inline	void								rotate_selected_npc				( const float arg_0 ) { /* no source */ }
	inline	void								move_selected_npc				( float3 const& arg_0 ) { /* no source */ }
	inline	void								delete_selected_npc				( ) { /* no source */ }

	inline	void								assign_behaviour				( ) { /* no source */ }

			void								on_npc_attributes_received		( configs::binary_config_value const& attributes_config, human_npc_ptr owner );

	inline	void								test_action_portal_system		( ) { /* no source */ }

			void								on_player_killed				( player_ptr& player, const u8 arg_1, const u8 arg_2 );

	inline	void								hit_player						( hit_info const& arg_0 ) { /* no source */ }

			void								put_victory_item				( u8 item_id, float4x4 const& transform );
	inline	void								take_victory_item				( u8 arg_0 ) { /* no source */ }

	inline	vectora< victory_item_ptr >&		get_victory_items				( ) { /* no source */ }

			void								on_project_loaded				(
													resources::queries_result&		data,
													u32								results_offset,
													boost::function< void( resources::queries_result& ) > const&	callback
												);
			void								on_portal_system_loaded			( resources::queries_result& data );

			void								tick_bullet_manager_engine		( bool is_game_paused );

			void								register_cooks					( );

	inline	void								initialize_physics				( ) { /* no source */ }
			void								initialize_ai					( );
			void								initialize_ai_navigation		( );

			void								switch_to_player_camera			( const bool first_person_view );
			void								switch_to_free_fly_camera		( );

	inline	void								get_frustum_objects_callback	(
													boost::function< void( ai::game_object const& ) > const*	arg_0,
													collision::object const&	arg_1
												) { /* no source */ }

	inline	human_npc*							find_npc_in_camera_direction	( ) const { /* no source */ }

			void								on_behaviour_created			( resources::queries_result& data );

			void								finish_npc_creation				( human_npc_ptr& new_npc, human_npc::npc_game_attributes& attributes );

			void								query_npc_dictionary			( );

	inline	void								on_npc_dictionary_created		( resources::queries_result& arg_0 ) { /* no source */ }

			void								tick_npcs						( const u32 current_frame_id, const bool is_game_paused );

			void								update_npc_stats				( );

	virtual	void								set_navmesh_info				( pcstr __formal ) const override { /* no source */ }

			void								add_enemy_position_for_team		( pcstr team_name );
			void								clear_enemies_positions_for_team( pcstr team_name );

	inline	void								draw_respawn_debug				( ) { /* no source */ }

			void								clear_player_spawn_info			( );

	static	void								kill_npc						( human_npc_ptr& condemned );

private:
	/* 0x0000 */	/* base_game_scene */
	/* 0x00bc */	/* ai::engine */
	/* 0x00c0 */	/* ai::navigation::engine */
	/* 0x00c4 */	/* bullet_manager_engine */
	/* 0x00c8 */	/* resources::unmanaged_resource */
	/* 0x01d0 */	/* input::handler */
	/* 0x01d4 */	game_world_ui							game_ui;
	/* 0x0214 */	vector< float3 >						m_enemies_for_team_1;
	/* 0x0220 */	vector< float3 >						m_enemies_for_team_2;
	/* 0x022c */	simple_game_project_ptr					m_game_project;
	/* 0x0230 */	render::culling::portal_sector_structure_ptr	m_portal_sector_structure;
	/* 0x0234 */	free_fly_camera*						m_free_fly_camera;
	/* 0x0238 */	player_input_handler*					m_player_camera;
	/* 0x023c */	bullet_manager*							m_bullet_manager;
	/* 0x0240 */	step_manager*							m_step_manager;
	/* 0x0244 */	game_material_manager_ptr				m_game_material_manager;
	/* 0x0248 */	ai::world*								m_ai_world;
	/* 0x024c */	ai::navigation::world*					m_ai_navigation_world;
	/* 0x0250 */	vector< game_world::bullet_tracer >		m_bullet_tracers;
	/* 0x025c */	resources::unmanaged_resource_ptr		death_particles[16];
	/* 0x029c */	u8										m_death_particles_it;
	/* 0x02a0 */	human_npc_list							m_npcs;
	/* 0x02b0 */	npc_stats*								m_active_npc_stats;
	/* 0x02b4 */	damage_model_stats*						m_damage_model_stats;
	/* 0x02b8 */	human_npc_ptr							m_selected_npc;
	/* 0x02bc */	input_mode_type_enum					m_input_mode;
	/* 0x02c0 */	bool									m_is_dictionary_created;
	/* 0x02c1 */	bool									m_active_npc_set;
	/* 0x02c2 */	bool									m_is_loading;
	/* 0x02c4 */	vectora< victory_item_ptr >				m_victory_items;
}; // class game_world

STATIC_SIZE_ASSERT(game_world, 0x2D8);

} // namespace survarium

#endif // #ifndef GAME_WORLD_H_INCLUDED
