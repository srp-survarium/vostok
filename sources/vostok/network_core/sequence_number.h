////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED
#define NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED

namespace vostok {
namespace network_core {

class packet_reader;
class udp_match_packet;

template < typename T >
class sequence_number {
public:
	// default ctor: holders (udp_match_packet, channel, connection) keep these as
	// uninitialised members; the PDB never recorded this (no instantiation) but the
	// aggregates need it to be default-constructible.
	inline						sequence_number	( ) { /* no source */ }

	inline	explicit			sequence_number	( const T number );

	inline	void				serialize		( udp_match_packet& packet );
	inline	void				serialize		( pbyte& pointer );

	inline	bool				operator==		( sequence_number const& other ) const;
	inline	bool				operator!=		( sequence_number const& other ) const;
	inline	bool				operator<		( sequence_number const& other ) const;
	inline	bool				operator<=		( sequence_number const& other ) const;
	inline	bool				operator>		( sequence_number const& other ) const;
	inline	bool				operator>=		( sequence_number const& other ) const;

	inline	sequence_number&	operator++		( );
	inline	sequence_number		operator++		( s32 );
	inline	sequence_number&	operator--		( );
	inline	sequence_number		operator--		( s32 );

	static	inline	sequence_number	deserialize	( packet_reader& reader );

private:
	// the free distance operator reads both operands' raw numbers directly.
	template < typename U >
	friend	s32			operator-	( sequence_number< U > const& left, sequence_number< U > const& right );

	/* 0x0000 */	T		m_number;
}; // class sequence_number

template < typename T >
inline s32 operator- ( sequence_number< T > const& left, sequence_number< T > const& right );

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/sequence_number_inline.h>

STATIC_SIZE_ASSERT(vostok::network_core::sequence_number< u8 >, 0x1);
STATIC_SIZE_ASSERT(vostok::network_core::sequence_number< u16 >, 0x2);

#endif // #ifndef NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED
