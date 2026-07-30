#ifndef VOSTOK_RENDER_ENGINE_STREAMING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STREAMING_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/math_sphere.h>
#include <vostok/resources_managed_resource.h>
#include "res_texture.h"

namespace vostok {
namespace render {

struct render_surface_instance;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

struct streaming_ready_texture {
	streaming_ready_texture( ) :
		num_mips( 0 ),
		distance( 0.f )
	{
	}

	fixed_string<260> name;
	res_texture_ptr texture;
	resources::managed_resource_ptr data;
	u32 num_mips;
	float distance;
};

STATIC_SIZE_ASSERT( streaming_ready_texture, 0x120 );

struct streaming_texture_instance {
	streaming_texture_instance( ) :
		object_sphere( ),
		texel_factor( 0.f ),
		surface_instance( 0 )
	{
	}

	math::sphere object_sphere;
	float texel_factor;
	render_surface_instance* surface_instance;
};

STATIC_SIZE_ASSERT( streaming_texture_instance, 0x18 );

float calculate_streaming_texture_factor(
	float3 const* positions,
	float2 const* uvs,
	u32 num_vertices,
	u32 vertex_stride,
	u16 const* indices,
	u32 num_indices
);

s32 calculate_needed_texture_mip_levels(
	float4x4 const& projection_matrix,
	float3 const& viewer_position,
	math::sphere const& object_sphere,
	u32 screen_size_x,
	u32 screen_size_y,
	float factor,
	float& out_distance
);

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_STREAMING_H_INCLUDED
