#ifndef VOSTOK_RENDER_ENGINE_SKY_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SKY_DEFAULT_MATERIAL_EFFECT_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class sky_default_material_effect : public effect_material_base {
public:
	sky_default_material_effect( ) { }
	virtual ~sky_default_material_effect( ) { }

	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override;
};

STATIC_SIZE_ASSERT( sky_default_material_effect, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SKY_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
