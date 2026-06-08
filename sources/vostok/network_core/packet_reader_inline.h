////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PACKET_READER_INLINE_H_INCLUDED
#define PACKET_READER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
inline void packet_reader::r( void* const destination, const u32 destination_size, const u32 size )
{
	VOSTOK_UNREFERENCED_PARAMETERS( destination, destination_size, size );

	// FUNCTION BODY[0x8e5a0]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x8e5a0>|0x000|+0x00f:'25'
	// <0x8e5af>|0x00f|+0x006:'26'
	// ******
}

// STATE[STUB]
template < typename T >
T packet_reader::r( )
{
	return T( );

	// FUNCTION BODY[0x96410]: 3 (r<u32>); [0xa7800] r<u16>; [0x8e950] r<u8>
	// <0>
	// <0x96410>|0x000|+0x00b:'33'
	// <0>
	// ******
}

// STATE[STUB]
template < u32 size >
char* packet_reader::r_string( char ( & string )[ size ] )
{
	VOSTOK_UNREFERENCED_PARAMETER( string );

	return NULL;

	// FUNCTION BODY[0xa8860]: 1
	// <0xa8860>|0x000|+0x028:'73'
	// ******
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
inline void packet_reader::advance( const u32 offset )
{
	VOSTOK_UNREFERENCED_PARAMETER( offset );

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

#endif // #ifndef PACKET_READER_INLINE_H_INCLUDED
