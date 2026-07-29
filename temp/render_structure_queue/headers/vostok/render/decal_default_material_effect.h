////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DECAL_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
#define RENDER_DECAL_DEFAULT_MATERIAL_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class decal_default_material_effect : public effect_material_base {
public:
	inline	explicit	decal_default_material_effect	( bool arg_0 ) { /* no source */ }

	virtual	void		compile							( effect_compiler& compiler, custom_config_value const& config ) override;

	virtual				~decal_default_material_effect	( ) { /* no source */ }

	/* 0x0000 */	/* effect_material_base */
private:
	/* 0x0004 */	bool	m_is_forward;
}; // class decal_default_material_effect

STATIC_SIZE_ASSERT(decal_default_material_effect, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DECAL_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
