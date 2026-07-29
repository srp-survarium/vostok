////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_LIGHTING_STAGE_ORGANIC_BASE_MATERIALS_H_INCLUDED
#define RENDER_EFFECT_LIGHTING_STAGE_ORGANIC_BASE_MATERIALS_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_lighting_stage_organic_base_materials : public effect_material_base {
public:
	virtual	void	compile	( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_lighting_stage_organic_base_materials( ) { /* no source */ }
	virtual			~effect_lighting_stage_organic_base_materials( ) { /* no source */ }
}; // class effect_lighting_stage_organic_base_materials

STATIC_SIZE_ASSERT(effect_lighting_stage_organic_base_materials, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_LIGHTING_STAGE_ORGANIC_BASE_MATERIALS_H_INCLUDED
