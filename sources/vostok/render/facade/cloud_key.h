// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_CLOUD_KEY_H_INCLUDED
#define VOSTOK_RENDER_FACADE_CLOUD_KEY_H_INCLUDED
namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

struct cloud_key_parameters {
	typedef configs::binary_config_value ConfigType;

	inline void load( configs::binary_config_value const& );

	inline cloud_key_parameters( );

	float	cloud_generate_octaves;
	float	cloud_generate_cloudiness;
	float	direct_light;
	float	indirect_light;
	float	ambient;
	float	extinction;
	float	diffusivity;
	float	cloud_base;
	float	layer_height;
	float	detail_noise_wave_lenght;
	float	detail_noise_amplitude;
	float	wind_speed;
	float	persistence;
	float	interp_alpha;
	u32		source_key_index;
	u32		target_key_index;
	float	linear_time;

	static inline cloud_key_parameters lerp(
		cloud_key_parameters const& left,
		cloud_key_parameters const& right,
		float const alpha
	);
};

STATIC_SIZE_ASSERT( cloud_key_parameters, 0x44 );

} // namespace render
} // namespace vostok

#include <vostok/render/facade/cloud_key_inline.h>

#endif // #ifndef VOSTOK_RENDER_FACADE_CLOUD_KEY_H_INCLUDED
