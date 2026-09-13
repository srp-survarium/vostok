// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "single_position_animation_controller.h"
// Complete types are required by the by-value expression and resource_ptr teardown.
#include <vostok/animation/mixing_expression.h>
// animation_space_graph is required by the resource_ptr destructor instantiation.
#include "animation_space_graph.h"
// The controller owns its search service and target vertex.
#include "game_memory.h"
#include "animations_search_service.h"
#include "animation_space_vertex_id.h"
// The movement-completion callback requires the complete owner type.
#include "human_npc.h"
#include <vostok/ai_navigation/world.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/mixing_multiplication_lexeme.h>
#include <vostok/animation/mixing_weight_lexeme.h>
// Debug drawing uses the facade renderer and its math builders.
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/math_float4x4.h>

namespace survarium {

// The initializer list is the complete retail constructor body.

single_position_animation_controller::single_position_animation_controller(
	animation_space_graph_ptr const&	graph,
	ai::navigation::world const&		ai_navigation_world,
	human_npc&							owner
) :
	m_animation_space_graph( graph ),
	m_search_service( VOSTOK_NEW_IMPL( ::survarium::g_allocator, animations_search_service ) ),
	m_target_vertex( VOSTOK_NEW_IMPL( ::survarium::g_allocator, animation_space_vertex_id ) ),
	m_ai_navigation_world( ai_navigation_world ),
	m_owner( owner ),
	m_navigation_path( g_allocator ),
	m_next_key_point( u32( -1 ) )
{
}

// Member destruction order is supplied by the compiler after these owned deletes.

single_position_animation_controller::~single_position_animation_controller( )
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_search_service );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_target_vertex );
}

void single_position_animation_controller::initialize( )
{
	m_target_vertex->rotation										= math::quaternion( float3( 0.f, 0.f, 0.f ) );
	m_target_vertex->translation									= m_owner.get_position( float3( 0.f, 0.f, 0.f ) );
}

// Retail returns the empty expression directly.

animation::mixing::expression single_position_animation_controller::try_finalize( base_animation_controller& next_controller, mutable_buffer& buffer )
{
	return													animation::mixing::expression( );
}

void single_position_animation_controller::query_new_target_if_needed( )
{
	if ( m_next_key_point > m_navigation_path.size( ) - 1 ) {
		// Notify the owner before clearing both controller parameter sets.

		m_owner.on_movement_end( );
		m_current_parameters.reset( );
		m_target_parameters.reset( );
	}
}

