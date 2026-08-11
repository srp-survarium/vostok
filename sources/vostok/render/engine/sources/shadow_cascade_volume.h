#ifndef VOSTOK_RENDER_ENGINE_SHADOW_CASCADE_VOLUME_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SHADOW_CASCADE_VOLUME_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/math_plane.h>

#include "ray.h"

namespace vostok {
namespace render {

class shadow_cascade_volume {
public:
	typedef fixed_vector<math::plane, 16> cascade_volume_planes;

	struct polygon {
		polygon( )
		{
		}

		s32			points[4];
		math::plane	plane;
	};

	void compute_planes( )
	{
		// FUNCTION BODY[0x76a20]
		// claude@NOTE: legacy LIGHT_CUBOIDSIDEPOLYS_COUNT constant adapted to the literal 4
		// (canonical dropped the constants)
		for (u32 it=0; it<4; it++)
		{
			polygon&	poly	=	light_cuboid_polys[it];

			poly.plane = math::create_plane( light_cuboid_points[poly.points[0]], light_cuboid_points[poly.points[2]], light_cuboid_points[poly.points[1]] );

// #if		DEBUG
// 			float3&		p0	= light_cuboid_points[poly.points[0]];
// 			float3&		p1	= light_cuboid_points[poly.points[1]];
// 			float3&		p2	= light_cuboid_points[poly.points[2]];
// 			float3&		p3	= light_cuboid_points[poly.points[3]];
// 			math::plane	p012;	p012.build(p0,p1,p2);
// 			math::plane	p123;	p123.build(p1,p2,p3);
// 			math::plane	p230;	p230.build(p2,p3,p0);
// 			math::plane	p301;	p301.build(p3,p0,p1);
// 			ASSERT	(p012.normal.similar(p123.normal) && p012.normal.similar(p230.normal) && p012.normal.similar(p301.normal));
// #endif
		}
	}

