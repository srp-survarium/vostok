////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "match_client_impl.h"
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>

namespace vostok {
namespace network {

// STATE[STUB]: init-list reconstructed from the target disassembly (0x776240:
// arena = 8192 * 300 at this+0, conditional flow-emulator NEW); body stmts
// (lines 34-35) are the matcher's job
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

	// FUNCTION BODY[0x776240]: 2
	// <0x776381>|0x141|+0x067:'34'
	// <0x7763e8>|0x1a8|+0x06b:'35'
	// ******
}

// STATE[STUB]
 match_client_impl::~match_client_impl( )
{
	// FUNCTION BODY[0x775f10]: 1
	// <0x775f1a>|0x00a|+0x01c:'40'
	// ******
}

// STATE[STUB]
void match_client_impl::on_packet_received( const u8 message_type, network_core::packet_reader& reader )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &message_type, &reader );

	// FUNCTION BODY[0x775d90]: 20
	// <0x775da8>|0x018|+0x012:'45'
	// <0>
	// <0x775dba>|0x02a|+0x010:'47'
	// <0x775dca>|0x03a|+0x029:'48'
	// <0x775df3>|0x063|+0x010:'49'
	// <0x775e03>|0x073|+0x01d:'50'
	// <0>
	// <0x775e20>|0x090|+0x018:'52'
	// <0x775e38>|0x0a8|+0x019:'53'
	// <0>
	// <0x775e51>|0x0c1|+0x005:'55'
	// <0x775e56>|0x0c6|+0x077:'56'
	// <0x775ecd>|0x13d|+0x021:'57'
	// <0x775eee>|0x15e|+0x019:'58'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
void match_client_impl::connect(
	pcstr const		host,
	const u16		port,
	const u32		current_time_in_ms,
	network_core::udp_match_packet const* const	packet,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	on_connected
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &host, &port, &current_time_in_ms, &packet, &on_connected );

	// FUNCTION BODY[0x776470]: 2
	// <0x77647a>|0x00a|+0x011:'75'
	// <0x77648b>|0x01b|+0x03d:'76'
	// ******
}

// STATE[STUB]
void match_client_impl::set_on_packet_received(
	boost::function< void( u8, network_core::packet_reader& ) > const&	on_packet_received
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &on_packet_received );

	// FUNCTION BODY[0x775d30]: 3
	// <0x775d40>|0x010|+0x014:'81'
	// <0x775d54>|0x024|+0x00f:'82'
	// <0x775d63>|0x033|+0x01d:'83'
	// ******
}

// STATE[STUB]
network_core::udp_match_packet* match_client_impl::clone_packet( network_core::udp_match_packet const& packet )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &packet );

	// LOCALS
	// network_core::udp_match_packet* const result
	// network_core::packet_reader 		reader
	// ******

	return NULL;

	// FUNCTION BODY[0x776160]: 9
	// <0x77616a>|0x00a|+0x014:'88'
	// <0x77617e>|0x01e|+0x011:'89'
	// <0x77618f>|0x02f|+0x00c:'90'
	// <0x77619b>|0x03b|+0x01b:'91'
	// <0x7761b6>|0x056|+0x023:'92'
	// <0x7761d9>|0x079|+0x022:'93'
	// <0x7761fb>|0x09b|+0x019:'94'
	// <0x776214>|0x0b4|+0x01c:'95'
	// <0x776230>|0x0d0|+0x003:'96'
	// ******
}

// STATE[STUB]
void match_client_impl::disconnect( )
{
	// FUNCTION BODY[0x776080]: 3
	// <0x776091>|0x011|+0x010:'101'
	// <0x7760a1>|0x021|+0x09c:'102'
	// <0x77613d>|0x0bd|+0x011:'103'
	// ******
}

// STATE[STUB]
void match_client_impl::on_disconnect(
	const network_core::disconnect_event_types_enum	disconnect_type
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &disconnect_type );

	// FUNCTION BODY[0x775f80]: 4
	// <0x775f91>|0x011|+0x010:'108'
	// <0x775fa1>|0x021|+0x09c:'109'
	// <0x77603d>|0x0bd|+0x022:'110'
	// <0x77605f>|0x0df|+0x015:'111'
	// ******
}


} // namespace network
} // namespace vostok
