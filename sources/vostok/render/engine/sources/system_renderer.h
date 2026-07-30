#ifndef VOSTOK_RENDER_ENGINE_SYSTEM_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SYSTEM_RENDERER_H_INCLUDED

#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/engine/world_pc.h>
#include "render_model_instance_impl.h"
#include "render_target.h"
#include "speedtree_instance.h"
#include "vertex_formats.h"

namespace vostok {
namespace render {

class renderer_context;
class untyped_buffer;

class system_renderer : public quasi_singleton< system_renderer > {
public:
	explicit system_renderer( renderer_context* renderer_context ) { }
	~system_renderer( );

	untyped_buffer* create_quad_ib( );
	void set_w( float4x4 const& m );

	void draw_lines(
		vertex_colored const* const	vertices_begin,
		vertex_colored const* const	vertices_end,
		u16 const* const			indices_begin,
		u16 const* const			indices_end,
		bool						covering_effect
	);

	void draw_triangles(
		vertex_colored const* const	vertices_begin,
		vertex_colored const* const	vertices_end,
		u16 const* const			indices_begin,
		u16 const* const			indices_end,
		bool						covering_effect
	);

	void draw_screen_lines(
		float3 const*		points,
		u32					count,
		math::color const&	color,
		float				width,
		u32					pattern,
		bool				use_depth,
		bool				is_screen_space_coord
	);

	void draw_3D_point(
		float3 const&		position,
		float				width,
		math::color const&	color,
		bool				use_depth
	);

	void draw_aabb( math::aabb const& aabb, math::color const& color );

	void fill_surface(
		render_target_ptr	surface0,
		render_target_ptr	surface1,
		render_target_ptr	surface2,
		render_target_ptr	surface3,
		render_target_ptr	depth_rt,
		bool				reset_depth_rt,
		D3D11_VIEWPORT*		user_view_port,
		float				pos_x,
		float				pos_y,
		float				size_x,
		float				size_y
	);

	void draw_ui_vertices(
		vertex_formats::TL const*	vertices,
		u32 const&					count,
		s32							prim_type,
		s32							point_type
	);

	void draw_render_models_selection( vector< render_model_instance_impl_ptr >& render_models );
	void draw_particle_system_instance_selections( vector< resources::unmanaged_resource_ptr > const& instances );
	void draw_speedtree_instance_selections( vector< speedtree_instance_ptr > const& instances );

private:
	bool is_effects_ready( ) const;
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SYSTEM_RENDERER_H_INCLUDED
