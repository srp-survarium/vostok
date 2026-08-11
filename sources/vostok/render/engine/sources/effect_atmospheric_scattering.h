#ifndef VOSTOK_RENDER_ENGINE_EFFECT_ATMOSPHERIC_SCATTERING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_ATMOSPHERIC_SCATTERING_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_atmospheric_scattering : public effect_descriptor {
public:
	enum {
		make_mie_rayleigh_pass,
		atmosphere_pass,
		clouds_pass_alpha_blend,
		clouds_pass_modulate,
		sun_moon_pass_for_alpha_blend_clouds,
		sun_moon_pass_for_modulate_clouds,
		scattering_on_geometry_pass_mul,
		scattering_on_geometry_pass_add
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_atmospheric_scattering, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_ATMOSPHERIC_SCATTERING_H_INCLUDED
