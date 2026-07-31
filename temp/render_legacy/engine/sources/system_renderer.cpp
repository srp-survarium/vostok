////////////////////////////////////////////////////////////////////////////
//	Created		: 17.05.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER: the 17 target-TU functions (ctor body, create_quad_ib,
// is_effects_ready, dtor, set_w, draw_lines, clip_2_screen, frac, calc_pattern,
// draw_screen_lines, draw_3D_point, draw_aabb, draw_triangles, draw_ui_vertices,
// draw_render_models_selection, draw_particle_system_instance_selections,
// draw_speedtree_instance_selections) were ported to
// sources/vostok/render/engine/sources/system_renderer.cpp.
// What remains below has NO stub in the canonical carcass TU (inlined /
// stripped in the target, or dropped from the canonical class) - manual review.

#include "pch.h"
#include "system_renderer.h"
#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>
#include "vertex_formats.h"
#include <vostok/render/core/effect_manager.h>
#include "renderer_context.h"
#include "material_manager.h"
#include <vostok/render/core/res_geometry.h>
#include <vostok/render/core/pix_event_wrapper.h>
#include "aabb_indices.h"
#include <vostok/render/core/render_target.h>
#include "renderer_context_targets.h"
#include "terrain_render_model.h"
#include "scene.h"
#include "scene_view.h"
#include "terrain.h"
#include "render_output_window.h"
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/res_effect.h>

#include <vostok/render/core/destroy_data_helper.h>

#include <vostok/render/core/options.h>


#include <vostok/render/facade/material_effects_instance_cook_data.h>

