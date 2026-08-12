#include "pch.h"
#include "stage_forward.h"

#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/render/facade/vertex_input_type.h>

#include "blend_mode.h"
#include "decal_instance.h"
#include "effect_debug_tracer.h"
#include "effect_decal_mask.h"
#include "effect_gbuffer_depth.h"
#include "material_effects.h"
#include "render_geometry.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"
#include "scene_view.h"
#include "statistics.h"

namespace vostok {
namespace render {

static math::random32 s_random( 1000 );

stage_forward::stage_forward(
	renderer* in_renderer,
	renderer_context* context,
	stage_type type
) :
	stage( in_renderer, context ),
	m_type( type )
{
	m_tree_position_and_scale_parameter = backend::ref( ).register_constant_host( "tree_position_and_scale", rc_float );
	m_tree_rotation_parameter = backend::ref( ).register_constant_host( "tree_rotation", rc_float );
	m_far_fog_color_and_distance = backend::ref( ).register_constant_host( "far_fog_color_and_distance", rc_float );
	m_fog_alpha = backend::ref( ).register_constant_host( "fog_alpha", rc_float );
	m_near_fog_distance = backend::ref( ).register_constant_host( "near_fog_distance", rc_float );
	m_eye_ray_corner_parameter = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_view_to_shadow_parameter = backend::ref( ).register_constant_host( "view_to_shadow", rc_float );
	m_rain_offset_parameter = backend::ref( ).register_constant_host( "rain_offset", rc_float );
	m_use_rain_parameter = backend::ref( ).register_constant_host( "use_rain", rc_float );
	m_tracer_debug_color_parameter = backend::ref( ).register_constant_host( "tracer_debug_color", rc_float );
	m_c_inscatter_parameters = backend::ref( ).register_constant_host( "inscatter_parameters", rc_float );

	effect_manager::ref( ).create_effect< effect_decal_mask >( &m_opaque_geometry_mask_effect );

	u8 tracer_data[Kb];
	effect_options_descriptor tracer_desc( tracer_data, sizeof( tracer_data ) );
	tracer_desc["vertex_input_type"] = static_mesh_vertex_input_type;
	tracer_desc["blend_mode"] = 1;
	effect_manager::ref( ).create_effect< effect_debug_tracer >( &m_debug_tracer_effect, tracer_desc );

	m_enabled = options::ref( ).current.m_enabled_forward_stage;

	m_rain_offset = 0.0f;
	m_rain_offset_counter = 0.0f;

	for ( u32 i = 0; i < array_size( m_gbuffer_depth_effect ); ++i )
	{
		if ( i == 12 )
			continue;

		u8 data[Kb];
		effect_options_descriptor desc( data, sizeof( data ) );
		desc["vertex_input_type"] = enum_vertex_input_type( i );
		desc["cull_mode"] = D3D11_CULL_NONE;
		effect_manager::ref( ).create_effect< effect_gbuffer_depth >( &m_gbuffer_depth_effect[i], desc );
	}
}

bool stage_forward::is_effects_ready( ) const
{
	for ( u32 i = 0; i < array_size( m_gbuffer_depth_effect ); ++i )
	{
		if ( i == 12 )
			continue;

		if ( !m_gbuffer_depth_effect[i].c_ptr( ) )
			return false;
	}

	return m_opaque_geometry_mask_effect.c_ptr( ) != NULL && m_debug_tracer_effect.c_ptr( ) != NULL;
}

stage_forward::~stage_forward( )
{
}

void stage_forward::accumulate_local_reflections( )
{
	D3D11_VIEWPORT prev_view_port;
	backend::ref( ).get_viewport( prev_view_port );

	D3D11_VIEWPORT view_port;
	view_port.Width = float( m_context->get_rt( rt_local_reflection_result )->width( ) );
	view_port.Height = float( m_context->get_rt( rt_local_reflection_result )->height( ) );
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	backend::ref( ).set_viewport( view_port );

	float const use_rain = m_context->get_scene_view( )->post_process_parameters( ).environment_use_rain ? 1.0f : 0.0f;

	vector< render_surface_instance* > m_dynamic_visuals;
	m_context->scene( )->select_models(
		m_context->get_culling_vp( ),
		m_dynamic_visuals,
		m_context->get_view_pos( ),
		visible_flag,
		false
	);

	render_surface_instance* const* end_d = m_dynamic_visuals.end( );
	render_surface_instance** it_d = m_dynamic_visuals.begin( );

	if ( m_dynamic_visuals.size( ) )
	{
		backend::ref( ).set_render_targets(
			&*m_context->get_rt( rt_local_reflection_result ),
			&*m_context->get_rt( rt_local_reflection_result_params ),
			0,
			0
		); backend::ref( ).reset_depth_stencil_target( );
	}

	for ( ; it_d != end_d; ++it_d )
	{
		if ( options::ref( ).current.m_use_hiz_occlusion_culling && ( *it_d )->m_occluded )
			continue;

		render_surface_instance& instance = **it_d;
		material_effects& effects = instance.m_render_surface->get_material_effects( );
		render_geometry& geometry = instance.m_render_surface->m_render_geometry;

		if ( !effects.has_local_reflections || !effects.m_effects[forward_render_stage].c_ptr( ) )
			continue;

		instance.set_constants( );
		m_context->set_w( *instance.m_transform );
		effects.m_effects[forward_render_stage]->apply( 1, 0 );
		geometry.geom->apply( );

		backend::ref( ).set_ps_constant(
			m_eye_ray_corner_parameter,
			m_context->get_eye_rays( )[0].x
		);
		backend::ref( ).set_ps_constant( m_view_to_shadow_parameter, math::transpose( m_renderer->get_view_to_rain_shadow( ) ) );
		backend::ref( ).set_ps_constant( m_rain_offset_parameter, m_rain_offset );
		backend::ref( ).set_ps_constant(
			m_far_fog_color_and_distance,
			float4(
				m_context->get_scene_view( )->post_process_parameters( ).environment_far_fog_color,
				m_context->get_scene_view( )->post_process_parameters( ).environment_far_fog_distance
			)
		);
		backend::ref( ).set_ps_constant( m_near_fog_distance, m_context->get_scene_view( )->post_process_parameters( ).environment_near_fog_distance );
		backend::ref( ).set_ps_constant( m_fog_alpha, m_context->get_scene_view( )->post_process_parameters( ).environment_fog_alpha );
		backend::ref( ).set_ps_constant( m_use_rain_parameter, use_rain );

		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			geometry.primitive_count * 3,
			0,
			0
		);
	}

