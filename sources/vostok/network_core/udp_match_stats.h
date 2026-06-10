////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED

namespace vostok {
namespace network_core {

// sushi@TODO: the `/* no source */` members below (reset/operator+=/operator/=/dump) have no
// inline-site evidence yet (no matched consumer accumulates/dumps stats); when one gets matched,
// reconstruct them and revisit whether the implemented ctors/free operators should route through them.
struct udp_match_items_stats {
	inline			udp_match_items_stats	( ) :
		count	( 0 ),
		bytes	( 0 )
	{
	}

	inline	void	operator+=				( udp_match_items_stats const& other ) { /* no source */ }
	inline	void	operator/=				( u32 value ) { /* no source */ }

	inline	void	reset					( ) { /* no source */ }

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

struct udp_match_stats {
	// STATE[0%|PARTIAL]: empty body + scalar init list (streams zero through their own
	// ctors), per the gold line table ('{' L96 carries all 0x61 bytes); the surviving
	// target COMDAT is an optimized (frameless, this-in-eax, 32 flat dword stores)
	// LTCG emission - a /Od body cannot pair against it (objdiff scores None).
	inline			udp_match_stats	( ) :
		max_local_sequence_difference	( 0 ),
		unacknowledged_packets			( 0 )
	{
		// STRUCTURE DIFF: target 0 stmts / base 0 stmts (no divergence rows)
		// VERDICT: STRUCTURE MATCH (shape ok) - both skeletons are empty, confirming the
		// init-list-only spelling (body assigns would emit base statement rows); the
		// None score is the optimized target emission (frameless, this-in-eax, flat
		// stores), unpairable from /Od and non-steerable.
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

// STATE[100%|DONE]: fully inlined into the stream/stats operator>= (no standalone
// target symbol to diff); single chained return. The inlined emission in the stream
// operator>= compares [+0]/[+0] then [+4]/[+4] - bytes against bytes.
inline bool operator>=( udp_match_items_stats const& left, udp_match_items_stats const& right )
{
	return left.count >= right.count && left.bytes >= right.bytes;
}

// STATE[100%|DONE]: the old -0x2 SIZE residual was the items operator>= transcription
// typo (right.count for right.bytes - two disp-less compares); byte-perfect once fixed.
inline bool operator>=( udp_match_stream_stats const& left, udp_match_stream_stats const& right )
{
	return left.packets >= right.packets
		&& left.messages >= right.messages
		&& left.data_bytes >= right.data_bytes;
}

// STATE[94.55%|PARTIAL]: structure now matches (dropped a spurious duplicate
// received_duplicated compare - was 7 terms, target has 6). Residual is a target
// idiom on the final term (bool materialize `mov eax,1; test`) + a short/near jump,
// not source-fixable - the six terms are out-of-line calls to the stream operator>=
// on both sides, so no inlined compare bytes are involved.
inline bool operator>=( udp_match_stats const& left, udp_match_stats const& right )
{
	return left.sent >= right.sent
		&& left.resent >= right.resent
		&& left.received >= right.received
		&& left.received_duplicated >= right.received_duplicated
		&& left.sent_low_level >= right.sent_low_level
		&& left.received_low_level >= right.received_low_level;

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE -0xd | 115 | && left.received_low_level >= right.received_low_level;
	// VERDICT: STRUCTURE MATCH (shape ok) - single chained return, 6 terms in target
	// order (all six are calls to the stream operator>= on both sides); sole SIZE is
	// the target's final-term bool-materialize idiom + short/near jump, non-steerable.
}

// STATE[INLINED]: fully inlined into the stream/stats operator- (no standalone target symbol).
inline udp_match_items_stats operator-( udp_match_items_stats const& left, udp_match_items_stats const& right )
{
	udp_match_items_stats	result;
	result.count	= left.count - right.count;
	result.bytes	= left.bytes - right.bytes;
	return			result;
}

// STATE[INLINED]: fully inlined into the stats operator- (no standalone target symbol).
inline udp_match_stream_stats operator-( udp_match_stream_stats const& left, udp_match_stream_stats const& right )
{
	udp_match_stream_stats	result;
	result.packets		= left.packets - right.packets;
	result.messages		= left.messages - right.messages;
	result.data_bytes	= left.data_bytes - right.data_bytes;
	return				result;
}

// STATE[5.80%|PARTIAL]: statement set/order per the gold line table (result; 6 stream
// subs in sent/resent/received/received_duplicated/sent_low_level/received_low_level
// order; math::max on max_local_sequence_difference; unacknowledged sub; return). The
// target COMDAT is an optimized LTCG emission (xmm pairs, args in edi/esi) - not /Od
// codegen; the % floor is that emission, not a source divergence.
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

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts (aligner cannot pair the optimized
	// emission - its SIZE/ONLY rows are pairing noise; the two single-side skeletons
	// compared by hand are identical: result / blank / 6 stream subs in sent, resent,
	// received, received_duplicated, sent_low_level, received_low_level order / blank /
	// max + unacknowledged / blank / return)
	// VERDICT: STRUCTURE MATCH (shape ok) - statement set, order and blank-gap pattern
	// reproduce the gold line table (L212-224); the 5.80% is the optimized target COMDAT
	// (xmm pairs, args LTCG-promoted to edi/esi), unpairable from /Od and non-steerable.
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_STATS_H_INCLUDED
