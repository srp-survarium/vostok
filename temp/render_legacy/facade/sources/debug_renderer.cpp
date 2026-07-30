////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/render/engine/world.h>
#include "debug_draw_lines_command.h"
#include "debug_draw_triangles_command.h"
#include <vostok/render/world.h>
#include <vostok/geometry_primitives.h>

using vostok::render::vertex_colored;
using vostok::render::debug::renderer;
using vostok::render::engine::world;

using vostok::float3;
using vostok::float4x4;
using vostok::math::color;
using vostok::render::scene_ptr;

static u32 const max_debug_vertices_count	= 64*1024;

void renderer::draw_triangle	( scene_ptr const& scene, float3 const& point_0, float3 const& point_1, float3 const& point_2, vostok::math::color const& color, bool const use_depth )
{
	vertex_colored const vertices [ 3 ]	=
	{
		vertex_colored( point_0, color ),
		vertex_colored( point_1, color ),
		vertex_colored( point_2, color )
	};
	draw_triangle				( scene, vertices, use_depth );
}

void renderer::draw_triangle	( scene_ptr const& scene, vertex_colored const& vertex_0, vertex_colored const& vertex_1, vertex_colored const& vertex_2, bool const use_depth )
{
	vertex_colored const vertices [ 3 ]	=
	{
		vertex_0,
		vertex_1,
		vertex_2
	};
	draw_triangle				( scene, vertices, use_depth );
}

void renderer::draw_frustum_solid	(
		scene_ptr const& scene,
		float const fov_in_radians,
		float const near_plane_distance,
		float const far_plane_distance,
		float const aspect_ratio,
		float3 const& position,
		float3 const& direction,
		float3 up_vector,
		color const& color,
		bool use_depth
	)
{
	float y_fov					= fov_in_radians;
	float x_fov					= fov_in_radians * aspect_ratio;

	float window_right_coord	= math::tan( x_fov / 2.f );
	float window_left_coord		= -window_right_coord;
	float window_top_coord		= math::tan( y_fov / 2.f );
	float window_bottom_coord	= -window_top_coord;

	CURE_ASSERT					( math::is_similar( direction.length(), 1.f, math::epsilon_5 ), return, "given direction vector isn't normalized" );

	float3 const& right_vector	= normalize( up_vector ^ direction );
	up_vector					= normalize( direction ^ right_vector );
	float3 const& offset		= position + direction;

	float3 const corner_vertices[] =
	{
		offset + right_vector * window_right_coord + up_vector * window_top_coord,
		offset + right_vector * window_right_coord + up_vector * window_bottom_coord,
		offset + right_vector * window_left_coord  + up_vector * window_top_coord,
		offset + right_vector * window_left_coord  + up_vector * window_bottom_coord
	};

	float3 const projectors[]		=
	{
		normalize( corner_vertices[0] - position ),
		normalize( corner_vertices[1] - position ),
		normalize( corner_vertices[2] - position ),
		normalize( corner_vertices[3] - position )
	};

	float const distance_to_vertex	= far_plane_distance / ( direction | normalize( projectors[0] ) );
	float3 const far_plane_points[] =
	{
		position + projectors[0] * distance_to_vertex,
		position + projectors[1] * distance_to_vertex,
		position + projectors[2] * distance_to_vertex,
		position + projectors[3] * distance_to_vertex
	};

	if ( near_plane_distance == 0.f )
	{
		// right triangles
		draw_triangle			( scene, position, far_plane_points[0], far_plane_points[1], color, use_depth );
		draw_triangle			( scene, position, far_plane_points[1], far_plane_points[0], color, use_depth );
		// left triangles
		draw_triangle			( scene, position, far_plane_points[2], far_plane_points[3], color, use_depth );
		draw_triangle			( scene, position, far_plane_points[3], far_plane_points[2], color, use_depth );
		// top triangles
		draw_triangle			( scene, position, far_plane_points[0], far_plane_points[2], color, use_depth );
		draw_triangle			( scene, position, far_plane_points[2], far_plane_points[0], color, use_depth );
		// bottom triangles
		draw_triangle			( scene, position, far_plane_points[1], far_plane_points[3], color, use_depth );
		draw_triangle			( scene, position, far_plane_points[3], far_plane_points[1], color, use_depth );
	}
	else
	{
		float const distance_to_vertex	= near_plane_distance / ( direction | normalize( projectors[0] ) );
		float3 const near_plane_points[] =
		{
			position + projectors[0] * distance_to_vertex,
			position + projectors[1] * distance_to_vertex,
			position + projectors[2] * distance_to_vertex,
			position + projectors[3] * distance_to_vertex
		};

		// right triangles
		draw_triangle			( scene, near_plane_points[0], far_plane_points[0], far_plane_points[1], color, use_depth );
		draw_triangle			( scene, near_plane_points[0], far_plane_points[1], far_plane_points[0], color, use_depth );
		draw_triangle			( scene, far_plane_points[1], near_plane_points[0], near_plane_points[1], color, use_depth );
		draw_triangle			( scene, far_plane_points[1], near_plane_points[1], near_plane_points[0], color, use_depth );
		// left triangles
		draw_triangle			( scene, near_plane_points[2], far_plane_points[2], far_plane_points[3], color, use_depth );
		draw_triangle			( scene, near_plane_points[2], far_plane_points[3], far_plane_points[2], color, use_depth );
		draw_triangle			( scene, far_plane_points[3], near_plane_points[2], near_plane_points[3], color, use_depth );
		draw_triangle			( scene, far_plane_points[3], near_plane_points[3], near_plane_points[2], color, use_depth );
		// top triangles
		draw_triangle			( scene, near_plane_points[0], far_plane_points[0], far_plane_points[2], color, use_depth );
		draw_triangle			( scene, near_plane_points[0], far_plane_points[2], far_plane_points[0], color, use_depth );
		draw_triangle			( scene, far_plane_points[2], near_plane_points[0], near_plane_points[2], color, use_depth );
		draw_triangle			( scene, far_plane_points[2], near_plane_points[2], near_plane_points[0], color, use_depth );
		// bottom triangles
		draw_triangle			( scene, near_plane_points[1], far_plane_points[1], far_plane_points[3], color, use_depth );
		draw_triangle			( scene, near_plane_points[1], far_plane_points[3], far_plane_points[1], color, use_depth );
		draw_triangle			( scene, far_plane_points[3], near_plane_points[1], near_plane_points[3], color, use_depth );
		draw_triangle			( scene, far_plane_points[3], near_plane_points[3], near_plane_points[1], color, use_depth );
		// near plane triangles
		draw_triangle			( scene, near_plane_points[0], near_plane_points[1], near_plane_points[2], color, use_depth );
		draw_triangle			( scene, near_plane_points[0], near_plane_points[2], near_plane_points[1], color, use_depth );
		draw_triangle			( scene, near_plane_points[3], near_plane_points[1], near_plane_points[2], color, use_depth );
		draw_triangle			( scene, near_plane_points[3], near_plane_points[2], near_plane_points[1], color, use_depth );
	}

	// far plane triangles
	draw_triangle				( scene, far_plane_points[0], far_plane_points[1], far_plane_points[2], color, use_depth );
	draw_triangle				( scene, far_plane_points[0], far_plane_points[2], far_plane_points[1], color, use_depth );
	draw_triangle				( scene, far_plane_points[3], far_plane_points[1], far_plane_points[2], color, use_depth );
	draw_triangle				( scene, far_plane_points[3], far_plane_points[2], far_plane_points[1], color, use_depth );
}

void renderer::draw_rectangle ( scene_ptr const& scene, float4x4 const& matrix, float3 const& size, color const& color, bool const use_depth )
{
	draw_lines					(
		scene,
		matrix,
		size,
		geometry_utils::rectangle::vertices,
		geometry_utils::rectangle::vertex_count,
		geometry_utils::rectangle::pairs,
		geometry_utils::rectangle::pair_count,
		color,
		use_depth
	);
}

