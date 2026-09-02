// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_CUBOID2_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CUBOID2_H_INCLUDED
#include <vostok/math_aabb.h>
#include <vostok/math_aabb_plane.h>
#include <vostok/math_plane.h>

namespace vostok {
namespace render {

class cuboid2 {
public:
	enum { plane_count = 16 };

	cuboid2( )
	{
	}

	cuboid2( math::plane* planes, u32 const num_planes ) :
		m_num_planes( num_planes )
	{
		for ( u32 i = 0; i < m_num_planes; ++i ) {
			m_planes[i].plane = planes[i];
			m_planes[i].normalize( );
		}
	}

	math::intersection test_inexact( math::aabb const& aabb ) const
	{
		math::intersection result = math::intersection_inside;

		for ( u32 i = 0; i < m_num_planes; ++i ) {
			math::intersection const current = m_planes[i].test( aabb );

			if ( current == math::intersection_outside )
				return current;

			if ( current == math::intersection_intersect )
				result = current;
		}

		return result;
	}

protected:
	math::aabb_plane	m_planes[plane_count];
	u32					m_num_planes;
};

STATIC_SIZE_ASSERT( cuboid2, 0x144 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CUBOID2_H_INCLUDED
