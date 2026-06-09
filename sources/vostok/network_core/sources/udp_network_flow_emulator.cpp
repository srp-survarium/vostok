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

// STATE[87%|PARTIAL]: only residual is the three random32 seeds. Target seeds
// m_lost_packets_random/m_ping_random/m_out_of_order_random with relocated .rdata
// addresses (mov ecx, 995A34h/35h/36h - consecutive, a reloc per the obj); base
// default-constructs them (xor ecx,ecx, seed 0). Source seed expression unknown
// (likely (u32)&<some const>); rest of init-list matches. claude@TODO: recover seeds.
 udp_network_flow_emulator::udp_network_flow_emulator(
	memory::base_allocator&		allocator,
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
	udp_network_flow_emulator_options const&	options
) :
	m_delayed_packets	( &allocator ),
	m_packets_allocator	( packets_allocator ),
	m_logging_id		( options.logging_id ),
	m_lost_packet_probability	( options.lost_packet_probability ),
	m_min_ping_time_in_ms	( options.min_ping_time_in_ms ),
	m_max_ping_time_in_ms	( options.max_ping_time_in_ms )
{
}

// STATE[58%|PARTIAL]: structure aligns (4/4 stmts). Residual is /Od iterator-temp
// materialization in the while condition - target stores begin()/end() into stack
// temps and does a direct cmp/je; base converts the != to a bool (sete/test/jne).
// Same source shape, different STL-iterator-compare codegen; larger frame in target.
// sushi@TODO: structure is WRONG here (disputes the 4/4 "aligns" claim) - the
// while-condition codegen divergence is a real shape miss, not just regalloc. See
// review_todos.md; likely the loop is a different form (the begin()!=end() vs a
// cached-end / for-loop), revisit before trusting the 58%.
 udp_network_flow_emulator::~udp_network_flow_emulator( )
{
	while ( m_delayed_packets.begin( ) != m_delayed_packets.end( ) ) {
		delete_udp_match_packet( m_packets_allocator, m_delayed_packets.back( ).first );
		m_delayed_packets.pop_back( );
	}
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

// STATE[STUB|INPROGRESS]: from 0x728a30 - the big one. Shape: build a stack
// buffer_vector<pair<udp_match_packet*,endpoint>> delayed_packets_to_appear sized
// from m_delayed_packets.size(); remove_if( m_delayed_packets, delayed_packets_predicate(
// delayed_packets_to_appear, time_in_ms ) ) then erase the moved tail; random_shuffle the
// appeared range with m_out_of_order_random (this+0x18); for each appeared packet build a
// packet_reader, read u16 received_local_sequence_id + remote_sequence_id, call functor(
// reader, endpoint ), then delete_udp_match_packet( m_packets_allocator, packet ). The
// predicate operator() (??Rdelayed_packets_predicate) is instantiated by this remove_if.
// NEXT: stand up the buffer_vector alloca + remove_if/erase pattern, then the appear loop.
void udp_network_flow_emulator::tick(
	const u32		time_in_ms,
	boost::function< void( packet_reader&, boost::asio::ip::udp::endpoint const& ) > const&	functor
)
{
	// LOCALS
	// buffer_vector< std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint > > delayed_packets_to_appear
	// std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint >* e<1>
	// std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint >* i<1>
	// packet_reader 					reader<2>
	// const u16 						received_local_sequence_id<3>
	// packet_reader 					reader<3>
	// const u16 						remote_sequence_id<3>
	// ******


	// FUNCTION BODY[0x738a30]: 30
	// <0x738a40>|0x010|+0x01d:'63'
	// <0x738a5d>|0x02d|+0x005:'64'
	// <0>
	// <0x738a62>|0x032|+0x060:'66'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x738ac2>|0x092|+0x06b:'74'
	// <0>
	// <0x738b2d>|0x0fd|+0x012:'76'
	// <0x738b3f>|0x10f|+0x026:'77'
	// <0>
	// <0x738b65>|0x135|+0x020:'79'
	// <0x738b85>|0x155|+0x023|[1]:'80'
	// <0>
	// <0x738ba8>|0x178|+0x05f|[3]:'82'
	// <0>
	// <0x738c07>|0x1d7|+0x00c:'84'
	// <0x738c13>|0x1e3|+0x00c:'85'
	// <0>
	// <1>
	// <2>
	// <0x738c1f>|0x1ef|+0x05f:'89'
	// <0x738c7e>|0x24e|+0x013:'90'
	// <0x738c91>|0x261|+0x016:'91'
	// <0x738ca7>|0x277|+0x005:'92'
	// ******
}

// STATE[73%|PARTIAL]: structure matches (logic/calls all aligned). Residual is /Od
// register allocation (edx<->ecx) and a larger target frame (0xB8 vs 0x94) - target
// materializes more iterator/temp slots. The two r<u16> reads map to different source
// lines in target (L109/L112); placing them to match exactly + recovering the extra
// temps would close it. claude@TODO: line-position of the sequence-id reads.
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
}

// STATE[100%|DONE]
// sushi@TODO: 100% without structure - objdiff pairs this at 100% but there's no
// structure-diff backing it (trivial shape: two unreferenced-param eaters + one
// is_low_level_packet call). The 100% is not structurally verified; confirm the
// body shape against target before trusting it. See review_todos.md.
void udp_network_flow_emulator::make_packet_lost(
	pbyte const		buffer,
	const u32		buffer_size,
	boost::asio::ip::udp::endpoint const&	endpoint
)
{
	VOSTOK_UNREFERENCED_PARAMETER( endpoint );

	const bool	is_low_level_packet	= udp_match_connection::is_low_level_packet( base_packet( buffer, buffer_size ) );
	VOSTOK_UNREFERENCED_PARAMETER( is_low_level_packet );
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
