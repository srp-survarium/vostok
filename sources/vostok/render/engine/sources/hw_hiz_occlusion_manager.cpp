#include "pch.h"
// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (hierarchical-Z occlusion is new-in-target) - matcher-phase work.
#include "hw_hiz_occlusion_manager.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>

#include "renderer_context.h"

namespace vostok {
namespace render {

hw_hiz_occlusion_manager::~hw_hiz_occlusion_manager( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7096b0]
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
	// STATE[STUB]
	// FUNCTION BODY[0x70a290]
}

void hw_hiz_occlusion_manager::render_occluders( renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a030]
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
		m_hiz_occlusion_effect->apply_pass( 1 );
		backend::ref( ).set_ps_constant(
			m_draw_color_parameter,
			float4( scale, scale, scale, 0.f )
		);
		m_sphere_occluder_geometry.render( );
	}
}

void hw_hiz_occlusion_manager::downsample_occlusion_buffer( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x709930]
}

void hw_hiz_occlusion_manager::render_model_bounds(
	renderer_context*,
	float4 const*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a4d0]
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

void hw_hiz_occlusion_manager::check_culling_buffer( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a320]
}

} // namespace render
} // namespace vostok
