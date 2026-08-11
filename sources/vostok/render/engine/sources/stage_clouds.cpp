#include "pch.h"
#include "stage_clouds.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/res_effect.h>

#include "clouds.h"
#include "effect_clouds.h"
#include "effect_clouds_god_rays.h"
#include "effect_read_cloud_base.h"
#include "help_math.h"
#include "renderer_context.h"
#include "scene.h"

namespace vostok {
namespace render {

struct screen_vertex {
	float4	position;
	float2	tc;

	void set( float4 const& in_position, float2 const& in_tc )
	{
		position = in_position;
		tc = in_tc;
	}
};

STATIC_SIZE_ASSERT( screen_vertex, 0x18 );

environment_temp::environment_temp( ) :
	key_time_step	( 5.0f ),
	num_keys		( 3 )
{
	// 20 target lines are likely retail-compiled-out source.
	keys = NEW_ARRAY( cloud_key_parameters, num_keys );

	keys[0].cloud_base = 3200.0f;
	keys[0].wind_speed = 2.0f;
	keys[0].layer_height = 6.0f;
	keys[0].ambient = -1.0f;
	keys[0].direct_light = -1.0f;
	keys[0].cloud_generate_cloudiness = 0.7f;
	keys[0].cloud_generate_octaves = 2.0f;
	keys[0].diffusivity = 0.0f;

	keys[1].cloud_base = 3200.0f;
	keys[1].wind_speed = 2.0f;
	keys[1].layer_height = 6.0f;
	keys[1].ambient = -1.0f;
	keys[1].direct_light = -1.0f;
	keys[1].cloud_generate_cloudiness = 0.8f;
	keys[1].cloud_generate_octaves = 2.0f;
	keys[1].diffusivity = 0.0f;

	keys[2].cloud_base = 2000.0f;
	keys[2].wind_speed = -1.0f;
	keys[2].layer_height = 6.0f;
	keys[2].ambient = -1.0f;
	keys[2].direct_light = -1.0f;
	keys[2].cloud_generate_cloudiness = 0.5f;
	keys[2].cloud_generate_octaves = 2.0f;
	keys[2].diffusivity = 0.0f;
}

environment_temp::~environment_temp( )
{
	DELETE_ARRAY( keys );
}
// 13 target lines are likely retail-compiled-out source.
cloud_key_parameters environment_temp::get_next_key( u32 const index )
{
	u32 const next_index = index + 1;
	if ( next_index >= num_keys )

		return keys[0];
	return keys[next_index];
}

cloud_key_parameters environment_temp::get_interp_key( float time )
{

	float const key_time = 0.0f / key_time_step;
	cloud_key_parameters result;

	s32 ipos = math::floor( key_time );
	u32 const source_index = ipos % num_keys;
	u32 const target_index = source_index + 1 < num_keys ? source_index + 1 : 0;
	float local_time = math::abs( key_time ) - static_cast<float>( math::abs( ipos ) );

	float alpha = math::pow( local_time, 1.0f );

	result = cloud_key_parameters::lerp( keys[source_index], keys[target_index], alpha );
	result.interp_alpha = alpha;
	result.source_key_index = source_index;
	result.target_key_index = target_index;
	return result;
}

stage_clouds::stage_clouds(
	renderer*				in_renderer,
	cloud_interp_textures&	in_interp_textures,
	cloud_simulation&		in_simulation,
	renderer_context*		context
) :
	stage						( in_renderer, context ),
	m_evaluate_geometry			( 25, 25 ),
	m_previous_view_position	( 0.0f, 0.0f, 0.0f ),
	m_camera_offset				( 0.0f ),
	m_clouds_size_x				( options::ref( ).current.m_clouds_grid_width ),
	m_clouds_size_y				( options::ref( ).current.m_clouds_grid_height ),
	m_clouds_size_z				( options::ref( ).current.m_clouds_grid_width ),
	m_clouds_scale_multiplier	( 3000.0f ),
	m_wind_offset				( 0.0f, 0.0f, 0.0f ),
	m_wind_direction			( -1.0f, 0.0f, -1.0f ),
	m_fixed_time				( 0.0f ),
	m_need_generate				( false ),
	m_tasks_type				( tasks::create_new_task_type( "generate_clouds_task", 0 ) ),
	m_first_tick				( true ),
	m_simulation				( in_simulation ),
	m_interp_textures			( in_interp_textures ),
	m_x_rotation				( 0.0f ),
	m_y_rotation				( 0.0f ),
	m_prev_right_vector			( 0.0f, 0.0f, 0.0f ),
	m_prev_up_vector			( 0.0f, 0.0f, 0.0f )
// 34 target lines are likely retail-compiled-out source.
{
	// 18 target lines are likely retail-compiled-out source.
	m_clouds_scale_multiplier *= options::ref( ).current.m_clouds_scale; m_clouds_scale = float3( static_cast<float>( m_clouds_size_x ), static_cast<float>( m_clouds_size_y ), static_cast<float>( m_clouds_size_z ) ) * m_clouds_scale_multiplier;

	for ( u32 i = 0; i < 2; ++i )
	{
		m_3d_clouds_density_texture[i] = resource_manager::ref( ).create_texture3d( i == 0 ? "$user$cloud_density0" : "$user$cloud_density1",
			m_clouds_size_x,
			m_clouds_size_y,
			m_clouds_size_z,
			0,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_USAGE_DYNAMIC,
			1
		);
	}
	m_3d_clouds_density_texture_left = m_3d_clouds_density_texture[0];
	m_3d_clouds_density_texture_right = m_3d_clouds_density_texture[1];

	if ( m_context->scene( ) && m_context->scene( )->get_clouds( ) )
	{
		fill_cloud_texture( 0 );
		fill_cloud_texture( 1 );
	}

	m_interp_textures.cloud_density_0 = m_3d_clouds_density_texture_left;
	m_interp_textures.cloud_density_1 = m_3d_clouds_density_texture_right;

	effect_manager::ref( ).create_effect<effect_clouds>( &m_clouds_effect );
	effect_manager::ref( ).create_effect<effect_read_cloud_base>( &m_read_cloud_base_effect );
	effect_manager::ref( ).create_effect<effect_clouds_god_rays>( &m_god_rays_effect );

	m_c_sphere_to_sky_matrix = backend::ref( ).register_constant_host( "sphere_to_sky_matrix", rc_float );
	m_c_clouds_grid_size = backend::ref( ).register_constant_host( "clouds_grid_size", rc_float );
	m_c_cloud_movement = backend::ref( ).register_constant_host( "cloud_movement", rc_float );
	m_c_interp_alpha = backend::ref( ).register_constant_host( "interp_alpha", rc_float );
	m_c_clouds_offset = backend::ref( ).register_constant_host( "clouds_offset", rc_float );
	m_c_layer_height = backend::ref( ).register_constant_host( "layer_height", rc_float );
	m_c_cloud_base = backend::ref( ).register_constant_host( "cloud_base", rc_float );
	m_c_light_multiplier_parameters = backend::ref( ).register_constant_host( "light_multiplier_parameters", rc_float );
	m_c_inverted_view_projection_matrix = backend::ref( ).register_constant_host( "inverted_view_projection_matrix", rc_float );
	m_to_sun_direction_parameter = backend::ref( ).register_constant_host( "to_sun_direction_parameter", rc_float );

	m_enabled = options::ref( ).current.m_enabled_clouds_stage;

	D3D_INPUT_ELEMENT_DESC const screen_vertex_layout[] = { { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D_INPUT_PER_VERTEX_DATA, 0 }, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D_INPUT_PER_VERTEX_DATA, 0 } };

