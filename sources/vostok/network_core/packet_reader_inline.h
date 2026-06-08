////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

inline packet_reader::packet_reader( base_packet const& packet ) :
	m_packet	( packet ),
	m_pointer	( NULL )
{
	/* no source */
}

// STATE[STUB]
inline void packet_reader::r( void* destination, u32 destination_size, u32 size )
{
	// FUNCTION BODY[0x8e5a0]: 6
	// <0x8e5a0>|0x000|+0x00f:'25'
	// <0x8e5af>|0x00f|+0x006:'26'
	// ******
}

// STATE[STUB]
template < typename T >
inline T packet_reader::r( )
{
	return T();
	// FUNCTION BODY[0x8e950]: 3 : r< u8 >
	// <0x8e950>|0x000|+0x009:'33'
	// ******
	// FUNCTION BODY[0xa7800]: 3 : r< u16 >
	// <0xa7800>|0x000|+0x00c:'33'
	// ******
	// FUNCTION BODY[0x96410]: 3 : r< u32 >
	// <0x96410>|0x000|+0x00b:'33'
	// ******
}

// STATE[STUB]
template < int count >
inline char* packet_reader::r_string( char ( &string )[ count ] )
{
	return NULL;
	// FUNCTION BODY[0xa8860]: 1 : r_string< 16 >
	// <0xa8860>|0x000|+0x028:'73'
	// ******
}

inline char* packet_reader::r_string( char* string, u8 count )
{
	return NULL;
	/* no source */
}

inline base_packet const& packet_reader::get_packet( ) const
{
	return m_packet;
}

// STATE[STUB]
inline bool packet_reader::eof( ) const
{
	return false;
	// FUNCTION BODY[0xa7650]: 1
	// <0xa7650>|0x000|+0x00f:'39'
	// ******
}

// STATE[STUB]
inline pcbyte packet_reader::pointer( ) const
{
	return NULL;
	// FUNCTION BODY[0x92cc0]: 1
	// <0x92cc0>|0x000|+0x003:'44'
	// ******
}

// STATE[STUB]
inline void packet_reader::advance( u32 offset )
{
	// FUNCTION BODY[0x131650]: 5
	// <0x13165a>|0x00a|+0x033:'49'
	// <0x13168d>|0x03d|+0x041:'50'
	// <0x1316ce>|0x07e|+0x046:'51'
	// <0>
	// <0x131714>|0x0c4|+0x00f:'53'
	// ******
}

// STATE[STUB]
inline u32 packet_reader::size_to_eof( ) const
{
	return 0;
	// FUNCTION BODY[0xa7660]: 1
	// <0xa7660>|0x000|+0x00a:'58'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_READER_INLINE_H_INCLUDED
