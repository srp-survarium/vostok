////////////////////////////////////////////////////////////////////////////
//	Created		: 23.02.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef LIGHT_PROPS_H_INCLUDED
#define LIGHT_PROPS_H_INCLUDED

#include <vostok/math_curve.h>

namespace vostok {
namespace render {

enum light_type {
	light_type_point,
	light_type_spot,

	light_type_obb,
	light_type_capsule,

	light_type_parallel,

	light_type_sphere,
	light_type_plane_spot,

	num_light_types,
}; // enum light_type

// layout refreshed to the shipped one (binaries/structure/target light_props,
// 0xF0) - game's weapon embeds it by value.
// sushi@TODO: canonical also declares light_props() + load_light_props(cfg);
// not added (no definitions in the tree) - the render rebuild supplies them.
struct light_props {
	/* 0x0000 */	float4x4		transform;
	/* 0x0040 */	float			attenuation_power;
	/* 0x0044 */	float			intensity;
	/* 0x0048 */	float			range;
	/* 0x004c */	float			spot_umbra_angle;
	/* 0x0050 */	float			spot_penumbra_angle;
	/* 0x0054 */	float			spot_falloff;
	/* 0x0058 */	float			diffuse_influence_factor;
	/* 0x005c */	float			specular_influence_factor;
	/* 0x0060 */	u32				color;
	/* 0x0064 */	light_type		type;
	/* 0x0068 */	s32				lighting_model;
	/* 0x006c */	u32				sun_shadow_map_size;
	/* 0x0070 */	u32				num_sun_cascades;
	/* 0x0074 */	bool			static_shadows;
	/* 0x0078 */	u32				shadow_map_size;
	/* 0x007c */	u32				shadow_map_size_index;
	/* 0x0080 */	float			local_light_z_bias;
	/* 0x0084 */	float			shadow_transparency;
	/* 0x0088 */	bool			shadow_distribution_sides[6];
	/* 0x008e */	bool			enabled;
	/* 0x008f */	bool			does_cast_shadows;
	/* 0x0090 */	bool			shadower;
	/* 0x0094 */	u32				m_year;
	/* 0x0098 */	u32				m_month;
	/* 0x009c */	u32				m_day;
	/* 0x00a0 */	u32				m_hours;
	/* 0x00a4 */	u32				m_minutes;
	/* 0x00a8 */	bool			m_use_auto_location;
	/* 0x00a9 */	bool			use_with_lpv;
	/* 0x00aa */	bool			is_light_animated;
	/* 0x00ac */	float			light_animation_length;
	/* 0x00b0 */	math::curve_line_color	m_color_curve;
}; // struct light_props

STATIC_SIZE_ASSERT(light_props, 0xF0);

// reads a light_props out of a config value; out-of-line in
// render/facade/sources/scene_renderer.cpp (the cook entry called by
// object_light::load). Templated over the config-value type.
template < typename config_t >
void load_props_impl( light_props& props, config_t const& cfg );

} // namespace render
} // namespace vostok

#endif // #ifndef LIGHT_PROPS_H_INCLUDED
