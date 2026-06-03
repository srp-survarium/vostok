////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED

typedef
	boost::asio::basic_socket<boost::asio::ip::udp, boost::asio::datagram_socket_service<boost::asio::ip::udp> >
	lowest_layer_type;

typedef
	boost::asio::datagram_socket_service<boost::asio::ip::udp>
	service_type;

typedef
	boost::asio::ip::basic_endpoint<boost::asio::ip::udp>
	endpoint_type;

typedef
	boost::asio::ip::udp
	protocol_type;

typedef
	boost::function<void __cdecl(udp_match_client_session &, u8, packet_reader &)>
	on_packet_received_type;


namespace vostok {
namespace network_core {

class udp_match_connection public : boost::noncopyable {
public:
										udp_match_connection			(
											boost::asio::ip::udp::socket&		socket,
											boost::asio::ip::udp::endpoint const&	remote_endpoint,
											memory::single_size_buffer_allocator<300,threading::single_threading_policy>&	packets_allocator,
											udp_match_packets_orderer&			packets_orderer,
											u32									disconnection_timeout_in_ms,
											u32									max_packet_wait_time_in_ms,
											u32									max_idle_time_in_ms,
											pcstr								logging_id
										);

										~udp_match_connection			( );

			void						connect							( udp_match_packet* packet );
			void						enqueue							( udp_match_packet* packet );
			void						send_queued_packets				( u32 current_time_in_ms );
			void						disconnect						( );
			void						instant_disconnect				( disconnect_event_types_enum type );
	inline	void						set_on_disconnect				( boost::function<void(enum disconnect_event_types_enum)> const& arg_0 ) { /* no source */ }

	inline	bool						is_connected					( ) const { /* no source */ }
	inline	bool						has_disconnection_initiated		( ) const { /* no source */ }
	inline	bool						is_disconnecting				( ) const { /* no source */ }
	inline	bool						is_disconnected					( ) const { /* no source */ }
	inline	void						set_disconnected				( ) { /* no source */ }

	inline	udp_match_packet*			new_packet						( u8 arg_0 ) { /* no source */ }
	inline	void						delete_packet					( udp_match_packet*& arg_0 ) { /* no source */ }

	inline	void						set_max_packet_wait_time_in_ms	( u32 arg_0 ) { /* no source */ }
	inline	bool						are_there_any_queued_packets	( ) const { /* no source */ }

	inline	u32							unacknowledged_packets_count	( ) const { /* no source */ }
			u32							packets_count					( ) const;

	inline	udp_match_stats const&		get_stats						( ) const { /* no source */ }

	inline	u32							last_send_time_in_ms			( ) const { /* no source */ }
	inline	u32							last_receive_time_in_ms			( ) const { /* no source */ }
	inline	u32							last_activity_time_in_ms		( ) const { /* no source */ }

	inline	u32							pending_operations_count		( ) const { /* no source */ }

			void						on_error						( client_error_codes_enum __formal, boost::system::error_code __formal );
			void						handle_send						( udp_match_packet* packet, boost::system::error_code const& error_code, u32 bytes_transferred );
			void						fill_packet_header				( udp_match_packet& packet );
			void						send							( udp_match_packet* packet );
			void						send_packets_list				( udp_match_packet* packets_list, u32 packets_count );
			void						update_acknowledgements			( sequence_number<u16> remote_sequence_id, sequence_number<u16> local_sequence_id, u16 local_acknowledgement_bits );

			void						dump							( pcstr caption, u32 current_time_in_ms );

			void						process_low_level_message		( packet_reader& reader, u32 time_in_ms );
			udp_match_packet*			new_low_level_packet			( u8 message_type );

	inline	void						disconnect_impl					( ) { /* no source */ }
			void						enqueue_impl					( udp_match_packet* packet );

	static	void						construct_packet				( udp_match_packets_orderer& packets_orderer, udp_match_packet& packet, u8 message_type );
	static	bool						is_low_level_packet				( base_packet const& packet );


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	udp_match_stats						m_stats;
	/* 0x0080 */	intrusive_list<udp_match_packet,udp_match_packet *,28,threading::single_threading_policy,size_policy,no_debug_policy>	m_packets_to_send;
	/* 0x0090 */	intrusive_list<udp_match_packet,udp_match_packet *,28,threading::single_threading_policy,size_policy,no_debug_policy>	m_outgoing_packets;
	/* 0x00a0 */	intrusive_list<udp_match_packet,udp_match_packet *,28,threading::single_threading_policy,size_policy,no_debug_policy>	m_unacknowledged_packets;
	/* 0x00b0 */	boost::array<udp_match_connection::channel,1>	m_channels;
	/* 0x00c8 */	boost::function<void(enum disconnect_event_types_enum)>	m_on_disconnect;
	/* 0x00e8 */	boost::asio::ip::udp::socket&		m_socket;
	/* 0x00ec */	boost::asio::ip::udp::endpoint const&	m_remote_endpoint;
	/* 0x00f0 */	memory::single_size_buffer_allocator<300,threading::single_threading_policy>&	m_packets_allocator;
	/* 0x00f4 */	udp_match_packets_orderer&			m_packets_orderer;
	/* 0x00f8 */	pcstr								m_logging_id;
	/* 0x00fc */	long								m_last_receive_time_in_ms;
	/* 0x0100 */	u32									m_disconnection_timeout_in_ms;
	/* 0x0104 */	u32									m_last_send_time_in_ms;
	/* 0x0108 */	long								m_last_send_attempt_time_in_ms;
	/* 0x010c */	u32									m_max_packet_wait_time_in_ms;
	/* 0x0110 */	u32									m_max_idle_time_in_ms;
	/* 0x0114 */	u32									m_disconnection_receive_time_in_ms;
	/* 0x0118 */	u32									m_pending_operations_count;
	/* 0x011c */	udp_match_connection::state			m_state;
	/* 0x0120 */	u16									m_remote_acknowledgement_bits;
	/* 0x0122 */	u16									m_received_local_acknowledgement_bits;
	/* 0x0124 */	sequence_number<u16>				m_local_sequence_id;
	/* 0x0126 */	sequence_number<u16>				m_remote_sequence_id;
	/* 0x0128 */	sequence_number<u16>				m_received_local_sequence_id;
	/* 0x012a */	sequence_number<u16>				m_disconnection_local_sequence_id;
	/* 0x0130 */	handler_allocator					m_handler_allocator;
}; // class udp_match_connection

STATIC_SIZE_ASSERT(udp_match_connection, 0x538);

STATIC_SIZE_ASSERT(udp_match_connection, 0x538);


// STATE[STUB]
// vostok::network_core::udp_match_connection::channel::channel()
udp_match_connection::channel::channel( )
{
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::channel::~channel()
void udp_match_connection::channel::~channel( )
{
	// FUNCTION BODY[0x137510]: 4
	// <0x1374b9>|0x000|0x000:'156'
	// <0x1374c5>|0x00c|0x00c:'157'
	// <0x1374cd>|0x014|0x008:'158'
	// <0x1374e9>|0x030|0x01c:'159'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED