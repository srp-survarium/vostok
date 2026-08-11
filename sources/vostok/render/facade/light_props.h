#ifndef VOSTOK_RENDER_FACADE_LIGHT_PROPS_H_INCLUDED
#define VOSTOK_RENDER_FACADE_LIGHT_PROPS_H_INCLUDED

#include <vostok/math_curve.h>
#include <vostok/math_float4x4.h>

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

enum light_type {
	light_type_point		= 0x0,
	light_type_spot			= 0x1,
	light_type_obb			= 0x2,
	light_type_capsule		= 0x3,
	light_type_parallel		= 0x4,
	light_type_sphere		= 0x5,
	light_type_plane_spot	= 0x6,
	num_light_types			= 0x7,
};

struct light_props {
	light_props( );

	void load_light_props( configs::binary_config_value const& cfg );

	float4x4				transform;
	float					attenuation_power;
	float					intensity;
	float					range;
	float					spot_umbra_angle;
	float					spot_penumbra_angle;
	float					spot_falloff;
	float					diffuse_influence_factor;
	float					specular_influence_factor;
	u32						color;
	light_type				type;
	s32						lighting_model;
	u32						sun_shadow_map_size;
	u32						num_sun_cascades;
	bool					static_shadows;
	u32						shadow_map_size;
	u32						shadow_map_size_index;
	float					local_light_z_bias;
	float					shadow_transparency;
	bool					shadow_distribution_sides[6];
	bool					enabled;
	bool					does_cast_shadows;
	bool					shadower;
	u32						m_year;
	u32						m_month;
	u32						m_day;
	u32						m_hours;
	u32						m_minutes;
	bool					m_use_auto_location;
	bool					use_with_lpv;
	bool					is_light_animated;
	float					light_animation_length;
	math::curve_line_color	m_color_curve;
};

STATIC_SIZE_ASSERT( light_props, 0xF0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_LIGHT_PROPS_H_INCLUDED
