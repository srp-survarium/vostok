#ifndef VOSTOK_RENDER_ENGINE_EFFECT_ATMOSPHERIC_SCATTERING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_ATMOSPHERIC_SCATTERING_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_atmospheric_scattering : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

	effect_atmospheric_scattering( ) { }
	virtual ~effect_atmospheric_scattering( ) { }
};

STATIC_SIZE_ASSERT( effect_atmospheric_scattering, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_ATMOSPHERIC_SCATTERING_H_INCLUDED
