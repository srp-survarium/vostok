////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/buffer_vector.h>

// the tick remove_if predicate lives at GLOBAL scope - the target mangling carries no
// namespace (??Rdelayed_packets_predicate@@QBE..); the namespaced spelling mangles
// @network_core@vostok@ and the symbol join fails SILENTLY (objdiff never pairs it).
class delayed_packets_predicate {
public:
	inline	delayed_packets_predicate	(
		vostok::buffer_vector< std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > >&	delayed_packets_to_appear,
		const u32	time_in_ms
	) :
		m_delayed_packets_to_appear	( delayed_packets_to_appear ),
		m_time_in_ms				( time_in_ms )
	{ }

	bool	operator()	( std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > const& message ) const;

private:
	/* 0x0000 */	vostok::buffer_vector< std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > >&	m_delayed_packets_to_appear;
	/* 0x0004 */	const u32	m_time_in_ms;
}; // class delayed_packets_predicate

// sushi@review: the size verifier swept only headers, so this .cpp-local class
// (and its ctor) was missed. Its size is 0x8 (ref + const u32). The ctor + operator()
// emit no standalone symbol until tick's remove_if instantiates them (see STATE below).
STATIC_SIZE_ASSERT(delayed_packets_predicate, 0x8);

namespace vostok {
namespace network_core {

// STATE[90.69%|PARTIAL]: structure CORRECT (member-init list); residual is the three
// operand-masked seed immediates (delinker asymmetry, not a source miss).
 udp_network_flow_emulator::udp_network_flow_emulator(
	memory::base_allocator&		allocator,
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
	udp_network_flow_emulator_options const&	options
) :
	m_delayed_packets	( &allocator ),
	m_lost_packets_random	( 0x995a34 ),
	m_ping_random		( 0x995a35 ),
	m_out_of_order_random	( 0x995a36 ),
	m_packets_allocator	( packets_allocator ),
	m_logging_id		( options.logging_id ),
	m_lost_packet_probability	( options.lost_packet_probability ),
	m_min_ping_time_in_ms	( options.min_ping_time_in_ms ),
	m_max_ping_time_in_ms	( options.max_ping_time_in_ms )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts - STRUCTURE MATCH, no diverging rows
	// (the rows the old embed carried here were tick's, bled in by the old aligner).
	// VERDICT: STRUCTURE MATCH (shape ok) - member-init list collapses to the ctor decl
	// line on both sides; residual 90.69% is the three seed immediates 0x995a34/35/36
	// (base operand-masked, target not - delinker asymmetry, not source).
}

// STATE[79.07%|PARTIAL]: structure CORRECT (4/4 stmts, 0 quantity-diffs); residual is
// the STL iterator-comparison codegen, NOT a loop-form miss (see body verdict).
 udp_network_flow_emulator::~udp_network_flow_emulator( )
{
	while ( m_delayed_packets.begin( ) != m_delayed_packets.end( ) ) {
		delete_udp_match_packet( m_packets_allocator, m_delayed_packets.back( ).first );
		m_delayed_packets.pop_back( );
	}

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x2|0| while ( m_delayed_packets.begin( ) != m_delayed_packets.end( ) ) {
	// VERDICT: STRUCTURE MATCH (shape ok) - 4/4 align 1:1; sole SIZE is the STLport
	// iterator-compare lowering, codegen not structure. Disproves sushi "structure is
	// WRONG"; leaving the review_todos sushi-confirm row open.
}

} // namespace network_core
} // namespace vostok

// STATE[100%|DONE]: tick's remove_if instantiates it; at GLOBAL scope the mangled names
// join (the namespaced spelling was byte-identical but scored None - silent join failure;
// target mangles ??Rdelayed_packets_predicate@@QBE.. with no namespace).
bool delayed_packets_predicate::operator()(
	std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > const&	message
) const
{
	if ( m_time_in_ms < message.first->last_send_time_in_ms )
		return false;

	m_delayed_packets_to_appear.push_back( message );
	return true;
}

namespace vostok {
namespace network_core {

// STATE[65.29%|PARTIAL]: from 0x728a30. Structure 15/15 per the target line table:
// two flat empty-guards (m_delayed_packets at entry; delayed_packets_to_appear after
// the erase - its `return` row carries the inlined buffer_vector dtor walk),
// single-declaration `for ( pair* i = begin(), * e = end(); ... )` (one stmt, no
// separate begin/end rows, no `packet` local - target derefs i->first inline), and
// both per-packet packet_readers named `reader` (first one + the two dead seq-id
// reads in an inner brace scope). The reader args are buffer_to_send( ) /
// buffer_to_send_size( ) - the target expansion is ONE folded buffer_size( ) call +
// ONE header_size( ) call on a single i->first spill, then the inlined +2Bh data
// pointer (resolves the old "exact reader size arg" TODO; `+ header_size( )` would
// emit a second header_size). Residual is the documented LTCG call-boundary wall -
// see body VERDICT.
typedef std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint >	flow_emulator_packet_pair;

void udp_network_flow_emulator::tick(
	const u32		time_in_ms,
	boost::function< void( packet_reader&, boost::asio::ip::udp::endpoint const& ) > const&	functor
)
{
	if ( m_delayed_packets.empty( ) )
		return;

	buffer_vector< flow_emulator_packet_pair >	delayed_packets_to_appear(
		ALLOCA( m_delayed_packets.size( ) * sizeof( flow_emulator_packet_pair ) ),
		m_delayed_packets.size( )
	);

	m_delayed_packets.erase(
		std::remove_if(
			m_delayed_packets.begin( ),
			m_delayed_packets.end( ),
			delayed_packets_predicate( delayed_packets_to_appear, time_in_ms )
		),
		m_delayed_packets.end( )
	);

	if ( delayed_packets_to_appear.empty( ) )
		return;

	std::random_shuffle(
		delayed_packets_to_appear.begin( ),
		delayed_packets_to_appear.end( ),
		m_out_of_order_random
	);

	for ( flow_emulator_packet_pair* i = delayed_packets_to_appear.begin( ), * e = delayed_packets_to_appear.end( ); i != e; ++i ) {
		{
			packet_reader	reader( base_packet( i->first->buffer_to_send( ), i->first->buffer_to_send_size( ) ) );
			const u16		received_local_sequence_id	= reader.r< u16 >( );
			const u16		remote_sequence_id			= reader.r< u16 >( );

			VOSTOK_UNREFERENCED_PARAMETER( received_local_sequence_id );
			VOSTOK_UNREFERENCED_PARAMETER( remote_sequence_id );
		}

		packet_reader	reader( base_packet( i->first->buffer_to_send( ), i->first->buffer_to_send_size( ) ) );
		functor( reader, i->second );

		delete_udp_match_packet( m_packets_allocator, i->first );
	}

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts
	// SIZE +0x47|+15| );  (the remove_if erase tail)
	// SIZE +0x3 |+28| packet_reader	reader( base_packet( i->first->buffer_to_send( ), i->first->buffer_to_send_size( ) ) );
	// BASE_ONLY |+29| const u16		received_local_sequence_id	= reader.r< u16 >( );
	// BASE_ONLY |+30| const u16		remote_sequence_id			= reader.r< u16 >( );
	// SIZE +0x5c|+36| packet_reader	reader( base_packet( i->first->buffer_to_send( ), i->first->buffer_to_send_size( ) ) );
	// SIZE +0x7 |+37| functor( reader, i->second );
	// TRGT_ONLY |t+26| -- (0x5f)
	// TRGT_ONLY |t+27| -- (0x13)
	// VERDICT: STRUCTURE MATCH (shape ok) - 15/15; the ONLY rows are aligner pairing
	// noise (it pairs our second reader 0x68 against the target's 0xc r<u16> row; hand
	// alignment is 1:1 in order: readers 0x5f/0x5f vs 0x62/0x68, r<u16> 0xc vs 0x1e each,
	// functor 0x13 = 0x13). Residuals are the documented wall: base inlines r<u16> and
	// the buffer accessors the target keeps out-of-line, and the remove_if site (+0x47)
	// constructs the by-value predicate in-place in target vs our /Od temp+copy.
}

// STATE[78.71%|PARTIAL]
void udp_network_flow_emulator::add_packet(
	pbyte const		buffer,
	const u32		buffer_size,
	boost::asio::ip::udp::endpoint const&	endpoint,
	const u32		time_in_ms,
	const u32		unacknowledged_packets_count
)
{
	packet_reader	reader( base_packet( buffer, buffer_size ) );

	const u16		received_local_sequence_id	= reader.r< u16 >( );
	const u16		remote_sequence_id			= reader.r< u16 >( );

	udp_match_packet* const	packet	= new_udp_match_packet( m_packets_allocator );
	packet->last_send_time_in_ms	= m_ping_random( m_max_ping_time_in_ms - m_min_ping_time_in_ms ) + m_min_ping_time_in_ms + time_in_ms;

	if ( m_delayed_packets.size( ) + unacknowledged_packets_count >= m_packets_allocator.total_size( ) / 4 - 1 )
		packet->last_send_time_in_ms	= time_in_ms;

	memory::copy( packet->m_buffer.data( ), 6, buffer, 6 );
	packet->append( buffer + 6, buffer_size - 6 );

	m_delayed_packets.push_back( std::make_pair( packet, endpoint ) );

	VOSTOK_UNREFERENCED_PARAMETER( received_local_sequence_id );
	VOSTOK_UNREFERENCED_PARAMETER( remote_sequence_id );

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts
	// SIZE -0x5|0 | packet_reader	reader( base_packet( buffer, buffer_size ) );
	// BASE_ONLY|+1| const u16		received_local_sequence_id	= reader.r< u16 >( );
	// BASE_ONLY|+2| const u16		remote_sequence_id			= reader.r< u16 >( );
	// TRGT_ONLY|t+2| -- (0xc)
	// TRGT_ONLY|t+3| -- (0xc)
	// SIZE +0x6|+5| packet->last_send_time_in_ms	= m_ping_random( ... ) + m_min_ping_time_in_ms + time_in_ms;
	// VERDICT: STRUCTURE MATCH (shape ok) - 10/10; the ONLY rows are the SAME two dead
	// seq-id reads in the same ordinal slots, mis-paired because the sizes flip (target
	// calls the r<u16> COMDAT at 0xc, base inlines it at 0x18 - the is_low_level_packet
	// wall); reader/ping rows are accessor/operator() inline depth. Non-steerable.
}

// STATE[100%|DONE]
void udp_network_flow_emulator::make_packet_lost(
	pbyte const		buffer,
	const u32		buffer_size,
	boost::asio::ip::udp::endpoint const&	endpoint
)
{
	VOSTOK_UNREFERENCED_PARAMETER( endpoint );

	const bool	is_low_level_packet	= udp_match_connection::is_low_level_packet( base_packet( buffer, buffer_size ) );
	VOSTOK_UNREFERENCED_PARAMETER( is_low_level_packet );

	// structure-diff: aligned 1, size-diffs 0, quantity-diffs 0, blank-gaps 0.
	// VERDICT: DONE 100% - STRUCTURE VERIFIED (resolves sushi@TODO "100% without
	// structure"). Target and base are byte-identical (the `mov [ebp-1],al` IS the
	// is_low_level_packet eater; `endpoint` ref-param needs no eater - ret 0Ch cleans
	// all 3 args). 1/1 stmt aligns with zero size/quantity divergence; the 100% is real.
}

// STATE[100%|DONE]
void udp_network_flow_emulator::on_packet_received(
	pbyte const		buffer,
	const u32		buffer_size,
	boost::asio::ip::udp::endpoint const&	endpoint,
	const u32		time_in_ms,
	const u32		unacknowledged_packets_count
)
{
	if ( m_lost_packets_random.random_f( 1.f ) > m_lost_packet_probability )
		add_packet( buffer, buffer_size, endpoint, time_in_ms, unacknowledged_packets_count );
	else
		make_packet_lost( buffer, buffer_size, endpoint );
}


} // namespace network_core
} // namespace vostok
