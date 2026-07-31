#ifndef VOSTOK_RENDER_ENGINE_STAGE_VIEW_MODE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_VIEW_MODE_H_INCLUDED

// claude@NOTE: legacy-harvest disposition: the shadow batch-render hooks are absent from the legacy stage_view_mode (it defines only execute) - matcher-phase work.
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/render/facade/vertex_input_type.h>

#include "effect_editor_accumulate_overdraw.h"
#include "effect_editor_apply_wireframe.h"
#include "effect_editor_geometry_complexity.h"
#include "effect_editor_shader_complexity.h"
#include "effect_editor_show_batched_geometry.h"
#include "effect_editor_show_miplevel.h"
#include "effect_editor_show_overdraw.h"
#include "effect_editor_texture_density.h"
#include "effect_editor_wireframe_accumulation.h"
#include "stage.h"

namespace vostok {
namespace render {

struct geometry_batch;
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
		// claude@NOTE: legacy ctor body ported; m_editor_vertex_alpha_effect and the debug-color/
		// show-lpv-component constants have no legacy ancestor - matcher-phase
		for (u32 i=0; i<num_vertex_input_types; i++)
		{
			if (i==post_process_vertex_input_type)
				continue;

			byte data[Kb];
			effect_options_descriptor desc(data, Kb);
			desc["vertex_input_type"] = (enum_vertex_input_type)i;

			effect_manager::ref().create_effect<effect_editor_wireframe_accumulation>(&m_editor_wireframe_accumulation_effect[i], desc);
			effect_manager::ref().create_effect<effect_editor_texture_density>(&m_editor_texture_density_effect[i], desc);
			effect_manager::ref().create_effect<effect_editor_shader_complexity>(&m_editor_shader_complexity_effect[i], desc);
			effect_manager::ref().create_effect<effect_editor_show_miplevel>(&m_editor_show_miplevel_effect[i], desc);
			effect_manager::ref().create_effect<effect_editor_accumulate_overdraw>(&m_editor_accumulate_overdraw_effect[i], desc);
			effect_manager::ref().create_effect<effect_editor_geometry_complexity>(&m_editor_geometry_complexity_effect[i], desc);
		}

		effect_manager::ref().create_effect<effect_editor_show_batched_geometry>(&m_editor_show_geometry_effect);
		effect_manager::ref().create_effect<effect_editor_apply_wireframe>(&m_editor_apply_wireframe_shader);
		effect_manager::ref().create_effect<effect_editor_show_overdraw>(&m_editor_show_overdraw_shader);


		m_shader_complexity_parameter				= backend::ref().register_constant_host("shader_complexity", rc_float);
		m_shader_complexity_min_parameter			= backend::ref().register_constant_host("shader_complexity_min", rc_float);
		m_shader_complexity_max_parameter			= backend::ref().register_constant_host("shader_complexity_max", rc_float);
		m_geometry_complexity_parameters			= backend::ref().register_constant_host("geometry_complexity_parameters", rc_float);
		m_current_max_texture_dimension_parameter	= backend::ref().register_constant_host("current_max_texture_dimension", rc_float);

		m_c_start_corner							= backend::ref().register_constant_host( "start_corner", rc_float);
	}

	virtual ~stage_view_mode( ) { }

	bool is_support_view_mode( scene_view_mode view_mode ) const
	{
		switch (view_mode)
		{
			case geometry_complexity_view_mode:
			case shader_complexity_view_mode:
			case texture_density_view_mode:
			case wireframe_two_sided_view_mode:
			case wireframe_view_mode:
			case miplevel_view_mode:
			case overdraw_view_mode:
				return true;
		}
		return false;
	}

	void execute( scene_view_mode )
	{
		// STATE[STUB]
		// claude@NOTE: legacy ~500-line execute blocked - select_models drift, dropped
		// m_dynamic_visuals, terrain/speedtree machinery; kept in temp/render_legacy remainder
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
		for (u32 i = 0; i < num_vertex_input_types; i++)
		{
			if (i==post_process_vertex_input_type)
				continue;

			if (   m_editor_wireframe_accumulation_effect[i].c_ptr()==NULL
				|| m_editor_texture_density_effect[i].c_ptr()==NULL
				|| m_editor_shader_complexity_effect[i].c_ptr()==NULL
				|| m_editor_show_miplevel_effect[i].c_ptr()==NULL
				|| m_editor_geometry_complexity_effect[i].c_ptr()==NULL
				|| m_editor_accumulate_overdraw_effect[i].c_ptr()==NULL)
			return false;
		}

		return m_editor_apply_wireframe_shader.c_ptr()!=NULL && m_editor_show_overdraw_shader.c_ptr()!=NULL;
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
