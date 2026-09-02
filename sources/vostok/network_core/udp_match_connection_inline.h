// SPDX-License-Identifier: GPL-3.0-or-later

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
	packet.message_type				= message_type;
	packet.append					( message_type );
	udp_match_message_type_info const&	info	= packets_orderer.get_sending_message_info( message_type );

	packet.channel_id				= info.channel_id;
	packet.is_reliable				= info.is_reliable;
	packet.is_ordered				= info.is_ordered;

	if ( packet.is_ordered )
		packet.append				( u16( 0xFFFF ) );
}

template < typename Predicate >
inline void udp_match_connection::call_predicate( Predicate const& predicate, packet_reader& reader )
{
	const u8	message_type		= reader.r< u8 >( );
	udp_match_message_type_info const&	info	= m_packets_orderer.get_received_message_info( message_type );
	if ( !info.is_ordered )
	{
		predicate					( message_type, reader );
		return;
	}

	sequence_number< u16 >	order_id( reader.r< u16 >( ) );

	channel&	channel				= m_channels[ info.channel_id ];

	if ( order_id <= channel.received_order_id )
	{
		return;
	}

	if ( channel.packets.find( order_id, comparer( ) ) != channel.packets.end( ) )
	{
		return;
	}

	udp_match_packet&	packet		= *new_udp_match_packet( m_packets_allocator );
	packet.message_type				= message_type;
	packet.order_id					= order_id;
	packet.append					( reader.pointer( ), reader.size_to_eof( ) );
	channel.packets.insert			( packet );

	sequence_number< u16 >	next_order_id( channel.received_order_id );
	++next_order_id;
	while ( !channel.packets.empty( ) && channel.packets.begin( )->order_id == next_order_id )
	{
		udp_match_packet*	packet	= &*channel.packets.begin( );
		channel.packets.erase		( *packet );
		packet_reader	reader		( *packet );

		predicate					( packet->message_type, reader );
		++channel.received_order_id;
		++next_order_id;
		delete_udp_match_packet		( m_packets_allocator, packet );
	}
}

template < typename Predicate >
inline void udp_match_connection::process_incoming_packet( packet_reader& reader, Predicate const& predicate )
{
	dump							( "before process_incoming   ", 0 );

	threading::interlocked_exchange	( m_last_receive_time_in_ms, m_last_send_attempt_time_in_ms );

	++m_stats.received.packets.count;

	const u32	message_bytes		= reader.size_to_eof( );
	const u32	packet_bytes		= message_bytes + 46;
	m_stats.received.packets.bytes	+= packet_bytes;
	m_stats.received.messages.bytes	+= message_bytes;

	sequence_number< u16 > const&	remote_sequence_id	= sequence_number< u16 >( reader.r< u16 >( ) );
	sequence_number< u16 > const&	local_sequence_id	= sequence_number< u16 >( reader.r< u16 >( ) );
	const u16	bits				= reader.r< u16 >( );
	const u16	local_acknowledgement_bits	= u16( ( bits >> 1 ) | 0x8000 );

	if ( m_remote_sequence_id == remote_sequence_id )
	{
		++m_stats.received_duplicated.packets.count;
		m_stats.received_duplicated.packets.bytes	+= packet_bytes;

		m_stats.received_duplicated.messages.bytes	+= message_bytes;

		dump						( "after  process_incoming   ", 0 );
		return;
	}

	if ( m_remote_sequence_id < remote_sequence_id )
		update_acknowledgements		( remote_sequence_id, local_sequence_id, local_acknowledgement_bits );

	const udp_match_packets_count_enum	packet_type	= udp_match_packets_count_enum( ( bits & 1 ) != 0 );
	if ( packet_type == udp_match_single_packet )
	{
		++m_stats.received.messages.count;
		m_stats.received.data_bytes	+= message_bytes;

		call_predicate				( predicate, reader );
		dump						( "after  process_incoming   ", 0 );
		return;
	}

	for ( u32 i = 0; !reader.eof( ); ++i )
	{
		++m_stats.received.messages.count;
		const u8	subpacket_size	= reader.r< u8 >( );
		m_stats.received.data_bytes	+= subpacket_size;

		packet_reader	subpacket_reader( base_packet( pbyte( reader.pointer( ) ), subpacket_size ) );
		reader.advance				( subpacket_size );
		if ( i || !reader.eof( ) )
			call_predicate			( predicate, subpacket_reader );
		else
		{
			++m_stats.received_low_level.packets.count;
			m_stats.received_low_level.packets.bytes	+= packet_bytes;
			++m_stats.received_low_level.messages.count;
			m_stats.received_low_level.messages.bytes	+= message_bytes;
			m_stats.received_low_level.data_bytes		+= subpacket_reader.size_to_eof( );
			process_low_level_message( subpacket_reader, m_last_send_attempt_time_in_ms );
		}
	}

	dump							( "after  process_incoming   ", 0 );
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED
