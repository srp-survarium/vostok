#ifndef VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_GBUFFER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_GBUFFER_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_downsample_gbuffer : public effect_descriptor {
public:

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_downsample_gbuffer, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_GBUFFER_H_INCLUDED
