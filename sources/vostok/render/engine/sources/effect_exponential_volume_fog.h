#ifndef VOSTOK_RENDER_ENGINE_EFFECT_EXPONENTIAL_VOLUME_FOG_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_EXPONENTIAL_VOLUME_FOG_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_exponential_volume_fog : public effect_material_base {
public:
	effect_exponential_volume_fog( ) { }
	virtual ~effect_exponential_volume_fog( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_exponential_volume_fog, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_EXPONENTIAL_VOLUME_FOG_H_INCLUDED
