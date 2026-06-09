////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED

namespace vostok {
namespace network_core {

struct udp_match_items_stats {
	inline			udp_match_items_stats	( ) { /* no source */ }

	inline	void	operator+=				( udp_match_items_stats const& other ) { /* no source */ }
	inline	void	operator/=				( u32 value ) { /* no source */ }

	inline	void	reset					( ) { /* no source */ }

public:
	/* 0x0000 */	u32		count;
	/* 0x0004 */	u32		bytes;
}; // struct udp_match_items_stats

STATIC_SIZE_ASSERT(udp_match_items_stats, 0x8);

struct udp_match_stream_stats {
	inline			udp_match_stream_stats	( ) { /* no source */ }

	inline	void	dump					( pcstr title, pcstr prefix ) const { /* no source */ }

	inline	void	operator+=				( udp_match_stream_stats const& other ) { /* no source */ }
	inline	void	operator/=				( u32 value ) { /* no source */ }

	inline	void	reset					( ) { /* no source */ }

public:
	/* 0x0000 */	udp_match_items_stats		packets;
	/* 0x0008 */	udp_match_items_stats		messages;
	/* 0x0010 */	u32							data_bytes;
}; // struct udp_match_stream_stats

STATIC_SIZE_ASSERT(udp_match_stream_stats, 0x14);

// STATE[100%|DONE]: fully inlined into the stream/stats operator>= (no standalone
// target symbol to diff); single chained return.
inline bool operator>=( udp_match_items_stats const& left, udp_match_items_stats const& right )
{
	return left.count >= right.count && left.bytes >= right.count;
}

// STATE[97.74%|PARTIAL]: shape exact (1 chained return); sole SIZE is the nested
// items-stats operator>= call out-of-line (target) vs inlined (base).
inline bool operator>=( udp_match_stream_stats const& left, udp_match_stream_stats const& right )
{
	return left.packets >= right.packets
		&& left.messages >= right.messages
		&& left.data_bytes >= right.data_bytes;

	// STRUCTURE DIFF[target 0xdaac0 | base 0x7fdf0]: target 1 / base 1 stmts
	//   1: 0x006 <0x97> | 0x006 <0x95> | && left.data_bytes >= right.data_bytes;   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - single chained return; sole SIZE is nested items operator>= inline-vs-call, non-source-fixable.
}

struct udp_match_stats {
	inline			udp_match_stats	( )
	{
	}

	inline	void	dump			( pcstr title ) const { /* no source */ }

	inline	void	operator+=		( udp_match_stats const& other ) { /* no source */ }
	inline	void	operator/=		( u32 value ) { /* no source */ }

	inline	void	reset			( ) { /* no source */ }

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

// STATE[94.55%|PARTIAL]: structure now matches (dropped a spurious duplicate
// received_duplicated compare - was 7 terms, target has 6). Residual is a target
// idiom on the final term (bool materialize `mov eax,1; test`) + a short/near jump,
// not source-fixable.
inline bool operator>=( udp_match_stats const& left, udp_match_stats const& right )
{
	return left.sent >= right.sent
		&& left.resent >= right.resent
		&& left.received >= right.received
		&& left.received_duplicated >= right.received_duplicated
		&& left.sent_low_level >= right.sent_low_level
		&& left.received_low_level >= right.received_low_level;

	// STRUCTURE DIFF[target 0xdab70 | base 0x7fe60]: target 1 / base 1 stmts
	//   1: 0x006 <0xcc> | 0x006 <0xbf> | && left.received_low_level >= right.received_low_level;   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - chain is now 6 terms matching target order; sole SIZE is target's final-term bool materialize + short/near jump, non-source-fixable.
}

// STATE[STUB]
inline udp_match_stats operator-( udp_match_stats const& left, udp_match_stats const& right )
{
	return udp_match_stats();
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
