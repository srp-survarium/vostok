////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_PARTICLES_H_INCLUDED
#define RENDER_STAGE_PARTICLES_H_INCLUDED

/* INCLUDES */
class vostok::render::stage;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;
class vostok::render::res_geometry;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_particles : public stage {
public:
					stage_particles	( renderer* in_renderer, renderer_context* context );
	virtual			~stage_particles( );

			bool	is_effects_ready( ) const;

	virtual	void	execute			( ) override;

	/* 0x0000 */	/* stage */
private:
	/* 0x0010 */	bool				m_particles_initialized;
	/* 0x0014 */	res_effect_ptr		m_sh_particle_sprite;
	/* 0x0018 */	res_effect_ptr		m_sh_particle_beamtrail;
	/* 0x001c */	res_effect_ptr		m_resolve_particles_effect;
	/* 0x0020 */	res_geometry_ptr	m_g_particle_sprite;
	/* 0x0024 */	res_geometry_ptr	m_g_subuv_particle_sprite;
	/* 0x0028 */	res_geometry_ptr	m_g_particle_beamtrail;
}; // class stage_particles

STATIC_SIZE_ASSERT(stage_particles, 0x2C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_PARTICLES_H_INCLUDED
