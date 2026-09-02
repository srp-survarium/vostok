// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "match_client.h"

#include <vostok/command_line_extensions.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/udp_match_packet.h>

// the flow-emulator command-line keys live at file (global) scope - the target's
// dynamic initializers demangle without a survarium:: qualifier. Their string
// members are constant-initialized in .data, so each dynamic initializer is just
// the key ctor's protected_call( protected_key_construct, this ) tail. Read by
// network_flow_emulator_options() outside MASTER_GOLD; literal names are a
// best-guess (the .data they fill is unscored, only the dynamic init scores).
static vostok::command_line::key	s_flow_emulator	( "net_flow_emulator", "", "network", "enable the network flow emulator" );
static vostok::command_line::key	s_lost_packets	( "net_lost_packets", "", "network", "network flow emulator lost packet probability" );
static vostok::command_line::key	s_min_ping		( "net_min_ping", "", "network", "network flow emulator minimum ping in ms" );
static vostok::command_line::key	s_max_ping		( "net_max_ping", "", "network", "network flow emulator maximum ping in ms" );

// the flow-emulator options assembled from the keys are compiled out in
// MASTER_GOLD; the function collapses to returning no options. STATIC at file
// (global) scope - the target delinks it under a plain unmangled name (no
// survarium:: qualifier), so it pairs only as a global-scope file-static; its
// sole caller is the survarium::match_client ctor init-list.
static vostok::network_core::udp_network_flow_emulator_options const* network_flow_emulator_options( )
{
	return NULL;
}

namespace survarium {

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
	packet->append			( session_id );
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
