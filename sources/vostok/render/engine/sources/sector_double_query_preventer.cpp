#include "pch.h"
#include "sector_double_query_preventer.h"

#include <vostok/render/engine/vertex_colored.h>

#include "system_renderer.h"

namespace vostok {
namespace render {
namespace culling {

sector_double_query_preventer::sector_double_query_preventer( spatial_sector const* sectors, u32 sectors_count ) :
	m_buffer_for_frustum_vectors( ALLOC( vector<math::frustum>, sectors_count ) ),
	m_sectors_max_frustums( NEW( buffer_vector<vector<math::frustum> > )( m_buffer_for_frustum_vectors, sectors_count ) ),
	m_buffer_for_rect_vectors( ALLOC( vector<aab_rect>, sectors_count ) ),
	m_sectors_max_rects( NEW( buffer_vector<vector<aab_rect> > )( m_buffer_for_rect_vectors, sectors_count ) )
{
	m_sectors_max_frustums->resize( sectors_count );
	m_sectors_max_rects->resize( sectors_count );
	for ( u32 i = 0; i < sectors_count; ++i )
	{
		( *m_sectors_max_frustums )[i].reserve( sectors[i].get_portals_count( ) * 2 );
		( *m_sectors_max_rects )[i].reserve( sectors[i].get_portals_count( ) * 2 );
	}
}

sector_double_query_preventer::~sector_double_query_preventer( )
{
	clear( );
	DELETE( m_sectors_max_frustums );
	FREE( m_buffer_for_frustum_vectors );
	DELETE( m_sectors_max_rects );
	FREE( m_buffer_for_rect_vectors );
}

void sector_double_query_preventer::clear( )
{
	buffer_vector<vector<math::frustum> >::iterator const frustums_end = m_sectors_max_frustums->end( );
	for ( buffer_vector<vector<math::frustum> >::iterator i = m_sectors_max_frustums->begin( ); i != frustums_end; ++i )
		i->clear( );

	buffer_vector<vector<aab_rect> >::iterator const rects_end = m_sectors_max_rects->end( );
	for ( buffer_vector<vector<aab_rect> >::iterator i = m_sectors_max_rects->begin( ); i != rects_end; ++i )
		i->clear( );
}

bool sector_double_query_preventer::is_possible_points_for_frustum( float3 const (&)[4], u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e86a0]
	return false;
}

void sector_double_query_preventer::add_ss_aab_rect( aab_rect const&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8b10]
}

bool sector_double_query_preventer::is_possible_ss_aab_rect( aab_rect const&, u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8540]
	return false;
}

void sector_double_query_preventer::add_frustum( math::frustum const&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8ad0]
}

bool sector_double_query_preventer::is_aabb_in_sector( math::aabb const&, u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8650]
	return false;
}

bool sector_double_query_preventer::is_visible_aabb( math::aabb const&, u16 const*, u16 const* ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8700]
	return false;
}

void sector_double_query_preventer::render( system_renderer& r )
{
	u16 const frustrum_edges_indices[]	= {
		0, 1,	1, 2,	2, 3,	3, 0,
		4, 5,	5, 6,	6, 7,	7, 4,
		0, 4,	1, 5,	2, 6,	3, 7
	};

	for ( vector<frustum_image>::const_iterator i = m_frustum_images.begin( ), e = m_frustum_images.end( ); i != e; ++i )
	{
		vertex_colored vertices[] = {
			vertex_colored( i->vertices[0], i->c ),
			vertex_colored( i->vertices[1], i->c ),
			vertex_colored( i->vertices[2], i->c ),
			vertex_colored( i->vertices[3], i->c ),
			vertex_colored( i->vertices[4], i->c ),
			vertex_colored( i->vertices[5], i->c ),
			vertex_colored( i->vertices[6], i->c ),
			vertex_colored( i->vertices[7], i->c ),
		};
		r.draw_lines	(
			vertices, vertices + array_size( vertices ),
			frustrum_edges_indices, frustrum_edges_indices + array_size( frustrum_edges_indices ),
			false
		);
	}
}

void sector_double_query_preventer::make_frustum_images( float3 const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8770]
}

u32 sector_double_query_preventer::frustums_count( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8480]
	return 0;
}

} // namespace culling
} // namespace render
} // namespace vostok
