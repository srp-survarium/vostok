////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_VIEW_MODE_H_INCLUDED
#define RENDER_STAGE_VIEW_MODE_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
class vostok::render::scene_view_mode;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::geometry_batch;
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_view_mode : public stage {
public:
	inline			stage_view_mode			( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }
	inline	bool	is_support_view_mode	( scene_view_mode arg_0 ) const { /* no source */ }

	inline	void	execute					( scene_view_mode arg_0 ) { /* no source */ }

	inline	void	pre_lpv_batch_render	( geometry_batch const& arg_0 ) { /* no source */ }
	inline	void	post_lpv_batch_render	( geometry_batch const& arg_0 ) { /* no source */ }
	inline	void	pre_shadow_batch_render	( geometry_batch const& arg_0 ) { /* no source */ }
	inline	void	post_shadow_batch_render( geometry_batch const& arg_0 ) { /* no source */ }

private:
	inline	bool	is_effects_ready		( ) const { /* no source */ }

public:
	virtual			~stage_view_mode		( ) { /* no source */ }

	/* 0x0000 */	/* stage */
private:
	/* 0x0010 */	res_effect_ptr				m_editor_wireframe_accumulation_effect[15];
	/* 0x004c */	res_effect_ptr				m_editor_texture_density_effect[15];
	/* 0x0088 */	res_effect_ptr				m_editor_shader_complexity_effect[15];
	/* 0x00c4 */	res_effect_ptr				m_editor_show_miplevel_effect[15];
	/* 0x0100 */	res_effect_ptr				m_editor_geometry_complexity_effect[15];
	/* 0x013c */	res_effect_ptr				m_editor_show_geometry_effect;
	/* 0x0140 */	res_effect_ptr				m_editor_accumulate_overdraw_effect[15];
	/* 0x017c */	res_effect_ptr				m_editor_vertex_alpha_effect;
	/* 0x0180 */	res_effect_ptr				m_editor_apply_wireframe_shader;
	/* 0x0184 */	res_effect_ptr				m_editor_show_overdraw_shader;
	/* 0x0188 */	shader_constant_host*		m_debug_color_parameter;
	/* 0x018c */	shader_constant_host*		m_show_lpv_geometry_component_index_parameter;
	/* 0x0190 */	shader_constant_host*		m_shader_complexity_parameter;
	/* 0x0194 */	shader_constant_host*		m_shader_complexity_min_parameter;
	/* 0x0198 */	shader_constant_host*		m_shader_complexity_max_parameter;
	/* 0x019c */	shader_constant_host*		m_current_max_texture_dimension_parameter;
	/* 0x01a0 */	shader_constant_host*		m_geometry_complexity_parameters;
	/* 0x01a4 */	shader_constant_host*		m_c_start_corner;
}; // class stage_view_mode

STATIC_SIZE_ASSERT(stage_view_mode, 0x1A8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_VIEW_MODE_H_INCLUDED
