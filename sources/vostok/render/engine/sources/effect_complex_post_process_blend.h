#ifndef VOSTOK_RENDER_ENGINE_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/dx11/effect_compiler.h>

#include "shared_names.h"

namespace vostok {
namespace render {

class custom_config_value;

template <
	bool t_option_0,
	bool t_option_1,
	bool t_option_2
>
class effect_complex_post_process_blend : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override
	{
		shader_configuration configuration;
		configuration.use_bokeh_dof		= t_option_0;
		configuration.use_bokeh_image	= t_option_1;
		configuration.use_image_grain	= t_option_2;

		compiler.begin_technique();
			compiler.begin_pass("copy_image", NULL, "complex_post_process_blend", configuration, NULL);
				compiler.set_depth(false, false);

				compiler.set_texture("t_blurred_frame_color", r2_rt_blur0, 0, false, u32(-1));
				compiler.set_texture("t_blurred_frame_bloom_only_color", "$user$blur2", 0, false, u32(-1));
				compiler.set_texture("t_blurred_frame_bloom_only_color2", "$user$blur3", 0, false, u32(-1));
				compiler.set_texture("t_frame_color", r2_rt_generic0, 0, false, u32(-1));
				compiler.set_texture("t_position", r2_rt_p, 0, false, u32(-1));
				// 3 target lines are likely retail-compiled-out source.
				compiler.set_texture("t_frame_luminance", r2_rt_frame_luminance, 0, false, u32(-1));


				compiler.set_texture("t_light_scattering_mask", "$user$light_scattering_mask", 0, false, u32(-1));
				compiler.set_texture("t_light_scattering_result", "$user$light_scattering_result", 0, false, u32(-1));
				compiler.set_texture("t_sphere_falloff", "fx/sphere_falloff", 0, false, u32(-1));
				compiler.set_texture("t_bokeh_image", "fx/bokeh_image", 0, false, u32(-1));


				compiler.set_texture("t_lighting_buffer", r2_rt_accum_diffuse, 0, false, u32(-1));

				compiler.set_texture("t_lens_flares", "$user$lens_flares", 0, false, u32(-1));

				if (configuration.use_image_grain)
					compiler.set_texture("t_grain_noise", "engine/noise_64x64", 0, false, u32(-1));

				compiler.set_texture("t_hiz_depth", "$user$hiz_occlusion_depth_mips", 0, false, u32(-1));
			// 19 target lines are likely retail-compiled-out source.
			compiler.end_pass();
		compiler.end_technique();
	}

};

typedef effect_complex_post_process_blend<
	false,
	false,
	false
> effect_complex_post_process_blend_000;

STATIC_SIZE_ASSERT( effect_complex_post_process_blend_000, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED
