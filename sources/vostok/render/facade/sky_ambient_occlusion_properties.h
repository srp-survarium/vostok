#ifndef VOSTOK_RENDER_FACADE_SKY_AMBIENT_OCCLUSION_PROPERTIES_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SKY_AMBIENT_OCCLUSION_PROPERTIES_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_float3.h>

namespace vostok {
namespace render {

struct sky_ambient_occlusion_properties {
	fixed_string<260>	texture_name;
	float3				location;
	float				width;
	float				height;
	float				depth;
	bool				enabled;
	bool				texture_invalidated;
};

STATIC_SIZE_ASSERT( sky_ambient_occlusion_properties, 0x12C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SKY_AMBIENT_OCCLUSION_PROPERTIES_H_INCLUDED
