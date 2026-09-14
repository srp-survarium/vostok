// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "match_client_impl.h"
#include <vostok/network/message_types.h>
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {
namespace network {

// claude@NOTE: init list byte-aligned, structure matches 2/2; residual is the two
// body statements' udp_match_client set_on_* expansions (base lowers bind ->
// function temp + operator= with extra slots, target's folds are tighter) - the
// boost-function-assign inline-vs-call wall, not source-steerable.
 match_client_impl::match_client_impl(
	boost::asio::io_service&	io_service,
	network_core::udp_match_packets_orderer&	packets_orderer,
	network_core::udp_network_flow_emulator_options const*	options
) :
	m_packets_allocator		( m_packets_storage.elems, 8192 * 300 ),
	m_network_flow_emulator	( options ? NEW( network_core::udp_network_flow_emulator )( *g_allocator, m_packets_allocator, *options ) : 0 ),
	m_client				( io_service, m_packets_allocator, packets_orderer, m_network_flow_emulator ),
	m_state					( waiting_for_permission )
{
	m_client.set_on_packet_received	( boost::bind( &match_client_impl::on_packet_received, this, _1, _2 ) );
	m_client.set_on_disconnect		( boost::bind( &match_client_impl::on_disconnect, this, _1 ) );
}

match_client_impl::~match_client_impl( )
{
	VOSTOK_DELETE_IMPL		( g_allocator, m_network_flow_emulator );
}

// sushi@TODO: Recover the first callback guard's source form; its safe-bool
// conversion still differs from the target's direct negation test.
void match_client_impl::on_packet_received( const u8 message_type, network_core::packet_reader& reader )
{
	switch ( m_state ) {
		case waiting_for_permission:
			if ( message_type == match_server_connection_successful ) {
				ASSERT_U			( reader.eof( ) );
				m_state				= handshaked;
				m_client.set_on_packet_received( m_on_packet_received );

				if ( m_on_connected )
					m_on_connected	( successfully_connected, successfully_handshaked, no_socket_error, connection_successful );

				return;
			}

			LOG_ERROR				( "connection forbidden" );
			if ( m_on_connected )
				m_on_connected		(
					successfully_connected,
					successfully_handshaked,
					no_socket_error,
					invalid_session_id
				);
			break;
		default: NODEFAULT( );
	}
}

// claude@NOTE: structure matches 2/2, the clone/connect statement byte-aligned;
// residual is `m_on_connected = on_connected` (base inlines copy-swap-clear,
// target calls the folded edi-promoted operator=) - the boost-function-assign
// inline-vs-call wall, not source-steerable.
void match_client_impl::connect(
	pcstr const	host,
	const u16	port,
	const u32	current_time_in_ms,
	network_core::udp_match_packet const* const	packet,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	on_connected
)
{
	m_on_connected			= on_connected;
	m_client.connect		( host, port, packet ? clone_packet( *packet ) : 0, current_time_in_ms );
}

// claude@NOTE: structure matches 3/3; residual is both function2 assigns (base
// inlines copy-swap-clear, target calls the folded edi-dest operator=) - the
// boost-function-assign inline-vs-call wall, not source-steerable.
void match_client_impl::set_on_packet_received(
	boost::function< void ( u8, network_core::packet_reader& ) > const&	on_packet_received
)
{
	m_on_packet_received	= on_packet_received;
	if ( m_state == handshaked )
		m_client.set_on_packet_received( m_on_packet_received );
}

network_core::udp_match_packet* match_client_impl::clone_packet( network_core::udp_match_packet const& packet )
{
	network_core::udp_match_packet* const result	= network_core::new_udp_match_packet( m_packets_allocator );
	network_core::packet_reader	reader( packet );
	result->message_type	= packet.message_type;
	result->channel_id		= packet.channel_id;
	result->is_reliable		= packet.is_reliable;
	result->is_ordered		= packet.is_ordered;
	result->m_buffer[ 0 ]	= packet.m_buffer.data( )[ 0 ];
	result->append			( reader.pointer( ), reader.size_to_eof( ) );
	return					result;
}

// claude@NOTE: structure matches 3/3, stmt sizes within +0x1; residual is
// register/slot renames inside the set_on_packet_received bind expansion - an
// LTCG artifact, not source-steerable.
void match_client_impl::disconnect( )
{
	m_state					= waiting_for_permission;
	m_client.set_on_packet_received( boost::bind( &match_client_impl::on_packet_received, this, _1, _2 ) );
	m_client.disconnect		( );
}

// claude@NOTE: structure matches 4/4; residual is the set_on_packet_received bind/
// function-temp lowering slots - the boost-function inline-vs-call wall, not
// source-steerable.
void match_client_impl::on_disconnect(
	const network_core::disconnect_event_types_enum	disconnect_type
)
{
	m_state					= waiting_for_permission;
	m_client.set_on_packet_received( boost::bind( &match_client_impl::on_packet_received, this, _1, _2 ) );
	if ( m_on_disconnect )
		m_on_disconnect		( disconnect_type );
}

} // namespace network
} // namespace vostok
