////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED

#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/intrusive/set.hpp>

#include <vostok/intrusive_list.h>
#include <vostok/memory_single_size_buffer_allocator.h>

#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/sequence_number.h>
#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/network_core/udp_match_types.h>

namespace vostok {
namespace network_core {

class base_packet;
class packet_reader;

class udp_match_connection : public boost::noncopyable {
public:
	enum state
	{
		connected					= 0x0,
		initiating_disconnection	= 0x1,
		confirming_disconnection	= 0x2,
		disconnected				= 0x3,
	}; // enum state

	struct comparer {
		inline	bool	operator()	( udp_match_packet const& arg_0, udp_match_packet const& arg_1 ) const { VOSTOK_UNREFERENCED_PARAMETERS( arg_0, arg_1 ); return false; }
		inline	bool	operator()	( const sequence_number< u16 > arg_0, udp_match_packet const& arg_1 ) const { VOSTOK_UNREFERENCED_PARAMETERS( arg_0, arg_1 ); return false; }
		inline	bool	operator()	( udp_match_packet const& arg_0, const sequence_number< u16 > arg_1 ) const { VOSTOK_UNREFERENCED_PARAMETERS( arg_0, arg_1 ); return false; }
	}; // struct comparer

	struct channel {
		inline			channel	( ) { }
		inline	void	reset	( ) { }
		inline			~channel( ) { }

	public:
		// claude@NOTE: PDB encodes the set member-hook as byte offset 8
		// (&udp_match_packet::set_member_hook); boost::intrusive takes the member ptr.
		/* 0x0000 */	boost::intrusive::set< udp_match_packet, boost::intrusive::member_hook< udp_match_packet, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_packet::set_member_hook >, boost::intrusive::compare< udp_match_connection::comparer >, boost::intrusive::none, boost::intrusive::none >	packets;
		/* 0x0014 */	sequence_number< u16 >		received_order_id;
		/* 0x0016 */	sequence_number< u16 >		sent_order_id;
	}; // struct channel

