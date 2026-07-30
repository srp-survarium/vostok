#ifndef VOSTOK_RENDER_ENGINE_CONVEX_VOLUME_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CONVEX_VOLUME_H_INCLUDED

#include <vostok/math_plane.h>
#include <vostok/math_sphere.h>

namespace vostok {
namespace render {

class convex_volume {
public:
	enum {
		max_planes = 12
	};

	void clear( ) { count = 0; }

	void add( math::plane& plane );
	void add( float3 const& point0, float3 const& point1, float3 const& point2 );

	void create_from_matrix( float4x4 const& matrix );
	void create_from_planes( math::plane const* planes, u32 count );

	math::intersection test_sphere( float3 const& center, float radius ) const;
	math::intersection test( math::sphere const& sphere ) const
	{
		return test_sphere( sphere.center, sphere.radius );
	}

	math::plane planes[max_planes];
	u32 count;
};

STATIC_SIZE_ASSERT( convex_volume, 0xC4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CONVEX_VOLUME_H_INCLUDED