#line 95
animation::mixing::expression single_position_animation_controller::selected_animations( mutable_buffer& buffer )
{
	if ( m_current_parameters != m_target_parameters )
		m_current_parameters = m_target_parameters;

	m_target_vertex->rotation = math::create_quaternion_from_direction_vector( m_current_parameters.eyes_direction );

	float3 const& movement_position = m_owner.get_position( float3( 0.f, 0.f, 0.f ) );
	if ( !m_target_vertex->translation.is_similar( movement_position ) )
		m_target_vertex->translation = movement_position;

	u32 const start_node_id = m_ai_navigation_world.get_node_id_at( movement_position );
	u32 const target_node_id = m_ai_navigation_world.get_node_id_at( m_target_vertex->translation );
	if ( start_node_id == u32( -1 ) || target_node_id == u32( -1 ) )
		m_next_key_point = u32( -1 );
	else
		m_next_key_point = m_ai_navigation_world.find_path(
			start_node_id,
			movement_position,
			target_node_id,
			m_target_vertex->translation,
			m_animation_space_graph->agent_radius( ),
			m_navigation_path
		) && !m_navigation_path.empty( ) ? 1 : u32( -1 );

	float3 target_position;
	if ( m_next_key_point == u32( -1 ) )
		target_position = movement_position;
	else {
		for ( u32 i = m_next_key_point; i < m_navigation_path.size( ); ++i ) {
			float const previous_to_current_length = ( m_navigation_path[i] - m_navigation_path[i - 1] ).length( );
			if (
				( ( movement_position - m_navigation_path[i - 1] ) | ( m_navigation_path[i] - m_navigation_path[i - 1] ) / previous_to_current_length ) < previous_to_current_length &&
				( movement_position - m_navigation_path[i] ).length( ) > 0.3f
			)
				break;

			++m_next_key_point;
		}

		if ( m_next_key_point >= m_navigation_path.size( ) ) {
			LOG_INFO( "path is over!" );
			m_next_key_point = u32( -1 );
			target_position = movement_position;
		}
		else
			target_position = m_navigation_path[m_next_key_point];
	}

	m_target_vertex->translation = target_position;

	animation_space_vertex_id start_vertex_id = {
		math::quaternion( m_owner.get_transform( ).get_angles_xyz( ) ),
		target_position
	};

	vector< u32 > path;
	if ( !m_search_service->search( m_animation_space_graph, &path, start_vertex_id, *m_target_vertex ) || path.empty( ) ) {
		while ( m_next_key_point < m_navigation_path.size( ) - 1 ) {
			++m_next_key_point;
			m_target_vertex->translation = m_navigation_path[m_next_key_point];
			if ( m_search_service->search( m_animation_space_graph, &path, start_vertex_id, *m_target_vertex ) && !path.empty( ) )
				break;
		}

		if ( m_next_key_point >= m_navigation_path.size( ) - 1 ) {
			m_next_key_point = u32( -1 );
			m_target_vertex->translation = movement_position;

			animation::mixing::animation_lexeme lexeme(
				animation::mixing::animation_lexeme_parameters(
					buffer,
					NULL,
					m_animation_space_graph->get_animations( )[0].animation,
					NULL,
					NULL
				).weight_interpolator( animation::linear_interpolator( 0.25f ) )
			);
			return animation::mixing::expression( lexeme );
		}
	}

	m_target_vertex->translation = movement_position;

	float const time_scale = m_animation_space_graph->edge( path[0] ).mixable_pair->first->length / m_animation_space_graph->edge( path[0] ).animation_length;

	animation::mixing::animation_lexeme right_animation(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_animation_space_graph->edge( path[0] ).mixable_pair->first->animation,
			NULL,
			NULL
		)
		.weight_interpolator( animation::linear_interpolator( 0.25f ) )
		.time_scale_interpolator( animation::linear_interpolator( 0.25f ) )
		.time_scale( time_scale )
	);

	animation::mixing::weight_lexeme left_weight(
		buffer,
		m_animation_space_graph->edge( path[0] ).first_animation_weight,
		animation::linear_interpolator( 0.25f )
	);

	animation::mixing::animation_lexeme left_animation(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_animation_space_graph->edge( path[0] ).mixable_pair->second->animation,
			&right_animation,
			NULL
		)
		.time_scale( time_scale )
	);

	return animation::mixing::expression( left_animation*( 1.f - left_weight ) + right_animation*left_weight );
}

// The checked downcast disappears in the retail build.

void single_position_animation_controller::set_target( animation_controller_parameters const& target )
{
	m_target_parameters												= static_cast_checked< movement_animation_controller_parameters const& >( target );
}

void single_position_animation_controller::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	if ( m_target_vertex ) {
		render.debug( ).draw_origin(
			scene,
			math::mul4x3(
				math::create_matrix(
					math::quaternion( ),
					float3( m_current_parameters.position.x, m_current_parameters.position.y + 0.5f, m_current_parameters.position.z )
				),
				math::create_translation(
					float3( m_current_parameters.position.x, m_current_parameters.position.y + 0.5f, m_current_parameters.position.z )
				)
			),
			0.5f,
			true
		);

		for ( float3 const* e = m_navigation_path.begin( ); e != m_navigation_path.end( ); ++e ) {
			render.debug( ).draw_cube( scene, math::create_translation( *e ), float3( 0.1f, 0.1f, 0.1f ), math::color( 0xff0000ff ), true );
			if ( e != m_navigation_path.begin( ) )
				render.debug( ).draw_arrow( scene, *( e - 1 ), *e, math::color( 0xff00ff00 ), true );
		}
	}
}

} // namespace survarium