	typedef	intrusive_list< udp_match_packet, udp_match_packet*, &udp_match_packet::next, threading::single_threading_policy, size_policy, no_debug_policy >	udp_match_packet_list;

public:
										udp_match_connection			(
											boost::asio::ip::udp::socket&		socket,
											boost::asio::ip::udp::endpoint const&	remote_endpoint,
											memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
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
	inline	void						set_on_disconnect				( boost::function< void( disconnect_event_types_enum ) > const& callback ) { m_on_disconnect = callback; }

	inline	bool						is_connected					( ) const { return false; }
	inline	bool						has_disconnection_initiated		( ) const { return false; }
	inline	bool						is_disconnecting				( ) const { return false; }
	inline	bool						is_disconnected					( ) const { return false; }
	inline	void						set_disconnected				( ) { }

	inline	udp_match_packet*			new_packet						( const u8 arg_0 ) { VOSTOK_UNREFERENCED_PARAMETER( arg_0 ); return NULL; }
	inline	void						delete_packet					( udp_match_packet*& arg_0 ) { VOSTOK_UNREFERENCED_PARAMETER( arg_0 ); }

	inline	void						set_max_packet_wait_time_in_ms	( const u32 arg_0 ) { VOSTOK_UNREFERENCED_PARAMETER( arg_0 ); }

	inline	bool						are_there_any_queued_packets	( ) const { return false; }

	inline	u32							unacknowledged_packets_count	( ) const { return 0; }
			u32							packets_count					( ) const;

	inline	udp_match_stats const&		get_stats						( ) const { return m_stats; }

	inline	u32							last_send_time_in_ms			( ) const { return m_last_send_time_in_ms; }
	inline	u32							last_receive_time_in_ms			( ) const { return m_last_receive_time_in_ms; }
	inline	u32							last_activity_time_in_ms		( ) const { return 0; }

	inline	u32							pending_operations_count		( ) const { return m_pending_operations_count; }

	template < typename predicate_type >
			void						process_incoming_packet			( packet_reader& reader, predicate_type const& predicate );

	static	inline	void				construct_packet				(
											udp_match_packets_orderer&		packets_orderer,
											udp_match_packet&				packet,
											u8								message_type
										) { VOSTOK_UNREFERENCED_PARAMETERS( packets_orderer, packet, message_type ); }
	static	bool						is_low_level_packet				( base_packet const& packet );

private:
			// PDB: both params __formal (genuinely unused in the original)
			void						on_error						( client_error_codes_enum client_error_code, boost::system::error_code error_code );

			void						handle_send						(
											udp_match_packet*					packet,
											boost::system::error_code const&	error_code,
											u32									bytes_transferred
										);

			void						fill_packet_header				( udp_match_packet& packet );

			void						send							( udp_match_packet* packet );

			void						send_packets_list				( udp_match_packet* packets_list, u32 packets_count );

			// claude@NOTE: canonical header shows inline/<u8>, but the actual symbol
			// (rich index + .cpp carcass) is a private out-of-line member with <u16>
			// args - the real symbol wins.
			void						update_acknowledgements			( sequence_number< u16 > remote_sequence_id, sequence_number< u16 > local_sequence_id, u16 local_acknowledgement_bits );

			void						dump							( pcstr const caption, const u32 current_time_in_ms );

			void						process_low_level_message		( packet_reader& reader, const u32 time_in_ms );

			udp_match_packet*			new_low_level_packet			( u8 message_type );

	inline	void						disconnect_impl					( ) { }
			void						enqueue_impl					( udp_match_packet* packet );

	template < typename predicate_type >
			void						call_predicate					( predicate_type const& predicate, packet_reader& reader );

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	udp_match_stats						m_stats;
	/* 0x0080 */	udp_match_packet_list				m_packets_to_send;
	/* 0x0090 */	udp_match_packet_list				m_outgoing_packets;
	/* 0x00a0 */	udp_match_packet_list				m_unacknowledged_packets;
	/* 0x00b0 */	boost::array< udp_match_connection::channel, 1 >	m_channels;
	/* 0x00c8 */	boost::function< void( disconnect_event_types_enum ) >	m_on_disconnect;
	/* 0x00e8 */	boost::asio::ip::udp::socket&		m_socket;
	/* 0x00ec */	boost::asio::ip::udp::endpoint const&	m_remote_endpoint;
	/* 0x00f0 */	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	m_packets_allocator;
	/* 0x00f4 */	udp_match_packets_orderer&			m_packets_orderer;
	/* 0x00f8 */	pcstr								m_logging_id;
	/* 0x00fc */	long								m_last_receive_time_in_ms;
	/* 0x0100 */	const u32							m_disconnection_timeout_in_ms;
	/* 0x0104 */	u32									m_last_send_time_in_ms;
	/* 0x0108 */	long								m_last_send_attempt_time_in_ms;
	/* 0x010c */	u32									m_max_packet_wait_time_in_ms;
	/* 0x0110 */	const u32							m_max_idle_time_in_ms;
	/* 0x0114 */	u32									m_disconnection_receive_time_in_ms;
	/* 0x0118 */	u32									m_pending_operations_count;
	/* 0x011c */	udp_match_connection::state			m_state;
	/* 0x0120 */	u16									m_remote_acknowledgement_bits;
	/* 0x0122 */	u16									m_received_local_acknowledgement_bits;
	/* 0x0124 */	sequence_number< u8 >				m_local_sequence_id;
	/* 0x0125 */	sequence_number< u8 >				m_remote_sequence_id;
	/* 0x0126 */	sequence_number< u8 >				m_received_local_sequence_id;
	/* 0x0127 */	sequence_number< u8 >				m_disconnection_local_sequence_id;
	/* 0x0128 */	handler_allocator					m_handler_allocator;
}; // class udp_match_connection

STATIC_SIZE_ASSERT( udp_match_connection, 0x530 );
STATIC_SIZE_ASSERT( udp_match_connection::channel, 0x18 );
STATIC_SIZE_ASSERT( udp_match_connection::comparer, 0x1 );

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/udp_match_connection_inline.h>

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED
