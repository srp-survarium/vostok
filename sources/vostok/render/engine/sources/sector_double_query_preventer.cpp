#include "pch.h"
#include "sector_double_query_preventer.h"

#include <vostok/math_randoms_generator.h>
#include <vostok/render/engine/vertex_colored.h>

#include "system_renderer.h"

namespace vostok {
namespace render {
namespace culling {

sector_double_query_preventer::sector_double_query_preventer( spatial_sector const* sectors, u32 sectors_count ) :
	m_buffer_for_frustum_vectors( ALLOC( frustums_type, sectors_count ) ),
	m_sectors_max_frustums( NEW( sectors_max_frustums_type )( m_buffer_for_frustum_vectors, sectors_count ) ),
	m_buffer_for_rect_vectors( ALLOC( aab_rects_type, sectors_count ) ),
	m_sectors_max_rects( NEW( sectors_max_rects_type )( m_buffer_for_rect_vectors, sectors_count ) )
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
	sectors_max_frustums_type::iterator const frustums_end = m_sectors_max_frustums->end( );
	for ( sectors_max_frustums_type::iterator i = m_sectors_max_frustums->begin( ); i != frustums_end; ++i )
		i->clear( );

	sectors_max_rects_type::iterator const rects_end = m_sectors_max_rects->end( );
	for ( sectors_max_rects_type::iterator i = m_sectors_max_rects->begin( ); i != rects_end; ++i )
		i->clear( );
}

bool sector_double_query_preventer::is_possible_points_for_frustum( float3 const (&vertices)[4], u32 sector_id ) const
{
	frustums_type const& frustums = ( *m_sectors_max_frustums )[sector_id];
	frustums_type::const_iterator const frustums_end = frustums.end( );
	for ( frustums_type::const_iterator i = frustums.begin( ); i != frustums_end; ++i )
	{
		bool is_inside = true;
		u32 j = 0;
		for ( ; j < 4; ++j )
		{
			if ( !math::is_point_inside_cuboid( vertices[j], *i ) )
			{
				is_inside = false;
				break;
			}
		}

		if ( is_inside )
			return false;
	}

	return true;
}

void sector_double_query_preventer::add_ss_aab_rect( aab_rect const& rect, u32 sector_id )
{
	( *m_sectors_max_rects )[sector_id].push_back( rect );
}

bool sector_double_query_preventer::is_possible_ss_aab_rect( aab_rect const& rect, u32 sector_id ) const
{
	aab_rects_type const& rects = ( *m_sectors_max_rects )[sector_id];
	aab_rects_type::const_iterator const rects_end = rects.end( );
	for ( aab_rects_type::const_iterator i = rects.begin( ); i != rects_end; ++i )
	{
		if ( i->contains( rect ) )
			return false;
	}

	return true;
}

void sector_double_query_preventer::add_frustum( math::frustum const& f, u32 sector_id )
{
	( *m_sectors_max_frustums )[sector_id].push_back( f );
}

bool sector_double_query_preventer::is_aabb_in_sector( math::aabb const& bbox, u32 sector_id ) const
{
	frustums_type const& frustums = ( *m_sectors_max_frustums )[sector_id];
	frustums_type::const_iterator const frustums_end = frustums.end( );
	for ( frustums_type::const_iterator i = frustums.begin( ); i != frustums_end; ++i )
	{
		if ( i->test_inexact( bbox ) != math::intersection_outside )
			return true;
	}

	return false;
}

bool sector_double_query_preventer::is_visible_aabb( math::aabb const& bbox, u16 const* sectors_begin, u16 const* sectors_end ) const
{
	for ( ; sectors_begin != sectors_end; ++sectors_begin )
	{
		if ( is_aabb_in_sector( bbox, *sectors_begin ) )
			return true;
	}

	return false;
}

void sector_double_query_preventer::render( system_renderer& r )
{
	u16 const frustrum_edges_indices[]	= {
		0, 1,	1, 2,	2, 3,	3, 0,
		4, 5,	5, 6,	6, 7,	7, 4,
		0, 4,	1, 5,	2, 6,	3, 7
	};
	for ( frustrum_images_type::const_iterator i = m_frustum_images.begin( ), e = m_frustum_images.end( ); i != e; ++i )
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

void sector_double_query_preventer::make_frustum_images( float3 const* furthest_vertices )
{
	m_frustum_images.clear( );
	math::random32 color_randomizer;
	sectors_max_frustums_type::const_iterator const sectors_max_frustums_end = m_sectors_max_frustums->end( );
	for ( sectors_max_frustums_type::const_iterator it = m_sectors_max_frustums->begin( ); it != sectors_max_frustums_end; ++it, ++furthest_vertices )
	{
		frustums_type::const_iterator const frutums_end = it->end( );
		if ( it != m_sectors_max_frustums->begin( ) )
		{
			math::color const color( math::color_xrgb( color_randomizer.random( 128 ) + 128, color_randomizer.random( 128 ) + 128, color_randomizer.random( 128 ) + 128 ) );
			for ( frustums_type::const_iterator frustum_it = it->begin( ); frustum_it != frutums_end; ++frustum_it )
			{
				m_frustum_images.push_back( frustum_image( ) );
				math::plane const planes[] = {
					frustum_it->planes( )[0].plane,
					frustum_it->planes( )[1].plane,
					frustum_it->planes( )[2].plane,
					frustum_it->planes( )[3].plane,
					math::plane( frustum_it->planes( )[4].plane.normal, -frustum_it->planes( )[4].plane.normal.dot_product( *furthest_vertices ) ),
					frustum_it->planes( )[5].plane,
				};
				math::frustum f( planes );
				math::get_frustum_vertices( f, m_frustum_images.back( ).vertices );
				m_frustum_images.back( ).c = color;
			}
		}
		else
		{
			for ( frustums_type::const_iterator frustum_it = it->begin( ); frustum_it != frutums_end; ++frustum_it )
			{
				m_frustum_images.push_back( frustum_image( ) );
				math::get_frustum_vertices( *frustum_it, m_frustum_images.back( ).vertices );
				m_frustum_images.back( ).c = math::color( 0xff00ff00 );
			}
		}
	}
}

u32 sector_double_query_preventer::frustums_count( ) const
{
	u32 result = 0;
	sectors_max_frustums_type::const_iterator const end = m_sectors_max_frustums->end( );
	for ( sectors_max_frustums_type::const_iterator i = m_sectors_max_frustums->begin( ); i != end; ++i )
		result += i->size( );
	return result;
}

} // namespace culling
} // namespace render
} // namespace vostok
