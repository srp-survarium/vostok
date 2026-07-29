////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DECAL_PROPERTIES_H_INCLUDED
#define RENDER_DECAL_PROPERTIES_H_INCLUDED

/* INCLUDES */
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct decal_properties {
	inline			decal_properties		( ) { /* no source */ }
	inline			decal_properties		(
						float4x4								arg_0,
						resources::unmanaged_resource_ptr		arg_1,
						float3									arg_2,
						float									arg_3,
						float									arg_4,
						bool									arg_5,
						bool									arg_6,
						bool									arg_7,
						bool									arg_8,
						bool									arg_9,
						float									arg_10
					) { /* no source */ }

	inline	bool	is_project_on_all		( ) const { /* no source */ }
	inline	bool	is_project_on_something	( ) const { /* no source */ }

	inline			~decal_properties		( ) { /* no source */ }

	/* 0x0000 */	float4x4								transform;
	/* 0x0040 */	resources::unmanaged_resource_ptr		material;
	/* 0x0044 */	float3									width_height_far_distance;
	/* 0x0050 */	float									alpha_angle;
	/* 0x0054 */	float									clip_angle;
	/* 0x0058 */	float									draw_priority;
	/* 0x005c */	bool									projection_on_terrain_geometry;
	/* 0x005d */	bool									projection_on_static_geometry;
	/* 0x005e */	bool									projection_on_speedtree_geometry;
	/* 0x005f */	bool									projection_on_skeleton_geometry;
	/* 0x0060 */	bool									projection_on_particle_geometry;
}; // struct decal_properties

STATIC_SIZE_ASSERT(decal_properties, 0x64);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DECAL_PROPERTIES_H_INCLUDED
