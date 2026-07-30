#ifndef VOSTOK_RENDER_ENGINE_STAGE_PARTICLES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_PARTICLES_H_INCLUDED

#include <vostok/math_float3.h>
#include <vostok/particle/particle_data_type.h>
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/res_effect.h>

#include "res_geometry.h"
#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class effect_resolve_particles : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& custom_config
	) override;
};

STATIC_SIZE_ASSERT( effect_resolve_particles, 0x4 );

class particle_shader_constants :
	public quasi_singleton<particle_shader_constants>
{
public:
	particle_shader_constants( );
	~particle_shader_constants( ) { }

	void set_time( float time );
	void set(
		float3 up_vector,
		float3 right_vector,
		float3 view_location,
		particle::enum_particle_locked_axis locked_axis,
		particle::enum_particle_screen_alignment screen_alignment
	);

private:
	shader_constant_host*	m_right_view_vector;
	shader_constant_host*	m_up_view_vector;
	shader_constant_host*	m_use_align_by_dir;
	shader_constant_host*	m_view_location;
	shader_constant_host*	m_current_time;
	shader_constant_host*	m_use_fixed_axis;
	shader_constant_host*	m_rotation_fixed_axis;
	shader_constant_host*	m_locked_no_ratate_axis_index;
};

STATIC_SIZE_ASSERT( particle_shader_constants, 0x20 );

class stage_particles : public stage {
public:
	stage_particles( renderer* in_renderer, renderer_context* context );
	virtual ~stage_particles( );

	bool is_effects_ready( ) const;
	virtual void execute( ) override;

private:
	bool				m_particles_initialized;
	res_effect_ptr		m_sh_particle_sprite;
	res_effect_ptr		m_sh_particle_beamtrail;
	res_effect_ptr		m_resolve_particles_effect;
	res_geometry_ptr	m_g_particle_sprite;
	res_geometry_ptr	m_g_subuv_particle_sprite;
	res_geometry_ptr	m_g_particle_beamtrail;
};

STATIC_SIZE_ASSERT( stage_particles, 0x2C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_PARTICLES_H_INCLUDED
