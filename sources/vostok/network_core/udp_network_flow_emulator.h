////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_H_INCLUDED
#define NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_H_INCLUDED

namespace vostok {
namespace network_core {

/* vostok::network_core::udp_network_flow_emulator */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class vectora<std::pair<udp_match_packet *,boost::asio::ip::udp::endpoint > >;
class memory::single_size_buffer_allocator<300,threading::single_threading_policy>;
class random32;
class core::noncopyable;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class udp_network_flow_emulator public : core::noncopyable {
public:
					udp_network_flow_emulator	( memory::base_allocator& allocator, memory::single_size_buffer_allocator<300,threading::single_threading_policy>& packets_allocator, udp_network_flow_emulator_options const& options );

					~udp_network_flow_emulator	( );

			void	tick						( u32 time_in_ms, boost::function<void(packet_reader &,boost::asio::ip::udp::endpoint const &)> const& functor );

			void	on_packet_received			(
						pbyte								buffer,
						u32									buffer_size,
						boost::asio::ip::udp::endpoint const&	endpoint,
						u32									time_in_ms,
						u32									unacknowledged_packets_count
					);

	inline	u32		max_ping_time_in_ms			( ) const { /* no source */ }

	inline	u32		delayed_packets_count		( ) const { /* no source */ }

			void	add_packet					(
						pbyte								buffer,
						u32									buffer_size,
						boost::asio::ip::udp::endpoint const&	endpoint,
						u32									time_in_ms,
						u32									unacknowledged_packets_count
					);

			void	make_packet_lost			( pbyte buffer, u32 buffer_size, boost::asio::ip::udp::endpoint const& endpoint );


private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	vectora<std::pair<udp_match_packet *,boost::asio::ip::udp::endpoint > >	m_delayed_packets;
	/* 0x0010 */	random32							m_lost_packets_random;
	/* 0x0014 */	random32							m_ping_random;
	/* 0x0018 */	random32							m_out_of_order_random;
	/* 0x001c */	memory::single_size_buffer_allocator<300,threading::single_threading_policy>&	m_packets_allocator;
	/* 0x0020 */	pcstr								m_logging_id;
	/* 0x0024 */	float								m_lost_packet_probability;
	/* 0x0028 */	u32									m_min_ping_time_in_ms;
	/* 0x002c */	u32									m_max_ping_time_in_ms;
}; // class udp_network_flow_emulator

STATIC_SIZE_ASSERT(udp_network_flow_emulator, 0x30);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_H_INCLUDED
