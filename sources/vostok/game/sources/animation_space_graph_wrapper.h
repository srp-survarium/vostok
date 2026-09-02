// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_SPACE_GRAPH_WRAPPER_H_INCLUDED
#define ANIMATION_SPACE_GRAPH_WRAPPER_H_INCLUDED

#include "animation_space_graph.h"

namespace survarium {

struct animation_space_graph_wrapper : private boost::noncopyable {
	struct vertex_impl {
	}; // struct vertex_impl

	struct look_up_cell_impl {
	}; // struct look_up_cell_impl

	typedef u32 const_edge_iterator;

	inline									animation_space_graph_wrapper	( animation_space_graph_ptr const& arg_0, animation_space_vertex_id const& arg_1 ) :
		m_graph( arg_0 ),
		m_start_vertex_id( arg_1 )
	{ /* no source */ }

	inline	void							on_before_search				( ) { /* no source */ }
	inline	void							on_after_search					( const bool arg_0 ) { /* no source */ }
	template < typename VertexType >
	inline	void							edge_iterators					( VertexType const& vertex, const_edge_iterator& begin, const_edge_iterator& end ) const
	{
		begin = 0;
		end = m_graph->get_edges_count( );
	}

			animation_space_vertex_id		vertex_id						( animation_space_vertex_id const& vertex_id, const u32 iterator ) const;
	inline	u32								edge_id							( animation_space_vertex_id const& arg_0, const u32 iterator ) const { return iterator; }


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	animation_space_graph_ptr const&	m_graph;
	/* 0x0004 */	animation_space_vertex_id const&	m_start_vertex_id;
}; // struct animation_space_graph_wrapper

STATIC_SIZE_ASSERT(animation_space_graph_wrapper, 0x8);
STATIC_SIZE_ASSERT(animation_space_graph_wrapper::vertex_impl, 0x1);
STATIC_SIZE_ASSERT(animation_space_graph_wrapper::look_up_cell_impl, 0x1);

} // namespace survarium

#include "animation_space_graph_wrapper_inline.h"

#endif // #ifndef ANIMATION_SPACE_GRAPH_WRAPPER_H_INCLUDED
