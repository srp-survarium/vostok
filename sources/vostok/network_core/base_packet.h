////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_BASE_PACKET_H_INCLUDED
#define NETWORK_CORE_BASE_PACKET_H_INCLUDED

namespace vostok {
namespace network_core {

class base_packet  {
public:
	inline	explicit	base_packet		( u8* buffer, u32 buffer_size ) : m_buffer( buffer ), m_buffer_size( buffer_size ) {}

	// sushi@NOTE: Considering that the class is private, is there a reason for this constructor
	inline	explicit	base_packet		( ) : m_buffer( NULL ), m_buffer_size( 0 ) {} 

public:
	inline	pbyte		buffer		( ) { return m_buffer; }
	inline	pcbyte		buffer		( ) const { return m_buffer; }
	inline	u32			buffer_size	( ) const { return m_buffer_size; }


private:
	/* 0x0000 */	pbyte		m_buffer;
	/* 0x0004 */	u32			m_buffer_size;
}; // class base_packet

STATIC_SIZE_ASSERT(base_packet, 0x8);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_BASE_PACKET_H_INCLUDED