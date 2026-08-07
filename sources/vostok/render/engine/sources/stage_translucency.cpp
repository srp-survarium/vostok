#include "pch.h"
#include "stage_translucency.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>

#include "effect_translucency.h"
#include "light.h"
#include "lights_db.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

stage_translucency::stage_translucency(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context )
{
	effect_manager::ref( ).create_effect<effect_translucency>( &m_translucency_effect );

	m_shadow[0] = backend::ref( ).register_constant_host( "m_shadow0", rc_float );
	m_shadow[1] = backend::ref( ).register_constant_host( "m_shadow1", rc_float );
	m_shadow[2] = backend::ref( ).register_constant_host( "m_shadow2", rc_float );
	m_shadow[3] = backend::ref( ).register_constant_host( "m_shadow3", rc_float );
	m_c_sun_direction = backend::ref( ).register_constant_host( "sun_direction", rc_float );
	m_c_sun_color = backend::ref( ).register_constant_host( "sun_color", rc_float );
	m_c_eye_ray_corner = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
}

bool stage_translucency::is_effects_ready( ) const
{
	return m_translucency_effect.c_ptr( ) != 0;
}

void stage_translucency::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	light_ptr sun = m_context->scene( )->lights( ).get_sun( );

	if ( !is_enabled( ) || !sun )
	{
		execute_disabled( );
		return;
	}

	m_translucency_effect->apply( 0, 0 );

	for ( u32 i = 0; i < 4; ++i )
		backend::ref( ).set_ps_constant( m_shadow[i], math::transpose( m_context->get_view2shadow( i ) ) );

	backend::ref( ).set_ps_constant( m_c_sun_direction, m_context->get_v( ).transform_direction( sun->direction ) );
	backend::ref( ).set_ps_constant( m_c_sun_color, math::pow( sun->color, 3.0f ) * sun->intensity );

	float3 const* const eye_rays = m_context->get_eye_rays( );
	backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( (float4*)eye_rays )[0] );

	system_renderer::ref( ).fill_surface( m_context->get_rt( rt_accumulator_diffuse ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );

	backend::ref( ).reset_render_targets( );
}

} // namespace render
} // namespace vostok
