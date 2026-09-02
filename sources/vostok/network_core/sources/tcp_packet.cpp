// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
//	Author		: Dmitriy Iassenev
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {

namespace network_core {

boost::asio::const_buffers_1 buffer_to_send( tcp_packet& packet )
{
	u32 buffer_size = packet.buffer_size( );
	pbyte buffer	= packet.buffer();
	if ( !buffer_size )
		return boost::asio::buffer( static_cast<pcvoid>( NULL ), 0 );

	if ( buffer_size < 256 ) {
		*(buffer - 1) = static_cast<u8>( buffer_size );
		return boost::asio::buffer( static_cast<pcbyte>(buffer - 1), buffer_size + 1 );
	}

	ASSERT( buffer_size < (u32(1) << 16) );
	*(buffer - 1) = 0;
	*static_cast<u16*>(static_cast<pvoid>(buffer - 3)) = static_cast<u16>( buffer_size );
	return boost::asio::buffer( static_cast<pcbyte>(buffer - 3), buffer_size + 3 );
}

boost::asio::mutable_buffers_1 buffer_to_receive_into( tcp_packet& packet )
{
	ASSERT( packet.buffer_size() );
	return boost::asio::buffer( packet.buffer(), packet.buffer_size() );
}

} // namespace network_core
} // namespace vostok
