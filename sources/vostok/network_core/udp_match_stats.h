// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED

namespace vostok {
namespace network_core {

struct udp_match_items_stats {
	inline			udp_match_items_stats	( ) :
		count	( 0 ),
		bytes	( 0 )
	{
	}

	inline	void	operator+=				( udp_match_items_stats const& other )
	{
		count	+= other.count;
		bytes	+= other.bytes;
	}
	inline	void	operator/=				( const u32 value )
	{
		count	/= value;
		bytes	/= value;
	}

	inline	void	reset					( )
	{
		count	= 0;
		bytes	= 0;
	}

public:
	/* 0x0000 */	u32		count;
	/* 0x0004 */	u32		bytes;
}; // struct udp_match_items_stats

STATIC_SIZE_ASSERT(udp_match_items_stats, 0x8);

struct udp_match_stream_stats {
	inline			udp_match_stream_stats	( ) :
		data_bytes	( 0 )
	{
	}

	inline	void	dump					( pcstr const title, pcstr const prefix ) const
	{
		LOG_INFO(
			"%s%s: packets %u/%u bytes, messages %u/%u bytes, data %u bytes",
			prefix,
			title,
			packets.count,
			packets.bytes,
			messages.count,
			messages.bytes,
			data_bytes
		);
	}

	inline	void	operator+=				( udp_match_stream_stats const& other )
	{
		packets		+= other.packets;
		messages	+= other.messages;
		data_bytes	+= other.data_bytes;
	}
	inline	void	operator/=				( const u32 value )
	{
		packets		/= value;
		messages	/= value;
		data_bytes	/= value;
	}

	inline	void	reset					( )
	{
		packets.reset	( );
		messages.reset	( );
		data_bytes	= 0;
	}

public:
	/* 0x0000 */	udp_match_items_stats		packets;
	/* 0x0008 */	udp_match_items_stats		messages;
	/* 0x0010 */	u32							data_bytes;
}; // struct udp_match_stream_stats

STATIC_SIZE_ASSERT(udp_match_stream_stats, 0x14);

struct udp_match_stats {
	inline			udp_match_stats	( ) :
		max_local_sequence_difference	( 0 ),
		unacknowledged_packets			( 0 )
	{
	}

	inline	void	dump			( pcstr const title ) const
	{
		LOG_INFO			( "%s", title );
		sent.dump			( "sent", "  " );
		sent_low_level.dump	( "sent low level", "  " );
		resent.dump			( "resent", "  " );
		received.dump		( "received", "  " );
		received_low_level.dump( "received low level", "  " );
		received_duplicated.dump( "received duplicated", "  " );
		LOG_INFO			( "  max local sequence difference: %u", max_local_sequence_difference );
		LOG_INFO			( "  unacknowledged packets: %d", unacknowledged_packets );
	}

	inline	void	operator+=		( udp_match_stats const& other )
	{
		sent					+= other.sent;
		sent_low_level			+= other.sent_low_level;
		resent					+= other.resent;
		received				+= other.received;
		received_low_level		+= other.received_low_level;
		received_duplicated		+= other.received_duplicated;
		max_local_sequence_difference	= math::max( max_local_sequence_difference, other.max_local_sequence_difference );
		unacknowledged_packets			+= other.unacknowledged_packets;
	}
	inline	void	operator/=		( const u32 value )
	{
		sent					/= value;
		sent_low_level			/= value;
		resent					/= value;
		received				/= value;
		received_low_level		/= value;
		received_duplicated		/= value;
		unacknowledged_packets		/= value;
	}

	inline	void	reset			( )
	{
		sent.reset				( );
		sent_low_level.reset		( );
		resent.reset				( );
		received.reset			( );
		received_low_level.reset	( );
		received_duplicated.reset	( );
		max_local_sequence_difference	= 0;
		unacknowledged_packets			= 0;
	}

public:
	/* 0x0000 */	udp_match_stream_stats		sent;
	/* 0x0014 */	udp_match_stream_stats		sent_low_level;
	/* 0x0028 */	udp_match_stream_stats		resent;
	/* 0x003c */	udp_match_stream_stats		received;
	/* 0x0050 */	udp_match_stream_stats		received_low_level;
	/* 0x0064 */	udp_match_stream_stats		received_duplicated;
	/* 0x0078 */	u32							max_local_sequence_difference;
	/* 0x007c */	s32							unacknowledged_packets;
}; // struct udp_match_stats

STATIC_SIZE_ASSERT(udp_match_stats, 0x80);

inline bool operator>=( udp_match_items_stats const& left, udp_match_items_stats const& right )
{
	return left.count >= right.count && left.bytes >= right.bytes;
}

inline bool operator>=( udp_match_stream_stats const& left, udp_match_stream_stats const& right )
{
	return left.packets >= right.packets
		&& left.messages >= right.messages
		&& left.data_bytes >= right.data_bytes;
}

inline bool operator>=( udp_match_stats const& left, udp_match_stats const& right )
{
	return left.sent >= right.sent
		&& left.resent >= right.resent
		&& left.received >= right.received
		&& left.received_duplicated >= right.received_duplicated
		&& left.sent_low_level >= right.sent_low_level
		&& left.received_low_level >= right.received_low_level;
}

inline udp_match_items_stats operator-( udp_match_items_stats const& left, udp_match_items_stats const& right )
{
	udp_match_items_stats	result;
	result.count	= left.count - right.count;
	result.bytes	= left.bytes - right.bytes;
	return			result;
}

inline udp_match_stream_stats operator-( udp_match_stream_stats const& left, udp_match_stream_stats const& right )
{
	udp_match_stream_stats	result;
	result.packets		= left.packets - right.packets;
	result.messages		= left.messages - right.messages;
	result.data_bytes	= left.data_bytes - right.data_bytes;
	return				result;
}

inline udp_match_stats operator-( udp_match_stats const& left, udp_match_stats const& right )
{
	udp_match_stats	result;

	result.sent					= left.sent - right.sent;
	result.resent				= left.resent - right.resent;
	result.received				= left.received - right.received;
	result.received_duplicated	= left.received_duplicated - right.received_duplicated;
	result.sent_low_level		= left.sent_low_level - right.sent_low_level;
	result.received_low_level	= left.received_low_level - right.received_low_level;

	result.max_local_sequence_difference	= math::max( left.max_local_sequence_difference, right.max_local_sequence_difference );
	result.unacknowledged_packets			= left.unacknowledged_packets - right.unacknowledged_packets;

	return			result;
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
