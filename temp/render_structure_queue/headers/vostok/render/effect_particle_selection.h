////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_PARTICLE_SELECTION_H_INCLUDED
#define RENDER_EFFECT_PARTICLE_SELECTION_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_particle_selection : public effect_material_base {
public:
	virtual	void	compile						( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_particle_selection	( ) { /* no source */ }
	virtual			~effect_particle_selection	( ) { /* no source */ }
}; // class effect_particle_selection

STATIC_SIZE_ASSERT(effect_particle_selection, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_PARTICLE_SELECTION_H_INCLUDED
