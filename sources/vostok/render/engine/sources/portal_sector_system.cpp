#include "pch.h"
#include "portal_sector_system.h"

#include <vostok/math_frustum.h>
#include <vostok/math_plane.h>

namespace vostok {
namespace render {
namespace culling {

portal_sector_system::portal_sector_system( portal_sector_structure_ptr structure ) :
	m_structure					( structure ),
	m_test_action				( false ),
	m_preventer					( 0 ),
	m_occlusion_bounds_buffer		( 0 ),
	m_occlusion_bounds				( 0, 0 ),
	m_occlusion_results_buffer		( 0 ),
	m_occlusion_results			( 0, 0 ),
	m_portals_geometry				( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc180]
}

portal_sector_system::~portal_sector_system( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc080]
}

math::frustum create_frustum_from_four_points(
	float3 const&,
	float3 const (&)[4],
	math::plane const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fb460]
	return math::frustum( );
}

bool cull_points_by_frustum( math::frustum const&, float3 (&)[4] )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fa5d0]
	return false;
}

void portal_sector_system::render( system_renderer&, float3 const&, float4x4 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc550]
}

void portal_sector_system::test_action( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9000]
}

void portal_sector_system::draw_quads( system_renderer& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc300]
}

void portal_sector_system::draw_portals( system_renderer&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc3f0]
}

void portal_sector_system::select_models(
	collision::space_partitioning_tree*,
	float3 const&,
	float4x4 const&,
	vector<render_surface_instance*>&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fbd80]
}

void portal_sector_system::process_sector( u32, u32, float3 const&, math::frustum const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9110]
}

void portal_sector_system::process_portal_by_frustum_intersection(
	u32,
	math::frustum const&,
	u32,
	float3 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fb8b0]
}

void portal_sector_system::process_sector(
	u32,
	u32,
	buffer_vector<aab_rect> const&,
	float3 const&,
	math::plane const&,
	float4x4 const&,
	float4x4 const&,
	aab_rect const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9090]
}

bool portal_screen_rect_to_four_points(
	aab_rect const&,
	math::plane const&,
	float4x4 const&,
	aab_rect const&,
	float3 (&)[4],
	aab_rect&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9bd0]
	return false;
}

void portal_sector_system::process_portal_in_screen_space(
	u32,
	u32,
	buffer_vector<aab_rect> const&,
	float3 const&,
	math::plane const&,
	float4x4 const&,
	float4x4 const&,
	aab_rect const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fb630]
}

void portal_sector_system::perform_frustum_culling_and_sectors_test(
	collision::space_partitioning_tree*,
	u32,
	math::frustum const&,
	vector<render_surface_instance*>&,
	float3 const&,
	float4x4 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fbae0]
}

void portal_sector_system::get_portals_occlusion_bounds( float4* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9010]
}

void portal_sector_system::initialize_portals_occlusion_bounds_and_results( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9940]
}

void portal_sector_system::update_portals_occlusion_culling( pcbyte )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9040]
}

void portal_sector_system::calculate_portal_rects_in_screen_space(
	float4x4 const&,
	float,
	buffer_vector<aab_rect>&,
	buffer_vector<float>&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9180]
}

void portal_sector_system::sort_portals_and_calculate_rects_in_screen_space(
	float4x4 const&,
	float,
	buffer_vector<aab_rect>&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f9b60]
}

u32 get_aabb_furthest_vertex_id( float3 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f8f90]
	return 0;
}

void portal_sector_system::make_frustum_images( float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fb340]
}

} // namespace culling
} // namespace render
} // namespace vostok
