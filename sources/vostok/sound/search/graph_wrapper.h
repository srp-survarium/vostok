////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SEARCH_GRAPH_WRAPPER_H_INCLUDED
#define VOSTOK_SOUND_SEARCH_GRAPH_WRAPPER_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/sound/search/vertex_id_type.h>
#include <vostok/render/culling/portal_sector_structure.h>

namespace vostok {
namespace sound {
namespace search {

class graph_wrapper : private boost::noncopyable
{
public:
	struct vertex_impl
	{
	};

	struct look_up_cell_impl
	{
	};

	typedef u32 const* const_edge_iterator;

public:
	inline explicit graph_wrapper( render::culling::portal_sector_structure_ptr const& graph ) :
		m_graph( graph )
	{
	}

	inline void on_before_search( )
	{
	}

	inline void on_after_search( bool const )
	{
	}

	template < typename vertex_type >
	inline void edge_iterators(
		vertex_type const& vertex,
		const_edge_iterator& begin,
		const_edge_iterator& end
	);

	inline vertex_id_type vertex_id( vertex_id_type const& vertex_id, const_edge_iterator const iterator ) const;

	inline u32 edge_id( vertex_id_type const&, const_edge_iterator const iterator ) const
	{
		return *iterator;
	}

	inline ~graph_wrapper( )
	{
	}

private:
	render::culling::portal_sector_structure_ptr const& m_graph;
}; // class graph_wrapper

STATIC_SIZE_ASSERT( graph_wrapper, 0x4 );

} // namespace search
} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SEARCH_GRAPH_WRAPPER_H_INCLUDED
