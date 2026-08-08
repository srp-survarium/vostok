////////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2009
//	Author		: Dmitriy Iassenev
//	Description	: a cuboid is a solid figure bounded by six faces, forming a convex polyhedron
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MATH_CUBOID_H_INCLUDED
#define VOSTOK_MATH_CUBOID_H_INCLUDED
	
#include <vostok/math_aabb_plane.h>

namespace vostok {
namespace math {

class VOSTOK_CORE_API cuboid {
public:
	enum {
		plane_count	= 6,
	}; // enum

	typedef aabb_plane const (planes_type)[plane_count];

public:
	inline					cuboid		( ) { }
			explicit		cuboid		( plane const (&planes)[plane_count] );
							cuboid		( aabb const& aabb, float4x4 const& matrix );
							cuboid		( cuboid const& other, float4x4 const& matrix );
			intersection	test_inexact( aabb const& aabb) const;
//.			intersection	test		( aabb const& aabb) const; not implemented
			intersection	test		( sphere const& sphere ) const;
	inline	planes_type const& planes	( ) const { return m_planes; }

protected:
	aabb_plane		m_planes[ plane_count ];
}; // class cuboid

inline bool is_point_inside_cuboid( float3 const& p, cuboid const& c )
{
	aabb_plane const* const plane_end = c.planes( ) + cuboid::plane_count;

	for ( aabb_plane const* plane_it = c.planes( ); plane_it != plane_end; ++plane_it )
	{
		float const distance = plane_it->plane.classify( p );
		if ( !is_zero( distance ) && distance < 0.0f )
			return false;
	}
	return true;
}

} // namespace math
} // namespace vostok

#endif // #ifndef VOSTOK_MATH_CUBOID_H_INCLUDED
