#ifndef VOSTOK_RENDER_ENGINE_EFFECT_DISTORTION_STAGE_PANNER_MATERIALS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_DISTORTION_STAGE_PANNER_MATERIALS_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_distortion_stage_panner_materials : public effect_material_base {
public:
	effect_distortion_stage_panner_materials( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

};

STATIC_SIZE_ASSERT( effect_distortion_stage_panner_materials, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_DISTORTION_STAGE_PANNER_MATERIALS_H_INCLUDED
