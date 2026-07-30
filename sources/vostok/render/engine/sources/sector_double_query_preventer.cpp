#include "pch.h"
#include "sector_double_query_preventer.h"

namespace vostok {
namespace render {
namespace culling {

sector_double_query_preventer::sector_double_query_preventer( spatial_sector const*, u32 ) :
	m_buffer_for_frustum_vectors( 0 ),
	m_sectors_max_frustums( 0 ),
	m_buffer_for_rect_vectors( 0 ),
	m_sectors_max_rects( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8c30]
}

sector_double_query_preventer::~sector_double_query_preventer( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8b60]
}

void sector_double_query_preventer::clear( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e84c0]
}

bool sector_double_query_preventer::is_possible_points_for_frustum( float3 const (&)[4], u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e86a0]
	return false;
}

void sector_double_query_preventer::add_ss_aab_rect( aab_rect const&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8b10]
}

bool sector_double_query_preventer::is_possible_ss_aab_rect( aab_rect const&, u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8540]
	return false;
}

void sector_double_query_preventer::add_frustum( math::frustum const&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8ad0]
}

bool sector_double_query_preventer::is_aabb_in_sector( math::aabb const&, u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8650]
	return false;
}

bool sector_double_query_preventer::is_visible_aabb( math::aabb const&, u16 const*, u16 const* ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8700]
	return false;
}

void sector_double_query_preventer::render( system_renderer& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8d10]
}

void sector_double_query_preventer::make_frustum_images( float3 const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8770]
}

u32 sector_double_query_preventer::frustums_count( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5e8480]
	return 0;
}

} // namespace culling
} // namespace render
} // namespace vostok
