////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DECAL_INSTANCE_H_INCLUDED
#define RENDER_DECAL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::collision::geometry_instance;
class vostok::collision::object;
class vostok::math::aabb;
class vostok::render::resource_intrusive_base;
struct vostok::collision::space_partitioning_tree;
struct vostok::render::decal_properties;
class vostok::render::enum_render_stage_type;
class vostok::render::res_effect;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class vostok::render::material_effects;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

struct decal_instance : public resource_intrusive_base , public boost::noncopyable {
										decal_instance		(
											collision::space_partitioning_tree*		tree,
											decal_properties const&					properties,
											const u32								id
										);
										~decal_instance		( );

			void						set_properties		( decal_properties const& in_properties );
			decal_properties const&		get_properties		( ) const;
			material_effects const&		get_effects			( ) const;

			u32							draw				(
											renderer_context*			context,
											res_effect_ptr				opaque_geometry_mask_effect,
											enum_render_stage_type		stage_type
										);

	inline	math::aabb					get_aabb			( ) const { /* no source */ }

			bool						is_occluded			( ) const;

			void						destroy_impl		( ) const;

private:
			void						render				( renderer_context* context, enum_render_stage_type stage_type );

			void						render_geometry		( );

			void						set_materail_effects( resources::unmanaged_resource_ptr const& in_ptr );

			void						remove_collision	( );

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	decal_properties					m_properties;
	/* 0x0068 */	math::aabb							m_aabb;
	/* 0x0080 */	u32									m_id;
	/* 0x0084 */	collision::space_partitioning_tree* const	m_collision_tree;
	/* 0x0088 */	collision::geometry_instance*		m_collision_geometry;
	/* 0x008c */	collision::object*					m_collision_object;
	/* 0x0090 */	u32									m_occlusion_info_index;
	/* 0x0094 */	float								m_draw_priority;
	/* 0x0098 */	bool								m_occluded;
}; // struct decal_instance

STATIC_SIZE_ASSERT(decal_instance, 0x9C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DECAL_INSTANCE_H_INCLUDED
