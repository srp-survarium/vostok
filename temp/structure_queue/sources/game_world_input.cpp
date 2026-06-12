////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\game_world_input.h"

namespace survarium {

// STATE[STUB]
bool game_world::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	// LOCALS
	// game_action_id 					game_action
	// toggle_action_enum 				action_type
	// bool 							btab
	// ******

	// CALL SITE INFO
	// <0x702376> -> input::keyboard const* < unknown >()
	// <0x702380> -> bool < unknown >( input::enum_keyboard ) const
	// ******

	return false;

	// FUNCTION BODY[0x702360]: 52
	// <0x702360>|0x000|+0x00c:'21'	{
	// <0x70236c>|0x00c|+0x01a:'22'
	// <0x702386>|0x026|+0x00d:'23'
	// <0>
	// <0x702393>|0x033|+0x008:'25'
	// <0x70239b>|0x03b|+0x006:'26'
	// <0>
	// <0x7023a1>|0x041|+0x006:'28'
	// <0>
	// <0x7023a7>|0x047|+0x014:'30'
	// <0>
	// <0x7023bb>|0x05b|+0x016:'32'
	// <0x7023d1>|0x071|+0x099:'33'
	// <0>
	// <1>
	// <0x70246a>|0x10a|+0x005:'36'
	// <0>
	// <0x70246f>|0x10f|+0x013:'38'
	// <0x702482>|0x122|+0x00b:'39'
	// <0>
	// <0x70248d>|0x12d|+0x005:'41'
	// <0>
	// <0x702492>|0x132|+0x013:'43'
	// <0x7024a5>|0x145|+0x00b:'44'
	// <0>
	// <0x7024b0>|0x150|+0x005:'46'
	// <0>
	// <0x7024b5>|0x155|+0x00b:'48'
	// <0x7024c0>|0x160|+0x00b:'49'
	// <0>
	// <1>
	// <0x7024cb>|0x16b|+0x007:'52'
	// <0>
	// <1>
	// <0x7024d2>|0x172|-0x04e:'55'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <0x702484>|0x124|+0x023:'74'
	// <0x7024a7>|0x147|+0x01b:'74'
	// <0x7024c2>|0x162|+0x01d:'74'
	// <0x7024df>|0x17f|      :'74'	}
	// ******
}

// STATE[STUB]
bool game_world::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return false;

	// FUNCTION BODY[0x702350]: 2
	// <0>
	// <0x702350>|0x000|+0x002:'79'
	// ******
}

// STATE[STUB]
bool game_world::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	return false;

	// FUNCTION BODY[0x702340]: 2
	// <0>
	// <0x702340>|0x000|+0x002:'85'
	// ******
}

// STATE[STUB]
bool game_world::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return false;

	// FUNCTION BODY[0x702330]: 2
	// <0>
	// <0x702330>|0x000|+0x002:'91'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium
