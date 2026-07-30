#ifndef VOSTOK_RENDER_ENGINE_CAPSULE_LIGHT_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CAPSULE_LIGHT_EFFECT_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class capsule_light_effect : public effect_descriptor {
	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x789c0]
	}

public:
	capsule_light_effect( ) { }
	virtual ~capsule_light_effect( ) { }
};

STATIC_SIZE_ASSERT( capsule_light_effect, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CAPSULE_LIGHT_EFFECT_H_INCLUDED