	backend::ref( ).set_viewport( prev_view_port );
}

void stage_forward::render_forward_models(
	vector< render_surface_instance* >& dynamic_visuals,
	u32 const pass_index
)
{
	render_surface_instance** it_d = dynamic_visuals.begin( );
	render_surface_instance* const* end_d = dynamic_visuals.end( );

	if ( dynamic_visuals.size( ) )
	{
		backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), 0, 0, 0 );
		backend::ref( ).reset_depth_stencil_target( );
	}

	for ( ; it_d != end_d; ++it_d )
	{
		if ( options::ref( ).current.m_use_hiz_occlusion_culling && ( *it_d )->m_occluded )
			continue;

		render_surface_instance& instance = **it_d; material_effects& effects = instance.m_render_surface->get_material_effects( );
		render_geometry& geometry = instance.m_render_surface->m_render_geometry;

		if ( pass_index == 0 && effects.is_forward_after_fog )
			continue;
		else if ( pass_index == 1 && !effects.is_forward_after_fog )
			continue;

		instance.set_constants( );
		m_context->set_w( *instance.m_transform );
		effects.m_effects[forward_render_stage]->apply( 0, 0 );
		geometry.geom->apply( );

		backend::ref( ).set_ps_constant( m_eye_ray_corner_parameter, m_context->get_eye_rays( )[0] );
		backend::ref( ).set_ps_constant( m_view_to_shadow_parameter, math::transpose( m_renderer->get_view_to_rain_shadow( ) ) );
		backend::ref( ).set_ps_constant( m_rain_offset_parameter, m_rain_offset );
		backend::ref( ).set_ps_constant(
			m_far_fog_color_and_distance,
			float4(
				m_context->get_scene_view( )->post_process_parameters( ).environment_far_fog_color,
				m_context->get_scene_view( )->post_process_parameters( ).environment_far_fog_distance
			)
		);
		backend::ref( ).set_ps_constant( m_near_fog_distance, m_context->get_scene_view( )->post_process_parameters( ).environment_near_fog_distance );
		backend::ref( ).set_ps_constant( m_fog_alpha, m_context->get_scene_view( )->post_process_parameters( ).environment_fog_alpha );

		float const use_rain = m_context->get_scene_view( )->post_process_parameters( ).environment_use_rain ? 1.0f : 0.0f;
		backend::ref( ).set_ps_constant( m_use_rain_parameter, use_rain );

		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			geometry.primitive_count * 3,
			0,
			0
		);
	}
}

