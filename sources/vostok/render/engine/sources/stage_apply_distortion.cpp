#include "pch.h"
#include "stage_apply_distortion.h"

#include <vostok/render/core/effect_manager.h>
#include "effect_apply_distortion.h"
#include <vostok/render/core/backend.h>
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "vertex_formats.h"
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include "statistics.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

stage_apply_distortion::stage_apply_distortion( renderer* in_renderer, renderer_context* context ) : stage( in_renderer, context )
{
	effect_manager::ref( ).create_effect<effect_apply_distortion>( &m_sh_apply_distortion );
}

stage_apply_distortion::~stage_apply_distortion( )
{

}

bool stage_apply_distortion::is_effects_ready( ) const
{
	return m_sh_apply_distortion.c_ptr( ) != NULL;
}

void stage_apply_distortion::execute( )
{

	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	m_sh_apply_distortion->apply( effect_apply_distortion::apply_distortion, 0 );
	system_renderer::ref( ).fill_surface( m_context->get_rt( rt_generic_1 ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );

	backend::ref( ).flush_rt_shader_resources( );

	m_sh_apply_distortion->apply( effect_apply_distortion::copy_result, 0 );
	system_renderer::ref( ).fill_surface( m_context->get_rt( rt_generic_0 ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
