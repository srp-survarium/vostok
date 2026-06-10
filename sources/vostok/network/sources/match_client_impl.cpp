////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "match_client_impl.h"
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {
namespace network {

// STATE[76.81%|PARTIAL]: init list byte-aligned, structure 2/2; residual = the two
// body statements' udp_match_client set_on_* expansions (base lowers the bind ->
// function temp + operator= with extra slots where the target's folds are tighter)
// - the boost::function-assign inline-vs-call wall
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

// STATE[85.43%|PARTIAL]: structure 1/1 (the g_allocator strip_pointer call is kept
// by BOTH sides - the pointer-allocator spelling); residual = the compiler-emitted
// member clears' folded-COMDAT this-convention (esi in target vs ecx in base, the
// receive_response-dtor class)
match_client_impl::~match_client_impl( )
{
	VOSTOK_DELETE_IMPL		( g_allocator, m_network_flow_emulator );
}

// STATE[27.18%|PARTIAL]: structure 11/11; residuals all per-call-site
// inline-vs-call: base inlines packet_reader::eof inside the ASSERT_U eater,
// inlines function2::operator= (set_on_packet_received) and function4::operator()
// (both m_on_connected calls, +0x4f each) where the target calls the COMDATs, and
// picks the and-form safe-bool at L52 where the target uses the operator!+test form
// (per-site, its L57 twin IS the and-form)
// claude@NOTE: previous_state has no PDB local record (LTCG dropped the unused
// symbol once the asserts that read it compiled out), but its line-45 init store
// survives in the target ([ebp-2B4h] temp-region slot there vs our named slot)
// claude@NOTE: the original's __LINE__ immediate pins the LOG to physical line 56
// (the `}` must have shared a line); we keep the natural layout and accept the
// 1-byte immediate residual per the no-line-padding rule
void match_client_impl::on_packet_received( const u8 message_type, network_core::packet_reader& reader )
{
	state const previous_state	= m_state;

	if ( message_type == set_status_ready_for_battle ) {
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

	// STRUCTURE DIFF: target 11 stmts / base 11 stmts
	// SIZE -0x3  | 55 | state const previous_state	= m_state;
	// SIZE +0x24 | 58 | ASSERT_U			( reader.eof( ) );
	// SIZE +0x1b | 60 | m_client.set_on_packet_received( m_on_packet_received );
	// SIZE +0xa  | 62 | if ( m_on_connected )
	// SIZE +0x4f | 63 | m_on_connected	( successfully_connected, ... );
	// SIZE -0x2  | 68 | LOG_ERROR				( "connection forbidden" );
	// SIZE +0x4f | 75 | );
	// VERDICT: STRUCTURE MATCH - all rows are per-call-site inline-vs-call (function4::operator() x2 verified in asm: target calls COMDAT, base inlines safe-bool/throw/get_vtable); non-steerable LTCG.
}

// STATE[59.58%|PARTIAL]: structure 2/2, the clone/connect statement byte-aligned;
// residual = `m_on_connected = on_connected` inlined as copy-swap-clear (target
// calls the folded operator=, edi-promoted) - the boost::function-assign wall
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

// STATE[31.05%|PARTIAL]: structure 3/3; residual = both function2 assigns inlined
// as copy-swap-clear in base where the target calls the folded operator= (edi
// dest) - the boost::function-assign inline-vs-call wall
void match_client_impl::set_on_packet_received(
	boost::function< void ( u8, network_core::packet_reader& ) > const&	on_packet_received
)
{
	m_on_packet_received	= on_packet_received;
	if ( m_state == handshaked )
		m_client.set_on_packet_received( m_on_packet_received );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x22 | 100 | m_on_packet_received	= on_packet_received;
	// SIZE +0x1  | 102 | m_client.set_on_packet_received( m_on_packet_received );
	// VERDICT: STRUCTURE MATCH - sole real SIZE is function2::operator= inline-vs-call (copy-swap-clear inlined in base); non-steerable LTCG.
}

// STATE[68.83%|PARTIAL]: structure 9/9 (field copies byte-aligned); residuals =
// base inlines packet_reader::pointer() at the append site and m_buffer[0]'s
// operator[] on the source side (target keeps one folded call) - per-call-site
// inline-vs-call, plus append's LTCG arg convention at the call boundary
network_core::udp_match_packet* match_client_impl::clone_packet( network_core::udp_match_packet const& packet )
{
	network_core::udp_match_packet* const result	= network_core::new_udp_match_packet( m_packets_allocator );
	network_core::packet_reader	reader( packet );
	result->message_type	= packet.message_type;
	result->channel_id		= packet.channel_id;
	result->is_reliable		= packet.is_reliable;
	result->is_ordered		= packet.is_ordered;
	result->m_buffer[ 0 ]	= packet.m_buffer[ 0 ];
	result->append			( reader.pointer( ), reader.size_to_eof( ) );
	return					result;
}

// STATE[88.16%|PARTIAL]: structure 3/3, stmt sizes within +0x1; residual is
// register/slot renames inside the set_on_packet_received bind expansion
void match_client_impl::disconnect( )
{
	m_state					= waiting_for_permission;
	m_client.set_on_packet_received( boost::bind( &match_client_impl::on_packet_received, this, _1, _2 ) );
	m_client.disconnect		( );
}

// STATE[81.63%|PARTIAL]: structure 4/4; residual = +0x1b on the
// set_on_packet_received expansion (function temp lowering slots) - the
// boost::function inline-vs-call wall
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