namespace vostok {
namespace render {

// UNPORTED ctor tail: the canonical system_renderer class dropped
// on_material_loaded / m_terrain_debug_material / m_terrain_debug_material_effects,
// so the editor_terrain_debug query below cannot be adapted. The rest of the
// legacy ctor body was ported. The commented-out state_descriptor block is kept
// as the only legacy reference for m_rotation_mode_states.
system_renderer::system_renderer( renderer_context * renderer_context)
{
	//state_descriptor state_desc;

	//state_desc.color_write_enable( D3D_COLOR_WRITE_ENABLE_NONE);
	//m_rotation_mode_states[0] = resource_manager::ref().create_state( state_desc);

	//state_desc.color_write_enable( D3D_COLOR_WRITE_ENABLE_ALL);
	//m_rotation_mode_states[1] = resource_manager::ref().create_state( state_desc);

	//m_rotation_mode_states[0] = resource_manager::ref().create_state( state_desc);

	//state_desc.set_depth( true, true);
	//state_desc.color_write_enable(D3D_COLOR_WRITE_ENABLE_NONE);

	//m_rotation_mode_states[1] = resource_manager::ref().create_state( state_desc);

	//state_desc.set_stencil( true, 0x00/*ref_value*/, 0xFF, 0xFF, D3D_COMPARISON_ALWAYS, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_REPLACE, D3D_STENCIL_OP_KEEP );
	//state_desc.color_write_enable( D3D_COLOR_WRITE_ENABLE_NONE);

	//m_rotation_mode_states[2] = resource_manager::ref().create_state( state_desc);
	//m_rotation_mode_states[3] = m_rotation_mode_states[2];

	//state_desc.set_stencil( true, 0x00/*ref_value*/, 0xFF, 0xFF, D3D_COMPARISON_ALWAYS, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_DECR_SAT, D3D_STENCIL_OP_KEEP );
	//state_desc.color_write_enable( D3D_COLOR_WRITE_ENABLE_NONE);

	//m_rotation_mode_states[4] = resource_manager::ref().create_state( state_desc);

	//state_desc.set_stencil( true, 0x00/*ref_value*/, 0xFF, 0xFF, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_KEEP );

	//state_desc.color_write_enable( D3D_COLOR_WRITE_ENABLE_ALL);
	//state_desc.set_depth(false, true);
	//m_rotation_mode_states[5] = resource_manager::ref().create_state( state_desc);

	m_cook_data_to_delete			=
		VOSTOK_NEW_IMPL(
			&::vostok::memory::g_mt_allocator, vostok::render::material_effects_instance_cook_data)(
			post_process_vertex_input_type,
			NULL,
			false
		);

	resources::user_data_variant	user_data;
	user_data.set					(m_cook_data_to_delete);

	resources::query_resource		(
		"editor_terrain_debug",
		vostok::resources::material_effects_instance_class,
		boost::bind(&system_renderer::on_material_loaded, this, _1),
		&::vostok::memory::g_mt_allocator,
		&user_data
	);
}

void system_renderer::on_material_loaded(vostok::resources::queries_result& result)
{
	VOSTOK_UNREFERENCED_PARAMETER(result);

 	if (result.is_successful())
 	{
		m_terrain_debug_material = vostok::static_cast_resource_ptr<material_effects_instance_ptr>(result[0].get_unmanaged_resource());
 	}

	VOSTOK_DELETE_IMPL(&::vostok::memory::g_mt_allocator, m_cook_data_to_delete);
}

void system_renderer::set_v	(const float4x4& m)
{
	m_renderer_context->set_v(m);
}
void system_renderer::set_p	(const float4x4& m)
{
	m_renderer_context->set_p(m);
}

const float4x4& system_renderer::get_w	()	const
{
	return m_renderer_context->get_w();
}
const float4x4& system_renderer::get_v	()	const
{
	return m_renderer_context->get_v();
}
const float4x4& system_renderer::get_p	()	const
{
	return m_renderer_context->get_p();
}

void system_renderer::draw_obb			( math::float4x4 const& transform, math::color const& color )
{
	if (!is_effects_ready())
		return;

	render::vertex_colored vertices[] = {
		vertex_colored( transform.transform_position( float3( -1.f, -1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( -1.f, -1.f, +1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( -1.f, +1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, -1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( -1.f, +1.f, +1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, -1.f, +1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, +1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, +1.f, +1.f ) ), color ),
	};

	draw_lines	(
		vertices,
		vertices + array_size(vertices),
		aabb_indices,
		aabb_indices + array_size(aabb_indices)
	);
}

void system_renderer::setup_grid_render_mode	( u32 grid_density )
{
	m_grid_mode = true;
	m_grid_density = grid_density/100.f;
}

void system_renderer::remove_grid_render_mode( )
{
	m_grid_mode = false;
}

void system_renderer::setup_rotation_control_modes ( bool color_write)
{
	m_color_write = color_write;
}

// Legacy ancestor of the canonical member
// system_renderer::fill_surface( surface0..3, depth_rt, reset_depth_rt,
// user_view_port, pos_x, pos_y, size_x, size_y ) [0x644a20] - the shipped
// version diverged (viewport save/restore, 4 surfaces, quad coords), left
// as a seed for the matcher phase.
static void fill_surface(ref_rt surf, renderer_context* context)
{
	float t_w = float(surf->width());
	float t_h = float(surf->height());

	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	backend::ref().reset_depth_stencil_target();

	u32	    color = math::color_rgba( 255u, 255u, 255u, 255u);

	float2	p0( 0,0);
	float2	p1( 1,1);

	u32		offset;

	vertex_formats::TL* pv = ( vertex_formats::TL*)backend::ref().vertex.lock( 4, sizeof(vertex_formats::TL), offset);
	pv->set( 0,	  t_h, 0, 1.0, color, p0.x, p1.y); pv++;
	pv->set( 0,	  0,   0, 1.0, color, p0.x, p0.y); pv++;
	pv->set( t_w, t_h, 0, 1.0, color, p1.x, p1.y); pv++;
	pv->set( t_w, 0,   0, 1.0, color, p1.x, p0.y); pv++;
	backend::ref().vertex.unlock();

	context->m_g_quad_uv->apply();

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}

void system_renderer::draw_debug_terrain( )
{
	if (!is_effects_ready())
		return;


	backend::ref().set_render_targets	(
		&*m_renderer_context->m_targets->m_family[rt_position].target,
		&*m_renderer_context->m_targets->m_family[rt_normal].target,
		&*m_renderer_context->m_targets->m_family[rt_normal_copy].target,
		0
	);
	backend::ref().clear_render_targets	(
		math::color(0.0f, 0.0f, 0.0f, 1.0f),
		math::color(0.0f, 0.0f, 0.0f, 1.0f),
		math::color(0.0f, 0.0f, 0.0f, 1.0f),
		math::color(0.0f, 0.0f, 0.0f, 1.0f)
	);
	backend::ref().reset_depth_stencil_target();
	backend::ref().clear_depth_stencil( D3D_CLEAR_STENCIL, 1.0f, 0);

//////////////////////////////////////////////////////////////////////////
	typedef render::vector<terrain_render_model_instance_ptr>		terrain_ptr_cells;
	typedef terrain_ptr_cells::const_iterator		terrain_ptr_cells_cit;

	terrain_ptr_cells									terrain_cells;

////// Rendering terrain cells ///////////////////////////////////////////

	if (options::ref().m_enabled_draw_terrain && m_renderer_context->scene()->terrain())
	{
		m_renderer_context->scene()->select_terrain_cells( m_renderer_context->get_culling_vp(), terrain_cells);

		terrain_ptr_cells_cit it_tr = terrain_cells.begin();
		terrain_ptr_cells_cit en_tr = terrain_cells.end();

		m_renderer_context->set_w( math::float4x4().identity() );

		for( ; it_tr != en_tr; ++it_tr)
		{
			// TODO: remove this check
			if (!(*it_tr)->m_terrain_model->effect())
				continue;

			(*it_tr)->m_terrain_model->effect()->apply(0);
			//(*it_tr)->m_terrain_model->render_geometry().geom->apply();

			float3	start_corner( 0.f, 0.f, 0.f);
			start_corner = (*it_tr)->m_terrain_model->transform().transform_position( start_corner);

			float3 distance_vector = start_corner + float3( (*it_tr)->m_terrain_model->physical_size()/2, 0, -(*it_tr)->m_terrain_model->physical_size()/2);
			distance_vector -= m_renderer_context->get_view_pos();

			u32 primitive_count = 0;
			if( distance_vector.length() > 150)
			{
				m_renderer_context->scene()->terrain()->m_grid_geom_1->apply();
				primitive_count = m_renderer_context->scene()->terrain()->m_grid_geom_1->intex_buffer()->size()/(sizeof(u16));
			}
			else
			{
				m_renderer_context->scene()->terrain()->m_grid_geom_0->apply();
				primitive_count = m_renderer_context->scene()->terrain()->m_grid_geom_0->intex_buffer()->size()/(sizeof(u16));
			}

//			backend::ref().set_ps_constant( m_object_transparency_scale_parameter, 1.0f);
			backend::ref().set_vs_constant( m_c_start_corner, float4( start_corner, 0));
//			backend::ref().set_ps_constant( m_c_start_corner, float4( start_corner, 0));

			// TODO: remove skylight from sun pass, add the skylight_pass

			backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, primitive_count/*(*it_tr)->m_terrain_model->render_geometry().primitive_count*3*/, 0, 0);
///			statistics::ref().visibility_stat_group.num_triangles.value += primitive_count / 3;
		}
	}

	if (m_terrain_debug_material.c_ptr() &&
		m_terrain_debug_material->get_material_effects().stage_enable[debug_post_process_render_stage] &&
		m_terrain_debug_material->get_material_effects().m_effects[debug_post_process_render_stage])
	{
		m_terrain_debug_material->get_material_effects().m_effects[debug_post_process_render_stage]->apply();
		fill_surface(m_renderer_context->m_targets->m_family[rt_present].target, m_renderer_context);
	}
}


void system_renderer::set_model_ghost_mode( polymorph_vector_base<render_model_instance> const& /*render_models*/, bool /*value*/)
{
	if (!is_effects_ready())
		return;

	//for (polymorph_vector_base<render_model_instance>::iterator it=render_models.begin(); it!=render_models.end(); ++it)
	//{
	//	render_model_instance* instance = (*it);
	//
	//	if (value)
	//	{
	//		m_render_model_to_material[instance] = instance->m_material_effects;
	//
	//		material_effects temp_material;
	//		temp_material.m_material = instance->m_material_effects.m_material;
	//		instance->m_material_effects = temp_material;
	//
	//		instance->m_material_effects.m_effects[forward_render_stage]	 = m_editor_model_ghost_shader;
	//		instance->m_material_effects.stage_enable[forward_render_stage] = true;
	//	}
	//	else
	//	{
	//		if (m_render_model_to_material.find(instance)!=m_render_model_to_material.end())
	//		{
	//			instance->m_material_effects = m_render_model_to_material[instance];
	//			m_render_model_to_material.erase(instance);
	//		}
	//	}
	//}
}

void system_renderer::draw_ghost_render_models	( render_surface_instances& render_models)
{
	if (!is_effects_ready())
		return;

	PIX_EVENT( draw_render_models_selection);

	float3 view_pos = m_renderer_context->get_v_inverted().c.xyz();

	for (render_surface_instances::iterator it=render_models.begin(); it!=render_models.end(); ++it)
	{
		render_surface_instance& instance = *(*it);

		float4x4 selected_transform = *instance.m_transform;
		m_renderer_context->set_w( selected_transform);

		float4 old_current_selection_color = m_current_selection_color;
		m_current_selection_color = m_ghost_model_color;

		m_editor_selection_shader[instance.m_render_surface->get_vertex_input_type()]->apply();
		instance.set_constants();
		instance.m_render_surface->m_render_geometry.geom->apply();
		backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, instance.m_render_surface->m_render_geometry.primitive_count*3, 0, 0);

		m_current_selection_color = old_current_selection_color;
	}
}

void system_renderer::draw_system_render_models	( render_surface_instances const& render_models)
{
	if (!is_effects_ready())
		return;

	PIX_EVENT( draw_system_render_models);

	for (render_surface_instances::const_iterator it=render_models.begin(); it!=render_models.end(); ++it)
	{
		render_surface_instance const& instance = *(*it);

		m_renderer_context->set_w( *instance.m_transform);

		instance.m_render_surface->get_material_effects().m_effects[forward_render_stage]->apply();
		instance.m_render_surface->m_render_geometry.geom->apply();
		backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, instance.m_render_surface->m_render_geometry.primitive_count*3, 0, 0);
	}
}

void system_renderer::setup_scene_view		( scene_view_ptr view_ptr)
{
	m_renderer_context->set_scene_view		( view_ptr);
}

void system_renderer::setup_render_output_window	( render_output_window_ptr in_output_window, viewport_type const& viewport)
{
	render_output_window* output_window = (render_output_window*)in_output_window.c_ptr();

	m_renderer_context->set_target_context		( &output_window->target_context(), false );
	backend::ref().set_render_output			( output_window->render_output() );
	backend::ref().reset_depth_stencil_target	();

	backend::ref().set_render_targets			( &*m_renderer_context->m_targets->m_family[rt_present].target,0, 0, 0);
	backend::ref().reset_depth_stencil_target	( );

	viewport_type res_viewport( math::float2( 0.f, 0.f ), math::float2( 1.f, 1.f ));

	R_ASSERT			( viewport.width() );
	R_ASSERT			( viewport.height() );

	res_viewport.left	= math::max( res_viewport.left,		viewport.left);
	res_viewport.right	= math::min( res_viewport.right,	viewport.right);
	res_viewport.top	= math::max( res_viewport.top,		viewport.top);
	res_viewport.bottom	= math::min( res_viewport.bottom,	viewport.bottom);

	u32 const window_width	= backend::ref().target_width( );
	u32 const window_height	= backend::ref().target_height( );

#pragma warning(push)
#pragma warning(disable:4244)
	D3D_VIEWPORT d3d_viewport	= { window_width * res_viewport.left, window_height * res_viewport.top, window_width * res_viewport.width(), window_height * res_viewport.height(), 0.f, 1.f};
#pragma warning(pop)
	backend::ref().set_viewport	( d3d_viewport);
}

void system_renderer::set_selection_parameters		( vostok::math::float4 in_selection_color, float in_selection_rate )
{
	m_selection_color = in_selection_color;
	m_selection_rate  = in_selection_rate;
}

} // namespace render
} // namespace vostok
