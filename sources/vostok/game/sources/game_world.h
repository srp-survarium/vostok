// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_WORLD_H_INCLUDED
#define GAME_WORLD_H_INCLUDED

#include <boost/bind.hpp>
#include <vostok/ai/engine.h>
#include <vostok/ai/world.h>
#include <vostok/ai_navigation/engine.h>
#include <vostok/configs_binary_config.h>
#include <vostok/game_core/bullet_manager_engine.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/input/handler.h>
#include <vostok/render/culling/portal_sector_structure.h>
#include <vostok/render/engine/sources/tracer_model_instance.h>
#include <vostok/render/facade/sources/scene_renderer.h>
#include <vostok/resources_unmanaged_resource.h>
#include <vostok/resources_queries_result.h>
#include <vostok/type_variant.h>	// variant< 32 > (load takes user datas)

#include "base_game_scene.h"
#include "ai_collision_object.h"
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
class game_world : public base_game_scene , private ai::engine , private ai::navigation::engine , private bullet_manager_engine , public resources::unmanaged_resource , public input::handler {
public:
	typedef base_game_scene super;

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
	inline	ai::world&							get_ai_world					( ) const { return *m_ai_world; }
	inline	ai::navigation::world&				get_ai_navigation_world			( ) const { return *m_ai_navigation_world; }

	inline	bool								is_loading						( ) const { return m_is_loading; }
	// sushi@TODO: Loading-flag model; recover the consumer and whether unloading adds another condition.
	inline	bool								is_loading_or_unloading			( ) const { return m_is_loading; }

	// sushi@TODO: Selected-member getter model; locate the original named consumer.
	inline	human_npc_ptr const&				selected_npc					( ) const { return m_selected_npc; }

	inline	input_mode_type_enum				get_current_input_mode			( ) const { return m_input_mode; }
	inline	free_fly_camera*					get_free_fly_camera				( ) const { return m_free_fly_camera; }
			void								set_local_player_camera			( player_input_handler* camera );

	virtual	u32									get_current_time_in_ms			( ) const override;
	inline	const simple_game_project_ptr		get_project						( ) const { return m_game_project; }

	virtual	bullet_manager&						get_bullet_manager				( ) const override
	{
		return *m_bullet_manager;
	}

	inline	game_material_manager const&		get_game_material_manager		( ) const { return *m_game_material_manager; }
	inline	step_manager const&					get_step_manager				( ) const { return *m_step_manager; }

	// sushi@TODO: Legacy selection model; the physics picker and original invocation remain open.
	inline	void								check_selected_npc				( )
	{
		m_active_npc_set = !m_active_npc_set;
		if ( m_active_npc_set )
			m_selected_npc = find_npc_in_camera_direction( );
		else
			m_selected_npc = NULL;
		if ( m_selected_npc == NULL )
			m_active_npc_set = false;
	}
	// sushi@TODO: Legacy degree/yaw model; verify the retail consumer and rotation composition.
	inline	void								rotate_selected_npc				( const float y_angle )
	{
		if ( m_selected_npc )
		{
			float const y_angle_rad = math::deg2rad( y_angle );
			float4x4 const& rotation = math::create_rotation( m_selected_npc->get_rotation_angles( ) );
			float4x4 const& new_rotation = math::create_rotation( float3( 0.f, y_angle_rad, 0.f ) );
			m_selected_npc->set_rotation( rotation * new_rotation );
		}
	}
	// sushi@TODO: Relative-translation model; verify whether the original argument was a delta.
	inline	void								move_selected_npc				( float3 const& offset )
	{
		if ( m_selected_npc )
		{
			float4x4 transform = m_selected_npc->get_transform( );
			transform.c.xyz( ) += offset;
			m_selected_npc->set_transform( transform );
		}
	}
	// sushi@TODO: Legacy removal model; verify original notification and selection ownership.
	inline	void								delete_selected_npc				( )
	{
		if ( m_selected_npc )
		{
			m_npcs.erase( m_selected_npc );
			kill_npc( m_selected_npc );
			m_selected_npc = NULL;
			m_active_npc_set = false;
		}
	}

	// sushi@TODO: Legacy test-query model; recover the retail path, parameters and invocation.
	inline	void								assign_behaviour				( )
	{
		if ( m_selected_npc )
		{
			ai::behaviour_cook_params behaviour_params;
			resources::user_data_variant new_params;
			new_params.set( behaviour_params );
			resources::query_resource( "test", resources::behaviour_class,
				boost::bind( &game_world::on_behaviour_created, this, _1 ),
				g_allocator, &new_params, NULL );
		}
	}

			void								on_npc_attributes_received		( configs::binary_config_value const& attributes_config, human_npc_ptr owner );

	// sushi@TODO: Scene-forwarding model; verify the original caller and scene-readiness policy.
	inline	void								test_action_portal_system		( ) { scene_renderer( ).test_action_portal_system( render_scene( ) ); }

			void								on_player_killed				( player_ptr& player, const u8 arg_1, const u8 arg_2 );

	// sushi@TODO: Resolve hit-info player lookup and local-apply versus network-send ownership.
	inline	void								hit_player						( hit_info const& arg_0 ) { /* no source */ }

			void								put_victory_item				( u8 item_id, float4x4 const& transform );
	// sushi@TODO: Indexed take model; verify the original caller and outer insertion guard.
	inline	void								take_victory_item				( u8 item_id ) { m_victory_items[item_id]->take( ); }

	inline	vectora< victory_item_ptr >&		get_victory_items				( ) { return m_victory_items; }

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

	// sushi@TODO: Verify the original constructor's physics-initialization wrapper boundary.
	inline	void								initialize_physics				( ) { init_physics( ); }
			void								initialize_ai					( );
			void								initialize_ai_navigation		( );

			void								switch_to_player_camera			( const bool first_person_view );
			void								switch_to_free_fly_camera		( );

	inline	void								get_frustum_objects_callback	(
													boost::function< void( ai::game_object const& ) > const*	update_callback,
													collision::object const&	frustum_object
												)
	{
		// sushi@TODO: Legacy collision-to-AI forwarding model; the original query binding remains open.
		ai_collision_object const& ai_object = static_cast_checked< ai_collision_object const& >( frustum_object );
		( *update_callback )( ai_object.get_game_object( ) );
	}

	// sushi@TODO: Recover physics-result NPC identity, occlusion/order, masks and ray extent.
	inline	human_npc*							find_npc_in_camera_direction	( ) const { /* no source */ return NULL; }

			void								on_behaviour_created			( resources::queries_result& data );

			void								finish_npc_creation				( human_npc_ptr& new_npc, human_npc::npc_game_attributes& attributes );

			void								query_npc_dictionary			( );

	// sushi@TODO: Legacy dictionary callback model; retail query_npc_dictionary emits no query.
	inline	void								on_npc_dictionary_created		( resources::queries_result& data )
	{
		if ( !data.is_successful( ) )
		{
			R_ASSERT( data.is_successful( ), "couldn't retrieve npc dictionary options" );
			return;
		}
		configs::binary_config_ptr config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
		configs::binary_config_value const& dictionary = config->get_root( );
		m_ai_world->fill_objects_dictionary( dictionary );
		m_is_dictionary_created = true;
	}

			void								tick_npcs						( const u32 current_frame_id, const bool is_game_paused );

			void								update_npc_stats				( );

	virtual	void								set_navmesh_info				( pcstr __formal ) const override
	{
	}

	static	void								kill_npc						( human_npc_ptr& condemned );

			void								add_enemy_position_for_team		( pcstr team_name );
			void								clear_enemies_positions_for_team( pcstr team_name );

	// sushi@TODO: Recover spawn/enemy debug primitives, colors, guards and the original draw site.
	inline	void								draw_respawn_debug				( ) { /* no source */ }

			void								clear_player_spawn_info			( );

public:
	/* 0x0000 */	/* base_game_scene */
	/* 0x00bc */	/* ai::engine */
	/* 0x00c0 */	/* ai::navigation::engine */
	/* 0x00c4 */	/* bullet_manager_engine */
	/* 0x00c8 */	/* resources::unmanaged_resource */
	/* 0x01d0 */	/* input::handler */
	/* 0x01d4 */	game_world_ui							game_ui;
private:
	typedef vostok::intrusive_list< human_npc,
		human_npc_ptr,
		&human_npc::next_npc,
		vostok::threading::single_threading_policy,
		vostok::size_policy,
		vostok::no_debug_policy > npcs_type;
	typedef vector< float3 > enemies_type;

	/* 0x0214 */	enemies_type							m_enemies_for_team_1;
	/* 0x0220 */	enemies_type							m_enemies_for_team_2;
	/* 0x022c */	simple_game_project_ptr					m_game_project;
	/* 0x0230 */	render::culling::portal_sector_structure_ptr	m_portal_sector_structure;
	/* 0x0234 */	free_fly_camera*						m_free_fly_camera;
	/* 0x0238 */	player_input_handler*					m_player_camera;
	/* 0x023c */	bullet_manager*							m_bullet_manager;
	/* 0x0240 */	step_manager*							m_step_manager;
	/* 0x0244 */	game_material_manager_ptr				m_game_material_manager;
	/* 0x0248 */	ai::world*								m_ai_world;
	/* 0x024c */	ai::navigation::world*					m_ai_navigation_world;
	struct bullet_tracer {
		inline		bullet_tracer	( class bullet* arg_0, render::tracer_model_instance_ptr arg_1 ) :
			bullet( arg_0 ),
			tracer( arg_1 ) { }

	public:
		/* 0x0000 */	class bullet*							bullet;
		/* 0x0004 */	render::tracer_model_instance_ptr		tracer;
	}; // struct bullet_tracer
	typedef vector< bullet_tracer > bullet_tracers_type;

	/* 0x0250 */	bullet_tracers_type						m_bullet_tracers;
	enum { death_particles_count = 16 };
	/* 0x025c */	resources::unmanaged_resource_ptr		death_particles[ death_particles_count ];
	/* 0x029c */	u8										m_death_particles_it;
	/* 0x02a0 */	npcs_type								m_npcs;
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
