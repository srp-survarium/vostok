#ifndef VOSTOK_RENDER_ENGINE_LIGHT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LIGHT_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/math_aabb.h>
#include <vostok/math_color.h>
#include <vostok/math_curve.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/facade/light_props.h>

#include "render_target.h"
#include "res_texture.h"

namespace vostok {

namespace collision {

class geometry_instance;
class object;
struct space_partitioning_tree;

} // namespace collision

namespace render {

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

class light :
	public resource_intrusive_base,
	public boost::noncopyable
{
public:
	enum shadow_distribution_side {
		cast_to_x_side = 0,
		cast_to_neg_x_side,
		cast_to_y_side,
		cast_to_neg_y_side,
		cast_to_z_side,
		cast_to_neg_z_side,
	};

	explicit light( collision::space_partitioning_tree* tree );
	~light( );

	void destroy_impl( ) const;

	void set_type( light_type type )
	{
		flags.type = type;
	}

	u32 get_type( ) const
	{
		return flags.type;
	}

	void set_cast_shadows( bool value )
	{
		flags.does_cast_shadows = value;
	}

	bool is_cast_shadows( ) const;

	bool is_cast_shadows_in( shadow_distribution_side side ) const
	{
		return shadow_distribution_sides[side];
	}

	void remove_collision( );

	void set_position( float3 const& value );
	void set_orientation( float3 const& direction, float3 const& right );
	void set_range( float value );
	void set_color( math::color const& value, float intensity );

	void set_hud_mode( bool value )
	{
		flags.is_hud_mode = value;
	}

	bool get_hud_mode( )
	{
		return flags.is_hud_mode;
	}

	bool is_inside_light_volume( float3 const& ) const
	{
		return false;
	}

	void xform_calc( );
	void set_attenuation_power( float value );
	void set_scale( float3 const& value );
	void on_properties_changed( );
	bool is_occluded( ) const;
	void tick_color_animation( float time_delta );

private:
	struct light_flags {
		u32 type					: 4;
		u32 is_static				: 1;
		u32 does_cast_shadows		: 1;
		u32 is_hud_mode				: 1;
		u32 cast_shadow_in_x		: 1;
		u32 cast_shadow_in_neg_x	: 1;
		u32 cast_shadow_in_y		: 1;
		u32 cast_shadow_in_neg_y	: 1;
		u32 cast_shadow_in_z		: 1;
		u32 cast_shadow_in_neg_z	: 1;
	};

	STATIC_SIZE_ASSERT( light_flags, 0x4 );

public:
	float4x4									m_xform;
	float4x4									m_plane_spot_xform;
	float3										color;
	float										intensity;
	float3										position;
	float										spot_umbra_angle;
	float3										direction;
	float3										previous_direction;
	float										spot_penumbra_angle;
	float3										right;
	float										spot_falloff;
	float3										scale;
	float										attenuation_power;
	float										range;
	float										shadow_transparency;
	u32											m_xform_frame;
	bool										m_occluded;
	bool										static_shadows;
	bool										need_refresh_static_shadows;
	bool										m_enabled;
	u32											m_occlusion_info_index;
	collision::space_partitioning_tree* const	m_collision_tree;
	collision::geometry_instance*				m_collision_geometry;
	collision::object*							m_collision_object;
	render_target_ptr							m_shadow_depth_stencil;
	res_texture_ptr								m_shadow_depth_stencil_texture;
	bool										m_is_light_animated;
	float										m_light_animation_length;
	math::curve_line_color						m_color_curve;
	float										m_current_animation_time;
	float										diffuse_influence_factor;
	float										specular_influence_factor;
	bool										is_shadower;
	bool										use_with_lpv;
	u32											sun_shadow_map_size;
	u32											num_sun_cascades;
	float										shadow_z_bias;
	u32											shadow_map_size;
	u32											shadow_map_size_index;
	u32											old_shadow_map_size_index;
	u32											lighting_model;
	light_flags									flags;
	bool										shadow_distribution_sides[6];
	bool										occluded;
	math::aabb									m_aabb;
};

STATIC_SIZE_ASSERT( light, 0x1A0 );

typedef intrusive_ptr<
	light,
	resource_intrusive_base,
	threading::single_threading_policy
> light_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_LIGHT_H_INCLUDED
