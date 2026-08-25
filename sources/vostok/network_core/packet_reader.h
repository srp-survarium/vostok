////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED
#define NETWORK_CORE_PACKET_READER_H_INCLUDED

#include <vostok/network_core/base_packet.h>

namespace vostok {
namespace network_core {

class packet_reader {
public:
	inline	explicit			packet_reader	( base_packet const& packet );

	inline	void				r				( void* destination, u32 destination_size, u32 size );
	template < typename T >
	__declspec( noinline )	T			r				( );

	template < int count >
	inline	char*				r_string		( char ( &string )[ count ] );
	inline	char*				r_string		( char* string, u8 buffer_size );

	inline	base_packet const&	get_packet		( ) const;

	inline	bool				eof				( ) const;

	// claude@MATCH: public per the target mangling ?pointer@..@@QBEPBEXZ (Q = public).
	inline	pcbyte				pointer			( ) const;

	inline	void				advance			( u32 offset );

	inline	u32					size_to_eof		( ) const;

private:
	packet_reader&				operator=			( packet_reader const& other );

	/* 0x0000 */	base_packet const&		m_packet;
	/* 0x0004 */	pcbyte					m_pointer;
}; // class packet_reader

STATIC_SIZE_ASSERT(packet_reader, 0x8);

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/packet_reader_inline.h>

#endif // #ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED
