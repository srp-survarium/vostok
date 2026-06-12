////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\chat_handler.h"

namespace survarium {

// STATE[STUB]
explicit chat_handler::chat_handler( game& game )
{
	// FUNCTION BODY[0x5dc4f0]: 0
	// <0x5dc4f0>|0x000|+0x070:'34'	{
	// <0x5dc560>|0x070|      :'35'	}
	// ******
}

// STATE[STUB]
void chat_handler::initialize( resources::unmanaged_resource_ptr const& ui )
{
	// LOCALS
	// flash_value 						func
	// flash_value 						proxy
	// ******

	// FUNCTION BODY[0x5dc570]: 18
	// <0x5dc570>|0x000|+0x075:'39'
	// <0>
	// <0x5dc5e5>|0x075|+0x01c:'41'
	// <0x5dc601>|0x091|+0x015:'42'
	// <0x5dc616>|0x0a6|+0x014:'43'
	// <0>
	// <0x5dc62a>|0x0ba|+0x00d:'45'
	// <0>
	// <1>
	// <0x5dc637>|0x0c7|+0x01d:'48'
	// <0>
	// <1>
	// <2>
	// <0x5dc654>|0x0e4|+0x023:'52'
	// <0>
	// <1>
	// <0x5dc677>|0x107|+0x023:'55'
	// <0x5dc69a>|0x12a|+0x02c:'56'
	// ******
}

// STATE[STUB]
 chat_handler::~chat_handler( )
{
	// FUNCTION BODY[0x5dc480]: 0
	// <0x5dc480>|0x000|+0x017:'60'	{
	// <0x5dc497>|0x017|      :'61'	}
	// ******
}

// STATE[STUB]
void chat_handler::call( flash_function_handler_params& params )
{
	// LOCALS
	// flash_value 						w_text
	// ******

	// CALL SITE INFO
	// <0x5dcb06> -> bool < unknown >() const
	// <0x5dcb1b> -> messaging_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5dcaa0]: 8
	// <0>
	// <1>
	// <0x5dcaac>|0x00c|+0x042:'67'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5dcaee>|0x04e|+0x037:'72'
	// ******
}

// STATE[STUB]
void chat_handler::callback(
	flash_movie*			__formal,
	pcstr					methodName,
	flash_value const*		args,
	u32						__formal
)
{
	// CALL SITE INFO
	// <0x5dbce4> -> bool < unknown >() const
	// <0x5dbd5f> -> input::world& < unknown >()
	// <0x5dbd69> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5dbc90]: 12
	// <0x5dbc90>|0x000|+0x00c:'76'	{
	// <0x5dbc9c>|0x00c|+0x037:'77'
	// <0>
	// <0x5dbcd3>|0x043|+0x0a1:'79'
	// <0>
	// <0x5dbd74>|0x0e4|+0x035:'81'
	// <0>
	// <0x5dbda9>|0x119|+0x012:'83'
	// <0>
	// <0x5dbdbb>|0x12b|+0x035:'85'
	// <0>
	// <0x5dbdf0>|0x160|-0x085:'87'
	// <0>
	// <0x5dbd6b>|0x0db|+0x047:'89'
	// <0x5dbdb2>|0x122|+0x052:'89'
	// <0x5dbe04>|0x174|      :'89'	}
	// ******
}

// STATE[STUB]
bool chat_handler::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	// LOCALS
	// toggle_action_enum 				action_type
	// ******

	return false;

	// FUNCTION BODY[0x5dbe10]: 28
	// <0x5dbe10>|0x000|+0x009:'96'	{
	// <0>
	// <1>
	// <0x5dbe19>|0x009|+0x01b:'99'
	// <0>
	// <1>
	// <0x5dbe34>|0x024|+0x006:'102'
	// <0>
	// <0x5dbe3a>|0x02a|+0x03e:'104'
	// <0>
	// <0x5dbe78>|0x068|+0x01c:'106'
	// <0x5dbe94>|0x084|-0x04d:'107'
	// <0>
	// <0x5dbe47>|0x037|+0x003:'109'
	// <0>
	// <0x5dbe4a>|0x03a|+0x002:'111'
	// <0>
	// <0x5dbe4c>|0x03c|+0x005:'113'
	// <0>
	// <0x5dbe51>|0x041|+0x04c:'115'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5dbe9d>|0x08d|+0x010:'120'
	// <0>
	// <0x5dbead>|0x09d|-0x040:'122'
	// <0>
	// <0x5dbe6d>|0x05d|+0x002:'124'
	// <0x5dbe6f>|0x05f|+0x065:'125'
	// <0x5dbed4>|0x0c4|      :'125'	}
	// ******
}

// STATE[STUB]
bool chat_handler::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return false;

	// FUNCTION BODY[0x5dba60]: 2
	// <0>
	// <0x5dba60>|0x000|+0x003:'130'
	// ******
}

