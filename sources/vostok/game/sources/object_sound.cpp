////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_sound.h"

namespace survarium {

// STATE[STUB]
 object_sound::object_sound( base_game_scene& w ) :
	game_object_static( w )
{
	// FUNCTION BODY[0x777e30]: 0
	// <0x777e30>|0x000|+0x027:'18'	{
	// <0x777e57>|0x027|      :'19'	}
	// ******
}

// STATE[STUB]
 object_sound::~object_sound( )
{
	// FUNCTION BODY[0x777dc0]: 0
	// <0x777dc0>|0x000|+0x00a:'22'	{
	// <0x777dca>|0x00a|      :'23'	}
	// ******
}

// STATE[STUB]
void object_sound::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	// LOCALS
	// resources::class_id_enum 		resource_id
	// ******

	// FUNCTION BODY[0x777f20]: 12
	// <0x777f2b>|0x00b|+0x017:'27'
	// <0>
	// <0x777f42>|0x022|+0x014:'29'
	// <0x777f56>|0x036|+0x00e:'30'
	// <0x777f64>|0x044|+0x014:'31'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x777f78>|0x058|+0x0a2:'38'
	// ******
}

// STATE[STUB]
void object_sound::on_sound_resources_ready(
	resources::queries_result&		data,
	boost::function< void( game_object_& ) >&	callback
)
{
	// FUNCTION BODY[0x777e60]: 3
	// <0x777e61>|0x001|+0x0ab:'43'
	// <0>
	// <0x777f0c>|0x0ac|+0x00a:'45'
	// ******
}

// STATE[STUB]
void object_sound::insert( )
{
	// CALL SITE INFO
	// <0x777d0d> -> sound::world_user& < unknown >() const
	// <0x777d59> -> void < unknown >( float3 const&, float3 const& )
	// <0x777d5d> -> sound::world_user& < unknown >() const
	// <0x777da2> -> void < unknown >( float3 const& )
	// <0x777db3> -> void < unknown >( sound::playback_mode, sound::sound_producer const* const, sound::sound_receiver const* const )
	// ******

	// FUNCTION BODY[0x777cd0]: 13
	// <0x777cd8>|0x008|+0x007:'50'
	// <0>
	// <0x777cdf>|0x00f|+0x065:'52'
	// <0x777d44>|0x074|+0x017:'53'
	// <0x777d5b>|0x08b|+0x002:'54'
	// <0>
	// <1>
	// <0x777d5d>|0x08d|+0x037:'57'
	// <0x777d94>|0x0c4|+0x010:'58'
	// <0>
	// <1>
	// <2>
	// <0x777da4>|0x0d4|+0x015:'62'
	// ******
}

// STATE[STUB]
void object_sound::remove( )
{
	// FUNCTION BODY[0x777c90]: 1
	// <0x777c90>|0x000|+0x001:'66'	{
	// <0x777c91>|0x001|+0x02d:'67'
	// <0x777cbe>|0x02e|-0x003:'67'
	// <0x777cbb>|0x02b|+0x005:'68'
	// <0x777cc0>|0x030|      :'68'	}
	// ******
}

} // namespace survarium
