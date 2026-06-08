////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
//	Author		: Dmitriy Iassenev
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/tcp_packet.h>

namespace vostok {
namespace network_core {

// STATE[90.67%|PARTIAL]: shape exact; sole SIZE is buffer_size()/buffer() accessor
// out-of-line call in target vs inlined in base - not source-fixable our side.
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

	// STRUCTURE DIFF[target 0x6d78e0 | base 0x54aa00]: target 11 / base 11 stmts
	//   1: 0x006 <0xb> | 0x006 <0x9> | u32 buffer_size = packet.buffer_size( );   SIZE
	// .. same ..
	// ; aligned 10, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is buffer_size() accessor out-of-line call (target) vs inlined (base), non-source-fixable.
}

// STATE[75.68%|PARTIAL]: shape exact (1 stmt each); sole SIZE is buffer()/buffer_size()
// accessor out-of-line call in target vs inlined in base - not source-fixable our side.
boost::asio::mutable_buffers_1 buffer_to_receive_into( tcp_packet& packet )
{
	ASSERT( packet.buffer_size() );
	return boost::asio::buffer( packet.buffer(), packet.buffer_size() );

	// STRUCTURE DIFF[target 0x6d79a0 | base 0x54a9b0]: target 2 / base 2 stmts
	//   2: 0x012 <0x30> | 0x012 <0x2e> | return boost::asio::buffer( packet.buffer(), packet.buffer_size() );   SIZE
	// ; aligned 1, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is buffer()/buffer_size() accessor out-of-line call (target) vs inlined (base), non-source-fixable.
}

} // namespace network_core
} // namespace vostok