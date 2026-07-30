#ifndef VOSTOK_RENDER_ENGINE_CLOUD_KEY_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CLOUD_KEY_PARAMETERS_H_INCLUDED

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

struct cloud_key_parameters {
	cloud_key_parameters( ) { }

	void load( configs::binary_config_value const& )
	{
		// STATE[STUB]
	}

	static cloud_key_parameters lerp(
		cloud_key_parameters const&,
		cloud_key_parameters const&,
		float
	)
	{
		// STATE[STUB]
		return cloud_key_parameters( );
	}

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
};

STATIC_SIZE_ASSERT( cloud_key_parameters, 0x44 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CLOUD_KEY_PARAMETERS_H_INCLUDED
