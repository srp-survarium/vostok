// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_BLUR_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_BLUR_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

class custom_config_value;

template < u32 t_sample_count >
class effect_blur : public effect_descriptor {
public:
	enum {
		horizontally,
		vertically,
		blur_accumulate,
		blur_downsample,
		blur_add_first,
		blur_add
	};

	effect_blur( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override
	{
		VOSTOK_UNREFERENCED_PARAMETERS(custom_config);
		shader_configuration config;
		config.post_process_blur_kernel = t_sample_count;

		compiler.begin_technique();
			compiler.begin_pass("blur", NULL, "blur_horizontally", config, NULL);
				compiler.set_depth( false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(false, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);
			compiler.end_pass();
		compiler.end_technique();

		compiler.begin_technique();
			compiler.begin_pass("blur", NULL, "blur_vertically", config, NULL);
				compiler.set_depth( false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(false, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);
			compiler.end_pass();
		compiler.end_technique();

		compiler.begin_technique();
			compiler.begin_pass("blur", NULL, "blur_accumulate", config, NULL);
				compiler.set_depth(false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE);
			compiler.end_pass();
		compiler.end_technique();

		compiler.begin_technique();
			compiler.begin_pass("blur", NULL, "blur_downsample", config, NULL);
				compiler.set_depth(false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
			compiler.end_pass();
		compiler.end_technique();

		compiler.begin_technique();
			compiler.begin_pass("blur", NULL, "blur_add_first", config, NULL);
				compiler.set_depth(false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE);
			compiler.end_pass();
		compiler.end_technique();

		compiler.begin_technique();
			compiler.begin_pass("blur", NULL, "blur_add", config, NULL);
				compiler.set_depth(false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE);
			compiler.end_pass();
		compiler.end_technique();
	}
};

typedef effect_blur< 3 > effect_blur_3;

STATIC_SIZE_ASSERT( effect_blur_3, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_BLUR_H_INCLUDED
