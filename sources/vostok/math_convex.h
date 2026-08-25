////////////////////////////////////////////////////////////////////////////
//	Created		: 25.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef MATH_CONVEX_H_INCLUDED
#define MATH_CONVEX_H_INCLUDED

#include <vostok/math_aabb_plane.h>
#include <vostok/fixed_vector.h>

namespace vostok {
namespace math {


class VOSTOK_CORE_API convex {
public:
	struct cache
	{
		typedef std::vector<math::float3>	vertices_type;
		typedef std::pair<u32, u32>			edge;
		typedef std::vector< edge >			edges_type;
	#pragma warning ( push )
	#pragma warning ( disable : 4251 )
		vertices_type	vertices;
		edges_type		edges;
	#pragma warning ( pop )
		float bounding_radius;
	};
	
	struct plane_similar : public std::binary_function< vostok::math::aabb_plane const&, vostok::math::plane const&, bool >
	{
		plane_similar( float normal_epsilon = vostok::math::epsilon_3, float distance_epsilon = vostok::math::epsilon_3 ):
		normal_e( normal_epsilon ),
		distance_e( distance_epsilon )
		{
		}
		bool operator() ( vostok::math::aabb_plane const& left, vostok::math::plane const& right ) const
		{
			return vostok::math::is_similar( left.plane.normal | right.normal, 1.0f, normal_e ) && vostok::math::is_similar( left.plane.d, right.d, distance_e );
		}
		float normal_e;
		float distance_e;
	};
	enum {
		msc_max_plane_count = 32,
	};
	typedef u8										adjacency;
	typedef fixed_vector<adjacency, 8>				vertex_adjacency;
	typedef std::vector<aabb_plane>					planes_type;
	typedef std::vector<vertex_adjacency>			adjacencies_type;
public:
								convex			( );
	explicit					convex			( aabb const& bbox );
								convex			( planes_type const& planes, adjacencies_type const& adjacency );
	void						split			( plane const& p, cache const& c, convex& positive, convex& negative ) const;
	intersection				test_inexact	( aabb const& aabb) const;
	bool						inside			( math::float3 const& point ) const;
	void						fill_cache		( cache& c ) const;
	bool						empty			( ) const { return m_planes.empty(); }
	planes_type const&			get_planes		( ) const { return m_planes; }
	adjacencies_type const&		get_adjacencies	( ) const { return m_adjacencies; }
private:
	void						sort_adjacencies( );
public:
	// explicit empty dtor pins the retail record position (between
	// sort_adjacencies and the private statics)
								~convex			( ) { }
private:
	typedef u64 vertex_flags;
private:
	static	std::vector<u8>	get_plane_flags							( adjacencies_type const& adjacency_vector );
	static	void			build_original_to_new_adjacency_dict	( std::vector<u8> const& plane_flags, buffer_vector<u8>& dict );
	static	void			convert_original_adjacency_to_new		( buffer_vector<u8> const& dict, vertex_adjacency& adjacency );
private:
#pragma warning ( push )
#pragma warning ( disable : 4251 )
	planes_type			m_planes;
	adjacencies_type	m_adjacencies;
#pragma warning ( pop )
}; // class convex

STATIC_SIZE_ASSERT( convex, 0x18 );

} // namespace math
} // namespace vostok

#endif // #ifndef MATH_CONVEX_H_INCLUDED