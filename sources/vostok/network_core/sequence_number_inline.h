// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_SEQUENCE_NUMBER_INLINE_H_INCLUDED
#define NETWORK_CORE_SEQUENCE_NUMBER_INLINE_H_INCLUDED

// The serialize/deserialize bodies touch packet< T >::append and packet_reader::r,
// but sequence_number.h is itself pulled in by udp_match_packet.h, so we cannot
// include those headers here (it would be circular). The bodies are templates,
// instantiated only where both udp_match_packet and packet_reader are already
// complete (the udp_match_connection / temp_include_all translation units), so the
// forward declarations in sequence_number.h suffice for parsing.

namespace vostok {
namespace network_core {

template < typename T >
inline sequence_number< T >::sequence_number( sequence_number const& other ) :
	m_number	( other.m_number )
{
	/* no source */
}

template < typename T >
inline sequence_number< T >::sequence_number( const sequence_number_type number ) :
	m_number	( number )
{
	/* no source */
}

template < typename T >
inline sequence_number< T >& sequence_number< T >::operator=( sequence_number const& other )
{
	m_number	= other.m_number;
	return *this;
}

// serialize(udp_match_packet&): append the raw number via the packet writer; this
// is one of the call sites that ODR-uses packet< T >::append (and keeps it
// out-of-line in the LTCG image).
template < typename T >
inline void sequence_number< T >::serialize( udp_match_packet& packet )
{
	packet.append		( m_number );
}

// serialize(pbyte&): write the number straight into a raw cursor and advance it.
template < typename T >
inline void sequence_number< T >::serialize( pbyte& pointer )
{
	*reinterpret_cast< T* >( pointer )	= m_number;
	pointer			+= sizeof( T );
}

template < typename T >
inline bool sequence_number< T >::operator==( sequence_number const& other ) const
{
	return m_number == other.m_number;
}

template < typename T >
inline bool sequence_number< T >::operator!=( sequence_number const& other ) const
{
	return m_number != other.m_number;
}

// operator< / operator<=: RFC1982-style serial-number comparison. *this precedes
// other when it is the smaller of the two within the wrap-around half-window, or
// when it is the larger one that has wrapped past it. The two clauses differ only
// in the strictness of the first comparison ( < vs <= ), matching the target.
template < typename T >
inline bool sequence_number< T >::operator<( sequence_number const& other ) const
{
	return ( m_number < other.m_number && u32( m_number ) + 0x8000 > other.m_number ) ||
		( other.m_number < m_number && u32( other.m_number ) + 0x8000 <= m_number );
}

template < typename T >
inline bool sequence_number< T >::operator<=( sequence_number const& other ) const
{
	return ( m_number <= other.m_number && u32( m_number ) + 0x8000 > other.m_number ) ||
		( other.m_number < m_number && u32( other.m_number ) + 0x8000 <= m_number );
}

template < typename T >
inline bool sequence_number< T >::operator>( sequence_number const& other ) const
{
	return other < *this;
}

template < typename T >
inline bool sequence_number< T >::operator>=( sequence_number const& other ) const
{
	return other <= *this;
}

template < typename T >
inline sequence_number< T >& sequence_number< T >::operator++( )
{
	++m_number;
	return *this;
}

template < typename T >
inline sequence_number< T > sequence_number< T >::operator++( s32 )
{
	sequence_number	result( *this );
	++m_number;
	return result;
}

template < typename T >
inline sequence_number< T >& sequence_number< T >::operator--( )
{
	--m_number;
	return *this;
}

template < typename T >
inline sequence_number< T > sequence_number< T >::operator--( s32 )
{
	sequence_number	result( *this );
	--m_number;
	return result;
}

// deserialize: pull a raw T off the reader and wrap it.
template < typename T >
inline sequence_number< T > sequence_number< T >::deserialize( packet_reader& reader )
{
	return sequence_number( reader.r< T >( ) );
}

// operator-: signed serial-number distance. When right does not precede left we take
// the direct 16-bit wrapped difference; otherwise the negated mirror distance.
template < typename T >
inline s32 operator- ( sequence_number< T > const& left, sequence_number< T > const& right )
{
	if ( right <= left )
		return s16( left.m_number - right.m_number );

	return -( right - left );
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_SEQUENCE_NUMBER_INLINE_H_INCLUDED