// STATE[STUB]
bool chat_handler::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	return false;

	// FUNCTION BODY[0x5dba50]: 2
	// <0>
	// <0x5dba50>|0x000|+0x003:'136'
	// ******
}

// STATE[STUB]
bool chat_handler::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return false;

	// FUNCTION BODY[0x5dba40]: 2
	// <0>
	// <0x5dba40>|0x000|+0x003:'142'
	// ******
}

// STATE[STUB]
void chat_handler::on_message_typed( wchar_t const* text, messaging::message_channel_enum message_chanel )
{
	// CALL SITE INFO
	// <0x5dca6f> -> bool < unknown >() const
	// <0x5dca88> -> messaging_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5dca60]: 2
	// <0x5dca61>|0x001|+0x014:'147'
	// <0x5dca75>|0x015|+0x020:'148'
	// ******
}

// STATE[STUB]
void chat_handler::add_message(
	const messaging::message_channel_enum	arg_0 /* messaging::message_channel_enum channel */,
	wchar_t const*		w_text,
	wchar_t const*		w_sender_name
)
{
	// LOCALS
	// flash_value 						obj
	// flash_value 						ret_args
	// wchar_t[512] 					text_to_send
	// char[32] 						sender_name
	// network_client* 					net_client
	// game_team_id 					sender_team
	// wchar_t[512] 					to_all_localized
	// ******

	// CALL SITE INFO
	// <0x5dc8a2> -> game_team_id < unknown >() const
	// ******

	// FUNCTION BODY[0x5dc720]: 47
	// <0>
	// <0x5dc72f>|0x00f|+0x029:'156'
	// <0>
	// <1>
	// <2>
	// <0x5dc758>|0x038|+0x018:'160'
	// <0x5dc770>|0x050|+0x02c:'161'
	// <0>
	// <0x5dc79c>|0x07c|+0x00a:'163'
	// <0x5dc7a6>|0x086|+0x02a:'164'
	// <0>
	// <0x5dc7d0>|0x0b0|+0x022:'166'
	// <0x5dc7f2>|0x0d2|+0x041:'167'
	// <0>
	// <1>
	// <2>
	// <0x5dc833>|0x113|+0x012:'171'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5dc845>|0x125|+0x02f:'177'
	// <0>
	// <0x5dc874>|0x154|+0x00d:'179'
	// <0>
	// <0x5dc881>|0x161|+0x04c:'181'
	// <0>
	// <0x5dc8cd>|0x1ad|+0x00e:'183'
	// <0x5dc8db>|0x1bb|+0x0d8:'184'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5dc9b3>|0x293|+0x01a:'190'
	// <0>
	// <0x5dc9cd>|0x2ad|+0x027:'192'
	// <0>
	// <1>
	// <0x5dc9f4>|0x2d4|-0x108:'195'
	// <0x5dc8ec>|0x1cc|+0x01f:'196'
	// <0>
	// <0x5dc90b>|0x1eb|+0x00d:'198'
	// <0x5dc918>|0x1f8|+0x02a:'199'
	// <0>
	// <0x5dc942>|0x222|+0x01e:'201'
	// ******
}

// STATE[STUB]
void chat_handler::add_to_recent_list( wchar_t const* name )
{
	// LOCALS
	// flash_value 						obj
	// ******

	// FUNCTION BODY[0x5dbc20]: 7
	// <0x5dbc28>|0x008|+0x007:'206'
	// <0>
	// <1>
	// <2>
	// <0x5dbc2f>|0x00f|+0x019:'210'
	// <0>
	// <0x5dbc48>|0x028|+0x01d:'212'
	// ******
}

