////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_client.h"

#include <boost/bind.hpp>							// on_lobby_packet_received's match-connect callbacks
#include <vostok/network_core/packet_reader.h>		// reader.r<>() / r_string() in the lobby dispatch

// network_client's player_ptr / player_desc members instantiate
// resource_ptr<player> dtors here, needing the complete player type
#include "player.h"

#include "game.h"			// m_game.lobby_menu()
#include "lobby_menu.h"		// query_lobby_info / show_disconnected_message + m_is_connected_to_lobby

namespace survarium {

// claude@NOTE: residual here is entirely LTCG/WPO inlining the target's delinked .obj reflects but
// our base .obj does not reproduce: (1) packet_reader::r<>()/r_string() are inlined to raw byte
// reads + memcpy in the target, called out-of-line in the base (the build-wide r<> residual noted in
// packet_reader_inline.h - their address-of anchor keeps them as COMDATs); (2) the LOG_* sites inline
// the filter_tree/callback/append idiom + boost::function1<char const*>::clear, where the base instead
// emits the helper boost::function ctor/dtor; (3) messaging_client::assign_match_channel_order is
// inlined into the connect case (the guarded match_id/team_id update + update_channel_subscriptions).
// These also drive the target's 8-byte stack alignment (and esp,-8). Structure is matched: the outer
// op_id switch is a bounds-check-free jump table (NODEFAULT), the client-status sub-dispatch is the
// source if/else-if chain the target emits, and every read/handler/log statement is reproduced.
void network_client::on_lobby_packet_received( network_core::packet_reader& reader )
{
	lobby_server_message_types_enum const op_id = (lobby_server_message_types_enum)reader.r< u8 >( );

	switch ( op_id )
	{
		case connect_to_match_server:
		{
			char host[ 64 ];
			reader.r_string	( host );

			u16 const port						= reader.r< u16 >( );
			lobby_client( ).match_id( )			= reader.r< u32 >( );
			lobby_client( ).team_id( )			= (game_team_id)reader.r< u8 >( );
			lobby_client( ).status( )			= lobby::in_match;

			m_last_tick_time_in_ms				= m_game.game_time_ms( );

			LOG_ERROR	( "[R] connect_to_game_server: %s: %d game time is %d", host, port, m_last_tick_time_in_ms );

			m_match_client.set_on_disconnect	( boost::bind( &network_client::on_match_disconnected, this, _1 ) );
			m_match_client.connect	(
				host,
				port,
				lobby_client( ).session_id( ),
				m_last_tick_time_in_ms,
				boost::bind( &network_client::on_connected_to_match, this, _1, _2, _3, _4 )
			);

			if ( messaging_client( ).local_player_team( ) != lobby_client( ).team_id( ) && lobby_client( ).match_id( ) != u32( -1 ) )
				messaging_client( ).assign_match_channel_order	( lobby_client( ).match_id( ), lobby_client( ).team_id( ) );

			m_game.lobby_menu( ).switch_to_level_loading	( );
			break;
		}
		case client_status:
		{
			lobby::query_info_types const type = (lobby::query_info_types)reader.r< u8 >( );

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
			else if ( type == 11 )
				lobby_client( ).read_player_reputations	( reader );
			else if ( type == 9 )
				lobby_client( ).read_player_skills_tree	( reader );
			else if ( type == 6 )
				m_game.lobby_menu( ).on_price_items_arrived	( lobby_client( ).read_price_items( reader ) );
			else if ( type == 7 )
				lobby_client( ).read_account_money	( reader );
			else if ( type == 10 )
				lobby_client( ).read_service_prices	( reader );
			else
				LOG_ERROR	( "Unknown client state received [%d]", type );

			m_game.lobby_menu( ).on_client_status_received	( type );
			break;
		}
		case operation_permitted:
		{
			lobby_client_message_types_enum const op_id = (lobby_client_message_types_enum)reader.r< u8 >( );

			LOG_INFO	( "[R] operation_permitted: %d", op_id );

			switch ( op_id )
			{
				case shop_action:
					process_shop_action	( reader );
					break;
				case skills_tree_action:
					if ( reader.r< u8 >( ) == 1 )
						lobby_client( ).query_client_status	( (lobby::query_info_types)7 );
					lobby_client( ).query_client_status	( (lobby::query_info_types)8 );
					break;
				default:
					m_game.lobby_menu( ).on_operation_permitted_received	( op_id );
					break;
			}
			break;
		}
		case operation_denied:
		{
			lobby_client_message_types_enum const op_id = (lobby_client_message_types_enum)reader.r< u8 >( );
			reader.r< u8 >( );

			char description[ 512 ];
			reader.r_string	( description );

			LOG_INFO	( "[R] operation_denied: %d", op_id );

			m_game.lobby_menu( ).on_operation_denied_received	( op_id, description );
			break;
		}
		case ping_server_answer:
			lobby_client( ).read_ping_server_answer	( reader );
			break;
		default:
			NODEFAULT( );
	}
}

// claude@NOTE: the target CSEs m_game.lobby_menu() into one held pointer across the
// body, so the faithful source caches the lobby_menu& once (the reference enregisters
// with 0 named locals - matches the target's recorded local set). Residual is the
// cross-TU LTCG inline of show_disconnected_message: in the base its scaleform Set*/
// Invoke calls are still empty stubs so it is tiny and the LTCG inliner folds its
// constant-pruned branch in here, whereas the target (real GFx ops) keeps it a call.
// Lifts once scaleform value.cpp/movie.cpp are matched.
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
	lobby_menu& menu = m_game.lobby_menu( );
	if ( menu.m_is_connected_to_lobby )
	{
		menu.m_is_connected_to_lobby = false;
		menu.show_disconnected_message( true );
	}
}

// claude@NOTE: structure-faithful (14/14 stmts). Residual is LTCG inline direction the base .obj
// can't reproduce: the four packet.r<>() reads are out-of-line calls here but inlined in the target,
// and our base inlines lobby_client::query_client_status (pulling in tcp_packet ctor/append/send)
// where the target keeps it a plain call. The inventory find-or-update-or-insert + push_back idiom
// itself matches (cmp [it+8],id / add [it],condition_or_stack).
void network_client::process_shop_action( network_core::packet_reader& packet )
{
	if ( packet.r< u8 >( ) )
		return;

	inventory_item_instance new_item;
	new_item.dict_id			= packet.r< u16 >( );
	new_item.id					= packet.r< u32 >( );
	new_item.condition_or_stack	= packet.r< u32 >( );
	new_item.amount_in_inventory	= 0;

	vectora< inventory_item_instance >::iterator i = lobby_client( ).inventory_item_instances( ).begin( );
	for ( ; i != lobby_client( ).inventory_item_instances( ).end( ); ++i )
		if ( i->id == new_item.id )
			break;

	if ( i == lobby_client( ).inventory_item_instances( ).end( ) )
		lobby_client( ).inventory_item_instances( ).push_back( new_item );
	else
		i->condition_or_stack	+= new_item.condition_or_stack;

	m_game.lobby_menu( ).fill_inventory_contents	( );
	lobby_client( ).query_client_status	( (lobby::query_info_types)7 );
}

void network_client::close_current_match( bool user_initiate )
{
	m_game_status	= game_status_inactive;

	if ( match_client( ).is_connected( ) )
		match_client( ).disconnect( );

	m_game.get_game_world( ).unload( );

	if ( user_initiate )
		lobby_client( ).discard_playing_order_on_connected( );

	if ( !lobby_client( ).net_connected( ) )
		lobby_client( ).connection_info( ).need_resolve	= true;

	if ( !m_game.get_game_world( ).is_loading( ) )
		m_game.switch_to_lobby( );
}

void network_client::process_match_finished( network_core::packet_reader& __formal )
{
	close_current_match	( false );
}

} // namespace survarium
