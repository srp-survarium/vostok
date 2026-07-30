#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SSAO_FILTER4X4_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SSAO_FILTER4X4_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_ssao_filter4x4 : public effect_descriptor {
public:
	effect_ssao_filter4x4( ) { }
	virtual ~effect_ssao_filter4x4( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;
};

STATIC_SIZE_ASSERT( effect_ssao_filter4x4, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SSAO_FILTER4X4_H_INCLUDED
