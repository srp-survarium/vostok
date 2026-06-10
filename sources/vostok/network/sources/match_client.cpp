////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network/match_client.h>
#include "match_client_impl.h"
#include "network_world.h"

// TU-local order living at GLOBAL namespace (per the PDB): deferred destruction
// of the match client + its allocators on the network thread
class client_destroyer : public vostok::network::order {
public:
	// STATE[STUB]: init-list only
	inline			client_destroyer	(
						vostok::network_core::udp_match_packets_allocator_ptr const&	responses_allocator,
						vostok::memory::base_allocator&			orders_allocator,
						vostok::network::match_client_impl**	client
					) :
		m_responses_allocator	( responses_allocator ),
		m_orders_allocator		( orders_allocator ),
		m_client				( client )
	{
	}

	virtual			~client_destroyer	( );

	virtual	void	execute				( );

private:
	vostok::network_core::udp_match_packets_allocator_ptr	m_responses_allocator;
	vostok::memory::base_allocator&			m_orders_allocator;
	vostok::network::match_client_impl**	m_client;
}; // class client_destroyer

STATIC_SIZE_ASSERT(client_destroyer, 0x14);

// STATE[STUB]
client_destroyer::~client_destroyer( )
{
	// FUNCTION BODY[0xeb220]: 1
	// <0xeb232>|0x012|+0x049:'118'
	// ******
}

// STATE[STUB]
void client_destroyer::execute( )
{
	// FUNCTION BODY[0xeb2a0]: 2
	// <0xeb2aa>|0x00a|+0x016:'123'
	// <0xeb2c0>|0x020|+0x00d:'124'
	// ******
}

namespace vostok {
namespace network {

// STATE[STUB]
void match_client::create_client( network_core::udp_network_flow_emulator_options const* options )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &options );

	// LOCALS
	// match_client_impl* const 		temp
	// ******

	// FUNCTION BODY[0x75d140]: 9
	// <0x75d151>|0x011|+0x00c:'46'
	// <0>
	// <0x75d15d>|0x01d|+0x081:'48'
	// <0>
	// <1>
	// <2>
	// <0x75d1de>|0x09e|+0x014:'52'
	// <0x75d1f2>|0x0b2|+0x09f:'53'
	// <0x75d291>|0x151|+0x07f:'54'
	// ******
}

// STATE[STUB]
void match_client::create_responses_packets_allocator( )
{
	// FUNCTION BODY[0x75cd00]: 8
	// <0x75cd0a>|0x00a|+0x00c:'59'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x75cd16>|0x016|+0x08a:'66'
	// ******
}

// STATE[STUB]
 match_client::match_client(
	world&		world,
	network_core::udp_match_packets_orderer&	packets_orderer,
	network_core::udp_network_flow_emulator_options const*	options
) :
	m_packets_orderer	( packets_orderer ),
	m_world				( static_cast_checked<network_world&>(world) ),
	m_client			( 0 )
{
	VOSTOK_UNREFERENCED_PARAMETER	( options );

	// FUNCTION BODY[0x75d320]: 11
	// <0x75d4aa>|0x18a|+0x012:'90'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75d4bc>|0x19c|+0x120:'95'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75d5dc>|0x2bc|+0x14e:'100'
	// ******
}

// STATE[STUB]
 match_client::~match_client( )
{
	// LOCALS
	// order* const 					order
	// ******

	// FUNCTION BODY[0x75c6c0]: 7
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75c6ca>|0x00a|+0x0d8:'140'
	// <0x75c7a2>|0x0e2|+0x00d:'141'
	// <0x75c7af>|0x0ef|+0x012:'142'
	// ******
}

// STATE[STUB]
void match_client::on_connected(
	connection_error_types_enum			connection_error,
	handshaking_error_types_enum		handshaking_error,
	socket_error_types_enum				socket_error,
	lobby_server_message_types_enum		lobby_error
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &connection_error, &handshaking_error, &socket_error, &lobby_error );

	// FUNCTION BODY[0x75cbf0]: 7
	// <0x75cc01>|0x011|+0x022:'152'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75cc23>|0x033|+0x0d0:'157'
	// <0>
	// ******
}

