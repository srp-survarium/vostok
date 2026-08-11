////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_client.h"

#include <boost/bind.hpp>

#include "game.h"
#include "game_memory.h"
#include "lobby_menu.h"
#include "price_item.h"
#include <vostok/login_server/message_types.h>
#include <vostok/game_core/game_net_defines.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/tcp_packet.h>
#include <vostok/configs.h>
#include <vostok/memory_buffer.h>

namespace survarium {

lobby_client::lobby_client( game& g ) :
	m_game							( g ),
	m_packet_client					( g.get_network_world( ) ),
	m_status						( lobby::unknown ),
	m_net_client_connected			( false ),
	m_match_id						( -1 ),
	m_match_order_id				( -1 ),
	m_team_id						( team_undefined ),
	m_profiles_count				( 0 ),
	m_inventory_item_instances		( g_allocator ),
	m_player_skills					( NULL ),
	m_player_skills_count			( 0 ),
	m_player_reputations			( NULL ),
	m_player_reputations_count		( 0 ),
	m_account_money					( ),
	m_player_perks					( NULL ),
	m_player_perks_count			( 0 ),
	m_profile_slot_restrictions		( NULL ),
	m_profile_slot_restrictions_count	( 0 ),
	m_items_compatibility			( NULL ),
	m_items_compatibilities_count	( 0 ),
	m_skills_tree_config			( NULL ),
	m_discard_playing_order_on_connected	( false )
{
	m_connection_info.session_id	= -1;
	m_connection_info.host[ 0 ]		= 0;
	m_connection_info.port			= 0;
	m_connection_info.need_resolve	= true;
	m_connection_info.connection_error_count	= 0;
	memset( m_prices, 0, sizeof( m_prices ) );

	m_packet_client.set_on_connected	( boost::bind( &lobby_client::on_connected, this ) );
	m_packet_client.set_on_disconnected	( boost::bind( &lobby_client::on_disconnected, this ) );
	m_packet_client.set_on_error		( boost::bind( &lobby_client::on_error, this, _1, _2 ) );
}

lobby_client::~lobby_client( )
{
	clear_initial_info	( );
	clear_profile_info	( );
}

void lobby_client::clear_initial_info( )
{
	FREE	( m_profile_slot_restrictions );
	FREE	( m_items_compatibility );
}

void lobby_client::clear_profile_info( )
{
	m_profiles_count	= 0;
	m_inventory_item_instances.clear	( );

	m_account_money.generic_money	= m_account_money.premium_money	= m_account_money.total_skill_points	= 0;

	FREE	( m_player_skills );
	FREE	( m_player_reputations );
	FREE	( m_player_perks );
}

void lobby_client::on_connected( )
{
	m_packet_client.set_on_packet_received	( boost::bind( &lobby_client::sign_in_on_packet_received, this, _1 ) );

	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::lobby_client_sign_in_info );
	packet.append	( m_connection_info.session_id );
	m_packet_client.send	( packet );
}

// claude@NOTE: structure-capped, not source-steerable. The boost::function safe-bool test
// ( if ( m_on_disconnected ) ) is emitted out-of-line in the target's full-LTCG build
// (call boost::function1<void,bool>::operator bool) but inlined to neg/sbb/test in our base,
// so target shows 4 stmts vs our 1. Systematic boost-header artifact, same in disconnect()/on_error().
void lobby_client::on_disconnected( )
{
	m_net_client_connected	= false;
	if ( m_on_disconnected )
		m_on_disconnected	( );
}

void lobby_client::on_error(
	network_core::client_error_codes_enum,
	boost::system::error_code
)
{
	LOG_ERROR	( "lobby client error. reconnecting" );

	disconnect	( );

	++m_connection_info.connection_error_count;	m_connection_info.need_resolve	= true;
}

void lobby_client::connect( server_connection_info const& lobby_connection_info )
{
	m_connection_info	= lobby_connection_info;
	m_packet_client.connect	( m_connection_info.host, m_connection_info.port );
}

void lobby_client::disconnect( )
{
	m_net_client_connected	= false;

	if ( m_on_disconnected )
		m_on_disconnected	( );

	LOG_INFO	( "lobby client initiate disconnect" );

	clear_profile_info	( );
	m_packet_client.disconnect	( );
}

