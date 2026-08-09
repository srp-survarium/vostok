#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SUN_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SUN_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_sun : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&		compiler,
		custom_config_value const&
	) override;

	effect_sun( ) { }
	virtual ~effect_sun( ) { }
};

STATIC_SIZE_ASSERT( effect_sun, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SUN_H_INCLUDED
