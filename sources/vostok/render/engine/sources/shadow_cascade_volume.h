#ifndef VOSTOK_RENDER_ENGINE_SHADOW_CASCADE_VOLUME_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SHADOW_CASCADE_VOLUME_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/math_plane.h>

#include "ray.h"

namespace vostok {
namespace render {

class shadow_cascade_volume {
public:
	typedef fixed_vector<math::plane, 16> cascade_volume_planes;

	struct polygon {
		polygon( )
		{
		}

		s32			points[4];
		math::plane	plane;
	};

public:
	shadow_cascade_volume( )
	{
	}

	~shadow_cascade_volume( )
	{
	}

	void compute_planes( )
	{
		// FUNCTION BODY[0x76a20]
		// claude@NOTE: legacy LIGHT_CUBOIDSIDEPOLYS_COUNT constant adapted to the literal 4
		// (canonical dropped the constants)
		for (u32 it=0; it<4; it++)
		{
			polygon&	poly	=	light_cuboid_polys[it];

			poly.plane = math::create_plane( light_cuboid_points[poly.points[0]], light_cuboid_points[poly.points[2]], light_cuboid_points[poly.points[1]] );

// #if		DEBUG
// 			float3&		p0	= light_cuboid_points[poly.points[0]];
// 			float3&		p1	= light_cuboid_points[poly.points[1]];
// 			float3&		p2	= light_cuboid_points[poly.points[2]];
// 			float3&		p3	= light_cuboid_points[poly.points[3]];
// 			math::plane	p012;	p012.build(p0,p1,p2);
// 			math::plane	p123;	p123.build(p1,p2,p3);
// 			math::plane	p230;	p230.build(p2,p3,p0);
// 			math::plane	p301;	p301.build(p3,p0,p1);
// 			ASSERT	(p012.normal.similar(p123.normal) && p012.normal.similar(p230.normal) && p012.normal.similar(p301.normal));
// #endif
		}
	}

	void compute_caster_model_fixed(
		cascade_volume_planes&,
		float3&,
		float,
		bool
	)
	{
		// claude@NOTE: legacy body diverged - legacy compute_caster_model_fixed carries three retired align_test debug params (7-param form); matcher-phase work.
		// STATE[STUB]
		// FUNCTION BODY[0x76be0]
	}

	bool check_cull_plane_valid( math::plane const&, float&, float )
	{
		return false;
	}

	void translate_light_model( float3 )
	{
	}

	fixed_vector<ray, 8>	view_frustum_rays;
	ray						view_ray;
	ray						light_ray;
	float3					light_cuboid_points[8];
	polygon					light_cuboid_polys[4];
};

STATIC_SIZE_ASSERT( shadow_cascade_volume::polygon, 0x20 );
STATIC_SIZE_ASSERT( shadow_cascade_volume, 0x1D8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SHADOW_CASCADE_VOLUME_H_INCLUDED
