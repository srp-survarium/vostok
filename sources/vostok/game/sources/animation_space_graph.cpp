////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animation_space_graph.h"
#include "animation_space_vertex.h"

namespace survarium {

// STATE[STUB]
// claude@NOTE: sets animation (intrusive_ptr::set), constructs caption from pcstr, then
// pins the resource as a cubic_spline_skeleton_animation and computes length(+0x118) from
// the spline duration (mulss 1/30.0f), group_id(+0x11c)/intervals_count(+0x120) = -1.
// Needs the cubic_spline_skeleton_animation + pinned_ptr_base internals to match.
 animation_space_vertex::animation_space_vertex( resources::managed_resource_ptr const& animation_vertex, pcstr animation_caption )
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
// claude@NOTE: lexeme-builder wall - builds animation_lexeme / weight_lexeme over a
// mutable_buffer and an interpolator; needs the mixing-lexeme machinery
// (create_animation_interval family) before it can be matched. Buildability return only.
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

// STATE[STUB]
// claude@NOTE: walks the vertices building an animation_player + get_movement() per
// pair, takes the max speed via sqrtf of the movement translation, then resets the
// player and destroys the n_ary_tree. Depends on get_movement (lexeme wall) and the
// animation_player/n_ary_tree machinery. Buildability return only.
float animation_space_graph::max_speed( ) const
{
	return 0.0f;
}

} // namespace survarium
