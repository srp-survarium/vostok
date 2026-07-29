////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCR_QUAD_EFFECT_H_INCLUDED
#define RENDER_SCR_QUAD_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class scr_quad_effect : public effect_descriptor {
public:
	virtual	void	compile			( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			scr_quad_effect	( ) { /* no source */ }
	virtual			~scr_quad_effect( ) { /* no source */ }
}; // class scr_quad_effect

STATIC_SIZE_ASSERT(scr_quad_effect, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCR_QUAD_EFFECT_H_INCLUDED
