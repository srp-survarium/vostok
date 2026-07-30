#ifndef VOSTOK_RENDER_ENGINE_EFFECT_GOD_RAYS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_GOD_RAYS_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_god_rays : public effect_descriptor {
public:
	effect_god_rays( ) { }
	virtual ~effect_god_rays( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_god_rays, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_GOD_RAYS_H_INCLUDED
