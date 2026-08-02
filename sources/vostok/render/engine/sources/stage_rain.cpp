#include "pch.h"
#include "stage_rain.h"

#include <vostok/console_command.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/resource_manager.h>

#include "effect_rain.h"
#include "effect_shadow_map.h"
#include "help_math.h"
#include "renderer.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene_view.h"

namespace vostok {
namespace render {

static bool s_rain_debug2 = true;
static console_commands::cc_bool s_rain_debug2_cc(
	"r_rain_debug2",
	s_rain_debug2,
	false,
	console_commands::command_type_user_specific
);

stage_rain::stage_rain(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context ),
	m_rain_geometry( 24, 3 ),
	m_rain_offsets( 0 ),
	m_camera_offset_view( 0.0f ),
	m_camera_offset_right( 0.0f ),
	m_shadow_map_size( 0 ),
	m_previous_view_position( 0.0f, 0.0f, 0.0f ),
	m_radius_parameter( 0 ),
	m_rain_speed_parameter( 0 ),
	m_rain_density_parameter( 0 ),
	m_rain_uv_scales_parameter( 0 ),
	m_view_to_shadow_parameter( 0 )
{
	m_radius_parameter = backend::ref( ).register_constant_host( "rain_radius", rc_float );
	m_rain_speed_parameter = backend::ref( ).register_constant_host( "rain_speed", rc_float );
	m_view_to_shadow_parameter = backend::ref( ).register_constant_host( "view_to_shadow", rc_float );
	m_rain_density_parameter = backend::ref( ).register_constant_host( "rain_density", rc_float );
	m_rain_uv_scales_parameter = backend::ref( ).register_constant_host( "rain_uv_scales", rc_float );

	m_shadow_map_size = 128;
	m_t_rain_shadow_map = resource_manager::ref( ).create_texture(
		"$user$rain_shadow_map",
		0,
		0,
		false,
		true,
		true,
		-1
	);

	effect_manager::ref( ).create_effect<effect_rain>( &m_rain_effect );
	effect_manager::ref( ).create_effect<effect_shadow_map>( &m_effect_shadow_direct );

	m_rain_offsets = NEW_ARRAY( float2, 100 );
	math::random32 r( 1000 );
	for ( u32 i = 0; i < 100; ++i )
	{
		m_rain_offsets[i].set( r.random_f( 2.0f ) - 1.0f, r.random_f( 2.0f ) - 1.0f );
		m_rain_rotation_x[i] = 0.0f;
		m_rain_rotation_y[i] = 0.0f;
	}
}

bool stage_rain::is_effects_ready( ) const
{
	return m_rain_effect.c_ptr( ) && m_effect_shadow_direct.c_ptr( );
}

stage_rain::~stage_rain( )
{
	DELETE_ARRAY( m_rain_offsets );
}

void stage_rain::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) ||
		 !m_context->get_scene_view( )->post_process_parameters( ).environment_use_rain ||
		 m_context->get_scene_view( )->post_process_parameters( ).environment_rain_density < 0.01f )
	{
		execute_disabled( );
		return;
	}

	if ( !m_context->get_scene_view( )->is_use_post_process( ) ||
		 m_context->get_scene_view( )->get_view_mode( ) == unlit_view_mode )
	{
		execute_disabled( );
		return;
	}

	vector< render_surface_instance* > m_caster_model;
	device::ref( ).d3d_context( )->CopyResource(
		m_context->get_t( rt_generic_1 )->hw_texture( ),
		m_context->get_t( rt_generic_0 )->hw_texture( )
	);

	static bool s_first_pass = true;
	if ( s_first_pass )
		m_previous_view_position = m_context->get_view_pos( );
	s_first_pass = false;

	float4x4 view_to_shadow = m_renderer->get_view_to_rain_shadow( );
	backend::ref( ).set_render_targets(
		&*m_context->get_rt( rt_generic_0 ),
		0,
		0,
		0
	);

	float4x4 view_matrix = m_context->get_v( );
	math::random32 r( 1000 );
	float2 view_dir_2d = math::normalize_safe(
		float2( m_context->get_view_dir( ).x, m_context->get_view_dir( ).z ),
		float2( 0.0f, 0.0f )
	);
	float2 right_dir_2d = math::normalize_safe(
		float2( view_matrix.i.x, view_matrix.k.x ),
		float2( 0.0f, 0.0f )
	);
	float2 offset_vector_ground(
		m_context->get_view_pos( ).x - m_previous_view_position.x,
		m_context->get_view_pos( ).z - m_previous_view_position.z
	);
	float2 offset_direction_ground = math::normalize_safe(
		offset_vector_ground,
		float2( 0.0f, 0.0f )
	);
	float const abs_ov_dot_dir_ground = offset_direction_ground | view_dir_2d;
	float const abs_ov_dot_dir_ground2 = offset_direction_ground | right_dir_2d;

	if ( abs_ov_dot_dir_ground > 0.0f )
		m_camera_offset_view +=
			offset_vector_ground.length( ) *
			math::abs( abs_ov_dot_dir_ground ) * 0.5f;
	if ( abs_ov_dot_dir_ground < 0.0f )
		m_camera_offset_view -=
			offset_vector_ground.length( ) *
			math::abs( abs_ov_dot_dir_ground ) * 0.5f;

	if ( m_camera_offset_view >= 1.0f )
		m_camera_offset_view = frac( m_camera_offset_view );
	else if ( m_camera_offset_view < 0.0f &&
			  math::abs( m_camera_offset_view ) >= 1.0f )
		m_camera_offset_view = 1.0f - frac( m_camera_offset_view );

	if ( abs_ov_dot_dir_ground2 > 0.0f )
		m_camera_offset_right +=
			offset_vector_ground.length( ) *
			math::abs( abs_ov_dot_dir_ground2 ) * 0.5f;
	if ( abs_ov_dot_dir_ground2 < 0.0f )
		m_camera_offset_right -=
			offset_vector_ground.length( ) *
			math::abs( abs_ov_dot_dir_ground2 ) * 0.5f;

	if ( m_camera_offset_right >= 1.0f )
		m_camera_offset_right = frac( m_camera_offset_right );
	else if ( m_camera_offset_right < 0.0f &&
			  math::abs( m_camera_offset_right ) >= 1.0f )
		m_camera_offset_right = 1.0f - frac( m_camera_offset_right );

	float const rain_angle_x = m_context->get_scene_view( )->post_process_parameters( ).environment_rain_angle_x;
	float const rain_angle_y = m_context->get_scene_view( )->post_process_parameters( ).environment_rain_angle_y;
	float const rain_density = m_context->get_scene_view( )->post_process_parameters( ).environment_rain_density;
	float const rain_speed = m_context->get_scene_view( )->post_process_parameters( ).environment_rain_speed;
	u32 const num_cones = m_context->get_scene_view( )->post_process_parameters( ).environment_rain_num_cones;
	float2 rain_uv_scales(
		m_context->get_scene_view( )->post_process_parameters( ).environment_rain_u_scale,
		m_context->get_scene_view( )->post_process_parameters( ).environment_rain_v_scale
	);
	float mult = 1.0f;

	if ( s_rain_debug2 )
	{
		u32 i = m_context->get_scene_view( )->post_process_parameters( ).environment_rain_start_cone_index;
		for ( ;
			  i < num_cones;
			  ++i )
		{
			float const cone_scale =
				( i == 1 ? 1.0f : 2.0f ) * float( i ) * mult;
			float4x4 cone_transform =
				math::create_scale(
					float3( cone_scale, cone_scale * 10.0f, cone_scale )
				) *
				math::create_rotation_y( r.random_f( 1.0f ) * math::pi_x2 ) *
				math::create_rotation_x(
					rain_angle_x +
					math::cos( m_rain_rotation_x[i] ) *
						m_context->get_scene_view( )->post_process_parameters( ).environment_rain_random_rotaion_radius +
					m_rain_offsets[i].x *
						m_context->get_scene_view( )->post_process_parameters( ).environment_rain_random_base_offset
				) *
				math::create_rotation_z(
					rain_angle_y +
					math::sin( m_rain_rotation_y[i] ) *
						m_context->get_scene_view( )->post_process_parameters( ).environment_rain_random_rotaion_radius +
					m_rain_offsets[i].y *
						m_context->get_scene_view( )->post_process_parameters( ).environment_rain_random_base_offset
				) *
				math::create_translation(
					m_context->get_view_pos( ) -
					float3( view_dir_2d.x, 0.0f, view_dir_2d.y ) *
						m_camera_offset_view -
					float3( right_dir_2d.x, 0.0f, right_dir_2d.y ) *
						m_camera_offset_right
				);

			m_rain_effect->apply( 0, 0 );
			m_context->set_w( cone_transform );
			backend::ref( ).set_ps_constant(
				m_radius_parameter,
				float( i ) * mult
			);
			backend::ref( ).set_ps_constant( m_rain_speed_parameter, rain_speed );
			backend::ref( ).set_ps_constant( m_rain_density_parameter, rain_density );
			backend::ref( ).set_ps_constant(
				m_rain_uv_scales_parameter,
				rain_uv_scales
			);
			backend::ref( ).set_ps_constant(
				m_view_to_shadow_parameter,
				math::transpose( view_to_shadow )
			);
			m_rain_geometry.draw( );

			m_rain_rotation_x[i] +=
				m_context->get_time_delta( ) *
				m_context->get_scene_view( )->post_process_parameters( ).environment_rain_random_rotaion_speed;
			m_rain_rotation_y[i] +=
				m_context->get_time_delta( ) *
				m_context->get_scene_view( )->post_process_parameters( ).environment_rain_random_rotaion_speed;
			mult *= m_context->get_scene_view( )->post_process_parameters( ).environment_rain_radius_scale;
		}
	}

	m_previous_view_position = m_context->get_view_pos( );
	m_context->set_w_identity( );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
