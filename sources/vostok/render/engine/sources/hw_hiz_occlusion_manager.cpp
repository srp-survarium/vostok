#include "pch.h"
#include "hw_hiz_occlusion_manager.h"

#include <vostok/fixed_string.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/resource_manager.h>

#include "effect_hiz_occlusion.h"
#include "renderer_context.h"
#include "system_renderer.h"

namespace vostok {
namespace render {
// 6 target lines are likely retail-compiled-out source.
hw_hiz_occlusion_manager::hw_hiz_occlusion_manager(
	bool const	use_scene_depth_buffer,
	u32 const	rasterize_width,
	u32 const	rasterize_height
) :
	m_use_scene_depth_buffer			( use_scene_depth_buffer ),
	m_rasterize_width				( rasterize_width ),
	m_rasterize_height				( rasterize_height ),
	m_num_mips						(
		u32(
			::log( double( math::max( rasterize_width, rasterize_height ) ) ) /
			::log( 2.0 )
		) + 1
	),
	m_source_mip_level_parameter		( 0 ),
	m_draw_color_parameter			( 0 ),
	m_render_target_size_parameter	( 0 ),
	m_rasterize_size_parameter		( 0 ),
	m_prev_texture_size_parameter	( 0 ),
	m_current_num_bounds			( 0 ),
	m_culling_buffer_width			( 0 ),
	m_culling_buffer_height			( 0 )
{
	for ( u32 mip_index = 0; mip_index < m_num_mips; ++mip_index ) {
		fixed_string<128> name;
		// 3 target lines are likely retail-compiled-out source.
		name.assignf( "%s_work_%d", "$user$hiz_occlusion_depth_mips", mip_index );
		m_rt_depth_mips_work[mip_index] = resource_manager::ref( ).create_render_target(
			name.c_str( ),
			rasterize_width >> mip_index,
			rasterize_height >> mip_index,
			DXGI_FORMAT_R32_FLOAT,
			enum_rt_usage_render_target,
			res_texture_ptr( ),
			0,
			D3D11_USAGE_DEFAULT,
			1,
			0 );
		m_t_depth_mips_work[mip_index] = m_rt_depth_mips_work[mip_index]->get_texture( );
	}

	m_t_depth_mips = resource_manager::ref( ).create_texture2d(
		"$user$hiz_occlusion_depth_mips",
		rasterize_width,
		rasterize_height,
		0,
		DXGI_FORMAT_R32_FLOAT,
		D3D11_USAGE_DEFAULT,
		m_num_mips,
		1,
		true
	);

	m_t_depth_mips_lockable = resource_manager::ref( ).create_texture2d(
		"$user$hiz_occlusion_depth_mips_l",
		rasterize_width,
		rasterize_height,
		0,
		DXGI_FORMAT_R32_FLOAT,
		D3D11_USAGE_STAGING,
		1,
		1,
		false
	);

	for ( u32 mip_index = 0; mip_index < m_num_mips; ++mip_index ) {
		u32 current_rasterize_height =

			rasterize_height >> mip_index;
		fixed_string<128> name;

		name.assignf( "%s_%d", "$user$hiz_occlusion_depth_mips", mip_index );

		m_rt_depth_mips[mip_index] = resource_manager::ref( ).create_render_target(
			name.c_str( ),
			rasterize_width >> mip_index,
			current_rasterize_height,
			DXGI_FORMAT_R32_FLOAT,
			enum_rt_usage_render_target,
			m_t_depth_mips,
			0,
			D3D11_USAGE_DEFAULT,
			1,
			mip_index );
	}

	m_ds_occlusion_depth = resource_manager::ref( ).create_render_target(
		"$user$hiz_occlusion_depth_mips_d", rasterize_width, rasterize_height,
		DXGI_FORMAT_R24G8_TYPELESS, enum_rt_usage_depth_stencil,
		res_texture_ptr( ),
		0, D3D11_USAGE_DEFAULT,
		1, 0
	);

	effect_manager::ref( ).create_effect<effect_hiz_occlusion>( &m_hiz_occlusion_effect );
	m_source_mip_level_parameter =
		backend::ref( ).register_constant_host( "source_mip_level", rc_int );
	m_draw_color_parameter = backend::ref( ).register_constant_host( "draw_color", rc_float );
	m_render_target_size_parameter = backend::ref( ).register_constant_host( "render_target_size", rc_float );
	m_rasterize_size_parameter = backend::ref( ).register_constant_host( "rasterize_size", rc_float );
	m_prev_texture_size_parameter = backend::ref( ).register_constant_host( "prev_texture_size", rc_float );

}

hw_hiz_occlusion_manager::~hw_hiz_occlusion_manager( )
{

}

bool hw_hiz_occlusion_manager::is_ready_effects( ) const
{

	return !!m_hiz_occlusion_effect;
}

void hw_hiz_occlusion_manager::process_culling(
	renderer_context* in_context,
	float4 const* in_bounds,
	u32 const in_num_bounds_and_results )
{


	if ( !in_num_bounds_and_results ||


		!m_hiz_occlusion_effect )
		return;

	backend::ref( ).draw_calls_counting = false;

	if ( m_use_scene_depth_buffer )
		copy_scene_depth( );
	else
		render_occluders( in_context );

	downsample_occlusion_buffer( );

	render_model_bounds( in_context, in_bounds, in_num_bounds_and_results );
	// 11 target lines are likely retail-compiled-out source.
	device::ref( ).d3d_context( )->CopyResource(
		m_t_culling_result_lockable->hw_texture( ),
		m_t_culling_result->hw_texture( )
	);

	backend::ref( ).draw_calls_counting = true;
}

void hw_hiz_occlusion_manager::copy_scene_depth( )
{


	m_hiz_occlusion_effect->apply( effect_hiz_occlusion::hiz_copy_scene_depth_pass, 0 );
	system_renderer::ref( ).fill_surface( m_rt_depth_mips[0], render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), false, 0, 0.f, 0.f, 1.f, 1.f );
}

void hw_hiz_occlusion_manager::render_occluders( renderer_context* in_context )
{
	static float clear_value = 1.f;

	D3D11_VIEWPORT prev_view_port;

	backend::ref( ).get_viewport( prev_view_port );

	D3D11_VIEWPORT view_port; view_port.Width = float( m_rasterize_width );
	view_port.Height = float( m_rasterize_height );
	view_port.TopLeftX = 0.f;
	view_port.TopLeftY = 0.f;
	view_port.MinDepth = 0.f;
	view_port.MaxDepth = clear_value;

	backend::ref( ).set_viewport( view_port );
	// 3 target lines are likely retail-compiled-out source.


	backend::ref( ).set_render_targets( m_rt_depth_mips[0].c_ptr( ), 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( m_ds_occlusion_depth.c_ptr( ) );
	backend::ref( ).clear_render_targets( 1.f, 1.f, 1.f, clear_value );
	backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, clear_value, 0 );
	// 3 target lines are likely retail-compiled-out source.


	in_context->set_w( math::create_scale( float3( 3.f, 3.f, 3.f ) ) );
	m_hiz_occlusion_effect->apply( effect_hiz_occlusion::hiz_occluders_depth_pass, 0 );
	m_sphere_occluder_geometry.render( );

	backend::ref( ).set_viewport( prev_view_port );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

void hw_hiz_occlusion_manager::render_debug(
	renderer_context* in_context,
	float4 const* in_bounds,
	pcbyte in_results, u32 const in_num_bounds_and_results )
{
	if ( !m_hiz_occlusion_effect ||


		!in_num_bounds_and_results )
		return;

	float const scale = 0.01f;
	for ( u32 i = 0; i < in_num_bounds_and_results; ++i, ++in_bounds ) {
		float4x4 const world_matrix =
			math::create_scale( float3( in_bounds->w, in_bounds->w, in_bounds->w ) ) *
			math::create_translation( in_bounds->xyz( ) );
		in_context->set_w( world_matrix ); m_hiz_occlusion_effect->apply( effect_hiz_occlusion::hiz_render_debug_geometry_invisible_pass, 0 ); backend::ref( ).set_ps_constant( m_draw_color_parameter, float4( scale, scale, scale, 0.f ) ); m_sphere_occluder_geometry.render( );
	}
}

void hw_hiz_occlusion_manager::downsample_occlusion_buffer( )
{
	static bool is_save = false;

	for ( u32 mip_level_index = 1; mip_level_index < m_num_mips; ++mip_level_index ) {
		D3D11_VIEWPORT prev_view_port;

		backend::ref( ).get_viewport( prev_view_port );
		D3D11_VIEWPORT view_port; view_port.Width = float( m_rt_depth_mips[mip_level_index]->width( ) );
		view_port.Height = float( m_rt_depth_mips[mip_level_index]->height( ) );
		view_port.TopLeftX = 0.f;
		view_port.TopLeftY = 0.f;
		view_port.MinDepth = 0.f;
		view_port.MaxDepth = 1.f;
		backend::ref( ).set_viewport( view_port );

		backend::ref( ).set_render_targets( m_rt_depth_mips[mip_level_index].c_ptr( ), 0, 0, 0 );
		backend::ref( ).clear_render_targets( 1.f, 1.f, 1.f, 1.f );

		backend::ref( ).set_viewport( prev_view_port );
	}
	// 3 target lines are likely retail-compiled-out source.
	res_texture_ptr read_texture = m_t_depth_mips;

	for ( u32 mip_level_index = 1; mip_level_index < m_num_mips; ++mip_level_index ) {
		m_hiz_occlusion_effect->apply( effect_hiz_occlusion::hiz_downsample_depth_pass, 0 );
		backend::ref( ).set_ps_texture( "hiz_depth_texture", read_texture.c_ptr( ) );
		backend::ref( ).set_ps_constant(
			m_prev_texture_size_parameter,
			float4(
				float( read_texture->width( ) ),
				float( read_texture->height( ) ),
				0.f,
				0.f
			)


		);

		system_renderer::ref( ).fill_surface( m_rt_depth_mips_work[mip_level_index], render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), false, 0, 0.f, 0.f, 1.f, 1.f );

		read_texture = m_t_depth_mips_work[mip_level_index];
	}


	for ( u32 mip_level_index = 1; mip_level_index < m_num_mips; ++mip_level_index ) {
		// 4 target lines are likely retail-compiled-out source.
		m_hiz_occlusion_effect->apply( effect_hiz_occlusion::hiz_merge_mip_pass, 0 );
		backend::ref( ).set_ps_texture( "hiz_depth_texture", m_t_depth_mips_work[mip_level_index].c_ptr( ) );
		system_renderer::ref( ).fill_surface( m_rt_depth_mips[mip_level_index], render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), false, 0, 0.f, 0.f, 1.f, 1.f );
	}
	// 3 target lines are likely retail-compiled-out source.
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
// 3 target lines are likely retail-compiled-out source.
}

