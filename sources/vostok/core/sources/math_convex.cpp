////////////////////////////////////////////////////////////////////////////
//	Created		: 25.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
//  Description	: Convex is a solid figure bounded by several faces, forming a convex volume
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/math_convex.h>
#include <vostok/linkage_helper.h>
#include <numeric>
#ifndef VOSTOK_STATIC_LIBRARIES
	VOSTOK_DECLARE_LINKAGE_ID(core_math_convex)
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

using vostok::buffer_vector;
using vostok::math::convex;
using vostok::math::plane;
using vostok::math::float4x4;
using vostok::math::aabb;
using vostok::math::intersection;
using vostok::math::float3;
using vostok::math::float4;

struct edge_less : public std::binary_function< convex::cache::edges_type::value_type const&, convex::cache::edges_type::value_type const&, bool >
{
	bool operator()( convex::cache::edges_type::value_type const& left, convex::cache::edges_type::value_type const& right )
	{
		return left.first < right.first || ( left.first == right.first && left.second < right.second );
	}
};

static bool three_planes_intersection( plane const& first, plane const& second, plane const& third, float3& intersection )
{
	return
		try_solve_linear_equations_system (
			first.normal,
			second.normal,
			third.normal,
			float3( -first.d, -second.d, -third.d ),
			intersection
		);
}

convex::convex()
{
}

convex::convex( aabb const& bbox )
{
	m_planes.resize( 6 );
	m_planes[0].plane = math::create_plane( bbox.vertex(0), bbox.vertex(1), bbox.vertex(2) );
	m_planes[1].plane = math::create_plane( bbox.vertex(0), bbox.vertex(2), bbox.vertex(4) );
	m_planes[2].plane = math::create_plane( bbox.vertex(0), bbox.vertex(4), bbox.vertex(1) );
	m_planes[3].plane = math::create_plane( bbox.vertex(7), bbox.vertex(5), bbox.vertex(6) );
	m_planes[4].plane = math::create_plane( bbox.vertex(7), bbox.vertex(3), bbox.vertex(5) );
	m_planes[5].plane = math::create_plane( bbox.vertex(7), bbox.vertex(6), bbox.vertex(3) );
	std::for_each( m_planes.begin(), m_planes.end(), std::mem_fun_ref( &aabb_plane::normalize ) );
	m_adjacencies.resize( 8 );
	u8 const vertex_planes[8][3] = {
		{ 0, 1, 2 }, { 0, 2, 4 }, { 0, 1, 5 }, { 0, 4, 5 },
		{ 1, 2, 3 }, { 2, 3, 4 }, { 1, 3, 5 }, { 3, 4, 5 },
	};
	for ( u32 i = 0; i < 8; ++i )
	{
		m_adjacencies[ i ].push_back( vertex_planes[ i ][ 0 ] );
		m_adjacencies[ i ].push_back( vertex_planes[ i ][ 1 ] );
		m_adjacencies[ i ].push_back( vertex_planes[ i ][ 2 ] );
	}
}

convex::convex( planes_type const& planes, adjacencies_type const& adjacency ):
m_planes( planes ),
m_adjacencies( adjacency )
{

}
void convex::split( plane const& p, cache const& c, convex& positive, convex& negative ) const
{
	R_ASSERT( p.valid() );
	positive.m_adjacencies.clear();
	negative.m_adjacencies.clear();
	positive.m_planes.clear();
	negative.m_planes.clear();
	if ( std::find_if( m_planes.begin(), m_planes.end(), std::bind2nd( plane_similar(), p ) ) != m_planes.end() )
	{
		negative = *this;
		return;
	}
	if ( std::find_if( m_planes.begin(), m_planes.end(), std::bind2nd( plane_similar(), math::plane( -p.normal, - p.d ) ) ) != m_planes.end() )
	{
		positive = *this;
		return;
	}
	R_ASSERT( m_adjacencies.size() == c.vertices.size() );
	R_ASSERT( sizeof( vertex_flags ) * CHAR_BIT >= c.vertices.size() );
	vertex_flags pos_vertex_flags = 0;
	vertex_flags neg_vertex_flags = 0;
	u32 const vertex_count = c.vertices.size();
	for ( u32 i = 0; i < vertex_count; ++i )
	{
		float const dist = p.classify( c.vertices[ i ] );
		if ( math::is_zero( dist ) )
			continue;
		if ( dist > 0.0f )
		{
			positive.m_adjacencies.push_back( m_adjacencies[ i ] );
			pos_vertex_flags |= static_cast<vertex_flags>( 1 << i );
		}
		else
		{
			negative.m_adjacencies.push_back( m_adjacencies[ i ] );
			neg_vertex_flags |= static_cast<vertex_flags>( 1 << i );
		}
	}
	//R_ASSERT( !positive.m_adjacencies.empty() && !negative.m_adjacencies.empty() );

	math::float3 intersection;
	cache::edges_type::const_iterator const edge_end_it = c.edges.end();
	for ( cache::edges_type::const_iterator it = c.edges.begin(); it != edge_end_it; ++it )
	{
		math::float3 const& first	= c.vertices[ it->first  ];
		math::float3 const& second	= c.vertices[ it->second ];
		if ( math::is_zero( p.classify( first ) ) && math::is_zero( p.classify( second ) ) )
			continue;
		if ( p.intersect_segment( first, second, intersection ) )
		{
			vertex_adjacency const& first_adjacency		= m_adjacencies[ it->first  ];
			vertex_adjacency const& second_adjacency	= m_adjacencies[ it->second ];
			vertex_adjacency edge_adjacency;
			vertex_adjacency::const_iterator const first_end_it = first_adjacency.end();
			for ( vertex_adjacency::const_iterator at = first_adjacency.begin(); at != first_end_it; ++at )
				if ( std::find( second_adjacency.begin(), second_adjacency.end(), *at ) != second_adjacency.end() )
					edge_adjacency.push_back( *at );
			R_ASSERT( edge_adjacency.size() == 2 );
			edge_adjacency.push_back( static_cast<adjacency>( m_planes.size() ) );
			vertex_flags const edge_vertex_flags = static_cast<vertex_flags>( ( 1 << it->first ) | ( 1 << it->second ) );
			if ( pos_vertex_flags & edge_vertex_flags )
			{
				positive.m_adjacencies.push_back( edge_adjacency );
			}
			if ( neg_vertex_flags & edge_vertex_flags )
			{
				negative.m_adjacencies.push_back( edge_adjacency );
			}
		}
	}
	std::vector<u8> const pos_plane_flags = get_plane_flags( positive.m_adjacencies );
	u8 original_to_positive_buffer[ msc_max_plane_count ];
	buffer_vector<u8> original_to_positive( original_to_positive_buffer, msc_max_plane_count );
	build_original_to_new_adjacency_dict( pos_plane_flags, original_to_positive );

	std::vector<u8> const neg_plane_flags = get_plane_flags( negative.m_adjacencies );
	u8 original_to_negative_buffer[ msc_max_plane_count ];
	buffer_vector<u8> original_to_negative( original_to_negative_buffer, msc_max_plane_count );
	build_original_to_new_adjacency_dict( neg_plane_flags, original_to_negative );

	u32 const planes_count = m_planes.size();
	for ( u32 i = 0; i < planes_count; ++i )
	{
		if ( std::find( pos_plane_flags.begin(), pos_plane_flags.end(), static_cast<u8>( i ) ) != pos_plane_flags.end() )
			positive.m_planes.push_back( m_planes[ i ] );
		if ( std::find( neg_plane_flags.begin(), neg_plane_flags.end(), static_cast<u8>( i ) ) != neg_plane_flags.end() )
			negative.m_planes.push_back( m_planes[ i ] );
	}

	if ( !positive.empty() )
	{
		aabb_plane new_plane;
		new_plane.plane.vector = -math::float4( p.vector );//normals are directed outwards
		new_plane.normalize();
		original_to_positive[ m_planes.size() ] = static_cast<u8>( positive.m_planes.size() );
		positive.m_planes.push_back( new_plane );
		adjacencies_type::iterator const end_it = positive.m_adjacencies.end();
		for ( adjacencies_type::iterator at = positive.m_adjacencies.begin(); at != end_it; ++at )
			convert_original_adjacency_to_new( original_to_positive, *at );
	}

	if ( !negative.empty() )
	{
		aabb_plane new_plane;
		new_plane.plane = p;
		new_plane.normalize();
		original_to_negative[ m_planes.size() ] = static_cast<u8>( negative.m_planes.size() );
		negative.m_planes.push_back( new_plane );
		adjacencies_type::iterator const end_it = negative.m_adjacencies.end();
		for ( adjacencies_type::iterator at = negative.m_adjacencies.begin(); at != end_it; ++at )
			convert_original_adjacency_to_new( original_to_negative, *at );
	}
}

void convex::sort_adjacencies( )
{
	adjacencies_type::iterator const end_it = m_adjacencies.end();
	for ( adjacencies_type::iterator it = m_adjacencies.begin(); it != end_it; ++it )
		std::sort( it->begin(), it->end() );
}

std::vector<u8> convex::get_plane_flags( adjacencies_type const& adjacency_vector )
{
	std::vector<u8> plane_flags;
	adjacencies_type::const_iterator const end_it = adjacency_vector.end();
	for ( adjacencies_type::const_iterator it = adjacency_vector.begin(); it != end_it; ++it )
	{
		vertex_adjacency::const_iterator const adjacency_end_it = it->end();
		for ( vertex_adjacency::const_iterator at = it->begin(); at != adjacency_end_it; ++at )
			if ( std::find( plane_flags.begin(), plane_flags.end(), *at ) == plane_flags.end() )
				plane_flags.push_back( *at );
	}
	std::sort( plane_flags.begin(), plane_flags.end() );
	return plane_flags;
}

void convex::convert_original_adjacency_to_new( buffer_vector<u8> const& dict, vertex_adjacency& adjacency )
{
	R_ASSERT( adjacency.size() >= 3 );
	vertex_adjacency::iterator const end_it = adjacency.end();
	for ( vertex_adjacency::iterator it = adjacency.begin(); it != end_it; ++it )
	{
		R_ASSERT( dict[ *it ] != u8( -1 ) );
		*it = dict[ *it ];
	}
}

void convex::build_original_to_new_adjacency_dict( std::vector<u8> const& plane_flags, buffer_vector<u8>& dict )
{
	dict.resize( msc_max_plane_count );
	for ( u32 i = 0; i < msc_max_plane_count; ++i )
		dict[ i ] = u8( -1 );
	for ( u32 i = 0; i < plane_flags.size(); ++i )
		dict[ plane_flags[ i ] ] = static_cast<u8>( i );
}

intersection convex::test_inexact( aabb const& aabb) const
{
	u32 inside_count				= 0;
	planes_type::const_iterator const end_it = m_planes.end();
	for ( planes_type::const_iterator it = m_planes.begin(); it != end_it; ++it )
	{
		switch ( it->test( aabb ) )
		{
			case intersection_outside	:
				return				intersection_outside;
			case intersection_intersect :
				continue;
			case intersection_inside	: {
				++inside_count;
				continue;
			}
			default	:		NODEFAULT();
		}
	}

	if ( inside_count < m_planes.size() )
		return	intersection_intersect;

	R_ASSERT	( inside_count == m_planes.size() );
	return		intersection_inside;
}

void convex::fill_cache	( cache& c ) const
{
	c.edges.clear();
	c.vertices.clear();
	c.bounding_radius = 0.0f;
	if ( this->empty() )
		return;
	typedef std::set<cache::edge, edge_less> edge_set_type;
	edge_set_type edges;
	u32 const vertex_count = m_adjacencies.size();
	for ( u32 i = 0; i < vertex_count; ++i )
	{
		vertex_adjacency const& adjacency = m_adjacencies[ i ];
		u32 const first		= adjacency[ 0 ];
		u32 const second	= adjacency[ 1 ];
		u32 const third		= adjacency[ 2 ];
		R_ASSERT( first != second && first != third && second != third );
		float3 intersection;
		bool success = three_planes_intersection( m_planes[ first ].plane, m_planes[ second ].plane, m_planes[ third ].plane, intersection );
		R_ASSERT( success );
		c.vertices.push_back( intersection );
		for ( u32 j = 0; j < vertex_count; ++j )
		{
			if ( i == j )
				continue;
			vertex_adjacency const& other = m_adjacencies[ j ];
			u32 shared_count = 0;
			vertex_adjacency::const_iterator const adjacency_end_it = adjacency.end();
			for ( vertex_adjacency::const_iterator at = adjacency.begin(); at != adjacency_end_it; ++at )
				if ( std::find( other.begin(), other.end(), *at ) != other.end() )
					++shared_count;
			if ( shared_count < 2 )//fewer than 2 shared planes is not an edge
				continue;
			edges.insert( std::make_pair( std::min( i, j ), std::max( i, j ) ) );
		}
	}
	c.edges.reserve( edges.size() );
	c.edges.insert( c.edges.end(), edges.begin(), edges.end() );
	math::float3 center_point = std::accumulate( c.vertices.begin(), c.vertices.end(), math::float3( 0.f, 0.f, 0.f ) ) / static_cast<float>( c.vertices.size() );
	cache::vertices_type::const_iterator const end_it = c.vertices.end();
	for ( cache::vertices_type::const_iterator it = c.vertices.begin(); it != end_it; ++it )
	{
		float const sqr_len = ( *it - center_point ).squared_length();
		if ( sqr_len > c.bounding_radius )
			c.bounding_radius = sqr_len;
	}
	c.bounding_radius = math::sqrt( c.bounding_radius );
}

bool convex::inside( float3 const& point ) const
{
	R_ASSERT( !m_planes.empty() );
	planes_type::const_iterator const end_it = m_planes.end();
	for ( planes_type::const_iterator it = m_planes.begin(); it != end_it; ++it )
	{
		float const dist = it->plane.classify( point );
		if ( !math::is_zero( dist ) && dist > 0.f )
			return false;
	}
	return true;
}

