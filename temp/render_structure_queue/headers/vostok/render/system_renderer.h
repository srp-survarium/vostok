////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SYSTEM_RENDERER_H_INCLUDED
#define RENDER_SYSTEM_RENDERER_H_INCLUDED

/* INCLUDES */
class vostok::quasi_singleton<vostok::render::system_renderer>;
class vostok::render::index_buffer;
class vostok::render::map<vostok::render::render_model_instance *,vostok::render::material_effects,stlp_std::less<vostok::render::render_model_instance *> >;
class vostok::render::renderer_context;
class vostok::render::shader_constant_host;
class vostok::render::vertex_buffer;
struct vostok::render::material_effects_instance_cook_data;
class vostok::render::base_output_window;
class vostok::render::base_scene_view;
class vostok::render::render_model_instance_impl;
class vostok::render::render_target;
class vostok::render::speedtree_instance;
class vostok::render::vertex_colored const* const;
class vostok::resources::unmanaged_resource;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_state,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_state_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;
class vostok::render::res_geometry;
class vostok::render::res_state;
class vostok::render::res_texture;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::math::aabb;
class vostok::math::color;
class vostok::math::rectangle<vostok::math::float2>;
class vostok::render::polymorph_vector_base<vostok::render::render_model_instance>;
class vostok::render::vector<vostok::render::render_surface_instance *>;
class vostok::render::vertex_formats::TL;

namespace vostok {
namespace render {

class system_renderer : public quasi_singleton< system_renderer > {
public:
	inline	explicit			system_renderer						( renderer_context* arg_0 ) { /* no source */ }
								~system_renderer					( );

			untyped_buffer*		create_quad_ib						( );

			void				set_w								( float4x4 const& m );
	inline	void				set_v								( float4x4 const& arg_0 ) { /* no source */ }
	inline	void				set_p								( float4x4 const& arg_0 ) { /* no source */ }

	inline	float4x4 const&		get_w								( ) const { /* no source */ }
	inline	float4x4 const&		get_v								( ) const { /* no source */ }
	inline	float4x4 const&		get_p								( ) const { /* no source */ }

	inline	void				setup_scene_view					( base_scene_view_ptr arg_0 ) { /* no source */ }
	inline	void				setup_render_output_window			( base_output_window_ptr arg_0, math::rectangle< float2 > const& arg_1 ) { /* no source */ }

			void				draw_lines							(
									vertex_colored const* const		vertices_begin,
									vertex_colored const* const		vertices_end,
									u16 const* const				indices_begin,
									u16 const* const				indices_end,
									bool							covering_effect
								);
			void				draw_triangles						(
									vertex_colored const* const		vertices_begin,
									vertex_colored const* const		vertices_end,
									u16 const* const				indices_begin,
									u16 const* const				indices_end,
									bool							covering_effect
								);
			void				draw_screen_lines					(
									float3 const*			points,
									u32						count,
									math::color const&		color,
									float					width,
									u32						pattern,
									bool					use_depth,
									bool					is_screen_space_coord
								);
			void				draw_3D_point						(
									float3 const&			position,
									float					width,
									math::color const&		color,
									bool					use_depth
								);
			void				draw_aabb							( math::aabb const& aabb, math::color const& color );
	inline	void				draw_obb							( float4x4 const& arg_0, math::color const& arg_1 ) { /* no source */ }

	inline	void				setup_grid_render_mode				( u32 arg_0 ) { /* no source */ }
	inline	void				remove_grid_render_mode				( ) { /* no source */ }

			void				fill_surface						(
									render_target_ptr		surface0,
									render_target_ptr		surface1,
									render_target_ptr		surface2,
									render_target_ptr		surface3,
									render_target_ptr		depth_rt,
									bool					reset_depth_rt,
									D3D11_VIEWPORT*			user_view_port,
									float					pos_x,
									float					pos_y,
									float					size_x,
									float					size_y
								);

	inline	void				setup_rotation_control_modes		( bool arg_0 ) { /* no source */ }

