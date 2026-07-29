////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOUD_KEY_PARAMETERS_H_INCLUDED
#define RENDER_CLOUD_KEY_PARAMETERS_H_INCLUDED

/* FORWARD REFS */
class vostok::configs::binary_config_value;

namespace vostok {
namespace render {

struct cloud_key_parameters {
	inline	void	load				( configs::binary_config_value const& arg_0 ) { /* no source */ }

	inline			cloud_key_parameters( ) { /* no source */ }

	static	inline	cloud_key_parameters	lerp				(
						cloud_key_parameters const&		arg_0,
						cloud_key_parameters const&		arg_1,
						const float						arg_2
					) { /* no source */ }

	/* 0x0000 */	float		cloud_generate_octaves;
	/* 0x0004 */	float		cloud_generate_cloudiness;
	/* 0x0008 */	float		direct_light;
	/* 0x000c */	float		indirect_light;
	/* 0x0010 */	float		ambient;
	/* 0x0014 */	float		extinction;
	/* 0x0018 */	float		diffusivity;
	/* 0x001c */	float		cloud_base;
	/* 0x0020 */	float		layer_height;
	/* 0x0024 */	float		detail_noise_wave_lenght;
	/* 0x0028 */	float		detail_noise_amplitude;
	/* 0x002c */	float		wind_speed;
	/* 0x0030 */	float		persistence;
	/* 0x0034 */	float		interp_alpha;
	/* 0x0038 */	u32			source_key_index;
	/* 0x003c */	u32			target_key_index;
	/* 0x0040 */	float		linear_time;
}; // struct cloud_key_parameters

STATIC_SIZE_ASSERT(cloud_key_parameters, 0x44);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOUD_KEY_PARAMETERS_H_INCLUDED
