#include "pch.h"
#include "stage_particles.h"

namespace vostok {
namespace render {

void effect_resolve_particles::compile(
	effect_compiler&,
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x68950]
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
	// STATE[STUB]
	// FUNCTION BODY[0x641d80]
}

void particle_shader_constants::set_time( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x641d30]
}

void particle_shader_constants::set(
	float3,
	float3,
	float3,
	particle::enum_particle_locked_axis,
	particle::enum_particle_screen_alignment
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x641a70]
}

stage_particles::stage_particles(
	renderer* in_renderer,
	renderer_context* context
) :
	stage					( in_renderer, context ),
	m_particles_initialized	( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x642600]
}

stage_particles::~stage_particles( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x642060]
}

bool stage_particles::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x641a60]
	return false;
}

void stage_particles::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x642130]
}

} // namespace render
} // namespace vostok
