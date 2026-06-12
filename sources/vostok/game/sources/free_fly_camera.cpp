////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "free_fly_camera.h"

namespace survarium {

// STATE[STUB]
 free_fly_camera::free_fly_camera( base_game_scene& w, camera_director& cd ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	game_camera( w ),
	m_camera_director( cd )
{
	// FUNCTION BODY[0x5cf320]: 0
	// <0x5cf320>|0x000|+0x07a:'33'	{
	// <0x5cf39a>|0x07a|      :'34'	}
	// ******
}

// STATE[STUB]
bool free_fly_camera::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	// LOCALS
	// toggle_action_enum 				actions_mask_type
	// ******

	return false;

	// FUNCTION BODY[0x5cf880]: 31
	// <0x5cf880>|0x000|+0x00c:'37'	{
	// <0>
	// <1>
	// <0x5cf88c>|0x00c|+0x00a:'40'
	// <0>
	// <0x5cf896>|0x016|+0x01a:'42'
	// <0>
	// <0x5cf8b0>|0x030|+0x007:'44'
	// <0>
	// <1>
	// <0x5cf8b7>|0x037|+0x030:'47'
	// <0>
	// <1>
	// <0x5cf8e7>|0x067|-0x011:'50'
	// <0>
	// <1>
	// <2>
	// <0x5cf8d6>|0x056|+0x00f:'54'
	// <0x5cf8e5>|0x065|-0x020:'55'
	// <0>
	// <1>
	// <0x5cf8c5>|0x045|+0x00f:'58'
	// <0x5cf8d4>|0x054|+0x01c:'59'
	// <0>
	// <1>
	// <2>
	// <0x5cf8f0>|0x070|+0x007:'63'
	// <0x5cf8f7>|0x077|+0x020:'64'
	// <0x5cf917>|0x097|+0x015:'64'
	// <0x5cf92c>|0x0ac|-0x010:'65'
	// <0x5cf91c>|0x09c|+0x01c:'66'
	// <0x5cf938>|0x0b8|-0x02c:'66'
	// <0>
	// <0x5cf90c>|0x08c|+0x015:'68'
	// <0x5cf921>|0x0a1|-0x013:'68'
	// <0x5cf90e>|0x08e|+0x015:'69'
	// <0x5cf923>|0x0a3|+0x02e:'69'
	// <0x5cf951>|0x0d1|      :'69'	}
	// ******
}

// STATE[STUB]
bool free_fly_camera::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	// CALL SITE INFO
	// <0x5ceee9> -> input::gamepad* < unknown >()
	// <0x5ceef4> -> input::gamepad* < unknown >()
	// <0x5cef06> -> float < unknown >( input::gamepad_vibrators ) const
	// <0x5cef18> -> void < unknown >( input::gamepad_vibrators, float )
	// <0x5cef34> -> input::gamepad* < unknown >()
	// <0x5cef3f> -> input::gamepad* < unknown >()
	// <0x5cef51> -> float < unknown >( input::gamepad_vibrators ) const
	// <0x5cef63> -> void < unknown >( input::gamepad_vibrators, float )
	// ******

	return false;

	// FUNCTION BODY[0x5ceed0]: 14
	// <0x5ceed0>|0x000|+0x000:'72'	{
	// <0>
	// <1>
	// <0x5ceed0>|0x000|+0x00e:'75'
	// <0>
	// <0x5ceede>|0x00e|+0x03e:'77'
	// <0x5cef1c>|0x04c|+0x006:'78'
	// <0>
	// <1>
	// <0x5cef22>|0x052|+0x007:'81'
	// <0>
	// <0x5cef29>|0x059|+0x03e:'83'
	// <0x5cef67>|0x097|+0x008:'84'
	// <0>
	// <0x5cef6f>|0x09f|-0x050:'86'
	// <0x5cef1f>|0x04f|+0x04b:'87'
	// <0x5cef6a>|0x09a|+0x008:'87'
	// <0x5cef72>|0x0a2|      :'87'	}
	// ******
}

// STATE[STUB]
bool free_fly_camera::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	return false;

	// FUNCTION BODY[0x5cf2e0]: 6
	// <0x5cf2e0>|0x000|+0x000:'90'	{
	// <0>
	// <1>
	// <0x5cf2e0>|0x000|+0x007:'93'
	// <0x5cf2e7>|0x007|+0x020:'94'
	// <0x5cf307>|0x027|-0x005:'94'
	// <0>
	// <0x5cf302>|0x022|+0x010:'96'
	// <0x5cf312>|0x032|-0x00e:'96'
	// <0x5cf304>|0x024|+0x010:'97'
	// <0x5cf314>|0x034|      :'97'	}
	// ******
}

// STATE[STUB]
bool free_fly_camera::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return false;

	// FUNCTION BODY[0x5cefe0]: 7
	// <0>
	// <1>
	// <0x5cefe0>|0x000|+0x028:'103'
	// <0x5cf008>|0x028|+0x018:'104'
	// <0x5cf020>|0x040|+0x018:'105'
	// <0>
	// <0x5cf038>|0x058|+0x002:'107'
	// ******
}

// STATE[STUB]
bool free_fly_camera::keyb_event_present( s32 e )
{
	return false;

	// FUNCTION BODY[0x5ceeb0]: 1
	// <0x5ceeb1>|0x001|+0x01b:'112'
	// ******
}

// STATE[STUB]
bool free_fly_camera::mouse_event_present( s32 e )
{
	return false;

	// FUNCTION BODY[0x5cee90]: 1
	// <0x5cee91>|0x001|+0x01b:'117'
	// ******
}

