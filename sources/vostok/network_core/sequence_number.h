////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED
#define NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED

namespace vostok {
namespace network_core {

class packet_reader;
class udp_match_packet;

// claude@NOTE: PDB shows sequence_number<u8> and sequence_number<u16>; modelled
// as a primary template so both instantiations are well-formed C++. Bodies are
// stubs - matchers add the meat.
template < typename T >
class sequence_number {
public:
	inline						sequence_number	( T number ) : m_number( number ) { }

	inline	void				serialize		( pbyte& stream ) const { VOSTOK_UNREFERENCED_PARAMETER( stream ); }
	inline	void				serialize		( udp_match_packet& packet ) const { VOSTOK_UNREFERENCED_PARAMETER( packet ); }

	inline	bool				operator==		( sequence_number const& other ) const { return m_number == other.m_number; }
	inline	bool				operator!=		( sequence_number const& other ) const { return m_number != other.m_number; }
			bool				operator<		( sequence_number const& other ) const;
			bool				operator<=		( sequence_number const& other ) const;
	inline	bool				operator>		( sequence_number const& other ) const { return other < *this; }
	inline	bool				operator>=		( sequence_number const& other ) const { return other <= *this; }

	inline	sequence_number		operator++		( s32 ) { sequence_number r( *this ); ++m_number; return r; }
	inline	sequence_number&	operator++		( ) { ++m_number; return *this; }
	inline	sequence_number		operator--		( s32 ) { sequence_number r( *this ); --m_number; return r; }
	inline	sequence_number&	operator--		( ) { --m_number; return *this; }

	inline	T					value			( ) const { return m_number; }

	static	sequence_number		deserialize		( packet_reader& reader );

private:
	/* 0x0000 */	T		m_number;
}; // class sequence_number

STATIC_SIZE_ASSERT( sequence_number< u8 >, 0x1 );

template < typename T >
s32 operator-( sequence_number< T > const& left, sequence_number< T > const& right );

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/sequence_number_inline.h>

#endif // #ifndef NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED
