////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADOW_CASCADE_VOLUME_H_INCLUDED
#define RENDER_SHADOW_CASCADE_VOLUME_H_INCLUDED

/* INCLUDES */
class vostok::fixed_vector<vostok::render::ray,8>;
struct vostok::render::ray;
struct vostok::render::shadow_cascade_volume::polygon;

/* FORWARD REFS */
class vostok::fixed_vector<vostok::math::plane,16>;
class vostok::math::plane;

namespace vostok {
namespace render {

class shadow_cascade_volume {
public:
	inline	void	compute_planes				( ) { /* no source */ }
	inline	void	compute_caster_model_fixed	(
						fixed_vector< math::plane, 16 >&	dest,
						float3&								translation,
						float								map_size,
						bool								clip_by_view_near
					) { /* no source */ }

	inline	bool	check_cull_plane_valid		( math::plane const& arg_0, float& arg_1, float arg_2 ) { /* no source */ }

	inline	void	translate_light_model		( float3 arg_0 ) { /* no source */ }

	inline			shadow_cascade_volume		( ) { /* no source */ }
	inline			~shadow_cascade_volume		( ) { /* no source */ }

	/* 0x0000 */	fixed_vector< ray, 8 >				view_frustum_rays;
	/* 0x00c8 */	ray									view_ray;
	/* 0x00e0 */	ray									light_ray;
	/* 0x00f8 */	float3								light_cuboid_points[8];
	/* 0x0158 */	shadow_cascade_volume::polygon		light_cuboid_polys[4];
}; // class shadow_cascade_volume

STATIC_SIZE_ASSERT(shadow_cascade_volume, 0x1D8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADOW_CASCADE_VOLUME_H_INCLUDED
