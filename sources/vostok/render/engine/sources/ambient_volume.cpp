#include "pch.h"

// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (new-in-target subsystem) - matcher-phase work.
#include "ambient_volume.h"

namespace vostok {
namespace render {

ambient_volume::ambient_volume(
	ambient_volume_properties const&	properties,
	u32									id
) :
	m_properties				( properties ),
	m_aabb						( math::create_zero_aabb( ) ),
	m_id						( id ),
	m_occlusion_info_index		( 0 ),
	m_occluded					( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x748920]
}

bool ambient_volume::is_occluded( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x748870]
	return false;
}

void ambient_volume::set_properties( ambient_volume_properties const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x748890]
}

} // namespace render
} // namespace vostok
