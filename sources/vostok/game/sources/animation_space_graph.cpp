////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

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

namespace survarium {

 animation_space_vertex::animation_space_vertex( resources::managed_resource_ptr const& animation_vertex, pcstr animation_caption ) :
	animation		( animation_vertex ),
	caption			( animation_caption ),
	length			( animation::cubic_spline_skeleton_animation_pinned( animation_vertex )->length_in_frames( ) / 30.f ),
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

// STATE[STUB]
// claude@NOTE: 10-statement lexeme builder (structure @0x765a70, 4 locals: buffer,
// left_lexeme/right_lexeme animation_lexeme, left_weight_lexeme weight_lexeme). Decoded
// flow: player.reset(false); build a mutable_buffer over a 0x4000-byte alloca; construct
// animation_lexeme_parameters (animation_intervals_count -> create_animation_intervals)
// for the right then the left animation, wrap each in a binary_tree_animation_node +
// animation_lexeme, and cloned_in_buffer them; build a weight_lexeme over an
// instant_interpolator (call [0x9735AC] = the interpolator vtable thunk) for
// (1 - left_weight); combine via the lexeme operator*/operator+ combinators; run the
// player and read back the resulting object_movement into the returned vertex_id.
// The lexeme TYPES are now available (mixing_animation_lexeme.h / mixing_weight_lexeme.h /
// animation_lexeme_parameters::create_animation_intervals), but the exact per-segment
// combinator spelling + the instant_interpolator construction idiom are unconfirmed.
// PARKED buildability return. NEXT STEP: reconstruct statement-by-statement against
// --view target, cross-referencing animation_space_graph_cook.cpp (same lexeme idiom over
// a buffer_vector<u32>) and simple_animation_controller::selected_animations (emitter->emit).
animation_space_vertex_id animation_space_graph::get_movement(
	animation::animation_player&		player,
	animation_space_vertex const*		left_animation,
	animation_space_vertex const*		right_animation,
	const float							left_weight
)
{
	return animation_space_vertex_id( );
}

animation_space_edge const& animation_space_graph::edge( const u32 index ) const
{
	return get_edges( )[ index ];
}

// claude@NOTE: 1 named local (player) matching the target - the loop body is a single
// statement (get_movement(...).translation.length() inlined; the target records no named
// `movement` local). The byte residual is dominated by
// get_movement still being a STATE[STUB] (its body is unrecovered) plus the EH-scope brace
// line the target emits for player's destructor (a lone TRGT_ONLY branch row, not steerable).
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
