// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
// VOSTOK_ANIMATION_API used unqualified by bone_animation.h (pulled in below)
#include <vostok/animation/api.h>
#include "animation_space_graph.h"
#include "animation_space_vertex.h"
#include "animation_space_vertex_id.h"
// length: pin the resource as a cubic_spline_skeleton_animation, read its frame duration
#include <vostok/animation/cubic_spline_skeleton_animation.h>
// max_speed(): an animation_player drives get_movement() per vertex
#include <vostok/animation/animation_player.h>
#include <vostok/animation/instant_interpolator.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/mixing_multiplication_lexeme.h>
#include <vostok/animation/mixing_weight_lexeme.h>

namespace survarium {

 animation_space_vertex::animation_space_vertex( resources::managed_resource_ptr const& animation_vertex, pcstr animation_caption ) :
	animation		( animation_vertex ),
	caption			( animation_caption ),
	length			( animation::cubic_spline_skeleton_animation_pinned( animation_vertex )->length_in_frames( ) / animation::default_fps ),
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

// claude@NOTE: target records 0 named locals + inlines the caption strcmp; the
// natural hand loop here keeps the begin/end iterators as named locals and resolves
// caption==pcstr to out-of-line vostok::operator==. Structure (linear caption search)
// is faithful; the 0-local / inlined-compare form (likely a std::find_if with an
// inlined predicate) is the byte residual left to recover.
animation_space_vertex const* animation_space_graph::get_animation_by_path( pcstr animation_path ) const
{
	animation_space_vertex const* it		= get_animations( );
	animation_space_vertex const* const end	= it + m_animations_count;
	for ( ; it != end; ++it )
		if ( it->caption == animation_path )
			return it;

	return NULL;
}

// keeps the buffer construction out-of-line here like retail (the two-store
// body ICF-folds with the ctor group); the global ctor stays inline - other
// sites inline it in retail
static __declspec( noinline ) mutable_buffer make_stack_buffer( pvoid data, u32 size )
{
	return mutable_buffer( data, size );
}

animation_space_vertex_id animation_space_graph::get_movement(
	animation::animation_player&		player,
	animation_space_vertex const*		left_animation,
	animation_space_vertex const*		right_animation,
	const float							left_weight
)
{
	player.reset( false );

	mutable_buffer buffer = make_stack_buffer( ALLOCA( animation::animation_player::stack_buffer_size ), animation::animation_player::stack_buffer_size );

	animation::mixing::animation_lexeme right_lexeme( animation::mixing::animation_lexeme_parameters( buffer, "", left_animation->animation, NULL, NULL ) );
	animation::mixing::weight_lexeme left_weight_lexeme( buffer, left_weight, animation::instant_interpolator( ) );
	animation::mixing::animation_lexeme left_lexeme( animation::mixing::animation_lexeme_parameters( buffer, "", right_animation->animation, &right_lexeme, NULL ) );

	player.set_target_and_tick( left_lexeme*(1.f - left_weight_lexeme) + right_lexeme*left_weight_lexeme, 0, math::float4x4( ).identity( ) );
	player.tick( math::floor( 1000.f*(left_weight*left_animation->length + (1.f - left_weight)*right_animation->length) ) );
	math::float4x4 const object_movement = player.get_object_transform( NULL );

	animation_space_vertex_id result = {
		math::quaternion( object_movement.get_angles_xyz( ) ),
		object_movement.c.xyz( )
	};
	return result;
}

animation_space_edge const& animation_space_graph::edge( const u32 index ) const
{
	return get_edges( )[ index ];
}

float animation_space_graph::max_speed( ) const
{
	if ( m_max_speed < 0.f )
	{
		animation::animation_player player;

		m_max_speed = 0.f;
		for ( animation_space_vertex const* vertex = get_animations( ); vertex != get_animations( ) + m_animations_count; ++vertex )
			m_max_speed = math::max( m_max_speed, get_movement( player, vertex, vertex, 1.f ).translation.length( ) );

		player.reset( true );
	}

	return m_max_speed;
}

} // namespace survarium
