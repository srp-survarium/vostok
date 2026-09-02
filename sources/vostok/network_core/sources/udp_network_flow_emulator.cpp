// SPDX-License-Identifier: GPL-3.0-or-later
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
// sushi@TODO: target derives this from boost::noncopyable, but std::remove_if
// copy-constructs the predicate (C2248) - cannot add the base without rework.
class delayed_packets_predicate {
public:
	inline	delayed_packets_predicate	(
		vostok::buffer_vector< std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > >&	delayed_packets_to_appear,
		const u32	time_in_ms
	) :
		m_packets		( delayed_packets_to_appear ),
		m_time_in_ms	( time_in_ms )
	{ }

	bool	operator()	( std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > const& message ) const;

private:
	/* 0x0000 */	vostok::buffer_vector< std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > >&	m_packets;
	/* 0x0004 */	const u32	m_time_in_ms;
}; // class delayed_packets_predicate

// sushi@review: the size verifier swept only headers, so this .cpp-local class
// (and its ctor) was missed. Its size is 0x8 (ref + const u32). The ctor + operator()
// emit no standalone symbol until tick's remove_if instantiates them (see STATE below).
STATIC_SIZE_ASSERT(delayed_packets_predicate, 0x8);

namespace vostok {
namespace network_core {

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
}

 udp_network_flow_emulator::~udp_network_flow_emulator( )
{
	while ( m_delayed_packets.begin( ) != m_delayed_packets.end( ) ) {
		delete_udp_match_packet( m_packets_allocator, m_delayed_packets.back( ).first );
		m_delayed_packets.pop_back( );
	}
}

} // namespace network_core
} // namespace vostok

bool delayed_packets_predicate::operator()(
	std::pair< vostok::network_core::udp_match_packet*, boost::asio::ip::udp::endpoint > const&	message
) const
{
	if ( m_time_in_ms < message.first->last_send_time_in_ms )
		return false;

	m_packets.push_back( message );
	return true;
}

namespace vostok {
namespace network_core {

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
}

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
