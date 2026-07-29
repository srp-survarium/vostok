////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_DEBUG_EDITOR_WIREFRAME_H_INCLUDED
#define RENDER_EFFECT_DEBUG_EDITOR_WIREFRAME_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_debug_editor_wireframe : public effect_material_base {
public:
	virtual	void	compile							( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_debug_editor_wireframe	( ) { /* no source */ }
	virtual			~effect_debug_editor_wireframe	( ) { /* no source */ }
}; // class effect_debug_editor_wireframe

STATIC_SIZE_ASSERT(effect_debug_editor_wireframe, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_DEBUG_EDITOR_WIREFRAME_H_INCLUDED
