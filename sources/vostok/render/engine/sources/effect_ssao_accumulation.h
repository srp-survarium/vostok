#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SSAO_ACCUMULATION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SSAO_ACCUMULATION_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_ssao_accumulation : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

	effect_ssao_accumulation( ) { }
	virtual ~effect_ssao_accumulation( ) { }
};

STATIC_SIZE_ASSERT( effect_ssao_accumulation, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SSAO_ACCUMULATION_H_INCLUDED
