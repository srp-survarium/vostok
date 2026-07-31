#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_LINE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_LINE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_system_line : public effect_descriptor {
public:
	enum techniques{ z_enabled, z_disabled };

	effect_system_line( ) { }
	virtual ~effect_system_line( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_system_line, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SYSTEM_LINE_H_INCLUDED
