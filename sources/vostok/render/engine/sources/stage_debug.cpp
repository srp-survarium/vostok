#include "pch.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>

#include "effect_debug_environment_probe_preview.h"
#include "environment_probe.h"
#include "material.h"
#include "material_effects.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "scene.h"
#include "stage_debug.h"

namespace vostok {
namespace render {








stage_debug::stage_debug( renderer* in_renderer, renderer_context* context ) :
	stage( in_renderer, context ),
	m_sphere_geometry( 32, 32 )
{
	effect_manager::ref( ).create_effect<effect_debug_environment_probe_preview>( &m_debug_environment_probe_preview_effect );
	m_preview_mip_index_parameter = backend::ref( ).register_constant_host( "preview_mip_index", rc_float );
}

stage_debug::~stage_debug( )
{

}

bool stage_debug::is_effects_ready( ) const
{
	return m_debug_environment_probe_preview_effect.c_ptr( ) != NULL;
}

void stage_debug::render_environment_probe_preview( )
{
	vector<environment_probe*>& probes = m_context->scene( )->environment_probes( );

	environment_probe** it = probes.begin( );
	environment_probe** end = probes.end( );

	for ( ; it != end; ++it )
	{

		environment_probe* probe = *it;
		if ( probe->is_occluded( ) || !probe->m_texture || !probe->m_properties.enabled )
			continue;

		float4x4 world_transform = math::create_scale( float3( 0.5f, 0.5f, 0.5f ) )
			* math::create_translation( probe->m_properties.location );

		m_debug_environment_probe_preview_effect->apply( 0, 0 );
		m_context->set_w( world_transform );
		backend::ref( ).set_ps_texture( "t_probe_cubemap", &*probe->m_texture );
		backend::ref( ).set_ps_constant( m_preview_mip_index_parameter, probe->m_properties.preview_mip );
		m_sphere_geometry.draw( );
	}
}

void stage_debug::execute( )
{


	if ( !is_enabled( ) || !is_effects_ready( ) )
	{
		execute_disabled( );
		return;
	}

	backend::ref( ).flush_rt_shader_resources( );

	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_present ), 0, 0, 0 );

	vector<render_surface_instance*> m_dynamic_visuals;
	m_context->scene( )->select_models(
		m_context->get_culling_vp( ), m_dynamic_visuals,
		m_context->get_view_pos( ),
		visible_flag, false
	);

	render_surface_instance** it_d = m_dynamic_visuals.begin( );
	render_surface_instance* const* end_d = m_dynamic_visuals.end( );

	for ( ; it_d != end_d; ++it_d )
	{
		render_surface_instance& instance = **it_d;
		material_effects& effects = instance.m_render_surface->get_material_effects( );
		render_geometry& geometry = instance.m_render_surface->m_render_geometry;

		if ( !effects.stage_enable[debug_render_stage] )
			continue;

		m_context->set_w( *instance.m_transform );
		effects.m_effects[debug_render_stage]->apply( 0, 0 );
		geometry.geom->apply( );
		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			geometry.primitive_count * 3, 0, 0
		);
	}
	render_environment_probe_preview( );
}

} // namespace render
} // namespace vostok
