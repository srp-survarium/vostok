////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED

namespace vostok {
namespace network_core {

struct udp_match_items_stats {
public:
	inline	explicit	udp_match_items_stats	( ) : count( 0 ), bytes( 0 ) { }

	inline	void		operator+=				( udp_match_items_stats const& other ) { VOSTOK_UNREFERENCED_PARAMETER( other ); }
	inline	void		operator/=				( u32 divisor ) { VOSTOK_UNREFERENCED_PARAMETER( divisor ); }

	inline	void		reset					( ) { }

public:
	/* 0x0000 */	u32		count;
	/* 0x0004 */	u32		bytes;
}; // struct udp_match_items_stats

STATIC_SIZE_ASSERT( udp_match_items_stats, 0x8 );


struct udp_match_stream_stats {
public:
	inline	explicit	udp_match_stream_stats	( ) : data_bytes( 0 ) { }

	inline	void		dump					( pcstr caption, pcstr prefix ) const { VOSTOK_UNREFERENCED_PARAMETERS( caption, prefix ); }

	inline	void		operator+=				( udp_match_stream_stats const& other ) { VOSTOK_UNREFERENCED_PARAMETER( other ); }
	inline	void		operator/=				( u32 divisor ) { VOSTOK_UNREFERENCED_PARAMETER( divisor ); }

	inline	void		reset					( ) { }

public:
	/* 0x0000 */	udp_match_items_stats		packets;
	/* 0x0008 */	udp_match_items_stats		messages;
	/* 0x0010 */	u32							data_bytes;
}; // struct udp_match_stream_stats

STATIC_SIZE_ASSERT( udp_match_stream_stats, 0x14 );


struct udp_match_stats {
public:
	inline	explicit	udp_match_stats	( );

	inline	void		dump			( pcstr caption ) const { VOSTOK_UNREFERENCED_PARAMETER( caption ); }

	inline	void		operator+=		( udp_match_stats const& rhs ) { VOSTOK_UNREFERENCED_PARAMETER( rhs ); }
	inline	void		operator/=		( u32 rhs ) { VOSTOK_UNREFERENCED_PARAMETER( rhs ); }

	inline	void		reset			( ) { }

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

STATIC_SIZE_ASSERT( udp_match_stats, 0x80 );

// STATE[STUB]
inline udp_match_stats::udp_match_stats( )
{
	// FUNCTION BODY[0x91f30]: 0
	// <0x91f30>|0x000|+0x061:'96'	{
	// <0x91f91>|0x061|      :'97'	}
	// ******
}

// STATE[STUB]
inline bool operator>=( udp_match_stream_stats const& left, udp_match_stream_stats const& right )
{
	VOSTOK_UNREFERENCED_PARAMETERS( left, right );

	return false;

	// FUNCTION BODY[0xeaac0]: 4
	// <0>
	// <1>
	// <2>
	// <0xeaac6>|0x006|+0x097:'168'
	// ******
}

// STATE[STUB]
inline bool operator>=( udp_match_stats const& left, udp_match_stats const& right )
{
	VOSTOK_UNREFERENCED_PARAMETERS( left, right );

	return false;

	// FUNCTION BODY[0xeab70]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0xeab76>|0x006|+0x0cc:'180'
	// ******
}

// STATE[STUB]
inline udp_match_stats operator-( udp_match_stats const& left, udp_match_stats const& right )
{
	VOSTOK_UNREFERENCED_PARAMETERS( left, right );

	return udp_match_stats( );

	// FUNCTION BODY[0x91fa0]: 15
	// <0>
	// <1>
	// <0x91fa8>|0x008|+0x007:'212'
	// <0>
	// <0x91faf>|0x00f|+0x044:'214'
	// <0x91ff3>|0x053|+0x047:'215'
	// <0x9203a>|0x09a|+0x047:'216'
	// <0x92081>|0x0e1|+0x047:'217'
	// <0x920c8>|0x128|+0x047:'218'
	// <0x9210f>|0x16f|+0x047:'219'
	// <0>
	// <0x92156>|0x1b6|+0x010:'221'
	// <0x92166>|0x1c6|+0x009:'222'
	// <0>
	// <0x9216f>|0x1cf|+0x005:'224'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
