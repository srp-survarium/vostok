////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKY_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
#define RENDER_SKY_DEFAULT_MATERIAL_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class sky_default_material_effect : public effect_material_base {
public:
	virtual	void	compile						( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			sky_default_material_effect	( ) { /* no source */ }
	virtual			~sky_default_material_effect( ) { /* no source */ }
}; // class sky_default_material_effect

STATIC_SIZE_ASSERT(sky_default_material_effect, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKY_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
