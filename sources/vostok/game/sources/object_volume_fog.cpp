// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "object_volume_fog.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/volume_fog_parameters.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32	volume_fog_ids	= 0;
static float const	clear_value		= 1.f;

object_volume_fog::object_volume_fog( base_game_scene& w ) :
	game_object_static( w )
{
	m_volume_fog_id				= volume_fog_ids++;
	m_color						= float3( clear_value, clear_value, clear_value );
	m_density					= clear_value;
	m_speed						= clear_value;
	m_direction					= float2( clear_value, 0.f );
	m_noise_scale				= clear_value;
	m_wave_length				= clear_value;
	m_near_density				= 0.f;
	m_transparency_multiplier	= clear_value;
	m_density_offset			= 0.f;
	m_height_falloff_offset		= 0.f;
}

object_volume_fog::~object_volume_fog( )
{
}

void object_volume_fog::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	if ( t.value_exists( "color" ) )
		m_color = t["color"];

	if ( t.value_exists( "density" ) )
		m_density = t["density"];

	if ( t.value_exists( "speed" ) )
		m_speed = t["speed"];

	if ( t.value_exists( "noise_scale" ) )
		m_noise_scale = t["noise_scale"];

	if ( t.value_exists( "wave_length" ) )
		m_wave_length = t["wave_length"];

	if ( t.value_exists( "direction" ) )
		m_direction = t["direction"];

	if ( t.value_exists( "near_density" ) )
		m_near_density = t["near_density"];

	if ( t.value_exists( "transparency_multiplier" ) )
		m_transparency_multiplier = t["transparency_multiplier"];

	if ( t.value_exists( "density_offset" ) )
		m_density_offset = t["density_offset"];

	if ( t.value_exists( "height_falloff_offset" ) )
		m_height_falloff_offset = t["height_falloff_offset"];

	cb( *this );
}

void object_volume_fog::insert( )
{
	render::volume_fog_parameters	v;

	v.transform					= m_transform;
	v.fog_color					= m_color;
	v.density					= m_density;
	v.speed						= m_speed;
	v.direction					= math::normalize_safe( m_direction, float2( 0.0f, 0.0f ) );
	v.noise_scale				= m_noise_scale;

	v.wave_length				= m_wave_length;
	v.near_density				= m_near_density;
	v.transparency_multiplier	= m_transparency_multiplier;
	v.density_offset			= m_density_offset;
	v.height_falloff_offset		= m_height_falloff_offset;

	get_game_scene().renderer().scene().update_volume_fog(
		get_game_scene().render_scene(),
		m_volume_fog_id,
		v
	);
}

void object_volume_fog::remove( )
{
	get_game_scene().renderer().scene().remove_volume_fog( get_game_scene().render_scene(), m_volume_fog_id );
}

} // namespace survarium
