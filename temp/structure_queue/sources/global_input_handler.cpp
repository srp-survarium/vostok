////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\global_input_handler.h"

namespace survarium {

// STATE[STUB]
explicit global_input_handler::global_input_handler( game& game )
{
	// FUNCTION BODY[0x705590]: 0
	// <0x705590>|0x000|+0x00f:'21'	{
	// <0x70559f>|0x00f|      :'22'	}
	// ******
}

// STATE[STUB]
 global_input_handler::~global_input_handler( )
{
	// FUNCTION BODY[0x705580]: 0
	// <0x705580>|0x000|+0x006:'25'	{
	// <0x705586>|0x006|      :'26'	}
	// ******
}

// STATE[STUB]
bool global_input_handler::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	// CALL SITE INFO
	// <0x70550a> -> input::keyboard const* < unknown >()
	// <0x705514> -> bool < unknown >( input::enum_keyboard ) const
	// <0x705521> -> input::keyboard const* < unknown >()
	// <0x70552e> -> bool < unknown >( input::enum_keyboard ) const
	// ******

	return false;

	// FUNCTION BODY[0x7054e0]: 23
	// <0x7054e0>|0x000|+0x000:'33'	{
	// <0x7054e0>|0x000|+0x00a:'34'
	// <0x7054ea>|0x00a|+0x006:'35'
	// <0>
	// <0x7054f0>|0x010|+0x00f:'37'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7054ff>|0x01f|+0x03c:'42'
	// <0>
	// <0x70553b>|0x05b|+0x014:'44'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x70554f>|0x06f|+0x022:'51'
	// <0x705571>|0x091|-0x028:'51'
	// <0x705549>|0x069|+0x022:'52'
	// <0x70556b>|0x08b|+0x00c:'52'
	// <0x705577>|0x097|-0x042:'52'
	// <0>
	// <1>
	// <2>
	// <0x705535>|0x055|-0x048:'56'
	// <0x7054ed>|0x00d|+0x04b:'57'
	// <0x705538>|0x058|+0x014:'57'
	// <0x70554c>|0x06c|+0x022:'57'
	// <0x70556e>|0x08e|+0x00c:'57'
	// <0x70557a>|0x09a|      :'57'	}
	// ******
}

// STATE[STUB]
bool global_input_handler::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return false;

	// FUNCTION BODY[0x7054d0]: 2
	// <0>
	// <0x7054d0>|0x000|+0x002:'62'
	// ******
}

// STATE[STUB]
bool global_input_handler::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	return false;

	// FUNCTION BODY[0x7054c0]: 2
	// <0>
	// <0x7054c0>|0x000|+0x002:'68'
	// ******
}

// STATE[STUB]
bool global_input_handler::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return false;

	// FUNCTION BODY[0x7054b0]: 2
	// <0>
	// <0x7054b0>|0x000|+0x002:'74'
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
