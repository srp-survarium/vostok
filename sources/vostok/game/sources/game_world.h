////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_WORLD_H_INCLUDED
#define GAME_WORLD_H_INCLUDED

#include <vostok/ai/engine.h>
#include <vostok/ai_navigation/engine.h>
#include <vostok/game_core/bullet_manager_engine.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/input/handler.h>
#include <vostok/render/culling/portal_sector_structure.h>
#include <vostok/render/facade/model.h>	// render_model_instance_ptr (parked tracer_model_instance)
#include <vostok/resources_unmanaged_resource.h>
#include <vostok/type_variant.h>	// variant< 32 > (load takes user datas)

#include "base_game_scene.h"
#include "game_world_ui.h"
#include "game_project.h"
#include "human_npc.h"
#include "input_mode_type_enum.h"	// the canonical dump inlines the enum here; it lives in its own header
#include "player.h"	// player_ptr
#include "victory_item.h"	// victory_item_ptr

namespace vostok {
namespace ai {
	struct world;
	namespace navigation {
		struct world;
	} // namespace navigation
} // namespace ai
namespace collision {
	class object;
} // namespace collision
namespace resources {
	class queries_result;
	class request;
} // namespace resources
namespace render {
	// vostok::render-pool type parked here (the server_connection_info
	// pattern): bullet_tracer's inline dtor needs it COMPLETE, and the render
	// carcass rebuild has not emitted it yet - identical blobs merge clean.
	// Canonical: binaries/structure/target/headers/vostok/render/tracer_model_instance.h
	struct tracer_model_instance : public resources::unmanaged_resource {
		inline	void	set_color				( math::color const& arg_0 ) { /* no source */ }

		inline			tracer_model_instance	( ) { /* no source */ }
		virtual			~tracer_model_instance	( ) { /* no source */ }

	public:
		/* 0x0000 */	/* resources::unmanaged_resource */
		/* 0x0108 */	float4x4						m_transform;
		/* 0x0148 */	render_model_instance_ptr		m_model;
		/* 0x014c */	math::color						m_color;
	}; // struct tracer_model_instance

	STATIC_SIZE_ASSERT(tracer_model_instance, 0x150);

	typedef resources::resource_ptr<
		tracer_model_instance,
		resources::unmanaged_intrusive_base
	> tracer_model_instance_ptr;
} // namespace render
} // namespace vostok

namespace survarium {

class bullet;
class bullet_manager;
class damage_model_stats;
class free_fly_camera;
class game;
struct hit_info;
class npc_stats;
class player_input_handler;
class step_manager;

typedef resources::resource_ptr<
	simple_game_project,
	resources::unmanaged_intrusive_base
> simple_game_project_ptr;

typedef resources::resource_ptr<
	game_material_manager,
	resources::unmanaged_intrusive_base
> game_material_manager_ptr;

// void* game_world::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0x8f7c0]: <0x8f7b0>|0x000|      :'27'	{

// the canonical game_world_1.h variant is byte-identical - no union needed
class game_world : public base_game_scene , public ai::engine , public ai::navigation::engine , public bullet_manager_engine , public resources::unmanaged_resource , public input::handler {
	// the network client's packet handlers forward HUD updates straight into the
	// private game_ui (m_game.get_game_world().game_ui.set_*()); the original reaches
	// it directly. Codegen-neutral - friendship is not recorded in the PDB.
	friend class network_client;
	// base_network_client::attach_to_player / detach_from_player reach the private
	// game_ui the same way (on_attached_to_player / show_ammo_indicator / show_quick_slots);
	// the original reaches it directly. Codegen-neutral - friendship is not in the PDB.
	friend class base_network_client;
	// player::detach_controller calls the private switch_to_free_fly_camera() directly
	// (the original reaches it directly). Codegen-neutral - friendship is not in the PDB.
	friend class player;
public:
	// canonical dump prints the nested type standalone (game_world__bullet_tracer.h)
	struct bullet_tracer {
		inline		bullet_tracer	( class bullet* arg_0, render::tracer_model_instance_ptr arg_1 ) :
			bullet( arg_0 ),
			tracer( arg_1 ) { /* no source */ }
		inline		~bullet_tracer	( ) { /* no source */ }

	public:
		/* 0x0000 */	class bullet*							bullet;
		/* 0x0004 */	render::tracer_model_instance_ptr		tracer;
	}; // struct bullet_tracer

	// member typedef (engine convention) - the member-pointer access check
	// happens in game_world's context (human_npc befriends game_world)
	typedef vostok::intrusive_list< human_npc,
		human_npc_ptr,
		&human_npc::next_npc,
		vostok::threading::single_threading_policy,
		vostok::size_policy,
		vostok::no_debug_policy > human_npc_list;

				explicit							game_world						( game& game );
	virtual											~game_world						( );

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

	virtual	s32									input_priority					( ) override
	{
		return 10;
	}

	// the nested bullet_tracer member `bullet` shadows type `bullet` in
	// game_world scope, so an unqualified `bullet*` here would not match the
	// base bullet_manager_engine pure virtual (C3668) - qualify it.
	virtual	bool								attach_tracer					( ::survarium::bullet* bullet ) override;
	virtual	bool								detach_tracer					( ::survarium::bullet* bullet ) override;
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
	virtual	u32									get_current_time_in_ms			( ) const override;

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
	inline	ai::world&							get_ai_world					( ) const { /* no source */ return *m_ai_world; }
	inline	ai::navigation::world&				get_ai_navigation_world			( ) const { /* no source */ return *m_ai_navigation_world; }

	inline	bool								is_loading						( ) const { /* no source */ return m_is_loading; }
	inline	bool								is_loading_or_unloading			( ) const { /* no source */ return m_is_loading; }

	inline	human_npc_ptr const&				selected_npc					( ) const { /* no source */ return m_selected_npc; }

	inline	input_mode_type_enum				get_current_input_mode			( ) const { /* no source */ return m_input_mode; }
	inline	free_fly_camera*					get_free_fly_camera				( ) const { /* no source */ return m_free_fly_camera; }
			void								set_local_player_camera			( player_input_handler* camera );

	inline	const simple_game_project_ptr		get_project						( ) const { /* no source */ return m_game_project; }

	virtual	bullet_manager&						get_bullet_manager				( ) const override
	{
		return *m_bullet_manager;
	}

	inline	game_material_manager const&		get_game_material_manager		( ) const { /* no source */ return *m_game_material_manager; }
	inline	step_manager const&					get_step_manager				( ) const { /* no source */ return *m_step_manager; }

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

	inline	vectora< victory_item_ptr >&		get_victory_items				( ) { /* no source */ return m_victory_items; }

protected:
			void								on_project_loaded				(
													resources::queries_result&		data,
													u32								results_offset,
													boost::function< void( resources::queries_result& ) > const&	callback
												);
			void								on_portal_system_loaded			( resources::queries_result& data );

			void								tick_bullet_manager_engine		( bool is_game_paused );

private:
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

	inline	human_npc*							find_npc_in_camera_direction	( ) const { /* no source */ return NULL; }

			void								on_behaviour_created			( resources::queries_result& data );

			void								finish_npc_creation				( human_npc_ptr& new_npc, human_npc::npc_game_attributes& attributes );

			void								query_npc_dictionary			( );

	inline	void								on_npc_dictionary_created		( resources::queries_result& arg_0 ) { /* no source */ }

			void								tick_npcs						( const u32 current_frame_id, const bool is_game_paused );

			void								update_npc_stats				( );

	virtual	void								set_navmesh_info				( pcstr __formal ) const override
	{
	}

			void								add_enemy_position_for_team		( pcstr team_name );
			void								clear_enemies_positions_for_team( pcstr team_name );

	inline	void								draw_respawn_debug				( ) { /* no source */ }

			void								clear_player_spawn_info			( );

	static	void								kill_npc						( human_npc_ptr& condemned );

public:
	/* 0x0000 */	/* base_game_scene */
	/* 0x00bc */	/* ai::engine */
	/* 0x00c0 */	/* ai::navigation::engine */
	/* 0x00c4 */	/* bullet_manager_engine */
	/* 0x00c8 */	/* resources::unmanaged_resource */
	/* 0x01d0 */	/* input::handler */
	/* 0x01d4 */	game_world_ui							game_ui;
private:
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
	/* 0x0250 */	vector< bullet_tracer >					m_bullet_tracers;
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
STATIC_SIZE_ASSERT(game_world::bullet_tracer, 0x8);

} // namespace survarium

#endif // #ifndef GAME_WORLD_H_INCLUDED