void renderer::draw_ellipse	( scene_ptr const& scene, float4x4 const& matrix, vostok::math::color const& color, bool const use_depth )
{
	draw_lines					(
		scene,
		matrix,
		geometry_utils::ellipse::vertices,
		geometry_utils::ellipse::vertex_count,
		geometry_utils::ellipse::pairs,
		geometry_utils::ellipse::pair_count,
		color,
		use_depth
	);
}

void renderer::draw_ellipsoid	( scene_ptr const& scene, float4x4 const& matrix, vostok::math::color const& color, bool const use_depth )
{
	draw_lines					(
		scene,
		matrix,
		geometry_utils::ellipsoid::vertices,
		geometry_utils::ellipsoid::vertex_count,
		geometry_utils::ellipsoid::pairs,
		geometry_utils::ellipsoid::pair_count,
		color,
		use_depth
	);
}

void renderer::draw_circle	( scene_ptr const& scene, float4x4 const& matrix, float3 const& size, vostok::math::color const& color, bool const use_depth )
{
	draw_lines					(
		scene,
		matrix,
		size,
		geometry_utils::circle::vertices,
		geometry_utils::circle::vertex_count,
		geometry_utils::circle::pairs,
		geometry_utils::circle::pair_count,
		color,
		use_depth
	);
}

void renderer::draw_cone_solid		( scene_ptr const& scene, float4x4 const& matrix, float3 const& size, vostok::math::color const& color, bool const use_depth )
{
	math::sphere sp(matrix.c.xyz(), math::max(size.x, size.y) );
	if(frustum.test(sp)==math::intersection_outside)
		return;

	draw_primitive_solid		(
		scene,
		// this is temporary, coordinates of the primitive need to be updated.
		math::create_scale( float3(0.5, 1, 0.5))*matrix,
		size,
		geometry_utils::cone_solid::vertices,
		geometry_utils::cone_solid::vertex_count,
		geometry_utils::cone_solid::faces,
		geometry_utils::cone_solid::index_count,
		color,
		use_depth
	);
}

void renderer::draw_rectangle_solid	( scene_ptr const& scene, float4x4 const& matrix, float3 const& size, vostok::math::color const& color, bool const use_depth )
{
	draw_primitive_solid		(
		scene,
		matrix,
		size,
		geometry_utils::rectangle_solid::vertices,
		geometry_utils::rectangle_solid::vertex_count,
		geometry_utils::rectangle_solid::faces,
		geometry_utils::rectangle_solid::index_count,
		color,
		use_depth
	);
}

void renderer::draw_lines(
		scene_ptr const& scene,
		vostok::vectora< vertex_colored >::const_iterator start_point,
		vostok::vectora< vertex_colored >::const_iterator end_point,
		vostok::vectora< u16 > const& indices,
		bool use_depth
	)
{
	R_ASSERT_CMP				( end_point - start_point, <=, max_debug_vertices_count );
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, debug::draw_lines_command ) ( scene, m_render_engine_world, m_allocator, start_point, end_point, indices, use_depth ) );
}

void renderer::draw_triangles	( scene_ptr const& scene, vostok::render::debug_vertices_type const &vertices, bool const use_depth )
{
	R_ASSERT_CMP				( vertices.size(), <=, max_debug_vertices_count );

	typedef vostok::buffer_vector< vertex_colored >	TempVertices;
	TempVertices temp_vertices	( ALLOCA( vertices.size( ) * sizeof( vertex_colored ) ), vertices.size( ), vertices.begin( ), vertices.end( ) );

	typedef vostok::buffer_vector< u16 >	TempIndices;
	TempIndices temp_indices	( ALLOCA( vertices.size( ) * sizeof( u16 ) ), vertices.size( ) );
	temp_indices.resize			( vertices.size( ) );

	TempIndices::iterator		i = temp_indices.begin( );
	TempIndices::iterator		e = temp_indices.end( );
	for ( u16 j = 0; i != e; ++i, ++j )
		*i						= j;

	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, debug::draw_triangles_command ) ( scene, m_render_engine_world, m_allocator, temp_vertices, temp_indices, use_depth ) );
}