			void				draw_ui_vertices					(
									vertex_formats::TL const*		vertices,
									u32 const&						count,
									s32								prim_type,
									s32								point_type
								);

	inline	void				set_model_ghost_mode				( polymorph_vector_base< render_model_instance > const& arg_0, bool arg_1 ) { /* no source */ }

	inline	void				draw_ghost_render_models			( vector< render_surface_instance* >& arg_0 ) { /* no source */ }
			void				draw_render_models_selection		( vector< render_model_instance_impl_ptr >& render_models );
	inline	void				draw_system_render_models			( vector< render_surface_instance* > const& arg_0 ) { /* no source */ }
	inline	void				draw_debug_terrain					( ) { /* no source */ }
			void				draw_particle_system_instance_selections( vector< resources::unmanaged_resource_ptr > const& instances );
			void				draw_speedtree_instance_selections	( vector< speedtree_instance_ptr > const& instances );

	inline	void				set_selection_parameters			( float4 arg_0, float arg_1 ) { /* no source */ }

private:
			bool				is_effects_ready					( ) const;

	/* 0x0000 */	/* quasi_singleton< system_renderer > */
	/* 0x0000 */	material_effects_instance_cook_data*	m_cook_data_to_delete;
	/* 0x0004 */	untyped_buffer_ptr			m_screen_vertex_ib;
	/* 0x0008 */	res_geometry_ptr			m_screen_vertex_geometry;
	/* 0x000c */	map< render_model_instance*, material_effects, std::less< render_model_instance* > >	m_render_model_to_material;
	/* 0x0024 */	float4						m_selection_color;
	/* 0x0034 */	float						m_selection_rate;
	/* 0x0038 */	float4						m_current_selection_color;
	/* 0x0048 */	float4						m_ghost_model_color;
	/* 0x0058 */	renderer_context*			m_renderer_context;
	/* 0x005c */	res_effect_ptr				m_sh_sl;
	/* 0x0060 */	res_geometry_ptr			m_colored_geom_sl;
	/* 0x0064 */	shader_constant_host*		m_WVP_sl;
	/* 0x0068 */	shader_constant_host*		m_c_start_corner;
	/* 0x006c */	vertex_buffer				m_vertex_stream;
	/* 0x0084 */	index_buffer				m_index_stream;
	/* 0x0098 */	vertex_buffer				m_vertex_stream_quad;
	/* 0x00b0 */	index_buffer				m_index_stream_quad;
	/* 0x00c4 */	res_effect_ptr				m_sh_particle_selection;
	/* 0x00c8 */	res_effect_ptr				m_sh_vcolor;
	/* 0x00cc */	res_effect_ptr				m_sh_grid_25;
	/* 0x00d0 */	res_effect_ptr				m_sh_grid_50;
	/* 0x00d4 */	res_effect_ptr				m_sh_ui;
	/* 0x00d8 */	res_effect_ptr				m_notexture_shader;
	/* 0x00dc */	res_effect_ptr				m_editor_selection_shader[15];
	/* 0x0118 */	res_effect_ptr				m_speedtree_selection_shader;
	/* 0x011c */	res_effect_ptr				m_editor_model_ghost_shader;
	/* 0x0120 */	res_geometry_ptr			m_colored_geom;
	/* 0x0124 */	res_geometry_ptr			m_grid_geom;
	/* 0x0128 */	res_geometry_ptr			m_ui_geom;
	/* 0x012c */	res_texture_ptr				m_grid_texture_25;
	/* 0x0130 */	res_texture_ptr				m_grid_texture_50;
	/* 0x0134 */	res_state_ptr				m_rotation_mode_states[2];
	/* 0x013c */	bool						m_grid_mode;
	/* 0x0140 */	float						m_grid_density;
	/* 0x0144 */	shader_constant_host*		m_grid_density_constant;
	/* 0x0148 */	bool						m_color_write;
	/* 0x014c */	u32							m_rotation_mode_ref_value;
}; // class system_renderer

STATIC_SIZE_ASSERT(system_renderer, 0x150);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SYSTEM_RENDERER_H_INCLUDED
