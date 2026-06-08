////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/tcp_packet.h>

namespace vostok {
namespace network_core {

// STATE[STUB]
boost::asio::const_buffers_1 buffer_to_send( tcp_packet& packet )
{
	// LOCALS
	// pbyte const 						buffer
	// const u32 						buffer_size
	// ******

	return boost::asio::const_buffers_1( NULL, 0 );

	// FUNCTION BODY[0x6e78e0]: 14
	// <0x6e78e6>|0x006|+0x00b:'14'
	// <0x6e78f1>|0x011|+0x008:'15'
	// <0x6e78f9>|0x019|+0x006:'16'
	// <0x6e78ff>|0x01f|+0x021:'17'
	// <0x6e7920>|0x040|+0x009:'19'
	// <0x6e7929>|0x049|+0x009:'20'
	// <0x6e7932>|0x052|+0x025:'21'
	// <0x6e7957>|0x077|+0x00c:'24'
	// <0x6e7963>|0x083|+0x007:'25'
	// <0x6e796a>|0x08a|+0x00b:'26'
	// <0x6e7975>|0x095|+0x023:'27'
	// ******
}

// STATE[STUB]
boost::asio::mutable_buffers_1 buffer_to_receive_into( tcp_packet& packet )
{
	return boost::asio::mutable_buffers_1( NULL, 0 );

	// FUNCTION BODY[0x6e79a0]: 2
	// <0x6e79a6>|0x006|+0x00c:'32'
	// <0x6e79b2>|0x012|+0x030:'33'
	// ******
}

} // namespace network_core
} // namespace vostok