void lobby_client::sign_in_on_packet_received( network_core::packet_reader& reader )
{
	lobby_client_message_types_enum const op_id = (lobby_client_message_types_enum)reader.r< u8 >( );

	if ( op_id != vostok::lobby_client_sign_in_info )
	{
		LOG_ERROR	( "Lobby client received unknown SignIn message type [%d]", op_id );
		return;
	}

	m_packet_client.set_on_packet_received	( m_on_packet_received );
	m_net_client_connected	= true;

	LOG_INFO	( "Lobby client: signed in!" );

	if ( m_discard_playing_order_on_connected )
	{
		discard_playing_order	( );
		m_discard_playing_order_on_connected	= false;
	}

	if ( m_on_connected )
		m_on_connected	( );
}

u32 lobby_client::session_id( ) const
{
	return m_connection_info.session_id;
}

void lobby_client::set_status_ready_for_match(
	const u32		profile_id
)
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::set_status_ready_for_match );
	packet.append	( profile_id );
	m_packet_client.send	( packet );
}

void lobby_client::query_client_status( lobby::query_info_types type )
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::query_client_status );
	packet.append	( (u32)type );
	m_packet_client.send	( packet );
}

void lobby_client::query_prices(
	const u32		faction_id
)
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::query_client_status );
	packet.append	( (u8)lobby::q_price_items );
	packet.append	( (u8)faction_id );
	m_packet_client.send	( packet );
}

void lobby_client::query_profile_contents(
	const u32		profile_id
)
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::query_client_status );
	packet.append	( (u8)lobby::q_profile_contents );
	packet.append	( profile_id );
	m_packet_client.send	( packet );
}

// claude@NOTE: the read_*/send handlers below are STRUCTURE-MATCHed but byte-capped:
// network_core::packet_reader::r<T>()/r_string()/eof() and tcp_packet::append()/send()
// are whole-program-inlined in the target but emitted as calls in our base. Match the
// networking implementations before chasing the remaining bytes here.
bool lobby_client::read_status_info( network_core::packet_reader& reader )
{
	m_status	= (lobby::client_state_enum)reader.r< u8 >( );

	if ( m_status == lobby::in_match_making_order || m_status == lobby::in_match_making || m_status == lobby::in_match )
	{
		m_match_order_id	= reader.r< u32 >( );
		m_match_id			= reader.r< u32 >( );
		m_team_id			= (game_team_id)reader.r< u8 >( );
	}
	else
	{
		m_match_order_id	= -1;
		m_match_id			= -1;
		m_team_id			= team_undefined;
	}

	if ( !reader.eof( ) )
		reader.r_string	( m_last_status_message.get_buffer( ), (u8)m_last_status_message.get_buffer_size( ) );
	else
		m_last_status_message	= "";

	return true;
}

bool lobby_client::read_enumerate_profiles_info( network_core::packet_reader& reader )
{
	m_profiles_count	= reader.r< u8 >( );

	for ( u8 i = 0; i < m_profiles_count; ++i )
	{
		m_profiles[ i ].profile_id	= reader.r< u32 >( );
		reader.r_string	( m_profiles[ i ].profile_name );
	}

	m_player_name	= "";
	return true;
}

u8 lobby_client::read_profile_content_info( network_core::packet_reader& reader )
{
	player_profile profile;
	reader.r	( &profile, sizeof( profile ), sizeof( profile ) );

	for ( u8 i = 0; i < 3; ++i )
	{
		if ( m_profiles[ i ].profile_id == profile.profile_id )
		{
			m_profiles[ i ]	= profile;
			m_profiles[ i ].team	= team_undefined;
			return i;
		}
	}

	return -1;
}

bool lobby_client::read_enumerate_inventory_info( network_core::packet_reader& reader )
{
	u32 const count	= reader.r< u32 >( );

	m_inventory_item_instances.resize	( count );
	if ( count )
		reader.r	( &m_inventory_item_instances[ 0 ], count * sizeof( inventory_item_instance ), count * sizeof( inventory_item_instance ) );

	return true;
}

faction_price const& lobby_client::price(
	const u8	faction_id
)
{
	return m_prices[ faction_id ];
}

