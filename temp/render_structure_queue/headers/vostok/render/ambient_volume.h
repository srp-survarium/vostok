////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_AMBIENT_VOLUME_H_INCLUDED
#define RENDER_AMBIENT_VOLUME_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::render::resource_intrusive_base;
struct vostok::render::ambient_volume_properties;

namespace vostok {
namespace render {

struct ambient_volume : public resource_intrusive_base , public boost::noncopyable {
							ambient_volume	( ambient_volume_properties const& properties, const u32 id );
	inline					~ambient_volume	( ) { /* no source */ }

			void			set_properties	( ambient_volume_properties const& in_properties );

			bool			is_occluded		( ) const;

	inline	void			destroy_impl	( ) const { /* no source */ }

	inline	math::aabb		get_aabb		( ) const { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	ambient_volume_properties		m_properties;
	/* 0x004c */	math::aabb						m_aabb;
	/* 0x0064 */	u32								m_id;
	/* 0x0068 */	u32								m_occlusion_info_index;
	/* 0x006c */	bool							m_occluded;
}; // struct ambient_volume

STATIC_SIZE_ASSERT(ambient_volume, 0x70);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_AMBIENT_VOLUME_H_INCLUDED
