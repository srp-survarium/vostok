#ifndef VOSTOK_RENDER_ENGINE_DECAL_SHADER_CONSTANTS_AND_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_DECAL_SHADER_CONSTANTS_AND_GEOMETRY_H_INCLUDED

#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/untyped_buffer.h>

#include "res_geometry.h"

namespace vostok {
namespace render {

class renderer_context;
class shader_constant_host;

class decal_shader_constants_and_geometry :
	public quasi_singleton<decal_shader_constants_and_geometry>
{
public:
	decal_shader_constants_and_geometry( );

	void set(
		renderer_context*	context,
		float4x4 const&		world_to_decal_matrix,
		float4x4 const&		decal_tangent_to_view_space_matrix,
		float				alpha_angle,
		float				clip_angle,
		float3 const&		decal_width_height_far_distance,
		float4x4 const&		decal_transform
	);

	void set_geometry( );

private:
	void create_decal_geometry( );

public:
	~decal_shader_constants_and_geometry( ) { }

private:
	shader_constant_host*	m_world_to_decal_parameter;
	shader_constant_host*	m_eye_ray_corner_parameter;
	shader_constant_host*	m_decal_tangent_to_view_space_matrix_parameter;
	shader_constant_host*	m_decal_angle_parameters;
	untyped_buffer_ptr		m_decal_vertex_buffer;
	untyped_buffer_ptr		m_decal_index_buffer;
	res_geometry_ptr		m_decal_geometry;
};

STATIC_SIZE_ASSERT( decal_shader_constants_and_geometry, 0x1C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_DECAL_SHADER_CONSTANTS_AND_GEOMETRY_H_INCLUDED
