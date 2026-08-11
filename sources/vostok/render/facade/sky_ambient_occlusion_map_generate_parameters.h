#ifndef VOSTOK_RENDER_FACADE_SKY_AMBIENT_OCCLUSION_MAP_GENERATE_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SKY_AMBIENT_OCCLUSION_MAP_GENERATE_PARAMETERS_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_float3.h>
#include <vostok/math_int2.h>

namespace vostok {
namespace render {

struct sky_ambient_occlusion_map_generate_parameters {
	fixed_string<260>	texture_name;
	math::int2			resolution;
	float3				location;
	float3				sizes;
};

STATIC_SIZE_ASSERT( sky_ambient_occlusion_map_generate_parameters, 0x130 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SKY_AMBIENT_OCCLUSION_MAP_GENERATE_PARAMETERS_H_INCLUDED
