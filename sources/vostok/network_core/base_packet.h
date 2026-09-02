// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_BASE_PACKET_H_INCLUDED
#define NETWORK_CORE_BASE_PACKET_H_INCLUDED

#include <vostok/network_core/api.h>

namespace vostok {
namespace network_core {

class base_packet {
public:
	inline				base_packet	( ) : m_buffer( NULL ), m_buffer_size( 0 ) { }
	inline				base_packet	( pbyte const buffer, const u32 buffer_size ) : m_buffer( buffer ), m_buffer_size( buffer_size ) { }

	inline	pcbyte		buffer		( ) const { return m_buffer; }
	inline	pbyte		buffer		( ) { return m_buffer; }

	inline	u32			buffer_size	( ) const { return m_buffer_size; }

protected:
	/* 0x0000 */	pbyte		m_buffer;
	/* 0x0004 */	u32			m_buffer_size;
}; // class base_packet

STATIC_SIZE_ASSERT(base_packet, 0x8);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_BASE_PACKET_H_INCLUDED
