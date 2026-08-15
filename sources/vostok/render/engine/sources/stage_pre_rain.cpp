#include "pch.h"
#include "stage_pre_rain.h"

#include <vostok/console_command.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>

#include "effect_shadow_map.h"
#include "effect_wet_surface.h"
#include "renderer.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "scene.h"
#include "scene_view.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

static math::random32 s_random( 1000 );

static bool s_rain_debug0 = true;
static console_commands::cc_bool s_rain_debug0_cc(
	"r_rain_debug0",
	s_rain_debug0,
	false,
	console_commands::command_type_user_specific
);

static bool s_rain_debug1 = true;
static console_commands::cc_bool s_rain_debug1_cc(
	"r_rain_debug1",
	s_rain_debug1,
	false,
	console_commands::command_type_user_specific
);

stage_pre_rain::stage_pre_rain(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context ),
	m_rain_offset( 0.0f ),
	m_rain_offset_counter( 0.0f )
{
	m_view_to_shadow_parameter = backend::ref( ).register_constant_host( "view_to_shadow", rc_float );
	m_eye_ray_corner_parameter = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_rain_offset_parameter = backend::ref( ).register_constant_host( "rain_offset", rc_float );
	m_rain_density_parameter = backend::ref( ).register_constant_host( "rain_density", rc_float );

	m_shadow_map_size = 128;
	m_rt_rain_shadow_map = resource_manager::ref( ).create_render_target(
		"$user$rain_shadow_map",
		m_shadow_map_size,
		m_shadow_map_size,
		DXGI_FORMAT_R16_TYPELESS,
		enum_rt_usage_depth_stencil,
		res_texture_ptr( ),
		0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rain_shadow_map = resource_manager::ref( ).create_texture(
		"$user$rain_shadow_map", 0, 0, false, true, true, -1 );

	effect_manager::ref( ).create_effect<effect_wet_surface>( &m_wet_surface_effect );
	effect_manager::ref( ).create_effect<effect_shadow_map>( &m_effect_shadow_direct );
}

bool stage_pre_rain::is_effects_ready( ) const
{
	return m_wet_surface_effect.c_ptr( ) && m_effect_shadow_direct.c_ptr( );
}

stage_pre_rain::~stage_pre_rain( )
{
}

float3 stage_pre_rain::compute_aligment(
	float3 const&		lightXZshift,
	float4x4 const&	light_space_transform,
	float				smap_res
)
{
	float3 align_origin( 0.0f, 0.0f, 0.0f );

	align_origin -= lightXZshift;

	float4x4 viewport(
		float4( smap_res / 2.0f, 0.0f, 0.0f, 0.0f ), float4( 0.0f, -smap_res / 2.0f, 0.0f, 0.0f ),
		float4( 0.0f, 0.0f, 1.0f, 0.0f ), float4( smap_res / 2.0f, smap_res / 2.0f, 0.0f, 1.0f )
	);

	float4x4 viewport_invert = math::invert4x3( viewport );
	float4x4 light_space_transform_invert = math::invert4x3( light_space_transform );

	float4 origin_pixel = light_space_transform.transform( float4( align_origin, 1.0f ) );
	origin_pixel *= 1.0f / origin_pixel.w;
	origin_pixel = viewport.transform( origin_pixel );

	// 2 target lines are likely retail-compiled-out source.

	origin_pixel.x = origin_pixel.x / 4.0f - floorf( origin_pixel.x / 4.0f );
	origin_pixel.y = origin_pixel.y / 4.0f - floorf( origin_pixel.y / 4.0f );
	origin_pixel.x *= 4.0f;
	origin_pixel.y *= 4.0f;
	origin_pixel.z = 0.0f;

	origin_pixel.xyz( ) = viewport_invert.transform_direction( origin_pixel.xyz( ) );
	origin_pixel.xyz( ) = light_space_transform_invert.transform_direction( origin_pixel.xyz( ) );

	return origin_pixel.xyz( );
	// 3 target lines are likely retail-compiled-out source.


}

float4x4 stage_pre_rain::render_rain_shadow_map( )
{
	scene_view* view = m_context->get_scene_view( );
	float4x4 rotation_matrix = math::mul4x3(
		math::create_rotation_x( view->post_process_parameters( ).environment_rain_angle_x ),
		math::create_rotation_z( view->post_process_parameters( ).environment_rain_angle_y ) );

	float3 view_dir = m_context->get_view_dir( );
	float3 direction = -rotation_matrix.j.xyz( );
	float3 position =
		m_context->get_view_pos( ) - direction * 100.0f +
		view_dir * 0.5f * 0.5f * float( m_shadow_map_size );
	float4x4 shadow_view_transform = math::create_camera_direction( position, direction, float3( 1.0f, 0.0f, 0.0f ) );

	float4x4 shadow_projection_transform = math::create_orthographic_projection(
		float( m_shadow_map_size ) * 0.5f,
		float( m_shadow_map_size ) * 0.5f,
		0.1f,
		float( m_shadow_map_size ) * 1.41421f + 200.0f
	);
	float4x4 shadow_full_transform = math::mul4x3( shadow_view_transform, shadow_projection_transform );
	float3 adjastment = compute_aligment( float3( 0.0f, 0.0f, 0.0f ), shadow_full_transform, float( m_shadow_map_size ) );
	shadow_view_transform = math::create_camera_direction(
		position + adjastment, direction, float3( 1.0f, 0.0f, 0.0f ) );



	m_context->push_set_v( shadow_view_transform );
	m_context->push_set_p( shadow_projection_transform );

	backend::ref( ).set_render_targets( 0, 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( &*m_rt_rain_shadow_map );
	backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );

	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( m_shadow_map_size );
	tmp_viewport.Height = float( m_shadow_map_size );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = 1.0f;

	backend::ref( ).set_viewport( tmp_viewport );

	vector< render_surface_instance* > m_caster_model;
	m_context->scene( )->select_models( m_context->get_culling_vp( ), m_caster_model,
		m_context->get_view_pos( ), visible_flag, false );

	vector< render_surface_instance* >::iterator it_d = m_caster_model.begin( );
	vector< render_surface_instance* >::const_iterator end_d = m_caster_model.end( );



	if ( s_rain_debug0 )
		for ( ; it_d != end_d; ++it_d )
		{
			render_surface_instance& instance = *(*it_d);
			render_surface* surface = instance.m_render_surface;

			if ( !surface->m_render_geometry.geom.c_ptr( ) ||
				 ( surface->get_vertex_input_type( ) != static_mesh_vertex_input_type &&
				   surface->get_vertex_input_type( ) != static_mesh_vertex_colored_input_type ) )
				continue;

			if ( instance.is_occluded( ) || instance.m_dynamic_screen_factor < 0.004f )
				continue;

			m_effect_shadow_direct->apply( 0, 0 );

			instance.set_constants( );
			surface->m_render_geometry.geom->apply( );
			m_context->set_w( *instance.m_transform );
			backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, surface->m_render_geometry.primitive_count * 3, 0, 0 );
		}



	m_context->pop_v( );


	m_context->pop_p( );




	float4x4 texture_space(
		float4( 0.5f, 0.0f, 0.0f, 0.0f ),
		float4( 0.0f, -0.5f, 0.0f, 0.0f ),
		float4( 0.0f, 0.0f, 1.0f, 0.0f ),
		float4( 0.5f, 0.5f, -0.001f, 1.0f )
	);
	float4x4 const view_to_shadow =
		m_context->get_v_inverted( ) * shadow_full_transform * texture_space;

	backend::ref( ).set_viewport( orig_viewport );
	m_context->set_w_identity( );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );

	return view_to_shadow;
}

void stage_pre_rain::execute( )
{
	if ( !is_effects_ready( ) )
		return;
	if ( !options::ref( ).current.m_shading_quality || !is_enabled( ) ||
		 !m_context->get_scene_view( )->post_process_parameters( ).environment_use_rain )
	{
		execute_disabled( );
		return;
	}

	if ( !m_context->get_scene_view( )->is_use_post_process( ) || m_context->get_scene_view( )->get_view_mode( ) == unlit_view_mode )
	{
		execute_disabled( );
		return;
	}

	vector< render_surface_instance* > m_caster_model;

	m_rain_offset_counter += m_context->get_time_delta( ) * 2.0f;

	if ( m_rain_offset_counter >= s_random.random_f( 0.5f ) + 1.0f )
	{
		m_rain_offset += s_random.random_f( 0.75f );
		m_rain_offset_counter = 0.0f;
	}



	backend::ref( ).flush_rt_shader_resources( );
	// 16 target lines are likely retail-compiled-out source.















	float4x4 view_to_shadow = render_rain_shadow_map( );

	if ( s_rain_debug1 )
	{
		m_wet_surface_effect->apply( 4, 0 );
		backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_normal ) );
		system_renderer::ref( ).fill_surface( m_context->get_rt( rt_normal_copy ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );
		backend::ref( ).flush_rt_shader_resources( );

		m_wet_surface_effect->apply( 0, 0 );
		renderer_context* const context = m_context;
		backend::ref( ).set_ps_constant( m_eye_ray_corner_parameter, context->get_eye_rays( )[0].x );
		backend::ref( ).set_ps_constant( m_view_to_shadow_parameter, math::transpose( view_to_shadow ) );
		backend::ref( ).set_ps_constant( m_rain_offset_parameter, m_rain_offset );
		backend::ref( ).set_ps_constant( m_rain_density_parameter, context->get_scene_view( )->post_process_parameters( ).environment_rain_density );
		system_renderer::ref( ).fill_surface( context->get_rt( rt_normal ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ),
			render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );
	}

	m_renderer->set_view_to_rain_shadow( view_to_shadow );
	// 20 target lines are likely retail-compiled-out source.



















	m_context->set_w_identity( );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
