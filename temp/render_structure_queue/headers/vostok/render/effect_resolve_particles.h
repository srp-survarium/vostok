////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_RESOLVE_PARTICLES_H_INCLUDED
#define RENDER_EFFECT_RESOLVE_PARTICLES_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_resolve_particles : public effect_descriptor {
public:
	virtual	void	compile						( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_resolve_particles	( ) { /* no source */ }
	virtual			~effect_resolve_particles	( ) { /* no source */ }
}; // class effect_resolve_particles

STATIC_SIZE_ASSERT(effect_resolve_particles, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_RESOLVE_PARTICLES_H_INCLUDED