	u16 indices[6] = { 0, 1, 2, 3, 2, 1 };
	m_screen_vertex_ib = resource_manager::ref( ).create_buffer( 6 * sizeof(u16), indices, enum_buffer_type_index, false, false );

	m_screen_vertex_geometry = resource_manager::ref( ).create_geometry( screen_vertex_layout, sizeof(screen_vertex), backend::ref( ).vertex.buffer( ), *m_screen_vertex_ib );
}

void stage_clouds::fill_cloud_texture( u32 index )
{
	u32 row_pitch = 0;

	pvoid const data = m_3d_clouds_density_texture[index]->map3D( D3D11_MAP_WRITE_DISCARD, 0, row_pitch );

	u32 const data_size = m_clouds_size_x * m_clouds_size_y * m_clouds_size_z * sizeof(voxel);

	if ( data )
	{

		memory::copy( data, data_size, index == 0 ? m_context->scene( )->get_clouds( )->m_cloud_simulation_0->get_voxels( ) : m_context->scene( )->get_clouds( )->m_cloud_simulation_1->get_voxels( ), data_size );
	}

	m_3d_clouds_density_texture[index]->unmap3D( 0 );
}

bool stage_clouds::is_effects_ready( ) const
{

	return m_clouds_effect.c_ptr( ) != 0 &&
		m_read_cloud_base_effect.c_ptr( ) != 0 &&
		m_god_rays_effect.c_ptr( ) != 0;
}

stage_clouds::~stage_clouds( )
{
	tasks::wait_for_task_list( &m_parent_task );
}
// 21 target lines are likely retail-compiled-out source.
void stage_clouds::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	if ( !m_context->scene( )->get_clouds( ) )
		return;

	if (
		m_context->scene( ) &&
		m_context->scene( )->get_clouds( )->m_is_updated
	)
	{
		fill_cloud_texture( 0 );
		fill_cloud_texture( 1 );
	}

	m_fixed_time = 0.0f;

	float const interp_alpha = m_context->scene( )->get_clouds( )->m_interp_alpha;

	cloud_key_parameters interp_key = m_context->scene( )->get_clouds( )->m_interp_key;
	float4x4 sphere_to_clouds_matrix;

	math::try_invert4x4(
		math::create_scale(
			float3(
				60000.0f * options::ref( ).current.m_clouds_scale,
				interp_key.layer_height * 1000.0f * options::ref( ).current.m_clouds_scale,
				60000.0f * options::ref( ).current.m_clouds_scale
			)
		) * math::create_translation(
			float3(
				0.0f,
				interp_key.cloud_base * options::ref( ).current.m_clouds_scale,
				0.0f
			)
		),
		sphere_to_clouds_matrix
	);

	float4x4 world_to_god_rays_matrix; math::try_invert4x4(
		math::create_scale(
			float3(
				60000.0f * options::ref( ).current.m_clouds_scale,
				math::max( 0.01f, interp_key.cloud_base * 2.0f ) *
					options::ref( ).current.m_clouds_scale,
				60000.0f * options::ref( ).current.m_clouds_scale
			)
		) * math::create_translation( float3( 0.0f, 0.0f, 0.0f ) ),
		world_to_god_rays_matrix
	);

	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_1 ), 0, 0, 0 );
	backend::ref( ).clear_render_targets( 0.0f, 0.0f, 0.0f, 0.0f );

	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), &*m_context->get_rt( rt_generic_1 ), 0, 0 );
	backend::ref( ).reset_depth_stencil_target( );

	static bool s_first_pass = true;
	if ( s_first_pass )
	{
		m_previous_view_position = m_context->get_view_pos( );
		s_first_pass = false;
	}

	float2 const view_dir_2d = math::normalize_safe( float2( m_context->get_view_dir( ).x, m_context->get_view_dir( ).z ), float2( 0.0f, 0.0f ) );
	float2 const offset_vector_ground(
		m_context->get_view_pos( ).x - m_previous_view_position.x,
		m_context->get_view_pos( ).z - m_previous_view_position.z
	);
	float2 const offset_direction_ground = math::normalize_safe( offset_vector_ground, float2( 0.0f, 0.0f ) );
	float const abs_ov_dot_dir_ground = offset_direction_ground | view_dir_2d;

	if ( abs_ov_dot_dir_ground > 0.0f )
	{ m_camera_offset += offset_vector_ground.length( ) / m_clouds_scale_multiplier * math::abs( abs_ov_dot_dir_ground ); }
	if ( abs_ov_dot_dir_ground < 0.0f )
	{ m_camera_offset -= offset_vector_ground.length( ) / m_clouds_scale_multiplier * math::abs( abs_ov_dot_dir_ground ); }

	m_previous_view_position = m_context->get_view_pos( );

	float3 const view_dir_2d2 = math::normalize_safe(
		float3( m_context->get_view_dir( ).x, 0.0f, m_context->get_view_dir( ).z ),
		float3( 0.0f, 0.0f, 0.0f )

	);

	if ( options::ref( ).current.m_clouds_allow_moving )
	{
		float const wind_dot_view_direction = m_wind_direction | view_dir_2d2;
		if ( math::abs( wind_dot_view_direction ) > 0.0f )
		{
			m_camera_offset += math::sign( wind_dot_view_direction ) * ( m_wind_direction * 0.0125f ).length( ) / m_wind_direction.length( ) * math::abs( wind_dot_view_direction ) * m_clouds_scale_multiplier;
		}
		m_wind_offset += m_wind_direction * ( m_clouds_scale_multiplier * 0.0125f ) * m_clouds_scale_multiplier;
	}

	if ( m_camera_offset >= 1.0f )
	{
		m_camera_offset = math::abs( m_camera_offset ) - math::abs( static_cast<s32>( m_camera_offset ) );
	}
	else if ( m_camera_offset < 0.0f )
	{
		if ( math::abs( m_camera_offset ) >= 1.0f )
			m_camera_offset = 1.0f - frac( m_camera_offset );
	}

	m_simulation.cloud_offset = m_wind_offset;

	m_simulation.world_to_cloud = sphere_to_clouds_matrix;
	m_simulation.interp_alpha = interp_alpha;
	m_interp_textures.cloud_density_0 = m_3d_clouds_density_texture_left;

	m_interp_textures.cloud_density_1 = m_3d_clouds_density_texture_right;

	float3 to_sun_direction( 0.0f, 1.0f, 0.0f );

	light* const sun = m_context->scene( )->lights( ).get_sun( ).c_ptr( );

	if ( sun )
		to_sun_direction = -sun->direction;

	float4x4 proj_matrix = m_context->get_p( );

	proj_matrix.k.z = 1.0000001f; proj_matrix.c.z = -1.0000001f;

	m_context->push_set_p( proj_matrix );

	for ( s32 i = options::ref( ).current.m_clouds_num_evaluate_slices; i > 0; --i )
	{
		float const cloud_base = interp_key.cloud_base;
		float3 const horizont_down_offset(
			0.0f,
			math::abs( cloud_base ) * 0.0f * 1.25f,
			0.0f

		);
		{
			float scale = ( static_cast<float>( i ) - m_camera_offset ) * m_clouds_scale_multiplier;
			float4x4 world_matrix = math::create_scale( float3( scale, scale, scale ) ) * math::create_translation( m_previous_view_position );
			m_clouds_effect->apply( 0, 0 );
			m_context->set_w( world_matrix );
			backend::ref( ).set_ps_constant( m_c_sphere_to_sky_matrix, math::transpose( sphere_to_clouds_matrix ) );
			backend::ref( ).set_ps_constant( m_c_clouds_grid_size, float3( static_cast<float>( m_clouds_size_x ), static_cast<float>( m_clouds_size_y ), static_cast<float>( m_clouds_size_z ) ) );
			backend::ref( ).set_ps_constant( m_c_clouds_offset, m_wind_offset + horizont_down_offset );
			backend::ref( ).set_ps_constant( m_c_interp_alpha, interp_alpha );
			backend::ref( ).set_ps_constant( m_c_layer_height, math::max( 0.5f, interp_key.layer_height ) );
			backend::ref( ).set_ps_constant( m_c_cloud_base, cloud_base );
			backend::ref( ).set_ps_constant( m_c_light_multiplier_parameters, float4( interp_key.direct_light, interp_key.indirect_light, interp_key.ambient, 0.0f ) );
			backend::ref( ).set_ps_constant( m_to_sun_direction_parameter, to_sun_direction );
			backend::ref( ).set_ps_constant( m_c_inverted_view_projection_matrix, math::transpose( math::invert4x4( m_context->get_vp( ) ) ) );

			backend::ref( ).set_ps_texture( "t_clouds_0", m_3d_clouds_density_texture_left.c_ptr( ) );
			backend::ref( ).set_ps_texture( "t_clouds_1", m_3d_clouds_density_texture_right.c_ptr( ) );

			m_evaluate_geometry.draw( );
		}

		if ( options::ref( ).current.m_use_god_rays )
		{

			float scale = ( static_cast<float>( i ) - m_camera_offset ) * m_clouds_scale_multiplier;

			float4x4 world_matrix = math::create_scale( float3( scale, scale, scale ) ) * math::create_translation( m_previous_view_position ); m_god_rays_effect->apply( 0, 0 );

			m_context->set_w( world_matrix );
			backend::ref( ).set_ps_constant( m_c_sphere_to_sky_matrix, math::transpose( world_to_god_rays_matrix ) );
			backend::ref( ).set_ps_constant( m_c_clouds_offset, m_wind_offset );
			backend::ref( ).set_ps_constant( m_c_interp_alpha, interp_alpha );
			backend::ref( ).set_ps_constant( m_c_cloud_base, cloud_base );
			backend::ref( ).set_ps_texture( "t_clouds_0", m_3d_clouds_density_texture_left.c_ptr( ) );
			backend::ref( ).set_ps_texture( "t_clouds_1", m_3d_clouds_density_texture_right.c_ptr( ) );
			backend::ref( ).set_ps_constant( m_to_sun_direction_parameter, to_sun_direction );

			m_evaluate_geometry.draw( );
		}
	}

	m_context->pop_p( );
	// 42 target lines are likely retail-compiled-out source.
	backend::ref( ).reset_render_targets( );
	// 23 target lines are likely retail-compiled-out source.
	m_interp_textures.cloud_density_0 = m_3d_clouds_density_texture[0];
	m_interp_textures.cloud_density_1 = m_3d_clouds_density_texture[1];

	m_fixed_time += 0.0125f;
}

} // namespace render
} // namespace vostok
