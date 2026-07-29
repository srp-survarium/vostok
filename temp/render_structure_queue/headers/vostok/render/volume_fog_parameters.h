////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VOLUME_FOG_PARAMETERS_H_INCLUDED
#define RENDER_VOLUME_FOG_PARAMETERS_H_INCLUDED

namespace vostok {
namespace render {

struct volume_fog_parameters {
	inline		volume_fog_parameters	( ) { /* no source */ }

	/* 0x0000 */	float4x4	transform;
	/* 0x0040 */	float3		fog_color;
	/* 0x004c */	float2		direction;
	/* 0x0054 */	float		height_falloff_offset;
	/* 0x0058 */	float		density;
	/* 0x005c */	float		speed;
	/* 0x0060 */	float		noise_scale;
	/* 0x0064 */	float		wave_length;
	/* 0x0068 */	float		near_density;
	/* 0x006c */	float		transparency_multiplier;
	/* 0x0070 */	float		density_offset;
}; // struct volume_fog_parameters

STATIC_SIZE_ASSERT(volume_fog_parameters, 0x74);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VOLUME_FOG_PARAMETERS_H_INCLUDED
