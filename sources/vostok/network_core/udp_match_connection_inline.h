////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

inline void udp_match_connection::construct_packet(
	udp_match_packets_orderer&		packets_orderer,
	udp_match_packet&				packet,
	u8								message_type
)
{
	// LOCALS
	// udp_match_message_type_info const& info
	// ******

	// FUNCTION BODY[0xeac90]: 14
	// <0xeac96>|0x006|+0x009:'22'
	// <0xeac9f>|0x00f|+0x00d:'23'
	// <0xeacac>|0x01c|+0x01b:'24'
	// <0xeacc7>|0x037|+0x01c:'26'
	// <0xeace3>|0x053|+0x021:'27'
	// <0xead04>|0x074|+0x020:'28'
	// <0xead24>|0x094|+0x013:'30'
	// <0xead37>|0x0a7|+0x016:'31'
	// ******
}

template < typename Predicate >
inline void udp_match_connection::process_incoming_packet( packet_reader& reader, Predicate const& predicate )
{
	// LOCALS
	// const u16 						local_acknowledgement_bits
	// const u32 						message_bytes
	// const u16 						bits
	// sequence_number< u16 > const& 	remote_sequence_id
	// sequence_number< u16 > const& 	local_sequence_id
	// const u32 						packet_bytes
	// ******

	// FUNCTION BODY[0x131980]: 65
	// <0x13198f>|0x00f|+0x012:'94'
	// <0x1319a1>|0x021|+0x01d:'96'
	// <0x1319be>|0x03e|+0x015:'98'
	// <0x1319d3>|0x053|+0x00b:'100'
	// <0x1319de>|0x05e|+0x009:'101'
	// <0x131a60>|0x0e0|+0x021:'112'
	// <0x131ad7>|0x157|+0x01c:'123'
	// <0x131b89>|0x209|+0x025|[1]:'136'
	// <0x131bae>|0x22e|+0x015|[2]:'137'
	// <0x131cdc>|0x35c|+0x012:'158'
	// ******
}

template < typename Predicate >
inline void udp_match_connection::call_predicate( Predicate const& predicate, packet_reader& reader )
{
	// LOCALS
	// udp_match_packet& 				packet
	// udp_match_message_type_info const& info
	// sequence_number< u16 > 			next_order_id
	// const u8 						message_type
	// udp_match_connection::channel& 	channel
	// sequence_number< u16 > 			order_id
	// ******

	// FUNCTION BODY[0x131d00]: 43
	// <0x131d10>|0x010|+0x00b:'46'
	// <0x131d1b>|0x01b|+0x02e:'47'
	// <0x131d49>|0x049|+0x011:'48'
	// <0x131d5a>|0x05a|+0x03f:'50'
	// <0x131d99>|0x099|+0x005:'51'
	// <0x131ebc>|0x1bc|+0x069:'79'
	// <0x131f25>|0x225|+0x02d|[1]:'80'
	// <0x13202e>|0x32e|+0x005:'88'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED
