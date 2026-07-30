#ifndef VOSTOK_RENDER_FACADE_DECAL_PROPERTIES_INLINE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_DECAL_PROPERTIES_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline decal_properties::decal_properties( )
{
	// FUNCTION BODY[0x68b70]
	transform							= math::float4x4().identity();
	material							= NULL;
	width_height_far_distance			= 0.5f * float3(1.0f, 1.0f, 1.0f);
	alpha_angle							= -1.0f;
	clip_angle							= -1.0f;
	draw_priority						= 0.0f;
	projection_on_terrain_geometry		= true;
	projection_on_static_geometry		= true;
	projection_on_speedtree_geometry	= true;
	projection_on_skeleton_geometry		= true;
	projection_on_particle_geometry		= true;
}

inline decal_properties::decal_properties(
	float4x4							in_transform,
	resources::unmanaged_resource_ptr	in_material,
	float3								in_width_height_far_distance,
	float								in_alpha_angle,
	float								in_clip_angle,
	bool								in_projection_on_terrain_geometry,
	bool								in_projection_on_static_geometry,
	bool								in_projection_on_speedtree_geometry,
	bool								in_projection_on_skeleton_geometry,
	bool								in_projection_on_particle_geometry,
	float								in_draw_priority
) :
	transform							( in_transform ),
	material							( in_material ),
	width_height_far_distance			( in_width_height_far_distance ),
	alpha_angle							( in_alpha_angle ),
	clip_angle							( in_clip_angle ),
	draw_priority						( in_draw_priority ),
	projection_on_terrain_geometry		( in_projection_on_terrain_geometry ),
	projection_on_static_geometry		( in_projection_on_static_geometry ),
	projection_on_speedtree_geometry	( in_projection_on_speedtree_geometry ),
	projection_on_skeleton_geometry		( in_projection_on_skeleton_geometry ),
	projection_on_particle_geometry		( in_projection_on_particle_geometry )
{
	transform.set_scale( float3( 1.0f, 1.0f, 1.0f ) );
	width_height_far_distance *= 0.5f;
}

inline bool decal_properties::is_project_on_all( ) const
{
	return
		projection_on_terrain_geometry &&
		projection_on_static_geometry &&
		projection_on_speedtree_geometry &&
		projection_on_skeleton_geometry &&
		projection_on_particle_geometry;
}

inline bool decal_properties::is_project_on_something( ) const
{
	return
		projection_on_terrain_geometry ||
		projection_on_static_geometry ||
		projection_on_speedtree_geometry ||
		projection_on_skeleton_geometry ||
		projection_on_particle_geometry;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_DECAL_PROPERTIES_INLINE_H_INCLUDED
