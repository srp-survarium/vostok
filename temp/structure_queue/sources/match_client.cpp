////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\match_client.h"

namespace survarium {

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

// STATE[STUB]
network_core::udp_network_flow_emulator_options const* network_flow_emulator_options( )
{
	return NULL;

	// FUNCTION BODY[0x5c73d0]: 15
	// <0>
	// <1>
	// <2>
	// <0x5c73d0>|0x000|+0x002:'23'
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
	// ******
}

// STATE[STUB]
explicit match_client::match_client( network::world& world )
{
	// FUNCTION BODY[0x5c7410]: 0
	// <0x5c7410>|0x000|+0x034:'41'	{
	// <0x5c7444>|0x034|      :'42'	}
	// ******
}

// STATE[STUB]
 match_client::~match_client( )
{
	// FUNCTION BODY[0x5c73e0]: 0
	// <0x5c73e0>|0x000|+0x000:'45'	{
	// <0x5c73e0>|0x000|      :'46'	}
	// ******
}

// STATE[STUB]
void match_client::connect(
	pcstr		host,
	u16			port,
	u32			session_id,
	u32			current_time_in_ms,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	callback
)
{
	// FUNCTION BODY[0x5c7470]: 3
	// <0x5c7477>|0x007|+0x009:'50'
	// <0x5c7480>|0x010|+0x01a:'51'
	// <0x5c749a>|0x02a|+0x01f:'52'
	// ******
}

// STATE[STUB]
void match_client::disconnect( )
{
	// FUNCTION BODY[0x5c73f0]: 1
	// <0x5c73f0>|0x000|+0x000:'64'	{
	// <0>
	// <0x5c73f0>|0x000|      :'66'	}
	// ******
}

// STATE[STUB]
void match_client::enqueue( network_core::udp_match_packet* const packet )
{
	// FUNCTION BODY[0x5c7400]: 2
	// <0x5c7400>|0x000|+0x008:'81'
	// <0x5c7408>|0x008|+0x007:'82'
	// ******
}

// STATE[STUB]
void match_client::send_queued_packets( const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x5c7450]: 3
	// <0>
	// <1>
	// <0x5c7450>|0x000|+0x013:'89'
	// ******
}

} // namespace survarium
