////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_environment_probe.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

// STATE[STUB]
 object_environment_probe::object_environment_probe( base_game_scene& w ) :
	game_object_static( w )
{
	// FUNCTION BODY[0x78ef40]: 2
	// <0x78ef7d>|0x03d|+0x011:'27'
	// <0x78ef8e>|0x04e|+0x00a:'28'
	// ******
}

// STATE[STUB]
 object_environment_probe::~object_environment_probe( )
{
	// FUNCTION BODY[0x78ef30]: 0
	// <0x78ef30>|0x000|+0x006:'32'	{
	// <0x78ef36>|0x006|      :'33'	}
	// ******
}

void object_environment_probe::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	m_cubemap_resolution	= t["cubemap_resolution"];
	m_radius				= t["radius"];
	m_diffuse_multiplier	= t["diffuse_multiplier"];
	m_specular_multiplier	= t["specular_multiplier"];
	m_enabled				= t["enabled"];
	m_texture_name			= pcstr( t["texture"] );
	m_clip_by_normal		= t["clip_by_normal"];
	m_with_shadows			= t["with_shadows"];

	if ( t.value_exists( "geometry" ) )
		m_geometry = t["geometry"];
	else
		m_geometry = 0;

	cb( *this );
}

// claude@NOTE: insert/remove need render::environment_probe_properties and call
// render::scene_renderer::update_environment_probe / remove_environment_probe,
// none of which are declared in our render-facade headers (they live in the
// vostok/render/facade unit - scene_renderer.cpp). Blocked until that facade cook
// lands in its own PR; left STUB.

// STATE[STUB]
void object_environment_probe::insert( )
{
	// LOCALS
	// render::environment_probe_properties properties
	// ******

	// FUNCTION BODY[0x78efd0]: 17
	// <0>
	// <1>
	// <0x78efe2>|0x012|+0x03d:'56'
	// <0>
	// <0x78f01f>|0x04f|+0x031:'58'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x78f050>|0x080|+0x007:'63'
	// <0x78f057>|0x087|+0x006:'64'
	// <0x78f05d>|0x08d|+0x015:'65'
	// <0x78f072>|0x0a2|+0x00e:'66'
	// <0x78f080>|0x0b0|+0x025:'67'
	// <0>
	// <1>
	// <0x78f0a5>|0x0d5|+0x05e:'70'
	// ******
}

// STATE[STUB]
void object_environment_probe::remove( )
{
	// FUNCTION BODY[0x78efa0]: 1
	// <0x78efa0>|0x000|+0x026:'75'
	// ******
}

} // namespace survarium
