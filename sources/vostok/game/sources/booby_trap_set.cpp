////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "booby_trap_set.h"

namespace survarium {

// STATE[STUB]
booby_trap_set::booby_trap_set( game_world& game_world ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_world( game_world )
{
	// FUNCTION BODY[0x740840]: 0
	// <0x740840>|0x000|+0x0a0:'28'	{
	// <0x7408e0>|0x0a0|      :'29'	}
	// ******
}

// STATE[STUB]
void booby_trap_set::action( bool key_down )
{
	// CALL SITE INFO
	// <0x74078b> -> bool < unknown >() const
	// <0x7407b1> -> base_player* < unknown >()
	// ******

	// FUNCTION BODY[0x740770]: 17
	// <0x740774>|0x004|+0x021:'33'
	// <0>
	// <0x740795>|0x025|+0x004:'35'
	// <0x740799>|0x029|+0x007:'36'
	// <0>
	// <1>
	// <2>
	// <0x7407a0>|0x030|+0x013:'40'
	// <0>
	// <1>
	// <0x7407b3>|0x043|+0x02a:'43'
	// <0>
	// <1>
	// <0x7407dd>|0x06d|+0x00a:'46'
	// <0>
	// <1>
	// <0x7407e7>|0x077|+0x00a:'49'
	// ******
}

// STATE[STUB]
void booby_trap_set::on_trap_placed_message( u8 index, float3 const& position, float3 const& angles )
{
	// LOCALS
	// float4x4 						transform
	// ******

	// CALL SITE INFO
	// <0x7402f6> -> void < unknown >( booby_trap_core&, float4x4 const& )
	// <0x740315> -> base_player* < unknown >()
	// ******

	// FUNCTION BODY[0x740280]: 19
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x740289>|0x009|+0x028:'58'
	// <0>
	// <1>
	// <2>
	// <0x7402b1>|0x031|+0x035:'62'
	// <0>
	// <0x7402e6>|0x066|+0x012:'64'
	// <0>
	// <1>
	// <2>
	// <0x7402f8>|0x078|+0x01f:'68'
	// <0>
	// <1>
	// <0x740317>|0x097|+0x02a:'71'
	// <0x740341>|0x0c1|+0x031:'72'
	// ******
}

// STATE[STUB]
void booby_trap_set::on_trap_removed_message( u8 index )
{
	// CALL SITE INFO
	// <0x74025c> -> void < unknown >( booby_trap_core& )
	// ******

	// FUNCTION BODY[0x740230]: 7
	// <0>
	// <1>
	// <0x740230>|0x000|+0x026:'79'
	// <0>
	// <1>
	// <2>
	// <0x740256>|0x026|+0x008:'83'
	// ******
}

// STATE[STUB]
void booby_trap_set::on_trap_fired_message( u8 index )
{
	// FUNCTION BODY[0x7403e0]: 8
	// <0>
	// <1>
	// <0x7403e0>|0x000|+0x026:'90'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x740406>|0x026|+0x012:'95'
	// ******
}

// STATE[STUB]
void booby_trap_set::on_trap_disarmed_message( u8 index )
{
	// FUNCTION BODY[0x7401e0]: 8
	// <0>
	// <1>
	// <0x7401e0>|0x000|+0x026:'102'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x740206>|0x026|+0x00b:'107'
	// ******
}

// STATE[STUB]
game_material_manager const& booby_trap_set::get_game_material_manager( )
{
	return m_game_world.get_game_material_manager( );	// buildability return (the obvious source)

	// FUNCTION BODY[0x7401d0]: 1
	// <0x7401d0>|0x000|+0x00c:'112'
	// ******
}

// STATE[STUB]
void booby_trap_set::tick( const u32 time_delta_ms, const u32 current_time_ms )
{
	// LOCALS
	// float4x4 						transform
	// bool 							is_placing_allowed
	// ******

	// FUNCTION BODY[0x7404d0]: 11
	// <0x7404d0>|0x000|+0x00c:'116'	{
	// <0>
	// <0x7404dc>|0x00c|+0x00a:'118'
	// <0>
	// <0x7404e6>|0x016|+0x00e:'120'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7404f4>|0x024|+0x00e:'125'
	// <0x740502>|0x032|+0x015:'126'
	// <0x740517>|0x047|-0x02a:'127'
	// <0x7404ed>|0x01d|+0x05b:'128'
	// <0x740548>|0x078|      :'128'	}
	// ******
}

// STATE[STUB]
void booby_trap_set::toggle_ghost_model( bool enable )
{
	// LOCALS
	// float4x4 						transform
	// bool 							is_placing_allowed
	// ******

	// FUNCTION BODY[0x740720]: 10
	// <0x740720>|0x000|+0x009:'132'	{
	// <0x740729>|0x009|+0x009:'133'
	// <0>
	// <1>
	// <0x740732>|0x012|+0x00e:'136'
	// <0x740740>|0x020|+0x018:'137'
	// <0>
	// <1>
	// <2>
	// <0x740758>|0x038|-0x007:'141'
	// <0>
	// <0x740751>|0x031|+0x00c:'143'
	// <0x74075d>|0x03d|      :'143'	}
	// ******
}

// STATE[STUB]
// PDB spells the return vostok::render::static_model_instance_ptr - our render
// tree typedefs it static_model_ptr
render::static_model_ptr booby_trap_set::pick_ghost_model( bool is_placing_allowed )
{
	return render::static_model_ptr( );	// buildability return

	// FUNCTION BODY[0x7403a0]: 3
	// <0>
	// <1>
	// <0x7403a0>|0x000|+0x032:'149'
	// ******
}

// STATE[STUB]
bool booby_trap_set::pick_current_ghost_model( float4x4 const& transform, bool is_placing_allowed )
{
	// LOCALS
	// scheduler& 						scheduler
	// ******

	return false;

	// FUNCTION BODY[0x740550]: 20
	// <0x740550>|0x000|+0x009:'153'	{
	// <0x740559>|0x009|+0x030:'154'
	// <0>
	// <0x740589>|0x039|+0x00e:'156'
	// <0>
	// <0x740597>|0x047|+0x00d:'158'
	// <0x7405a4>|0x054|+0x025:'159'
	// <0>
	// <0x7405c9>|0x079|+0x029:'161'
	// <0>
	// <0x7405f2>|0x0a2|+0x00d:'163'
	// <0>
	// <0x7405ff>|0x0af|+0x013:'165'
	// <0x740612>|0x0c2|+0x07f:'166'
	// <0>
	// <1>
	// <0x740691>|0x141|+0x040:'169'
	// <0x7406d1>|0x181|+0x026:'170'
	// <0>
	// <1>
	// <0x7406f7>|0x1a7|-0x009:'173'
	// <0x7406ee>|0x19e|+0x024:'174'
	// <0x740712>|0x1c2|      :'174'	}
	// ******
}

// STATE[STUB]
void booby_trap_set::remove_current_ghost_model( )
{
	// FUNCTION BODY[0x740440]: 8
	// <0x740440>|0x000|+0x014:'178'
	// <0>
	// <0x740454>|0x014|+0x026:'180'
	// <0x74047a>|0x03a|+0x02b:'181'
	// <0>
	// <1>
	// <0x7404a5>|0x065|+0x01b:'184'
	// <0>
	// ******
}

// STATE[STUB]
void booby_trap_set::holder_assigned( )
{
	// CALL SITE INFO
	// <0x7401b4> -> base_player* < unknown >()
	// ******

	// FUNCTION BODY[0x7401a0]: 3
	// <0x7401a3>|0x003|+0x013:'190'
	// <0>
	// <0x7401b6>|0x016|+0x00f:'192'
	// ******
}

// STATE[STUB]
void booby_trap_set::holder_removed( )
{
	// CALL SITE INFO
	// <0x740184> -> base_player* < unknown >()
	// ******

	// FUNCTION BODY[0x740170]: 3
	// <0x740173>|0x003|+0x013:'197'
	// <0>
	// <0x740186>|0x016|+0x00f:'199'
	// ******
}

// STATE[STUB]
void booby_trap_set::on_player_death( )
{
	// FUNCTION BODY[0x740820]: 1
	// <0x740828>|0x008|+0x007:'204'
	// ******
}

// STATE[STUB]
void booby_trap_set::remove( )
{
	// FUNCTION BODY[0x740800]: 2
	// <0x74080a>|0x00a|+0x005:'209'
	// <0x74080f>|0x00f|+0x007:'210'
	// ******
}


} // namespace survarium
