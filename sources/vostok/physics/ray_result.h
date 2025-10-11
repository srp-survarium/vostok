////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_CLOSEST_RAY_RESULT_H_INCLUDED
#define VOSTOK_PHYSICS_CLOSEST_RAY_RESULT_H_INCLUDED

namespace vostok {
namespace physics {

class base_physics_object;

struct closest_ray_result  {
	closest_ray_result() {};

	/* offset 0x0000 */ base_physics_object*                object;
	/* offset 0x0004 */ float3                              hit_point_world;
	/* offset 0x0010 */ float3                              hit_normal_world;
	/* offset 0x001c */ s32                                 triangle_index;
	/* offset 0x0020 */ bool                                is_shape_index;
	/* offset 0x0024 */ float                               fraction;
}; // struct closest_ray_result

namespace {
	typedef char size_assert[
		sizeof(closest_ray_result) == 0x28 ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_CLOSEST_RAY_RESULT_H_INCLUDED