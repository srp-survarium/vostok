#include "pch.h"
#include "stage_pre_lighting.h"
#include <vostok/render/core/backend.h>
#include "renderer_context.h"
#include "renderer_context_targets.h"

namespace vostok {
namespace render {

const u32 stencil_light_marker_shift = 2;
void stage_pre_lighting::execute( )
{
	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	m_context->m_light_marker_id = 1 << stencil_light_marker_shift;

	backend::ref( ).reset_depth_stencil_target( );

	backend::ref( ).set_render_targets(
		&*m_context->get_rt( rt_accumulator_diffuse ),
		&*m_context->get_rt( rt_accumulator_specular ),
		&*m_context->get_rt( rt_sun_translucensy_help_data ),
		0
	);
	backend::ref( ).clear_render_targets( math::color( 0.0f, 0.0f, 0.0f, 0.0f ) );

	backend::ref( ).set_render_targets(
		&*m_context->get_rt( rt_particle_lighting ),
		0,
		0,
		0
	);
	backend::ref( ).clear_render_targets( math::color( 0.0f, 0.0f, 0.0f, 0.0f ) );

	backend::ref( ).set_render_targets(
		&*m_context->get_rt( rt_particle_result ),
		0,
		0,
		0
	);
	backend::ref( ).clear_render_targets( math::color( 0.0f, 0.0f, 0.0f, 0.0f ) );

	m_context->set_w( float4x4( ).identity( ) );

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
