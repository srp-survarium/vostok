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

namespace vostok {
namespace network_core {

class delayed_packets_predicate {
public:
	inline	delayed_packets_predicate	(
		buffer_vector< std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint > >&	delayed_packets_to_appear,
		const u32	time_in_ms
	) :
		m_delayed_packets_to_appear	( delayed_packets_to_appear ),
		m_time_in_ms				( time_in_ms )
	{ }

	bool	operator()	( std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint > const& message ) const;

private:
	/* 0x0000 */	buffer_vector< std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint > >&	m_delayed_packets_to_appear;
	/* 0x0004 */	const u32	m_time_in_ms;
}; // class delayed_packets_predicate

// sushi@review: the size verifier swept only headers, so this .cpp-local class
// (and its ctor) was missed. Its size is 0x8 (ref + const u32). The ctor + operator()
// emit no standalone symbol until tick's remove_if instantiates them (see STATE below).
STATIC_SIZE_ASSERT(delayed_packets_predicate, 0x8);

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
	// structure-diff: 0 stmts / 0 size-diffs / 0 quantity-diffs (rich masks operands).
	// VERDICT: PARTIAL 87.59% - STRUCTURE CORRECT. Explicit seeded init now emits the
	// matching ??0random32@@QAE@I@Z(u32) ctor at +0x10/+0x14/+0x18 (was the default
	// `seed` path). Residual is purely the three seed immediates 0x995a34/35/36, which
	// in the TARGET delinked obj survive as literal bytes but in the BASE delink are
	// operand-masked to 0 (verified: any seed literal, address-range OR not e.g.0x12345,
	// is zeroed on base only). This is a DELINKER asymmetry (base operand masked, target
	// not), not a source miss - unrecoverable from C++. Seeds kept at target values so a
	// future delinker fix matches byte-exact.
}

// STATE[58%|PARTIAL]: structure CORRECT (4/4 stmts, 0 quantity-diffs); residual is
// the STL iterator-comparison codegen, NOT a loop-form miss (see body verdict).
 udp_network_flow_emulator::~udp_network_flow_emulator( )
{
	while ( m_delayed_packets.begin( ) != m_delayed_packets.end( ) ) {
		delete_udp_match_packet( m_packets_allocator, m_delayed_packets.back( ).first );
		m_delayed_packets.pop_back( );
	}

	// structure-diff: aligned 2, size-diffs 2, quantity-diffs 0, blank-gaps 0.
	// VERDICT: PARTIAL 58.46% - STRUCTURE CORRECT (disproves sushi@TODO "structure is
	// WRONG"). The condition divergence is NOT a different loop form: tried `!empty()`,
	// `!( begin()==end() )` and swapped operands - ALL fold to the identical base body
	// (raw-pointer iterators -> builtin cmp). The real residual: TARGET resolves the
	// iterator `!=` through a bool-returning operator (`cmp begin,[this+4]; sete; movzx;
	// test; jne`, end read INLINE, no temp) while BASE inlines raw-pointer `!=` (begin
	// AND end cached to stack temps `ebp-4`/`ebp-8`; direct `je`). Same source, divergent
	// STLport iterator-compare lowering (env/config), unsteerable from C++. The 4 stmts
	// (cond / delete / pop_back recompute / back-edge) align 1:1; only 2 carry SIZE diffs.
}

// STATE[INPROGRESS]: body reconstructed from 0x1369d0; emitted only once tick's
// remove_if instantiates it (no standalone base symbol until then), so currently 0%.
bool delayed_packets_predicate::operator()(
	std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint > const&	message
) const
{
	if ( m_time_in_ms < message.first->last_send_time_in_ms )
		return false;

	m_delayed_packets_to_appear.push_back( message );
	return true;
}

// STATE[INPROGRESS|PARTIAL]: from 0x728a30. Reconstructed the documented shape -
// alloca buffer_vector sized from size(); remove_if(.., delayed_packets_predicate)
// + erase the moved tail; random_shuffle the appeared range with m_out_of_order_random
// (this+0x18); appear-loop builds two packet_readers per packet (one reads the two dead
// u16 seq-ids, one is passed to functor) then delete_udp_match_packet. This remove_if
// instantiates ??Rdelayed_packets_predicate (and its ctor), so the predicate operator()
// now emits. Residual: the per-packet base_packet size arg (target's mis-symbolized
// accessor) + alloca/temp shapes - see body VERDICT. claude@TODO: exact reader size arg.
typedef std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint >	flow_emulator_packet_pair;

void udp_network_flow_emulator::tick(
	const u32		time_in_ms,
	boost::function< void( packet_reader&, boost::asio::ip::udp::endpoint const& ) > const&	functor
)
{
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

	std::random_shuffle(
		delayed_packets_to_appear.begin( ),
		delayed_packets_to_appear.end( ),
		m_out_of_order_random
	);

	flow_emulator_packet_pair*			i	= delayed_packets_to_appear.begin( );
	flow_emulator_packet_pair* const	e	= delayed_packets_to_appear.end( );
	for ( ; i != e; ++i ) {
		udp_match_packet* const	packet	= i->first;

		packet_reader	reader( base_packet( packet->m_buffer.data( ), packet->buffer_to_send_size( ) + packet->header_size( ) ) );
		const u16		received_local_sequence_id	= reader.r< u16 >( );
		const u16		remote_sequence_id			= reader.r< u16 >( );

		packet_reader	functor_reader( base_packet( packet->m_buffer.data( ), packet->buffer_to_send_size( ) + packet->header_size( ) ) );
		functor( functor_reader, i->second );

		delete_udp_match_packet( m_packets_allocator, i->first );

		VOSTOK_UNREFERENCED_PARAMETER( received_local_sequence_id );
		VOSTOK_UNREFERENCED_PARAMETER( remote_sequence_id );
	}

	// VERDICT: was STUB(3.49%). Reconstructed the full documented shape; this is the
	// first version that instantiates delayed_packets_predicate (its operator()/ctor
	// now emit). The two per-packet readers + the dead seq-id reads (mirroring
	// add_packet) match the target's <0x178>/<0x1ef> blocks. Open: the base_packet size
	// arg uses stubbed accessors (buffer_to_send_size + header_size both return 0 today),
	// and the appeared functor takes `i->second` (the endpoint). Measure after build.
}

// STATE[73%|PARTIAL]
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

	// structure-diff: target 10 stmts / base 9; aligned 3, size-diffs 4, quantity-diffs 5.
	// VERDICT: PARTIAL 73.08% - the lower half (memory::copy -> make_pair rep-movsd ->
	// push_back) is structurally IDENTICAL. Residual is at the TOP: the two dead
	// `reader.r<u16>()` reads. TARGET keeps both calls + the reader-ctor operator* (real
	// `call r<u16>` x2 at L109/L112); our LTCG proves the reader+values dead and DCEs
	// them to `xor;mov [seq],0` (no calls), shrinking the frame 0xB8->0x94. The reads
	// ARE written in source; this is whole-statement dead-read DCE + the operator()-vs-
	// random inline depth on m_ping_random, not a source-shape miss. Unsteerable without
	// inventing a use for the seq-ids (forbidden). claude@TODO superseded: not regalloc.
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
