////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKY_AMBIENT_OCCLUSION_MAP_GENERATE_PARAMETERS_H_INCLUDED
#define RENDER_SKY_AMBIENT_OCCLUSION_MAP_GENERATE_PARAMETERS_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;
class vostok::math::int2;

namespace vostok {
namespace render {

struct sky_ambient_occlusion_map_generate_parameters {
	inline		sky_ambient_occlusion_map_generate_parameters( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >		texture_name;
	/* 0x0110 */	math::int2				resolution;
	/* 0x0118 */	float3					location;
	/* 0x0124 */	float3					sizes;
}; // struct sky_ambient_occlusion_map_generate_parameters

STATIC_SIZE_ASSERT(sky_ambient_occlusion_map_generate_parameters, 0x130);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKY_AMBIENT_OCCLUSION_MAP_GENERATE_PARAMETERS_H_INCLUDED
