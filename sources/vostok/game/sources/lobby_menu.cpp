// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "lobby_menu.h"
#include "game_project.h" // complete simple_game_project for the resource_ptr dtor

#include <vostok/input/world.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>

#include "game.h"
#include "game_memory.h"
#include "base_network_client.h"
#include "lobby_client.h"
#include "messaging_client.h"
#include "chat_handler.h"
#include "camera_director.h"
#include "lobby_camera.h"
#include "profile_player_character.h"

#include <boost/bind.hpp>

namespace survarium {

 lobby_menu::lobby_menu( game& g )
	: base_game_scene					( g )
	, m_update_status_handler			( )
	, m_update_friends_status_handler	( )
	, m_selected_profile				( 0 )
	, m_ui_static_info_initialized		( false )
	, m_is_in_match_making				( false )
	, m_is_connected_to_lobby			( false )
	, m_last_ping_time_in_ms			( 0 )
	, m_player_max_carried_weight		( 0.f )
	, m_player_total_items_weight		( 0.f )
	, m_level_loading_progress			( 0.f )
	, m_last_queries_count				( 0 )
	, m_in_destroying					( false )
	, m_match_stats						( )
{
	m_camera = VOSTOK_NEW_IMPL( ::survarium::g_allocator, lobby_camera )( *this );
	get_camera_director( ).switch_to_camera( m_camera, "lobby camera" );
	init_physics( );
	query_scene_resources( );
}

 lobby_menu::~lobby_menu( )
{
	if ( m_update_status_handler.m_active )
		scheduler( ).unregister( &m_update_status_handler );

	if ( m_update_friends_status_handler.m_active )
		scheduler( ).unregister( &m_update_friends_status_handler );

	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_camera );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_character );

	m_lobby_game_project = NULL;

	on_ui_destroy( );
	destroy_physics( );
}

void lobby_menu::on_activate( )
{
	base_game_scene::on_activate( );
	get_game( ).input_world( ).add_handler( *this );
	if ( lobby_client( ).net_connected( ) )
		query_lobby_info( );
	get_game( ).get_chat_handler( ).set_mode( false );
}

// claude@NOTE: target GFx uses slot +0xC0 with value 2; the available wrapper emits
// slot +0x34 with value 0. Target LTCG also passes hide_movie arguments in EAX/EDX.

void lobby_menu::on_deactivate( )
{
	base_game_scene::on_deactivate( );
	get_game( ).input_world( ).remove_handler( *this );
	get_game( ).deactivate_main_menu( );
	m_lobby_menu_ui->movie->SetViewScaleMode( flash_movie::SM_ExactFit );	// sushi@TODO: target GFx slot [+0xC0](2); wrapper unverified

	if ( m_is_in_match_making )
		show_match_making( false );
}

void lobby_menu::on_disconnected_from_lobby( )
{
	if ( m_is_connected_to_lobby )
	{
		m_is_connected_to_lobby = false;
		show_disconnected_message( true );
	}
}

void lobby_menu::on_connected_to_lobby( )
{
	query_lobby_info( );
	if ( !m_is_connected_to_lobby )
	{
		m_is_connected_to_lobby = true;
		show_disconnected_message( false );
	}
}
// claude@NOTE: source structure matches; target LTCG uses a different frame and register arguments.
void lobby_menu::show_ui( bool b_show )
{
	if ( m_is_ui_shown != b_show )
	{
		if ( b_show )
		{
			show_movie( m_lobby_menu_ui );
			show_movie( m_message_ui );
			show_movie( m_cursor_ui );
			get_game( ).get_chat_handler( ).show( this );
		}
		else
		{
			hide_movie( m_lobby_menu_ui );
			hide_movie( m_message_ui );
			hide_movie( m_cursor_ui );
			get_game( ).get_chat_handler( ).hide( this );
		}

		m_is_ui_shown = b_show;
	}
}
// claude@NOTE: residual is the update_ui this-register convention and an ICF c_ptr fold representative.
void lobby_menu::tick(
	const u32		frame_delta_in_ms,
	const u32		current_time_in_ms,
	const bool		is_game_paused
)
{
	base_game_scene::tick( frame_delta_in_ms, current_time_in_ms, is_game_paused );

	if ( current_time_in_ms - m_last_ping_time_in_ms >= 1000 )
	{
		lobby_client( ).ping_server( );
		m_last_ping_time_in_ms = current_time_in_ms;
	}

	get_camera_director( ).apply( );

	if ( m_is_ui_shown )
		update_ui( frame_delta_in_ms, current_time_in_ms );

	m_character->update( current_time_in_ms );
}

