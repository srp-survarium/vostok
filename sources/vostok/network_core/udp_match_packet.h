////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED

#include <vostok/network_core/packet.h>
#include <vostok/network_core/sequence_number.h>
#include <vostok/memory_single_size_buffer_allocator.h>
#include <boost/array.hpp>
#include <boost/intrusive/set_hook.hpp>

namespace vostok {
namespace network_core {

class udp_match_client_session;
class delayed_packets_predicate;
class udp_network_flow_emulator;

class udp_match_packet : public packet< udp_match_packet > {
public:
	class helper {
	public:
		static	inline	void	call_constructor	( udp_match_packet& packet ) { /* no source */ }
	private:
		// STATE[STUB]
		static	inline	void	call_destructor		( udp_match_packet& packet )
		{
			// FUNCTION BODY[0xea9c0]: 1
			// <0xea9c4>|0x004|+0x02c:'112'
			// ******
		}
	}; // class helper

private:
	// STATE[STUB]
	inline				udp_match_packet	( )
	{
		// FUNCTION BODY[0xea900]: 2
		// <0xea999>|0x099|+0x00b:'48'
		// <0xea9a4>|0x0a4|+0x007:'49'
		// ******
	}
public:
	inline				~udp_match_packet	( ) { /* no source */ }

	inline	u32			allocated_size		( ) const { return 0; }

	inline	pcbyte		buffer_to_send		( ) const { return NULL; }
	inline	pbyte		buffer_to_send		( ) { return NULL; }

	// STATE[STUB]
	inline	u8			header_size			( ) const
	{
		return 0;
		// FUNCTION BODY[0x8d670]
		// <0x8d670>|0x000|      :'62'	{
		// ******
	}
	inline	u32			buffer_to_send_size	( ) const { return 0; }

	inline	void		reallocate			( u32 new_size ) { /* no source */ }

	// the channel's boost::intrusive::set names &udp_match_packet::set_member_hook and
	// the connection's udp_match_packet_list names &udp_match_packet::next - both reach
	// these private members (a friend's nested types share its access).
	friend	class		udp_match_connection;
	friend	class		delayed_packets_predicate;
	friend	class		udp_network_flow_emulator;

private:
	/* 0x0008 */	boost::intrusive::set_member_hook<>	set_member_hook;
	/* 0x0018 */	udp_match_client_session*		client_session;
	/* 0x001c */	udp_match_packet*				next;
	/* 0x0020 */	u32								last_send_time_in_ms;
	/* 0x0024 */	sequence_number< u8 >			sequence_id;
	/* 0x0026 */	sequence_number< u16 >			order_id;
	/* 0x0028 */	u8								message_type;
	/* 0x0029 */	u8								send_count;
	/* 0x002a */	u8								channel_id		: 6;
	/* 0x002a */	u8								is_reliable		: 1;
	/* 0x002a */	u8								is_ordered		: 1;
	/* 0x002b */	boost::array< u8, 256 >			m_buffer;
}; // class udp_match_packet

STATIC_SIZE_ASSERT(udp_match_packet, 0x12C);
STATIC_SIZE_ASSERT(udp_match_packet::helper, 0x1);

// STATE[STUB]
inline udp_match_packet* new_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
)
{
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
