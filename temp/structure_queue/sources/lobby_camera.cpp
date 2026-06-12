////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\lobby_camera.h"

namespace survarium {

// STATE[STUB]
explicit lobby_camera::lobby_camera( base_game_scene& w )
{
	// FUNCTION BODY[0x5c3e90]: 1
	// <0x5c3eeb>|0x05b|+0x038:'29'
	// ******
}

// STATE[STUB]
bool lobby_camera::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		actions_mask
)
{
	return false;

	// FUNCTION BODY[0x5c3e80]: 2
	// <0>
	// <0x5c3e80>|0x000|+0x002:'35'
	// ******
}

// STATE[STUB]
bool lobby_camera::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		actions_mask
)
{
	return false;

	// FUNCTION BODY[0x5c3e70]: 2
	// <0>
	// <0x5c3e70>|0x000|+0x002:'41'
	// ******
}

// STATE[STUB]
bool lobby_camera::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	actions_mask
)
{
	// CALL SITE INFO
	// <0x5c3e2b> -> bool < unknown >()
	// ******

	return false;

	// FUNCTION BODY[0x5c3e20]: 15
	// <0x5c3e20>|0x000|+0x003:'45'	{
	// <0>
	// <0x5c3e23>|0x003|+0x00c:'47'
	// <0x5c3e2f>|0x00f|+0x002:'48'
	// <0>
	// <0x5c3e31>|0x011|+0x00a:'50'
	// <0>
	// <0x5c3e3b>|0x01b|+0x00d:'52'
	// <0x5c3e48>|0x028|+0x00a:'53'
	// <0x5c3e52>|0x032|+0x005:'54'
	// <0x5c3e57>|0x037|-0x00b:'55'
	// <0>
	// <0x5c3e4c>|0x02c|+0x00f:'57'
	// <0x5c3e5b>|0x03b|+0x006:'57'
	// <0>
	// <1>
	// <0x5c3e61>|0x041|-0x012:'60'
	// <0x5c3e4f>|0x02f|+0x00f:'61'
	// <0x5c3e5e>|0x03e|+0x006:'61'
	// <0x5c3e64>|0x044|      :'61'	}
	// ******
}

// STATE[STUB]
bool lobby_camera::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	// LOCALS
	// float2 							render_window_size
	// ******

	// CALL SITE INFO
	// <0x5c3d4e> -> bool < unknown >()
	// <0x5c3d95> -> float2 < unknown >() const
	// ******

	return false;

	// FUNCTION BODY[0x5c3d40]: 16
	// <0x5c3d40>|0x000|+0x006:'64'	{
	// <0x5c3d46>|0x006|+0x00e:'65'
	// <0x5c3d54>|0x014|+0x01c:'66'
	// <0>
	// <0x5c3d70>|0x030|+0x006:'68'
	// <0x5c3d76>|0x036|+0x009:'69'
	// <0>
	// <1>
	// <2>
	// <0x5c3d7f>|0x03f|+0x018:'73'
	// <0x5c3d97>|0x057|+0x010:'74'
	// <0>
	// <1>
	// <0x5c3da7>|0x067|+0x03b:'77'
	// <0x5c3de2>|0x0a2|+0x033:'78'
	// <0>
	// <0x5c3e15>|0x0d5|-0x09c:'80'
	// <0x5c3d79>|0x039|+0x09f:'81'
	// <0x5c3e18>|0x0d8|      :'81'	}
	// ******
}

// STATE[STUB]
void lobby_camera::on_before_processing( input::world* input_world, const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x5c3d10]: 3
	// <0>
	// <0x5c3d13>|0x003|+0x01e:'86'
	// <0x5c3d31>|0x021|+0x005:'87'
	// ******
}

// STATE[STUB]
void lobby_camera::on_after_processing( input::world* input_world )
{
	// FUNCTION BODY[0x5c3d00]: 1
	// <0x5c3d00>|0x000|+0x000:'91'	{
	// <0>
	// <0x5c3d00>|0x000|      :'93'	}
	// ******
}

// STATE[STUB]
s32 lobby_camera::input_priority( )
{
	return 0;

	// FUNCTION BODY[0x5c3c90]: 1
	// <0x5c3c90>|0x000|+0x005:'97'
	// ******
}

// STATE[STUB]
void lobby_camera::on_activate( camera_director* cd )
{
	// FUNCTION BODY[0x5c3cf0]: 1
	// <0x5c3cf0>|0x000|+0x000:'101'	{
	// <0>
	// <0x5c3cf0>|0x000|      :'103'	}
	// ******
}

// STATE[STUB]
void lobby_camera::on_deactivate( )
{
	// FUNCTION BODY[0x5c3c80]
	// <0x5c3c80>|0x000|      :'106'	{
	// ******
}

// STATE[STUB]
void lobby_camera::tick( )
{
	// LOCALS
	// float3 							ray_direction
	// float4x4 						new_inverted_view
	// ******

	// FUNCTION BODY[0x5c3fc0]: 23
	// <0>
	// <0x5c3fcf>|0x00f|+0x00f:'111'
	// <0x5c3fde>|0x01e|+0x00f:'112'
	// <0>
	// <0x5c3fed>|0x02d|+0x029:'114'
	// <0>
	// <0x5c4016>|0x056|+0x009:'116'
	// <0x5c401f>|0x05f|+0x02a:'117'
	// <0>
	// <1>
	// <0x5c4049>|0x089|+0x022:'120'
	// <0>
	// <0x5c406b>|0x0ab|+0x00e:'122'
	// <0x5c4079>|0x0b9|+0x033:'123'
	// <0x5c40ac>|0x0ec|+0x061:'124'
	// <0>
	// <0x5c410d>|0x14d|+0x00c:'126'
	// <0x5c4119>|0x159|+0x062:'127'
	// <0x5c417b>|0x1bb|+0x019:'128'
	// <0>
	// <0x5c4194>|0x1d4|+0x052:'130'
	// <0x5c41e6>|0x226|+0x01e:'131'
	// <0>
	// ******
}

// STATE[STUB]
void lobby_camera::process_collision(
	float3 const&		target_point,
	float3 const&		direction,
	float&				distance_to_focus_point
)
{
	// LOCALS
	// physics::closest_ray_result 		result
	// ******

	// CALL SITE INFO
	// <0x5c3f56> -> physics::closest_ray_result < unknown >( float3 const&, float3 const&, const float, u16, u16 )
	// ******

	// FUNCTION BODY[0x5c3f30]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c3f30>|0x000|+0x028:'141'
	// <0>
	// <0x5c3f58>|0x028|+0x006:'143'
	// <0x5c3f5e>|0x02e|+0x04d:'144'
	// ******
}

// STATE[STUB]
void lobby_camera::on_focus( bool b_focus_enter )
{
	// CALL SITE INFO
	// <0x5c3cb8> -> input::world& < unknown >()
	// <0x5c3cc5> -> void < unknown >( input::handler& )
	// <0x5c3cd9> -> input::world& < unknown >()
	// <0x5c3ce6> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5c3ca0]: 6
	// <0x5c3ca0>|0x000|+0x000:'148'	{
	// <0>
	// <1>
	// <0x5c3ca0>|0x000|+0x00a:'151'
	// <0x5c3caa>|0x00a|+0x012:'152'
	// <0>
	// <0x5c3cbc>|0x01c|+0x00f:'154'
	// <0x5c3ccb>|0x02b|-0x003:'154'
	// <0x5c3cc8>|0x028|+0x021:'155'
	// <0x5c3ce9>|0x049|      :'155'	}
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
