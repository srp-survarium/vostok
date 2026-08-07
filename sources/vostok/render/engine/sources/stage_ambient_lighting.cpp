#include "pch.h"
#include "stage_ambient_lighting.h"

#include <vostok/geometry_utils.h>
#include <vostok/geometry_primitives.h>
#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/resource_manager.h>

#include "du_sphere.h"
#include "ambient_volume.h"
#include "effect_ambient_volume.h"
#include "effect_environment_probe_lighting.h"
#include "effect_light_mask.h"
#include "effect_reflection_mask.h"
#include "effect_sky_ambient_occlusion.h"
#include "effect_skylight.h"
#include "effect_ssao_downsample_position_and_normal.h"
#include "environment_probe.h"
#include "light.h"
#include "post_process_parameters.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"
#include "system_renderer.h"

#include <vostok/render/core/options.h>

namespace vostok {
namespace render {

static u32 debug_sky_light_tech = 0;
static console_commands::cc_u32 s_debug_sky_light_tech(
	"r_debug_sky_light_tech",
	debug_sky_light_tech,
	0,
	1,
	true,
	console_commands::command_type_user_specific
);

stage_ambient_lighting::stage_ambient_lighting(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context )
{
	// FUNCTION BODY[0x62a800]
	set_ambient_multiplier( 0.0f );
	use_probes( true );

	effect_manager::ref( ).create_effect<effect_light_mask>( &m_effect_accum_mask );
	effect_manager::ref( ).create_effect<effect_sky_ambient_occlusion>( &m_sky_ambient_occlusion_effect );
	effect_manager::ref( ).create_effect<effect_ambient_volume>( &m_ambient_volume_effect );
	effect_manager::ref( ).create_effect<effect_reflection_mask>( &m_reflection_mask_effect );

	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<false, false, false> >( &m_environment_probe_lighting_effect[0][0][0] );
	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<true, false, false> >( &m_environment_probe_lighting_effect[1][0][0] );
	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<false, true, false> >( &m_environment_probe_lighting_effect[0][1][0] );
	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<true, true, false> >( &m_environment_probe_lighting_effect[1][1][0] );
	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<false, false, true> >( &m_environment_probe_lighting_effect[0][0][1] );
	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<true, false, true> >( &m_environment_probe_lighting_effect[1][0][1] );
	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<false, true, true> >( &m_environment_probe_lighting_effect[0][1][1] );
	effect_manager::ref( ).create_effect<effect_environment_probe_lighting<true, true, true> >( &m_environment_probe_lighting_effect[1][1][1] );

	effect_manager::ref( ).create_effect<effect_ssao_downsample_position_and_normal>( &m_sh_ssao_downsample_position_and_normal );

	effect_manager::ref( ).create_effect<effect_skylight>( &m_skylight_effect );

	m_c_eye_ray_corner = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_c_light_range = backend::ref( ).register_constant_host( "light_range", rc_float );
	m_c_num_mips = backend::ref( ).register_constant_host( "num_mips", rc_int );
	m_c_skylight_parameters0 = backend::ref( ).register_constant_host( "skylight_parameters0", rc_float );
	m_c_skylight_parameters1 = backend::ref( ).register_constant_host( "skylight_parameters1", rc_float );
	m_c_skylight_parameters2 = backend::ref( ).register_constant_host( "skylight_parameters2", rc_float );
	m_c_skylight_parameters3 = backend::ref( ).register_constant_host( "skylight_parameters3", rc_float );
	m_c_skylight_parameters4 = backend::ref( ).register_constant_host( "skylight_parameters4", rc_float );
	m_c_skylight_parameters5 = backend::ref( ).register_constant_host( "skylight_parameters5", rc_float );
	m_c_skylight_parameters6 = backend::ref( ).register_constant_host( "skylight_parameters6", rc_float );
	m_c_skylight_parameters7 = backend::ref( ).register_constant_host( "skylight_parameters7", rc_float );

	m_c_color_parameters = backend::ref( ).register_constant_host( "color_parameters", rc_float );
	m_c_world_to_ao_map = backend::ref( ).register_constant_host( "world_to_ao_map", rc_float );
	m_c_ambient_volume_multiplier = backend::ref( ).register_constant_host( "ambient_volume_multiplier", rc_float );
	m_c_world_to_probe = backend::ref( ).register_constant_host( "world_to_probe", rc_float );

	m_sphere_vertex_buffer = resource_manager::ref( ).create_buffer( DU_SPHERE_NUMVERTEX * sizeof( float3 ), du_sphere_vertices, enum_buffer_type_vertex, false, false );
	m_sphere_index_buffer = resource_manager::ref( ).create_buffer( DU_SPHERE_NUMFACES * 3 * sizeof( u16 ), du_sphere_faces, enum_buffer_type_index, false, false );

	D3D_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_sphere_geometry = resource_manager::ref( ).create_geometry( desc, sizeof( float3 ), *m_sphere_vertex_buffer, *m_sphere_index_buffer );

	m_box_vertex_buffer = resource_manager::ref( ).create_buffer(
		geometry_utils::cube_solid::vertex_count * sizeof( float3 ),
		geometry_utils::cube_solid::vertices,
		enum_buffer_type_vertex,
		false,
		false
	);
	u32 const buffer_size = geometry_utils::cube_solid::index_count * sizeof( u16 );
	u16* const indices = static_cast<u16*>( ALLOCA( buffer_size ) );
	std::copy( geometry_utils::cube_solid::faces, geometry_utils::cube_solid::faces + geometry_utils::cube_solid::index_count, indices );
	m_box_index_buffer = resource_manager::ref( ).create_buffer(
		geometry_utils::cube_solid::index_count * sizeof( u16 ),
		indices,
		enum_buffer_type_index,
		false,
		false
	);
	m_box_geometry = resource_manager::ref( ).create_geometry( desc, sizeof( float3 ), *m_box_vertex_buffer, *m_box_index_buffer );
}

bool stage_ambient_lighting::is_effects_ready( ) const
{
	// FUNCTION BODY[0x629a20]
	return m_skylight_effect.c_ptr( ) != NULL &&
		m_sky_ambient_occlusion_effect.c_ptr( ) != NULL &&
		m_ambient_volume_effect.c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[0][0][0].c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[1][0][0].c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[0][1][0].c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[1][1][0].c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[0][0][1].c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[1][0][1].c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[0][1][1].c_ptr( ) != NULL &&
		m_environment_probe_lighting_effect[1][1][1].c_ptr( ) != NULL &&
		m_reflection_mask_effect.c_ptr( ) != NULL &&
		m_sh_ssao_downsample_position_and_normal.c_ptr( ) != NULL;
}

void stage_ambient_lighting::execute( )
{
	if ( !is_enabled( ) || !is_effects_ready( ) )
	{
		execute_disabled( );
		return;
	}

	environment_probe** it = m_context->get_scene_view( )->get_visible_environment_probes( ).begin( );

	struct sort_by_size_predicate {
		bool operator()( environment_probe const* left, environment_probe const* right ) const
		{
			return left->m_properties.radius > right->m_properties.radius;
		}
	};

	std::sort(
		it,
		m_context->get_scene_view( )->get_visible_environment_probes( ).end( ),
		sort_by_size_predicate( )
	);

	float3 const* const eye_rays = m_context->get_eye_rays( );
	post_process_parameters const& parameters = m_context->get_scene_view( )->post_process_parameters( );
	float3 const skylight_color = math::pow( parameters.skylight_color, 2.2f ) * parameters.skylight_multiplier;
	float const skylight_upper_limit = parameters.skylight_upper;
	float const skylight_lower_limit = parameters.skylight_lower;
	float const skylight_power = parameters.skylight_power;

	backend::ref( ).reset_depth_stencil_target( );
	m_skylight_effect->apply( 1, 0 );
	backend::ref( ).set_ps_constant( m_c_skylight_parameters0, float4( skylight_color * m_ambient_multiplier, 0.0f ) );
	backend::ref( ).set_ps_constant( m_c_skylight_parameters1, float4( skylight_lower_limit, skylight_upper_limit - skylight_lower_limit, skylight_power, 0.0f ) );

	light* sun = &*m_context->scene( )->lights( ).get_sun( ).c_ptr( );

	float3 plus_x = parameters.environment_skycolor[0].xyz( );
	float3 minus_x = parameters.environment_skycolor[1].xyz( );
	float3 plus_y = parameters.environment_skycolor[2].xyz( );
	float3 minus_y = parameters.environment_skycolor[3].xyz( );
	float3 plus_z = parameters.environment_skycolor[4].xyz( );
	float3 minus_z = parameters.environment_skycolor[5].xyz( );

	backend::ref( ).set_ps_constant( m_c_skylight_parameters2, float4( plus_x, plus_z.x ) );
	backend::ref( ).set_ps_constant( m_c_skylight_parameters3, float4( minus_x, plus_z.y ) );
	backend::ref( ).set_ps_constant( m_c_skylight_parameters4, float4( minus_y, plus_z.z ) );
	backend::ref( ).set_ps_constant( m_c_skylight_parameters5, float4( minus_z, plus_y.x ) );
	backend::ref( ).set_ps_constant( m_c_skylight_parameters6, float4( sun ? sun->direction : float3( 0.0f, 0.0f, 0.0f ), plus_y.y ) );
	backend::ref( ).set_vs_constant( m_c_skylight_parameters6, float4( sun ? sun->direction : float3( 0.0f, 0.0f, 0.0f ), plus_y.y ) );
	backend::ref( ).set_ps_constant(
		m_c_skylight_parameters7,
		float4(
			sun ? parameters.environment_suncolor.xyz( ) * sun->color * sun->intensity : float3( 0.0f, 0.0f, 0.0f ),
			plus_y.z
		)
	);

	backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( (float4*)eye_rays )[0] );
	system_renderer::ref( ).fill_surface(
		m_context->get_rt( rt_accumulator_diffuse ),
		render_target_ptr( ),
		render_target_ptr( ),
		render_target_ptr( ),
		render_target_ptr( ),
		true,
		0,
		0.0f,
		0.0f,
		1.0f,
		1.0f
	);

	ambient_volume** it_av = m_context->get_scene_view( )->get_visible_ambient_volumes( ).begin( );
	u32 tech_index;
	if ( it_av != m_context->get_scene_view( )->get_visible_ambient_volumes( ).end( ) )
	{
		backend::ref( ).set_render_targets( &*m_context->get_rt( rt_accumulator_diffuse ), 0, 0, 0 );
		backend::ref( ).reset_depth_stencil_target( );

		for ( ; it_av != m_context->get_scene_view( )->get_visible_ambient_volumes( ).end( ); ++it_av )
		{
			ambient_volume* const volume = *it_av;
			if ( volume->is_occluded( ) || !volume->m_properties.enabled )
				continue;

			m_context->set_w( volume->m_properties.transform );

			m_effect_accum_mask->apply( 0, 0 );
			m_box_geometry->apply( );
			backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 36, 0, 0 );

			for ( tech_index = 0; tech_index < 2; ++tech_index )
			{
				m_ambient_volume_effect->apply_pass( tech_index );
				m_box_geometry->apply( );
				backend::ref( ).set_ps_constant( m_c_ambient_volume_multiplier, volume->m_properties.ambient_multiplier );
				backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 36, 0, 0 );
			}
		}
	}

	if ( m_use_probes && it != m_context->get_scene_view( )->get_visible_environment_probes( ).end( ) )
	{
		backend::ref( ).set_render_targets(
			&*m_context->get_rt( rt_accumulator_diffuse ),
			&*m_context->get_rt( rt_accumulator_specular ),
			0,
			0
		);
		backend::ref( ).reset_depth_stencil_target( );

		for ( ; it != m_context->get_scene_view( )->get_visible_environment_probes( ).end( ); ++it )
		{
			environment_probe* probe = *it;
			if ( probe->is_occluded( ) || !probe->m_texture || !probe->m_properties.enabled )
				continue;

			float const radius = probe->m_properties.radius;
			float4x4 world_transform = probe->m_properties.geometry == 0 ?
				math::create_scale( float3( radius, radius, radius ) ) * math::create_translation( probe->m_properties.location ) :
				probe->m_properties.transform;

			m_context->set_w( world_transform );

			m_effect_accum_mask->apply( 0, 0 );
			if ( probe->m_properties.geometry == 0 )
			{
				m_sphere_geometry->apply( );
				backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DU_SPHERE_NUMFACES * 3, 0, 0 );
			}
			else
			{
				m_box_geometry->apply( );
				backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 36, 0, 0 );
			}

			u32 const clip_by_normal = probe->m_properties.clip_by_normal;
			u32 const with_shadows = probe->m_properties.with_shadows;
			u32 const geometry = math::min( probe->m_properties.geometry, 1u );

			for ( tech_index = 0; tech_index < 2; ++tech_index )
			{
				m_environment_probe_lighting_effect[clip_by_normal][with_shadows][geometry]->apply_pass( tech_index );
				backend::ref( ).set_ps_texture( "t_probe_cubemap", &*probe->m_texture );
				if ( probe->m_properties.with_shadows && probe->m_texture_depth )
					backend::ref( ).set_ps_texture( "t_probe_cubemap_depth", &*probe->m_texture_depth );

				backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( (float4*)eye_rays )[0] );
				backend::ref( ).set_ps_constant( m_c_light_range, radius );
				backend::ref( ).set_ps_constant( m_c_num_mips, probe->m_num_mips );
				backend::ref( ).set_ps_constant(
					m_c_color_parameters,
					float2(
						parameters.environment_probes_diffuse_instensity_multiplier * probe->m_properties.diffuse_multiplier,
						parameters.environment_probes_specular_instensity_multiplier * probe->m_properties.specular_multiplier
					)
				);

				if ( probe->m_properties.geometry == 0 )
				{
					m_sphere_geometry->apply( );
					backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DU_SPHERE_NUMFACES * 3, 0, 0 );
				}
				else
				{
					float4x4 world_to_probe = probe->m_properties.transform;
					world_to_probe.try_invert( world_to_probe );
					backend::ref( ).set_ps_constant( m_c_world_to_probe, math::transpose( world_to_probe ) );
					m_box_geometry->apply( );
					backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 36, 0, 0 );
				}
			}
		}
	}

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
