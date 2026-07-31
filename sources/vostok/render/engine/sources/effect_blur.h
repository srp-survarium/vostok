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
	effect_blur( ) { }
	virtual ~effect_blur( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override
	{
		// FUNCTION BODY[0x82070] for <3>
		VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

		compiler.begin_technique(); // horizontally
			compiler.begin_pass("blur", NULL, "blur_horizontally", shader_configuration(), NULL);
				compiler.set_depth( false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(false, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);
			compiler.end_pass();
		compiler.end_technique();

		compiler.begin_technique(); // vertically
			compiler.begin_pass("blur", NULL, "blur_vertically", shader_configuration(), NULL);
				compiler.set_depth( false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(false, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);
			compiler.end_pass();
		compiler.end_technique();
	}
};

typedef effect_blur< 3 > effect_blur_3;

STATIC_SIZE_ASSERT( effect_blur_3, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_BLUR_H_INCLUDED
