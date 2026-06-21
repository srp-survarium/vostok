////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_ambient_volume.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

// claude@NOTE: clear_value file-static float; init value is not encoded in the
// load asm (referenced only by address) and cannot be recovered from the
// disassembly. Guessed 0.0f - does not affect load's codegen, only the unmatched
// data initializer.
static float clear_value = 0.0f;

// STATE[STUB]
 object_ambient_volume::object_ambient_volume( base_game_scene& w ) :
	game_object_static( w )
{
	// FUNCTION BODY[0x78ea70]: 1
	// <0x78ea83>|0x013|+0x01e:'25'
	// ******
}

// STATE[STUB]
 object_ambient_volume::~object_ambient_volume( )
{
	// FUNCTION BODY[0x78ea60]: 0
	// <0x78ea60>|0x000|+0x006:'29'	{
	// <0x78ea66>|0x006|      :'30'	}
	// ******
}

void object_ambient_volume::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	m_enabled				= t["enabled"];
	m_ambient_multiplier	= t["ambient_multiplier"];

	if ( !m_enabled || m_ambient_multiplier != clear_value )
		m_valid = false;

	cb( *this );
}

// claude@NOTE: insert/remove need render::ambient_volume_properties and call
// render::scene_renderer::update_ambient_volume / remove_ambient_volume, none of
// which are declared in our render-facade headers (the type + the cook methods
// live in the vostok/render/facade unit - vostok/render/facade/sources/
// scene_renderer.cpp). Blocked until the render-facade ambient-volume cook lands
// in its own PR; left STUB.

// STATE[STUB]
void object_ambient_volume::insert( )
{
	// LOCALS
	// render::ambient_volume_properties properties
	// ******

	// FUNCTION BODY[0x78eab0]: 9
	// <0x78eabb>|0x00b|+0x00b:'47'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x78eac6>|0x016|+0x019:'52'
	// <0x78eadf>|0x02f|+0x00a:'53'
	// <0>
	// <0x78eae9>|0x039|+0x031:'55'
	// ******
}

// STATE[STUB]
void object_ambient_volume::remove( )
{
	// FUNCTION BODY[0x78eb20]: 4
	// <0x78eb20>|0x000|+0x009:'60'
	// <0>
	// <1>
	// <0x78eb29>|0x009|+0x026:'63'
	// ******
}

} // namespace survarium
