#include "pch.h"
#include "stage_atmosphere.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>

#include "effect_atmospheric_scattering.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"

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
void stage_atmosphere::fill_surfaces(
	render_target_ptr	surf0,
	render_target_ptr	surf1,
	bool				use_base_depth_stencil
)
{

	if ( surf1.c_ptr( ) )
		backend::ref( ).set_render_targets( &*surf0, &*surf1, 0, 0 );
	else
		backend::ref( ).set_render_targets( &*surf0, 0, 0, 0 );

	if ( use_base_depth_stencil )
		backend::ref( ).reset_depth_stencil_target( );
	else
		backend::ref( ).set_depth_stencil_target( 0 );

	D3D11_VIEWPORT view_port;
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	view_port.Width = static_cast<float>( surf0->width( ) );
	view_port.Height = static_cast<float>( surf0->height( ) );
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;

	D3D11_VIEWPORT view_port_saved;
	u32 viewport_count = 1;
	device::ref( ).d3d_context( )->RSGetViewports( &viewport_count, &view_port_saved );

	device::ref( ).d3d_context( )->RSSetViewports( 1, &view_port );
	u32 offset;
	screen_vertex* pv = reinterpret_cast<screen_vertex*>( backend::ref( ).vertex.lock( 4, sizeof(screen_vertex), offset ) );
	pv->set( float4( -1.0f, -1.0f, 0.0f, 1.0f ), float2( 0.0f, 1.0f ) );
	++pv;
	pv->set( float4( -1.0f, 1.0f, 0.0f, 1.0f ), float2( 0.0f, 0.0f ) );
	++pv;
	pv->set( float4( 1.0f, -1.0f, 0.0f, 1.0f ), float2( 1.0f, 1.0f ) );
	++pv;
	pv->set( float4( 1.0f, 1.0f, 0.0f, 1.0f ), float2( 1.0f, 0.0f ) );
	backend::ref( ).vertex.unlock( );

	m_screen_vertex_geometry->apply( );

	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6, 0, offset );

	device::ref( ).d3d_context( )->RSSetViewports( 1, &view_port_saved );
}
stage_atmosphere::stage_atmosphere(
	renderer*			in_renderer,
	renderer_context*	context,
	stage_type			type
) :
	stage( in_renderer, context ),
	m_clouds_geometry( 16, 16 ), m_type( type ) {
	effect_manager::ref( ).create_effect<effect_atmospheric_scattering>( &m_atmospheric_scattering_effect );

	m_to_sun_direction_parameter = backend::ref( ).register_constant_host( "to_sun_direction_parameter", rc_float );
	m_c_inverted_view_projection_matrix = backend::ref( ).register_constant_host( "inverted_view_projection_matrix", rc_float );
	m_c_atmosphere_parameters = backend::ref( ).register_constant_host( "atmosphere_parameters", rc_float );
	m_c_inscatter_parameters = backend::ref( ).register_constant_host( "inscatter_parameters", rc_float );
	m_c_eye_ray_corner = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_sky_clouds_parameters0 = backend::ref( ).register_constant_host( "sky_clouds_parameters0", rc_float );
	m_sky_clouds_parameters1 = backend::ref( ).register_constant_host( "sky_clouds_parameters1", rc_float );
	m_sky_clouds_parameters2 = backend::ref( ).register_constant_host( "sky_clouds_parameters2", rc_float );
	m_sun_moon_parameters = backend::ref( ).register_constant_host( "sun_moon_parameters", rc_float );

	D3D_INPUT_ELEMENT_DESC const screen_vertex_layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D_INPUT_PER_VERTEX_DATA, 0 },
	};
	u16 indices[6] = { 0, 1, 2, 3, 2, 1 };
	m_screen_vertex_ib = resource_manager::ref( ).create_buffer( 6 * sizeof(u16), indices, enum_buffer_type_index, false, false );
	m_screen_vertex_geometry = resource_manager::ref( ).create_geometry( screen_vertex_layout, sizeof(screen_vertex), backend::ref( ).vertex.buffer( ), *m_screen_vertex_ib );

	m_enabled = options::ref( ).current.m_enabled_atmosphere_stage;
}

stage_atmosphere::~stage_atmosphere( )
{

}

bool stage_atmosphere::is_effects_ready( ) const
{
	return m_atmospheric_scattering_effect.c_ptr( ) != 0;
}

void stage_atmosphere::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	post_process_parameters& pp_parameters = m_context->get_scene_view( )->post_process_parameters( );
	light* sun = m_context->scene( )->lights( ).get_sun( ).c_ptr( );
	float3 to_sun_direction( 0.0f, 1.0f, 0.0f );

	if ( m_type == atmosphere_on_sky )
	{
		bool recalc_rayleigh_scattering =
			m_context->get_scene_view( )->m_atmosphere_prev_targets_size !=
			m_context->m_targets->get_id( );

		if ( sun )
		{
			if ( !sun->direction.is_similar( sun->previous_direction, math::epsilon_3 ) )
			{
				sun->previous_direction = sun->direction;
				recalc_rayleigh_scattering = true;
			}
			to_sun_direction = -sun->direction;

			if ( pp_parameters.atmosphere_params_changed )
			{
				pp_parameters.atmosphere_params_changed = false;
				recalc_rayleigh_scattering = true;
			}
		}

		if ( recalc_rayleigh_scattering )
		{
			m_context->get_scene_view( )->need_recalc_atmosphere = false;

			m_atmospheric_scattering_effect->apply( effect_atmospheric_scattering::make_mie_rayleigh_pass, 0 );
			float sun_int = sun ? sun->intensity : 1.0f;
			backend::ref( ).set_ps_constant( m_to_sun_direction_parameter, float4( to_sun_direction, sun_int ) );
			backend::ref( ).set_ps_constant( m_c_atmosphere_parameters, float4( pp_parameters.atmosphere_kresun_multiplier, pp_parameters.atmosphere_kmesun_multiplier, pp_parameters.atmosphere_kr4pi_multiplier, pp_parameters.atmosphere_km4pi_multiplier ) );
			fill_surfaces( m_context->get_rt( rt_mie_scattering ), m_context->get_rt( rt_rayleigh_scattering ), false );

			m_context->get_scene_view( )->m_atmosphere_prev_targets_size = m_context->m_targets->get_id( );
		}

		backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), 0, 0, 0 );
		backend::ref( ).reset_depth_stencil_target( );

		float sun_int = sun ? sun->intensity : 1.0f;
		if ( !pp_parameters.use_sun_moon_atmosphere_illumination )
			sun_int = 0.0f;

		m_atmospheric_scattering_effect->apply( effect_atmospheric_scattering::atmosphere_pass, 0 );
		backend::ref( ).set_ps_constant( m_to_sun_direction_parameter, float4( to_sun_direction, sun_int ) );
		backend::ref( ).set_vs_constant( m_to_sun_direction_parameter, float4( to_sun_direction, sun_int ) );

		float4x4 proj_matrix = m_context->get_p( );
		proj_matrix.k.z = 1.0000001f;
		proj_matrix.c.z = -1.0000001f;
		m_context->push_set_p( proj_matrix );
		m_context->set_w( float4x4( ).identity( ) );

		m_sky_dome_geometry.draw( );
		if ( pp_parameters.sky_clouds_texture )
		{

			m_atmospheric_scattering_effect->apply( pp_parameters.sky_clouds_blend_mode ? effect_atmospheric_scattering::clouds_pass_alpha_blend : effect_atmospheric_scattering::clouds_pass_modulate, 0 );
			backend::ref( ).set_ps_texture( "sky_clouds_texture", pp_parameters.sky_clouds_texture.c_ptr( ) );

			backend::ref( ).set_ps_constant( m_sky_clouds_parameters0, float4( pp_parameters.sky_clouds_color.xyz( ), pp_parameters.sky_clouds_u_tile ) );
			backend::ref( ).set_ps_constant( m_sky_clouds_parameters1, float4( pp_parameters.sky_clouds_fog_power, pp_parameters.sky_clouds_fog_up_limit, 0.0f, 0.0f ) );

			float const angle_in_rad = pp_parameters.sky_clouds_rotation / 180.0f * math::pi;
			backend::ref( ).set_vs_constant( m_sky_clouds_parameters2, float4( math::cos( angle_in_rad ), math::sin( angle_in_rad ), 0.0f, 0.0f ) );

			m_clouds_geometry.draw( );
		}
		if ( pp_parameters.sun_moon_texture && sun )
		{
			u32 offset;
			screen_vertex* pv = reinterpret_cast<screen_vertex*>( backend::ref( ).vertex.lock( 4, sizeof(screen_vertex), offset ) );

			pv->set( float4( -1.0f, -1.0f, 0.0f, 1.0f ), float2( 0.0f, 0.0f ) );
			++pv;
			pv->set( float4( -1.0f, 1.0f, 0.0f, 1.0f ), float2( 0.0f, 1.0f ) );
			++pv;
			pv->set( float4( 1.0f, -1.0f, 0.0f, 1.0f ), float2( 1.0f, 0.0f ) );
			++pv;
			pv->set( float4( 1.0f, 1.0f, 0.0f, 1.0f ), float2( 1.0f, 1.0f ) );

			backend::ref( ).vertex.unlock( );

			m_screen_vertex_geometry->apply( );
			m_atmospheric_scattering_effect->apply( effect_atmospheric_scattering::sun_moon_pass_for_alpha_blend_clouds, 0 );
			float3 L_dir = sun->direction;
			float3 L_right;
			float3 L_up;
			if ( sun->right.squared_length( ) > math::epsilon_5 )
			{

				L_up = sun->right;
				L_up.normalize( );
				L_right = L_up ^ L_dir;
				L_right.normalize( );
				L_up = L_right ^ L_dir;
				L_up.normalize( );
			} else {
				L_up.set( 0.0f, 1.0f, 0.0f );
				if ( math::abs( L_dir | L_up ) > 0.99f )
					L_up.set( 0.0f, 0.0f, 1.0f );
				L_right = L_up ^ L_dir;
				L_right.normalize( );
				L_up = L_right ^ L_dir;
				L_up.normalize( );
			}
			float scale = ( m_context->get_view_pos( ) - sun->position ).length( );
			scale /= 384467000.0f;
			scale *= 6948400.0f;
			float4x4 rotation_X_translation( float4( L_up, 0.0f ), float4( L_right, 0.0f ),
				float4( L_dir, 0.0f ), float4( sun->position, 1.0f ) );
			scale *= pp_parameters.sun_moon_billboard_scale;
			float4x4 world_transform =
				math::create_scale( float3( scale, scale, scale ) ) * math::create_rotation( rotation_X_translation.get_angles_xyz( ) ) * math::create_translation( sun->position + L_dir * 0.0f );
			m_context->set_w( world_transform );

			backend::ref( ).set_ps_texture( "sun_moon_texture", pp_parameters.sun_moon_texture.c_ptr( ) );
			backend::ref( ).set_ps_constant( m_sun_moon_parameters, float4( pp_parameters.sun_moon_color, 0.0f ) );

			backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6, 0, offset );

			m_context->set_w( float4x4( ).identity( ) );
		}
		m_context->pop_p( );

		backend::ref( ).reset_render_targets( );
	}

	if ( m_type == atmosphere_on_geometry )
	{
		float3 const* const eye_rays = m_context->get_eye_rays( );

		if ( pp_parameters.use_atmosphere_inscattering_on_geometry )
		{
			m_atmospheric_scattering_effect->apply( effect_atmospheric_scattering::scattering_on_geometry_pass_mul, 0 );
			backend::ref( ).set_ps_constant( m_to_sun_direction_parameter, float4( to_sun_direction, 1.0f ) );
			backend::ref( ).set_ps_constant( m_c_inverted_view_projection_matrix, math::transpose( math::invert4x4( m_context->get_vp( ) ) ) );
			backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( (float4*)eye_rays )[0] );
			backend::ref( ).set_ps_constant( m_c_inscatter_parameters, float4( pp_parameters.atmosphere_inscatter_distance, pp_parameters.atmosphere_inscatter_power, 0.0f, 0.0f ) );
			fill_surfaces( m_context->get_rt( rt_generic_0 ), 0, true );

			m_atmospheric_scattering_effect->apply( effect_atmospheric_scattering::scattering_on_geometry_pass_add, 0 );
			backend::ref( ).set_ps_constant( m_to_sun_direction_parameter, float4( to_sun_direction, 1.0f ) );
			backend::ref( ).set_ps_constant( m_c_inverted_view_projection_matrix, math::transpose( math::invert4x4( m_context->get_vp( ) ) ) );
			backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( (float4*)eye_rays )[0] );
			backend::ref( ).set_ps_constant( m_c_inscatter_parameters, float4( pp_parameters.atmosphere_inscatter_distance, pp_parameters.atmosphere_inscatter_power, 0.0f, 0.0f ) );
			fill_surfaces( m_context->get_rt( rt_generic_0 ), 0, true );
		}
	}

	backend::ref( ).reset_render_targets( );
}

} // namespace render
} // namespace vostok
