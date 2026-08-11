#ifndef VOSTOK_RENDER_ENGINE_EFFECT_OLTA_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_OLTA_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_olta : public effect_descriptor {
public:
	enum {
		blend_pass,
		blur_pass
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

};

STATIC_SIZE_ASSERT( effect_olta, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_OLTA_H_INCLUDED
