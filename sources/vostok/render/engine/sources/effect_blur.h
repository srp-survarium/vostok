#ifndef VOSTOK_RENDER_ENGINE_EFFECT_BLUR_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_BLUR_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template < u32 t_sample_count >
class effect_blur : public effect_descriptor {
public:
	effect_blur( ) { }
	virtual ~effect_blur( ) { }

	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x82070] for <3>
	}
};

typedef effect_blur< 3 > effect_blur_3;

STATIC_SIZE_ASSERT( effect_blur_3, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_BLUR_H_INCLUDED
