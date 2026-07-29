////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKY_AMBIENT_OCCLUSION_H_INCLUDED
#define RENDER_SKY_AMBIENT_OCCLUSION_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::render::resource_intrusive_base;
struct vostok::render::sky_ambient_occlusion_properties;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct sky_ambient_occlusion : public resource_intrusive_base , public boost::noncopyable {
							sky_ambient_occlusion	( sky_ambient_occlusion_properties const& properties, const u32 id );
							~sky_ambient_occlusion	( );

			void			set_properties			( sky_ambient_occlusion_properties const& in_properties );

	inline	bool			is_occluded				( ) const { /* no source */ }

	inline	void			destroy_impl			( ) const { /* no source */ }

	inline	math::aabb		get_aabb				( ) const { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	sky_ambient_occlusion_properties	m_properties;
	/* 0x0130 */	math::aabb							m_aabb;
	/* 0x0148 */	res_texture_ptr						m_texture;
	/* 0x014c */	u32									m_id;
	/* 0x0150 */	u32									m_occlusion_info_index;
	/* 0x0154 */	bool								m_occluded;
}; // struct sky_ambient_occlusion

STATIC_SIZE_ASSERT(sky_ambient_occlusion, 0x158);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKY_AMBIENT_OCCLUSION_H_INCLUDED
