#ifndef VOSTOK_RENDER_FACADE_ENVIRONMENT_PROBE_PROPERTIES_H_INCLUDED
#define VOSTOK_RENDER_FACADE_ENVIRONMENT_PROBE_PROPERTIES_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

struct environment_probe_properties {
	fixed_string<260>	texture_name;
	float4x4			transform;
	float3				location;
	float				radius;
	float				diffuse_multiplier;
	float				specular_multiplier;
	u32					preview_mip;
	u32					cubemap_resolution;
	bool				texture_invalidated;
	bool				enabled;
	bool				clip_by_normal;
	bool				with_shadows;
	u32					geometry;
};

STATIC_SIZE_ASSERT( environment_probe_properties, 0x178 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_ENVIRONMENT_PROBE_PROPERTIES_H_INCLUDED
