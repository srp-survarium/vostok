#ifndef VOSTOK_RENDER_ENGINE_SPHERE_LIGHT_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPHERE_LIGHT_EFFECT_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template < bool t_shadowed >
class sphere_light_effect : public effect_descriptor {
	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x7c1f0] for <false>
	}

public:
	sphere_light_effect( ) { }
	virtual ~sphere_light_effect( ) { }
};

typedef sphere_light_effect< false > sphere_light_effect_unshadowed;

STATIC_SIZE_ASSERT( sphere_light_effect_unshadowed, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SPHERE_LIGHT_EFFECT_H_INCLUDED
