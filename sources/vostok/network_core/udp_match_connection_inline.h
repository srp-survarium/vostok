////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CONNECTION_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
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

// STATE[61.81%|PARTIAL]: structure 27/27 stmts aligned; residual is the documented
// inline-vs-call wall (base /Ob2 inlines r<u8>/r<u16>, pointer/size_to_eof and the
// folded intrusive-set empty/begin thunks the target keeps out-of-line) plus the
// new_/delete_udp_match_packet inline residual, all cascading reg/slot renames.
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

// STATE[49.60%|PARTIAL]: structure 43/43 stmts aligned (structure-diff shows only
// SIZE rows + one else-jmp line-attribution split); residual is the inline-vs-call
// wall (r<u16>/r<u8>/size_to_eof/eof inlined in base, out-of-line in target) and its
// cascading frame (0x124 vs 0x108) / register renames; update_acknowledgements' arg
// setup stays LTCG-shrunk while that callee is a carcass (call-boundary class).
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
