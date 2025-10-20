////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED

namespace vostok {
namespace network_core {

/* vostok::network_core::udp_match_server */

// Interesting! We got a header for class to which we don't have source code for

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class intrusive_list<udp_match_client_session,udp_match_client_session *,1388,threading::single_threading_policy,size_policy,no_debug_policy>;
class boost::intrusive::set<udp_match_client_session,boost::intrusive::member_hook<udp_match_client_session,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,1372>,boost::intrusive::compare<udp_match_server::comparer>,boost::intrusive::none,boost::intrusive::none>;
class boost::function<void(udp_match_client_session &,u8,packet_reader &)>;
class handler_allocator;
class udp_network_flow_emulator;
struct udp_match_packets_orderer;
class boost::asio::ip::udp::socket;
class boost::asio::ip::udp::endpoint;
class memory::single_size_buffer_allocator<300,threading::single_threading_policy>;
class boost::array<u8,256>;
class boost::noncopyable;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class udp_match_server public : boost::noncopyable {
public:
	inline									udp_match_server			(
												boost::asio::io_service&			arg_0,
												memory::single_size_buffer_allocator<300,threading::single_threading_policy>&	arg_1,
												udp_match_packets_orderer&			arg_2,
												udp_network_flow_emulator*			arg_3
											) { /* no source */ }

	virtual	inline							~udp_match_server			( ) { /* no source */ }

	inline	void							remove_disconnected_clients	( ) { /* no source */ }

	inline	void							send_queued_packets			( u32 arg_0 ) { /* no source */ }

	inline	bool							are_there_any_queued_packets( ) const { /* no source */ }

	inline	u32								clients_count				( ) const { /* no source */ }

	inline	udp_match_stats					get_stats					( ) const { /* no source */ }

	inline	boost::asio::io_service&		io_service					( ) { /* no source */ }

	inline	void							set_on_packet_received		( boost::function<void(udp_match_client_session &,u8,packet_reader &)> const& arg_0 ) { /* no source */ }

	inline	void							start_accepting				( u16 arg_0 ) { /* no source */ }

	inline	void							stop_accepting				( ) { /* no source */ }

	inline	void							enqueue						( udp_match_client_session& arg_0, udp_match_packet* arg_1 ) { /* no source */ }

	virtual	inline	void							delete_client				( udp_match_client_session*& arg_0 ) { /* no source */ }

	virtual	inline	udp_match_client_session*		new_client					( boost::asio::ip::udp::endpoint const& arg_0 ) { /* no source */ }

	virtual	inline	void							delete_client_impl			( udp_match_client_session*& arg_0 ) { /* no source */ }

	inline	void							process_incoming_packet		( packet_reader& arg_0, boost::asio::ip::udp::endpoint const& arg_1 ) { /* no source */ }

	inline	void							start_receiving				( ) { /* no source */ }

	inline	u32								unacknowledged_packets_count( ) const { /* no source */ }

	inline	void							handle_receive				( boost::system::error_code const& arg_0, u32 arg_1 ) { /* no source */ }

	inline	void							on_error					( client_error_codes_enum arg_0, boost::system::error_code arg_1 ) { /* no source */ }

	inline	bool							try_reopen_socket			( ) { /* no source */ }


private:
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	boost::function<void(udp_match_client_session &,u8,packet_reader &)>	m_on_packet_received;
	/* 0x0028 */	boost::asio::ip::udp::socket		m_socket;
	/* 0x006c */	boost::asio::ip::udp::endpoint		m_remote_endpoint;
	/* 0x0088 */	boost::intrusive::set<udp_match_client_session,boost::intrusive::member_hook<udp_match_client_session,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,1372>,boost::intrusive::compare<udp_match_server::comparer>,boost::intrusive::none,boost::intrusive::none>	m_clients;
	/* 0x009c */	intrusive_list<udp_match_client_session,udp_match_client_session *,1388,threading::single_threading_policy,size_policy,no_debug_policy>	m_clients_to_destroy;
	/* 0x00ac */	udp_match_packets_orderer&			m_packets_orderer;
	/* 0x00b0 */	udp_network_flow_emulator*			m_network_flow_emulator;
	/* 0x00b4 */	memory::single_size_buffer_allocator<300,threading::single_threading_policy>&	m_packets_allocator;
	/* 0x00b8 */	boost::array<u8,256>				m_receive_buffer;
	/* 0x01b8 */	u32									m_time_in_ms;
	/* 0x01bc */	bool								m_should_reopen_socket;
	/* 0x01bd */	bool								m_is_accepting;
	/* 0x01c0 */	handler_allocator					m_handler_allocator;
}; // class udp_match_server

STATIC_SIZE_ASSERT(udp_match_server, 0x5C8);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED
