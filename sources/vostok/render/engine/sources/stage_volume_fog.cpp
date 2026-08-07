#include "pch.h"
#include "stage_volume_fog.h"

#include <vostok/math_aabb.h>
#include <vostok/math_functions.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>

#include "effect_exponential_volume_fog.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"

namespace vostok {
namespace render {

void effect_simple_fog::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "gather_luminance2", NULL, "fog_simple", shader_configuration( ), NULL );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.set_texture( "t_position", "$user$position", 0, false, 0 );
	compiler.set_stencil(
		true,
		0x80,
		0xff,
		0xff,
		D3D_COMPARISON_LESS,
		D3D_STENCIL_OP_KEEP,
		D3D_STENCIL_OP_KEEP,
		D3D_STENCIL_OP_KEEP
	);
	compiler.end_pass( );
	compiler.end_technique( );
}

stage_volume_fog::stage_volume_fog(
	renderer*			in_renderer,
	renderer_context*	in_context
) :
	stage( in_renderer, in_context ),
	m_fog_sphere_geometry( 32, 32 )
{
	m_eye_ray_corner_parameter = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_inverted_world_matrix_parameter = backend::ref( ).register_constant_host( "inverted_world_matrix", rc_float );
	m_eye_pos_os_parameter = backend::ref( ).register_constant_host( "eye_pos_os", rc_float );
	m_eye_pos_ws_parameter = backend::ref( ).register_constant_host( "eye_pos_ws", rc_float );
	m_is_inside_volume_parameter = backend::ref( ).register_constant_host( "is_inside_volume", rc_int );
	m_fog_parameters0 = backend::ref( ).register_constant_host( "fog_parameters0", rc_float );
	m_fog_parameters1 = backend::ref( ).register_constant_host( "fog_parameters1", rc_float );
	m_fog_parameters2 = backend::ref( ).register_constant_host( "fog_parameters2", rc_float );
	m_fog_parameters3 = backend::ref( ).register_constant_host( "fog_parameters3", rc_float );
	m_far_fog_color_and_distance = backend::ref( ).register_constant_host( "far_fog_color_and_distance", rc_float );
	m_near_fog_distance = backend::ref( ).register_constant_host( "near_fog_distance", rc_float );
	m_fog_alpha = backend::ref( ).register_constant_host( "fog_alpha", rc_float );

	effect_manager::ref( ).create_effect<effect_exponential_volume_fog>( &m_exponential_volume_fog_effect );
	effect_manager::ref( ).create_effect<effect_simple_fog>( &m_simple_fog_effect );

	m_enabled = options::ref( ).current.m_enabled_volume_fog_stage;
}

bool stage_volume_fog::is_effects_ready( ) const
{
	return m_exponential_volume_fog_effect.c_ptr( ) != NULL &&
		m_simple_fog_effect.c_ptr( ) != NULL;
}

void stage_volume_fog::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	float3 const* const eye_rays = m_context->get_eye_rays( );

	vector<volume_fog_parameters> out_instances;
	m_context->scene( )->select_volume_fog_instances( m_context->get_vp( ), out_instances );
	if ( out_instances.size( ) )
	{
		D3D11_VIEWPORT tmp_viewport;
		D3D11_VIEWPORT orig_viewport;
		backend::ref( ).get_viewport( orig_viewport );
		tmp_viewport.TopLeftX = 0.f;
		tmp_viewport.TopLeftY = 0.f;
		tmp_viewport.Width = float( m_context->get_rt( rt_generic_1 )->width( ) );
		tmp_viewport.Height = float( m_context->get_rt( rt_generic_1 )->height( ) );
		tmp_viewport.MinDepth = 0.f;
		tmp_viewport.MaxDepth = 1.f;
		backend::ref( ).set_viewport( tmp_viewport );

		backend::ref( ).set_render_targets( &*m_context->get_rt( rt_generic_0 ), 0, 0, 0 );
		backend::ref( ).reset_depth_stencil_target( );

		float const accuracy = 0.0001f;
		float const far_plane = m_context->get_far( );
		float const near_plane = far_plane - accuracy;
		float4x4 new_p = m_context->get_p( );
		new_p.e22 = far_plane / near_plane;
		new_p.e32 = near_plane / far_plane * -accuracy;
		m_context->push_set_p( new_p );

		for (
			vector<volume_fog_parameters>::iterator i = out_instances.begin( );
			i != out_instances.end( );
			++i
		)
		{
			m_exponential_volume_fog_effect->apply( 0, 0 );

			float4x4 world_interted_matrix = float4x4( ).identity( );
			world_interted_matrix.try_invert( i->transform );
			m_context->set_w( i->transform );

			s32 is_inside_volume = math::create_identity_aabb( ).contains(
				world_interted_matrix.transform_position( m_context->get_view_pos( ) )
			);

			backend::ref( ).set_ps_constant( m_eye_ray_corner_parameter, eye_rays[0].x );
			backend::ref( ).set_vs_constant( m_inverted_world_matrix_parameter, math::transpose( world_interted_matrix ) );
			backend::ref( ).set_vs_constant(
				m_eye_pos_os_parameter,
				world_interted_matrix.transform_position( m_context->get_view_pos( ) )
			);
			backend::ref( ).set_vs_constant( m_eye_pos_ws_parameter, m_context->get_view_pos( ) );
			backend::ref( ).set_vs_constant( m_is_inside_volume_parameter, is_inside_volume );
			backend::ref( ).set_ps_constant( m_fog_parameters0, float4( i->fog_color, i->density ) );
			backend::ref( ).set_ps_constant(
				m_fog_parameters1,
				float4( i->speed, 0.f, i->direction.x, i->direction.y )
			);
			backend::ref( ).set_ps_constant(
				m_fog_parameters2,
				float4(
					i->noise_scale,
					1.f / math::max( i->wave_length, math::epsilon_3 ),
					i->near_density,
					0.f
				)
			);
			backend::ref( ).set_ps_constant(
				m_fog_parameters3,
				float4(
					i->height_falloff_offset,
					i->density_offset,
					i->transparency_multiplier,
					0.f
				)
			);

			m_fog_box_geometry.render( );
		}

		m_context->pop_p( );
		m_context->set_w_identity( );
		backend::ref( ).set_viewport( orig_viewport );
	}

	m_context->set_w_identity( );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
