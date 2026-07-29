////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_FSTAGE_SMOKE_CUSTOM_LIGHTED_MATERIALS_H_INCLUDED
#define RENDER_EFFECT_FSTAGE_SMOKE_CUSTOM_LIGHTED_MATERIALS_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_fstage_smoke_custom_lighted_materials : public effect_material_base {
public:
	virtual	void	compile	( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_fstage_smoke_custom_lighted_materials( ) { /* no source */ }
	virtual			~effect_fstage_smoke_custom_lighted_materials( ) { /* no source */ }
}; // class effect_fstage_smoke_custom_lighted_materials

STATIC_SIZE_ASSERT(effect_fstage_smoke_custom_lighted_materials, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_FSTAGE_SMOKE_CUSTOM_LIGHTED_MATERIALS_H_INCLUDED
