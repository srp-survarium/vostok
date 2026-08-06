#include "pch.h"
#include "stage_gbuffer.h"

#include "effect_fill_reflective_shadow_map.h"
#include "geometry_type.h"
#include "grass_world.h"
#include "material_effects.h"
#include "render_particle_emitter_instance.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "render_target.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"
#include "stage_particles.h"
#include "vertex_formats.h"

#include <vostok/command_line_extensions.h>
#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_render_output.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/facade/vertex_input_type.h>

namespace vostok {
namespace render {

static bool s_debug_profile_dip = false;
static console_commands::cc_bool s_debug_profile_dip_cc(
	"r_debug_profile_dip",
	s_debug_profile_dip,
	false,
	console_commands::command_type_user_specific
);

static command_line::key s_z_only_1("z_only_1", "", "", "");

void effect_copy_depth_rt::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique();
		compiler.begin_pass("post_process0", NULL, "copy_depth_rt", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_fill_mode(D3D_FILL_SOLID);
			compiler.set_alpha_blend(false);
			compiler.set_stencil(true, 0x00, 0xff, 0x00, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();
}

void fill_surface( render_target_ptr surf, renderer_context* context )
{
	float w = float(surf->width());
	float h = float(surf->height());
	float z = 0.0f;

	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	backend::ref().reset_depth_stencil_target();
	backend::ref().clear_render_targets( vostok::math::color( 1.0f, 1.0f, 1.0f, 0.0f));

	float2	p0( 0,0);
	float2	p1( 1,1);

	u32		offset;

	float3 const* eye_rays = context->get_eye_rays();

	// Fill vertex buffer
	vertex_formats::Tquad* pv = backend::ref().vertex.lock<vertex_formats::Tquad>( 4, offset);
	pv->set( 0, h, z, 1.0, eye_rays[1].x, eye_rays[1].y, eye_rays[1].z, p0.x, p1.y); pv++;
	pv->set( 0, 0, z, 1.0, eye_rays[0].x, eye_rays[0].y, eye_rays[0].z, p0.x, p0.y); pv++;
	pv->set( w, h, z, 1.0, eye_rays[3].x, eye_rays[3].y, eye_rays[3].z, p1.x, p1.y); pv++;
	pv->set( w, 0, z, 1.0, eye_rays[2].x, eye_rays[2].y, eye_rays[2].z, p1.x, p0.y); pv++;
	backend::ref().vertex.unlock();

	context->m_g_quad_eye_ray->apply();

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}

stage_gbuffer::stage_gbuffer(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context ),
	m_fill_view_space_depth( false )
{
	m_object_transparency_scale_parameter	= backend::ref().register_constant_host("object_transparency_scale", rc_float);
	m_c_start_corner						= backend::ref().register_constant_host("start_corner", rc_float);
	m_far_fog_color_and_distance			= backend::ref().register_constant_host("far_fog_color_and_distance", rc_float);
	m_c_bound_box_min						= backend::ref().register_constant_host("bound_box_min", rc_float);
	m_c_bound_box_max						= backend::ref().register_constant_host("bound_box_max", rc_float);
	m_c_sun_near_aabb_point				= backend::ref().register_constant_host("sun_near_aabb_point", rc_float);
	m_near_fog_distance						= backend::ref().register_constant_host("near_fog_distance", rc_float);
	m_fog_alpha								= backend::ref().register_constant_host("fog_alpha", rc_float);
	m_ambient_color							= backend::ref().register_constant_host("ambient_color", rc_float );
	m_c_environment_skylight_upper_color	= backend::ref().register_constant_host("environment_skylight_upper_color", rc_float);
	m_c_environment_skylight_lower_color	= backend::ref().register_constant_host("environment_skylight_lower_color", rc_float);
	m_c_environment_skylight_parameters		= backend::ref().register_constant_host("environment_skylight_parameters", rc_float);
	m_c_gs_test_constant					= backend::ref().register_constant_host("gs_test_constant", rc_float);
	m_c_sun_direction						= backend::ref().register_constant_host("sun_direction", rc_float);
	m_c_translucency_max_scatter			= backend::ref().register_constant_host("translucency_max_scatter", rc_float);
	m_c_sun_color							= backend::ref().register_constant_host("sun_color", rc_float);
	m_shadow[0]								= backend::ref().register_constant_host("m_shadow0", rc_float);
	m_shadow[1]								= backend::ref().register_constant_host("m_shadow1", rc_float);
	m_shadow[2]								= backend::ref().register_constant_host("m_shadow2", rc_float);
	m_shadow[3]								= backend::ref().register_constant_host("m_shadow3", rc_float);
	m_wind_info_parameters					= backend::ref().register_constant_host("wind_info_parameters", rc_float);
	m_smoothness_multiplier				= backend::ref().register_constant_host("smoothness_multiplier", rc_float);

	effect_manager::ref().create_effect<effect_copy_depth_rt>(&m_copy_depth_rt);

	u8 data[Kb];
	effect_options_descriptor desc(data, sizeof(data));
	desc["vertex_input_type"] = static_mesh_vertex_input_type;
	effect_manager::ref().create_effect<effect_fill_reflective_shadow_map>(&m_fill_depth_effect, desc);

	m_enabled = options::ref().current.m_enabled_g_stage;
}

stage_gbuffer::~stage_gbuffer( )
{
}

bool stage_gbuffer::is_effects_ready( ) const
{
	return m_copy_depth_rt.c_ptr() && m_fill_depth_effect.c_ptr();
}

void stage_gbuffer::render_models(
	vector<render_surface_instance*>& models,
	u32 shader_lod_index,
	u32& out_num_rendered,
	bool z_only
)
{
	render_surface_instance* const* it = models.begin();
	render_surface_instance* const* end = models.end();

	for (; it != end; ++it)
	{
		render_surface_instance& instance = **it;
		render_surface* surface = instance.m_render_surface;
		material_effects& effects = surface->get_material_effects();

		m_context->set_w(*instance.m_transform);

		if (z_only)
			effects.m_effects[gbuffer_render_stage]->apply(8, 0);
		else
			effects.m_effects[gbuffer_render_stage]->apply(shader_lod_index, 0);

		instance.set_constants();
		surface->m_render_geometry.geom->apply();

		if (effects.is_wind_swings)
		{
			post_process_parameters const& parameters =
				m_context->scene_view()->post_process_parameters();
			float3 wind_info_parameters(
				parameters.wind_direction.x,
				parameters.wind_direction.z,
				parameters.wind_strength
			);
			backend::ref().set_vs_constant(m_wind_info_parameters, wind_info_parameters);
		}

		if (!z_only)
		{
			float const wet_intensity = options::ref().current.m_shading_quality == 0
				? m_context->scene_view()->post_process_parameters().environment_rain_wet_intensity
				: 1.f;
			backend::ref().set_ps_constant(m_smoothness_multiplier, wet_intensity);
		}

		backend::ref().render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			surface->m_render_geometry.primitive_count * 3,
			0,
			0
		);
		++out_num_rendered;
	}
}

bool remove_model_if_not_lod_predicate::operator()( render_surface_instance* in_model )
{
	if (options::ref().current.m_use_hiz_occlusion_culling && in_model->m_occluded)
		return true;

	if (in_model->m_shader_lod_index != m_shader_lod_index)
		return true;

	material_effects& effects = in_model->m_render_surface->get_material_effects();
	if (
		!effects.m_effects[gbuffer_render_stage].c_ptr()
		|| !in_model->m_render_surface->m_render_geometry.geom.c_ptr()
	)
		return true;

	return false;
}

bool remove_model_if_not_static_predicate::operator()( render_surface_instance* in_model )
{
	bool const is_static =
		in_model->m_render_surface->get_vertex_input_type() == wires_vertex_input_type
		|| in_model->m_render_surface->get_vertex_input_type() == static_mesh_vertex_input_type
		|| in_model->m_render_surface->get_vertex_input_type() == static_mesh_vertex_colored_input_type;
	material_effects& effects = in_model->m_render_surface->get_material_effects();
	return effects.has_translucency || !is_static;
}

bool remove_model_if_not_skeletal_predicate::operator()( render_surface_instance* in_model )
{
	material_effects& effects = in_model->m_render_surface->get_material_effects();
	enum_vertex_input_type const vertex_input_type =
		in_model->m_render_surface->get_vertex_input_type();
	return effects.has_translucency
		|| (
			vertex_input_type != skeletal_1_bones_mesh_vertex_input_type
			&& vertex_input_type != skeletal_2_bones_mesh_vertex_input_type
			&& vertex_input_type != skeletal_3_bones_mesh_vertex_input_type
			&& vertex_input_type != skeletal_4_bones_mesh_vertex_input_type
		);
}

bool remove_model_if_not_translucency_predicate::operator()( render_surface_instance* in_model )
{
	material_effects& effects = in_model->m_render_surface->get_material_effects();
	return !effects.has_translucency;
}

bool sort_by_ps_predicate::operator()(
	render_surface_instance const* left,
	render_surface_instance const* right
) const
{
	material_effects const& left_material_effects =
		left->m_render_surface->get_material_effects();
	material_effects const& right_material_effects =
		right->m_render_surface->get_material_effects();

	res_pass const* const left_pass = left_material_effects.m_effects[m_stage_type]
		->get_technique(m_tech_index)->get_pass(0);
	res_pass const* const right_pass = right_material_effects.m_effects[m_stage_type]
		->get_technique(m_tech_index)->get_pass(0);

	return left_pass->get_vs()->hardware_shader()->hardware_shader()
		< right_pass->get_vs()->hardware_shader()->hardware_shader();
}

void stage_gbuffer::z_only_pass( )
{
	vector<render_surface_instance*> visible_models;
	vector<render_surface_instance*> visible_static_models;
	vector<render_surface_instance*> visible_skeletal_models;
	vector<render_surface_instance*> visible_translucency_models;

	visible_models.reserve(2048);
	visible_static_models.reserve(1024);
	visible_skeletal_models.reserve(1024);
	visible_translucency_models.reserve(1024);

	visible_models = m_context->get_scene_view()->get_visible_opaque_models();
	visible_static_models = visible_models;
	visible_skeletal_models = visible_models;
	visible_translucency_models = visible_models;

	visible_static_models.erase(
		std::remove_if(
			visible_static_models.begin(),
			visible_static_models.end(),
			remove_model_if_not_static_predicate()
		),
		visible_static_models.end()
	);
	visible_skeletal_models.erase(
		std::remove_if(
			visible_skeletal_models.begin(),
			visible_skeletal_models.end(),
			remove_model_if_not_skeletal_predicate()
		),
		visible_skeletal_models.end()
	);
	visible_translucency_models.erase(
		std::remove_if(
			visible_translucency_models.begin(),
			visible_translucency_models.end(),
			remove_model_if_not_translucency_predicate()
		),
		visible_translucency_models.end()
	);

	u32 num_rendered = 0;
	render_models(visible_static_models, 0, num_rendered, true);
	render_models(visible_skeletal_models, 0, num_rendered, true);
	render_models(visible_translucency_models, 0, num_rendered, true);
	render_grass(true);
	render_particles(true);
}

void stage_gbuffer::execute( )
{
	if (!is_enabled() || !is_effects_ready())
	{
		execute_disabled();
		return;
	}

	backend::ref().allow_debug_profile_dip = s_debug_profile_dip;
	backend::ref().m_set_ps_sources = true;

	{
	vector<render_surface_instance*> m_visible_models;
	vector<render_surface_instance*> m_visible_static_models;
	vector<render_surface_instance*> m_visible_skeletal_models;
	vector<render_surface_instance*> m_visible_translucency_models;

	m_visible_models.reserve(2048);
	m_visible_static_models.reserve(1024);
	m_visible_skeletal_models.reserve(1024);
	m_visible_translucency_models.reserve(1024);

	backend::ref().set_render_targets(
		&*m_context->get_rt(rt_normal),
		&*m_context->get_rt(rt_albedo),
		&*m_context->get_rt(rt_one_layer_transparency_alpha),
		0
	);
	backend::ref().clear_render_targets(
		math::color(1.f, .5f, .5f, 1.f),
		math::color(0.f, 0.f, 0.f, 0.f),
		math::color(1.f, 1.f, 1.f, 1.f),
		math::color(0.f, 0.f, 0.f, 0.f)
	);
	backend::ref().reset_depth_stencil_target();

	if (s_z_only_1.is_set())
		z_only_pass();

	u32 const num_shader_lods = options::ref().current.m_use_shader_lods ? 2 : 1;
	u32 num_rendered = 0;
	for (u32 shader_lod_index = 0; shader_lod_index < num_shader_lods; ++shader_lod_index)
	{
		m_visible_models = m_context->get_scene_view()->get_visible_opaque_models();
		m_visible_models.erase(
			std::remove_if(
				m_visible_models.begin(),
				m_visible_models.end(),
				remove_model_if_not_lod_predicate(shader_lod_index)
			),
			m_visible_models.end()
		);

		m_visible_static_models = m_visible_models;
		m_visible_skeletal_models = m_visible_models;
		m_visible_translucency_models = m_visible_models;

		m_visible_static_models.erase(
			std::remove_if(
				m_visible_static_models.begin(),
				m_visible_static_models.end(),
				remove_model_if_not_static_predicate()
			),
			m_visible_static_models.end()
		);
		m_visible_skeletal_models.erase(
			std::remove_if(
				m_visible_skeletal_models.begin(),
				m_visible_skeletal_models.end(),
				remove_model_if_not_skeletal_predicate()
			),
			m_visible_skeletal_models.end()
		);
		m_visible_translucency_models.erase(
			std::remove_if(
				m_visible_translucency_models.begin(),
				m_visible_translucency_models.end(),
				remove_model_if_not_translucency_predicate()
			),
			m_visible_translucency_models.end()
		);

		backend::ref().set_stencil_ref(all_geometry_type + static_geometry_type);
		std::sort(
			m_visible_static_models.begin(),
			m_visible_static_models.end(),
			sort_by_ps_predicate(gbuffer_render_stage, 0)
		);
		render_models(m_visible_static_models, shader_lod_index, num_rendered, false);

		backend::ref().set_stencil_ref(all_geometry_type + skeleton_geometry_type);
		std::sort(
			m_visible_skeletal_models.begin(),
			m_visible_skeletal_models.end(),
			sort_by_ps_predicate(gbuffer_render_stage, 0)
		);
		render_models(m_visible_skeletal_models, shader_lod_index, num_rendered, false);

		backend::ref().set_stencil_ref(all_geometry_type + translucency_geometry_type);
		std::sort(
			m_visible_translucency_models.begin(),
			m_visible_translucency_models.end(),
			sort_by_ps_predicate(gbuffer_render_stage, 0)
		);
		render_models(m_visible_translucency_models, shader_lod_index, num_rendered, false);
	}

	backend::ref().set_stencil_ref(all_geometry_type + static_geometry_type);
	render_grass(false);
	render_particles(false);

	if (m_copy_depth_rt.c_ptr())
	{
		m_copy_depth_rt->apply(0, 0);
		backend::ref().set_ps_texture(
			"t_depth_render_target",
			&*backend::ref().m_render_output->m_texture_zb
		);
		fill_surface(m_context->get_rt(rt_position), m_context);
	}
	}

	if (m_fill_view_space_depth)
	{
		backend::ref().set_render_targets(&*m_context->get_rt(rt_position), 0, 0, 0);
		backend::ref().clear_render_targets(math::color(0.f, 0.f, 0.f, 0.f));
		backend::ref().reset_depth_stencil_target();
		backend::ref().clear_depth_stencil(
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.f,
			0
		);

		render_surface_instance* const* it_d =
			m_context->get_scene_view()->get_visible_opaque_models().begin();
		render_surface_instance* const* end_d =
			m_context->get_scene_view()->get_visible_opaque_models().end();
		for (; it_d != end_d; ++it_d)
		{
			render_surface_instance& instance = **it_d;
			render_surface* surface = instance.m_render_surface;
			material_effects& effects = surface->get_material_effects();

			if (!effects.m_effects[gbuffer_render_stage].c_ptr())
				continue;

			if (surface->get_vertex_input_type() == static_mesh_vertex_input_type)
				continue;

			if (!surface->m_render_geometry.geom.c_ptr())
				continue;

			m_context->set_w(*instance.m_transform);
			m_fill_depth_effect->apply(2, 0);
			instance.set_constants();
			surface->m_render_geometry.geom->apply();
			backend::ref().render_indexed(
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				surface->m_render_geometry.primitive_count * 3,
				0,
				0
			);
		}
	}

	backend::ref().reset_render_targets();
	backend::ref().reset_depth_stencil_target();
	backend::ref().m_set_ps_sources = true;
	backend::ref().allow_debug_profile_dip = false;
	s_debug_profile_dip = false;
}

void stage_gbuffer::render_grass( bool z_only )
{
	grass_world* grass = m_context->scene()->get_grass();
	if (grass)
		grass->render(
			m_context,
			m_context->get_view_pos(),
			gbuffer_render_stage,
			z_only ? 8 : 0,
			1000000.f,
			false,
			0,
			false,
			0
		);
}

void stage_gbuffer::render_particles( bool z_only )
{
	particle::world* part_world = m_context->scene()->particle_world();

	if (!part_world)
		return;

	particle::render_particle_emitter_instances_type emitters(g_allocator);
	part_world->get_render_emitter_instances(m_context->get_culling_vp(), emitters);

	for (
		particle::render_particle_emitter_instances_type::const_iterator it = emitters.begin();
		it != emitters.end();
		++it
	)
	{
		render_particle_emitter_instance* instance =
			static_cast<render_particle_emitter_instance*>(*it);
		u32 const num_particles = instance->get_num_particles();

		if (!num_particles)
			continue;

		particle::enum_particle_render_mode particle_render_mode =
			m_context->get_scene_view()->get_particles_render_mode();

		if (
			particle_render_mode == particle::normal_particle_render_mode
			&& instance->get_material_effects().stage_enable[gbuffer_render_stage]
		)
		{
			instance->get_material_effects().m_effects[gbuffer_render_stage]
				->apply(z_only ? 8 : 0, 0);

			particle_shader_constants::ref().set(
				m_context->get_v_inverted().transform_direction(float3(0, 1000, 0)).normalize(),
				m_context->get_v_inverted().transform_direction(float3(1000, 0, 0)).normalize(),
				m_context->get_v_inverted().lines[3].xyz(),
				instance->locked_axis(),
				instance->screen_alignment()
			);
			particle_shader_constants::ref().set_time(m_context->m_current_time);

			m_context->set_w(instance->transform());
			instance->render(m_context->get_v_inverted().lines[3].xyz(), num_particles);
		}
		else
		{
			instance->draw_debug(m_context->get_v(), particle_render_mode);
		}
	}
}

void stage_gbuffer::render_speedtree( )
{
}

} // namespace render
} // namespace vostok
