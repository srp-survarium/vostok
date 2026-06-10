////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/udp_match_client.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/process_packet_predicate.h>
#include <vostok/network_core/custom_alloc_handler.h>

namespace vostok {
namespace network_core {

// STATE[79.20%|PARTIAL]: init list + body shape exact; residuals are per-call-site LTCG
// inline-vs-call, both directions: (a) base CALLS math::max(u32,u32) where target INLINES the
// max_integral body (both keep the standalone, target 0x0241d0 / base 0x03b4c0, operator| precedent);
// (b) base INLINES boost::function1::operator= (+0xC frame) where target calls the ICF-folded COMDAT; plus folded-rep this-convention/temp noise.
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
// STATE[99.86%|PARTIAL]: re-diffed after PR #288's process_incoming_packet<process_packet_predicate> body landed: structure-diff clean, 5/5 stmts, 0xb1 bytes BOTH sides; residual is the frame immediate (base 0xD0 vs target 0xB8 spill area) + ICF naming on the folded assert-eater stubs - not source-steerable.
void udp_match_client::process_incoming_packet( packet_reader& reader, boost::asio::ip::udp::endpoint const& endpoint )
{
	ASSERT_U( endpoint == m_server_endpoint );
	if ( m_network_flow_emulator && m_connection.is_disconnected( ) )
		return;

	ASSERT( UNKNOWN_EXPRESSION_T( !m_connection.is_disconnected( ) ) );
	m_connection.process_incoming_packet( reader, process_packet_predicate( *this ) );
	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// (no diverging rows - clean align, 0xb1 bytes both sides)
	// VERDICT: STRUCTURE MATCH (shape ok) - closed by PR #288's udp_match_connection::
	// process_incoming_packet<process_packet_predicate> body (the dropped call + arg pushes are back);
	// residual is the frame immediate (base 0xD0 vs target 0xB8) + ICF stub naming, not source-steerable.
}

// STATE[93.53%|PARTIAL]: 23/23 stmts aligned, all slots/frame identical; residuals are the LOG __LINE__ immediates (line-padding dropped per policy) plus the
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
	// STRUCTURE DIFF (condensed): 23/23 stmts; SIZE rows: -0x3|97 `);` / -0x3|103 LOG_ERROR(unable to read) / +0x1|108 if(server!=remote) / -0x5|109 LOG_ERROR(unexpected sender) / -0x5|118 packet_reader reader(...) / -0x1|119 process_incoming_packet(...)
	// VERDICT: STRUCTURE MATCH (shape ok) - 23/23 aligned; rows are LTCG: function1-ctor fold-rep convention/schedule at the 3 LOG sites (-3/-3/-5), packet.buffer() inline-vs-call (-5), register-form +/-1 (108/119); non-steerable.
}
// STATE[90.97%|PARTIAL]: frame/slots exact (0xD0) after make_custom_alloc_handler named-return fix; residual is the result-copy (0x12 bytes) our LTCG fails to elide in this standalone emission (target keeps it in the tcp_packet_client::start_reading emission, 100% there - the spelling proof).
void udp_match_client::start_receiving( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_is_receiving ) );
	m_is_receiving				= true;
	m_socket.async_receive_from	(
		boost::asio::buffer( m_receive_buffer ),
		m_remote_endpoint,
		make_custom_alloc_handler	(
			m_handler_allocator,
			boost::bind( &udp_match_client::handle_receive, this, _1, _2 )
		)
	);
	// STRUCTURE DIFF 3/3; SIZE +0x12|130 `);` - VERDICT: STRUCTURE MATCH (shape ok) - sole row is make_custom_alloc_handler's named-return result-copy (6 movs) target LTCG elides in this emission (slot kept, frame 0xD0 exact both sides); non-steerable backend copy-prop variance.
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
// STATE[94.51%|PARTIAL]: shape/strings exact; __LINE__ immediate off by design (line-padding dropped per policy, -0.01); residual is the LOG macro's log_callback_boost ctor COMDAT - target calls it first with this in EAX, base last with this in ESI (+8 frame, reg renames cascade) - ICF/LTCG call-boundary convention, http_client LOG precedent.
void udp_match_client::enqueue( udp_match_packet* packet )
{
	if ( m_connection.is_connected( ) ) {
		m_connection.enqueue		( packet );
	}
	else {
		LOG_ERROR					( "disconnection initiated but new packet has been enqueued" );
		m_connection.delete_packet	( packet );
	}
	check_consistency				( );

	// STRUCTURE DIFF: target 6 stmts / base 6 stmts
	// SIZE -0x3 | 172 | LOG_ERROR					( "disconnection initiated but new packet has been enqueued" );
	// VERDICT: STRUCTURE MATCH (shape ok) - 6/6 aligned (braced-then + cuddled `else {` lands the jmp-over-else on line 171 = target); sole SIZE row is the log_callback_boost function-ctor COMDAT convention (target first/this-in-EAX vs base last/this-in-ESI, +8 frame), LTCG call-boundary residual, http_client precedent.
}

// STATE[100%|DONE]
void udp_match_client::send_queued_packets( const u32 current_time_in_ms )
{
	if ( m_network_flow_emulator ) {
		// claude@MATCH: tick gets the OLD m_time_in_ms - the member is updated only after this block
		m_network_flow_emulator->tick( m_time_in_ms, boost::bind( &udp_match_client::process_incoming_packet, this, _1, _2 ) );
		if ( m_connection.is_disconnected( ) )
			return;
	}

	m_time_in_ms						= current_time_in_ms;
	check_consistency					( );
	m_connection.send_queued_packets	( current_time_in_ms );
	check_consistency					( );
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
