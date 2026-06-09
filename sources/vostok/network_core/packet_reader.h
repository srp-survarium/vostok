////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED
#define NETWORK_CORE_PACKET_READER_H_INCLUDED

#include <vostok/network_core/base_packet.h>

namespace vostok { void use_network_core_packet_reader( ); }

namespace vostok {
namespace network_core {

class packet_reader {
public:
	inline	explicit			packet_reader	( base_packet const& packet );

	inline	void				r				( void* destination, u32 destination_size, u32 size );
	template < typename T >
	inline	T					r				( );

	template < int count >
	inline	char*				r_string		( char ( &string )[ count ] );
	inline	char*				r_string		( char* string, u8 count );

	inline	base_packet const&	get_packet		( ) const;

	inline	bool				eof				( ) const;

private:
	inline	pcbyte				pointer			( ) const;

	// the network_core anchor ODR-uses pointer() by address to keep its standalone body.
	friend	void				::vostok::use_network_core_packet_reader	( );

public:
	inline	void				advance			( u32 offset );

	inline	u32					size_to_eof		( ) const;

private:
	/* 0x0000 */	base_packet const&		m_packet;
	/* 0x0004 */	pcbyte					m_pointer;
}; // class packet_reader

STATIC_SIZE_ASSERT(packet_reader, 0x8);

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/packet_reader_inline.h>

#endif // #ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED
