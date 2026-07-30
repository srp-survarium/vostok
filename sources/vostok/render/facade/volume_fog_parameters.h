#ifndef VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED

#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

struct volume_fog_parameters {
	volume_fog_parameters( ) { /* no source */ }

	float4x4	transform;
	float3		fog_color;
	float2		direction;
	float		height_falloff_offset;
	float		density;
	float		speed;
	float		noise_scale;
	float		wave_length;
	float		near_density;
	float		transparency_multiplier;
	float		density_offset;
};

STATIC_SIZE_ASSERT( volume_fog_parameters, 0x74 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED
