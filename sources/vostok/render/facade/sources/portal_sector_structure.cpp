#include "pch.h"
#include <vostok/render/culling/portal_sector_structure.h>

namespace vostok {
namespace render {
namespace culling {

// STATE[STUB]
portal_sector_structure::portal_sector_structure(
	memory::base_allocator* allocator,
	u32,
	u32
)
	: m_allocator( allocator ),
	  m_portals_buffer( 0 ),
	  m_portals( 0, 0 ),
	  m_portal_ids_buffer( 0 ),
	  m_sectors_buffer( 0 ),
	  m_sectors( 0, 0 ),
	  m_sectors_spatial_tree( 0 ),
	  m_portals_spatial_tree( 0 ),
	  m_portals_geometry( 0 )
{
	// FUNCTION BODY[0x75e9c0]
}

// STATE[STUB]
portal_sector_structure::~portal_sector_structure( )
{
	// FUNCTION BODY[0x75e8c0]
}

// STATE[STUB]
u32 portal_sector_structure::get_sector_id( memory::base_allocator&, float3 const& ) const
{
	// FUNCTION BODY[0x75e620]
	return 0;
}

// STATE[STUB]
void portal_sector_structure::load( configs::binary_config_value* )
{
	// FUNCTION BODY[0x75ebf0]
}

// STATE[STUB]
void portal_sector_structure::sort_portal_ids( float const* )
{
	// FUNCTION BODY[0x75e5d0]
}

// STATE[STUB]
void portal_sector_structure::initialize_portals_geometry( )
{
	// FUNCTION BODY[0x75ea80]
}

// STATE[STUB]
void portal_sector_structure::update_portals_visability( math::frustum const&, pcbyte )
{
	// FUNCTION BODY[0x75e520]
}

} // namespace culling
} // namespace render
} // namespace vostok
