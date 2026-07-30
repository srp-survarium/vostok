#ifndef VOSTOK_RENDER_ENGINE_SYSTEM_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SYSTEM_RENDERER_H_INCLUDED

#include <vostok/render/core/index_buffer.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/res_pass.h>
#include <vostok/render/core/res_state.h>
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/core/vertex_buffer.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/engine/world_pc.h>

#include "material_effects.h"
#include "render_model_instance_impl.h"
#include "render_target.h"
#include "res_geometry.h"
#include "speedtree_instance.h"
#include "vertex_formats.h"

namespace vostok {
namespace render {

struct material_effects_instance_cook_data;
struct render_surface_instance;

class render_model_instance;
class renderer_context;
class shader_constant_host;

template < typename T >
class polymorph_vector_base;

class system_renderer : public quasi_singleton< system_renderer > {
public:
	explicit system_renderer( renderer_context* renderer_context );
	~system_renderer( );

	untyped_buffer* create_quad_ib( );

	void set_w( float4x4 const& matrix );
	void set_v( float4x4 const& matrix );
	void set_p( float4x4 const& matrix );

	float4x4 const& get_w( ) const;
	float4x4 const& get_v( ) const;
	float4x4 const& get_p( ) const;

	void setup_scene_view( base_scene_view_ptr view );
	void setup_render_output_window(
		base_output_window_ptr output_window,
		math::rectangle< float2 > const& viewport
	);

	void draw_lines(
		vertex_colored const* const vertices_begin,
		vertex_colored const* const vertices_end,
		u16 const* const indices_begin,
		u16 const* const indices_end,
		bool covering_effect
	);

	void draw_triangles(
		vertex_colored const* const vertices_begin,
		vertex_colored const* const vertices_end,
		u16 const* const indices_begin,
		u16 const* const indices_end,
		bool covering_effect
	);

	void draw_screen_lines(
		float3 const* points,
		u32 count,
		math::color const& color,
		float width,
		u32 pattern,
		bool use_depth,
		bool is_screen_space_coord
	);

	void draw_3D_point(
		float3 const& position,
		float width,
		math::color const& color,
		bool use_depth
	);

	void draw_aabb( math::aabb const& aabb, math::color const& color );
	void draw_obb( float4x4 const& transform, math::color const& color );

	void setup_grid_render_mode( u32 grid_density );
	void remove_grid_render_mode( );

	void fill_surface(
		render_target_ptr surface0,
		render_target_ptr surface1,
		render_target_ptr surface2,
		render_target_ptr surface3,
		render_target_ptr depth_rt,
		bool reset_depth_rt,
		D3D11_VIEWPORT* user_view_port,
		float pos_x,
		float pos_y,
		float size_x,
		float size_y
	);

	void setup_rotation_control_modes( bool color_write );

	void draw_ui_vertices(
		vertex_formats::TL const* vertices,
		u32 const& count,
		s32 prim_type,
		s32 point_type
	);

	void set_model_ghost_mode(
		polymorph_vector_base< render_model_instance > const& render_models,
		bool value
	);

	void draw_ghost_render_models( vector< render_surface_instance* >& render_models );
	void draw_render_models_selection( vector< render_model_instance_impl_ptr >& render_models );
	void draw_system_render_models( vector< render_surface_instance* > const& render_models );
	void draw_debug_terrain( );
	void draw_particle_system_instance_selections(
		vector< resources::unmanaged_resource_ptr > const& instances
	);
	void draw_speedtree_instance_selections(
		vector< speedtree_instance_ptr > const& instances
	);

	void set_selection_parameters( float4 selection_color, float selection_rate );

private:
	bool is_effects_ready( ) const;

private:
	material_effects_instance_cook_data* m_cook_data_to_delete;
	untyped_buffer_ptr m_screen_vertex_ib;
	res_geometry_ptr m_screen_vertex_geometry;
	map< render_model_instance*, material_effects > m_render_model_to_material;
	float4 m_selection_color;
	float m_selection_rate;
	float4 m_current_selection_color;
	float4 m_ghost_model_color;
	renderer_context* m_renderer_context;
	res_effect_ptr m_sh_sl;
	res_geometry_ptr m_colored_geom_sl;
	shader_constant_host* m_WVP_sl;
	shader_constant_host* m_c_start_corner;
	vertex_buffer m_vertex_stream;
	index_buffer m_index_stream;
	vertex_buffer m_vertex_stream_quad;
	index_buffer m_index_stream_quad;
	res_effect_ptr m_sh_particle_selection;
	res_effect_ptr m_sh_vcolor;
	res_effect_ptr m_sh_grid_25;
	res_effect_ptr m_sh_grid_50;
	res_effect_ptr m_sh_ui;
	res_effect_ptr m_notexture_shader;
	res_effect_ptr m_editor_selection_shader[15];
	res_effect_ptr m_speedtree_selection_shader;
	res_effect_ptr m_editor_model_ghost_shader;
	res_geometry_ptr m_colored_geom;
	res_geometry_ptr m_grid_geom;
	res_geometry_ptr m_ui_geom;
	res_texture_ptr m_grid_texture_25;
	res_texture_ptr m_grid_texture_50;
	res_state_ptr m_rotation_mode_states[2];
	bool m_grid_mode;
	float m_grid_density;
	shader_constant_host* m_grid_density_constant;
	bool m_color_write;
	u32 m_rotation_mode_ref_value;
};

STATIC_SIZE_ASSERT( system_renderer, 0x150 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SYSTEM_RENDERER_H_INCLUDED
