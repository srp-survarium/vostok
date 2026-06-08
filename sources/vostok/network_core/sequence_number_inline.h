////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_SEQUENCE_NUMBER_INLINE_H_INCLUDED
#define NETWORK_CORE_SEQUENCE_NUMBER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

template < typename T >
inline sequence_number< T >::sequence_number( T number ) :
	m_number	( number )
{
	/* no source */
}

template < typename T >
inline void sequence_number< T >::serialize( udp_match_packet& packet ) { /* no source */ }

template < typename T >
inline void sequence_number< T >::serialize( pbyte& pointer ) { /* no source */ }

template < typename T >
inline bool sequence_number< T >::operator==( sequence_number const& other ) const { /* no source */ }

template < typename T >
inline bool sequence_number< T >::operator!=( sequence_number const& other ) const { /* no source */ }

// STATE[STUB]
template < typename T >
inline bool sequence_number< T >::operator<( sequence_number const& other ) const
{
	return false;
	// FUNCTION BODY[0x131910]: 2 : sequence_number< u16 >
	// <0x131919>|0x009|+0x05f:'66'
	// <0>
	// ******
}

// STATE[STUB]
template < typename T >
inline bool sequence_number< T >::operator<=( sequence_number const& other ) const
{
	return false;
	// FUNCTION BODY[0x132ae0]: 2 : sequence_number< u16 >
	// <0x132ae9>|0x009|+0x05f:'73'
	// <0>
	// ******
}

template < typename T >
inline bool sequence_number< T >::operator>( sequence_number const& other ) const { /* no source */ }

template < typename T >
inline bool sequence_number< T >::operator>=( sequence_number const& other ) const { /* no source */ }

template < typename T >
inline sequence_number< T >& sequence_number< T >::operator++( ) { /* no source */ }

template < typename T >
inline sequence_number< T > sequence_number< T >::operator++( s32 ) { /* no source */ }

template < typename T >
inline sequence_number< T >& sequence_number< T >::operator--( ) { /* no source */ }

template < typename T >
inline sequence_number< T > sequence_number< T >::operator--( s32 ) { /* no source */ }

template < typename T >
inline sequence_number< T > sequence_number< T >::deserialize( packet_reader& reader ) { /* no source */ }

// STATE[STUB]
template < typename T >
inline s32 operator- ( sequence_number< T > const& left, sequence_number< T > const& right )
{
	return 0;
	// FUNCTION BODY[0x137b20]: 4 : sequence_number< u16 >
	// <0x137b23>|0x003|+0x013:'122'
	// <0x137b36>|0x016|+0x023:'123'
	// <0>
	// <0x137b59>|0x039|+0x012:'125'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_SEQUENCE_NUMBER_INLINE_H_INCLUDED
