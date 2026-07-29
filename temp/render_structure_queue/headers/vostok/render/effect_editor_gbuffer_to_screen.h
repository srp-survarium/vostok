////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_EDITOR_GBUFFER_TO_SCREEN_H_INCLUDED
#define RENDER_EFFECT_EDITOR_GBUFFER_TO_SCREEN_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_editor_gbuffer_to_screen : public effect_descriptor {
public:
	virtual	void	compile							( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_editor_gbuffer_to_screen	( ) { /* no source */ }
	virtual			~effect_editor_gbuffer_to_screen( ) { /* no source */ }
}; // class effect_editor_gbuffer_to_screen

STATIC_SIZE_ASSERT(effect_editor_gbuffer_to_screen, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_EDITOR_GBUFFER_TO_SCREEN_H_INCLUDED
