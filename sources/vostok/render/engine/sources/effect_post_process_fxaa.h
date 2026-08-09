#ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_FXAA_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_FXAA_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_post_process_fxaa : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

	effect_post_process_fxaa( ) { }
	virtual ~effect_post_process_fxaa( ) { }
};

STATIC_SIZE_ASSERT( effect_post_process_fxaa, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_FXAA_H_INCLUDED
