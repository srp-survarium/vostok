#ifndef VOSTOK_RENDER_ENGINE_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

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
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x83df0] for <false, false, false>
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
