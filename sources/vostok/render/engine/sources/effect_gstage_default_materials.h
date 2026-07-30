#ifndef VOSTOK_RENDER_ENGINE_EFFECT_GSTAGE_DEFAULT_MATERIALS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_GSTAGE_DEFAULT_MATERIALS_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_gstage_default_materials : public effect_material_base {
public:
	effect_gstage_default_materials( ) { }
	virtual ~effect_gstage_default_materials( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_gstage_default_materials, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_GSTAGE_DEFAULT_MATERIALS_H_INCLUDED