u8 lobby_client::read_price_items( network_core::packet_reader& reader )
{
	u8 const faction_id	= reader.r< u8 >( );

	faction_price& price	= m_prices[ faction_id ];
	price.faction_id	= faction_id;
	price.count			= reader.r< u16 >( );

	if ( price.count )
	{
		price.items	= ALLOC( price_item, price.count );
		reader.r	( price.items, price.count * sizeof( price_item ), price.count * sizeof( price_item ) );
	}

	return faction_id;
}

bool lobby_client::read_profile_slots_restrictions( network_core::packet_reader& reader )
{
	FREE	( m_profile_slot_restrictions );

	m_profile_slot_restrictions_count	= reader.r< u32 >( );
	m_profile_slot_restrictions	= ALLOC( profile_slot_restriction, m_profile_slot_restrictions_count );

	if ( m_profile_slot_restrictions_count )
		reader.r	( m_profile_slot_restrictions, m_profile_slot_restrictions_count * sizeof( profile_slot_restriction ), m_profile_slot_restrictions_count * sizeof( profile_slot_restriction ) );

	return true;
}

bool lobby_client::read_items_compatibility( network_core::packet_reader& reader )
{
	FREE	( m_items_compatibility );

	m_items_compatibilities_count	= reader.r< u32 >( );
	m_items_compatibility	= ALLOC( items_compatibility, m_items_compatibilities_count );

	if ( m_items_compatibilities_count )
		reader.r	( m_items_compatibility, m_items_compatibilities_count * sizeof( items_compatibility ), m_items_compatibilities_count * sizeof( items_compatibility ) );

	return true;
}

bool lobby_client::read_player_skills( network_core::packet_reader& reader )
{
	FREE	( m_player_skills );

	m_player_leveling_info.total_experience			= reader.r< u32 >( );
	m_player_leveling_info.next_level_experience	= reader.r< u32 >( );
	m_player_leveling_info.prev_level_experience	= reader.r< u32 >( );

	m_player_skills_count	= reader.r< u8 >( );
	m_player_skills	= ALLOC( survarium::player_skill, m_player_skills_count );
	if ( m_player_skills_count )
		reader.r	( m_player_skills, m_player_skills_count * sizeof( survarium::player_skill ), m_player_skills_count * sizeof( survarium::player_skill ) );

	FREE	( m_player_perks );

	m_player_perks_count	= reader.r< u8 >( );
	m_player_perks	= ALLOC( u8, m_player_perks_count );
	if ( m_player_perks_count )
		reader.r	( m_player_perks, m_player_perks_count * sizeof( u8 ), m_player_perks_count * sizeof( u8 ) );

	return true;
}

// claude@NOTE: PARKED - the recovered body is
//   m_skills_tree_config = configs::create_binary_config(
//       mutable_buffer( reader.pointer( ), reader.size_to_eof( ) ) );
//   return true;
// but configs::create_binary_config(mutable_buffer) is not linked into the
// MASTER_GOLD exe (its provider configs_lua_config.cpp is excluded from the gold
// core lib), so the call is an LNK2001 unresolved external. Restore once the gold
// core lib exports create_binary_config (build-graph gap, not a source problem).
bool lobby_client::read_player_skills_tree( network_core::packet_reader& reader )
{
	return false;
}

lobby::client_state_enum lobby_client::status( fixed_string< 128 >& dest ) const
{
	switch ( m_status )
	{
	case lobby::surf_lobby_menu:
		dest.assignf	( "Lobby menu. %s", m_last_status_message.c_str( ) );
		break;
	case lobby::in_match_making_order:
		dest.assignf	( "In match making. order[%d] %s", m_match_order_id, m_last_status_message.c_str( ) );
		break;
	case lobby::in_match_making:
		dest.assignf	( "In match making. match [%d] order[%d] %s", m_match_id, m_match_order_id, m_last_status_message.c_str( ) );
		break;
	case lobby::in_match:
		dest.assignf	( "Waiting for match served[%d] order[%d] %s", m_match_id, m_match_order_id, m_last_status_message.c_str( ) );
		break;
	}

	return m_status;
}

bool lobby_client::can_move_item( const u32 item_category_id, const u32 target_slot_id )
{
	if ( target_slot_id == 100 )
		return true;

	for ( u32 i = 0; i < m_profile_slot_restrictions_count; ++i )
	{
		if ( m_profile_slot_restrictions[ i ].slot_dict_id == target_slot_id && m_profile_slot_restrictions[ i ].category_dict_id == item_category_id )
			return true;
	}

	return false;
}

void lobby_client::move_item( vector< relocate_item_descr >& items )
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::inventory_action );
	packet.append	( (u8)0 );
	packet.append	( (u8)items.size( ) );

	for ( vector< relocate_item_descr >::iterator i = items.begin( ); i != items.end( ); ++i )
		( *i ).serialize	( packet );

	m_packet_client.send	( packet );
}

void lobby_client::buy_item(
	const u16		item_dict_id,
	const u32		count,
	const u8		faction_id,
	const bool		use_premium_money
)
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::shop_action );
	packet.append	( (u8)0 );
	packet.append	( item_dict_id );
	packet.append	( count );
	packet.append	( faction_id );
	packet.append	( use_premium_money );
	m_packet_client.send	( packet );
}

void lobby_client::set_player_skills( vectora< survarium::player_skill >& skills, vectora< u8 >& perks )
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::skills_tree_action );
	packet.append	( (u8)0 );

	packet.append	( (u8)skills.size( ) );
	if ( !skills.empty( ) )
		packet.append	( &skills[ 0 ], skills.size( ) * sizeof( survarium::player_skill ) );

	packet.append	( (u8)perks.size( ) );
	if ( !perks.empty( ) )
		packet.append	( &perks[ 0 ], perks.size( ) );

	m_packet_client.send	( packet );
}

void lobby_client::reroll_player_skills( )
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::skills_tree_action );
	packet.append	( (u8)1 );
	m_packet_client.send	( packet );
}

bool lobby_client::check_compatibility( const u32 first_item_id, const u32 second_item_id )
{
	for ( u32 i = 0; i < m_items_compatibilities_count; ++i )
	{
		if ( ( m_items_compatibility[ i ].first_item_dict_id == first_item_id && m_items_compatibility[ i ].second_item_dict_id == second_item_id ) ||
			 ( m_items_compatibility[ i ].first_item_dict_id == second_item_id && m_items_compatibility[ i ].second_item_dict_id == first_item_id ) )
			return true;
	}

	return false;
}

bool lobby_client::read_account_money( network_core::packet_reader& reader )
{
	m_account_money.generic_money	= reader.r< u32 >( );
	m_account_money.premium_money	= reader.r< u32 >( );
	m_player_leveling_info.total_skill_points	= reader.r< u8 >( );
	reader.r_string	( account_nickname_ );
	return true;
}

bool lobby_client::read_service_prices( network_core::packet_reader& reader )
{
	m_service_prices.reroll_cost		= reader.r< u32 >( );
	m_service_prices.add_profile_cost	= reader.r< u32 >( );
	m_service_prices.rename_account_cost	= reader.r< u32 >( );
	return true;
}

bool lobby_client::read_player_reputations( network_core::packet_reader& reader )
{
	FREE	( m_player_reputations );

	m_player_reputations_count	= reader.r< u8 >( );
	m_player_reputations	= ALLOC( player_reputation, m_player_reputations_count );

	if ( m_player_reputations_count )
		reader.r	( m_player_reputations, m_player_reputations_count * sizeof( player_reputation ), m_player_reputations_count * sizeof( player_reputation ) );

	return true;
}

void lobby_client::discard_playing_order_on_connected( )
{
	m_discard_playing_order_on_connected	= true;
}

void lobby_client::discard_playing_order( )
{
	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::discard_playing_order );
	packet.append	( m_match_order_id );
	m_packet_client.send	( packet );
}

void lobby_client::ping_server( )
{
	if ( !m_net_client_connected )
		return;

	network_core::tcp_packet packet( memory::g_mt_allocator );
	packet.append	( (u8)vostok::ping_server );
	packet.append	( m_game.game_time_ms( ) );
	m_packet_client.send	( packet );
}

bool lobby_client::read_ping_server_answer( network_core::packet_reader& reader )
{
	u32 const sent_time	= reader.r< u32 >( );
	float const ping	= ( m_game.game_time_ms( ) - sent_time ) * 0.5f;
	m_game.lobby_menu( ).set_ping	( (s64)ping );
	return true;
}

} // namespace survarium
