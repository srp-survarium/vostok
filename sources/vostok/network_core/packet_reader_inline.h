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

inline void packet_reader::r( void* destination, u32 destination_size, u32 const size )
{
	ASSERT_U		( m_pointer >= m_packet.buffer( ) );
	ASSERT_U		( m_pointer <= m_packet.buffer( ) + m_packet.buffer_size( ) );
	ASSERT_U		( m_packet.buffer( ) + m_packet.buffer_size( ) >= ( m_pointer + size ) );

	memory::copy	( destination, destination_size, m_pointer, size );
	m_pointer		+= size;
}

// EXPERIMENT (exp/packet-reader-noinline): force MSVC to keep r<T> out-of-line.
template < typename T >
__declspec( noinline ) T packet_reader::r( )
{
	T				result;
	r				( &result, sizeof( result ), sizeof( result ) );
	return			( result );
}

template < int count >
inline char* packet_reader::r_string( char ( &string )[ count ] )
{
	return r_string( &string[ 0 ], (u8)std::min( sizeof( string ), size_t( 255 ) ) );
}

inline char* packet_reader::r_string( char* string, u8 buffer_size )
{
	u8 const string_length	= r< u8 >( );
	ASSERT			( string_length < 255 );
	r				( string, buffer_size, string_length );
	string[ string_length ]	= 0;
	return			string;
}

inline base_packet const& packet_reader::get_packet( ) const
{
	return m_packet;
}

inline bool packet_reader::eof( ) const
{
	return m_pointer == m_packet.buffer( ) + m_packet.buffer_size( );
}

inline pcbyte packet_reader::pointer( ) const
{
	return m_pointer;
}

inline void packet_reader::advance( u32 offset )
{
	ASSERT_U	( m_pointer >= m_packet.buffer( ) );
	ASSERT_U	( m_pointer <= m_packet.buffer( ) + m_packet.buffer_size( ) );
	ASSERT_U	( m_packet.buffer( ) + m_packet.buffer_size( ) >= ( m_pointer + offset ) );

	m_pointer	+= offset;
}

inline u32 packet_reader::size_to_eof( ) const
{
	return m_packet.buffer_size( ) - u32( m_pointer ) + u32( m_packet.buffer( ) );
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED
