////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_EDITOR_MODEL_GHOST_H_INCLUDED
#define RENDER_EFFECT_EDITOR_MODEL_GHOST_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_editor_model_ghost : public effect_material_base {
public:
	virtual	void	compile						( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_editor_model_ghost	( ) { /* no source */ }
	virtual			~effect_editor_model_ghost	( ) { /* no source */ }
}; // class effect_editor_model_ghost

STATIC_SIZE_ASSERT(effect_editor_model_ghost, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_EDITOR_MODEL_GHOST_H_INCLUDED
