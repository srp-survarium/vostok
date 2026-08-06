#include "pch.h"
#include "hw_hiz_occlusion_manager.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/resource_manager.h>

#include "renderer_context.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

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
	u32 const in_num_bounds_and_results
)
{
	if ( !in_num_bounds_and_results || !m_hiz_occlusion_effect )
		return;

	backend::ref( ).draw_calls_counting = false;
	if ( m_use_scene_depth_buffer )
		copy_scene_depth( );
	else
		render_occluders( in_context );

	downsample_occlusion_buffer( );
	render_model_bounds( in_context, in_bounds, in_num_bounds_and_results );
	device::ref( ).d3d_context( )->CopyResource(
		m_t_culling_result_lockable->hw_texture( ),
		m_t_culling_result->hw_texture( )
	);
	backend::ref( ).draw_calls_counting = true;
}

void hw_hiz_occlusion_manager::copy_scene_depth( )
{
	m_hiz_occlusion_effect->apply( 2, 0 );
	system_renderer::ref( ).fill_surface(
		m_rt_depth_mips[0],
		render_target_ptr( ),
		render_target_ptr( ),
		render_target_ptr( ),
		render_target_ptr( ),
		false,
		0,
		0.f,
		0.f,
		1.f,
		1.f
	);
}

void hw_hiz_occlusion_manager::render_occluders( renderer_context* in_context )
{
	static float clear_value = 1.f;

	D3D11_VIEWPORT prev_view_port;
	backend::ref( ).get_viewport( prev_view_port );

	D3D11_VIEWPORT view_port;
	view_port.Width = float( m_rasterize_width );
	view_port.Height = float( m_rasterize_height );
	view_port.TopLeftX = 0.f;
	view_port.TopLeftY = 0.f;
	view_port.MinDepth = 0.f;
	view_port.MaxDepth = clear_value;
	backend::ref( ).set_viewport( view_port );

	backend::ref( ).set_render_targets( m_rt_depth_mips[0].c_ptr( ), 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( m_ds_occlusion_depth.c_ptr( ) );
	backend::ref( ).clear_render_targets( 1.f, 1.f, 1.f, clear_value );
	backend::ref( ).clear_depth_stencil(
		D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL,
		clear_value,
		0
	);

	in_context->set_w( math::create_scale( float3( 3.f, 3.f, 3.f ) ) );
	m_hiz_occlusion_effect->apply( 3, 0 );
	m_sphere_occluder_geometry.render( );

	backend::ref( ).set_viewport( prev_view_port );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

void hw_hiz_occlusion_manager::render_debug(
	renderer_context* in_context,
	float4 const* in_bounds,
	pcbyte in_results,
	u32 const in_num_bounds_and_results
)
{
	if ( !m_hiz_occlusion_effect || !in_num_bounds_and_results )
		return;

	float const scale = 0.01f;
	for ( u32 i = 0; i < in_num_bounds_and_results; ++i, ++in_bounds ) {
		float4x4 const world_matrix =
			math::create_scale( float3( in_bounds->w, in_bounds->w, in_bounds->w ) ) *
			math::create_translation( in_bounds->xyz( ) );
		in_context->set_w( world_matrix );
		m_hiz_occlusion_effect->apply( 1, 0 );
		backend::ref( ).set_ps_constant(
			m_draw_color_parameter,
			float4( scale, scale, scale, 0.f )
		);
		m_sphere_occluder_geometry.render( );
	}
}

void hw_hiz_occlusion_manager::downsample_occlusion_buffer( )
{
	static bool is_save = false;
	static float clear_value = 1.f;

	for ( u32 mip_level_index = 1; mip_level_index < m_num_mips; ++mip_level_index ) {
		D3D11_VIEWPORT prev_view_port;
		backend::ref( ).get_viewport( prev_view_port );

		D3D11_VIEWPORT view_port;
		view_port.Width = float( m_rt_depth_mips[mip_level_index]->width( ) );
		view_port.Height = float( m_rt_depth_mips[mip_level_index]->height( ) );
		view_port.TopLeftX = 0.f;
		view_port.TopLeftY = 0.f;
		view_port.MinDepth = 0.f;
		view_port.MaxDepth = clear_value;
		backend::ref( ).set_viewport( view_port );

		backend::ref( ).set_render_targets(
			m_rt_depth_mips[mip_level_index].c_ptr( ),
			0,
			0,
			0
		);
		backend::ref( ).clear_render_targets( 1.f, 1.f, 1.f, clear_value );
		backend::ref( ).set_viewport( prev_view_port );
	}

	res_texture_ptr read_texture = m_t_depth_mips;
	for ( u32 mip_level_index = 1; mip_level_index < m_num_mips; ++mip_level_index ) {
		m_hiz_occlusion_effect->apply( 4, 0 );
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
		system_renderer::ref( ).fill_surface(
			m_rt_depth_mips_work[mip_level_index],
			render_target_ptr( ),
			render_target_ptr( ),
			render_target_ptr( ),
			render_target_ptr( ),
			false,
			0,
			0.f,
			0.f,
			1.f,
			1.f
		);
		read_texture = m_t_depth_mips_work[mip_level_index];
	}

	for ( u32 mip_level_index = 1; mip_level_index < m_num_mips; ++mip_level_index ) {
		m_hiz_occlusion_effect->apply( 5, 0 );
		backend::ref( ).set_ps_texture(
			"hiz_depth_texture",
			m_t_depth_mips_work[mip_level_index].c_ptr( )
		);
		system_renderer::ref( ).fill_surface(
			m_rt_depth_mips[mip_level_index],
			render_target_ptr( ),
			render_target_ptr( ),
			render_target_ptr( ),
			render_target_ptr( ),
			false,
			0,
			0.f,
			0.f,
			1.f,
			1.f
		);
	}

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

void hw_hiz_occlusion_manager::render_model_bounds(
	renderer_context* in_context,
	float4 const* in_bounds,
	u32 const in_num_bounds
)
{
	static float clear_value = 1.f;

	check_culling_buffer( in_num_bounds );
	m_hw_hiz_point_list.set_points( in_bounds, m_culling_buffer_width );
	in_context->set_w( float4x4( ).identity( ) );

	D3D11_VIEWPORT prev_view_port;
	backend::ref( ).get_viewport( prev_view_port );

	D3D11_VIEWPORT view_port;
	view_port.Width = float( m_culling_buffer_width );
	view_port.Height = float( m_culling_buffer_height );
	view_port.TopLeftX = 0.f;
	view_port.TopLeftY = 0.f;
	view_port.MinDepth = 0.f;
	view_port.MaxDepth = clear_value;
	backend::ref( ).set_viewport( view_port );

	backend::ref( ).set_render_targets( m_rt_culling_result.c_ptr( ), 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( 0 );
	backend::ref( ).clear_render_targets( 0.f, 0.f, 0.f, 0.f );
	m_hiz_occlusion_effect->apply( 6, 0 );
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

bool hw_hiz_occlusion_manager::quary_and_get_results_if_ready(
	pbyte out_results,
	u32 const in_num_results
)
{
	static bool is_save = false;

	if ( !in_num_results || !m_t_culling_result_lockable )
		return true;

	u32 row_pitch = 0;
	pbyte data = static_cast< pbyte >(
		m_t_culling_result_lockable->map2D(
			D3D11_MAP_READ,
			0,
			row_pitch,
			in_num_results != 0
		)
	);
	if ( !data )
		return false;

	for ( u32 y = 0; y < m_culling_buffer_height; ++y ) {
		for ( u32 x = 0; x < m_culling_buffer_width; ++x ) {
			u32 const index = y * m_culling_buffer_width + x;
			if ( index > in_num_results - 1 )
				goto unmap;
			out_results[index] = data[x];
		}
		data += row_pitch;
	}

unmap:
	m_t_culling_result_lockable->unmap2D( 0 );
	return true;
}

void hw_hiz_occlusion_manager::check_culling_buffer( u32 const in_num_bounds )
{
	m_current_num_bounds = in_num_bounds;
	if ( in_num_bounds <= m_culling_buffer_width * m_culling_buffer_height )
		return;

	m_culling_buffer_width = 256;
	m_culling_buffer_height = math::floor(
		float( in_num_bounds ) / float( m_culling_buffer_width ) + 1.f
	);
	m_hw_hiz_point_list.initialize( m_culling_buffer_width * m_culling_buffer_height );

	m_t_culling_result_lockable = 0;
	m_t_culling_result_lockable = resource_manager::ref( ).create_texture2d(
		"$user$hiz_lockable",
		m_culling_buffer_width,
		m_culling_buffer_height,
		0,
		DXGI_FORMAT_R8_UNORM,
		D3D11_USAGE_STAGING,
		1,
		1,
		false
	);

	m_rt_culling_result = 0;
	m_rt_culling_result = resource_manager::ref( ).create_render_target(
		"$user$hiz_result",
		m_culling_buffer_width,
		m_culling_buffer_height,
		DXGI_FORMAT_R8_UNORM,
		enum_rt_usage_render_target,
		res_texture_ptr( ),
		0,
		D3D11_USAGE_DEFAULT,
		1,
		0
	);
	m_t_culling_result = m_rt_culling_result->get_texture( );
}

} // namespace render
} // namespace vostok
