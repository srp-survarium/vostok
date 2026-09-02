// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "convex_volume.h"
namespace vostok {
namespace render {

math::intersection convex_volume::test_sphere( float3 const& c, float r ) const
{
	bool outside = false;
	bool inside = true;

	for (u32 i=0; i<count; ++i)
	{
		const math::plane& p = planes[i];

		outside |= p.classify(c) > r+math::epsilon_5;

		if (outside)
			break;

		inside  &= p.classify(c) < -r-math::epsilon_5;
	}

	if (outside)
		return math::intersection_outside;

	if (inside)
		return math::intersection_inside;

	return math::intersection_intersect;
}

void convex_volume::create_from_planes( math::plane const* p, u32 plane_count )
{
	for (u32 k=0; k<plane_count; ++k)
	{
		planes[k] = p[k];
	}

	for (u32 i=0;i<plane_count;i++)
	{
		planes[i].normalize();
		//planes[i].cache	();
	}

	count = plane_count;
}

void convex_volume::create_from_matrix( float4x4 const& mat )
{
	planes[ 0 ].vector.x	= -( mat.e03 + mat.e00 );
	planes[ 0 ].vector.y	= -( mat.e13 + mat.e10 );
	planes[ 0 ].vector.z	= -( mat.e23 + mat.e20 );
	planes[ 0 ].vector.w	= -( mat.e33 + mat.e30 );

	planes[ 1 ].vector.x	= -( mat.e03 - mat.e00 );
	planes[ 1 ].vector.y	= -( mat.e13 - mat.e10 );
	planes[ 1 ].vector.z	= -( mat.e23 - mat.e20 );
	planes[ 1 ].vector.w	= -( mat.e33 - mat.e30 );

	planes[ 2 ].vector.x	= -( mat.e03 - mat.e01 );
	planes[ 2 ].vector.y	= -( mat.e13 - mat.e11 );
	planes[ 2 ].vector.z	= -( mat.e23 - mat.e21 );
	planes[ 2 ].vector.w	= -( mat.e33 - mat.e31 );

	planes[ 3 ].vector.x	= -( mat.e03 + mat.e01 );
	planes[ 3 ].vector.y	= -( mat.e13 + mat.e11 );
	planes[ 3 ].vector.z	= -( mat.e23 + mat.e21 );
	planes[ 3 ].vector.w	= -( mat.e33 + mat.e31 );

	planes[ 4 ].vector.x	= -( mat.e03 - mat.e02 );
	planes[ 4 ].vector.y	= -( mat.e13 - mat.e12 );
	planes[ 4 ].vector.z	= -( mat.e23 - mat.e22 );
	planes[ 4 ].vector.w	= -( mat.e33 - mat.e32 );

	planes[ 5 ].vector.x	= -( mat.e03 + mat.e02 );
	planes[ 5 ].vector.y	= -( mat.e13 + mat.e12 );
	planes[ 5 ].vector.z	= -( mat.e23 + mat.e22 );
	planes[ 5 ].vector.w	= -( mat.e33 + mat.e32 );

	count = 6;

	for (u32 i=0; i<count; ++i)
	{
		planes[i].normalize();
		//planes[i].cache	();
	}
}

} // namespace render
} // namespace vostok
