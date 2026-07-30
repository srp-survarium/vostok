#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_sky_ambient_occlusion : public effect_descriptor {
public:
	effect_sky_ambient_occlusion( ) { }
	virtual ~effect_sky_ambient_occlusion( ) { }

	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override
	{
		// STATE[STUB]
		// FUNCTION BODY[0x740a0]
	}
};

STATIC_SIZE_ASSERT( effect_sky_ambient_occlusion, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
