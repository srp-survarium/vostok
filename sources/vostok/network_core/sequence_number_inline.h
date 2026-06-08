////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SEQUENCE_NUMBER_INLINE_H_INCLUDED
#define SEQUENCE_NUMBER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
template < typename T >
bool sequence_number< T >::operator<( sequence_number const& other ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( other );

	return false;

	// FUNCTION BODY[0x131910]: 2
	// <0x131919>|0x009|+0x05f:'66'
	// <0>
	// ******
}

// STATE[STUB]
template < typename T >
bool sequence_number< T >::operator<=( sequence_number const& other ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( other );

	return false;

	// FUNCTION BODY[0x132ae0]: 2
	// <0x132ae9>|0x009|+0x05f:'73'
	// <0>
	// ******
}

// STATE[STUB]
template < typename T >
sequence_number< T > sequence_number< T >::deserialize( packet_reader& reader )
{
	VOSTOK_UNREFERENCED_PARAMETER( reader );

	return sequence_number< T >( 0 );
}

// STATE[STUB]
template < typename T >
s32 operator-( sequence_number< T > const& left, sequence_number< T > const& right )
{
	VOSTOK_UNREFERENCED_PARAMETERS( left, right );

	return 0;

	// FUNCTION BODY[0x137b20]: 4
	// <0x137b23>|0x003|+0x013:'122'
	// <0x137b36>|0x016|+0x023:'123'
	// <0>
	// <0x137b59>|0x039|+0x012:'125'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef SEQUENCE_NUMBER_INLINE_H_INCLUDED
