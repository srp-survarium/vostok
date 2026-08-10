////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SEARCH_SEARCH_SERVICE_H_INCLUDED
#define VOSTOK_SOUND_SEARCH_SEARCH_SERVICE_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/sound/sound.h>
#include <vostok/sound/search/vertex_id_type.h>
#include <vostok/sound/search/graph_wrapper.h>
#include <vostok/ai/search/a_star.h>
#include <vostok/ai/search/path_constructor_base.h>
#include <vostok/ai/search/priority_queue_binary_heap.h>
#include <vostok/ai/search/vertex_allocator_fixed_count.h>
#include <vostok/ai/search/vertex_manager_fixed_count_hash.h>

namespace vostok {
namespace sound {
namespace search {

class search_service : public noncopyable
{
public:
	struct vertex_type :
		public ai::a_star::vertex_helper< float >::vertex_impl,
		public graph_wrapper::vertex_impl,
		public ai::path_constructor::base::vertex_impl< vertex_type >,
		public ai::priority_queue::binary_heap::vertex_impl,
		public ai::vertex_manager::fixed_count_hash::vertex_helper< vertex_id_type >::vertex_impl,
		public ai::vertex_allocator::fixed_count::vertex_impl
	{
		inline vertex_type( )
		{
		}
	};

	struct look_up_cell_type :
		public ai::a_star::look_up_cell_impl,
		public graph_wrapper::look_up_cell_impl,
		public ai::path_constructor::base::look_up_cell_impl,
		public ai::priority_queue::binary_heap::look_up_cell_impl,
		public ai::vertex_manager::fixed_count_hash::look_up_cell_helper< vertex_type >::look_up_cell_impl< look_up_cell_type >,
		public ai::vertex_allocator::fixed_count::look_up_cell_impl
	{
	};

	struct vertex_allocator_impl_type : public ai::vertex_allocator::fixed_count::impl< vertex_type >
	{
		typedef ai::vertex_allocator::fixed_count::impl< vertex_type > super;

		inline vertex_allocator_impl_type( memory::base_allocator* allocator, u32 const vertex_count ) :
			super( allocator, vertex_count )
		{
		}

		inline ~vertex_allocator_impl_type( )
		{
		}
	};

	struct vertex_manager_impl_type : public ai::vertex_manager::fixed_count_hash::impl< vertex_allocator_impl_type, look_up_cell_type >
	{
		typedef ai::vertex_manager::fixed_count_hash::impl< vertex_allocator_impl_type, look_up_cell_type > super;

		inline vertex_manager_impl_type(
			vertex_allocator_impl_type& allocator,
			u32 const hash_size,
			u32 const fix_size
		) :
			super( allocator, hash_size, fix_size )
		{
		}

		inline ~vertex_manager_impl_type( )
		{
		}
	};

	struct priority_queue_impl_type : public ai::priority_queue::binary_heap::impl< vertex_manager_impl_type >
	{
		typedef ai::priority_queue::binary_heap::impl< vertex_manager_impl_type > super;

		inline priority_queue_impl_type( vertex_manager_impl_type& manager, u32 const vertex_count ) :
			super( manager, vertex_count )
		{
		}

		inline ~priority_queue_impl_type( )
		{
		}
	};

public:
	inline explicit search_service( memory::base_allocator* allocator ) :
		m_vertex_allocator( allocator, 4096 ),
		m_vertex_manager( m_vertex_allocator, 256, 4096 ),
		m_priority_queue( m_vertex_manager, 4096 )
	{
	}

	bool search(
		memory::base_allocator& allocator,
		render::culling::portal_sector_structure_ptr const& graph,
		vector< vertex_id_type >* path,
		float3 const& start_position,
		float3 const& target_position,
		float const& max_distance,
		vectora< fixed_vector< u32, 32 > >& result_paths
	);

	inline ~search_service( )
	{}

private:
	vertex_allocator_impl_type m_vertex_allocator;
	vertex_manager_impl_type m_vertex_manager;
	priority_queue_impl_type m_priority_queue;
}; // class search_service

STATIC_SIZE_ASSERT( search_service, 0x3C );
STATIC_SIZE_ASSERT( search_service::vertex_type, 0x1C );
STATIC_SIZE_ASSERT( search_service::look_up_cell_type, 0x18 );
STATIC_SIZE_ASSERT( search_service::vertex_allocator_impl_type, 0x10 );
STATIC_SIZE_ASSERT( search_service::vertex_manager_impl_type, 0x1C );
STATIC_SIZE_ASSERT( search_service::priority_queue_impl_type, 0x10 );

} // namespace search
} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SEARCH_SEARCH_SERVICE_H_INCLUDED
