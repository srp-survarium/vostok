////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LIGHT_H_INCLUDED
#define RENDER_LIGHT_H_INCLUDED

/* INCLUDES */
class const vostok::render::light_type;
class vostok::collision::geometry_instance;
class vostok::collision::object;
class vostok::math::aabb;
class vostok::render::resource_intrusive_base;
struct vostok::collision::space_partitioning_tree;
struct vostok::math::curve_line_color;
struct vostok::render::light::light_flags;
class vostok::render::light::shadow_distribution_side;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::render_target;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::math::color;

namespace vostok {
namespace render {

class light : public resource_intrusive_base , public boost::noncopyable {
public:
			explicit	light					( collision::space_partitioning_tree* tree );
						~light					( );

			void		destroy_impl			( ) const;

	inline	void		set_type				( const light_type arg_0 ) { /* no source */ }
	inline	u32			get_type				( ) const { /* no source */ }

	inline	void		set_cast_shadows		( const bool arg_0 ) { /* no source */ }
			bool		is_cast_shadows			( ) const;
	inline	bool		is_cast_shadows_in		( light::shadow_distribution_side arg_0 ) const { /* no source */ }

			void		remove_collision		( );

			void		set_position			( float3 const& P );
			void		set_orientation			( float3 const& direction, float3 const& right );
			void		set_range				( float R );
			void		set_color				( math::color const& c, const float intensity );
	inline	void		set_hud_mode			( const bool arg_0 ) { /* no source */ }
	inline	bool		get_hud_mode			( ) { /* no source */ }

	inline	bool		is_inside_light_volume	( float3 const& arg_0 ) const { /* no source */ }

			void		xform_calc				( );

			void		set_attenuation_power	( float value );
			void		set_scale				( float3 const& scale );

			void		on_properties_changed	( );

			bool		is_occluded				( ) const;

			void		tick_color_animation	( const float time_delta );

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	float4x4							m_xform;
	/* 0x0044 */	float4x4							m_plane_spot_xform;
	/* 0x0084 */	float3								color;
	/* 0x0090 */	float								intensity;
	/* 0x0094 */	float3								position;
	/* 0x00a0 */	float								spot_umbra_angle;
	/* 0x00a4 */	float3								direction;
	/* 0x00b0 */	float3								previous_direction;
	/* 0x00bc */	float								spot_penumbra_angle;
	/* 0x00c0 */	float3								right;
	/* 0x00cc */	float								spot_falloff;
	/* 0x00d0 */	float3								scale;
	/* 0x00dc */	float								attenuation_power;
	/* 0x00e0 */	float								range;
	/* 0x00e4 */	float								shadow_transparency;
	/* 0x00e8 */	u32									m_xform_frame;
	/* 0x00ec */	bool								m_occluded;
	/* 0x00ed */	bool								static_shadows;
	/* 0x00ee */	bool								need_refresh_static_shadows;
	/* 0x00ef */	bool								m_enabled;
	/* 0x00f0 */	u32									m_occlusion_info_index;
	/* 0x00f4 */	collision::space_partitioning_tree* const	m_collision_tree;
	/* 0x00f8 */	collision::geometry_instance*		m_collision_geometry;
	/* 0x00fc */	collision::object*					m_collision_object;
	/* 0x0100 */	render_target_ptr					m_shadow_depth_stencil;
	/* 0x0104 */	res_texture_ptr						m_shadow_depth_stencil_texture;
	/* 0x0108 */	bool								m_is_light_animated;
	/* 0x010c */	float								m_light_animation_length;
	/* 0x0110 */	math::curve_line_color				m_color_curve;
	/* 0x0150 */	float								m_current_animation_time;
	/* 0x0154 */	float								diffuse_influence_factor;
	/* 0x0158 */	float								specular_influence_factor;
	/* 0x015c */	bool								is_shadower;
	/* 0x015d */	bool								use_with_lpv;
	/* 0x0160 */	u32									sun_shadow_map_size;
	/* 0x0164 */	u32									num_sun_cascades;
	/* 0x0168 */	float								shadow_z_bias;
	/* 0x016c */	u32									shadow_map_size;
	/* 0x0170 */	u32									shadow_map_size_index;
	/* 0x0174 */	u32									old_shadow_map_size_index;
	/* 0x0178 */	u32									lighting_model;
	/* 0x017c */	light::light_flags					flags;
	/* 0x0180 */	bool								shadow_distribution_sides[6];
	/* 0x0186 */	bool								occluded;
	/* 0x0188 */	math::aabb							m_aabb;
}; // class light

STATIC_SIZE_ASSERT(light, 0x1A0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LIGHT_H_INCLUDED
