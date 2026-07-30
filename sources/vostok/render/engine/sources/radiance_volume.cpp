#include "pch.h"
#include "radiance_volume.h"

namespace vostok {
namespace render {

injection_geometry::injection_geometry( u32 rsm_size ) :
	m_num_points	( 0 ),
	m_stride		( sizeof( float2 ) ),
	m_rsm_size_x	( rsm_size ),
	m_rsm_size_y	( rsm_size )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5efee0]
}

injection_geometry::injection_geometry( u32 rsm_size_x, u32 rsm_size_y ) :
	m_num_points	( 0 ),
	m_stride		( sizeof( float2 ) ),
	m_rsm_size_x	( rsm_size_x ),
	m_rsm_size_y	( rsm_size_y )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5efec0]
}

void injection_geometry::prepare( u32 rsm_size_x, u32 rsm_size_y )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5efd80]
	m_rsm_size_x = rsm_size_x;
	m_rsm_size_y = rsm_size_y;
}

void injection_geometry::draw( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ef240]
}

sliced_cube_geometry::sliced_cube_geometry( u32 in_num_cells ) :
	m_slices	( in_num_cells ),
	m_stride	( sizeof( sliced_cube_geometry_vertex ) )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ef970]
}

void sliced_cube_geometry::draw( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ef100]
}

radiance_volume::radiance_volume(
	u32		in_rsm_size,
	u32		in_num_cells,
	u32		in_num_propagate_iterations,
	float	in_scale,
	float	in_flux_amplifier
) :
	m_rsm_downsampled_size			( in_rsm_size ),
	m_rsm_source_size				( in_rsm_size ),
	m_injection_geometry			( in_rsm_size ),
	m_injection_geometry_from_camera( in_rsm_size ),
	m_sliced_cube_geometry			( in_num_cells ),
	m_cell_size						( 0.0f ),
	m_scale							( in_scale ),
	m_flux_amplifier				( in_flux_amplifier ),
	m_num_cells						( in_num_cells ),
	m_bbox							( math::create_zero_aabb( ) ),
	m_previous_origin				( 0.0f, 0.0f, 0.0f ),
	m_prev_previous_origin			( 0.0f, 0.0f, 0.0f ),
	m_next_origin					( 0.0f, 0.0f, 0.0f ),
	m_num_propagate_iterations		( in_num_propagate_iterations ),
	m_is_position_changed			( false ),
	m_prev_position					( 0.0f, 0.0f, 0.0f )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f1780]
}

radiance_volume::~radiance_volume( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ef340]
}

bool radiance_volume::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eebf0]
	return false;
}

void radiance_volume::set_origin( float3 const& in_origin )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eecc0]
	m_prev_previous_origin = m_previous_origin;
	m_previous_origin = m_next_origin;
	m_next_origin = in_origin;
}

float3 const& radiance_volume::get_origin( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eebe0]
	return m_next_origin;
}

float3 const& radiance_volume::get_previous_origin( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eebd0]
	return m_previous_origin;
}

float radiance_volume::get_scale( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eebc0]
	return m_scale;
}

u32 radiance_volume::get_num_cells( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eebb0]
	return m_num_cells;
}

void radiance_volume::prepare_gv( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eec00]
}

void radiance_volume::prepare( float3 const&, float3 const&, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eedc0]
}

void radiance_volume::begin_render_to_cells( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ef050]
}

void radiance_volume::end_render_to_cells( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ef000]
}

void radiance_volume::fill_previous_result( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5eeba0]
}

void radiance_volume::prepare_final( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f1620]
}

void radiance_volume::propagate_lighting_iter( u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f1090]
}

void radiance_volume::propagate_lighting( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f0a30]
}

void radiance_volume::inject_occluder_geometry(
	renderer_context*,
	float3 const&,
	float3 const&,
	vector<float4x4> const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f07c0]
}

void radiance_volume::inject_occluders(
	renderer_context*,
	float3 const&,
	float3 const&,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f0580]
}

void radiance_volume::inject_camera_occluders( renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f0320]
}

void radiance_volume::inject_lighting( float3 const&, float3 const&, float, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f0060]
}

} // namespace render
} // namespace vostok
