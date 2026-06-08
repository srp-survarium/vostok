////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

inline tcp_packet::tcp_packet( memory::base_allocator& allocator ) :
	m_allocator			( allocator ),
	m_allocated_size	( 0 )
{
	/* no source */
}

inline tcp_packet::~tcp_packet( )
{
	if ( buffer( ) )										// <0xa72b0>|0x000|0x000:'21'
	{
		pbyte real_buffer	= buffer( ) - 3;				// <0xa72b6>|0x006|0x006:'23'
		VOSTOK_FREE_IMPL	( m_allocator, real_buffer );	// <0xa72b9>|0x009|0x003:'24'
	}
}

inline u32 tcp_packet::allocated_size( ) const
{
	return m_allocated_size;
}

inline void tcp_packet::reallocate( u32 new_size ) { /* no source */ }

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED
