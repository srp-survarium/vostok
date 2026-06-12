////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\player_input_handler.h"

namespace survarium {

// STATE[STUB]
explicit player_input_handler::player_input_handler( game_world& world )
{
	// FUNCTION BODY[0x5cfca0]: 0
	// <0x5cfca0>|0x000|+0x0b2:'32'	{
	// <0x5cfd52>|0x0b2|      :'33'	}
	// ******
}

// STATE[STUB]
bool player_input_handler::on_keyboard_action(
	input::world* const				input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		actions_mask
)
{
	// LOCALS
	// toggle_action_enum 				actions_mask_type
	// ******

	return false;

	// FUNCTION BODY[0x5d0430]: 37
	// <0x5d0430>|0x000|+0x00c:'40'	{
	// <0>
	// <1>
	// <0x5d043c>|0x00c|+0x00d:'43'
	// <0>
	// <1>
	// <0x5d0449>|0x019|+0x01d:'46'
	// <0>
	// <0x5d0466>|0x036|+0x003:'48'
	// <0x5d0469>|0x039|+0x006:'49'
	// <0>
	// <0x5d046f>|0x03f|+0x008:'51'
	// <0>
	// <1>
	// <0x5d0477>|0x047|+0x033:'54'
	// <0>
	// <1>
	// <0x5d04aa>|0x07a|-0x011:'57'
	// <0>
	// <1>
	// <2>
	// <0x5d0499>|0x069|+0x00f:'61'
	// <0x5d04a8>|0x078|-0x020:'62'
	// <0>
	// <1>
	// <0x5d0488>|0x058|+0x00f:'65'
	// <0x5d0497>|0x067|+0x01c:'66'
	// <0>
	// <1>
	// <2>
	// <0x5d04b3>|0x083|+0x00d:'70'
	// <0x5d04c0>|0x090|+0x01f:'71'
	// <0x5d04df>|0x0af|+0x00c:'72'
	// <0x5d04eb>|0x0bb|+0x01f:'73'
	// <0x5d050a>|0x0da|-0x03c:'74'
	// <0x5d04ce>|0x09e|+0x02b:'75'
	// <0x5d04f9>|0x0c9|+0x01d:'75'
	// <0x5d0516>|0x0e6|-0x042:'75'
	// <0>
	// <0x5d04d4>|0x0a4|+0x02b:'77'
	// <0x5d04ff>|0x0cf|-0x029:'77'
	// <0x5d04d6>|0x0a6|+0x02b:'78'
	// <0x5d0501>|0x0d1|+0x029:'78'
	// <0x5d052a>|0x0fa|      :'78'	}
	// ******
}

// STATE[STUB]
bool player_input_handler::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		actions_mask
)
{
	return false;

	// FUNCTION BODY[0x5cfc90]: 2
	// <0>
	// <0x5cfc90>|0x000|+0x002:'83'
	// ******
}

// STATE[STUB]
bool player_input_handler::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	actions_mask
)
{
	// LOCALS
	// toggle_action_enum 				actions_mask_type
	// ******

	return false;

	// FUNCTION BODY[0x5cfc00]: 17
	// <0x5cfc00>|0x000|+0x002:'87'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cfc02>|0x002|+0x025:'92'
	// <0>
	// <0x5cfc27>|0x027|+0x003:'94'
	// <0x5cfc2a>|0x02a|+0x002:'95'
	// <0>
	// <0x5cfc2c>|0x02c|+0x014:'97'
	// <0x5cfc40>|0x040|+0x01c:'98'
	// <0x5cfc5c>|0x05c|-0x00d:'99'
	// <0x5cfc4f>|0x04f|+0x019:'100'
	// <0x5cfc68>|0x068|+0x014:'100'
	// <0>
	// <0x5cfc7c>|0x07c|-0x026:'102'
	// <0>
	// <0x5cfc56>|0x056|+0x028:'104'
	// <0x5cfc7e>|0x07e|-0x025:'104'
	// <0x5cfc59>|0x059|+0x028:'105'
	// <0x5cfc81>|0x081|      :'105'	}
	// ******
}

// STATE[STUB]
bool player_input_handler::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	// LOCALS
	// const float 						horizontal_sensitivity
	// ******

	// CALL SITE INFO
	// <0x5cfb2e> -> float2 < unknown >() const
	// <0x5cfb3f> -> float2 < unknown >() const
	// ******

	return false;

	// FUNCTION BODY[0x5cfaf0]: 12
	// <0>
	// <1>
	// <2>
	// <0x5cfaf8>|0x008|+0x049:'112'
	// <0>
	// <0x5cfb41>|0x051|+0x02a:'114'
	// <0>
	// <0x5cfb6b>|0x07b|+0x036:'116'
	// <0x5cfba1>|0x0b1|+0x00a:'117'
	// <0x5cfbab>|0x0bb|+0x040:'118'
	// <0>
	// <0x5cfbeb>|0x0fb|+0x003:'120'
	// ******
}

// STATE[STUB]
void player_input_handler::on_before_processing( input::world* const input_world, const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x5cfa90]: 10
	// <0>
	// <0x5cfa93>|0x003|+0x003:'126'
	// <0>
	// <0x5cfa96>|0x006|+0x01e:'128'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cfab4>|0x024|+0x02a:'133'
	// <0x5cfade>|0x04e|+0x006:'134'
	// ******
}

// STATE[STUB]
void player_input_handler::on_after_processing( input::world* const input_world )
{
	// LOCALS
	// const float 						time_delta
	// ******

	// FUNCTION BODY[0x5d0620]: 11
	// <0x5d0620>|0x000|+0x008:'138'	{
	// <0>
	// <1>
	// <0x5d0628>|0x008|+0x01a:'141'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d0642>|0x022|+0x0a4:'146'
	// <0x5d06e6>|0x0c6|+0x010:'147'
	// <0>
	// <0x5d06f6>|0x0d6|-0x007:'149'
	// <0x5d06ef>|0x0cf|+0x00c:'150'
	// <0x5d06fb>|0x0db|      :'150'	}
	// ******
}

// STATE[STUB]
bool player_input_handler::alt_is_held( ) const
{
	// CALL SITE INFO
	// <0x5cfa52> -> input::world& < unknown >()
	// <0x5cfa5b> -> input::keyboard const* < unknown >()
	// <0x5cfa6a> -> bool < unknown >( input::enum_keyboard ) const
	// <0x5cfa78> -> bool < unknown >( input::enum_keyboard ) const
	// ******

	return false;

	// FUNCTION BODY[0x5cfa40]: 2
	// <0x5cfa40>|0x000|+0x000:'153'	{
	// <0x5cfa40>|0x000|+0x01f:'154'
	// <0x5cfa5f>|0x01f|+0x023:'155'
	// <0x5cfa82>|0x042|-0x001:'155'
	// <0x5cfa81>|0x041|+0x007:'156'
	// <0x5cfa88>|0x048|      :'156'	}
	// ******
}

// STATE[STUB]
void player_input_handler::process_first_person_mode( const bool use_mouse_move )
{
	// LOCALS
	// const float 						horizontal_sensitivity
	// float 							vertical_sensitivity
	// ******

	// CALL SITE INFO
	// <0x5cffe3> -> float2 < unknown >() const
	// <0x5cfff4> -> float2 < unknown >() const
	// ******

	// FUNCTION BODY[0x5cff10]: 144
	// <0x5cff10>|0x000|+0x020:'160'
	// <0>
	// <1>
	// <2>
	// <0x5cff30>|0x020|+0x01a:'164'
	// <0>
	// <0x5cff4a>|0x03a|+0x007:'166'
	// <0>
	// <1>
	// <0x5cff51>|0x041|+0x014:'169'
	// <0>
	// <0x5cff65>|0x055|+0x007:'171'
	// <0>
	// <1>
	// <0x5cff6c>|0x05c|+0x014:'174'
	// <0>
	// <0x5cff80>|0x070|+0x007:'176'
	// <0>
	// <1>
	// <0x5cff87>|0x077|+0x014:'179'
	// <0>
	// <0x5cff9b>|0x08b|+0x007:'181'
	// <0>
	// <1>
	// <0x5cffa2>|0x092|+0x00b:'184'
	// <0>
	// <1>
	// <0x5cffad>|0x09d|+0x049:'187'
	// <0>
	// <0x5cfff6>|0x0e6|+0x032:'189'
	// <0>
	// <0x5d0028>|0x118|+0x01a:'191'
	// <0x5d0042>|0x132|+0x01e:'192'
	// <0>
	// <0x5d0060>|0x150|+0x014:'194'
	// <0x5d0074>|0x164|+0x022:'195'
	// <0>
	// <0x5d0096>|0x186|+0x014:'197'
	// <0x5d00aa>|0x19a|+0x022:'198'
	// <0>
	// <0x5d00cc>|0x1bc|+0x011:'200'
	// <0x5d00dd>|0x1cd|+0x01e:'201'
	// <0>
	// <1>
	// <0x5d00fb>|0x1eb|+0x014:'204'
	// <0x5d010f>|0x1ff|+0x007:'205'
	// <0>
	// <0x5d0116>|0x206|+0x014:'207'
	// <0x5d012a>|0x21a|+0x00b:'208'
	// <0x5d0135>|0x225|+0x00f:'209'
	// <0x5d0144>|0x234|+0x002:'210'
	// <0x5d0146>|0x236|+0x007:'211'
	// <0>
	// <1>
	// <0x5d014d>|0x23d|+0x01a:'214'
	// <0x5d0167>|0x257|+0x007:'215'
	// <0>
	// <0x5d016e>|0x25e|+0x014:'217'
	// <0x5d0182>|0x272|+0x00a:'218'
	// <0>
	// <0x5d018c>|0x27c|+0x014:'220'
	// <0x5d01a0>|0x290|+0x00a:'221'
	// <0>
	// <0x5d01aa>|0x29a|+0x014:'223'
	// <0x5d01be>|0x2ae|+0x00a:'224'
	// <0>
	// <0x5d01c8>|0x2b8|+0x014:'226'
	// <0x5d01dc>|0x2cc|+0x00a:'227'
	// <0>
	// <0x5d01e6>|0x2d6|+0x014:'229'
	// <0x5d01fa>|0x2ea|+0x00a:'230'
	// <0>
	// <0x5d0204>|0x2f4|+0x014:'232'
	// <0x5d0218>|0x308|+0x00a:'233'
	// <0>
	// <0x5d0222>|0x312|+0x014:'235'
	// <0x5d0236>|0x326|+0x00a:'236'
	// <0>
	// <0x5d0240>|0x330|+0x017:'238'
	// <0>
	// <0x5d0257>|0x347|+0x004:'240'
	// <0x5d025b>|0x34b|+0x00c:'241'
	// <0>
	// <1>
	// <0x5d0267>|0x357|+0x005:'244'
	// <0x5d026c>|0x35c|+0x00a:'245'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d0276>|0x366|+0x017:'250'
	// <0>
	// <0x5d028d>|0x37d|+0x004:'252'
	// <0x5d0291>|0x381|+0x00c:'253'
	// <0x5d029d>|0x38d|+0x005:'254'
	// <0x5d02a2>|0x392|+0x00a:'255'
	// <0>
	// <1>
	// <0x5d02ac>|0x39c|+0x017:'258'
	// <0>
	// <0x5d02c3>|0x3b3|+0x004:'260'
	// <0x5d02c7>|0x3b7|+0x00c:'261'
	// <0x5d02d3>|0x3c3|+0x005:'262'
	// <0x5d02d8>|0x3c8|+0x00a:'263'
	// <0>
	// <1>
	// <0x5d02e2>|0x3d2|+0x017:'266'
	// <0>
	// <0x5d02f9>|0x3e9|+0x004:'268'
	// <0x5d02fd>|0x3ed|+0x00c:'269'
	// <0x5d0309>|0x3f9|+0x005:'270'
	// <0x5d030e>|0x3fe|+0x00a:'271'
	// <0>
	// <1>
	// <0x5d0318>|0x408|+0x017:'274'
	// <0>
	// <0x5d032f>|0x41f|+0x004:'276'
	// <0x5d0333>|0x423|+0x00c:'277'
	// <0x5d033f>|0x42f|+0x005:'278'
	// <0x5d0344>|0x434|+0x00a:'279'
	// <0>
	// <1>
	// <0x5d034e>|0x43e|+0x017:'282'
	// <0>
	// <0x5d0365>|0x455|+0x004:'284'
	// <0x5d0369>|0x459|+0x00c:'285'
	// <0x5d0375>|0x465|+0x005:'286'
	// <0x5d037a>|0x46a|+0x00a:'287'
	// <0>
	// <1>
	// <0x5d0384>|0x474|+0x014:'290'
	// <0x5d0398>|0x488|+0x00a:'291'
	// <0>
	// <0x5d03a2>|0x492|+0x014:'293'
	// <0x5d03b6>|0x4a6|+0x00a:'294'
	// <0>
	// <0x5d03c0>|0x4b0|+0x014:'296'
	// <0x5d03d4>|0x4c4|+0x00a:'297'
	// <0>
	// <0x5d03de>|0x4ce|+0x014:'299'
	// <0x5d03f2>|0x4e2|+0x00a:'300'
	// <0>
	// <0x5d03fc>|0x4ec|+0x014:'302'
	// <0x5d0410>|0x500|+0x00a:'303'
	// ******
}

// STATE[STUB]
void player_input_handler::process_third_person_mode( )
{
	// FUNCTION BODY[0x5d0540]: 13
	// <0x5d0540>|0x000|+0x006:'307'	{
	// <0x5d0546>|0x006|+0x018:'308'
	// <0>
	// <0x5d055e>|0x01e|+0x012:'310'
	// <0x5d0570>|0x030|+0x00c:'311'
	// <0>
	// <1>
	// <0x5d057c>|0x03c|+0x018:'314'
	// <0x5d0594>|0x054|+0x010:'315'
	// <0x5d05a4>|0x064|+0x00f:'316'
	// <0x5d05b3>|0x073|+0x017:'317'
	// <0x5d05ca>|0x08a|+0x026:'318'
	// <0x5d05f0>|0x0b0|-0x079:'319'
	// <0>
	// <0x5d0577>|0x037|+0x09f:'321'
	// <0x5d0616>|0x0d6|      :'321'	}
	// ******
}

// STATE[STUB]
void player_input_handler::update_inverted_view( float4x4 const& player_head_transform )
{
	// LOCALS
	// float4x4 						new_inverted_view
	// ******

	// FUNCTION BODY[0x5cfd60]: 13
	// <0x5cfd76>|0x016|+0x009:'325'
	// <0x5cfd7f>|0x01f|+0x007:'326'
	// <0x5cfd86>|0x026|+0x006:'327'
	// <0>
	// <1>
	// <0x5cfd8c>|0x02c|+0x003:'330'
	// <0x5cfd8f>|0x02f|+0x055:'331'
	// <0x5cfde4>|0x084|+0x03d:'332'
	// <0x5cfe21>|0x0c1|+0x0c7:'333'
	// <0x5cfee8>|0x188|+0x00e:'334'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void player_input_handler::set_yaw_pitch_distance( const float yaw, const float arg_1, const float arg_2 )
{
	// FUNCTION BODY[0x5cfa00]: 4
	// <0>
	// <0x5cfa10>|0x010|+0x010:'343'
	// <0x5cfa20>|0x020|+0x008:'344'
	// <0x5cfa28>|0x028|+0x008:'345'
	// ******
}

// STATE[STUB]
void player_input_handler::set_input_mode(
	const input_mode_type_enum		arg_0 /* input_mode_type_enum input_mode */
)
{
	// FUNCTION BODY[0x5cf960]: 2
	// <0x5cf960>|0x000|+0x000:'349'	{
	// <0x5cf960>|0x000|+0x020:'350'
	// <0x5cf980>|0x020|-0x00d:'350'
	// <0x5cf973>|0x013|+0x012:'351'
	// <0x5cf985>|0x025|-0x006:'351'
	// <0x5cf97f>|0x01f|+0x012:'352'
	// <0x5cf991>|0x031|      :'352'	}
	// ******
}

// STATE[STUB]
void player_input_handler::on_focus( bool b_focus_enter )
{
	// CALL SITE INFO
	// <0x5cf9bb> -> input::world& < unknown >()
	// <0x5cf9c8> -> void < unknown >( input::handler& )
	// <0x5cf9df> -> input::world& < unknown >()
	// <0x5cf9ec> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5cf9a0]: 6
	// <0x5cf9a0>|0x000|+0x000:'384'	{
	// <0>
	// <1>
	// <0x5cf9a0>|0x000|+0x00a:'387'
	// <0x5cf9aa>|0x00a|+0x015:'388'
	// <0>
	// <0x5cf9bf>|0x01f|+0x00f:'390'
	// <0x5cf9ce>|0x02e|-0x003:'390'
	// <0x5cf9cb>|0x02b|+0x024:'391'
	// <0x5cf9ef>|0x04f|      :'391'	}
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

	// typedef
	// 	vostok::fixed_vector< std::pair< enum survarium::game_action_id, enum survarium::player_input_handler::action_state_enum >, 32 >
	// 	actions_type;

	// ******

} // namespace survarium
