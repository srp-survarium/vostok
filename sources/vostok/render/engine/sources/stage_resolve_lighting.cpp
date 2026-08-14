#include "pch.h"
#include "stage_resolve_lighting.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/facade/render_stage_types.h>

#include "effect_resolve_lighting.h"
#include "light.h"
#include "material_effects.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

stage_resolve_lighting::stage_resolve_lighting(
	renderer* in_renderer,
	renderer_context* context
) :
	stage( in_renderer, context )
{
	effect_manager::ref( ).create_effect< effect_resolve_lighting >( &m_resolve_lighting_effect );

	m_wind_info_parameters = backend::ref( ).register_constant_host( "wind_info_parameters", rc_float );
	m_sun_light_parameters = backend::ref( ).register_constant_host( "sun_light_parameters", rc_float );
	m_eye_ray_corner_parameter = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_enabled = true;
}

bool stage_resolve_lighting::is_effects_ready( ) const
{
	return m_resolve_lighting_effect.c_ptr( ) != NULL;
}

void stage_resolve_lighting::render_models(
	vector<render_surface_instance*>& models,
	u32& out_num_rendered
)
{
	render_surface_instance** it = models.begin( );
	render_surface_instance* const* end = models.end( );
	light* sun = m_context->scene( )->lights( ).get_sun( ).c_ptr( );

	float4 sun_light_parameters( 1.f, 1.f, 1.f, 0.f );

	if ( sun )
		sun_light_parameters = float4( sun->direction, sun->intensity );

	for ( ; it != end; ++it )
	{
		render_surface_instance& instance = **it;
		material_effects& effects = instance.m_render_surface->get_material_effects( );

		if ( !effects.use_subsurface_scattering )
			continue;

		m_context->set_w( *instance.m_transform );
		effects.m_effects[gbuffer_render_stage]->apply( 7, 0 );
		instance.set_constants( );
		instance.m_render_surface->m_render_geometry.geom->apply( );

		if ( effects.is_wind_swings )
		{
			post_process_parameters const& parameters = m_context->scene_view( )->post_process_parameters( );
			float3 wind_info_parameters(
				parameters.wind_direction.x,
				parameters.wind_direction.z,
				parameters.wind_strength
			);
			backend::ref( ).set_vs_constant( m_wind_info_parameters, wind_info_parameters );
		}

		backend::ref( ).set_ps_constant( m_sun_light_parameters, sun_light_parameters );
		backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, instance.m_render_surface->m_render_geometry.primitive_count * 3, 0, 0 );
		++out_num_rendered;
	}
}

void stage_resolve_lighting::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}
	// 5 target lines are likely retail-compiled-out source.
	float3 const* const eye_rays = m_context->get_eye_rays( );
	m_resolve_lighting_effect->apply( 0, 0 );
	backend::ref( ).set_ps_constant( m_eye_ray_corner_parameter, eye_rays[0].x );
	system_renderer::ref( ).fill_surface( m_context->get_rt( rt_generic_0 ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.f, 0.f, 1.f, 1.f );
	// 6 target lines are likely retail-compiled-out source.
	vector<render_surface_instance*> visible_models;
	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), 0, 0, 0 );
	backend::ref( ).reset_depth_stencil_target( );

	visible_models = m_context->get_scene_view( )->get_visible_opaque_models( );
	u32 num_rendered = 0;
	render_models( visible_models, num_rendered );

	backend::ref( ).reset_render_targets( );
}

} // namespace render
} // namespace vostok
