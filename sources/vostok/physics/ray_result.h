// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_PHYSICS_CLOSEST_RAY_RESULT_H_INCLUDED
#define VOSTOK_PHYSICS_CLOSEST_RAY_RESULT_H_INCLUDED

namespace vostok {
namespace physics {

class base_physics_object;

struct closest_ray_result  {
					closest_ray_result	( ) :
					object			( NULL ),
					triangle_index	( -1 ),
					is_shape_index	( false ),
					fraction		( 0.0f ) {}

	/* 0x0000 */	base_physics_object*	object;
	/* 0x0004 */	float3					hit_point_world;
	/* 0x0010 */	float3					hit_normal_world;
	/* 0x001c */	s32						triangle_index;
	/* 0x0020 */	bool					is_shape_index;
	/* 0x0024 */	float					fraction;
}; // struct closest_ray_result

STATIC_SIZE_ASSERT(closest_ray_result, 0x28);

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_CLOSEST_RAY_RESULT_H_INCLUDED
