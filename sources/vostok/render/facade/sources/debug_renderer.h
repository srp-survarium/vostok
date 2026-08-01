#ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_DEBUG_RENDERER_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <vostok/math_color.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/math_frustum.h>
#include <vostok/render/api.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/engine/vertex_colored.h>
#include <vostok/vectora.h>

namespace vostok {

namespace memory {
class base_allocator;
}

namespace render {

class one_way_render_channel;

namespace engine {
class world;
}

namespace editor {
class renderer;
}

namespace game {
class renderer;
}

typedef vectora< vertex_colored > debug_vertices_type;
typedef vectora< u16 > debug_indices_type;

namespace debug {

class VOSTOK_RENDER_API renderer : private core::noncopyable {
private:
	friend class editor::renderer;
	friend class game::renderer;

	renderer( one_way_render_channel& channel, memory::base_allocator& allocator, engine::world& engine_world );

public:
	void draw_line( base_scene_ptr const& scene, float3 const& start_point, float3 const& end_point, math::color const& color, bool use_depth = true );
	void draw_origin( base_scene_ptr const& scene, float4x4 const& matrix, float half_size, bool use_depth = true );
	void draw_cross( base_scene_ptr const& scene, float3 const& point, float half_size, math::color const& color, bool use_depth = true );
	void draw_triangle( base_scene_ptr const& scene, vertex_colored const (&vertices)[3], bool use_depth = true );
	void draw_triangle( base_scene_ptr const& scene, float3 const& point_0, float3 const& point_1, float3 const& point_2, math::color const& color, bool use_depth = true );
	void draw_triangle( base_scene_ptr const& scene, vertex_colored const& vertex_0, vertex_colored const& vertex_1, vertex_colored const& vertex_2, bool use_depth = true );

	void draw_cube( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_frustum(
		base_scene_ptr const& scene,
		float fov_in_radians,
		float near_plane_distance,
		float far_plane_distance,
		float aspect_ratio,
		float3 const& position,
		float3 const& direction,
		float3 up_vector,
		math::color const& color,
		bool use_depth = true
	);
	void draw_rectangle( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_aabb( base_scene_ptr const& scene, float3 const& center, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_ellipse( base_scene_ptr const& scene, float4x4 const& matrix, math::color const& color, bool use_depth = true );
	void draw_ellipsoid( base_scene_ptr const& scene, float4x4 const& matrix, math::color const& color, bool use_depth = true );
	void draw_ellipsoid( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_circle( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_cylinder( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_sphere( base_scene_ptr const& scene, float4x4 const& matrix, float const& radius, math::color const& color, bool use_depth = true );
	void draw_cone( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );

	void draw_arrow( base_scene_ptr const& scene, float3 const& start_point, float3 const& end_point, math::color const& line_color, math::color const& cone_color, bool use_depth = true );
	void draw_arrow( base_scene_ptr const& scene, float3 const& start_point, float3 const& end_point, math::color const& color, bool use_depth = true );

	void draw_lines( base_scene_ptr const& scene, vectora< vertex_colored > const& vertices, vectora< u16 > const& indices, bool use_depth = true );
	void draw_lines(
		base_scene_ptr const& scene,
		float4x4 const& matrix,
		float const* vertices,
		u32 vertex_count,
		u16 const* pairs,
		u32 pair_count,
		math::color const& color,
		bool use_depth = true
	);

private:
	void draw_lines(
		base_scene_ptr const& scene,
		float4x4 const& matrix,
		float3 const& size,
		float const* vertices,
		u32 vertex_count,
		u16 const* pairs,
		u32 pair_count,
		math::color const& color,
		bool use_depth = true
	);

public:
	void draw_triangles( base_scene_ptr const& scene, debug_vertices_type const& vertices, bool use_depth = true );
	void draw_triangles( base_scene_ptr const& scene, debug_vertices_type const& vertices, debug_indices_type const& indices, bool use_depth = true );

	void draw_frustum_solid(
		base_scene_ptr const& scene,
		float fov_in_radians,
		float near_plane_distance,
		float far_plane_distance,
		float aspect_ratio,
		float3 const& position,
		float3 const& direction,
		float3 up_vector,
		math::color const& color,
		bool use_depth
	);
	void draw_rectangle_solid( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_cube_solid( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_cone_solid( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_cylinder_solid( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_sphere_solid( base_scene_ptr const& scene, float3 const& center, float const& radius, math::color const& color, bool use_depth = true );
	void draw_ellipsoid_solid( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_line_ellipsoid( base_scene_ptr const& scene, float4x4 const& matrix, math::color const& color, bool use_depth = true );
	void draw_line_hemisphere( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, bool draw_base, math::color const& color, bool use_depth = true );
	void draw_line_capsule( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );
	void draw_solid_capsule( base_scene_ptr const& scene, float4x4 const& matrix, float3 const& size, math::color const& color, bool use_depth = true );

	memory::base_allocator& get_allocator( ) { return m_allocator; }

private:
	void draw_primitive_solid(
		base_scene_ptr const& scene,
		float4x4 const& matrix,
		float3 const& size,
		float const* vertices,
		u32 vertex_count,
		u16 const* faces,
		u32 index_count,
		math::color const& color,
		bool use_depth = true
	);

public:
	math::frustum frustum;

private:
	engine::world& m_render_engine_world;
	one_way_render_channel& m_channel;
	memory::base_allocator& m_allocator;
};

STATIC_SIZE_ASSERT( renderer, 0x84 );

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_RENDERER_H_INCLUDED
