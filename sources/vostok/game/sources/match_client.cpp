////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "match_client.h"

#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/udp_match_packet.h>

namespace survarium {

// TU statics behind the flow-emulator console commands (compiler-generated
// dynamic initializers); a matcher recovers their types/initializers from the
// init asm when this TU is enabled.
/*
// STATE[STUB]
void `dynamic initializer for 's_flow_emulator''( )
{
	// FUNCTION BODY[0x7d8c20]
	// <0x7d8c20>|0x000|      :'13'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_lost_packets''( )
{
	// FUNCTION BODY[0x7d8c40]
	// <0x7d8c40>|0x000|      :'14'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_min_ping''( )
{
	// FUNCTION BODY[0x7d8c60]
	// <0x7d8c60>|0x000|      :'15'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_max_ping''( )
{
	// FUNCTION BODY[0x7d8c80]
	// <0x7d8c80>|0x000|      :'16'	{
	// ******
}
*/

// the flow-emulator console vars (s_flow_emulator/s_lost_packets/s_min_ping/
// s_max_ping) and the options assembled from them are compiled out in
// MASTER_GOLD; the function collapses to returning no options
network_core::udp_network_flow_emulator_options const* network_flow_emulator_options( )
{
	return NULL;
}

 match_client::match_client( network::world& world ) :
	m_client( world, m_packets_orderer, network_flow_emulator_options( ) ),
	m_last_send_queed_packets_time_in_ms( 0 ),
	m_are_there_any_packets_to_send( false )
{
}

 match_client::~match_client( )
{
}

void match_client::connect(
	pcstr		host,
	u16			port,
	u32			session_id,
	u32			current_time_in_ms,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	callback
)
{
	network_core::udp_match_packet* const	packet	= m_client.new_packet( ( u8 )0x40 );
	packet->append			( &session_id, ( u32 )4 );
	m_client.connect		( host, port, current_time_in_ms, packet, callback );
}

void match_client::disconnect( )
{
	m_client.disconnect		( );
}

void match_client::enqueue( network_core::udp_match_packet* const packet )
{
	m_client.enqueue		( packet );
	m_are_there_any_packets_to_send	= true;
}

void match_client::send_queued_packets( const u32 current_time_in_ms )
{
	m_last_send_queed_packets_time_in_ms	= current_time_in_ms;
	m_are_there_any_packets_to_send			= false;
	m_client.send_queued_packets			( current_time_in_ms );
}

} // namespace survarium
