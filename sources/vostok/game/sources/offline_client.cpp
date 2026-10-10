// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "offline_client.h"

#include "game.h"
#include "game_world.h"
#include "game_project.h"
#include "player.h"
#include "lobby_client.h"
#include "chat_handler.h"
#include "lobby_menu.h"
#include "flash_text_manager.h"
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <stdlib.h>
#include <vostok/console_command.h>
#include <vostok/configs_binary_config.h>
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/player_initial_info.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_fs.h>

namespace survarium {

namespace {

char s_offline_map[64] = "level_03_evn";
char s_offline_weapon[32] = "ak_74u";
char s_offline_spawn[64] = "";

console_commands::cc_string s_offline_map_command(
	"offline_map", s_offline_map, sizeof(s_offline_map), true,
	console_commands::command_type_user_specific );
console_commands::cc_string s_offline_weapon_command(
	"offline_weapon", s_offline_weapon, sizeof(s_offline_weapon), true,
	console_commands::command_type_user_specific );
console_commands::cc_string s_offline_spawn_command(
	"offline_spawn", s_offline_spawn, sizeof(s_offline_spawn), true,
	console_commands::command_type_user_specific );

struct weapon_choice {
	pcstr name;
	u16 weapon_id;
	u16 ammo_id;
};

// IDs are from the stock v0.100b items dictionary. A profile slot's id is a
// local inventory instance ID; dict_id selects the actual weapon or ammunition.
weapon_choice const s_weapons[] = {
	{ "ak_74u", 13, 7 }, { "toz_122", 12, 51 },
	{ "rem_700", 14, 51 }, { "rem_870", 15, 22 },
	{ "toz_34", 16, 22 }, { "toz_66", 17, 22 },
	{ "tt_33", 18, 50 }, { "vityaz", 19, 53 },
	{ "uzi", 55, 53 }, { "magnum", 56, 20 },
	{ "fort_17", 64, 70 },
};
enum { weapon_count = sizeof(s_weapons) / sizeof(s_weapons[0]) };

} // namespace

offline_client::offline_client( game& game ) :
	network_client( game, false ),
	m_map_chat_delay( 0 )
{
	static console_commands::cc_delegate s_offline_load_command(
		"offline_load", boost::bind( &offline_client::load_command, this, _1 ),
		true, console_commands::command_type_engine_internal );
}

offline_client::~offline_client( )
{
	unload( );

	// ~network_client destroys these draw_stats texts whenever the match world still has a
	// text manager, as it does when offline quits straight from a match without draw_stats.
	if ( flash_text_manager* const text_manager = m_game.get_game_world( ).get_text_manager( ) )
	{
		flash_text* const texts[] = {
			&m_unacknowledged_packets_value,
			&m_unacknowledged_packets_caption,
			&m_max_local_sequence_difference_value,
			&m_max_local_sequence_difference_caption,
		};
		for ( u32 i = 0; i < array_size( texts ); ++i )
			if ( !texts[i]->text_impl )
				*texts[i] = text_manager->create_text( "" );
	}
}

pcstr offline_client::default_map( )
{
	return s_offline_map;
}

void offline_client::initialize_lobby( )
{
	if ( !configure_weapon( ) )
	{
		strcpy_s( s_offline_weapon, sizeof(s_offline_weapon), "ak_74u" );
		configure_weapon( );
	}
	items_compatibility pairs[weapon_count];
	for ( u32 i = 0; i < weapon_count; ++i )
	{
		pairs[i].first_item_dict_id = s_weapons[i].weapon_id;
		pairs[i].second_item_dict_id = s_weapons[i].ammo_id;
	}
	lobby_client( ).set_offline_compatibilities( pairs, weapon_count );
	m_game.get_chat_handler( ).set_local_player_name( "Offline Player" );
	resources::query_vfs_iterator( "resources/projects",
		boost::bind( &offline_client::on_maps_ready, this, _1 ), g_allocator,
		resources::recursive_true );
}

void offline_client::on_maps_ready( vfs::vfs_locked_iterator const& maps )
{
	m_available_maps.clear( );
	if ( !maps )
	{
		LOG_ERROR( "offline map projects are unavailable" );
		return;
	}
	for ( vfs::vfs_iterator project = maps.children_begin( ); project != maps.children_end( ); ++project )
	{
		if ( !project.is_folder( ) || !project.find_child( "client_project" ) ||
			!project.find_child( "server_project" ) || strlen( project.get_name( ) ) >= 64 )
			continue;
		m_available_maps.push_back( project.get_name( ) );
	}
	if ( !m_available_maps.empty( ) )
	{
		u32 i = 0;
		for ( ; i < m_available_maps.size( ); ++i )
			if ( !_stricmp( s_offline_map, m_available_maps[i].c_str( ) ) )
				break;
		strcpy_s( s_offline_map, sizeof(s_offline_map),
			m_available_maps[i < m_available_maps.size( ) ? i : 0].c_str( ) );
	}
	LOG_INFO( "offline mode found %d playable maps", m_available_maps.size( ) );
	open_map_chat( );
}

bool offline_client::map_available( pcstr map_name ) const
{
	for ( u32 i = 0; i < m_available_maps.size( ); ++i )
		if ( !_stricmp( map_name, m_available_maps[i].c_str( ) ) )
			return true;
	return false;
}

void offline_client::show_map_error( pcstr map_name )
{
	fixed_string< 512 > message;
	wchar_t wide[512];
	message.assignf( "incorrect map: %s", map_name );
	mbstowcs_s( NULL, wide, 512, message.c_str( ), _TRUNCATE );
	m_game.get_chat_handler( ).add_message( messaging::player_system_channel, wide, L"Offline" );
	print_map_list( );
}

void offline_client::show_maps( )
{
	// Offline the chat is the map picker: every time it opens, it starts from a clean log
	// holding only the numbered list.
	flash_movie_resource_ptr& chat = m_game.get_chat_handler( ).get_movie( );
	for ( u32 channel = messaging::server_message_channel; channel < messaging::max_channel_num; ++channel )
	{
		flash_value argument;
		argument.SetUInt( channel );
		chat->movie->Invoke( "root.clear_channel", NULL, &argument, 1 );
	}
	chat->movie->SetVariable( "root.chat.text_chat.text", "" );
	print_map_list( );
}

void offline_client::open_map_chat( )
{
	// The lobby opens with the map picker ready. The map list and the lobby UI arrive in
	// either order, and the chat movie rejects messages until it has run a few frames
	// with its channels set, so the request is served from tick once the lobby has settled.
	m_map_chat_delay = map_chat_settle_frames;
}

void offline_client::tick_map_chat( )
{
	chat_handler& chat = m_game.get_chat_handler( );
	if ( m_available_maps.empty( ) || !chat.is_active( ) || chat.in_match( ) )
	{
		m_map_chat_delay = map_chat_settle_frames;
		return;
	}
	if ( --m_map_chat_delay )
		return;
	// The lobby chat starts folded (frame 3); unfold it first and let the unfolded
	// frame build its input and log before focusing it.
	flash_value frame;
	chat.get_movie( )->movie->GetVariable( &frame, "root.chat.currentFrame" );
	// currentFrame comes back as an int; the getters read the raw union, so accept either form.
	if ( frame.GetInt( ) == 3 || frame.GetNumber( ) == 3.f )
	{
		chat.get_movie( )->movie->Invoke( "root.chat.onClose", NULL, NULL, 0 );
		m_map_chat_delay = map_chat_unfold_frames;
		return;
	}
	flash_value argument;
	argument.SetBoolean( true );
	chat.get_movie( )->movie->Invoke( "root.focus_chat", NULL, &argument, 1 );
	chat.focus( true );
}

void offline_client::print_map_list( )
{
	fixed_string< 512 > message;
	wchar_t wide[512];
	wchar_t header[] = L"available maps:";
	m_game.get_chat_handler( ).add_message( messaging::player_system_channel, header, L"Offline" );
	for ( u32 i = 0; i < m_available_maps.size( ); ++i )
	{
		message.assignf( "%u - %s", i + 1, m_available_maps[i].c_str( ) );
		mbstowcs_s( NULL, wide, 512, message.c_str( ), _TRUNCATE );
		m_game.get_chat_handler( ).add_message( messaging::player_system_channel, wide, L"Offline" );
	}
}

void offline_client::choose_map_from_chat( wchar_t const* text )
{
	if ( !text )
		return;
	char map_name[64];
	size_t converted = 0;
	if ( wcstombs_s( &converted, map_name, text, _TRUNCATE ) )
		return;
	// Typed letters may not reach the chat under Wine, so a map can also be picked by its
	// number in the list show_map_error prints.
	char* number_end = NULL;
	unsigned long const number = strtoul( map_name, &number_end, 10 );
	if ( number_end != map_name && !*number_end && number >= 1 && number <= m_available_maps.size( ) )
	{
		strcpy_s( s_offline_map, sizeof(s_offline_map), m_available_maps[number - 1].c_str( ) );
		start_match( );
		return;
	}
	for ( u32 i = 0; i < m_available_maps.size( ); ++i )
		if ( !_stricmp( map_name, m_available_maps[i].c_str( ) ) )
		{
			strcpy_s( s_offline_map, sizeof(s_offline_map), m_available_maps[i].c_str( ) );
			start_match( );
			return;
		}
	show_map_error( map_name );
}

void offline_client::start_match( )
{
	if ( m_player || m_game.get_game_world( ).is_loading( ) )
		return;
	if ( !map_available( s_offline_map ) )
	{
		show_map_error( s_offline_map );
		return;
	}
	// Non-demo players get no empty hands (player_cook), so player::insert needs a weapon.
	player_profile const& profile = lobby_client( ).profile( 0 );
	if ( !profile.slots[weapon1_slot].item.id && !profile.slots[weapon2_slot].item.id )
	{
		wchar_t message[] = L"equip a weapon before starting a match";
		m_game.get_chat_handler( ).add_message( messaging::player_system_channel, message, L"Offline" );
		return;
	}
	// A focused chat keeps consuming every key, which would block movement in the match.
	m_game.get_chat_handler( ).focus( false );
	// Online shows this when the match server is assigned; the lobby hides it on deactivation.
	m_game.lobby_menu( ).switch_to_level_loading( );
	m_game.load( s_offline_map );
}

bool offline_client::configure_weapon( )
{
	weapon_choice const* choice = NULL;
	for ( u32 i = 0; i < array_size(s_weapons); ++i )
		if ( !_stricmp( s_offline_weapon, s_weapons[i].name ) )
			choice = &s_weapons[i];

	if ( !choice )
	{
		LOG_WARNING( "unknown offline weapon: %s", s_offline_weapon );
		return false;
	}

	lobby_client( ).initialize_offline( "Offline Player", choice->weapon_id, choice->ammo_id );
	return true;
}

void offline_client::load( pcstr map_name, camera_director const& )
{
	if ( !map_name || !*map_name || m_game.get_game_world( ).is_loading( ) )
		return;
	if ( !map_available( map_name ) )
	{
		show_map_error( map_name );
		return;
	}
	if ( strlen( map_name ) >= sizeof(match_client( ).get_match_options( ).map_name) )
	{
		LOG_WARNING( "offline map name is too long: %s", map_name );
		return;
	}

	m_map_name = map_name;
	m_profile = lobby_client( ).profile( 0 );

	struct match_options& options = match_client( ).get_match_options( );
	options.player_profiles[0] = m_profile;
	options.players_count = 1;
	options.victory_items_count = 0;
	options.match_mode_ = capture_enemy_base;
	strcpy_s( options.map_name, sizeof(options.map_name), map_name );

	player_initial_info info;
	info.id = 0;
	info.profile = &options.player_profiles[0];
	info.game_scene = &m_game.get_game_world( );
	info.is_demo_player = false;

	variant<32> user_data;
	user_data.set( info );
	variant<32> const* user_data_ptr = &user_data;
	resources::request request = resources::create_request(
		"gameplay/players/default.player", resources::player_class );
	m_game.load( map_name, &request, &request + 1, &user_data_ptr,
		boost::bind( &offline_client::on_player_loaded, this, _1 ) );
}

void offline_client::on_player_loaded( resources::queries_result& data )
{
	if ( !data.is_successful( ) )
	{
		LOG_ERROR( "offline player or map failed to load: %s", m_map_name.c_str( ) );
		m_game.lobby_menu( ).show_match_making( false );
		return;
	}

	m_player = static_cast_resource_ptr< player_ptr >( data[0].get_unmanaged_resource( ) );
	m_local_player = m_player;
	float3 position = m_game.get_game_world( ).get_project( )->m_config->get_root( )["camera"]["position"];
	float orientation = 0.f;
	if ( !_stricmp( m_map_name.c_str( ), "level_03_evn" ) )
	{
		position = float3( -7.76438f, 13.95794f, -31.85271f );
		orientation = 10.f;
	}
	float x, y, z;
	if ( sscanf_s( s_offline_spawn, "%f %f %f", &x, &y, &z ) == 3 )
		position = float3( x, y, z );

	m_player->set_character_transform( position, orientation, 0.f );
	m_player->insert( true );
	attach_to_player( m_player );
}

void offline_client::load_command( pcstr map_name )
{
	if ( map_name && *map_name )
	{
		if ( strlen( map_name ) >= sizeof(s_offline_map) )
		{
			LOG_WARNING( "offline map name is too long: %s", map_name );
			return;
		}
		u32 i = 0;
		for ( ; i < m_available_maps.size( ); ++i )
			if ( !_stricmp( map_name, m_available_maps[i].c_str( ) ) )
				break;
		if ( i == m_available_maps.size( ) )
		{
			show_map_error( map_name );
			return;
		}
		strcpy_s( s_offline_map, sizeof(s_offline_map), m_available_maps[i].c_str( ) );
	}
	start_match( );
}

void offline_client::tick( u32 current_time_in_ms, bool is_game_paused )
{
	if ( m_map_chat_delay )
		tick_map_chat( );
	if ( !m_player || !m_player->has_been_inserted( ) )
		return;
	if ( !is_game_paused )
		m_player->tick( current_time_in_ms );
	m_player->update_camera( );
}

void offline_client::unload( )
{
	if ( !m_player )
		return;
	detach_from_player( );
	if ( m_player->has_been_inserted( ) )
		m_player->remove( );
	m_player = NULL;
	m_local_player = NULL;
}

void offline_client::disconnect( )
{
	unload( );
}

void offline_client::initiate_kill_current_player( )
{
	if ( m_player && m_player->is_alive( ) )
		m_player->kill( m_game.game_time_ms( ) );
}

void offline_client::initiate_respawn_current_player( )
{
	if ( !m_player || m_player->is_alive( ) )
		return;
	m_player->remove( );
	m_player->insert( true );
	attach_to_player( m_player );
}

player_ptr offline_client::get_player( u8 id ) const
{
	return id == 0 ? m_player : player_ptr( );
}

bool offline_client::is_player_local( u8 id ) const
{
	return id == 0 && m_player;
}

void offline_client::close_current_match( bool )
{
	m_game.unload( "", false );
	m_game.switch_to_lobby( );
}

} // namespace survarium
