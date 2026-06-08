////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_BASE_PACKET_H_INCLUDED
#define NETWORK_CORE_BASE_PACKET_H_INCLUDED

#include <vostok/network_core/api.h>

namespace vostok {
namespace network_core {

class base_packet {
public:
	inline				base_packet	( ) { /* no source */ }
	inline				base_packet	( pbyte const arg_0, const u32 arg_1 ) { /* no source */ }

	inline	pcbyte		buffer		( ) const { return NULL; }
	inline	pbyte		buffer		( ) { return NULL; }

	inline	u32			buffer_size	( ) const { return 0; }

private:
	/* 0x0000 */	pbyte		m_buffer;
	/* 0x0004 */	u32			m_buffer_size;
}; // class base_packet

STATIC_SIZE_ASSERT(base_packet, 0x8);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_BASE_PACKET_H_INCLUDED
