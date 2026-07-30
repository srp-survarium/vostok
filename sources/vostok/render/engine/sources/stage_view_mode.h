#ifndef VOSTOK_RENDER_ENGINE_STAGE_VIEW_MODE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_VIEW_MODE_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/facade/scene_view_mode.h>

#include "stage.h"

namespace vostok {
namespace render {

class geometry_batch;
class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_view_mode : public stage {
public:
	stage_view_mode( renderer* in_renderer, renderer_context* in_context ) :
		stage( in_renderer, in_context ),
		m_debug_color_parameter( 0 ),
		m_show_lpv_geometry_component_index_parameter( 0 ),
		m_shader_complexity_parameter( 0 ),
		m_shader_complexity_min_parameter( 0 ),
		m_shader_complexity_max_parameter( 0 ),
		m_current_max_texture_dimension_parameter( 0 ),
		m_geometry_complexity_parameters( 0 ),
		m_c_start_corner( 0 )
	{
		// STATE[STUB]
	}

	virtual ~stage_view_mode( ) { }

	bool is_support_view_mode( scene_view_mode ) const
	{
		// STATE[STUB]
		return false;
	}

	void execute( scene_view_mode )
	{
		// STATE[STUB]
	}

	void pre_lpv_batch_render( geometry_batch const& )
	{
		// STATE[STUB]
	}

	void post_lpv_batch_render( geometry_batch const& )
	{
		// STATE[STUB]
	}

	void pre_shadow_batch_render( geometry_batch const& )
	{
		// STATE[STUB]
	}

	void post_shadow_batch_render( geometry_batch const& )
	{
		// STATE[STUB]
	}

private:
	bool is_effects_ready( ) const
	{
		// STATE[STUB]
		return false;
	}

private:
	res_effect_ptr			m_editor_wireframe_accumulation_effect[15];
	res_effect_ptr			m_editor_texture_density_effect[15];
	res_effect_ptr			m_editor_shader_complexity_effect[15];
	res_effect_ptr			m_editor_show_miplevel_effect[15];
	res_effect_ptr			m_editor_geometry_complexity_effect[15];
	res_effect_ptr			m_editor_show_geometry_effect;
	res_effect_ptr			m_editor_accumulate_overdraw_effect[15];
	res_effect_ptr			m_editor_vertex_alpha_effect;
	res_effect_ptr			m_editor_apply_wireframe_shader;
	res_effect_ptr			m_editor_show_overdraw_shader;
	shader_constant_host*	m_debug_color_parameter;
	shader_constant_host*	m_show_lpv_geometry_component_index_parameter;
	shader_constant_host*	m_shader_complexity_parameter;
	shader_constant_host*	m_shader_complexity_min_parameter;
	shader_constant_host*	m_shader_complexity_max_parameter;
	shader_constant_host*	m_current_max_texture_dimension_parameter;
	shader_constant_host*	m_geometry_complexity_parameters;
	shader_constant_host*	m_c_start_corner;
};

STATIC_SIZE_ASSERT( stage_view_mode, 0x1A8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_VIEW_MODE_H_INCLUDED
