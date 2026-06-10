////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/udp_match_client.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/udp_network_flow_emulator.h>

namespace vostok {
namespace network_core {

// STATE[79.20%|PARTIAL]: init list + body shape exact; residuals are per-call-site LTCG
// inline-vs-call, both directions: (a) base CALLS math::max(u32,u32) (LTCG reg-promoted)
// where target INLINES the max_integral branchless body - both binaries keep the standalone
// (target 0x0241d0 / base 0x03b4c0), the operator| precedent; (b) base INLINES
// boost::function1::operator= (copy temp + swap + clear, +0xC frame, drops push esi/edi)
// where target calls the ICF-folded COMDAT; plus folded-rep this-convention/temp noise
// around the boost::function default-ctor / bind fold representatives.
udp_match_client::udp_match_client(
	boost::asio::io_service&			io_service,
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
	udp_match_packets_orderer&			packets_orderer,
	udp_network_flow_emulator* const	network_flow_emulator
) :
	m_connection		(
		m_socket,
		m_server_endpoint,
		packets_allocator,
		packets_orderer,
		120000,
		network_flow_emulator ? math::max( 250u, network_flow_emulator->max_ping_time_in_ms( ) * 6 ) : 500,
		33,
		"client"
	),
	m_socket			( io_service ),
	m_io_service		( io_service ),
	m_packets_allocator	( packets_allocator ),
	m_network_flow_emulator	( network_flow_emulator ),
	m_time_in_ms		( 0 ),
	m_is_receiving		( false )
{
	m_connection.set_on_disconnect( boost::bind( &udp_match_client::on_disconnect, this, _1 ) );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x1b | 38 | m_connection.set_on_disconnect( boost::bind( &udp_match_client::on_disconnect, this, _1 ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE row is LTCG inline-vs-call both ways (base inlines function1::operator=, target inlines math::max in the init region); non-steerable.
}

// STATE[100%|DONE]
void udp_match_client::on_error( client_error_codes_enum, boost::system::error_code )
{
	m_connection.instant_disconnect( disconnected_by_connection_lost );
}

// STATE[STUB]
void udp_match_client::process_incoming_packet( packet_reader& reader, boost::asio::ip::udp::endpoint const& endpoint )
{
	// FUNCTION BODY[0x758670]: 6
	// <0x75867f>|0x00f|+0x03a:'78'
	// <0x7586b9>|0x049|+0x028:'79'
	// <0x7586e1>|0x071|+0x002:'80'
	// <0>
	// <0x7586e3>|0x073|+0x00c:'82'
	// <0x7586ef>|0x07f|+0x02c:'83'
	// ******
}

// claude@NOTE: handle_receive must sit at its original source lines - the three
// LOG_ERROR sites compile __LINE__ immediates (97/103/109) into the bytes; the
// padding below stands in for process_incoming_packet's original body span
// (another match unit) so those land correctly.








// STATE[93.54%|PARTIAL]: 23/23 stmts aligned, all slots/frame identical; residuals are the
// shared LOG_ERROR helper machinery (base builds the boost::function1 callback at the append
// site where target builds it at block entry, swapping the two guarded temp cleanups - the
// http_client::on_error precedent) and stmt-118 packet.buffer() inline-vs-call, both LTCG.
void udp_match_client::handle_receive( boost::system::error_code const& error_code, const u32 bytes_transferred )
{
	check_consistency		( );

	ASSERT					( UNKNOWN_EXPRESSION_T( m_is_receiving ) );
	m_is_receiving			= false;

	if ( error_code ) {
		LOG_ERROR			(
			"error during reading from socket: %s\r\n",
			error_code.message( ).c_str( )
		);
		on_error			( unable_to_read_from_socket, error_code );
		return;
	}

	if ( !bytes_transferred ) {
		LOG_ERROR			( "unable to read from socket\r\n" );
		on_error			( unable_to_read_from_socket, error_code );
		return;
	}

	if ( m_server_endpoint != m_remote_endpoint ) {
		LOG_ERROR			( "unexpected sender\r\n" );
		on_error			( unable_to_read_from_socket, error_code );
		return;
	}

	if ( m_network_flow_emulator ) {
		m_network_flow_emulator->on_packet_received( m_receive_buffer.c_array( ), bytes_transferred, m_remote_endpoint, m_time_in_ms, m_connection.unacknowledged_packets_count( ) );
	}
	else {
		packet_reader	reader( base_packet( m_receive_buffer.c_array( ), bytes_transferred ) );
		process_incoming_packet( reader, m_remote_endpoint );
	}

	check_consistency		( );

	if ( !m_connection.is_disconnected( ) )
		start_receiving		( );

	// STRUCTURE DIFF: target 23 stmts / base 23 stmts
	// SIZE -0x3 | 97  | 		);
	// SIZE -0x3 | 103 | 		LOG_ERROR			( "unable to read from socket\r\n" );
	// SIZE +0x1 | 108 | 	if ( m_server_endpoint != m_remote_endpoint ) {
	// SIZE -0x5 | 109 | 		LOG_ERROR			( "unexpected sender\r\n" );
	// SIZE -0x5 | 118 | 		packet_reader	reader( base_packet( m_receive_buffer.c_array( ), bytes_transferred ) );
	// SIZE -0x1 | 119 | 		process_incoming_packet( reader, m_remote_endpoint );
	// VERDICT: STRUCTURE MATCH (shape ok) - 23/23 aligned; rows are LTCG: function1-ctor fold-rep convention/schedule at the 3 LOG sites (-3/-3/-5), packet.buffer() inline-vs-call (-5), register-form +/-1 (108/119); non-steerable.
}

// STATE[STUB]
void udp_match_client::start_receiving( )
{
	// FUNCTION BODY[0x758bf0]: 11
	// <0x758bff>|0x00f|+0x00c:'130'
	// <0x758c0b>|0x01b|+0x00d:'131'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x758c18>|0x028|+0x0c8:'140'
	// ******
}

// STATE[100%|DONE]
void udp_match_client::connect(
	pcstr const					host,
	const u16					port,
	udp_match_packet* const		packet,
	const u32					current_time_in_ms
)
{
	if ( m_socket.is_open( ) )
		m_socket.close		( );
	m_socket.open			( boost::asio::ip::udp::v4( ) );
	m_socket.bind			( boost::asio::ip::udp::endpoint( ) );

	m_server_endpoint		= boost::asio::ip::udp::endpoint( boost::asio::ip::address::from_string( host ), port );

	m_connection.connect	( packet );

	check_consistency		( );
	start_receiving			( );

	m_connection.send_queued_packets( current_time_in_ms );
	check_consistency		( );
}

// STATE[100%|DONE]
void udp_match_client::disconnect( )
{
	m_connection.disconnect( );
}

// STATE[STUB]
void udp_match_client::enqueue( udp_match_packet* packet )
{
	// FUNCTION BODY[0x758590]: 8
	// <0x7585a1>|0x011|+0x016:'168'
	// <0x7585b7>|0x027|+0x00c:'169'
	// <0>
	// <0x7585c3>|0x033|+0x005:'171'
	// <0x7585c8>|0x038|+0x07a:'172'
	// <0x758642>|0x0b2|+0x016:'173'
	// <0>
	// <0x758658>|0x0c8|+0x008:'175'
	// ******
}

// STATE[STUB]
void udp_match_client::send_queued_packets( const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x758af0]: 10
	// <0x758af9>|0x009|+0x010:'180'
	// <0x758b09>|0x019|+0x0a1:'181'
	// <0x758baa>|0x0ba|+0x016:'182'
	// <0x758bc0>|0x0d0|+0x002:'183'
	// <0>
	// <1>
	// <0x758bc2>|0x0d2|+0x00c:'186'
	// <0x758bce>|0x0de|+0x008:'187'
	// <0x758bd6>|0x0e6|+0x00c:'188'
	// <0x758be2>|0x0f2|+0x008:'189'
	// ******
}

// STATE[99.94%|DONE]: STRUCTURE MATCH (3/3 stmts, sizes byte-identical); sole residual is
// the two locals' ebp slots swapped (-4/-8) - /Od+LTCG slot-allocation noise, no source lever
// (statement order is fixed by the target and names do not affect slots).
void udp_match_client::check_consistency( ) const
{
	u32 const registered_packets_count	= m_packets_allocator.allocated_size( ) / sizeof( udp_match_packet );
	u32 const allocated_count			= ( m_network_flow_emulator ? m_network_flow_emulator->delayed_packets_count( ) : 0 ) + m_connection.packets_count( );
	ASSERT( UNKNOWN_EXPRESSION_T( registered_packets_count == allocated_count ) );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (no diverging rows)
	// VERDICT: STRUCTURE MATCH - byte-equal stmt sizes; assert site verified 1/1 at +0x67 (mov byte[ebp-9],0 / lea eax / call empty-stub, 0xc bytes); residual is the locals' ebp-4/-8 slot swap, LTCG allocation noise.
}

// STATE[100%|DONE]
void udp_match_client::on_disconnect( const disconnect_event_types_enum disconnect_type )
{
	if ( m_is_receiving )
		m_socket.cancel( );

	if ( m_on_disconnect )
		m_on_disconnect( disconnect_type );
}


} // namespace network_core
} // namespace vostok
