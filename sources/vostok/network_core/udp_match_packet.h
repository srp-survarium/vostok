////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED

#include <boost/array.hpp>
#include <boost/intrusive/set_hook.hpp>

#include <vostok/memory_single_size_buffer_allocator.h>

#include <vostok/network_core/packet.h>
#include <vostok/network_core/sequence_number.h>
#include <vostok/network_core/udp_match_types.h>

namespace vostok {
namespace network_core {

class udp_match_packet : public packet< udp_match_packet > {
public:
	struct helper {
		static	inline	void	call_constructor	( udp_match_packet& packet ) { VOSTOK_UNREFERENCED_PARAMETER( packet ); }
		static	inline	void	call_destructor		( udp_match_packet& packet ) { VOSTOK_UNREFERENCED_PARAMETER( packet ); }
	}; // struct helper

public:
	inline				udp_match_packet	( ) { }
	inline				~udp_match_packet	( ) { }

	inline	u32			allocated_size		( ) const { return 0; }

	inline	pcbyte		buffer_to_send		( ) const { return m_buffer.data( ); }
	inline	pbyte		buffer_to_send		( ) { return m_buffer.c_array( ); }

	inline	u8			header_size			( ) const { return 0; }
	inline	u32			buffer_to_send_size	( ) const { return 0; }

	inline	void		reallocate			( const u32 arg_0 ) { VOSTOK_UNREFERENCED_PARAMETER( arg_0 ); }

	// the channel's boost::intrusive::set names &udp_match_packet::set_member_hook /
	// ::next, and the connection drives the lists - both reach the private members
	friend	class		udp_match_connection;

private:
	/* 0x0000 */	/* packet< udp_match_packet > */
	/* 0x0008 */	boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >	set_member_hook;
	/* 0x0018 */	udp_match_client_session*		client_session;
	/* 0x001c */	udp_match_packet*				next;
	/* 0x0020 */	u32								last_send_time_in_ms;
	/* 0x0024 */	sequence_number< u8 >			sequence_id;
	/* 0x0026 */	sequence_number< u16 >			order_id;
	/* 0x0028 */	u8								message_type;
	/* 0x0029 */	u8								send_count;
	/* 0x002a */	u8								channel_id	: 6;
	/* 0x002a */	u8								is_reliable	: 1;
	/* 0x002a */	u8								is_ordered	: 1;
	/* 0x002b */	boost::array< u8, 256 >			m_buffer;
}; // class udp_match_packet

STATIC_SIZE_ASSERT( udp_match_packet, 0x12C );


// STATE[STUB]
inline udp_match_packet* new_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
)
{
	VOSTOK_UNREFERENCED_PARAMETER( allocator );

	// LOCALS
	// udp_match_packet* const 			result
	// ******

	return NULL;

	// FUNCTION BODY[0xeaa00]: 3
	// <0xeaa06>|0x006|+0x00b:'118'
	// <0xeaa11>|0x011|+0x02e:'119'
	// <0xeaa3f>|0x03f|+0x003:'120'
	// ******
}

// STATE[STUB]
inline void delete_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator,
	udp_match_packet*&		packet
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( allocator, packet );

	// LOCALS
	// void* 							buffer
	// ******

	// FUNCTION BODY[0xeaa50]: 4
	// <0xeaa56>|0x006|+0x00e:'125'
	// <0xeaa64>|0x014|+0x008:'126'
	// <0xeaa6c>|0x01c|+0x03b:'127'
	// <0xeaaa7>|0x057|+0x009:'128'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED
