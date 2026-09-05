// SPDX-License-Identifier: GPL-3.0-or-later

// The reconstructed UI paths retain a few target/base Scaleform wrapper
// inline-boundary differences under LTCG.

#include "pch.h"
#include "lobby_menu.h"
#include "lobby_menu_external_handler.h"
#include "game.h"
#include "game_memory.h"
#include "lobby_client.h"
#include "messaging_client.h"
#include "base_network_client.h"
#include "chat_handler.h"
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/math_constants.h>
#include <vostok/vectora.h>
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/inventory_item_instance.h>
#include <vostok/game_core/player_parameters_modifyer.h>
#include <vostok/network/login_client.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/strings_functions.h>
#include "ui_label.h"
#include "price_item.h"
#include "player_leveling_info.h"
#include "player_parameters_cooker_data.h"
#include "profile_player_character.h"
#include "game_project.h"
#include "game_object_.h"
#include "render_visual.h"
#include "lobby_camera.h"
#include <vostok/game_core/static_collision.h>
#include <vostok/sound/world.h>
#include <vostok/sound/world_user.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/scaleform/sources/flash_function_handler.h>
#include <vostok/scaleform/sources/flash_function_handler_params.h>

namespace survarium {

inline network::login_client& lobby_menu::login_client( )
{
	return get_game( ).get_network_client( )->login_client( );
}

// TU-local (canonical headers/relocate_item_func.h; owner mapping in
// git show 885e1d4a4:temp/triage_log.md) - lobby_menu holds it through m_relocate_item_func
class relocate_item_func : public flash_function_handler , private boost::noncopyable {
public:
			explicit	relocate_item_func	( game& g );

	virtual	void		call				( flash_function_handler_params& params ) override;

private:
	/* 0x0000 */	/* flash_function_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&		m_game;
}; // class relocate_item_func

STATIC_SIZE_ASSERT(relocate_item_func, 0xC);

static float const	clear_value		= 1.0f;

relocate_item_func::relocate_item_func( game& g )
	: m_game( g )
{
}

void relocate_item_func::call( flash_function_handler_params& params )
{
	LOG_INFO( "TRY RELOCATE(FLASH)!!!" );

	vector< relocate_item_descr > descriptions;
	lobby_client& lobby = m_game.get_network_client( )->lobby_client( );

	for ( u8 i = 0; i < params.pArgs[ 0 ].GetArraySize( ); ++i )
	{
		flash_value descr_value;
		params.pArgs[ 0 ].GetElement( i, &descr_value );

		flash_value descr_member_value;
		relocate_item_descr current;

		descr_value.GetMember( "profile", &descr_member_value );
		current.profile_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "id", &descr_member_value );
		current.item_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "dict_id", &descr_member_value );
		current.item_dict_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "sourceSlot", &descr_member_value );
		current.source_slot_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "targetSlot", &descr_member_value );
		current.target_slot_id = descr_member_value.GetUInt( );

		descr_value.GetMember( "count", &descr_member_value );
		current.amount = ( u16 )descr_member_value.GetUInt( );

		dictionary_item current_item = m_game.items_dictionary( ).item_by_id( current.item_dict_id );

		LOG_INFO(
			"try move item %d from %d to %d. amount=%d",
			current.item_id,
			current.source_slot_id,
			current.target_slot_id,
			current.amount
		);

		u32 second_item_id = 0;
		if ( current.target_slot_id == ammo1_weapon1_slot || current.target_slot_id == ammo2_weapon1_slot )
			second_item_id = weapon1_slot;
		else if ( current.target_slot_id == ammo1_weapon2_slot || current.target_slot_id == ammo2_weapon2_slot )
			second_item_id = weapon2_slot;

		if ( second_item_id )
		{
			profile_slot_enum const weapon_slot = ( profile_slot_enum )second_item_id;
			second_item_id = lobby.profile( m_game.lobby_menu( ).selected_profile( ) ).slots[ weapon_slot ].item.dict_id;

			for ( vector< relocate_item_descr >::const_iterator j = descriptions.begin( ); j != descriptions.end( ); ++j )
			{
				if ( j->target_slot_id == weapon_slot )
					second_item_id = j->item_dict_id;
			}
		}

		if ( lobby.can_move_item( current_item.item_category, current.target_slot_id ) &&
			( !second_item_id || lobby.check_compatibility( current.item_dict_id, second_item_id ) ) )
			descriptions.push_back( current );
	}

	lobby.move_item( descriptions );
}

// Target and base retain different Scaleform temporary and argument statement
// partitions under LTCG; keep the target's named-local and dispatch order.
void lobby_menu_external_handler::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
	if ( strings::equal( methodName, "leave_queue" ) )
	{
		m_game.get_network_client( )->lobby_client( ).discard_playing_order( );
	}
	else if ( strings::equal( methodName, "play_button_clicked" ) )
	{
		if ( m_game.get_network_client( )->lobby_client( ).profiles_count( ) )
		{
			u8 faction_id = m_game.lobby_menu( ).selected_profile( );
			m_game.get_network_client( )->lobby_client( ).set_status_ready_for_match(
				m_game.get_network_client( )->lobby_client( ).profile( faction_id ).profile_id );
		}
	}
	else if ( strings::equal( methodName, "profile_changed" ) )
	{
		m_game.lobby_menu( ).on_profile_changed( ( u8 )args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "shop_ready" ) )
	{
		m_game.lobby_menu( ).on_shop_ui_ready( );
	}
	else if ( strings::equal( methodName, "set_mouse_cursor" ) )
	{
		m_game.lobby_menu( ).set_cursor( ( u8 )args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "buy_ok_clicked" ) )
	{
		u16 item_dict_id	= ( u16 )args[ 0 ].GetUInt( );
		u32 const items_count	= args[ 1 ].GetUInt( );

		u8 faction_id = ( u8 )args[ 2 ].GetUInt( );

		if ( !faction_id )
		{
			for ( u8 i = 1; i <= 4; ++i )
			{
				faction_price const& price = m_game.get_network_client( )->lobby_client( ).price( i );
				for ( u8 j = 0; j < price.count; ++j )
				{
					if ( price.items[ j ].item_dict_id == item_dict_id )
					{
						faction_id = i;
						break;
					}
				}

				if ( faction_id )
					break;
			}
		}

		m_game.get_network_client( )->lobby_client( ).buy_item( item_dict_id, items_count, faction_id, false );
	}
	else if ( strings::equal( methodName, "unlock_perks" ) )
	{
		flash_value skills_array	= args[ 0 ];
		flash_value perks_array		= args[ 1 ];

		vectora< player_skill > skills( g_allocator );

		for ( u8 i = 0; i < skills_array.GetArraySize( ); ++i )
		{
			flash_value branch_value;
			skills_array.GetElement			( i, &branch_value );

			flash_value branch_member_value;
			branch_value.GetMember			( "id", &branch_member_value );

			player_skill current_skill;
			current_skill.skill_id			= ( u8 )branch_member_value.GetUInt( );

			branch_value.GetMember			( "points", &branch_member_value );
			current_skill.skill_points		= ( u8 )branch_member_value.GetUInt( );

			skills.push_back				( current_skill );
		}

		vectora< u8 > perks( g_allocator );

		for ( u8 i = 0; i < perks_array.GetArraySize( ); ++i )
		{
			flash_value perk_value;
			perks_array.GetElement			( i, &perk_value );

			u8 perk = ( u8 )perk_value.GetUInt( );
			perks.push_back					( perk );
		}

		m_game.get_network_client( )->lobby_client( ).set_player_skills( skills, perks );
	}
	else if ( strings::equal( methodName, "reroll_ok_clicked" ) )
	{
		m_game.get_network_client( )->lobby_client( ).reroll_player_skills( );
	}
	else if ( strings::equal( methodName, "find_players" ) )
	{
		pcstr player_name = args[ 0 ].GetString( );
		enum { c_min_name_len_to_search = 3 };
		if ( strlen( player_name ) >= c_min_name_len_to_search )
			m_game.get_network_client( )->messaging_client( ).find_players_by_name( player_name );
	}
	else if ( strings::equal( methodName, "add_friend" ) )
	{
		const u32 account_id = args[ 0 ].GetUInt( );
		m_game.get_network_client( )->messaging_client( ).add_to_friend_list( account_id );
	}
	else if ( strings::equal( methodName, "remove_friend" ) )
	{
		m_game.get_network_client( )->messaging_client( ).remove_from_friend_list( args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "add_ignore" ) )
	{
		const u32 account_id = args[ 0 ].GetUInt( );
		m_game.get_network_client( )->messaging_client( ).add_to_ignore_list( account_id );
	}
	else if ( strings::equal( methodName, "remove_ignored" ) )
	{
		m_game.get_network_client( )->messaging_client( ).remove_from_ignore_list( args[ 0 ].GetUInt( ) );
	}
	else if ( strings::equal( methodName, "start_friend_message" ) )
	{
		flash_value ret_args[2];
		ret_args[ 0 ].SetString			( args[ 0 ].GetString( ) );
		ret_args[ 1 ].SetUInt			( 100 );
		m_game.get_chat_handler( ).get_movie( )->movie->Invoke( "root.start_message", NULL, ret_args, 2 );
		m_game.get_chat_handler( ).focus( true );
	}
	else if ( strings::equal( methodName, "show_settings" ) )
	{
		m_game.activate_main_menu( );
	}
}

// claude@NOTE: source structure and constants are exhausted. The target keeps calls
// to flash_value::SetString/SetStringW because its wrappers inline GFx::Value::operator=;
// the base wrappers call operator= and are then inlined here. Reopen only with the
// exact Scaleform 4.2.21 inline-header/compiler partition.
ui_label match_making_labels[] =
{
	{ "label_status",			"st_label_status" },
	{ "label_quenue",			"st_label_quenue" },
	{ "label_time_current",		"st_time_current" },
	{ "label_time_average",		"st_label_time_average" },
	{ "label_teamA",			"st_label_teamA" },
	{ "label_teamB",			"st_label_teamB" },
	{ "btnLeave",				"st_label_leave_queue_btn" },
};

void lobby_menu::show_match_making( bool b_show )
{
	if ( m_is_in_match_making != b_show )
	{
		if ( b_show )
		{
			m_level_loading_progress = 0.f;
			m_last_queries_count = 0;
			m_match_making_ui->movie->Restart	( );

			flash_value labels_array;
			m_match_making_ui->movie->CreateArray( &labels_array );

			for ( u32 i = 0; i < 7; ++i )
			{
				flash_value label;
				m_match_making_ui->movie->CreateObject( &label );

				flash_value label_member;
				label_member.SetString			( match_making_labels[ i ].name );
				label.SetMember					( "name", label_member );

				wchar_t label_txt[512];
				get_game( ).text_translator( ).translate_text( match_making_labels[ i ].label, label_txt );
				label_member.SetStringW			( label_txt );
				label.SetMember					( "label", label_member );

				labels_array.SetElement			( i, label );
			}

			m_match_making_ui->movie->Invoke	( "root.set_labels", NULL, &labels_array, 1 );

			flash_value text;
			text.SetStringW						( L"0 \x0441\x0435\x043a" );
			m_match_making_ui->movie->Invoke	( "root.set_current_time", NULL, &text, 1 );

			text.SetStringW						( L"0 \x0441\x0435\x043a" );
			m_match_making_ui->movie->Invoke	( "root.set_average_time", NULL, &text, 1 );

			text.SetStringW						( L"" );
			m_match_making_ui->movie->Invoke	( "root.set_place", NULL, &text, 1 );

			text.SetStringW						( L"\x0421\x043e\x0437\x0434\x0430\x043d\x0438\x0435 \x043c\x0430\x0442\x0447\x0430!" );
			m_match_making_ui->movie->Invoke	( "root.set_status", NULL, &text, 1 );

			show_movie							( m_match_making_ui );
		}
		else
		{
			hide_movie							( m_match_making_ui );
		}

		m_is_in_match_making = b_show;
	}
}

// claude@NOTE: flash glue now inlines at /Ox (scaleform Master Gold /GL); structure
// is faithful and the residual is LTCG scheduling of the inlined flash_value
// ctor/dtor, not the old inline-vs-call wall.
void lobby_menu::update_level_loading_progress( )
{
	char buff[64];
	vostok::sprintf						( buff, "(%d)", resources::pending_queries_count( ) );

	if ( m_last_queries_count > resources::pending_queries_count( ) )
		m_level_loading_progress += ( float )( m_last_queries_count - resources::pending_queries_count( ) ) / ( float )m_last_queries_count * ( 1.0f - m_level_loading_progress );

	m_last_queries_count = resources::pending_queries_count( );

	wchar_t w_text[512] = L"\x0417\x0430\x0433\x0440\x0443\x0437\x043a\x0430 \x0443\x0440\x043e\x0432\x043d\x044f[";

	wchar_t level_name[512];
	mbstowcs_s							( NULL, level_name, 512, get_game( ).project_resource_name( ), _TRUNCATE );

	wchar_t queries_count[512];
	mbstowcs_s							( NULL, queries_count, 512, buff, _TRUNCATE );

	wcscat_s							( w_text, sizeof( w_text ), level_name );
	wcscat_s							( w_text, sizeof( w_text ), L"]" );
	wcscat_s							( w_text, sizeof( w_text ), queries_count );

	flash_value text;
	text.SetStringW						( w_text );
	m_match_making_ui->movie->Invoke	( "root.set_status", NULL, &text, 1 );

	m_level_loading_progress = clear_value - m_level_loading_progress < math::epsilon_3
		? clear_value
		: m_level_loading_progress;

	flash_value progress;
	progress.SetUInt					( ( u32 )( m_level_loading_progress * 100.0f ) );
	m_match_making_ui->movie->Invoke	( "root.set_percent", NULL, &progress, 1 );
}

void lobby_menu::on_ui_destroy( )
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_lobby_menu_external_handler );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_relocate_item_func );
}

void lobby_menu::update_ui( const u32 frame_delta_in_ms, const u32 current_time_in_ms )
{
	if ( get_game( ).get_game_world( ).is_loading( ) )
		update_level_loading_progress( );

	if ( is_active( ) )
	{
		flash_value queries_count;
		queries_count.SetUInt			( resources::pending_queries_count( ) );
		m_lobby_menu_ui->movie->Invoke	( "root.set_disk_query", NULL, &queries_count, 1 );
	}

	float deltaTime = frame_delta_in_ms * math::epsilon_3;

	m_cursor_ui->movie->Advance			( deltaTime, 0 );
	m_lobby_menu_ui->movie->Advance		( deltaTime, 0 );
	m_message_ui->movie->Advance		( deltaTime, 0 );

	if ( m_is_in_match_making )
		m_match_making_ui->movie->Advance( deltaTime, 0 );
}

void lobby_menu::update_status( )
{
	flash_value b_val;

	fixed_string< 128 > status_str;
	if ( lobby_client( ).status( status_str ) == lobby::surf_lobby_menu )
		b_val.SetBoolean( true );

	network::login_client& login_client = this->login_client( );
	class lobby_client& lobby_client = this->lobby_client( );

	flash_value account_info;
	m_lobby_menu_ui->movie->CreateArray	( &account_info );
	account_info.SetElement				( 0, login_client.account_name( ) );
	account_info.SetElement				( 1, login_client.m_server_host );

	flash_value port;
	port.SetUInt					( login_client.m_server_port );
	account_info.SetElement				( 2, port );

	fixed_string< 128 > buff;
	buff.assignf				( "%s:%d", lobby_client.connection_info( ).host, lobby_client.connection_info( ).port );
	account_info.SetElement				( 3, buff.c_str( ) );

	m_lobby_menu_ui->movie->Invoke		( "root.lobby_menu.set_account_info", NULL, &account_info, 1 );

	flash_value log_message;
	log_message.SetString				( status_str.c_str( ) );
	m_lobby_menu_ui->movie->Invoke		( "root.lock_play_button", NULL, &b_val, 1 );
	m_lobby_menu_ui->movie->Invoke		( "root.set_status_info", NULL, &log_message, 1 );
}

void lobby_menu::on_render_scenes_ready( resources::queries_result& data )
{
	m_render_scene		= static_cast_resource_ptr< render::scene_ptr >( data[0].get_unmanaged_resource( ) );
	m_render_scene_view	= static_cast_resource_ptr< render::scene_view_ptr >( data[1].get_unmanaged_resource( ) );
	m_sound_scene			= data[2].get_unmanaged_resource( );

	get_game( ).get_sound_world( ).get_logic_world_user( ).set_listener_properties_interlocked(
		reinterpret_cast< sound::sound_scene_ptr& >( get_sound_scene( ) ),
		float3( 0.31f, 2.9f, 26.07f ),
		float3( 0.9f, -0.22f, 0.37f ),
		float3( 0.f, 1.f, 0.f )
	);

	m_lobby_game_project = static_cast_resource_ptr< simple_game_project_ptr >( data[3].get_unmanaged_resource( ) );
	m_character = VOSTOK_NEW_IMPL( ::survarium::g_allocator, profile_player_character )( *this );

	m_cursor_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[4].get_unmanaged_resource( ) );
	m_cursor_ui->movie->SetBackgroundAlpha( 0.f );
	m_cursor_ui->movie->SetPriority( 100 );

	m_lobby_menu_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[5].get_unmanaged_resource( ) );
	m_lobby_menu_ui->movie->SetBackgroundAlpha( 0.f );
	m_lobby_menu_ui->movie->SetPriority( 10 );

	m_message_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[6].get_unmanaged_resource( ) );
	m_message_ui->movie->SetBackgroundAlpha( 0.f );
	m_message_ui->movie->SetPriority( 12 );

	m_match_making_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[7].get_unmanaged_resource( ) );
	m_match_making_ui->movie->SetBackgroundAlpha( 0.5f );
	m_match_making_ui->movie->SetPriority( 15 );

	configs::binary_config_ptr player_config = static_cast_resource_ptr< configs::binary_config_ptr >( data[8].get_unmanaged_resource( ) );
	m_player_max_carried_weight = player_config->get_root( )["player"]["stamina_params"]["max_carried_weight"];

	m_lobby_menu_external_handler = VOSTOK_NEW_IMPL( ::survarium::g_allocator, lobby_menu_external_handler )( get_game( ) );
	m_lobby_menu_ui->movie->SetExternalInterface( m_lobby_menu_external_handler );
	m_match_making_ui->movie->SetExternalInterface( m_lobby_menu_external_handler );

	m_cursor_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_cursor_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );
	m_lobby_menu_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_lobby_menu_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );
	m_match_making_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_match_making_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );
	m_message_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_message_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );
	m_message_ui->movie->SetExternalInterface( m_lobby_menu_external_handler );

	flash_value proxy;
	m_lobby_menu_ui->movie->GetVariable( &proxy, "_root.player_profile" );
	m_relocate_item_func = VOSTOK_NEW_IMPL( ::survarium::g_allocator, relocate_item_func )( get_game( ) );
	flash_value func;
	m_lobby_menu_ui->movie->CreateFunction( &func, m_relocate_item_func );
	proxy.SetMember( "_relocateFunction", func );

	m_lobby_menu_ui->movie->Advance( 0.f, 0 );
	show_ui( true );

	flash_value players_count;
	players_count.SetUInt( 5 );
	m_lobby_menu_ui->movie->Invoke( "root.lobby_menu.set_max_players", NULL, &players_count, 1 );

	for ( vector< game_object_* >::iterator i = m_lobby_game_project->m_objects.begin( ),
		e = m_lobby_game_project->m_objects.end( );
		i != e;
		++i )
		( *i )->
			insert( );

	for ( u32 i = 0; i < m_lobby_game_project->m_render_visuals_count; ++i )
		m_lobby_game_project->m_render_visuals[i].insert( *this );

	for ( u32 i = 0; i < m_lobby_game_project->m_static_collision_objects_count; ++i )
		m_lobby_game_project->m_static_collision_objects[i].insert( get_physics_world( ) );

	float3 pos = m_lobby_game_project->m_config->get_root( )["camera"]["position"];
	float3 dir = m_lobby_game_project->m_config->get_root( )["camera"]["direction"];
	m_camera->set_position_direction( pos, dir );

	fill_items_dictionary( );
	fill_inventory_labels( );

	get_game( ).on_queried_by_network_client_scene_ready( login_scene_ready );
	show_disconnected_message( true );

}

ui_label lobby_labels[] =
{
	{ "shop_label", "st_shop_label" },
	{ "inventory_label", "st_inventory_label" },
	{ "friends_online", "st_friends_online" },
	{ "points_earned", "st_points_earned" },
	{ "points_aviable", "st_points_aviable" },
	{ "enter_search_player_name", "st_enter_search_player_name" },
	{ "enter_chat_message", "st_enter_chat_message" },
	{ "add_to_friends", "st_add_to_friends" },
	{ "add_to_ignore", "st_add_to_ignore" },
	{ "remove_from_friends", "st_remove_from_friends" },
	{ "remove_from_ignore", "st_remove_from_ignore" },
	{ "invite_to_squad", "st_invite_to_squad" },
	{ "ok", "st_ok" },
	{ "accept", "st_accept" },
	{ "confirm", "st_confirm" },
	{ "cancel", "st_cancel" },
	{ "reroll", "st_reroll" },
	{ "relocate", "st_relocate" },
	{ "character", "st_character" },
	{ "inventrory", "st_inventrory" },
	{ "shop", "st_shop" },
	{ "missions", "st_missions" },
	{ "clan", "st_clan" },
	{ "tournaments", "st_tournaments" },
	{ "settings", "st_settings" },
	{ "fix", "st_fix" },
	{ "wear", "st_wear" },
	{ "sell", "st_sell" },
	{ "buy", "st_buy" },
	{ "filter_all", "st_filter_all" },
	{ "filter_equipment", "st_filter_equipment" },
	{ "filter_weapons", "st_filter_weapons" },
	{ "filter_ammo", "st_filter_ammo" },
	{ "filter_items", "st_filter_items" },
	{ "filter_body_armor", "st_filter_body_armor" },
	{ "filter_helmet", "st_filter_helmet" },
	{ "filter_gloves", "st_filter_gloves" },
	{ "filter_boots", "st_filter_boots" },
	{ "filter_pants", "st_filter_pants" },
	{ "filter_mask", "st_filter_mask" },
	{ "filter_rig", "st_filter_rig" },
	{ "filter_pistol", "st_filter_pistol" },
	{ "filter_submachine_gun", "st_filter_submachine_gun" },
	{ "filter_assault_rifle", "st_filter_assault_rifle" },
	{ "filter_machine_gun", "st_filter_machine_gun" },
	{ "filter_shotgun", "st_filter_shotgun" },
	{ "filter_sniper_rifle", "st_filter_sniper_rifle" },
	{ "filter_ammo_pistol", "st_filter_ammo_pistol" },
	{ "filter_ammo_submachine_gun", "st_filter_ammo_submachine_gun" },
	{ "filter_ammo_assault_rifle", "st_filter_ammo_assault_rifle" },
	{ "filter_ammo_machine_gun", "st_filter_ammo_machine_gun" },
	{ "filter_ammo_shotgun", "st_filter_ammo_shotgun" },
	{ "filter_ammo_sniper_rifle", "st_filter_ammo_sniper_rifle" },
	{ "filter_medkit", "st_filter_medkit" },
	{ "filter_mine", "st_filter_mine" },
	{ "filter_grenade", "st_filter_grenade" },
	{ "filter_device", "st_filter_device" },
	{ "paperdoll_btn_ammo", "st_paperdoll_btn_ammo" },
	{ "paperdoll_btn_fix", "st_paperdoll_btn_fix" },
	{ "paperdoll_btn_weight", "st_paperdoll_btn_weight" },
	{ "support", "st_support" },
	{ "add", "st_add" },
	{ "item_piece_postfix", "st_item_piece_postfix" },
	{ "friend_state_lobby", "st_friend_state_lobby" },
	{ "friend_state_game", "st_friend_state_game" },
	{ "friend_state_offline", "st_friend_state_offline" },
	{ "friend_state_away", "st_friend_state_away" },
	{ "message_reroll_text", "st_message_reroll_text" },
	{ "refill_ammo_btn", "st_refill_ammo_btn" },
	{ "refill_ammo_label", "st_refill_ammo_label" },
	{ "backpack", "st_backpack" },
	{ "cost", "st_cost" },
	{ "storage", "st_storage" },
};

void lobby_menu::fill_inventory_labels( )
{
	flash_value labels;
	m_lobby_menu_ui->movie->CreateObject( &labels );

	for ( u32 i = 0; i < 0x49; ++i )
	{
		wchar_t label_w[512];
		get_game( ).text_translator( ).translate_text( lobby_labels[ i ].label, label_w );

		flash_value label_translate;
		label_translate.SetStringW		( label_w );

		labels.SetMember				( lobby_labels[ i ].name, label_translate );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.set_localization_data", NULL, &labels, 1 );
}

void lobby_menu::fill_items_dictionary( )
{
	map< u32, dictionary_item > const& itm_dict = get_game( ).items_dictionary( ).get_dictionary( );
	map< u32, dictionary_item >::const_iterator itm_it = itm_dict.begin( );

	flash_value items_descr_array;
	flash_value inventory_item_property;
	flash_value traders_array;
	m_lobby_menu_ui->movie->CreateArray( &items_descr_array );

	u32 in_array_index = 0;
	u32 j;
	for ( ; itm_it != itm_dict.end( ); ++itm_it )
	{
		dictionary_item current_item = itm_it->second;
		u32 current_item_dict_id = current_item.item_id;
		u8 current_item_category_id = current_item.item_category;
		bool current_item_is_stack = current_item.is_stack;

		if ( !current_item.item_cfg->get_root( ).value_exists( "ui_desc" ) )
		{
			LOG_WARNING( "There is no ui_desc info for [%s]", current_item.item_cfg_name );
			continue;
		}

		u32 icon = current_item.item_cfg->get_root( )["ui_desc"]["icon"];

		wchar_t item_name[512];
		get_game( ).text_translator( ).translate_text(
			current_item.item_cfg->get_root( )["ui_desc"]["text_descriptions"]["name"],
			item_name
		);

		wchar_t item_desc[512];
		get_game( ).text_translator( ).translate_text(
			current_item.item_cfg->get_root( )["ui_desc"]["text_descriptions"]["description"],
			item_desc
		);

		flash_value inventory_item_descr;
		m_lobby_menu_ui->movie->CreateObject( &inventory_item_descr );

		inventory_item_property.SetUInt( current_item_dict_id );
		inventory_item_descr.SetMember( "dictId", inventory_item_property );

		inventory_item_property.SetStringW( item_name );
		inventory_item_descr.SetMember( "name", inventory_item_property );

		inventory_item_property.SetStringW( item_desc );
		inventory_item_descr.SetMember( "descr", inventory_item_property );

		inventory_item_property.SetUInt( current_item_category_id );
		inventory_item_descr.SetMember( "category", inventory_item_property );

		inventory_item_property.SetUInt( icon );
		inventory_item_descr.SetMember( "icon", inventory_item_property );

		float item_weight;
		if ( current_item.is_ammo( ) )
		{
			inventory_item_property.SetUInt( current_item.item_cfg->get_root( )["parameters"]["clip_size"] );
			inventory_item_descr.SetMember( "clip_size", inventory_item_property );
			item_weight = current_item.item_cfg->get_root( )["parameters"]["clip_weight"];
		}
		else
			item_weight = current_item.item_cfg->get_root( )["parameters"]["weight"];

		inventory_item_property.SetNumber( item_weight );
		inventory_item_descr.SetMember( "weight", inventory_item_property );

		inventory_item_property.SetBoolean( current_item_is_stack );
		inventory_item_descr.SetMember( "is_stack", inventory_item_property );

		flash_value inventory_item_propertyies_array;
		m_lobby_menu_ui->movie->CreateArray( &inventory_item_propertyies_array );

		configs::binary_config_value const* it = current_item.item_cfg->get_root( )["ui_desc"]["props_list"].begin( );
		configs::binary_config_value const* it_end = current_item.item_cfg->get_root( )["ui_desc"]["props_list"].end( );
		flash_value item_property_member;
		for ( j = 0; it != it_end; ++it, ++j )
		{
			flash_value item_property;
			m_lobby_menu_ui->movie->CreateObject( &item_property );

			wchar_t prop_name[512];
			get_game( ).text_translator( ).translate_text( (*it)["prop_name"], prop_name );

			u32 prop_icon = it->value_exists( "prop_icon" ) ? (*it)["prop_icon"] : 0;

			item_property_member.SetUInt( (*it)["prop_value"] );
			item_property.SetMember( "prop_value", item_property_member );

			item_property_member.SetStringW( prop_name );
			item_property.SetMember( "prop_name", item_property_member );

			item_property_member.SetUInt( prop_icon );
			item_property.SetMember( "prop_icon", item_property_member );

			inventory_item_propertyies_array.SetElement( j, item_property );
		}

		inventory_item_descr.SetMember( "item_properties", inventory_item_propertyies_array );
		items_descr_array.SetElement( in_array_index, inventory_item_descr );
		++in_array_index;
	}

	m_lobby_menu_ui->movie->Invoke( "root._itemDescriptor.setItemsDictionary", NULL, &items_descr_array, 1 );

	m_lobby_menu_ui->movie->CreateArray( &traders_array );

	fixed_string< 32 > sellers_names[6];
	sellers_names[0] = "st_scavengers_faction";
	sellers_names[1] = "st_black_market_faction";
	sellers_names[2] = "st_renaissance_faction";
	sellers_names[3] = "st_border_faction";
	sellers_names[4] = "st_scientists_faction";
	sellers_names[5] = "st_mercenaries_faction";

	flash_value traders_array_item_property;
	j = 0;
	for ( ; j < 6; ++j )
	{
		flash_value traders_array_item;
		m_lobby_menu_ui->movie->CreateObject( &traders_array_item );

		wchar_t faction_name[512];
		get_game( ).text_translator( ).translate_text( sellers_names[j].c_str( ), faction_name );

		traders_array_item_property.SetStringW( faction_name );
		traders_array_item.SetMember( "name", traders_array_item_property );

		traders_array_item_property.SetUInt( j + 1 );
		traders_array_item.SetMember( "faction_id", traders_array_item_property );

		traders_array.SetElement( j, traders_array_item );
	}

	m_lobby_menu_ui->movie->Invoke( "root.shop_list.fillSellers", NULL, &traders_array, 1 );
}

void lobby_menu::fill_inventory_contents( )
{
	vectora< inventory_item_instance >& inventory_items = lobby_client( ).inventory_item_instances( ); inventory_item_instance const* it = inventory_items.begin( ), *it_e = inventory_items.end( );

	flash_value inventory_array;
	m_lobby_menu_ui->movie->CreateArray	( &inventory_array );
	flash_value inventory_item_property;

	u32 i = 0;
	for ( ; it != it_e; ++it )
	{
		flash_value inventory_item;
		m_lobby_menu_ui->movie->CreateObject( &inventory_item );

		u32 const item_id				= it->id;
		u32 const item_dict_id			= it->dict_id;
		u32 const condition_or_stack	= it->condition_or_stack;

		inventory_item_property.SetInt	( item_id );
		inventory_item.SetMember		( "id", inventory_item_property );

		inventory_item_property.SetInt	( item_dict_id );
		inventory_item.SetMember		( "dictId", inventory_item_property );

		inventory_item_property.SetUInt	( condition_or_stack );
		inventory_item.SetMember		( "condition", inventory_item_property );

		inventory_item_property.SetUInt	( condition_or_stack );
		inventory_item.SetMember		( "condition_or_stack", inventory_item_property );

		inventory_array.SetElement		( i, inventory_item );
		++i;
	}

	m_lobby_menu_ui->movie->Invoke		( "root.inventory_list.setupInventoryData", NULL, &inventory_array, 1 );
}

// sushi@TODO: Resolve the element-reference/construction boundary; retail captures the element before CreateObject.
void lobby_menu::on_items_compatibility_arrived( )
{
	class lobby_client& client			= lobby_client( );

	flash_value slot_restrictions_array;
	m_lobby_menu_ui->movie->CreateArray	( &slot_restrictions_array );

	flash_value items_compatibility_item_property;
	for ( u8 i = 0; i < client.item_compatibilities_count( ); ++i )
	{
		flash_value items_compatibility_item;
		m_lobby_menu_ui->movie->CreateObject( &items_compatibility_item );

		items_compatibility_item_property.SetUInt	( client.get_items_compatibility( i ).first_item_dict_id );
		items_compatibility_item.SetMember			( "first_item_dict_id", items_compatibility_item_property );

		items_compatibility_item_property.SetUInt	( client.get_items_compatibility( i ).second_item_dict_id );
		items_compatibility_item.SetMember			( "second_item_dict_id", items_compatibility_item_property );

		slot_restrictions_array.SetElement			( i, items_compatibility_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.player_profile.profileItems.setItemsCompatibility", NULL, &slot_restrictions_array, 1 );
}

// sushi@TODO: Resolve the element-reference/construction boundary; retail captures the element before CreateObject.
void lobby_menu::on_slot_restrictions_arrived( )
{
	class lobby_client& client			= lobby_client( );

	flash_value slot_restrictions_array;
	m_lobby_menu_ui->movie->CreateArray	( &slot_restrictions_array );

	flash_value slot_restriction_item_property;
	for ( u8 i = 0; i < client.slot_restrictions_count( ); ++i )
	{
		flash_value slot_restriction_item;
		m_lobby_menu_ui->movie->CreateObject( &slot_restriction_item );

		slot_restriction_item_property.SetUInt	( client.slot_restriction( i ).slot_dict_id );
		slot_restriction_item.SetMember			( "slot_id", slot_restriction_item_property );

		slot_restriction_item_property.SetUInt	( client.slot_restriction( i ).category_dict_id );
		slot_restriction_item.SetMember			( "category_id", slot_restriction_item_property );

		slot_restrictions_array.SetElement		( i, slot_restriction_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.player_profile.profileItems.setSlotsRestrictions", NULL, &slot_restrictions_array, 1 );
}

void lobby_menu::fill_profiles( )
{
	flash_value profiles_array;
	class lobby_client& client			= lobby_client( );
	m_lobby_menu_ui->movie->CreateArray	( &profiles_array );

	u8 const profiles_count				= client.profiles_count( );
	flash_value profile_item_property;
	for ( u8 i = 0; i < profiles_count; ++i )
	{
		flash_value profile_item;
		m_lobby_menu_ui->movie->CreateObject( &profile_item );

		wchar_t profile_name_w[512];
		size_t converted_chars_count		= 0;
		mbstowcs_s						( &converted_chars_count, profile_name_w, 512, client.profile( i ).profile_name, _TRUNCATE );

		profile_item_property.SetStringW( profile_name_w );
		profile_item.SetMember			( "name", profile_item_property );

		profile_item_property.SetInt	( 1 );
		profile_item.SetMember			( "icon", profile_item_property );

		profiles_array.SetElement		( i, profile_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.player_profile.setupProfiles", NULL, &profiles_array, 1 );
}

void lobby_menu::on_profile_changed( u8 profile_id )
{
	m_selected_profile = profile_id;

	class lobby_client& client = lobby_client( );
	player_profile const& profile = client.profile( profile_id );
	m_character->profile_changed( &profile );

	flash_value profile_items_array;
	m_lobby_menu_ui->movie->CreateArray( &profile_items_array );

	fixed_string< 260 > player_parameters_query_path;
	flash_value slot_item_property;
	u8 in_array_index = 0;
	for ( u32 i = 0; i != max_slots_count; ++i )
	{
		if ( profile.slots[i].item.id == 0 )
			continue;

		u32 condition_or_stack = profile.slots[i].item.condition_or_stack;
		player_parameters_query_path.appendf( "%d_", profile.slots[i].item.id );

		flash_value slot_item;
		m_lobby_menu_ui->movie->CreateObject( &slot_item );

		slot_item_property.SetInt( profile.slots[i].item.id );
		slot_item.SetMember( "itemId", slot_item_property );

		slot_item_property.SetInt( i );
		slot_item.SetMember( "slotId", slot_item_property );

		slot_item_property.SetInt( condition_or_stack );
		slot_item.SetMember( "condition_or_stack", slot_item_property );

		profile_items_array.SetElement( in_array_index, slot_item );
		++in_array_index;
	}

	flash_value profile_descriptor;
	m_lobby_menu_ui->movie->CreateObject( &profile_descriptor );
	profile_descriptor.SetMember( "items", profile_items_array );

	flash_value profile_descriptor_property;
	profile_descriptor_property.SetString( profile.profile_name );
	profile_descriptor.SetMember( "name", profile_descriptor_property );

	flash_value v;
	v.SetUInt( profile.profile_id );
	profile_descriptor.SetMember( "profileId", v );

	profile_descriptor_property.SetUInt( 1 );
	profile_descriptor.SetMember( "icon", profile_descriptor_property );

	m_lobby_menu_ui->movie->Invoke( "root.player_profile.fillProfileItems", NULL, &profile_descriptor, 1 );

	player_parameters_cooker_data* cook_data = VOSTOK_NEW_IMPL( ::survarium::g_allocator, player_parameters_cooker_data );
	cook_data->dictionary = &get_game( ).items_dictionary( );
	cook_data->profile = &profile;

	variant< 32 > user_data;
	user_data.set( cook_data );

	resources::query_resource(
		player_parameters_query_path.c_str( ),
		resources::player_parameters_class,
		boost::bind( &lobby_menu::player_parameters_ready, this, _1, cook_data ),
		g_allocator,
		&user_data
	);
}

void lobby_menu::player_parameters_ready( resources::queries_result& data, player_parameters_cooker_data* cook_data )
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, cook_data );

	player_parameters_modifyer_ptr player_parameters =
		static_cast_resource_ptr< player_parameters_modifyer_ptr >( data[0].get_unmanaged_resource( ) );
	m_player_total_items_weight = player_parameters->total_items_weight;

	flash_value args[2];
	args[0].SetUInt( player_parameters->additional_artefact_slots );
	args[1].SetUInt( player_parameters->additional_devices_slots );
	m_lobby_menu_ui->movie->Invoke( "root.player_profile.updateSlots", NULL, args, 2 );

	args[0].SetNumber( player_parameters->total_items_weight );
	args[1].SetNumber( m_player_max_carried_weight );
	m_lobby_menu_ui->movie->Invoke( "root.player_profile.updateWeight", NULL, args, 2 );
}

void lobby_menu::on_profile_arrived( u8 profile_id )
{
	if ( m_selected_profile == profile_id )
	{
		flash_value profile_id_value;
		profile_id_value.SetUInt		( profile_id );
		m_lobby_menu_ui->movie->Invoke	( "_root.player_profile.selectProfile", NULL, &profile_id_value, 1 );
	}
}

void lobby_menu::on_price_items_arrived( u8 trader_id )
{
	class lobby_client& lobby_client = this->lobby_client( );

	char faction_str[32];
	sprintf_s( faction_str, "faction_%d", trader_id );
	configs::binary_config_value const& faction_levels = get_game( ).items_dictionary( ).dict_config->get_root( )["factions_dict"][faction_str]["levels"];
	u8 levels_count = faction_levels.size( );

	for ( u8 current_reputation_level = 0; current_reputation_level < levels_count; ++current_reputation_level )
	{
		pcstr faction_level_name = faction_levels[current_reputation_level]["name"];

		flash_value current_level[5];
		current_level[0].SetUInt( trader_id );
		m_lobby_menu_ui->movie->CreateArray( &current_level[1] );
		current_level[2].SetUInt( current_reputation_level );

		wchar_t faction_level_name_w[512];
		get_game( ).text_translator( ).translate_text( faction_level_name, faction_level_name_w );
		current_level[3].SetStringW( faction_level_name_w );
		current_level[4].SetUInt( faction_levels[current_reputation_level]["value"] );

		flash_value price_item_property;
		for ( u32 i = 0; i < lobby_client.price( trader_id ).count; ++i )
		{
			price_item const& item = lobby_client.price( trader_id ).items[i];
			if ( item.reputation_level != current_reputation_level )
				continue;

			flash_value prices_array_item;
			m_lobby_menu_ui->movie->CreateObject( &prices_array_item );

			price_item_property.SetUInt( item.item_dict_id );
			prices_array_item.SetMember( "dictId", price_item_property );

			price_item_property.SetUInt( 10 );
			prices_array_item.SetMember( "count", price_item_property );

			price_item_property.SetUInt( item.cost );
			prices_array_item.SetMember( "cost", price_item_property );

			current_level[1].PushBack( prices_array_item );
		}

		m_lobby_menu_ui->movie->Invoke( "root.setup_shop_data", NULL, current_level, 5 );
	}
}

void lobby_menu::reset_account_money( )
{
	flash_value account_info;
	m_lobby_menu_ui->movie->CreateObject( &account_info );

	wchar_t an[256];
	mbstowcs_s						( NULL, an, 256, lobby_client( ).account_nickname_, _TRUNCATE );

	flash_value account_info_property;
	account_info_property.SetStringW( an );
	account_info.SetMember			( "nickname", account_info_property );

	account_info_property.SetUInt	( lobby_client( ).get_account_money( ).generic_money );
	account_info.SetMember			( "generic_money", account_info_property );

	account_info_property.SetUInt	( lobby_client( ).get_account_money( ).premium_money );
	account_info.SetMember			( "premium_money", account_info_property );

	m_lobby_menu_ui->movie->Invoke	( "root.setPlayerInfo", NULL, &account_info, 1 );
}

void lobby_menu::on_shop_ui_ready( )
{
	for ( u32 trader_id = 1; trader_id <= 3; ++trader_id )
		lobby_client( ).query_prices( trader_id );
}

void lobby_menu::fill_skills_tree( )
{
	configs::binary_config_value const& tree = lobby_client( ).skills_tree_config( )->get_root( );
	configs::binary_config_value const& db_dictionaries = get_game( ).items_dictionary( ).dict_config->get_root( );

	flash_value skills_tree_value_prop;
	for ( u32 skill_branch = 1; skill_branch <= 5; ++skill_branch )
	{
		char skill_branch_name[32];
		sprintf_s( skill_branch_name, "skill_%d", skill_branch );

		configs::binary_config_value const& current_skill_value = tree[skill_branch_name];
		u8 skill_id = current_skill_value["id"];

		flash_value skills_tree_value;
		m_lobby_menu_ui->movie->CreateObject( &skills_tree_value );

		char skill_name[32];
		sprintf_s( skill_name, "skill_%d", skill_id );

		wchar_t branch_name[512];
		get_game( ).text_translator( ).translate_text(
			db_dictionaries["skills_dict"][skill_name]["skill_name"],
			branch_name
		);
		skills_tree_value_prop.SetStringW( branch_name );
		skills_tree_value.SetMember( "name", skills_tree_value_prop );

		wchar_t branch_descr[512];
		get_game( ).text_translator( ).translate_text(
			db_dictionaries["skills_dict"][skill_name]["skill_description"],
			branch_descr
		);
		skills_tree_value_prop.SetStringW( branch_descr );
		skills_tree_value.SetMember( "description", skills_tree_value_prop );

		skills_tree_value_prop.SetUInt( db_dictionaries["skills_dict"][skill_name]["skill_icon"] );
		skills_tree_value.SetMember( "color", skills_tree_value_prop );

		skills_tree_value_prop.SetUInt( skill_id );
		skills_tree_value.SetMember( "id", skills_tree_value_prop );

		skills_tree_value_prop.SetUInt( 0 );
		skills_tree_value.SetMember( "opened", skills_tree_value_prop );

		m_lobby_menu_ui->movie->CreateArray( &skills_tree_value_prop );

		u32 levels_count = current_skill_value["levels"].size( );
		for ( u32 i = 1; i <= levels_count; ++i )
		{
			char skill_level_table_name[32];
			sprintf_s( skill_level_table_name, "skill_level_%d", i );

			configs::binary_config_value const& current_level_cfg = current_skill_value["levels"][skill_level_table_name];

			flash_value skills_tree_level_value;
			m_lobby_menu_ui->movie->CreateObject( &skills_tree_level_value );

			flash_value skills_tree_level_value_prop;
			skills_tree_level_value_prop.SetStringW( branch_name );
			skills_tree_level_value.SetMember( "name", skills_tree_level_value_prop );

			skills_tree_level_value_prop.SetBoolean( current_level_cfg.value_exists( "perks" ) );
			skills_tree_level_value.SetMember( "power", skills_tree_level_value_prop );

			m_lobby_menu_ui->movie->CreateArray( &skills_tree_level_value_prop );

			configs::binary_config_value const* booster_it = current_level_cfg["boosters"].begin( );
			configs::binary_config_value const* booster_it_e = current_level_cfg["boosters"].end( );
			flash_value item_property_member;
			u32 booster_index = 0;
			for ( ; booster_it != booster_it_e; ++booster_it, ++booster_index )
			{
				flash_value item_property;
				m_lobby_menu_ui->movie->CreateObject( &item_property );

				float prop_value = (*booster_it)["value"];

				char booster_table_name[32];
				sprintf_s( booster_table_name, "booster_%d", (u32)(*booster_it)["id"] );

				wchar_t prop_name[512];
				get_game( ).text_translator( ).translate_text(
					db_dictionaries["boosters_dict"][booster_table_name]["booster_name"],
					prop_name
				);

				char prop_value_str[32];
				sprintf_s( prop_value_str, "%f", prop_value );

				item_property_member.SetString( prop_value_str );
				item_property.SetMember( "prop_value", item_property_member );

				item_property_member.SetStringW( prop_name );
				item_property.SetMember( "prop_name", item_property_member );

				item_property_member.SetUInt( db_dictionaries["boosters_dict"][booster_table_name]["booster_icon"] );
				item_property.SetMember( "prop_icon", item_property_member );

				skills_tree_level_value_prop.SetElement( booster_index, item_property );
			}

			skills_tree_level_value.SetMember( "properties", skills_tree_level_value_prop );

			if ( current_level_cfg.value_exists( "perks" ) )
			{
				m_lobby_menu_ui->movie->CreateArray( &skills_tree_level_value_prop );

				configs::binary_config_value const* perk_it = current_level_cfg["perks"].begin( );
				configs::binary_config_value const* perk_it_e = current_level_cfg["perks"].end( );
				flash_value perk_property_member;
				u32 perk_index = 0;
				for ( ; perk_it != perk_it_e; ++perk_it, ++perk_index )
				{
					flash_value perk_property;
					m_lobby_menu_ui->movie->CreateObject( &perk_property );

					char perk_table_name[32];
					sprintf_s( perk_table_name, "perk_%d", (u32)(*perk_it)["id"] );

					wchar_t perk_name[512];
					get_game( ).text_translator( ).translate_text(
						db_dictionaries["perks_dict"][perk_table_name]["name"],
						perk_name
					);

					wchar_t perk_descr[512];
					get_game( ).text_translator( ).translate_text(
						db_dictionaries["perks_dict"][perk_table_name]["description"],
						perk_descr
					);

					perk_property_member.SetStringW( perk_name );
					perk_property.SetMember( "name", perk_property_member );

					perk_property_member.SetStringW( perk_descr );
					perk_property.SetMember( "item_description", perk_property_member );

					perk_property_member.SetUInt( (*perk_it)["id"] );
					perk_property.SetMember( "id", perk_property_member );

					skills_tree_level_value_prop.SetElement( perk_index, perk_property );
				}

				skills_tree_level_value.SetMember( "perks", skills_tree_level_value_prop );
			}

			skills_tree_value_prop.SetElement( i - 1, skills_tree_level_value );
		}

		skills_tree_value.SetMember( "slots", skills_tree_value_prop );
		m_lobby_menu_ui->movie->Invoke( "root.create_perk_tree", NULL, &skills_tree_value, 1 );
	}

}

// The target keeps the Scaleform value construction and destruction inline here.
void lobby_menu::fill_character_data( )
{
	flash_value char_info_value;
	m_lobby_menu_ui->movie->CreateObject( &char_info_value );

	flash_value player_skills_value;
	m_lobby_menu_ui->movie->CreateArray( &player_skills_value );

	flash_value player_skills_value_prop;
	u8 total_points_in_tree = 0;
	flash_value skill_value_prop;

	for ( u8 i = 0; i < lobby_client( ).player_skills_count( ); ++i )
	{
		player_skill const& current_skill = lobby_client( ).player_skill( i );

		flash_value skill_value;
		m_lobby_menu_ui->movie->CreateObject( &skill_value );

		skill_value_prop.SetUInt		( current_skill.skill_id );
		skill_value.SetMember			( "id", skill_value_prop );

		skill_value_prop.SetUInt		( current_skill.skill_points );
		skill_value.SetMember			( "points", skill_value_prop );

		player_skills_value.SetElement	( i, skill_value );
		total_points_in_tree				+= current_skill.skill_points;
	}
	char_info_value.SetMember			( "trees", player_skills_value );

	player_skills_value_prop.SetUInt	( lobby_client( ).get_player_leveling( ).total_skill_points - total_points_in_tree );
	char_info_value.SetMember			( "points_available", player_skills_value_prop );

	player_skills_value_prop.SetUInt	( lobby_client( ).get_player_leveling( ).total_skill_points );
	char_info_value.SetMember			( "points_unlocked", player_skills_value_prop );

	player_skills_value_prop.SetUInt	( lobby_client( ).get_player_leveling( ).total_experience - lobby_client( ).get_player_leveling( ).prev_level_experience );
	char_info_value.SetMember			( "experience_current", player_skills_value_prop );

	u32 const next_level_experience = lobby_client( ).get_player_leveling( ).next_level_experience;
	u32 const prev_level_experience = lobby_client( ).get_player_leveling( ).prev_level_experience;
	player_skills_value_prop.SetUInt	( next_level_experience > prev_level_experience ? next_level_experience - prev_level_experience : 0 );
	char_info_value.SetMember			( "experience_next_level", player_skills_value_prop );

	player_skills_value_prop.SetUInt	( m_match_stats.last_match_exp_delta );
	char_info_value.SetMember			( "experience_delta", player_skills_value_prop );

	m_lobby_menu_ui->movie->CreateArray( &player_skills_value_prop );

	for ( u8 i = 0; i < lobby_client( ).player_perks_count( ); ++i )
	{
		flash_value perk_value;
		perk_value.SetUInt				( lobby_client( ).player_perk( i ) );
		player_skills_value_prop.SetElement( i, perk_value );
	}
	char_info_value.SetMember			( "perks", player_skills_value_prop );

	m_lobby_menu_ui->movie->Invoke		( "root.fill_char_info", NULL, &char_info_value, 1 );
}

void lobby_menu::fill_service_prices( )
{
	flash_value reroll_cost_value;
	reroll_cost_value.SetUInt		( lobby_client( ).get_service_prices( ).reroll_cost );
	m_lobby_menu_ui->movie->Invoke	( "root.set_reroll_cost", NULL, &reroll_cost_value, 1 );
}

void lobby_menu::fill_friend_list( )
{
	vectora< account_list_item > const& players_list = messaging_client( ).get_friend_list( );

	flash_value array_value;
	m_lobby_menu_ui->movie->CreateArray	( &array_value );
	flash_value value;

	for ( u32 i = 0;
			i < players_list.size( ); ++i )
	{
		wchar_t player_name_w[512];
		flash_value list_item;
		m_lobby_menu_ui->movie->CreateObject( &list_item );

		value.SetUInt					( players_list[ i ].account_id );
		list_item.SetMember				( "id", value );

		mbstowcs_s						( NULL, player_name_w, 512, players_list[ i ].account_name.c_str( ), _TRUNCATE );
		value.SetStringW				( player_name_w );
		list_item.SetMember				( "name", value );

		value.SetUInt					( 3 );
		list_item.SetMember				( "icon", value );

		value.SetUInt					( players_list[ i ].online ? 0 : 2 );
		list_item.SetMember				( "status", value );

		array_value.SetElement			( i, list_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.set_friends_list", NULL, &array_value, 1 );
}

void lobby_menu::fill_ignore_list( )
{
	vectora< account_list_item > const& players_list = messaging_client( ).get_ignore_list( );

	flash_value array_value;
	m_lobby_menu_ui->movie->CreateArray	( &array_value );
	flash_value value;

	for ( u32 i = 0;
			i < players_list.size( ); ++i )
	{
		flash_value list_item;
		m_lobby_menu_ui->movie->CreateObject( &list_item );

		value.SetUInt					( players_list[ i ].account_id );
		list_item.SetMember				( "id", value );

		value.SetString					( players_list[ i ].account_name.c_str( ) );
		list_item.SetMember				( "name", value );

		value.SetUInt					( 3 );
		list_item.SetMember				( "icon", value );

		array_value.SetElement			( i, list_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.set_ignored_list", NULL, &array_value, 1 );
}

void lobby_menu::fill_found_players( )
{
	vectora< account_list_item > const& players_list = messaging_client( ).get_found_players_list( );

	flash_value array_value;
	m_lobby_menu_ui->movie->CreateArray	( &array_value );
	flash_value value;

	const u32 count = players_list.size( );
	for ( u32 i = 0;
			i < count; ++i )
	{
		flash_value list_item;
		m_lobby_menu_ui->movie->CreateObject( &list_item );

		value.SetUInt					( players_list[ i ].account_id );
		list_item.SetMember				( "id", value );

		value.SetString					( players_list[ i ].account_name.c_str( ) );
		list_item.SetMember				( "name", value );

		array_value.SetElement			( i, list_item );
	}

	m_lobby_menu_ui->movie->Invoke		( "root.fill_players_search", NULL, &array_value, 1 );
}

void lobby_menu::set_cursor( u8 id )
{
	flash_value c_id;
	c_id.SetUInt					( id );
	m_cursor_ui->movie->Invoke		( "root.setCursor", NULL, &c_id, 1 );
}

void lobby_menu::set_fps_stats( float fps )
{
	flash_value f_val;
	f_val.SetUInt					( ( u16 )fps );
	m_lobby_menu_ui->movie->Invoke	( "root.set_fps", NULL, &f_val, 1 );
}

bool lobby_menu::is_mouse_over_ui( )
{
	flash_value is_mouse_over_val;
	m_lobby_menu_ui->movie->Invoke					( "root.get_mouse_over", &is_mouse_over_val, NULL, 0 );
	bool is_over_lobby = is_mouse_over_val.GetBool	( );
	get_game( ).get_chat_handler( ).get_movie( )->movie->Invoke( "root.get_mouse_over", &is_mouse_over_val, NULL, 0 );
	return is_over_lobby || is_mouse_over_val.GetBool( );
}

// claude@NOTE: flash glue now inlines at /Ox (scaleform Master Gold /GL); structure
// (the three independent message blocks) is faithful and the residual is LTCG
// scheduling of the inlined flash_value ctor/dtor + SetStringW/SetMember/SetUInt, not
// the old inline-vs-call wall.
static wchar_t const player_joined_pref[]	= L"#+p:[ ";
static wchar_t const player_left_pref[]		= L"#-p:[ ";
static wchar_t const player_queue_pref[]	= L"#q:[";
static wchar_t const player_team_pref[]		= L"#t:[";
static wchar_t const player_id_pref[]		= L"Player [ ";
static wchar_t const player_exp_pref[]		= L"#e:[";
static wchar_t const match_count_pref[]		= L"#mc:[";
static wchar_t const player_count_pref[]	= L"#pc:[";

void lobby_menu::on_match_message_arrived( wchar_t const* w_text )
{
	game_team_id team;
	wchar_t const* player_joined_message	= wcsstr( w_text, player_joined_pref );
	wchar_t const* player_left_message		= wcsstr( w_text, player_left_pref );
	wchar_t const* queue_state_message		= wcsstr( w_text, player_queue_pref );

	wchar_t w_player_team[8];
	wchar_t w_player_in_queue[16];
	wchar_t w_player_name[32];

	if ( player_joined_message )
	{
		wchar_t const* const player_name_end = wcsstr( player_joined_message, L" ]" );
		wcsncpy_s			( w_player_name, player_joined_message + 6, player_name_end - player_joined_message - 6 );

		wchar_t const* player_team = wcsstr( w_text, player_team_pref );
		wchar_t const* const player_team_end = wcsstr( player_team, L"]" );
		wcsncpy_s			( w_player_team, player_team + 4, player_team_end - player_team - 4 );

		team = ( game_team_id )_wtoi( w_player_team );

		flash_value add_player_args[2];
		m_match_making_ui->movie->CreateObject( &add_player_args[1] );

		flash_value player_member_value;
		player_member_value.SetStringW	( w_player_name );
		add_player_args[1].SetMember( "name", player_member_value );

		player_member_value.SetUInt	( 0 );
		add_player_args[1].SetMember( "icon", player_member_value );

		add_player_args[0].SetUInt	( team );
		m_match_making_ui->movie->Invoke( "root.add_player", NULL, add_player_args, 2 );
	}

	if ( player_left_message )
	{
		wchar_t const* const player_left_end = wcsstr( player_left_message, L" ]" );
		wcsncpy_s			( w_player_name, player_left_message + 6, player_left_end - player_left_message - 6 );

		flash_value player_name_val;
		player_name_val.SetStringW	( w_player_name );
		m_match_making_ui->movie->Invoke( "root.remove_player", NULL, &player_name_val, 1 );
	}

	if ( queue_state_message )
	{
		wchar_t const* const queue_state_end = wcsstr( queue_state_message, L"]" );
		wcsncpy_s			( w_player_in_queue, queue_state_message + 4, queue_state_end - queue_state_message - 4 );

		flash_value players_in_queue_val;
		players_in_queue_val.SetStringW	( w_player_in_queue );
		m_match_making_ui->movie->Invoke( "root.set_place", NULL, &players_in_queue_val, 1 );
	}
}

void lobby_menu::on_stats_message_arrived(
	wchar_t const*						w_text,
	wchar_t const*						w_sender_name,
	messaging::message_channel_enum		message_channel
)
{
	wchar_t const* player_id	= wcsstr( w_text, player_id_pref );
	wchar_t const* player_exp	= wcsstr( w_text, player_exp_pref );
	wcsstr( w_text, match_count_pref );
	wchar_t const* player_count	= wcsstr( w_text, player_count_pref );

	if ( player_id )
	{
		wchar_t w_player_id[32];
		wchar_t const* const player_id_end = wcsstr( player_id, L" ]" );
		wcsncpy_s			( w_player_id, player_id + 9, player_id_end - player_id - 9 );

		char player_name[32];
		size_t converted_chars_count = 0;
		wcstombs_s			( &converted_chars_count, player_name, 32, w_player_id, _TRUNCATE );

		if ( strings::equal( player_name, lobby_client( ).account_nickname_ ) )
		{
			get_game( ).get_chat_handler( ).add_message( message_channel, w_text, w_sender_name );

			if ( player_exp )
			{
				wchar_t w_player_exp[32];
				wchar_t const* const player_exp_end = wcsstr( player_exp, L"]" );
				wcsncpy_s	( w_player_exp, player_exp + 4, player_exp_end - player_exp - 4 );

				m_match_stats.last_match_exp_delta = _wtoi( w_player_exp );

				if ( lobby_client( ).net_connected( ) )
				{
					lobby_client( ).query_client_status( lobby::q_account_money );	// 7
					lobby_client( ).query_client_status( lobby::q_player_skills );	// 8
				}
			}

			get_game( ).get_chat_handler( ).add_message( message_channel, w_text, w_sender_name );
		}
	}
	else if ( player_count )
	{
		wchar_t w_player_count[8];
		wchar_t const* const player_count_end = wcsstr( player_count, L"]" );
		wcsncpy_s			( w_player_count, player_count + 5, player_count_end - player_count - 5 );

		flash_value player_count_val;
		player_count_val.SetStringW		( w_player_count );
		m_lobby_menu_ui->movie->Invoke	( "root.set_games_online", NULL, &player_count_val, 1 );
	}
}

void lobby_menu::show_disconnected_message( bool b_show )
{
	if ( b_show )
	{
		flash_value v[4];
		v[0].SetUInt					( 13 );
		v[1].SetString					( "noclose" );

		wchar_t message_txt[512];
		get_game( ).text_translator( ).translate_text( "st_disconnected_from_lobby", message_txt );
		v[2].SetStringW					( message_txt );
		v[3].SetBoolean					( true );

		m_message_ui->movie->Invoke		( "root.showMessage", NULL, v, 4 );
	}
	else
	{
		flash_value window_id;
		window_id.SetUInt				( 13 );
		m_message_ui->movie->Invoke		( "root.close", NULL, &window_id, 1 );
	}
}

void lobby_menu::on_player_reputations_arrived( )
{
	for ( u8 reputation_id = 0; reputation_id < lobby_client( ).get_player_reputations_count( ); ++reputation_id )
	{
		player_reputation const& reputation = lobby_client( ).get_player_reputation( reputation_id );
		u16 const reputation_points = reputation.reputation_points;

		char faction_str[32];
		sprintf_s						( faction_str, "faction_%d", reputation.faction_id );

		configs::binary_config_value const& faction_levels = get_game( ).items_dictionary( ).dict_config->get_root( )["factions_dict"][faction_str]["levels"];
		u8 const levels_count = faction_levels.size( );
		u8 player_reputation_level = 0;
		for ( u8 current_reputation_level = 0; current_reputation_level < levels_count; ++current_reputation_level )
		{
			if ( reputation_points >= (u32)faction_levels[current_reputation_level]["value"] )
				player_reputation_level = current_reputation_level;
		}

		flash_value player_progress_args[3];
		player_progress_args[0].SetUInt	( player_reputation_level );
		player_progress_args[1].SetUInt	( reputation_points );
		m_lobby_menu_ui->movie->Invoke	( "root.setup_player_progress", NULL, player_progress_args, 3 );
	}
}

void lobby_menu::set_ping( u32 ping_val )
{
	flash_value args; args.SetUInt( ping_val ); m_lobby_menu_ui->movie->Invoke( "root.set_ping", NULL, &args, 1 );
}

} // namespace survarium
