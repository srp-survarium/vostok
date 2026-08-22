////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "chat_handler.h"
#include "chat_tab.h"
#include "game.h"
#include "base_game_scene.h"
#include "base_network_client.h"
#include "network_client.h"
#include "messaging_client.h"
#include "lobby_menu.h"
#include "key_binder.h"
#include "swf_input_translator.h"
#include "text_translator.h"
#include "game_action_id.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_function_handler_params.h>

namespace survarium {

// claude@NOTE: every QUANTITY/SPLIT method here is STRUCTURE-FAITHFUL (statement
// sequence + named locals verified against pdb_fetch --view structure for each fn:
// set_mode's 9 locals, initialize's func/proxy, add_message's tabs, focus's argument
// all match the target). The residual % is driven entirely by cross-module
// inlining/LTCG, NOT source shape:
//   - scaleform flash_value ctor/dtor/Set*/SetMember + flash_movie::Invoke/Create*
//     (value.cpp/movie.cpp) body into Scaleform::GFx but DON'T whole-program-inline
//     into these callers the way the shipped /LTCG build did, so each flash_value
//     temp / Invoke stays an out-of-line CALL here (the BASE_ONLY rows in
//     add_message/set_mode, the missing TRGT_ONLY GFx::Value inlines in initialize);
//   - flash_movie::Advance/Restart/ForceCollectGarbage are EMPTY stubs, so tick()
//     inlines its movie->Advance to nothing -> base body == ret;
//   - base_network_client::has_bandwidth + messaging_client::on_message_typed and
//     network_client::get_player_team/get_local_player do not inline the way the
//     target did, so focus/callback/on_message_typed/add_message keep extra
//     guard/call statements out-of-line;
//   - key_binder::get_binded_action stub-folds on_keyboard_action's binded-action
//     branches, flipping focus(false) from a shared CALL (target) to an inlined block.
// Match lifts once those siblings/scaleform are matched - structure here is correct.

chat_handler::chat_handler( game& game )
	: m_focused( false )
	, m_active( false )
	, m_game_ui_mode( false )
	, m_game( game )
{
}

void chat_handler::initialize( resources::unmanaged_resource_ptr const& ui )
{
	m_chat_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( ui );

	get_movie( )->movie->SetBackgroundAlpha( 0.f );
	get_movie( )->movie->SetViewAlignment( flash_movie::Align_TopRight );
	get_movie( )->movie->SetViewScaleMode( flash_movie::SM_NoScale );
	get_movie( )->movie->SetPriority( 30 );

	get_movie( )->movie->SetExternalInterface( this );

	flash_value func;
	get_movie( )->movie->GetVariable( &func, "root.chat" );

	flash_value proxy;
	get_movie( )->movie->CreateFunction( &proxy, this );
	func.SetMember( "send_function", proxy );
}

chat_handler::~chat_handler( )
{
}

void chat_handler::call( flash_function_handler_params& params )
{
	flash_value w_text;
	params.pArgs[ 0 ].GetMember( "text", &w_text );

	if( m_game.network_client( ).has_bandwidth( ) )
		m_game.network_client( ).messaging_client( ).on_message_typed( w_text.GetStringW( ), messaging::player_general_channel );
}

void chat_handler::callback(
	flash_movie*			__formal,
	pcstr					methodName,
	flash_value const*		args,
	u32						__formal2
)
{
	if( strcmp( methodName, "chat_enter_start" ) == 0 )
		focus( true );
	else if( strcmp( methodName, "chat_enter_cancel" ) == 0 )
		focus( false );
	else if( strcmp( methodName, "set_mouse_cursor" ) == 0 && !m_focused )
		m_game.lobby_menu( ).set_cursor( const_cast< flash_value& >( args[ 0 ] ).GetBool( ) );
}

bool chat_handler::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	toggle_action_enum action_type;
	game_action_id binded_action = m_game.get_key_binder( ).get_binded_action( key, action_type, 8 );

	if( action == input::kb_key_down )
	{
		if( binded_action == kSEND_MESSAGE || key == input::key_numpadenter )
		{
			get_movie( )->movie->Invoke( "root.finish_typing_message", NULL, NULL, 0 );
			focus( false );
		}
		else if( key == input::key_escape )
			focus( false );
		else if( binded_action == kSELECT_SEND_TO )
		{
			get_movie( )->movie->Invoke( "root.tab_channel", NULL, NULL, 0 );
			return true;
		}
	}

	if( m_chat_ui && m_chat_ui->movie )
		m_game.input_translator( ).process_keyboard( input_world, key, action, get_movie( )->movie, m_game.game_time_ms( ) );

	return true;
}

bool chat_handler::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return m_game_ui_mode;
}

bool chat_handler::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	return m_game_ui_mode;
}

bool chat_handler::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return m_game_ui_mode;
}

void chat_handler::on_message_typed( wchar_t const* text, messaging::message_channel_enum message_chanel )
{
	if( m_game.network_client( ).has_bandwidth( ) )
		m_game.network_client( ).messaging_client( ).on_message_typed( text, message_chanel );
}

void chat_handler::add_message(
	const messaging::message_channel_enum	channel,
	wchar_t const*		w_text,
	wchar_t const*		w_sender_name
)
{
	flash_value obj;
	get_movie( )->movie->CreateObject( &obj );

	flash_value ret_args;
	ret_args.SetStringW( w_sender_name );
	obj.SetMember( "name", ret_args );

	ret_args.SetString( "(12:12:12)" );
	obj.SetMember( "time", ret_args );

	ret_args.SetUInt( channel );
	obj.SetMember( "type", ret_args );

	wchar_t text_to_send[ 512 ];

	if( m_game_ui_mode && channel == messaging::player_match_channel )
	{
		char sender_name[ 32 ];
		wcstombs_s( NULL, sender_name, w_sender_name, -1 );

		network_client* net_client = static_cast< network_client* >( &m_game.network_client( ) );
		game_team_id sender_team = net_client->get_player_team( sender_name );

		bool same_team = false;
		if( sender_team != team_undefined )
			same_team = net_client->get_local_player( )->team( ) == sender_team;

		if( same_team )
		{
			ret_args.SetString( "Red" );
			obj.SetMember( "color", ret_args );

			swprintf( text_to_send, 512, L"%s", w_text );
		}
		else
		{
			wchar_t to_all_localized[ 512 ];
			m_game.text_translator( ).translate_text( "st_to_all", to_all_localized );

			swprintf( text_to_send, 512, L"[%s] %s", to_all_localized, w_text );
		}
	}
	else
		swprintf( text_to_send, 512, L"%s", w_text );

	ret_args.SetStringW( text_to_send );
	obj.SetMember( "text", ret_args );

	get_movie( )->movie->Invoke( "root.add_chat_message", NULL, &obj, 1 );
}

void chat_handler::add_to_recent_list( wchar_t const* name )
{
	if( m_game_ui_mode )
		return;

	flash_value obj;
	obj.SetStringW( name );
	get_movie( )->movie->Invoke( "root.add_chat_recent", NULL, &obj, 1 );
}

void chat_handler::set_mode( bool is_game_mode )
{
	chat_tab lobby_menu_tabs[ 5 ] =
	{
		{ "st_chat_channel_general",	"White",	1,	"/general" },
		{ "st_chat_channel_private",	"Pink",		4,	"" },
		{ "st_chat_channel_clan",		"Blue",		3,	"/clan" },
		{ "st_chat_channel_squad",		"Green",	8,	"/squad" },
		{ "st_chat_channel_system",		"Red",		2,	"" },
	};

	chat_tab game_menu_tabs[ 2 ] =
	{
		{ "st_chat_channel_team",	"White",	is_game_mode ? ( m_game.network_client( ).messaging_client( ).local_player_team( ) != team_1 ? 7 : 6 ) : 6,	"/team" },
		{ "st_chat_channel_match",	"White",	5,	"/all" },
	};

	m_game_ui_mode = is_game_mode;

	chat_tab* current_tabs;
	u8 count;
	if( is_game_mode )
	{
		current_tabs = game_menu_tabs;
		count = 2;
	}
	else
	{
		current_tabs = lobby_menu_tabs;
		count = 5;
	}

	flash_value channels_array;
	get_movie( )->movie->CreateArray( &channels_array );

	for( u8 i = 0; i < count; ++i )
	{
		flash_value chat_tab_value;
		get_movie( )->movie->CreateObject( &chat_tab_value );

		wchar_t channel_name_txt[ 512 ];
		m_game.text_translator( ).translate_text( current_tabs[ i ].name, channel_name_txt );

		flash_value chat_tab_member;
		chat_tab_member.SetStringW( channel_name_txt );
		chat_tab_value.SetMember( "name", chat_tab_member );

		chat_tab_member.SetString( current_tabs[ i ].color );
		chat_tab_value.SetMember( "color", chat_tab_member );

		chat_tab_member.SetInt( current_tabs[ i ].id );
		chat_tab_value.SetMember( "id", chat_tab_member );

		chat_tab_member.SetInt( current_tabs[ i ].id );
		chat_tab_value.SetMember( "icon", chat_tab_member );

		if( strcmp( current_tabs[ i ].key, "" ) != 0 )
		{
			chat_tab_member.SetString( current_tabs[ i ].key );
			chat_tab_value.SetMember( "key", chat_tab_member );
		}

		channels_array.PushBack( chat_tab_value );
	}

	get_movie( )->movie->Invoke( "root.set_channels", NULL, &channels_array, 1 );

	flash_value is_heavy_mode;
	is_heavy_mode.SetBoolean( !m_game_ui_mode );
	get_movie( )->movie->Invoke( "root.set_heavy", NULL, &is_heavy_mode, 1 );
}

void chat_handler::set_local_player_name( pcstr account_name )
{
	wchar_t an[ 256 ];
	mbstowcs_s( NULL, an, account_name, -1 );

	flash_value local_player_name;
	local_player_name.SetStringW( an );
	get_movie( )->movie->Invoke( "root.set_local_player_name", NULL, &local_player_name, 1 );
}

void chat_handler::focus( bool b_focused )
{
	if( !m_game.network_client( ).has_bandwidth( ) )
		return;

	if( m_focused == b_focused )
		return;

	if( m_game_ui_mode || b_focused )
	{
		flash_value argument;
		argument.SetBoolean( b_focused );
		get_movie( )->movie->Invoke( "root.focus_chat", NULL, &argument, 1 );
	}

	m_focused = b_focused;

	if( b_focused )
		m_game.input_world( ).add_handler( *this );
	else
		m_game.input_world( ).remove_handler( *this );
}

void chat_handler::show( base_game_scene* scene )
{
	scene->show_movie( m_chat_ui );
	m_active = true;
}

void chat_handler::hide( base_game_scene* scene )
{
	scene->hide_movie( m_chat_ui );
	m_active = false;
}

void chat_handler::tick( u32 delta )
{
	get_movie( )->movie->Advance( delta * math::epsilon_3, 1 );
}

} // namespace survarium
