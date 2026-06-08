////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED
#define NETWORK_CORE_PACKET_READER_H_INCLUDED

#include <vostok/network_core/base_packet.h>

namespace vostok {
namespace network_core {

class packet_reader {
public:
	inline						packet_reader	( base_packet const& packet ) : m_packet( packet ), m_pointer( packet.buffer( ) ) { }

			void				r				( void* destination, u32 destination_size, u32 size );

	template < typename T >
			T					r				( );

	template < u32 size >
			char*				r_string		( char ( & string )[ size ] );

	inline	base_packet const&	get_packet		( ) const { return m_packet; }

			bool				eof				( ) const;
			pcbyte				pointer			( ) const;
			void				advance			( u32 offset );
			u32					size_to_eof		( ) const;

private:
	/* 0x0000 */	base_packet const&		m_packet;
	/* 0x0004 */	pcbyte					m_pointer;
}; // class packet_reader

STATIC_SIZE_ASSERT( packet_reader, 0x8 );

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/packet_reader_inline.h>

#endif // #ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED
