#ifndef VOSTOK_RENDER_FACADE_CLOUD_KEY_INLINE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_CLOUD_KEY_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline cloud_key_parameters::cloud_key_parameters( )
{
	cloud_generate_octaves = -1.0f;
	cloud_generate_cloudiness = 0.5f;
	direct_light = -1.0f;
	indirect_light = -1.0f;
	ambient = -1.0f;
	extinction = -1.0f;
	diffusivity = 0.0f;
	cloud_base = 3200.0f;
	layer_height = 7.0f;
	detail_noise_wave_lenght = -1.0f;
	detail_noise_amplitude = -1.0f;
	wind_speed = -1.0f;
	persistence = -1.0f;
	interp_alpha = 0.0f;
	source_key_index = 0;
	target_key_index = 1;
	linear_time = 0.0f;
}

inline void cloud_key_parameters::load( configs::binary_config_value const& )
{
}

inline cloud_key_parameters cloud_key_parameters::lerp(
	cloud_key_parameters const& left,
	cloud_key_parameters const& right,
	float const alpha
)
{
	cloud_key_parameters result = left;
	float const reverse_alpha = 1.0f - alpha;

	result.cloud_base				= left.cloud_base * reverse_alpha + right.cloud_base * alpha;
	result.layer_height				= left.layer_height * reverse_alpha + right.layer_height * alpha;
	result.detail_noise_wave_lenght	= left.detail_noise_wave_lenght * reverse_alpha + right.detail_noise_wave_lenght * alpha;
	result.detail_noise_amplitude	= left.detail_noise_amplitude * reverse_alpha + right.detail_noise_amplitude * alpha;
	result.wind_speed				= left.wind_speed * reverse_alpha + right.wind_speed * alpha;
	result.direct_light				= left.direct_light * reverse_alpha + right.direct_light * alpha;
	result.indirect_light				= left.indirect_light * reverse_alpha + right.indirect_light * alpha;
	result.ambient					= left.ambient * reverse_alpha + right.ambient * alpha;

	return result;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_CLOUD_KEY_INLINE_H_INCLUDED
