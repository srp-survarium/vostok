#include "pch.h"
#include "stage_particles.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>

namespace vostok {
namespace render {

void effect_resolve_particles::compile(
	effect_compiler&,
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x68950]
	// claude@NOTE: no legacy ancestor - particle resolve effect postdates the legacy corpus
}

particle_shader_constants::particle_shader_constants( ) :
	m_right_view_vector				( 0 ),
	m_up_view_vector				( 0 ),
	m_use_align_by_dir				( 0 ),
	m_view_location					( 0 ),
	m_current_time					( 0 ),
	m_use_fixed_axis				( 0 ),
	m_rotation_fixed_axis			( 0 ),
	m_locked_no_ratate_axis_index	( 0 )
{
	// FUNCTION BODY[0x641d80]
	m_right_view_vector				= backend::ref().register_constant_host( "right_view_vector", rc_float );
	m_up_view_vector				= backend::ref().register_constant_host( "up_view_vector", rc_float );
	m_view_location					= backend::ref().register_constant_host( "view_location", rc_float );
	m_rotation_fixed_axis			= backend::ref().register_constant_host( "rotation_fixed_axis", rc_float );

	m_current_time					= backend::ref().register_constant_host( "current_time", rc_float );

	m_use_align_by_dir				= backend::ref().register_constant_host( "use_align_by_dir", rc_float );
	m_use_fixed_axis				= backend::ref().register_constant_host( "use_fixed_axis", rc_float );
	m_locked_no_ratate_axis_index	= backend::ref().register_constant_host( "locked_no_ratate_axis_index", rc_float );

}

void particle_shader_constants::set_time( float time )
{
	// FUNCTION BODY[0x641d30]
	backend::ref().set_ps_constant ( m_current_time, time);
}

void particle_shader_constants::set(
	float3 const up_vector,
	float3 const right_vector,
	float3 const view_location,
	particle::enum_particle_locked_axis locked_axis,
	particle::enum_particle_screen_alignment screen_alignment
)
{
	// FUNCTION BODY[0x641a70]
	backend::ref().set_vs_constant ( m_right_view_vector, right_vector);
	backend::ref().set_vs_constant ( m_up_view_vector, up_vector);
	backend::ref().set_vs_constant ( m_view_location, view_location);

	float f_use_align_by_dir			= 0.0f;
	float f_use_fixed_axis				= 0.0f;
	float f_locked_no_ratate_axis_index = -1.0f;

	float3 rotation_fixed_axis	(0,0,0);

	if (screen_alignment==particle::particle_screen_alignment_to_axis)
	{
		bool const is_rotate_x = locked_axis==particle::particle_locked_axis_rotate_x;
		bool const is_rotate_y = locked_axis==particle::particle_locked_axis_rotate_y;
		bool const is_rotate_z = locked_axis==particle::particle_locked_axis_rotate_z;

		rotation_fixed_axis = float3(
			is_rotate_x,
			is_rotate_y,
			is_rotate_z
		);

		if (is_rotate_x || is_rotate_y || is_rotate_z)
		{
			f_use_fixed_axis = 1.0f;
		}
		else
		{
			f_locked_no_ratate_axis_index = float(locked_axis);
		}
	}
	else
	{
		f_use_align_by_dir = (screen_alignment==particle::particle_screen_alignment_to_path) ? 1.0f : 0.0f;
	}

	backend::ref().set_vs_constant ( m_rotation_fixed_axis, rotation_fixed_axis);
	backend::ref().set_vs_constant ( m_locked_no_ratate_axis_index, f_locked_no_ratate_axis_index);
	backend::ref().set_vs_constant ( m_use_align_by_dir, f_use_align_by_dir);
	backend::ref().set_vs_constant ( m_use_fixed_axis, f_use_fixed_axis);
}

stage_particles::stage_particles(
	renderer* in_renderer,
	renderer_context* context
) :
	stage					( in_renderer, context ),
	m_particles_initialized	( false )
{
	// FUNCTION BODY[0x642600]
	m_enabled						= options::ref().current.m_enabled_particles_stage;
}

stage_particles::~stage_particles( )
{
	// FUNCTION BODY[0x642060]
}

bool stage_particles::is_effects_ready( ) const
{
	// claude@NOTE: no legacy ancestor - absent from the legacy stage_particles (only execute survives there); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x641a60]
	return false;
}

void stage_particles::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x642130]
	// claude@NOTE: legacy execute's emitter loop depends on render_particle_emitter_instance /
	// material_effects wiring plus the new resolve/lighting targets - legacy body kept in
	// temp/render_legacy remainder, matcher-phase
}

} // namespace render
} // namespace vostok
