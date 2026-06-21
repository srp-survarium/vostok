////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_lpv_occluder.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32 occluder_ids = 0;

// STATE[STUB]
 object_lpv_occluder::object_lpv_occluder( base_game_scene& w ) :
	game_object_static( w )
{
	// claude@NOTE: body is m_occluder_id = occluder_ids++ (decal-family idiom);
	// occluder_ids static initializer is not encoded in the ctor asm and cannot be
	// recovered from the disassembly, so left STUB rather than guessed.
	// FUNCTION BODY[0x78e940]: 1
	// <0x78e953>|0x013|+0x017:'22'
	// ******
}

// STATE[STUB]
 object_lpv_occluder::~object_lpv_occluder( )
{
	// FUNCTION BODY[0x78e930]: 0
	// <0x78e930>|0x000|+0x006:'26'	{
	// <0x78e936>|0x006|      :'27'	}
	// ******
}

void object_lpv_occluder::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );
	cb( *this );
}

// claude@NOTE: insert/remove call render::scene_renderer::update_lpv_occluder /
// remove_lpv_occluder, which are NOT declared in our render-facade
// scene_renderer.h (they live in vostok/render/facade/sources/scene_renderer.cpp,
// a different unit). Blocked until the render-facade lpv-occluder cook methods are
// added in their own PR; left STUB.

// STATE[STUB]
void object_lpv_occluder::insert( )
{
	// FUNCTION BODY[0x78e970]: 1
	// <0x78e970>|0x000|+0x02d:'37'
	// ******
}

// STATE[STUB]
void object_lpv_occluder::remove( )
{
	// FUNCTION BODY[0x78e9a0]: 1
	// <0x78e9a0>|0x000|+0x026:'42'
	// ******
}

} // namespace survarium