// STATE[STUB]
void chat_handler::set_mode( bool is_game_mode )
{
	// LOCALS
	// chat_tab[5] 						lobby_menu_tabs
	// chat_tab[2] 						game_menu_tabs
	// flash_value 						channels_array
	// flash_value 						is_heavy_mode
	// chat_tab* 						current_tabs
	// wchar_t[512] 					channel_name_txt
	// flash_value 						chat_tab_member
	// flash_value 						chat_tab_value
	// ******

	// CALL SITE INFO
	// <0x5dbfd7> -> messaging_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5dbee0]: 88
	// <0>
	// <1>
	// <0x5dbef0>|0x010|+0x00c:'227'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5dbefc>|0x01c|+0x0f6:'236'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5dbff2>|0x112|+0x024:'243'
	// <0x5dc016>|0x136|+0x025:'244'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5dc03b>|0x15b|+0x003:'251'
	// <0>
	// <0x5dc03e>|0x15e|+0x004:'253'
	// <0>
	// <0x5dc042>|0x162|+0x00b:'255'
	// <0x5dc04d>|0x16d|+0x002:'256'
	// <0>
	// <0x5dc04f>|0x16f|+0x002:'258'
	// <0>
	// <0x5dc051>|0x171|+0x00b:'260'
	// <0x5dc05c>|0x17c|+0x002:'261'
	// <0>
	// <1>
	// <2>
	// <0x5dc05e>|0x17e|+0x01e:'265'
	// <0>
	// <0x5dc07c>|0x19c|+0x024:'267'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5dc0a0>|0x1c0|+0x021:'272'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5dc0c1>|0x1e1|+0x024:'277'
	// <0>
	// <0x5dc0e5>|0x205|+0x06e:'279'
	// <0x5dc153>|0x273|+0x02e:'280'
	// <0>
	// <0x5dc181>|0x2a1|+0x069:'282'
	// <0x5dc1ea>|0x30a|+0x02c:'283'
	// <0>
	// <0x5dc216>|0x336|+0x027:'285'
	// <0x5dc23d>|0x35d|+0x03b:'286'
	// <0>
	// <0x5dc278>|0x398|+0x029:'288'
	// <0x5dc2a1>|0x3c1|+0x036:'289'
	// <0>
	// <0x5dc2d7>|0x3f7|+0x033:'291'
	// <0>
	// <0x5dc30a>|0x42a|+0x009:'293'
	// <0x5dc313>|0x433|+0x02a:'294'
	// <0>
	// <1>
	// <0x5dc33d>|0x45d|+0x01d:'297'
	// <0x5dc35a>|0x47a|+0x070:'298'
	// <0>
	// <0x5dc3ca>|0x4ea|+0x01e:'300'
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
	// <0x5dc3e8>|0x508|+0x004:'311'
	// <0x5dc3ec>|0x50c|+0x031:'312'
	// ******
}

// STATE[STUB]
void chat_handler::set_local_player_name( pcstr account_name )
{
	// LOCALS
	// flash_value 						local_player_name
	// wchar_t[256] 					an
	// ******

	// FUNCTION BODY[0x5dbb80]: 7
	// <0>
	// <0x5dbb88>|0x008|+0x022:'318'
	// <0>
	// <1>
	// <0x5dbbaa>|0x02a|+0x01d:'321'
	// <0>
	// <0x5dbbc7>|0x047|+0x024:'323'
	// ******
}

// STATE[STUB]
void chat_handler::focus( bool b_focused )
{
	// LOCALS
	// flash_value 						argument
	// ******

	// CALL SITE INFO
	// <0x5dbac7> -> bool < unknown >() const
	// <0x5dbb49> -> input::world& < unknown >()
	// <0x5dbb5b> -> void < unknown >( input::handler& )
	// <0x5dbb62> -> input::world& < unknown >()
	// <0x5dbb74> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5dbab0]: 19
	// <0x5dbab0>|0x000|+0x000:'327'	{
	// <0x5dbab0>|0x000|+0x021:'328'
	// <0>
	// <1>
	// <0x5dbad1>|0x021|+0x009:'331'
	// <0>
	// <1>
	// <0x5dbada>|0x02a|+0x00a:'334'
	// <0>
	// <1>
	// <0x5dbae4>|0x034|+0x01a:'337'
	// <0x5dbafe>|0x04e|+0x01d:'338'
	// <0x5dbb1b>|0x06b|+0x01f:'339'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5dbb3a>|0x08a|+0x018:'344'
	// <0x5dbb52>|0x0a2|+0x00b:'344'
	// <0>
	// <0x5dbb5d>|0x0ad|+0x00e:'346'
	// <0x5dbb6b>|0x0bb|-0x01e:'346'
	// <0x5dbb4d>|0x09d|+0x019:'347'
	// <0x5dbb66>|0x0b6|+0x010:'347'
	// <0x5dbb76>|0x0c6|      :'347'	}
	// ******
}

// STATE[STUB]
void chat_handler::show( base_game_scene* scene )
{
	// FUNCTION BODY[0x5dca50]: 4
	// <0>
	// <1>
	// <0x5dca51>|0x001|+0x009:'353'
	// <0x5dca5a>|0x00a|+0x004:'354'
	// ******
}

// STATE[STUB]
void chat_handler::hide( base_game_scene* scene )
{
	// FUNCTION BODY[0x5dca00]: 4
	// <0>
	// <1>
	// <0x5dca01>|0x001|+0x03c:'360'
	// <0x5dca3d>|0x03d|+0x005:'361'
	// ******
}

// STATE[STUB]
void chat_handler::tick( u32 delta )
{
	// FUNCTION BODY[0x5dba70]: 3
	// <0x5dba94>|0x024|-0x024:'366'
	// <0>
	// <0x5dba70>|0x000|+0x02a:'368'
	// <0x5dba9a>|0x02a|+0x00b:'368'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	char[32]
	// 	account_name_type;

	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::inventory_item_instance*
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
