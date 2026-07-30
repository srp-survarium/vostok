#include "pch.h"
#include "sky_ambient_occlusion.h"

namespace vostok {
namespace render {

sky_ambient_occlusion::sky_ambient_occlusion(
	sky_ambient_occlusion_properties const&	properties,
	u32										id
) :
	m_properties			( properties ),
	m_aabb					( math::create_zero_aabb( ) ),
	m_id					( id ),
	m_occlusion_info_index	( 0 ),
	m_occluded				( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x78af40]
}

sky_ambient_occlusion::~sky_ambient_occlusion( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x78adc0]
}

void sky_ambient_occlusion::set_properties(
	sky_ambient_occlusion_properties const& in_properties
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x78ade0]
	m_properties = in_properties;
}

} // namespace render
} // namespace vostok
