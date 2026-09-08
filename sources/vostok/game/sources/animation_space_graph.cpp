// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
// VOSTOK_ANIMATION_API used unqualified by bone_animation.h (pulled in below)
#include <vostok/animation/api.h>
#include "animation_space_graph.h"
#include "animation_space_vertex.h"
#include "animation_space_vertex_id.h"
#include <vostok/animation/cubic_spline_skeleton_animation.h>
#include <vostok/animation/animation_player.h>
#include <vostok/animation/instant_interpolator.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/mixing_multiplication_lexeme.h>
#include <vostok/animation/mixing_weight_lexeme.h>
#include <vostok/strings_functions.h>

namespace survarium {

extern float const agent_radius = 0.35f;

 animation_space_vertex::animation_space_vertex( resources::managed_resource_ptr const& animation_vertex, pcstr animation_caption ) :
	animation		( animation_vertex ),
	caption			( animation_caption ),
	length			( animation::cubic_spline_skeleton_animation_pinned( animation )->length_in_frames( ) / 30.f ),
	group_id		( u32( -1 ) ),
	intervals_count	( u32( -1 ) )
{
}

 animation_space_graph::animation_space_graph(
	ai::navigation::world&		navigation_world,
	const float					agent_radius,
	const u32					animations_count,
	const u32					mixes_count,
	const u32					edges_count
) :
	m_navigation_world( navigation_world ),
	m_agent_radius( agent_radius ),
	m_max_speed( -1.0f ),
	m_animations_count( animations_count ),
	m_mixes_count( mixes_count ),
	m_edges_count( edges_count )
{
}

 animation_space_graph::~animation_space_graph( )
{
}

animation_space_vertex const* animation_space_graph::get_animation_by_path( pcstr animation_path ) const
{
	for ( animation_space_vertex const* it = get_animations( ), *const end = it + m_animations_count;
		it != end;
		++it )
		if ( strings::equal( it->caption.c_str( ), animation_path ) )
			return it;

	return NULL;
}

animation_space_vertex_id animation_space_graph::get_movement(
	animation::animation_player&		player,
	animation_space_vertex const*		left_animation,
	animation_space_vertex const*		right_animation,
	const float							left_weight
)
{
	player.reset( false );

	mutable_buffer buffer( ALLOCA( animation::animation_player::stack_buffer_size ), animation::animation_player::stack_buffer_size );

	// sushi@TODO: Resolve the parameter header's user-data initialization and synchronization-setter inlining against retail.
	animation::mixing::animation_lexeme right_lexeme(
		animation::mixing::animation_lexeme_parameters( buffer, "", left_animation->animation, NULL, NULL )
			.time_synchronization_group_id( 0 ).weight_synchronization_group_id( 0 )
	);
	animation::mixing::weight_lexeme left_weight_lexeme( buffer, left_weight, animation::instant_interpolator( ) );
	animation::mixing::animation_lexeme left_lexeme( animation::mixing::animation_lexeme_parameters( buffer, "", right_animation->animation, &right_lexeme, NULL ) );

	player.set_target_and_tick( right_lexeme*left_weight_lexeme + left_lexeme*(1.f - left_weight_lexeme), 0, math::float4x4( ).identity( ) );
	player.tick( math::floor( 1000.f*(left_weight*left_animation->length + (1.f - left_weight)*right_animation->length) ) );
	math::float4x4 const& object_movement = player.get_object_transform( NULL );

	animation_space_vertex_id result;
	result.rotation = math::quaternion( object_movement.get_angles_xyz( ) );
	result.translation = object_movement.c.xyz( );
	return result;
}

animation_space_edge const& animation_space_graph::edge( const u32 index ) const
{
	return get_edges( )[ index ];
}

float animation_space_graph::max_speed( ) const
{
	if ( m_max_speed >= 0.f )
		return m_max_speed;

	animation::animation_player player;

	m_max_speed = 0.f;
	for ( animation_space_vertex const* vertex = get_animations( ), *const end = vertex + m_animations_count;
		vertex != end;
		++vertex )
	{
		animation_space_vertex_id const& movement = get_movement( player, vertex, vertex, 1.f );
		m_max_speed = math::max( m_max_speed, movement.translation.length( ) );
	}

	return m_max_speed;
}

} // namespace survarium