void stage_forward::render_opaque_models( )
{
	vector< render_surface_instance* >& opaque_models = m_context->get_scene_view( )->get_visible_opaque_models( );

	render_surface_instance** opaque_it_d = opaque_models.begin( );
	render_surface_instance* const* end_d = opaque_models.end( );

	if ( opaque_models.size( ) )
	{
		backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), 0, 0, 0 );
		backend::ref( ).reset_depth_stencil_target( );
	}

	for ( ; opaque_it_d != end_d; ++opaque_it_d )
	{
		if ( options::ref( ).current.m_use_hiz_occlusion_culling && ( *opaque_it_d )->m_occluded )
			continue;

		render_surface_instance& instance = **opaque_it_d; material_effects& effects = instance.m_render_surface->get_material_effects( );
		render_geometry& geometry = instance.m_render_surface->m_render_geometry;

		if ( !effects.is_emissive || !effects.m_effects[gbuffer_render_stage].c_ptr( ) )
			continue;

		m_context->set_w( *instance.m_transform );
		effects.m_effects[gbuffer_render_stage]->apply( 5, 0 );
		geometry.geom->apply( );
		instance.set_constants( );

		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			geometry.primitive_count * 3,
			0,
			0
		);
	}
}

bool remove_model_if_not_forward_predicate::operator()(
	render_surface_instance const* in_model
)
{
	return !in_model->m_render_surface->get_material_effects( ).m_effects[forward_render_stage].c_ptr( );
}

void stage_forward::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	device::ref( ).d3d_context( )->CopyResource(
		m_context->get_t( rt_generic_1 )->hw_texture( ),
		m_context->get_t( rt_generic_0 )->hw_texture( )
	);

	m_rain_offset_counter += m_context->get_time_delta( ) * 2.0f;
	if ( m_rain_offset_counter >= s_random.random_f( 0.5f ) + 1.0f )
	{
		m_rain_offset += s_random.random_f( 0.75f );
		m_rain_offset_counter = 0.0f;
	}

	vector< render_surface_instance* > m_dynamic_visuals;
	m_context->scene( )->select_models(
		m_context->get_culling_vp( ),
		m_dynamic_visuals,
		m_context->get_view_pos( ),
		visible_flag,
		false
	);
	m_dynamic_visuals.erase(
		std::remove_if(
			m_dynamic_visuals.begin( ),
			m_dynamic_visuals.end( ),
			remove_model_if_not_forward_predicate( )
		),
		m_dynamic_visuals.end( )
	);

	m_renderer->sort_models_by_distance( m_dynamic_visuals, false );

	if ( m_type == forward_sky )
		render_forward_models( m_dynamic_visuals, 1 );

	if ( m_dynamic_visuals.size( ) )
	{
		backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), 0, 0, 0 );
		backend::ref( ).reset_depth_stencil_target( );
	}

	if ( m_type == forward_sky )
	{
		m_context->set_w( float4x4( ).identity( ) );
		backend::ref( ).reset_render_targets( );
		backend::ref( ).reset_depth_stencil_target( );
		return;
	}

	render_opaque_models( );
	accumulate_local_reflections( );
	render_forward_models( m_dynamic_visuals, 0 );

	{
		scene::decal_instance_list_type& decals = m_context->scene( )->decals( );
		collision::objects_type decals_objects( g_allocator );

		backend::ref( ).reset_depth_stencil_target( );

		decals_objects.reserve( decals.size( ) );

		math::frustum frustum = m_context->get_culling_vp( );
		m_context->scene( )->decals_tree( ).cuboid_query( u32( -1 ), frustum, decals_objects );

		if ( decals_objects.size( ) )
		{
			backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), 0, 0, 0 );
			backend::ref( ).reset_depth_stencil_target( );
		}

		for ( collision::objects_type::iterator i = decals_objects.begin( ); i != decals_objects.end( ); ++i )
		{
			decal_instance* decal = static_cast_checked< decal_instance* >( ( *i )->user_data( ) );

			statistics::ref( ).forward_decals_stat_group.num_decal_draw_calls.value +=
				decal->draw( m_context, m_opaque_geometry_mask_effect, forward_render_stage );

			statistics::ref( ).forward_decals_stat_group.num_decals.value++;
		}
	}

	m_context->set_w( float4x4( ).identity( ) );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
