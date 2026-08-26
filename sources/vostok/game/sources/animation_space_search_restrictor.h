////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_SEARCH_RESTRICTOR_H_INCLUDED
#define ANIMATION_SPACE_SEARCH_RESTRICTOR_H_INCLUDED

#include "animation_space_graph.h"
#include <vostok/ai/search/search_restrictor_generic.h>

namespace survarium {

class animation_space_search_restrictor : public ai::search_restrictor::generic< animation_space_vertex_id, u32, float > {
public:
	typedef ai::search_restrictor::generic< animation_space_vertex_id, u32, float > super;

	inline			animation_space_search_restrictor	(
						animation_space_graph_ptr const&	arg_0,
						animation_space_vertex_id const&	arg_1,
						animation_space_vertex_id const&	arg_2,
						float const&						arg_3,
						const u32							arg_4,
						const u32							arg_5
					) :
		// the base refs/m_graph force the init-list; the arg mapping is the
		// generic<> ctor's own (start, target, max_range, max_iterations,
		// max_visited) - a matcher confirms at the inlining call site
		super( arg_1, arg_2, arg_3, arg_4, arg_5 ),
		m_graph( arg_0 )
	{ /* no source */ }

	template < typename VertexType >
	inline	bool	accessible						(
					animation_space_vertex_id const& neighbour_vertex_id,
					VertexType const& current_vertex,
					const u32 iterator
				) const
	{
		return true;
	}

	inline	u32		get_start_vertices_count			( ) const { return 1; }


private:
	/* 0x0000 */	/* ai::search_restrictor::generic< animation_space_vertex_id, u32, float > */
	/* 0x0014 */	animation_space_graph_ptr const&	m_graph;
}; // class animation_space_search_restrictor

STATIC_SIZE_ASSERT(animation_space_search_restrictor, 0x18);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_SEARCH_RESTRICTOR_H_INCLUDED
