////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED
#define NETWORK_CORE_PACKET_READER_H_INCLUDED

namespace vostok {
namespace network_core {

class base_packet;


class packet_reader {
public:
	inline							packet_reader	( base_packet const& arg_0 ) { /* no source */ }

			void					r				( void* destination, u32 size );
	inline	char*					r_string		( char* arg_0, u8 arg_1 ) { /* no source */ }

	inline	base_packet const&		get_packet		( ) const { /* no source */ }

			bool					eof				( ) const;
			pcbyte					pointer			( ) const;
			void					advance			( u32 offset );
			u32						size_to_eof		( ) const;

private:
	/* 0x0000 */	base_packet const&		m_packet;
	/* 0x0004 */	pcbyte					m_pointer;
}; // class packet_reader

STATIC_SIZE_ASSERT(packet_reader, 0x8);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_READER_H_INCLUDED