////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED

#include <boost/intrusive/set_hook.hpp>

#include <vostok/network_core/packet.h>

namespace vostok {
namespace network_core {

class udp_match_packet : packet<udp_match_packet> {
public:
	inline	explicit	udp_match_packet	( ); // where is source?
	inline				~udp_match_packet	( ) { /* no source */ }

	inline	u32			allocated_size		( ) const { /* no source */ }

	inline	pbyte		buffer_to_send		( ) { return m_buffer; }
	inline	pcbyte		buffer_to_send		( ) const { return m_buffer; }

	inline	u8			header_size			( ) const { return 0; }; // sushi@TODO:
	inline	u32			buffer_to_send_size	( ) const { /* no source */ }

	inline	void		reallocate			( u32 new_size ) { /* no source */ }

private:
	/* 0x0000 */	/* packet<udp_match_packet> */
	/* 0x0008 */	boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>	set_member_hook;
	/* 0x0018 */	udp_match_client_session*			client_session;
	/* 0x001c */	udp_match_packet*					next;
	/* 0x0020 */	u32									last_send_time_in_ms;
	/* 0x0024 */	sequence_number<u16>				sequence_id;
	/* 0x0026 */	sequence_number<u16>				order_id;
	/* 0x0028 */	u8									message_type;
	/* 0x0029 */	u8									send_count;
	/* 0x002a */	u8									channel_id	: 6;
	/* 0x002a */	u8									is_reliable : 1;
	/* 0x002a */	u8									is_ordered	: 1;
	/* 0x002b */	boost::array<u8, 256> 				m_buffer;
}; // class udp_match_packet

STATIC_SIZE_ASSERT(udp_match_packet, 0x12C);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED