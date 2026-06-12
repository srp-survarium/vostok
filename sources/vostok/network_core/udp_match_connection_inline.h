////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[85.68%|DONE]: structure 8/8 stmts aligned; residual is the append(u8)/append(u16)
// COMDAT-forward wall (base inlines the scalar overloads into append(pcvoid,u32), target
// calls the kept scalar COMDATs directly), cascading reg/slot + frame size (0x28 vs 0x1C).
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
	{
		packet.append				( u16( 0xFFFF ) );
	}

	// STRUCTURE DIFF: target 8 stmts / base 8 stmts
	// b.diff    |t.addr |b.addr |t.sz|b.sz|t.ln|b.ln|b.code
	// ----------+-------+-------+----+----+----+----+------
	// SIZE +0x7 |0xdac9f|0x8e0df|0xd |0x14|+1  |+1  |packet.append( message_type );
	// SIZE -0x2 |0xdacc7|0x8e10e|0x1c|0x1a|+4  |+4  |packet.channel_id = info.channel_id;
	// BASE_ONLY |--     |0x8e128|--  |0x22|--  |+5  |packet.is_reliable = info.is_reliable;
	// SIZE -0xe |0xdad04|0x8e16b|0x20|0x12|+6  |+8  |if ( packet.is_ordered )
	// SIZE +0x4 |0xdad24|0x8e17d|0x13|0x17|+8  |+10 |packet.append( u16( 0xFFFF ) );
	// TRGT_ONLY |0xdad37|--     |0x16|--  |+9  |--  |--
	// VERDICT: STRUCTURE MATCH (shape ok) - 8/8 stmts, all SIZE-class: the documented
	// append(u8)/append(u16) COMDAT-forward wall (base inlines scalar overloads into
	// append(pcvoid,u32), target calls the kept COMDATs directly); BASE_ONLY/TRGT_ONLY
	// pair is a line-attribution split on the is_reliable assign vs the append(u16) body,
	// same statement count both sides. Frame 0x28 vs 0x1C from the extra inline spill.
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
	if ( packet_type == udp_match_regular_packet )
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
