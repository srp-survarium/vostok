// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

inline tcp_packet::tcp_packet( memory::base_allocator& allocator ) :
	m_allocator			( allocator ),
	m_allocated_size	( 0 )
{
}

inline tcp_packet::~tcp_packet( )
{
	if ( buffer( ) )
	{
		pbyte real_buffer	= buffer( ) - 3;
		VOSTOK_FREE_IMPL	( m_allocator, real_buffer );
	}
}

inline u32 tcp_packet::allocated_size( ) const
{
	return m_allocated_size;
}

inline void tcp_packet::reallocate( u32 new_size )
{
	m_allocated_size	= new_size;
	m_buffer_size		= std::min( m_buffer_size, m_allocated_size );
	m_buffer			= static_cast< pbyte >( VOSTOK_REALLOC_IMPL( m_allocator, m_buffer ? m_buffer - 3 : 0, new_size + 3, "packet" ) ) + 3;
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED
