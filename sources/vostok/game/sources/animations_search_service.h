// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATIONS_SEARCH_SERVICE_H_INCLUDED
#define ANIMATIONS_SEARCH_SERVICE_H_INCLUDED

#include "animation_space_graph_wrapper.h"
#include <vostok/ai/search/a_star.h>
#include <vostok/ai/search/vertex_allocator_fixed_count.h>
#include <vostok/ai/search/vertex_manager_fixed_count_hash.h>
#include <vostok/ai/search/priority_queue_binary_heap.h>
#include <vostok/ai/search/path_constructor_edge.h>

namespace survarium {

class animation_space_heuristics;
class animation_space_search_restrictor;

// the survarium twin of ai::planning::search_base - same A* machinery,
// instantiated over the animation-space graph (the TU-scope typedefs the PDB
// records: distance_type=float, vertex_id_type=animation_space_vertex_id,
// path_type=vector<u32>, path_heuristics_type=animation_space_heuristics,
// search_restrictor_type=animation_space_search_restrictor,
// path_constructor_type=ai::path_constructor::edge::impl<vertex_type, vector<u32>>)
class animations_search_service {
public:
	typedef float distance_type;
	typedef animation_space_vertex_id vertex_id_type;
	typedef u32 edge_id_type;
	typedef ai::vertex_allocator::fixed_count vertex_allocator_type;
	typedef ai::vertex_manager::fixed_count_hash vertex_manager_type;
	typedef ai::priority_queue::binary_heap priority_queue_type;
	typedef animation_space_graph_wrapper graph_wrapper_type;
	typedef ai::path_constructor::edge path_constructor_helper_type;
	typedef ai::a_star algorithm_type;

	struct vertex_type :
		public ai::a_star::vertex_helper< float >::vertex_impl,
		public animation_space_graph_wrapper::vertex_impl,
		public ai::path_constructor::edge::helper< u32 >::vertex_impl< vertex_type >,
		public ai::priority_queue::binary_heap::vertex_impl,
		public ai::vertex_manager::fixed_count_hash::vertex_helper< animation_space_vertex_id >::vertex_impl,
		public ai::vertex_allocator::fixed_count::vertex_impl
	{
	}; // struct vertex_type

	struct look_up_cell_type :
		public ai::a_star::look_up_cell_impl,
		public animation_space_graph_wrapper::look_up_cell_impl,
		public ai::path_constructor::base::look_up_cell_impl,
		public ai::priority_queue::binary_heap::look_up_cell_impl,
		public ai::vertex_manager::fixed_count_hash::look_up_cell_helper< vertex_type >::look_up_cell_impl< look_up_cell_type >,
		public ai::vertex_allocator::fixed_count::look_up_cell_impl
	{
	}; // struct look_up_cell_type

	struct vertex_allocator_impl_type : public ai::vertex_allocator::fixed_count::impl< vertex_type > {
		typedef ai::vertex_allocator::fixed_count::impl< vertex_type >	super;
		inline		vertex_allocator_impl_type	( memory::base_allocator& arg_0, const u32 arg_1 ) :
			super( &arg_0, arg_1 )
		{ /* no source */ }
	}; // struct vertex_allocator_impl_type

	struct vertex_manager_impl_type : public ai::vertex_manager::fixed_count_hash::impl< vertex_allocator_impl_type, look_up_cell_type > {
		typedef ai::vertex_manager::fixed_count_hash::impl< vertex_allocator_impl_type, look_up_cell_type >	super;
		inline		vertex_manager_impl_type	(
						vertex_allocator_impl_type&	arg_0,
						const u32		arg_1,
						const u32		arg_2
					) :
			super( arg_0, arg_1, arg_2 )
		{ /* no source */ }
	}; // struct vertex_manager_impl_type

	struct priority_queue_impl_type : public ai::priority_queue::binary_heap::impl< vertex_manager_impl_type > {
		typedef ai::priority_queue::binary_heap::impl< vertex_manager_impl_type >	super;
		inline		priority_queue_impl_type	( vertex_manager_impl_type& arg_0, const u32 arg_1 ) :
			super( arg_0, arg_1 )
		{ /* no source */ }
	}; // struct priority_queue_impl_type

	typedef vector< u32 > path_type;
	typedef animation_space_heuristics path_heuristics_type;
	typedef ai::path_constructor::edge::impl< vertex_type, path_type > path_constructor_type;
	typedef animation_space_search_restrictor search_restrictor_type;
	enum { max_vertex_count = 1024, hash_size = 4096 };

public:
	inline			animations_search_service	( ) :
		m_vertex_allocator( *::survarium::g_allocator, 0x400 ),
		m_vertex_manager( m_vertex_allocator, 0x1000, 0x400 ),
		m_priority_queue( m_vertex_manager, 0x400 )
	{ }

			bool	search						(
						animation_space_graph_ptr const&	graph,
							path_type*							path,
						const animation_space_vertex_id		arg_2 /* animation_space_vertex_id start_vertex_id */,
						const animation_space_vertex_id		arg_3 /* animation_space_vertex_id target_vertex_id */
					);


public:
	/* 0x0000 */	animations_search_service::vertex_allocator_impl_type	m_vertex_allocator;
	/* 0x0010 */	animations_search_service::vertex_manager_impl_type	m_vertex_manager;
	/* 0x002c */	animations_search_service::priority_queue_impl_type	m_priority_queue;
}; // class animations_search_service

STATIC_SIZE_ASSERT(animations_search_service, 0x3C);
STATIC_SIZE_ASSERT(animations_search_service::vertex_type, 0x30);
STATIC_SIZE_ASSERT(animations_search_service::look_up_cell_type, 0x18);
STATIC_SIZE_ASSERT(animations_search_service::vertex_allocator_impl_type, 0x10);
STATIC_SIZE_ASSERT(animations_search_service::vertex_manager_impl_type, 0x1C);
STATIC_SIZE_ASSERT(animations_search_service::priority_queue_impl_type, 0x10);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SEARCH_SERVICE_H_INCLUDED
