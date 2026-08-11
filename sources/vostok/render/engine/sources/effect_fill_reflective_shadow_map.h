#ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_REFLECTIVE_SHADOW_MAP_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_FILL_REFLECTIVE_SHADOW_MAP_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_fill_reflective_shadow_map : public effect_material_base {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

};

STATIC_SIZE_ASSERT( effect_fill_reflective_shadow_map, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_REFLECTIVE_SHADOW_MAP_H_INCLUDED