void hw_hiz_occlusion_manager::render_model_bounds(
	renderer_context* in_context,
	float4 const* in_bounds,
	u32 const in_num_bounds )
{


	check_culling_buffer( in_num_bounds );

	m_hw_hiz_point_list.set_points( in_bounds, m_culling_buffer_width );

	in_context->set_w( float4x4( ).identity( ) );

	D3D11_VIEWPORT prev_view_port;


	backend::ref( ).get_viewport( prev_view_port );

	D3D11_VIEWPORT view_port; view_port.Width = float( m_culling_buffer_width );
	view_port.Height = float( m_culling_buffer_height );
	view_port.TopLeftX = 0.f;
	view_port.TopLeftY = 0.f;
	view_port.MinDepth = 0.f;
	view_port.MaxDepth = 1.f;

	backend::ref( ).set_viewport( view_port );
	backend::ref( ).set_render_targets( m_rt_culling_result.c_ptr( ), 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( 0 );
	backend::ref( ).clear_render_targets( 0.f, 0.f, 0.f, 0.f );

	m_hiz_occlusion_effect->apply( effect_hiz_occlusion::hiz_fill_culling_results_buffer_pass, 0 );
	backend::ref( ).set_ps_texture( "hiz_depth_texture", m_t_depth_mips.c_ptr( ) );
	backend::ref( ).set_vs_constant(
		m_render_target_size_parameter,
		float4(
			float( m_culling_buffer_width ),
			float( m_culling_buffer_height ),
			0.f,
			0.f
		)
	);
	backend::ref( ).set_ps_constant(
		m_rasterize_size_parameter,
		float4(
			float( m_rasterize_width ),
			float( m_rasterize_height ),
			0.f,
			0.f
		)
	);
	m_hw_hiz_point_list.render( m_current_num_bounds );

	backend::ref( ).set_viewport( prev_view_port );
}

bool hw_hiz_occlusion_manager::quary_and_get_results_if_ready( pbyte out_results, u32 const in_num_results )
{
	static bool is_save = false;
	if ( !in_num_results || !m_t_culling_result_lockable )
		return true;

	u32 row_pitch = 0;
	pbyte data = static_cast< pbyte >( m_t_culling_result_lockable->map2D( D3D11_MAP_READ, 0, row_pitch, in_num_results != 0 ) );

	if ( !data )
		return false;
	// 3 target lines are likely retail-compiled-out source.
	for ( u32 y = 0; y < m_culling_buffer_height; ++y ) {
		// 3 target lines are likely retail-compiled-out source.
		for ( u32 x = 0; x < m_culling_buffer_width; ++x ) {


			u32 const index = y * m_culling_buffer_width + x; if ( index > in_num_results - 1 )
				goto unmap;
			// 4 target lines are likely retail-compiled-out source.
			out_results[index] = data[x];
		}
		// 3 target lines are likely retail-compiled-out source.
		data += row_pitch;
	}
unmap:
	m_t_culling_result_lockable->unmap2D( 0 );
	// 7 target lines are likely retail-compiled-out source.
	return true;
}

void hw_hiz_occlusion_manager::check_culling_buffer( u32 const in_num_bounds )
{
	m_current_num_bounds = in_num_bounds;
	if ( in_num_bounds <= m_culling_buffer_width * m_culling_buffer_height )
		return;

	m_culling_buffer_width = 256;

	m_culling_buffer_height = math::floor( float( in_num_bounds ) / float( m_culling_buffer_width ) + 1.f );

	m_hw_hiz_point_list.initialize( m_culling_buffer_width * m_culling_buffer_height );

	m_t_culling_result_lockable = 0;
	m_t_culling_result_lockable = resource_manager::ref( ).create_texture2d(
		"$user$hiz_lockable",
		m_culling_buffer_width,
		m_culling_buffer_height,
		0,
		DXGI_FORMAT_R8_UNORM,
		D3D11_USAGE_STAGING,
		1, 1, false
	);

	m_rt_culling_result = 0;
	m_rt_culling_result = resource_manager::ref( ).create_render_target(
		"$user$hiz_result",
		m_culling_buffer_width, m_culling_buffer_height,
		DXGI_FORMAT_R8_UNORM, enum_rt_usage_render_target,
		res_texture_ptr( ),
		0, D3D11_USAGE_DEFAULT, 1, 0
	);

	m_t_culling_result = m_rt_culling_result->get_texture( );
}

} // namespace render
} // namespace vostok