	void compute_caster_model_fixed( cascade_volume_planes& dest, float3& translation, float map_size, bool clip_by_view_near )
	{
		translation.set( 0.f, 0.f, 0.f );

		if( math::is_zero( 1-abs(view_ray.direction.dot_product( light_ray.direction )), math::epsilon_7) )
			return;

		// Compute planes for each polygon.
		compute_planes();

		for( u32 i = 0; i < 4; i++ )
		{
			ASSERT( light_cuboid_polys[i].plane.classify(light_ray.origin) > 0);
		}

		s32	align_planes[2];
		s32	align_planes_count = 0;

		// Find one or two planes that align to the view frustum from behind.
		for( u32 i = 0; i < 4; i++ )
		{
			float tmp_dot = view_ray.direction.dot_product(light_cuboid_polys[i].plane.normal);
			if( tmp_dot <= math::epsilon_3)
				continue;

			align_planes[align_planes_count] = i;
			++align_planes_count;

			if( align_planes_count == 2 )
				break;
		}


		float3 align_vector;
		align_vector.set( 0.f, 0.f, 0.f );

		// Align ray points to the align planes.
		for( s32 p = 0; p < align_planes_count; ++p )
		{

			float min_dist = 10000;
			for( u32 i = 0; i < view_frustum_rays.size(); ++i )
			{
				float tmp_dist = 0;
				float3 tmp_point = view_frustum_rays[i].origin;

				tmp_dist = light_cuboid_polys[align_planes[p]].plane.classify( tmp_point );
				min_dist = math::min( tmp_dist, min_dist );
			}

			float3 shift = light_cuboid_polys[align_planes[p]].plane.normal;
			shift		*=	( min_dist );
			align_vector +=		( shift );
		}

		translation += align_vector;

		// Move the light ray by the alignment shift.
		light_ray.origin +=	align_vector;

		// The next translation contains this shift as well.
		// 3 target lines are likely retail-compiled-out source.
		// Reset to reuse.
		align_vector.set( 0.f, 0.f, 0.f );

		// Check if view edges intersect, and push planes.
		for( s32 p = 0; p < align_planes_count; ++p )
		{
			float max_mag = 0;
			for( u32 i = 0; i < view_frustum_rays.size(); ++i )
			{
				float plane_dot_ray = view_frustum_rays[i].direction.dot_product( light_cuboid_polys[align_planes[p]].plane.normal );
				if( plane_dot_ray < 0 )
				{
					float3 per_plane_view;
					per_plane_view		= math::cross_product( light_cuboid_polys[align_planes[p]].plane.normal, view_ray.direction );
					float3 per_view_to_plane;
					per_view_to_plane	= math::cross_product( per_plane_view, view_ray.direction );
					// 4 target lines are likely retail-compiled-out source.
					float tmp_mag = -plane_dot_ray / view_frustum_rays[i].direction.dot_product(per_view_to_plane);

					max_mag = (max_mag < tmp_mag) ?  tmp_mag : max_mag;
				}
			}

			if( math::is_zero( max_mag ) )
				continue;
			// 3 target lines are likely retail-compiled-out source.
			float dist = -light_cuboid_polys[align_planes[p]].plane.normal.dot_product( translation );
			align_vector += (light_cuboid_polys[align_planes[p]].plane.normal*(dist*max_mag));
		}


		translation	+=			 align_vector;
		light_ray.origin +=		 align_vector;
		translate_light_model	( translation );

		// Compute culling planes using rays as edges.
		for( u32 i = 0; i< view_frustum_rays.size(); ++i )
		{
			float3 tmp_vector;
			tmp_vector	= math::cross_product( view_frustum_rays[i].direction, light_ray.direction );

			// Check if the vectors are parallel.
			if( math::is_zero( tmp_vector.squared_length(), math::epsilon_5) )
				continue;

			tmp_vector.normalize();
			math::plane tmp_plane  = math::create_plane_normalized( tmp_vector, view_frustum_rays[i].origin);

			float sign = 0;
			if( check_cull_plane_valid( tmp_plane, sign, 5 ) )
			{
				tmp_plane.normal	*= -sign;
				tmp_plane.d			*= -sign;
				dest.push_back( tmp_plane );
			}
		}

		// Compute culling planes using ray-point pairs as edges.
		if( clip_by_view_near && abs(view_ray.direction.dot_product( light_ray.direction )) < 0.8 )
		{
			float3 perp_light_view, perp_light_to_view;
			perp_light_view		= math::cross_product( view_ray.direction, light_ray.direction );
			perp_light_to_view	= math::cross_product( perp_light_view, light_ray.direction );

			perp_light_to_view.normalize();
			math::plane plane = math::create_plane_normalized( perp_light_to_view, view_ray.origin);

			float max_dist = -1000;
			for( u32 i = 0; i< view_frustum_rays.size(); ++i )
				max_dist = math::max( plane.classify( view_frustum_rays[i].origin ), max_dist);

			for( u32 i = 0; i< view_frustum_rays.size(); ++i )
			{
				float3 origin = view_frustum_rays[i].origin;
				origin += ( view_frustum_rays[i].direction * 5);

				if( plane.classify( origin ) > max_dist)
				{
					max_dist = 0.f;
					break;
				}
			}

			if( max_dist > -1000 )
			{
				plane.d += max_dist;
				dest.push_back(plane);
			}
		}

		for( u32 i = 0; i < 4; i++ )
		{
			dest.push_back( light_cuboid_polys[i].plane );
			dest.back().normal	*= -1;
			dest.back().d		*= -1;
			ASSERT( light_cuboid_polys[i].plane.classify(light_ray.origin) > 0);
		}

		// Move each ray origin to its intersection with the light model.
		for( u32 i = 0; i < view_frustum_rays.size(); ++i )
		{
			float min_dist = 2*map_size;
			for( s32 p = 0; p < 4; ++p )
			{
				float dist;
				if( ( light_cuboid_polys[p].plane.normal.dot_product(view_frustum_rays[i].direction) ) > -0.1 )
					dist = map_size;
				else
					light_cuboid_polys[p].plane.intersect_ray( view_frustum_rays[i].origin, view_frustum_rays[i].direction, dist);

				if( dist > math::epsilon_3 && dist < min_dist )
					min_dist = dist;
			}

			view_frustum_rays[i].origin	+=	( view_frustum_rays[i].direction*min_dist );
		}
	}
	// 4 target lines are likely retail-compiled-out source.
	bool check_cull_plane_valid( math::plane const &plane, float &sign, float mad_factor = 0.f )
	{
		bool	valid = false;
		bool	oriented = false;
		float	orient = 0;
		for( u32 j = 0; j< view_frustum_rays.size(); ++j )
		{
			float	tmp_dist = 0.f;
			float3	tmp_pt = view_frustum_rays[j].origin;
			tmp_pt	+= (  view_frustum_rays[j].direction * mad_factor );
			tmp_dist = plane.classify( tmp_pt );

			if( math::is_zero(tmp_dist, math::epsilon_3))
				continue;

			if( !oriented )
			{
				orient = tmp_dist > 0.f ? 1.f : -1.f;
				valid = true;
				oriented = true;
				continue;
			}

			if( tmp_dist < 0 && orient < 0 || tmp_dist > 0 && orient > 0)
				continue;

			valid = false;
			break;
		}
		sign = orient;
		return valid;
	}

	void translate_light_model( float3 translate )
	{
		for( s32 i = 0; i < 4; ++i )
			light_cuboid_polys[i].plane.d -= translate.dot_product	(light_cuboid_polys[i].plane.normal);
	}

private:
	friend class stage_shadow_direct;

	fixed_vector<ray, 8>	view_frustum_rays;
	ray						view_ray;
	ray						light_ray;
	float3					light_cuboid_points[8];
	polygon					light_cuboid_polys[4];
};

STATIC_SIZE_ASSERT( shadow_cascade_volume::polygon, 0x20 );
STATIC_SIZE_ASSERT( shadow_cascade_volume, 0x1D8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SHADOW_CASCADE_VOLUME_H_INCLUDED
