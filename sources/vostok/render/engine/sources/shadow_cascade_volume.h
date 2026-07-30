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
		// STATE[STUB]
		// FUNCTION BODY[0x76a20]
	}

	void compute_caster_model_fixed(
		cascade_volume_planes&,
		float3&,
		float,
		bool
	)
	{
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
