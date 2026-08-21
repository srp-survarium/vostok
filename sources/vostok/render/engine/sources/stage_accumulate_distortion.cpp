#include "pch.h"
#include "stage_accumulate_distortion.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/options.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/particle/world.h>

#include "material_effects.h"
#include "render_particle_emitter_instance.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"
#include "stage_particles.h"

namespace vostok {
namespace render {

stage_accumulate_distortion::stage_accumulate_distortion(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context )
{
	m_enabled = options::ref( ).current.m_enabled_distortion_stage;
}

stage_accumulate_distortion::~stage_accumulate_distortion( )
{
}

void stage_accumulate_distortion::execute( )
{
	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	vector< render_surface_instance* > m_dynamic_visuals;
	bool need_execute = false;
	m_context->scene( )->select_models(
		m_context->get_culling_vp( ),
		m_dynamic_visuals,
		m_context->get_view_pos( ),
		visible_flag,
		false );

	if ( m_dynamic_visuals.size( ) )
		need_execute = true;
	{
		particle::world* part_world = m_context->scene( )->particle_world( );

		if ( part_world )
		{
			particle::render_particle_emitter_instances_type emitters( g_allocator );

			part_world->get_render_emitter_instances( m_context->get_culling_vp( ), emitters );

			if ( emitters.size( ) )

				need_execute = true;
		}
	}

	if ( !need_execute )
	{
		execute_disabled( );
		return;
	}

	D3D11_VIEWPORT prev_view_port;

	backend::ref( ).get_viewport( prev_view_port );
	D3D11_VIEWPORT view_port;
	view_port.Width = float( m_context->get_rt( rt_distortion )->width( ) );
	view_port.Height = float( m_context->get_rt( rt_distortion )->height( ) );
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;

	backend::ref( ).set_viewport( view_port );

	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_distortion ), &*m_context->get_rt( rt_distortion_mask ), 0, 0 );
	backend::ref( ).clear_render_targets( math::color( 0.0f, 0.0f, 0.0f, 0.0f ) );
	backend::ref( ).reset_depth_stencil_target( );
	{
		particle::world* part_world = m_context->scene( )->particle_world( );

		if ( part_world )
		{
			particle::render_particle_emitter_instances_type emitters( g_allocator );

			part_world->get_render_emitter_instances( m_context->get_culling_vp( ), emitters );

			for ( particle::render_particle_emitter_instances_type::const_iterator it = emitters.begin( ); it != emitters.end( ); ++it )
			{
				render_particle_emitter_instance& instance = static_cast< render_particle_emitter_instance& >( **it );
				u32 const num_particles = instance.get_num_particles( );

				if ( !num_particles || !instance.get_material_effects( ).stage_enable[accumulate_distortion_render_stage] || !instance.get_material_effects( ).m_effects[accumulate_distortion_render_stage] )
					continue;

				instance.get_material_effects( ).m_effects[accumulate_distortion_render_stage]->apply( 0, 0 );

				particle_shader_constants::ref( ).set(
					m_context->get_v_inverted( ).transform_direction( float3( 0, 1000, 0 ) ).normalize( ),
					m_context->get_v_inverted( ).transform_direction( float3( 1000, 0, 0 ) ).normalize( ),
					m_context->get_v_inverted( ).lines[3].xyz( ),
					instance.locked_axis( ),
					instance.screen_alignment( )
				);
				particle_shader_constants::ref( ).set_time( m_context->m_current_time );

				m_context->set_w( instance.transform( ) );

				instance.render( m_context->get_v_inverted( ).lines[3].xyz( ), num_particles );
			}
		}
	}

	backend::ref( ).flush_rt_shader_resources( );
	render_surface_instance** it_d = m_dynamic_visuals.begin( );
	render_surface_instance* const* end_d = m_dynamic_visuals.end( );

	for ( ; it_d != end_d; ++it_d )
	{
		render_surface_instance& instance = **it_d;
		material_effects& effects = instance.m_render_surface->get_material_effects( );
		render_geometry& geometry = instance.m_render_surface->m_render_geometry;

		if ( !effects.stage_enable[accumulate_distortion_render_stage] )
			continue;

		m_context->set_w( *instance.m_transform );
		effects.m_effects[accumulate_distortion_render_stage]->apply( 0, 0 );
		geometry.geom->apply( );
		backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, geometry.primitive_count * 3, 0, 0 );
	}

	backend::ref( ).set_viewport( prev_view_port );

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
	m_context->set_w( float4x4( ).identity( ) );
}

} // namespace render
} // namespace vostok
