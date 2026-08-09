#ifndef VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_SKIN_IRRADIANCE_TEXTURE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_SKIN_IRRADIANCE_TEXTURE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_downsample_skin_irradiance_texture : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

	effect_downsample_skin_irradiance_texture( ) { }
	virtual ~effect_downsample_skin_irradiance_texture( ) { }
};

STATIC_SIZE_ASSERT( effect_downsample_skin_irradiance_texture, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_DOWNSAMPLE_SKIN_IRRADIANCE_TEXTURE_H_INCLUDED
