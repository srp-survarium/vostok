// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "network_client.h"

// network_client's player_ptr / player_desc members instantiate
// resource_ptr<player> dtors here, needing the complete player type
#include "player.h"

#include "game.h"			// m_game.lobby_menu()
#include "lobby_menu.h"		// query_lobby_info / show_disconnected_message + m_is_connected_to_lobby

#include <vostok/network_core/packet_reader.h>	// reader.r<T>() / r_string in the lobby dispatch

#include <algorithm>		// std::find over the inventory item instances

namespace survarium {












// op_id is typed lobby_client_message_types_enum, as the PDB records it: the server
// message values reuse the client enum's numbering
void network_client::on_lobby_packet_received( network_core::packet_reader& reader )
{
	lobby_client_message_types_enum const op_id = (lobby_client_message_types_enum)reader.r< u8 >( );

	switch ( op_id )
	{
	case vostok::connect_to_match_server:
		{
			char host[ 64 ];
			reader.r_string	( host );

			const u16 port	= reader.r< u16 >( );
			lobby_client( ).match_id( )	= reader.r< u32 >( );
			lobby_client( ).team_id( )	= (game_team_id)reader.r< u8 >( );
			lobby_client( ).status( )	= lobby::in_match;

			m_last_tick_time_in_ms	= m_game.game_time_ms( );

			LOG_ERROR	( "[R] connect_to_game_server: %s: %d game time is %d", host, port, m_last_tick_time_in_ms );

			m_match_client.set_on_disconnect	( boost::bind( &network_client::on_match_disconnected, this, _1 ) );
			m_match_client.connect	( host, port, lobby_client( ).session_id( ), m_last_tick_time_in_ms, boost::bind( &network_client::on_connected_to_match, this, _1, _2, _3, _4 ) );

			messaging_client( ).assign_match_channel_order	( lobby_client( ).match_id( ), lobby_client( ).team_id( ) );

			m_game.lobby_menu( ).switch_to_level_loading	( );
		}
		break;

	case vostok::client_status:
		{
			lobby_client_message_types_enum const type = (lobby_client_message_types_enum)reader.r< u8 >( );

			if ( type == 0 )
			{
				lobby_client( ).read_status_info	( reader );
				messaging_client( ).assign_match_channel_order	( lobby_client( ).match_id( ), lobby_client( ).team_id( ) );
			}
			else if ( type == 1 )
				lobby_client( ).read_enumerate_profiles_info	( reader );
			else if ( type == 2 )
				m_game.lobby_menu( ).on_profile_arrived	( lobby_client( ).read_profile_content_info( reader ) );
			else if ( type == 3 )
				lobby_client( ).read_enumerate_inventory_info	( reader );
			else if ( type == 4 )
				lobby_client( ).read_profile_slots_restrictions	( reader );
			else if ( type == 5 )
				lobby_client( ).read_items_compatibility	( reader );
			else if ( type == 8 )
				lobby_client( ).read_player_skills	( reader );
			else if ( type == 0xB )
				lobby_client( ).read_player_reputations	( reader );
			else if ( type == 9 )
				lobby_client( ).read_player_skills_tree	( reader );
			else if ( type == 6 )
				m_game.lobby_menu( ).on_price_items_arrived	( lobby_client( ).read_price_items( reader ) );
			else if ( type == 7 )
				lobby_client( ).read_account_money	( reader );
			else if ( type == 0xA )
				lobby_client( ).read_service_prices	( reader );
			else
				LOG_ERROR	( "Unknown client state received [%d]", type );

			m_game.lobby_menu( ).on_client_status_received	( (lobby::query_info_types)type );
		}
		break;

	case vostok::operation_permitted:
		{
			lobby_client_message_types_enum const op = (lobby_client_message_types_enum)reader.r< u8 >( );

			LOG_INFO	( "[R] operation_permitted: %d", op );

			if ( op == vostok::shop_action )
				process_shop_action	( reader );
			else if ( op == vostok::skills_tree_action )
			{
				if ( reader.r< u8 >( ) == 1 )
					lobby_client( ).query_client_status	( lobby::q_account_money );		// 7
				lobby_client( ).query_client_status	( lobby::q_player_skills );			// 8
			}
			else
				m_game.lobby_menu( ).on_operation_permitted_received	( op );
		}
		break;

	case vostok::operation_denied:
		{
			lobby_client_message_types_enum op_id = (lobby_client_message_types_enum)reader.r< u8 >( );
			u8 faction_id = reader.r< u8 >( );

			char description[ 512 ];
			reader.r_string	( description );

			LOG_INFO	( "[R] operation_denied: %d", op_id );

			m_game.lobby_menu( ).on_operation_denied_received	( op_id, description );
		}
		break;

	case vostok::ping_server_answer:
		lobby_client( ).read_ping_server_answer( reader );
		break;
	}
}







// m_game.lobby_menu( ) is read once, as retail does
void network_client::on_connected_to_lobby( )
{
	lobby_menu& menu = m_game.lobby_menu( );
	menu.query_lobby_info( );
	if ( !menu.m_is_connected_to_lobby )
	{
		menu.m_is_connected_to_lobby = true;
		menu.show_disconnected_message( false );
	}
}








void network_client::on_disconnected_from_lobby( )
{

	m_game.lobby_menu( ).on_disconnected_from_lobby( );
}










void network_client::process_shop_action( network_core::packet_reader& packet )
{
	if ( packet.r< u8 >( ) )
		return;

	inventory_item_instance new_item;
	new_item.dict_id				= packet.r< u16 >( );
	new_item.id						= packet.r< u32 >( );
	new_item.condition_or_stack		= packet.r< u32 >( );
	new_item.amount_in_inventory	= 0;

	vectora< inventory_item_instance >::iterator const found	= std::find(
		lobby_client( ).inventory_item_instances( ).begin( ),
		lobby_client( ).inventory_item_instances( ).end( ),
		new_item.id
	);
	if ( found != lobby_client( ).inventory_item_instances( ).end( ) )
		found->condition_or_stack	+= new_item.condition_or_stack;
	else
		lobby_client( ).inventory_item_instances( ).push_back( new_item );

	m_game.lobby_menu( ).fill_inventory_contents( );
	lobby_client( ).query_client_status	( ( lobby::query_info_types )7 );
}





void network_client::close_current_match( bool user_initiate )
{
	m_game_status	= game_status_inactive;

	if ( match_client( ).is_connected( ) )
		match_client( ).disconnect		( );

	get_game_world( ).unload	( );

	if ( user_initiate )
		lobby_client( ).discard_playing_order_on_connected	( );

	if ( !lobby_client( ).net_connected( ) )
		lobby_client( ).connection_info( ).need_resolve	= true;

	if ( !get_game_world( ).is_loading( ) )
		m_game.switch_to_lobby	( );
}

void network_client::process_match_finished( network_core::packet_reader& __formal )
{
	close_current_match	( false );
}

} // namespace survarium
