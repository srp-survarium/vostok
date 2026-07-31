#include "pch.h"
#include "stage_ambient_occlusion.h"

#include "render_target.h"

#include <vostok/render/core/effect_manager.h>
#include "effect_ssao_accumulation.h"
#include "effect_ssao_filter4x4.h"
#include "vertex_formats.h"
#include <vostok/render/core/backend.h>
#include "renderer_context.h"
#include <vostok/render/core/dx11/res_geometry.h>
#include "renderer_context_targets.h"
#include "scene.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/options.h>
#include "scene_view.h"
#include "statistics.h"

namespace vostok {
namespace render {

stage_ambient_occlusion::stage_ambient_occlusion(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage						( in_renderer, context ),
	m_ao_parameters				( 0 ),
	m_c_eye_ray_corner			( 0 ),
	m_prev_view_parameter		( 0 ),
	m_prev_ssao_valid_parameter	( 0 )
{
	// FUNCTION BODY[0x70c950]
	// claude@NOTE: canonical adds m_sh_combine/m_sh_ssao_downsample_position_and_normal/
	// m_post_process_* effects, m_g_combine and m_vb with no legacy ancestor - matcher-phase
	effect_manager::ref().create_effect<effect_ssao_accumulation>(&m_sh_ssao_accumulation);
	effect_manager::ref().create_effect<effect_ssao_filter4x4>(&m_sh_ssao_filter4x4);

	m_ao_parameters	= backend::ref().register_constant_host( "ao_parameters", rc_float );

	m_enabled		= options::ref().current.m_enabled_ambient_occlusion_stage;
}

bool stage_ambient_occlusion::is_effects_ready( ) const
{
	// FUNCTION BODY[0x70be40]
	return m_sh_ssao_accumulation.c_ptr() != NULL
		&& m_sh_ssao_filter4x4.c_ptr() != NULL;
}

// TODO: Make as general functions.
static void fill_surface(
	render_target_ptr	surf,
	renderer_context*	context,
	bool
)
{
	// FUNCTION BODY[0x70bf50]
	// claude@NOTE: third bool parameter is post-legacy; legacy 2-arg body ported, flag unused
	float w = float(surf->width());
	float h = float(surf->height());
	float z = 0.0f;

	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	//backend::ref().set_depth_stencil_target(0);//reset_depth_stencil_target();
	backend::ref().clear_render_targets( vostok::math::color( float3(1.0f, 1.0f, 1.0f)));

	//u32	    color = math::color_rgba( 255,255,255,255);

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

void stage_ambient_occlusion::execute_disabled( )
{
	// FUNCTION BODY[0x70be60]
	backend::ref().set_render_targets( &*m_context->m_targets->m_family[rt_ssao_accumulator].target, 0, 0, 0);
	backend::ref().clear_render_targets( vostok::math::color( 1.0f, 1.0f, 1.0f, 0.0f));
}

void stage_ambient_occlusion::execute( )
{
	// FUNCTION BODY[0x70c260]
	// claude@NOTE: legacy PIX_EVENT(stage_ambient_occlusion) dropped - zero PIX symbols in target

	if (!is_effects_ready())
		return;

	if (!is_enabled())
	{
		execute_disabled();
		return;
	}

	bool use_ssao = m_context->scene_view()->post_process_parameters().enable_ssao;

	if (use_ssao)
	{
		backend::ref().set_depth_stencil_target(NULL);

		render::post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();

		BEGIN_CPUGPU_TIMER(statistics::ref().ssao_stat_group.ssao_accumulate_time);

		// SSAO pass.
		m_sh_ssao_accumulation->apply( 0, 0 );
		backend::ref().set_ps_constant(m_ao_parameters, float4(pp_parameters.ssao_saturation, pp_parameters.ssao_radius_scale, 0.0f, 0.0f));
		fill_surface(m_context->m_targets->m_family[rt_ssao_accumulator].target, m_context, false);

		END_CPUGPU_TIMER;

		backend::ref().flush_rt_shader_resources();

		BEGIN_CPUGPU_TIMER(statistics::ref().ssao_stat_group.ssao_blurring_time);

		// SSAO filter 4x4
		m_sh_ssao_filter4x4->apply( 0, 0 );
		backend::ref().set_ps_texture("t_ssao_accumulator", &*m_context->m_targets->m_family[rt_ssao_accumulator].texture);
		fill_surface(m_context->m_targets->m_family[rt_ssao_temporal_mask].target, m_context, false);
		backend::ref().flush_rt_shader_resources();

		// SSAO filter 4x4
		m_sh_ssao_filter4x4->apply( 0, 0 );
		backend::ref().set_ps_texture("t_ssao_accumulator", &*m_context->m_targets->m_family[rt_ssao_temporal_mask].texture);
		fill_surface(m_context->m_targets->m_family[rt_ssao_accumulator].target, m_context, false);
		backend::ref().flush_rt_shader_resources();

		// SSAO filter 4x4
		m_sh_ssao_filter4x4->apply( 1, 0 );
		backend::ref().set_ps_texture("t_ssao_accumulator", &*m_context->m_targets->m_family[rt_ssao_accumulator].texture);
		fill_surface(m_context->m_targets->m_family[rt_ssao_temporal_mask].target, m_context, false);
		backend::ref().flush_rt_shader_resources();

		// SSAO filter 4x4
		m_sh_ssao_filter4x4->apply( 1, 0 );
		backend::ref().set_ps_texture("t_ssao_accumulator", &*m_context->m_targets->m_family[rt_ssao_temporal_mask].texture);
		fill_surface(m_context->m_targets->m_family[rt_ssao_accumulator].target, m_context, false);

		END_CPUGPU_TIMER;
	}
	else
	{
		backend::ref().set_render_targets( &*m_context->m_targets->m_family[rt_ssao_accumulator].target, 0, 0, 0);
		backend::ref().clear_render_targets( vostok::math::color( 1.0f, 1.0f, 1.0f, 0.0f));
	}
	backend::ref().reset_render_targets();
	backend::ref().reset_depth_stencil_target();
}

} // namespace render
} // namespace vostok
