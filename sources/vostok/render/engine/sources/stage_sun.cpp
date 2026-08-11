#include "pch.h"
#include "convex_volume.h"
#include "stage_sun.h"

#include <vostok/console_command.h>
#include <vostok/math_half.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include "cloud_simulation.h"
#include "effect_sun.h"
#include "light.h"
#include "lights_db.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"
#include "system_renderer.h"

static bool s_debug_set_cascade_textures = true;
static vostok::console_commands::cc_bool s_debug_set_cascade_textures_cc(
	"r_debug_set_cascade_textures",
	s_debug_set_cascade_textures,
	false,
	vostok::console_commands::command_type_user_specific
);

namespace vostok {
namespace render {

stage_sun::stage_sun(
	renderer*				in_renderer,
	renderer_context*		in_context,
	cloud_interp_textures&	in_cloud_interp_textures,
	cloud_simulation&		in_simulation
) :
	stage						( in_renderer, in_context ),
	m_cloud_interp_textures		( in_cloud_interp_textures ),
	m_simulation				( in_simulation )
{
	// 9 target lines are likely retail-compiled-out source.
	m_c_light_color					= backend::ref().register_constant_host( "light_color", rc_float );
	m_c_light_direction				= backend::ref().register_constant_host( "light_direction", rc_float );
	m_c_light_intensity				= backend::ref().register_constant_host( "light_intensity", rc_float );
	m_c_shadow_transparency			= backend::ref().register_constant_host( "shadow_transparency",  rc_float );
	m_c_diffuse_influence_factor	= backend::ref().register_constant_host( "light_diffuse_influence_factor", rc_float );
	m_c_specular_influence_factor	= backend::ref().register_constant_host( "light_specular_influence_factor", rc_float );
	m_c_inverted_view_projection_matrix = backend::ref().register_constant_host( "inverted_view_projection_matrix", rc_float );
	m_c_sun_fixed_matrix				= backend::ref().register_constant_host( "sun_fixed_matrix", rc_float );
	m_c_eye_ray_corner				= backend::ref().register_constant_host( "s_eye_ray_corner", rc_float );
	m_shadow[0]					= backend::ref().register_constant_host( "m_shadow0", rc_float );
	m_shadow[1]					= backend::ref().register_constant_host( "m_shadow1", rc_float );
	m_shadow[2]					= backend::ref().register_constant_host( "m_shadow2", rc_float );
	m_shadow[3]					= backend::ref().register_constant_host( "m_shadow3", rc_float );
	m_c_clouds_offset				= backend::ref().register_constant_host( "clouds_offset", rc_float );
	m_c_world_to_cloud				= backend::ref().register_constant_host( "world_to_cloud", rc_float );
	m_c_cloud_interp_alpha			= backend::ref().register_constant_host( "cloud_interp_alpha", rc_float );
	m_c_environment_skylight_upper_color = backend::ref().register_constant_host( "environment_skylight_upper_color", rc_float );
	m_c_environment_skylight_lower_color = backend::ref().register_constant_host( "environment_skylight_lower_color", rc_float );

	m_enabled					= options::ref().current.m_enabled_sun_stage;
	// 5 target lines are likely retail-compiled-out source.
	struct half2 {
		math::half x;
		math::half y;
	};

	u32 const jitter_size = 16;
	half2* temp_data = static_cast< half2* >(
		MALLOC( jitter_size * jitter_size * jitter_size * sizeof( half2 ), "" )
	);
	math::random32 random( 100000 );
	for ( u32 z = 0; z < jitter_size; ++z )
		for ( u32 y = 0; y < jitter_size; ++y )
			for ( u32 x = 0; x < jitter_size; ++x ) {
				float angle = random.random_f( SpeedTree::c_fTwoPi );
				half2& value = temp_data[z * jitter_size * jitter_size + y * jitter_size + x];
				value.x = math::half( cos( angle ) );
				// 2 target lines are likely retail-compiled-out source.
				value.y = math::half( sinf( angle ) );
			}

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = temp_data;
	data.SysMemPitch = jitter_size * sizeof( half2 );
	data.SysMemSlicePitch = jitter_size * jitter_size * sizeof( half2 );
	// 8 target lines are likely retail-compiled-out source.
	m_shadow_jitter = resource_manager::ref().create_texture3d( "$user$jitter_lookup", jitter_size, jitter_size, jitter_size, &data, DXGI_FORMAT_R16G16_FLOAT, D3D11_USAGE_IMMUTABLE, 1 );

	FREE( temp_data );

	effect_manager::ref().create_effect< effect_sun >( &m_sun_effect );
}

bool stage_sun::is_effects_ready( ) const
{
	return	m_sun_effect.c_ptr() != NULL;
}

void stage_sun::execute( )
{
	// 2 target lines are likely retail-compiled-out source.
	if ( !is_effects_ready( ) )
		return;
	// 3 target lines are likely retail-compiled-out source.
	light_ptr sun = m_context->scene( )->lights( ).get_sun( );
	if ( !sun || !sun->m_enabled )
		return;

	float3 sun_dir = m_context->get_v( ).transform_direction( sun->direction );

	float3 const* const eye_rays = m_context->get_eye_rays( );
	float3 sun_clr = sun->color;
	sun_dir.normalize( );
	// 5 target lines are likely retail-compiled-out source.
	m_sun_effect->apply( 0, 0 );
	backend::ref( ).set_ps_constant( m_c_light_direction, float4( sun_dir, 0.f ) );
	backend::ref( ).set_ps_constant( m_c_light_color, float4( sun_clr, clear_value ) );
	backend::ref( ).set_ps_constant( m_c_light_intensity, sun->intensity );
	backend::ref( ).set_ps_constant( m_c_diffuse_influence_factor, sun->diffuse_influence_factor );

	backend::ref( ).set_ps_constant( m_c_specular_influence_factor, sun->specular_influence_factor );
	for ( u32 i = 0; i < 4; ++i )
		backend::ref( ).set_ps_constant( m_shadow[i], math::transpose( m_context->get_view2shadow( i ) ) );
	backend::ref( ).set_ps_constant( m_c_clouds_offset, m_simulation.cloud_offset );
	backend::ref( ).set_ps_constant( m_c_world_to_cloud, math::transpose( m_simulation.world_to_cloud ) );
	backend::ref( ).set_ps_constant( m_c_cloud_interp_alpha, m_simulation.interp_alpha );
	backend::ref( ).set_ps_constant( m_c_inverted_view_projection_matrix, math::transpose( math::invert4x4( m_context->get_vp( ) ) ) );

	float4x4 sun_fixed_matrix = math::create_camera_direction( sun->direction * -10000.f, sun->direction, float3( clear_value, 0.f, 0.f ) );

	backend::ref( ).set_ps_constant( m_c_sun_fixed_matrix, math::transpose( math::invert4x4( sun_fixed_matrix ) ) );
	backend::ref( ).set_ps_constant( m_c_shadow_transparency, sun->shadow_transparency );

	backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( (float4*)eye_rays )[0] );
	// 8 target lines are likely retail-compiled-out source.
	system_renderer::ref( ).fill_surface( m_context->get_rt( rt_accumulator_diffuse ), m_context->get_rt( rt_accumulator_specular ), m_context->get_rt( rt_sun_translucensy_help_data ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.f, 0.f, 1.f, 1.f );

	m_context->set_w( float4x4( ).identity( ) );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