void lobby_menu::query_lobby_info( )
{
	if ( !m_ui_static_info_initialized ) {
		lobby_client( ).query_client_status( lobby::q_profile_slots_restrictions );	// 4
		lobby_client( ).query_client_status( lobby::q_items_compatibility );			// 5
		lobby_client( ).query_client_status( lobby::q_player_skills_tree );			// 9
		lobby_client( ).query_client_status( lobby::q_service_prices ); for ( u32 i = 1, requests_count = i + 3 ;


			requests_count ;
			++i, --requests_count )
			lobby_client( ).query_prices( i );


		m_ui_static_info_initialized = true;
	}
	lobby_client( ).query_client_status( lobby::q_client_state );	// 0
}
void lobby_menu::query_account_data( )
{
	lobby_client( ).query_client_status( lobby::q_enumerate_inventory );	// 3
	lobby_client( ).query_client_status( lobby::q_account_money );		// 7
	lobby_client( ).query_client_status( lobby::q_player_skills );		// 8
	lobby_client( ).query_client_status( lobby::q_player_reputations );	// 0xB
}

void lobby_menu::switch_to_level_loading( )
{
	get_game( ).lobby_menu( ).show_match_making( true );
	m_match_making_ui->movie->Invoke( "root.switch_to_loading", NULL, NULL, 0 );
}

// claude@NOTE: source and LOG geometry match; residual is LTCG register arguments/inlining.
#line 189
void lobby_menu::on_client_status_received( lobby::query_info_types type )
{
	switch ( type )
	{
	case lobby::q_client_state:	// 0

		switch ( lobby_client( ).status( ) )
		{
		case lobby::surf_lobby_menu:


			if ( !is_active( ) )
			{
				get_game( ).switch_to_lobby( );
			}

			if ( !lobby_client( ).profiles_count( ) )
				query_account_data( );

			if ( get_game( ).get_game_world( ).is_loading( ) ) {
				if ( get_game( ).lobby_menu( ).m_is_in_match_making ) {
					get_game( ).lobby_menu( ).hide_movie( get_game( ).lobby_menu( ).m_match_making_ui );
					get_game( ).lobby_menu( ).m_is_in_match_making = false;
				}
			}
			break;
		case lobby::in_match_making_order:
		case lobby::in_match:
			break;

		case lobby::in_match_making:
			show_match_making( true );
			request_status_from_server( 1000 );
			break;
		default:
			LOG_ERROR( "Unknown client state %d", lobby_client( ).status( ) );
		}
		update_status( );
		break;


	case lobby::q_enumerate_profiles:	// 1
		LOG_INFO( "[R] enumerate_profiles" );
		for ( u8 i = 0; i < lobby_client( ).profiles_count( ); ++i )
		{

			lobby_client( ).query_profile_contents(

				lobby_client( ).profile( i ).profile_id
			);
		}
		fill_profiles( );
		break;

	case lobby::q_profile_contents:	// 2
		LOG_INFO( "[R] porfile_contents" );
		break;

	case lobby::q_enumerate_inventory:	// 3
		LOG_INFO( "[R] enumerate_inventory" );
		fill_inventory_contents( );
		lobby_client( ).query_client_status( lobby::q_enumerate_profiles );	// 1
		break;

	case lobby::q_profile_slots_restrictions:	// 4
		LOG_INFO( "[R] profile_slots_restrictions" );
		on_slot_restrictions_arrived( );
		break;

	case lobby::q_items_compatibility:	// 5
		LOG_INFO( "[R] items_compatibility" );
		on_items_compatibility_arrived( );
		break;

	case lobby::q_price_items:	// 6
		LOG_INFO( "[R] price_items" );
		break;

	case lobby::q_account_money:	// 7

		LOG_INFO( "[R] account_money" );
		reset_account_money( );
		break;

	case lobby::q_player_skills:	// 8
		LOG_INFO( "[R] player_skills" );
		fill_character_data( );
		break;

	case lobby::q_player_skills_tree:	// 9
		LOG_INFO( "[R] player_skills_tree" );
		fill_skills_tree( );
		break;

	case lobby::q_service_prices:	// 0xA
		LOG_INFO( "[R] service_prices" );
		fill_service_prices( );
		break;

	case lobby::q_player_reputations:	// 0xB
		LOG_INFO( "[R] player_reputations" );
		on_player_reputations_arrived( );
		break;
	default:
		LOG_ERROR( "Unknown Client status received. type = %d", type );
	}
}
// claude@NOTE: source and LOG geometry match; residual is optimized callee/log partitioning.
#line 302
void lobby_menu::on_friendship_status_recivied(
	const messaging::friendship_actions_enum	type
)
{
	switch ( type )
	{
	case messaging::query_friend_list:
		LOG_INFO( "[R] friend_list" );
		fill_friend_list( );

		request_friends_status_from_server( 10000 );
		break;

	case messaging::query_ignore_list:

		LOG_INFO( "[R] ignore_list" );
		fill_ignore_list( );
		break;

	case messaging::find_players:

		LOG_INFO( "[R] find_players" );
		fill_found_players( );
		break;
	}
}
// claude@NOTE: source and LOG geometry match; residual is optimized callee/log partitioning.
#line 328
void lobby_menu::on_operation_permitted_received( lobby_client_message_types_enum op_type )
{
	switch ( op_type )
	{
	case set_status_ready_for_match:

		request_status_from_server( 1000 );
		break;
	case inventory_action:
	case shop_action:
		lobby_client( ).query_profile_contents(
			lobby_client( ).profile( m_selected_profile ).profile_id );
		break;

	case skills_tree_action:
		lobby_client( ).query_client_status( lobby::q_player_skills );	// 8
		break;
	default:
		LOG_ERROR( "Unknown (operation permitted) type received %d", op_type );
	}
}
// claude@NOTE: target keeps the four identical case bodies separate; the base folds them.
void lobby_menu::on_operation_denied_received( lobby_client_message_types_enum op_type, pcstr description )
{
	switch ( op_type )
	{
	case set_status_ready_for_match:
		{
			flash_value v[3];
			v[0].SetUInt					( 99 );
			v[1].SetString					( "debug" );
			v[2].SetString					( description );
			m_message_ui->movie->Invoke		( "root.showMessage", NULL, v, 3 );
			request_status_from_server		( 500 );
		} break;
	case inventory_action:
		{
			flash_value v[3];
			v[0].SetUInt					( 99 );
			v[1].SetString					( "debug" );
			v[2].SetString					( description );
			m_message_ui->movie->Invoke		( "root.showMessage", NULL, v, 3 );
			request_status_from_server		( 500 );
		} break;
	case shop_action:
		{
			flash_value v[3];
			v[0].SetUInt					( 99 );
			v[1].SetString					( "debug" );
			v[2].SetString					( description );
			m_message_ui->movie->Invoke		( "root.showMessage", NULL, v, 3 );
			request_status_from_server		( 500 );
		} break;
	case skills_tree_action:
		{
			flash_value v[3];
			v[0].SetUInt					( 99 );
			v[1].SetString					( "debug" );
			v[2].SetString					( description );
			m_message_ui->movie->Invoke		( "root.showMessage", NULL, v, 3 );
			request_status_from_server		( 500 );
		} break;
	default:
		LOG_ERROR( "Unknown (operation denied) type received %d", op_type );
	}
}

