////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GEOMETRY_BATCH_H_INCLUDED
#define RENDER_GEOMETRY_BATCH_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
class vostok::render::material_effects_instance;
class vostok::render::res_geometry;

namespace vostok {
namespace render {

struct geometry_batch {
	inline				geometry_batch	( ) { /* no source */ }
	inline				geometry_batch	(
							math::aabb const&					arg_0,
							res_geometry_ptr const&				arg_1,
							const u32							arg_2,
							material_effects_instance_ptr		arg_3
						) { /* no source */ }
			explicit	geometry_batch	( geometry_batch const& __that );
						~geometry_batch	( );

	/* 0x0000 */	math::aabb							bbox;
	/* 0x0018 */	material_effects_instance_ptr		mtl;
	/* 0x001c */	res_geometry_ptr					geometry;
	/* 0x0020 */	u32									num_indices;
}; // struct geometry_batch

STATIC_SIZE_ASSERT(geometry_batch, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GEOMETRY_BATCH_H_INCLUDED
