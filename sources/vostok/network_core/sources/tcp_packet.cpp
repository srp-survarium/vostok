////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "tcp_packet.h"

namespace vostok {
namespace network_core {

// STATE[STUB]
// boost::asio::const_buffers_1 vostok::network_core::buffer_to_send(vostok::network_core::tcp_packet&)
boost::asio::const_buffers_1 buffer_to_send( tcp_packet& packet )
{
	// LOCALS
	// u8* 							buffer
	// u32 							buffer_size
	// ******

	// FUNCTION BODY
	// <0x6e78e6>|0x000|0x000:'14'
	// <0x6e78f1>|0x00b|0x00b:'15'
	// <0x6e78f9>|0x013|0x008:'16'
	// <0x6e78ff>|0x019|0x006:'17'
	// 1
	// <0x6e7920>|0x03a|0x021:'19'
	// <0x6e7929>|0x043|0x009:'20'
	// <0x6e7932>|0x04c|0x009:'21'
	// 1
	// 2
	// <0x6e7957>|0x071|0x025:'24'
	// <0x6e7963>|0x07d|0x00c:'25'
	// <0x6e796a>|0x084|0x007:'26'
	// <0x6e7975>|0x08f|0x00b:'27'
	// ******
}

// STATE[STUB]
// boost::asio::mutable_buffers_1 vostok::network_core::buffer_to_receive_into(vostok::network_core::tcp_packet&)
boost::asio::mutable_buffers_1 buffer_to_receive_into( tcp_packet& packet )
{
	// FUNCTION BODY
	// <0x6e79a6>|0x000|0x000:'32'
	// <0x6e79b2>|0x00c|0x00c:'33'
	// ******
}

} // namespace network_core
} // namespace vostok
