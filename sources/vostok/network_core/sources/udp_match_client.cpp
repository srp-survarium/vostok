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

// STATE[STUB]
void udp_match_client::handle_receive( boost::system::error_code const& error_code, const u32 bytes_transferred )
{
	// LOCALS
	// packet_reader 					reader<1>
	// ******

	// FUNCTION BODY[0x758730]: 38
	// <0x75874a>|0x01a|+0x00b:'88'
	// <0>
	// <0x758755>|0x025|+0x00c:'90'
	// <0x758761>|0x031|+0x00d:'91'
	// <0>
	// <0x75876e>|0x03e|+0x014:'93'
	// <0>
	// <1>
	// <2>
	// <0x758782>|0x052|+0x0da:'97'
	// <0x75885c>|0x12c|+0x017:'98'
	// <0x758873>|0x143|+0x005:'99'
	// <0>
	// <1>
	// <0x758878>|0x148|+0x00a:'102'
	// <0x758882>|0x152|+0x080:'103'
	// <0x758902>|0x1d2|+0x017:'104'
	// <0x758919>|0x1e9|+0x005:'105'
	// <0>
	// <1>
	// <0x75891e>|0x1ee|+0x036:'108'
	// <0x758954>|0x224|+0x089:'109'
	// <0x7589dd>|0x2ad|+0x017:'110'
	// <0x7589f4>|0x2c4|+0x005:'111'
	// <0>
	// <1>
	// <0x7589f9>|0x2c9|+0x00f:'114'
	// <0x758a08>|0x2d8|+0x058:'115'
	// <0>
	// <0x758a60>|0x330|+0x002:'117'
	// <0x758a62>|0x332|+0x032|[1]:'118'
	// <0x758a94>|0x364|+0x01c:'119'
	// <0>
	// <1>
	// <0x758ab0>|0x380|+0x00b:'122'
	// <0>
	// <0x758abb>|0x38b|+0x019:'124'
	// <0x758ad4>|0x3a4|+0x00b:'125'
	// ******
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

// STATE[STUB]
void udp_match_client::connect(
	pcstr const					host,
	const u16					port,
	udp_match_packet* const		packet,
	const u32					current_time_in_ms
)
{
	// FUNCTION BODY[0x758cf0]: 14
	// <0x758d01>|0x011|+0x029:'145'
	// <0x758d2a>|0x03a|+0x011:'146'
	// <0x758d3b>|0x04b|+0x01c:'147'
	// <0x758d57>|0x067|+0x042:'148'
	// <0>
	// <0x758d99>|0x0a9|+0x04b:'150'
	// <0>
	// <0x758de4>|0x0f4|+0x00f:'152'
	// <0>
	// <0x758df3>|0x103|+0x00b:'154'
	// <0x758dfe>|0x10e|+0x00b:'155'
	// <0>
	// <0x758e09>|0x119|+0x00f:'157'
	// <0x758e18>|0x128|+0x00b:'158'
	// ******
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
