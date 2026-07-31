#include "pch.h"
#include "convex_volume.h"
#include "stage_sun.h"

#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include "effect_sun.h"

namespace vostok {
namespace render {

stage_sun::stage_sun(
	renderer*				in_renderer,
	renderer_context*		in_context,
	cloud_interp_textures&	cloud_interp_textures,
	cloud_simulation&		simulation
) :
	stage						( in_renderer, in_context ),
	m_cloud_interp_textures		( cloud_interp_textures ),
	m_simulation				( simulation )
{
	// FUNCTION BODY[0x626a70]
	// claude@NOTE: adapted from legacy - the 5 effect_light_direct<N>/light_mask effects collapsed
	// into the single m_sun_effect (effect_sun); legacy m_c_lighting_model constant dropped with its
	// member; the new cloud/sun-matrix constant hosts have no legacy ancestor and stay unregistered
	effect_manager::ref().create_effect<effect_sun>(&m_sun_effect);

	m_c_light_color					= backend::ref().register_constant_host( "light_color", rc_float );
	m_c_light_direction				= backend::ref().register_constant_host( "light_direction", rc_float );
	m_c_light_intensity				= backend::ref().register_constant_host( "light_intensity", rc_float );
	m_c_shadow_transparency			= backend::ref().register_constant_host( "shadow_transparency",  rc_float );
	m_c_diffuse_influence_factor	= backend::ref().register_constant_host( "light_diffuse_influence_factor", rc_float );
	m_c_specular_influence_factor	= backend::ref().register_constant_host( "light_specular_influence_factor", rc_float );

	m_shadow[0]					= backend::ref().register_constant_host( "m_shadow0",  rc_float );
	m_shadow[1]					= backend::ref().register_constant_host( "m_shadow1", rc_float );
	m_shadow[2]					= backend::ref().register_constant_host( "m_shadow2", rc_float );
	m_shadow[3]					= backend::ref().register_constant_host( "m_shadow3", rc_float );


	m_c_environment_skylight_upper_color = backend::ref().register_constant_host( "environment_skylight_upper_color", rc_float );
	m_c_environment_skylight_lower_color = backend::ref().register_constant_host( "environment_skylight_lower_color", rc_float );

	m_enabled					= options::ref().current.m_enabled_sun_stage;

//	m_sunmask					= backend::ref().register_constant_host( "sunmask", rc_float );
}

bool stage_sun::is_effects_ready( ) const
{
	// FUNCTION BODY[0x626370]
	return	m_sun_effect.c_ptr() != NULL;
}

void stage_sun::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x626380]
	// claude@NOTE: legacy execute delegated to execute_cascade/phase_sun_mask which the canonical
	// class dropped; shipped execute is rewritten around m_sun_effect + cloud shadows - matcher-phase
}

} // namespace render
} // namespace vostok