// STATE[STUB]
void match_client::connect(
	pcstr const		host,
	const u16		port,
	const u32		current_time_in_ms,
	network_core::udp_match_packet* const	packet,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	callback
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &host, &port, &current_time_in_ms, &packet, &callback );

	// LOCALS
	// boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const& on_connected
	// ******

	// FUNCTION BODY[0x75d8e0]: 19
	// <0x75d8f8>|0x018|+0x014:'169'
	// <0x75d90c>|0x02c|+0x06d:'170'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <0x75d979>|0x099|+0x1d4:'187'
	// ******
}

// STATE[STUB]
void match_client::disconnect( )
{
	// FUNCTION BODY[0x75ca80]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75ca93>|0x013|+0x14d:'196'
	// ******
}

// STATE[STUB]
void enqueue_impl(
	match_client_impl** const			client,
	network_core::udp_match_packet&		packet
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &client, &packet );

	// FUNCTION BODY[0x75ca40]: 1
	// <0x75ca46>|0x006|+0x02b:'201'
	// ******
}

// STATE[STUB]
void match_client::enqueue( network_core::udp_match_packet* packet )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &packet );

	// FUNCTION BODY[0x75cf90]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x75cfa2>|0x012|+0x18f:'214'
	// ******
}

// STATE[STUB]
void match_client::send_queued_packets( const u32 current_time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &current_time_in_ms );

	// FUNCTION BODY[0x75d740]: 7
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x75d752>|0x012|+0x182:'225'
	// ******
}

// STATE[STUB]
void match_client::on_packet_received_impl( const u8 message_type, network_core::packet_reader& reader )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &message_type, &reader );

	// FUNCTION BODY[0x75c860]: 2
	// <0x75c86f>|0x00f|+0x021:'230'
	// <0x75c890>|0x030|+0x01a:'231'
	// ******
}

// STATE[STUB]
void match_client::on_packet_received( const u8 message_type, network_core::packet_reader& reader )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &message_type, &reader );

	// LOCALS
	// network_core::udp_match_packet* const packet<1>
	// ******

	// FUNCTION BODY[0x75cdb0]: 13
	// <0x75cdc5>|0x015|+0x022:'236'
	// <0x75cde7>|0x037|+0x017|[1]:'237'
	// <0x75cdfe>|0x04e|+0x01c:'238'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x75ce1a>|0x06a|+0x168:'247'
	// <0>
	// ******
}

// STATE[STUB]
void match_client::on_disconnect_impl( network_core::disconnect_event_types_enum type )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &type );

	// FUNCTION BODY[0x75c810]: 2
	// <0x75c81f>|0x00f|+0x021:'253'
	// <0x75c840>|0x030|+0x015:'254'
	// ******
}

// STATE[STUB]
void match_client::on_disconnect( network_core::disconnect_event_types_enum type )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &type );

	// FUNCTION BODY[0x75c900]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75c913>|0x013|+0x123:'263'
	// ******
}

// STATE[STUB]
network_core::udp_match_packet* match_client::new_packet( const u8 message_type )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &message_type );

	// LOCALS
	// network_core::udp_match_packet* const result
	// ******

	return NULL;

	// FUNCTION BODY[0x75c8b0]: 4
	// <0x75c8b9>|0x009|+0x014:'268'
	// <0>
	// <0x75c8cd>|0x01d|+0x021:'270'
	// <0x75c8ee>|0x03e|+0x003:'271'
	// ******
}

// STATE[STUB]
bool match_client::is_connected( ) const
{
	return false;

	// FUNCTION BODY[0x75c670]: 1
	// <0x75c679>|0x009|+0x03f:'276'
	// ******
}

// STATE[STUB]
bool match_client::is_disconnected( ) const
{
	return false;

	// FUNCTION BODY[0x75c620]: 1
	// <0x75c629>|0x009|+0x03f:'281'
	// ******
}

// STATE[STUB]
u32 match_client::last_receive_time_in_ms( ) const
{
	return 0;

	// FUNCTION BODY[0x75c5f0]: 2
	// <0x75c5f9>|0x009|+0x00c:'286'
	// <0x75c605>|0x015|+0x011:'287'
	// ******
}


} // namespace network
} // namespace vostok
