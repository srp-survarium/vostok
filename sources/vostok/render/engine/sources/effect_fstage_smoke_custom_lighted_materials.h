#ifndef VOSTOK_RENDER_ENGINE_EFFECT_FSTAGE_SMOKE_CUSTOM_LIGHTED_MATERIALS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_FSTAGE_SMOKE_CUSTOM_LIGHTED_MATERIALS_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_fstage_smoke_custom_lighted_materials : public effect_material_base {
public:
	effect_fstage_smoke_custom_lighted_materials( ) { }
	virtual ~effect_fstage_smoke_custom_lighted_materials( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_fstage_smoke_custom_lighted_materials, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_FSTAGE_SMOKE_CUSTOM_LIGHTED_MATERIALS_H_INCLUDED
