#ifndef VOSTOK_RENDER_FACADE_DECAL_PROPERTIES_H_INCLUDED
#define VOSTOK_RENDER_FACADE_DECAL_PROPERTIES_H_INCLUDED

#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

struct decal_properties {
	inline decal_properties( );

	inline decal_properties(
		float4x4							in_transform,
		resources::unmanaged_resource_ptr	in_material,
		float3								in_width_height_far_distance = float3( 1.0f, 1.0f, 1.0f ),
		float								in_alpha_angle = -1.0f,
		float								in_clip_angle = -1.0f,
		bool								in_projection_on_terrain_geometry = true,
		bool								in_projection_on_static_geometry = true,
		bool								in_projection_on_speedtree_geometry = true,
		bool								in_projection_on_skeleton_geometry = true,
		bool								in_projection_on_particle_geometry = true,
		float								in_draw_priority = 0.0f
	);

	inline bool is_project_on_all( ) const;
	inline bool is_project_on_something( ) const;

	float4x4							transform;
	resources::unmanaged_resource_ptr	material;
	float3								width_height_far_distance;
	float								alpha_angle;
	float								clip_angle;
	float								draw_priority;
	bool								projection_on_terrain_geometry;
	bool								projection_on_static_geometry;
	bool								projection_on_speedtree_geometry;
	bool								projection_on_skeleton_geometry;
	bool								projection_on_particle_geometry;
};

STATIC_SIZE_ASSERT( decal_properties, 0x64 );

} // namespace render
} // namespace vostok

#include <vostok/render/facade/decal_properties_inline.h>

#endif // #ifndef VOSTOK_RENDER_FACADE_DECAL_PROPERTIES_H_INCLUDED
