////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENVIRONMENT_PROBE_H_INCLUDED
#define RENDER_ENVIRONMENT_PROBE_H_INCLUDED

/* INCLUDES */
class vostok::collision::geometry_instance;
class vostok::collision::object;
class vostok::math::aabb;
class vostok::render::resource_intrusive_base;
struct vostok::collision::space_partitioning_tree;
struct vostok::render::environment_probe_properties;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct environment_probe : public resource_intrusive_base , public boost::noncopyable {
							environment_probe	(
								collision::space_partitioning_tree*		tree,
								environment_probe_properties const&		properties,
								const u32								id
							);
							~environment_probe	( );

			void			set_properties		( environment_probe_properties const& in_properties );

			bool			is_occluded			( ) const;

	inline	void			destroy_impl		( ) const { /* no source */ }

	inline	math::aabb		get_aabb			( ) const { /* no source */ }

			void			remove_collision	( );

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	environment_probe_properties		m_properties;
	/* 0x017c */	math::aabb							m_aabb;
	/* 0x0194 */	res_texture_ptr						m_texture;
	/* 0x0198 */	res_texture_ptr						m_texture_depth;
	/* 0x019c */	u32									m_id;
	/* 0x01a0 */	u32									m_num_mips;
	/* 0x01a4 */	collision::space_partitioning_tree* const	m_collision_tree;
	/* 0x01a8 */	collision::geometry_instance*		m_collision_geometry;
	/* 0x01ac */	collision::object*					m_collision_object;
	/* 0x01b0 */	u32									m_occlusion_info_index;
	/* 0x01b4 */	bool								m_occluded;
}; // struct environment_probe

STATIC_SIZE_ASSERT(environment_probe, 0x1B8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENVIRONMENT_PROBE_H_INCLUDED
