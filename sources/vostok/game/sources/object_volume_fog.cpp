////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_volume_fog.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

// STATE[STUB]
 object_volume_fog::object_volume_fog( base_game_scene& w ) :
	game_object_static( w )
{
	// FUNCTION BODY[0x78e6d0]: 11
	// <0>
	// <0x78e6dc>|0x00c|+0x038:'25'
	// <0>
	// <1>
	// <0x78e714>|0x044|+0x04e:'28'
	// <0x78e762>|0x092|+0x008:'29'
	// <0x78e76a>|0x09a|+0x008:'30'
	// <0x78e772>|0x0a2|+0x008:'31'
	// <0x78e77a>|0x0aa|+0x008:'32'
	// <0x78e782>|0x0b2|+0x008:'33'
	// <0x78e78a>|0x0ba|+0x008:'34'
	// ******
}

// STATE[STUB]
 object_volume_fog::~object_volume_fog( )
{
	// FUNCTION BODY[0x78e6c0]: 0
	// <0x78e6c0>|0x000|+0x006:'38'	{
	// <0x78e6c6>|0x006|      :'39'	}
	// ******
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

// claude@NOTE: insert/remove need render::volume_fog_parameters and call
// render::scene_renderer::update_volume_fog / remove_volume_fog, none of which
// are declared in our render-facade headers (they live in the vostok/render/facade
// unit - scene_renderer.cpp). Blocked until that facade cook lands in its own PR;
// left STUB.

// STATE[STUB]
void object_volume_fog::insert( )
{
	// LOCALS
	// render::volume_fog_parameters 	v
	// ******

	// FUNCTION BODY[0x78e7a0]: 14
	// <0x78e7af>|0x00f|+0x00c:'85'
	// <0>
	// <0x78e7bb>|0x01b|+0x025:'87'
	// <0x78e7e0>|0x040|+0x00e:'88'
	// <0x78e7ee>|0x04e|+0x008:'89'
	// <0x78e7f6>|0x056|+0x035:'90'
	// <0x78e82b>|0x08b|+0x008:'91'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x78e833>|0x093|+0x08e:'98'
	// ******
}

// STATE[STUB]
void object_volume_fog::remove( )
{
	// FUNCTION BODY[0x78e8d0]: 1
	// <0x78e8d0>|0x000|+0x026:'103'
	// ******
}

} // namespace survarium
