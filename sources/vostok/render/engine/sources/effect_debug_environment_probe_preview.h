#ifndef VOSTOK_RENDER_ENGINE_EFFECT_DEBUG_ENVIRONMENT_PROBE_PREVIEW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_DEBUG_ENVIRONMENT_PROBE_PREVIEW_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_debug_environment_probe_preview : public effect_descriptor {
public:
	effect_debug_environment_probe_preview( ) { }
	virtual ~effect_debug_environment_probe_preview( ) { }

	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override;
};

STATIC_SIZE_ASSERT( effect_debug_environment_probe_preview, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_DEBUG_ENVIRONMENT_PROBE_PREVIEW_H_INCLUDED
