////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_PARTICLE_ENGINE_H_INCLUDED
#define RENDER_SCENE_PARTICLE_ENGINE_H_INCLUDED

/* INCLUDES */
class vostok::render::scene;
struct vostok::particle::engine;
struct vostok::particle::world;
class vostok::particle::base_particle;
class vostok::particle::enum_particle_locked_axis;
class vostok::particle::enum_particle_screen_alignment;
class vostok::render::base_scene;
class vostok::render::scene* const;

/* FORWARD REFS */
class vostok::particle::beamtrail_parameters;
class vostok::particle::billboard_parameters;
class vostok::particle::particle_emitter_instance;
class vostok::particle::render_particle_emitter_instance;

namespace vostok {
namespace render {

struct scene::particle_engine : public particle::engine , public boost::noncopyable {
	inline	explicit			particle_engine					( scene* const arg_0 ) { /* no source */ }

	inline	void				set_particle_world				( particle::world& arg_0 ) { /* no source */ }

	virtual	particle::render_particle_emitter_instance*	create_render_emitter_instance	(
									particle::particle_emitter_instance&	particle_emitter_instance,
									particle::base_particle_list const&		particle_list,
									particle::billboard_parameters*			billboard_parameters,
									particle::beamtrail_parameters*			beamtrail_parameters,
									particle::enum_particle_locked_axis		locked_axis,
									particle::enum_particle_screen_alignment	screen_alignment,
									float4x4 const&							transform,
									float4 const&							instance_color
								) override;

	virtual	void				destroy							( particle::render_particle_emitter_instance*& instance ) override;

	virtual	base_scene_ptr		get_scene						( particle::world& world ) override;

	virtual						~particle_engine				( ) { /* no source */ }

	/* 0x0000 */	/* particle::engine */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	particle::world*	m_particle_world;
private:
	/* 0x0008 */	scene* const		m_scene;
}; // struct scene::particle_engine

STATIC_SIZE_ASSERT(scene::particle_engine, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_PARTICLE_ENGINE_H_INCLUDED
