////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_H_INCLUDED
#define NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <vostok/vectora.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/memory_single_size_buffer_allocator.h>
#include <boost/function.hpp>

#include <boost/asio.hpp>

namespace vostok {

namespace memory {
	class base_allocator;
} // namespace memory

namespace network_core {

class packet_reader;
class udp_match_packet;
struct udp_network_flow_emulator_options;

class udp_network_flow_emulator : private core::noncopyable {
public:
					udp_network_flow_emulator	(
						memory::base_allocator&		allocator,
						memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
						udp_network_flow_emulator_options const&	options
					);
					~udp_network_flow_emulator	( );

			void	tick						(
						u32		time_in_ms,
						boost::function< void( packet_reader&, boost::asio::ip::udp::endpoint const& ) > const&	functor
					);

			void	on_packet_received			(
						pbyte		buffer,
						u32			buffer_size,
						boost::asio::ip::udp::endpoint const&	endpoint,
						u32			time_in_ms,
						u32			unacknowledged_packets_count
					);

	inline	u32		max_ping_time_in_ms			( ) const { return m_max_ping_time_in_ms; }

	inline	u32		delayed_packets_count		( ) const { return m_delayed_packets.size( ); }

private:
			void	add_packet					(
						pbyte		buffer,
						u32			buffer_size,
						boost::asio::ip::udp::endpoint const&	endpoint,
						u32			time_in_ms,
						u32			unacknowledged_packets_count
					);

			void	make_packet_lost			( pbyte buffer, u32 buffer_size, boost::asio::ip::udp::endpoint const& endpoint );

private:
	/* 0x0000 */	vectora< std::pair< udp_match_packet*, boost::asio::ip::udp::endpoint > >	m_delayed_packets;
	/* 0x0010 */	math::random32		m_lost_packets_random;
	/* 0x0014 */	math::random32		m_ping_random;
	/* 0x0018 */	math::random32		m_out_of_order_random;
	/* 0x001c */	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	m_packets_allocator;
	/* 0x0020 */	pcstr const			m_logging_id;
	/* 0x0024 */	const float			m_lost_packet_probability;
	/* 0x0028 */	const u32			m_min_ping_time_in_ms;
	/* 0x002c */	const u32			m_max_ping_time_in_ms;
}; // class udp_network_flow_emulator

STATIC_SIZE_ASSERT(udp_network_flow_emulator, 0x30);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_H_INCLUDED
