// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "stage_ambient_occlusion.h"
#include <vostok/console_command.h>
#include "render_target.h"

#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/resource_manager.h>
#include "effect_ssao_accumulation.h"
#include "effect_ssao_downsample_position_and_normal.h"
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
#include "system_renderer.h"

namespace vostok {
namespace render {
static u32 debug_ssao_tech = 0;
static console_commands::cc_u32 s_debug_ssao_techh(
	"r_debug_ssao_tech", debug_ssao_tech, 0, 1, true, console_commands::command_type_user_specific );
static bool s_ssao_filter_0 = true;

static console_commands::cc_bool s_ssao_filter_0_cc( "r_ssao_filter_0", s_ssao_filter_0, false, console_commands::command_type_engine_internal );
stage_ambient_occlusion::stage_ambient_occlusion(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context )
{
	effect_manager::ref( ).create_effect<effect_ssao_accumulation>( &m_sh_ssao_accumulation );
	effect_manager::ref( ).create_effect<effect_ssao_filter4x4>( &m_sh_ssao_filter4x4 );
	effect_manager::ref( ).create_effect<effect_ssao_downsample_position_and_normal>( &m_sh_ssao_downsample_position_and_normal );

	m_c_eye_ray_corner = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_ao_parameters = backend::ref( ).register_constant_host( "ao_parameters", rc_float );
	m_prev_view_parameter = backend::ref( ).register_constant_host( "prev_view", rc_float );
	m_prev_ssao_valid_parameter = backend::ref( ).register_constant_host( "prev_ssao_valid", rc_int );
	m_enabled = options::ref( ).current.m_enabled_post_process_stage &&
		options::ref( ).current.m_enabled_ambient_occlusion_stage;
}

bool stage_ambient_occlusion::is_effects_ready( ) const
{
	return m_sh_ssao_accumulation.c_ptr( ) != 0 &&
		m_sh_ssao_filter4x4.c_ptr( ) != 0 &&
		m_sh_ssao_downsample_position_and_normal.c_ptr( ) != 0;
}

// TODO: Make as general functions.
static void fill_surface( render_target_ptr surf, renderer_context* context, bool null_depth_stencil )
{

	float w = float(surf->width());
	float h = float(surf->height());
	float z = 0.0f;
	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	backend::ref().set_depth_stencil_target(0);
	backend::ref().clear_render_targets( vostok::math::color( float3(0.5f, 0.5f, 1.0f)));

	float2	p0( 0,0);
	float2	p1( 1,1);

	u32		offset;

	float3 const* eye_rays = context->get_eye_rays();
	vertex_formats::Tquad* pv = backend::ref().vertex.lock<vertex_formats::Tquad>( 4, offset);
	pv->set( 0, h, z, 1.0, eye_rays[1].x, eye_rays[1].y, eye_rays[1].z, p0.x, p1.y);
	pv++;
	pv->set( 0, 0, z, 1.0, eye_rays[0].x, eye_rays[0].y, eye_rays[0].z, p0.x, p0.y);
	pv++;
	pv->set( w, h, z, 1.0, eye_rays[3].x, eye_rays[3].y, eye_rays[3].z, p1.x, p1.y);
	pv++;
	pv->set( w, 0, z, 1.0, eye_rays[2].x, eye_rays[2].y, eye_rays[2].z, p1.x, p0.y);
	pv++;
	backend::ref().vertex.unlock();

	context->m_g_quad_eye_ray->apply();

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}

void stage_ambient_occlusion::execute_disabled( )
{
	backend::ref().set_render_targets( &*m_context->get_rt( rt_ssao_accumulator_full_x ), 0, 0, 0);
	backend::ref().clear_render_targets( vostok::math::color( float3( 0.0f, 0.0f, 1.0f ) ) * 1.0f );
}

void stage_ambient_occlusion::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	bool use_ssao = m_context->scene_view( )->post_process_parameters( ).enable_ssao;

	if ( !is_enabled( ) || !use_ssao )
	{
		execute_disabled( );
		return;
	}

	if ( options::ref( ).current.m_ssao_use_temporal_filtering )
	{
		backend::ref( ).set_render_targets( &*m_context->get_rt( rt_ssao_temporal_mask ), 0, 0, 0 );
		backend::ref( ).clear_render_targets( vostok::math::color( 0.0f, 0.0f, 0.0f, 0.0f ) );
	}

	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_ssao_accumulator ), 0, 0, 0 );
	backend::ref( ).clear_render_targets( vostok::math::color( float3( 1.0f, 1.0f, 1.0f ) ) );

	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_ssao_accumulator_full_x ), 0, 0, 0 );
	backend::ref( ).clear_render_targets( vostok::math::color( float3( 1.0f, 1.0f, 1.0f ) ) );

	render::post_process_parameters const& pp_parameters = m_context->scene_view( )->post_process_parameters( );
	BEGIN_CPUGPU_TIMER(statistics::ref( ).ssao_stat_group.ssao_accumulate_time);
	m_sh_ssao_accumulation->apply( 0, 0 );
	backend::ref( ).set_ps_constant( m_ao_parameters, float4( pp_parameters.ssao_saturation, pp_parameters.ssao_radius_scale, 0.0f, 0.0f ) );
	fill_surface( m_context->get_rt( rt_ssao_accumulator ), m_context, false );
	END_CPUGPU_TIMER;
	static float4x4 prev_view = float4x4( ).identity( );

	backend::ref( ).flush_rt_shader_resources( );

	BEGIN_CPUGPU_TIMER(statistics::ref( ).ssao_stat_group.ssao_blurring_time);
	if ( options::ref( ).current.m_ssao_use_filtering )
	{
		if ( options::ref( ).current.m_ssao_use_temporal_filtering )
		{
			m_sh_ssao_filter4x4->apply( effect_ssao_filter4x4::temporal_mask_pass, 0 );
			backend::ref( ).set_ps_constant(
				m_c_eye_ray_corner, ( (float4*)m_context->get_eye_rays( ) )[0] );
			backend::ref( ).set_ps_constant( m_prev_view_parameter, math::transpose( prev_view ) );
			system_renderer::ref( ).fill_surface( m_context->get_rt( rt_ssao_temporal_mask ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), false, 0, 0.0f, 0.0f, 1.0f, 1.0f );
		}

		m_sh_ssao_filter4x4->apply(
			options::ref( ).current.m_ssao_use_temporal_filtering ? effect_ssao_filter4x4::upsample_pass_temporal : effect_ssao_filter4x4::upsample_pass,
			0
		);

		if ( options::ref( ).current.m_ssao_use_temporal_filtering )
		{
			backend::ref( ).set_ps_constant(
				m_c_eye_ray_corner, ( (float4*)m_context->get_eye_rays( ) )[0] );
			backend::ref( ).set_ps_constant( m_prev_view_parameter, math::transpose( prev_view ) );
		}
		system_renderer::ref( ).fill_surface(
			m_context->get_rt( rt_ssao_accumulator_full_x ),
			options::ref( ).current.m_ssao_use_temporal_filtering ? m_context->get_rt( rt_ssao_accumulator_z ) : render_target_ptr( ),
			render_target_ptr( ),
			render_target_ptr( ),
			render_target_ptr( ),
			true, 0, 0.0f, 0.0f, 1.0f, 1.0f
		);
	}
	else
	{
		m_sh_ssao_filter4x4->apply( effect_ssao_filter4x4::filter_0_pass, 0 );
		system_renderer::ref( ).fill_surface( m_context->get_rt( rt_ssao_accumulator_full_x ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );
	}
	END_CPUGPU_TIMER;
	if ( options::ref( ).current.m_ssao_use_temporal_filtering )
	{
		resource_manager::ref( ).copy2D(
			&*m_context->get_t( rt_ssao_prev_accumulator_full_x ),
			0, 0,
			&*m_context->get_t( rt_ssao_accumulator_full_x ), 0, 0,
			m_context->get_t( rt_ssao_accumulator_full_x )->width( ),
			m_context->get_t( rt_ssao_accumulator_full_x )->height( ),
			0, 0
		);
		resource_manager::ref( ).copy2D(
			&*m_context->get_t( rt_ssao_prev_accumulator_z ),
			0, 0,
			&*m_context->get_t( rt_ssao_accumulator_z ),
			0, 0,
			m_context->get_t( rt_ssao_accumulator_z )->width( ),
			m_context->get_t( rt_ssao_accumulator_z )->height( ),
			0, 0
		);
	}
	prev_view = m_context->get_v( );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