void lobby_menu::request_status_from_server( u32 delay_ms )
{
	if ( m_in_destroying )
		return;

	u32 const current_time	= get_game( ).game_time_ms( );

	scheduler( ).register_for_update(
		&m_update_status_handler, boost::bind( &lobby_menu::request_status_from_server_impl, this, _1, _2 ),
		true, delay_ms, 1, current_time );

}

#line 419
void lobby_menu::request_status_from_server_impl( const u32 frame_delta_ms, const u32 current_time_ms )
{
	VOSTOK_UNREFERENCED_PARAMETERS( frame_delta_ms, current_time_ms );

	scheduler( ).unregister( &m_update_status_handler );
	lobby_client( ).query_client_status( lobby::q_client_state );	// 0
	LOG_INFO( "request_status_from_server_impl" );
}

void lobby_menu::request_friends_status_from_server( u32 delay_ms )
{
	if ( m_in_destroying )
		return;

	u32 const current_time	= get_game( ).game_time_ms( );

	scheduler( ).register_for_update(
		&m_update_friends_status_handler,
		boost::bind( &lobby_menu::request_friends_status_from_server_impl, this, _1, _2 ), true,
		10000, 1, current_time );
}

void lobby_menu::request_friends_status_from_server_impl( const u32 frame_delta_ms, const u32 current_time_ms )
{
	VOSTOK_UNREFERENCED_PARAMETERS( frame_delta_ms, current_time_ms );
	scheduler( ).unregister( &m_update_friends_status_handler );
	messaging_client( ).query_for_friends_status( );
}

} // namespace survarium
