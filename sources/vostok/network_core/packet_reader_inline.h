////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// claude@MATCH: every target inline site (udp_match_client::handle_receive,
// udp_match_connection::is_low_level_packet stmt 1, process_incoming_packet<..>
// L141 subpacket_reader) stores m_packet then CALLS the folded
// base_packet::buffer() const into m_pointer - the ctor inits m_pointer from
// the packet's buffer, not NULL.
inline packet_reader::packet_reader( base_packet const& packet ) :
	m_packet	( packet ),
	m_pointer	( packet.buffer( ) )
{
}

// Body shapes below are exact against the target disasm. The base instances come
// from an address-of anchor (network_core/tcp_packet.cpp) in a single /Ot TU, so
// they emit debug-quality COMDATs (frame ptr + stack temps) and score below the
// target's whole-program-inlined codegen - a single-TU anchor cannot reproduce
// that. The bodies, not the anchor codegen, are the deliverable.

// STATE[PARTIAL]: memcpy(dst,m_pointer,size); m_pointer+=size - matches target shape.
inline void packet_reader::r( void* destination, u32 destination_size, u32 size )
{
	ASSERT			( size <= destination_size );
	memcpy( destination, m_pointer, size );
	m_pointer		+= size;
}

// STATE[PARTIAL]: read *m_pointer, advance by sizeof(T) - target shape exact.
template < typename T >
inline T packet_reader::r( )
{
	T const result	= *static_cast< T const* >( static_cast< pcvoid >( m_pointer ) );
	m_pointer		+= sizeof( T );
	return result;
}

// STATE[PARTIAL]: forwards to the u8-counted overload (inlined in target).
template < int count >
inline char* packet_reader::r_string( char ( &string )[ count ] )
{
	return r_string( string, count );
}

// STATE[PARTIAL]: read u8 length prefix, memcpy, null-terminate - target shape exact.
inline char* packet_reader::r_string( char* string, u8 count )
{
	u8 const length	= r< u8 >( );
	ASSERT			( length < count );
	memcpy( string, m_pointer, length );
	m_pointer		+= length;
	string[ length ]= 0;
	return string;
}

inline base_packet const& packet_reader::get_packet( ) const
{
	return m_packet;
}

// STATE[PARTIAL]: m_pointer == buffer()+buffer_size() - target shape exact.
inline bool packet_reader::eof( ) const
{
	return m_pointer == m_packet.buffer( ) + m_packet.buffer_size( );
}

// STATE[PARTIAL]: returns m_pointer ([+4]) - target shape exact.
inline pcbyte packet_reader::pointer( ) const
{
	return m_pointer;
}

// STATE[PARTIAL]: net body (m_pointer += offset) matched; the three leading bounds
// ASSERTs resolve to mis-symbolized inline buffer()/buffer_size() accessors in target.
inline void packet_reader::advance( u32 offset )
{
	ASSERT	( pointer( ) );
	ASSERT	( pointer( ) <= m_packet.buffer( ) + m_packet.buffer_size( ) );
	ASSERT	( pointer( ) + offset <= m_packet.buffer( ) + m_packet.buffer_size( ) );
	m_pointer	+= offset;
}

// STATE[PARTIAL]: (buffer()+buffer_size()) - m_pointer, target's buffer_size - ptr + buffer order.
inline u32 packet_reader::size_to_eof( ) const
{
	return m_packet.buffer_size( ) - u32( m_pointer ) + u32( m_packet.buffer( ) );
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED
