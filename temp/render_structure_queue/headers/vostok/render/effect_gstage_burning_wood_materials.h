////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_GSTAGE_BURNING_WOOD_MATERIALS_H_INCLUDED
#define RENDER_EFFECT_GSTAGE_BURNING_WOOD_MATERIALS_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_gstage_burning_wood_materials : public effect_material_base {
public:
	virtual	void	compile	( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_gstage_burning_wood_materials( ) { /* no source */ }
	virtual			~effect_gstage_burning_wood_materials( ) { /* no source */ }
}; // class effect_gstage_burning_wood_materials

STATIC_SIZE_ASSERT(effect_gstage_burning_wood_materials, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_GSTAGE_BURNING_WOOD_MATERIALS_H_INCLUDED