// STATE[STUB]
void free_fly_camera::build_view_matrix(
	float2 const&		raw_angles,
	const float			shift_forward,
	const float			shift_right,
	const float			shift_up
)
{
	// LOCALS
	// float4x4 						translation
	// float3 							angles_zxy
	// float4x4 						rotation
	// float4x4 						view_inverted
	// float3 							new_angles_zxy
	// float3 							position
	// ******

	// FUNCTION BODY[0x5cf120]: 15
	// <0x5cf137>|0x017|+0x012:'122'
	// <0x5cf149>|0x029|+0x00d:'123'
	// <0>
	// <0x5cf156>|0x036|+0x02f:'125'
	// <0x5cf185>|0x065|+0x01d:'126'
	// <0>
	// <0x5cf1a2>|0x082|+0x013:'128'
	// <0>
	// <1>
	// <2>
	// <0x5cf1b5>|0x095|+0x0ab:'132'
	// <0>
	// <0x5cf260>|0x140|+0x03b:'134'
	// <0>
	// <0x5cf29b>|0x17b|+0x029:'136'
	// ******
}

// STATE[STUB]
void free_fly_camera::on_focus( bool b_focus_enter )
{
	// CALL SITE INFO
	// <0x5cefa1> -> input::world& < unknown >()
	// <0x5cefab> -> void < unknown >( input::handler& )
	// <0x5cefca> -> input::world& < unknown >()
	// <0x5cefd4> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5cef80]: 6
	// <0x5cef80>|0x000|+0x000:'140'	{
	// <0>
	// <1>
	// <0x5cef80>|0x000|+0x008:'143'
	// <0x5cef88>|0x008|+0x022:'144'
	// <0>
	// <0x5cefaa>|0x02a|+0x007:'146'
	// <0x5cefb1>|0x031|-0x003:'146'
	// <0x5cefae>|0x02e|+0x029:'147'
	// <0x5cefd7>|0x057|      :'147'	}
	// ******
}

// STATE[STUB]
void free_fly_camera::on_activate( camera_director* cd )
{
	// FUNCTION BODY[0x5cf040]: 7
	// <0>
	// <1>
	// <0x5cf043>|0x003|+0x008:'153'
	// <0>
	// <1>
	// <0x5cf04b>|0x00b|+0x03c:'156'
	// <0x5cf087>|0x047|+0x085:'157'
	// ******
}

// STATE[STUB]
void free_fly_camera::tick( )
{
	// LOCALS
	// float 							angle_factor
	// const float 						current_time_delta
	// float 							right
	// float 							forward
	// float 							factor
	// float 							up
	// ******

	// STATICS
	// static u32 						counter = <0x4c26614>;
	// ******

	// FUNCTION BODY[0x5cf3a0]: 87
	// <0>
	// <0x5cf3b1>|0x011|+0x013:'163'
	// <0>
	// <1>
	// <0x5cf3c4>|0x024|+0x017:'166'
	// <0>
	// <0x5cf3db>|0x03b|+0x011:'168'
	// <0x5cf3ec>|0x04c|+0x006:'169'
	// <0x5cf3f2>|0x052|+0x002:'170'
	// <0x5cf3f4>|0x054|+0x01a:'171'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5cf40e>|0x06e|+0x043:'179'
	// <0x5cf451>|0x0b1|+0x0e0:'180'
	// <0>
	// <1>
	// <0x5cf531>|0x191|+0x081:'183'
	// <0>
	// <1>
	// <0x5cf5b2>|0x212|+0x035:'186'
	// <0x5cf5e7>|0x247|+0x014:'187'
	// <0>
	// <0x5cf5fb>|0x25b|+0x032:'189'
	// <0x5cf62d>|0x28d|+0x014:'190'
	// <0>
	// <0x5cf641>|0x2a1|+0x032:'192'
	// <0x5cf673>|0x2d3|+0x00e:'193'
	// <0>
	// <0x5cf681>|0x2e1|+0x003:'195'
	// <0>
	// <1>
	// <2>
	// <0x5cf684>|0x2e4|+0x02d:'199'
	// <0x5cf6b1>|0x311|+0x014:'200'
	// <0>
	// <0x5cf6c5>|0x325|+0x018:'202'
	// <0x5cf6dd>|0x33d|+0x01e:'203'
	// <0>
	// <0x5cf6fb>|0x35b|+0x01a:'205'
	// <0x5cf715>|0x375|+0x014:'206'
	// <0>
	// <0x5cf729>|0x389|+0x018:'208'
	// <0x5cf741>|0x3a1|+0x01e:'209'
	// <0>
	// <0x5cf75f>|0x3bf|+0x018:'211'
	// <0x5cf777>|0x3d7|+0x014:'212'
	// <0>
	// <0x5cf78b>|0x3eb|+0x018:'214'
	// <0x5cf7a3>|0x403|+0x01e:'215'
	// <0>
	// <1>
	// <2>
	// <0x5cf7c1>|0x421|+0x053:'219'
	// <0>
	// <0x5cf814>|0x474|+0x020:'221'
	// <0x5cf834>|0x494|+0x020:'222'
	// <0>
	// <0x5cf854>|0x4b4|+0x008:'224'
	// <0x5cf85c>|0x4bc|+0x008:'225'
	// <0x5cf864>|0x4c4|+0x008:'226'
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
	// <18>
	// <19>
	// <20>
	// <21>
	// ******
}

} // namespace survarium
