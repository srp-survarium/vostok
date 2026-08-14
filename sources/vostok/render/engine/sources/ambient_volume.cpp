#include "pch.h"
#include <vostok/render/core/options.h>

// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (new-in-target subsystem) - matcher-phase work.
#include "ambient_volume.h"

namespace vostok {
namespace render {

ambient_volume::ambient_volume(
	ambient_volume_properties const&	properties,
	u32 const							id
) :
	m_aabb						( math::create_identity_aabb( ) ),
	m_id						( id ),
	m_occlusion_info_index		( u32(-1) ),
	m_occluded					( false )
{
	// FUNCTION BODY[0x748920]
	set_properties(properties);
}

bool ambient_volume::is_occluded( ) const
{
	// FUNCTION BODY[0x748870]
	return options::ref( ).current.m_use_hiz_occlusion_culling && m_occluded;
}

void ambient_volume::set_properties( ambient_volume_properties const& in_properties )
{
	// FUNCTION BODY[0x748890]
	m_properties = in_properties;
	m_aabb = math::create_identity_aabb();
	m_aabb.modify(in_properties.transform);
}

} // namespace render
} // namespace vostok
