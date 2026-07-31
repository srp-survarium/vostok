#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_COLORED_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_COLORED_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_system_colored : public effect_descriptor {
public:
	enum techniques{ solid, stenciled, voided, cover };

	effect_system_colored( ) { }
	virtual ~effect_system_colored( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_system_colored, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_COLORED_H_INCLUDED
