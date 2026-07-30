#ifndef VOSTOK_RENDER_ENGINE_PLANE_SPOT_LIGHT_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_PLANE_SPOT_LIGHT_EFFECT_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template < bool t_shadowed >
class plane_spot_light_effect : public effect_descriptor {
	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x7c780] for <false>
	}

public:
	plane_spot_light_effect( ) { }
	virtual ~plane_spot_light_effect( ) { }
};

typedef plane_spot_light_effect< false > plane_spot_light_effect_unshadowed;

STATIC_SIZE_ASSERT( plane_spot_light_effect_unshadowed, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_PLANE_SPOT_LIGHT_EFFECT_H_INCLUDED
