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
	effect_complex_post_process_blend( ) { }
	virtual ~effect_complex_post_process_blend( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override
	{
		// FUNCTION BODY[0x83df0] for <false, false, false>
		// claude@NOTE: legacy use_bokeh_dof/use_bokeh_image members mapped to t_option_0/1;
		// t_option_2's shipped meaning is unrecovered (only <false,false,false> instantiated)
		VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

		shader_configuration configuration;
		configuration.use_bokeh_dof		= t_option_0;
		configuration.use_bokeh_image	= t_option_1;

		compiler.begin_technique();
			compiler.begin_pass("copy_image", NULL, "complex_post_process_blend", configuration, NULL);
				compiler.set_texture("t_blurred_frame_color", r2_rt_blur0, 0, false, 0); // set here?
				compiler.set_texture("t_frame_color", r2_rt_generic0, 0, false, 0);		// set here?
				compiler.set_texture("t_position", r2_rt_p, 0, false, 0);				// set here?
				compiler.set_texture("t_frame_luminance", r2_rt_frame_luminance, 0, false, 0);
				compiler.set_texture("t_frame_luminance_previous", r2_rt_frame_luminance_previous, 0, false, 0);
				compiler.set_texture("t_frame_luminance_histogram", r2_rt_frame_luminance_histogram, 0, false, 0);
				compiler.set_texture("t_light_scattering", r2_rt_light_scattering, 0, false, 0);
				compiler.set_texture("t_sphere_falloff", "fx/sphere_falloff", 0, true, 0);
				compiler.set_texture("t_bokeh_image", "fx/bokeh_image", 0, true, 0);
				//compiler.set_texture("t_skin_position", r2_rt_skin_scattering_position);
				//compiler.set_texture("t_skin_scattering", r2_rt_skin_scattering);
				//compiler.set_texture("t_skin_scattering_temp", r2_rt_skin_scattering_temp);
				//compiler.set_texture("t_skin_scattering_blurred_0", r2_rt_skin_scattering_blurred_0);
				//compiler.set_texture("t_skin_scattering_blurred_1", r2_rt_skin_scattering_blurred_1);
				//compiler.set_texture("t_mlaa_edges", r2_rt_mlaa_edges);
				//compiler.set_texture("t_mlaa_blend", r2_rt_mlaa_blended_weights);
				//compiler.set_depth(true, false);
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
