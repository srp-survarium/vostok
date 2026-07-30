#ifndef VOSTOK_RENDER_ENGINE_SPOT_LIGHT_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPOT_LIGHT_EFFECT_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template < bool t_shadowed >
class spot_light_effect : public effect_descriptor {
	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x7b6a0] for <false>
	}

public:
	spot_light_effect( ) { }
	virtual ~spot_light_effect( ) { }
};

typedef spot_light_effect< false > spot_light_effect_unshadowed;

STATIC_SIZE_ASSERT( spot_light_effect_unshadowed, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SPOT_LIGHT_EFFECT_H_INCLUDED
