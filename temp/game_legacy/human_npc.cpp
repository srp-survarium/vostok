////////////////////////////////////////////////////////////////////////////
//	Created		: 22.11.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "human_npc.h"
#include <vostok/ai/world.h>
#include <vostok/sound/world.h>
#include <vostok/sound/world_user.h>
#include <vostok/ai/navigation_environment.h>
#include <vostok/ai/sensed_sound_object.h>
#include <vostok/ai/sensed_hit_object.h>
#include <vostok/collision/geometry.h>
#include "collision_object_types.h"
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/ai/npc_statistics.h>
#include <vostok/ai/animation_item.h>
#include <vostok/ai/movement_target.h>
#include <vostok/animation/instant_interpolator.h>
#include "human_npc_animation_controller_planner.h"
#include "game.h"
#include "game_world.h"
#include <vostok/ai/animation_item.h>
#include <vostok/console_command.h>

static bool s_npc_debug_draw		= false;
static vostok::console_commands::cc_bool s_npc_debug_draw_command( "npc_debug_draw", s_npc_debug_draw, true, vostok::console_commands::command_type_engine_internal );

namespace survarium {

human_npc::human_npc		(
		ai::world& ai_world,
		sound::world& sound_world,
		sound::sound_scene_ptr const& sound_scene,
		collision::space_partitioning_tree& spatial_tree,
		render::scene_ptr const& scene,
		render::game::renderer& renderer,
		game_world& game_world
	) :
	game_object_			( game_world ),
	m_ai_world				( ai_world ),
	m_sound_world			( sound_world ),
	m_sound_scene			( sound_scene ),
	m_spatial_tree			( spatial_tree ),
	m_scene					( scene ),
	m_renderer				( renderer ),
	m_visibility_parameters	( 0.f ),
	m_sound_perceived		( false ),
	m_sound_produced		( false ),
	m_transform				( float4x4().identity() ),
	m_dbg_sound				( false ),
	m_current_target		( 0 ),
	m_current_weapon		( 0 ),
	m_is_patrolling			( false ),
	m_current_animation		( 0 ),
	m_last_animation_emitted( true ),
	m_is_current_animation_finished( true ),
	m_current_movement_target( 0 ),
	m_target_vertex			( 0 ),
	m_search_service		( 0 ),
	m_animation_space_graph	( 0 )
#ifdef MASTER_GOLD
	,m_navigation_path		( g_allocator )
#endif // #ifdef MASTER_GOLD
{	
}

human_npc::~human_npc		( )
{
#ifndef MASTER_GOLD
	DELETE					( m_target_vertex );
	DELETE					( m_search_service );
	DELETE					( m_animation_space_graph );
#endif // #ifndef MASTER_GOLD
}

void human_npc::clear_resources	( )
{
	m_sound_world.get_logic_world_user().unregister_receiver( m_sound_scene, *this );

	m_renderer.scene().remove_model( m_scene, m_model_instance->m_render_model->m_model );
	m_spatial_tree.erase		( m_collision_object );
	
	R_ASSERT					( m_collision_object );
	ai_collision_object::delete_ai_collision_object( g_allocator, m_collision_object );
	m_collision_object			= 0;
	
	m_ai_world.on_destruction_event( *this );
	m_ai_world.remove_brain_unit( m_brain_unit.c_ptr() );
}

void human_npc::set_model		( animated_model_instance_ptr const& model )
{
	R_ASSERT					( !m_model_instance );
	m_model_instance			= model;
	
	m_collision_object			= &*ai_collision_object::new_ai_geometry_object(
									g_allocator,
									this,
									collision_object_type_ai,
									float4x4().identity(),
									m_model_instance->m_damage_collision->get_geometry(),
									&m_visibility_parameters
								);
}

void human_npc::set_idle_animation			( animation::skeleton_animation_ptr const& idle_animation )
{
	m_idle_animation						= idle_animation;
}

void human_npc::set_walk_forward_animation	( animation::skeleton_animation_ptr const& fwd_walk_animation )
{
	m_walk_forward_animation				= fwd_walk_animation;
}

void human_npc::set_arc_left_animation		( animation::skeleton_animation_ptr const& arc_left_animation )
{
	m_walk_forward_arc_left_animation		= arc_left_animation;
}

void human_npc::set_arc_right_animation		( animation::skeleton_animation_ptr const& arc_right_animation )
{
	m_walk_forward_arc_right_animation		= arc_right_animation;
}

void human_npc::enable			( )
{
	R_ASSERT					( m_brain_unit );
	m_ai_world.add_brain_unit	( m_brain_unit.c_ptr() );
	
	m_sound_world.get_logic_world_user().register_receiver( m_sound_scene, *this );
	sound_receiver::set_position( get_position() );

	R_ASSERT					( m_model_instance );

	m_spatial_tree.insert		( m_collision_object, m_transform );
	m_renderer.scene().add_model( m_scene, m_model_instance->m_render_model->m_model, m_transform );
	m_model_instance->m_animation_player->set_object_transform( m_transform );

	setup_animations_controller	( );
}

ai::collision_object* human_npc::get_collision_object( ) const
{
	return m_collision_object;
}

void human_npc::draw		( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	//m_model_instance->m_damage_collision->draw_collision( scene, render.debug(), m_transform );

	m_renderer.debug().draw_aabb(
		m_scene,
		m_collision_object->get_geom_instance().get_aabb().center(),
		m_collision_object->get_geom_instance().get_aabb().extents(),
		math::color( 0, 255, 0 )
	);

	m_renderer.debug().draw_origin( m_scene, m_transform, 3.0f );

	float3 const& start_pos	= get_eyes_position();
	float3 const& end_pos	= start_pos + get_eyes_direction() * 2.f;
	m_renderer.debug().draw_arrow( m_scene, start_pos, end_pos, math::color( 255, 0, 0 ) );

	if ( m_sound_perceived )
	{
		render.debug().draw_line_ellipsoid( scene, math::create_translation( get_eyes_position() ), math::color( 255, 0, 0 ) );
		m_sound_perceived	= false;
	}
	if ( m_sound_produced )
	{	
		render.debug().draw_line_ellipsoid( scene, m_transform, math::color( 0, 255, 0 ) );
		m_sound_produced	= false;
	}

#ifndef MASTER_GOLD
	if ( m_target_vertex )
	{
		m_renderer.debug().draw_origin	( m_scene, math::create_rotation( m_target_vertex->rotation ) * math::create_translation( m_game_world.get_game().movement_target() + float3( 0.f, .5f, 0.f ) ), .5f );

		typedef vostok::ai::navigation::path_type	path_type;
		for ( path_type::const_iterator b = m_navigation_path.begin(), e = m_navigation_path.end(), i = b; i != e; ++i ) {
			m_renderer.debug().draw_aabb( m_scene, *i, float3( .1f, .1f, .1f ), vostok::math::color( 255, 0, 0 ) );
			if ( i != b )
				m_renderer.debug().draw_arrow( m_scene, *(i-1), *i, vostok::math::color( 0, 255, 0 ) );
		}
	}
#endif // #ifndef MASTER_GOLD
}

void human_npc::set_filter		( ai::ignorable_game_object const* begin, ai::ignorable_game_object const* end )
{
	m_ai_world.set_ignore_filter( m_brain_unit, begin, end );
}

void human_npc::clear_filter		( )
{
	m_ai_world.clear_ignore_filter	( m_brain_unit );
}

void human_npc::set_transform	( float4x4 const& transform )
{
	m_transform					= transform;
	LOG_INFO					( "Position after set_transform: [%f][%f][%f]", m_transform.c.x, m_transform.c.y, m_transform.c.z );

	m_renderer.scene().update_model( m_scene, m_model_instance->m_render_model->m_model, m_transform );
	m_model_instance->m_animation_player->set_object_transform( m_transform );
}

void human_npc::tick			( u32 const current_time_in_ms )
{
	m_spatial_tree.move			( m_collision_object, m_transform ); 
	sound_receiver::set_position( m_transform.c.xyz() );
	VOSTOK_UNREFERENCED_PARAMETER	( current_time_in_ms );

	if ( m_current_animation || m_current_movement_target )
		tick_animation_player	( m_ai_world.get_current_time_in_ms() );

	if ( m_current_animation && m_is_current_animation_finished )
		play_animation			( m_current_animation );

	if ( debug_draw_allowed() )
		draw					( m_renderer, m_scene );
}

void human_npc::render_model	( )
{
	animation::animation_player* animation_player	= m_model_instance->m_animation_player;
	animation::skeleton_ptr	skeleton	= m_model_instance->m_physics_model->m_skeleton;

	u32 const bone_matrices_count		= skeleton->get_non_root_bones_count();
	float4x4* const bone_matrices		= static_cast< float4x4* >( ALLOCA( bone_matrices_count * sizeof( float4x4 ) ) );
	animation_player->compute_bones_matrices( *skeleton, bone_matrices, bone_matrices + bone_matrices_count );

	m_renderer.scene().update_model		( m_scene, m_model_instance->m_render_model->m_model, m_transform );
	m_renderer.scene().update_skeleton	(
		m_model_instance->m_render_model->m_model,
		bone_matrices,
		bone_matrices_count
	);

	m_model_instance->m_render_model->m_bounding_collision->update	( bone_matrices, bone_matrices + bone_matrices_count );
	m_model_instance->m_physics_model->m_collision->update			( bone_matrices, bone_matrices + bone_matrices_count );
	m_model_instance->m_damage_collision->update					( bone_matrices, bone_matrices + bone_matrices_count );
}

void human_npc::add_weapon				( object_weapon* weapon )
{
	if ( !m_game_attributes.weapons.contains_object( weapon ) )
		m_game_attributes.weapons.push_back	( weapon );
}

void human_npc::remove_weapon			( object_weapon* weapon )
{
	m_game_attributes.weapons.erase		( weapon );
}

bool human_npc::is_playing_animation( ) const
{
	return m_current_animation	!= 0;
}

bool human_npc::is_moving			( ) const
{
	return m_current_movement_target != 0;
}

void human_npc::set_rotation			( float4x4 const& new_rotation )
{
	float4x4 const new_transform		= create_scale( m_transform.get_scale() ) *
										  new_rotation *
										  create_translation( m_transform.c.xyz() );

	set_transform						( new_transform );
}

void human_npc::move_to_position		( ai::movement_target const* const target )
{
	m_current_movement_target			= target;
	LOG_INFO							(
		"%s: trying to move to point %.2f %.2f %.2f",
		get_name(),
		m_current_movement_target->target_position.x,
		m_current_movement_target->target_position.y,
		m_current_movement_target->target_position.z

	);
	float4x4 const& transform			= m_model_instance->m_animation_player->get_object_transform();
#ifndef MASTER_GOLD
	m_model_instance->m_animation_player->reset( false );
#endif // #ifndef MASTER_GOLD
	m_model_instance->m_animation_player->set_object_transform( transform );
	setup_animations					( m_ai_world.get_current_time_in_ms() );
}

} // namespace survarium