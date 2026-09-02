// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "grass_instance.h"
namespace vostok {
namespace render {

grass_instance::grass_instance(
	u32 const in_id,
	grass_template* in_template,
	math::color const& in_color,
	float4x4 const& in_transform,
	u8 in_layer,
	float in_wind_scale
) :
	m_template( in_template ),
	m_color( in_color ),
	m_transform( in_transform ),
	m_wind_scale( in_wind_scale ),
	m_index( in_id ),
	m_layer_id( in_layer )
{
}

} // namespace render
} // namespace vostok
