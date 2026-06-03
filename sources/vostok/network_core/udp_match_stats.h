////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED

namespace vostok {
namespace network_core {

struct udp_match_items_stats {
public:
	inline	explicit	udp_match_items_stats	( ) { /* no source */ }

	inline	void		operator+=				( udp_match_items_stats const& arg_0 ) { /* no source */ }
	inline	void		operator/=				( u32 arg_0 ) { /* no source */ }

	inline	void		reset					( ) { /* no source */ }

public:
	/* 0x0000 */	u32		count;
	/* 0x0004 */	u32		bytes;
}; // struct udp_match_items_stats

STATIC_SIZE_ASSERT(udp_match_items_stats, 0x8);


struct udp_match_stream_stats {
public:
	inline	explicit	udp_match_stream_stats	( ) { /* no source */ }

	inline	void		dump					( pcstr arg_0, pcstr arg_1 ) const { /* no source */ }

	inline	void		operator+=				( udp_match_stream_stats const& arg_0 ) { /* no source */ }
	inline	void		operator/=				( u32 arg_0 ) { /* no source */ }

	inline	void		reset					( ) { /* no source */ }

public:
	/* 0x0000 */	udp_match_items_stats		packets;
	/* 0x0008 */	udp_match_items_stats		messages;
	/* 0x0010 */	u32							data_bytes;
}; // struct udp_match_stream_stats

STATIC_SIZE_ASSERT(udp_match_stream_stats, 0x14);


struct udp_match_stats {
public:
	inline	explicit	udp_match_stats	( );

	inline	void		dump			( pcstr arg_0 ) const { /* no source */ }

	inline	void		operator+=		( udp_match_stats const& rhs ) { /* no source */ }
	inline	void		operator/=		( u32 rhs ) { /* no source */ }

	inline	void		reset			( ) { /* no source */ }

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

// STATE[STUB] inline?
bool operator>=( udp_match_items_stats const& left, udp_match_items_stats const& right )
{
	return left.count >= right.count && left.bytes >= right.count;
}

// STATE[STUB]
bool operator>=( udp_match_stream_stats const& left, udp_match_stream_stats const& right )
{
												// 1
	return left.packets >= right.packets		// 2
		&& left.messages >= right.messages		// 3
		&& left.data_bytes >= right.data_bytes;	// <0xeaac6>|0x000|0x000:'168'
}

// STATE[STUB]
bool operator>=( udp_match_stats const& left, udp_match_stats const& right )
{
																	// 1
	return left.sent >= right.sent									// 2
		&& left.resent >= right.resent 								// 3
		&& left.received >= right.received							// 4
		&& left.received_duplicated >= right.received_duplicated	// 5
		&& left.received_duplicated >= right.received_duplicated 	// 6
		&& left.sent_low_level > right.sent_low_level				// 7
		&& left.received_low_level > right.received_low_level;		// <0xeab76>|0x000|0x000:'180'
}

// STATE[STUB]
inline	udp_match_stats	operator-( udp_match_stats const& left, udp_match_stats const& right )
{
	// FUNCTION BODY[0x91fa0]: 15
	// 1
	// 2
	// <0x91fa8>|0x000|0x000:'212'
	// 1
	// <0x91faf>|0x007|0x007:'214'
	// <0x91ff3>|0x04b|0x044:'215'
	// <0x9203a>|0x092|0x047:'216'
	// <0x92081>|0x0d9|0x047:'217'
	// <0x920c8>|0x120|0x047:'218'
	// <0x9210f>|0x167|0x047:'219'
	// 1
	// <0x92156>|0x1ae|0x047:'221'
	// <0x92166>|0x1be|0x010:'222'
	// 1
	// <0x9216f>|0x1c7|0x009:'224'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED