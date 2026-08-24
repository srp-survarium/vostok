////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

inline packet_reader::packet_reader( base_packet const& packet ) :
	m_packet	( packet ),
	m_pointer	( packet.buffer( ) )
{
}

inline void packet_reader::r( void* destination, u32 destination_size, u32 size )
{
	ASSERT_U		( m_pointer >= m_packet.buffer( ) );
	ASSERT_U		( m_pointer <= m_packet.buffer( ) + m_packet.buffer_size( ) );
	ASSERT_U		( m_packet.buffer( ) + m_packet.buffer_size( ) >= ( m_pointer + size ) );

	memory::copy	( destination, destination_size, m_pointer, size );
	m_pointer		+= size;
}

template < typename T >
inline T packet_reader::r( )
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
