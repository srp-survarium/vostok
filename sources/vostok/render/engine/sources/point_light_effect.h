#ifndef VOSTOK_RENDER_ENGINE_POINT_LIGHT_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_POINT_LIGHT_EFFECT_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template < bool t_shadowed, bool t_shadower >
class point_light_effect : public effect_descriptor {
	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x79c30] for <false, false>
	}

public:
	point_light_effect( ) { }
	virtual ~point_light_effect( ) { }
};

typedef point_light_effect< false, false > point_light_effect_unshadowed;

STATIC_SIZE_ASSERT( point_light_effect_unshadowed, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_POINT_LIGHT_EFFECT_H_INCLUDED
