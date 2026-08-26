#include "pch.h"
#include "stage_view_mode.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/facade/vertex_input_type.h>

#include "effect_editor_accumulate_overdraw.h"
#include "effect_editor_apply_wireframe.h"
#include "effect_editor_geometry_complexity.h"
#include "effect_editor_shader_complexity.h"
#include "effect_editor_show_batched_geometry.h"
#include "effect_editor_show_miplevel.h"
#include "effect_editor_show_overdraw.h"
#include "effect_editor_texture_density.h"
#include "effect_editor_vertex_alpha.h"
#include "effect_editor_wireframe_accumulation.h"

namespace vostok {
namespace render {

stage_view_mode::stage_view_mode( renderer* in_renderer, renderer_context* context ) :
	stage( in_renderer, context )
{
	for (u32 i=0; i<num_vertex_input_types; i++)
	{
		if (i==post_process_vertex_input_type)
			continue;

		byte data[Kb];
		effect_options_descriptor desc(data, Kb);
		desc["vertex_input_type"] = (enum_vertex_input_type)i;
		desc["cull_mode"] = D3D11_CULL_BACK;

		effect_manager::ref().create_effect<effect_editor_wireframe_accumulation>(&m_editor_wireframe_accumulation_effect[i], desc);
		effect_manager::ref().create_effect<effect_editor_texture_density>(&m_editor_texture_density_effect[i], desc);
		effect_manager::ref().create_effect<effect_editor_shader_complexity>(&m_editor_shader_complexity_effect[i], desc);
		effect_manager::ref().create_effect<effect_editor_show_miplevel>(&m_editor_show_miplevel_effect[i], desc);
		effect_manager::ref().create_effect<effect_editor_accumulate_overdraw>(&m_editor_accumulate_overdraw_effect[i], desc);
		effect_manager::ref().create_effect<effect_editor_geometry_complexity>(&m_editor_geometry_complexity_effect[i], desc);
	}

	{
		byte data[Kb];
		effect_options_descriptor desc(data, Kb);
		desc["vertex_input_type"] = static_mesh_vertex_input_type;
		desc["cull_mode"] = D3D11_CULL_NONE;
		effect_manager::ref().create_effect<effect_editor_show_batched_geometry>(&m_editor_show_geometry_effect, desc);
	}

	{
		byte data[Kb];
		effect_options_descriptor desc(data, Kb);
		desc["vertex_input_type"] = static_mesh_vertex_colored_input_type;
		desc["cull_mode"] = D3D11_CULL_NONE;
		effect_manager::ref().create_effect<effect_editor_vertex_alpha>(&m_editor_vertex_alpha_effect, desc);
	}

	effect_manager::ref().create_effect<effect_editor_apply_wireframe>(&m_editor_apply_wireframe_shader);
	effect_manager::ref().create_effect<effect_editor_show_overdraw>(&m_editor_show_overdraw_shader);

	m_show_lpv_geometry_component_index_parameter	= backend::ref().register_constant_host("show_component_index", rc_int);
	m_debug_color_parameter					= backend::ref().register_constant_host("debug_color", rc_float);
	m_shader_complexity_parameter				= backend::ref().register_constant_host("shader_complexity", rc_float);
	m_shader_complexity_min_parameter			= backend::ref().register_constant_host("shader_complexity_min", rc_float);
	m_shader_complexity_max_parameter			= backend::ref().register_constant_host("shader_complexity_max", rc_float);
	m_geometry_complexity_parameters			= backend::ref().register_constant_host("geometry_complexity_parameters", rc_float);
	m_current_max_texture_dimension_parameter	= backend::ref().register_constant_host("current_max_texture_dimension", rc_float);

	m_c_start_corner							= backend::ref().register_constant_host( "start_corner", rc_float);
}

} // namespace render
} // namespace vostok
