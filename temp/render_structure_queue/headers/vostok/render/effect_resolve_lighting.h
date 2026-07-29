////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_RESOLVE_LIGHTING_H_INCLUDED
#define RENDER_EFFECT_RESOLVE_LIGHTING_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_resolve_lighting : public effect_descriptor {
public:
	virtual	void	compile					( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_resolve_lighting	( ) { /* no source */ }
	virtual			~effect_resolve_lighting( ) { /* no source */ }
}; // class effect_resolve_lighting

STATIC_SIZE_ASSERT(effect_resolve_lighting, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_RESOLVE_LIGHTING_H_INCLUDED
