#ifndef VOSTOK_RENDER_ENGINE_OBB_LIGHT_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_OBB_LIGHT_EFFECT_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template < bool t_shadowed >
class obb_light_effect : public effect_descriptor {
	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x7bc30] for <false>
	}

public:
	obb_light_effect( ) { }
	virtual ~obb_light_effect( ) { }
};

typedef obb_light_effect< false > obb_light_effect_unshadowed;

STATIC_SIZE_ASSERT( obb_light_effect_unshadowed, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_OBB_LIGHT_EFFECT_H_INCLUDED
