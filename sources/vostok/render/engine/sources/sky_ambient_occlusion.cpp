// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/resource_manager.h>
#include "sky_ambient_occlusion.h"

namespace vostok {
namespace render {

sky_ambient_occlusion::sky_ambient_occlusion(
	sky_ambient_occlusion_properties const&	properties,
	u32 const								id
) :
	m_properties			( ),
	m_aabb					( math::create_identity_aabb( ) ),
	m_id					( id ),
	m_occlusion_info_index	( u32(-1) ),
	m_occluded				( false )
{
	set_properties(properties);
}

sky_ambient_occlusion::~sky_ambient_occlusion( )
{
}

void sky_ambient_occlusion::set_properties(
	sky_ambient_occlusion_properties const& in_properties
)
{
	m_properties = in_properties;

	if (in_properties.texture_invalidated)
	{
		m_texture = 0;
		m_texture = resource_manager::ref().create_texture(m_properties.texture_name.get_buffer(), 0, 0, false, true, true, u32(-1));
	}

	float4x4 new_transform = math::create_translation(in_properties.location);
	new_transform.set_scale(float3(in_properties.width, in_properties.depth, in_properties.height));

	m_aabb = math::create_identity_aabb();
	m_aabb.modify(new_transform);
}

} // namespace render
} // namespace vostok
