#ifndef VOSTOK_RENDER_ENGINE_EFFECT_MOTION_BLUR_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_MOTION_BLUR_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_motion_blur : public effect_descriptor {
public:
	effect_motion_blur( ) { }
	virtual ~effect_motion_blur( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_motion_blur, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_MOTION_BLUR_H_INCLUDED
