////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_GRAPH_H_INCLUDED
#define ANIMATION_SPACE_GRAPH_H_INCLUDED

#include "animation_space_edge.h"
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace ai {
namespace navigation {
	struct world;
} // namespace navigation
} // namespace ai
namespace animation {
	class animation_player;
} // namespace animation
} // namespace vostok

namespace survarium {

class animation_space_graph : public resources::unmanaged_resource , public boost::noncopyable {
public:
												animation_space_graph	(
													ai::navigation::world&		navigation_world,
													const float					agent_radius,
													const u32					animations_count,
													const u32					mixes_count,
													const u32					edges_count
												);
	virtual										~animation_space_graph	( );

			float								max_speed				( ) const;

			animation_space_edge const&			edge					( const u32 index ) const;

	inline	u32									get_animations_count	( ) const { /* no source */ return m_animations_count; }
	inline	u32									get_mixes_count			( ) const { /* no source */ return m_mixes_count; }
	inline	u32									get_edges_count			( ) const { /* no source */ return m_edges_count; }
	// the vertex/mix/edge arrays have no named members - the cook allocates them
	// behind the object; buildability stubs until a matcher recovers the layout math
	inline	animation_space_vertex const*		get_animations			( ) const { /* no source */ return NULL; }
	inline	std::pair< animation_space_vertex const*, animation_space_vertex const* > const*	get_mixes				( ) const { /* no source */ return NULL; }
	inline	animation_space_edge const*			get_edges				( ) const { /* no source */ return NULL; }

	inline	float								agent_radius			( ) const { /* no source */ return m_agent_radius; }

	inline	bool								is_passable				( animation_space_vertex_id const& arg_0, animation_space_vertex_id const& arg_1 ) const { /* no source */ return false; }

			animation_space_vertex const*		get_animation_by_path	( pcstr animation_path ) const;

	static	animation_space_vertex_id			get_movement			(
													animation::animation_player&		player,
													animation_space_vertex const*		left_animation,
													animation_space_vertex const*		right_animation,
													const float							left_weight
												);

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	ai::navigation::world&		m_navigation_world;
	/* 0x010c */	const float					m_agent_radius;
	/* 0x0110 */	float						m_max_speed;
	/* 0x0114 */	const u32					m_animations_count;
	/* 0x0118 */	const u32					m_mixes_count;
	/* 0x011c */	const u32					m_edges_count;
}; // class animation_space_graph

STATIC_SIZE_ASSERT(animation_space_graph, 0x120);

typedef resources::resource_ptr<
	animation_space_graph,
	resources::unmanaged_intrusive_base
> animation_space_graph_ptr;

// STATE[STUB]
inline bool operator!=( animation_space_vertex_id const& left, animation_space_vertex_id const& right )
{
	return false;

	// FUNCTION BODY[0xad210]: 1
	// <0xad210>|0x000|+0x000:'29'	{
	// <0xad210>|0x000|+0x063:'30'
	// <0xad273>|0x063|-0x001:'30'
	// <0xad272>|0x062|+0x00c:'31'
	// <0xad27e>|0x06e|      :'31'	}
	// ******
}

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_GRAPH_H_INCLUDED
