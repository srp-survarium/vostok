// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED

#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/noncopyable.hpp>

#include <vostok/intrusive_list.h>
#include <vostok/memory_single_size_buffer_allocator.h>

#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/sequence_number.h>
#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/network_core/disconnect_event_types_enum.h>
#include <vostok/network_core/udp_match_types.h>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class base_packet;
class packet_reader;

class udp_match_connection : private boost::noncopyable {
public:
	enum state
	{
		connected					= 0x0,
		initiating_disconnection	= 0x1,
		confirming_disconnection	= 0x2,
		disconnected				= 0x3,
	}; // enum state

	// low-level (connection-control) message ids carried in new_low_level_packet /
	// process_low_level_message; the values are inferred from the dispatch order.
	enum low_level_message_type_enum
	{
		initiate_disconnection		= 0x0,
		confirm_disconnection		= 0x1,
		continuous_flow				= 0x2,
	}; // enum low_level_message_type_enum

	// claude@MATCH: bodies read from the target key_nodeptr_comp<comparer,...> thunks
	// (0x122ed0/0x122f20/0x123120/0x1231e0): each compares order_id (packet @0x26)
	// via sequence_number<u16>::operator<.
	struct comparer {
		inline	bool	operator()	( udp_match_packet const& left, udp_match_packet const& right ) const { return left.order_id < right.order_id; }
		inline	bool	operator()	( const sequence_number< u16 > left, udp_match_packet const& right ) const { return left < right.order_id; }
		inline	bool	operator()	( udp_match_packet const& left, const sequence_number< u16 > right ) const { return left.order_id < right; }
	}; // struct comparer

	struct channel {
		inline			channel	( ) :
			received_order_id	( 0xFFFF ),
			sent_order_id		( 0 )
		{
		}

		inline	void	reset	( )
		{
			ASSERT( UNKNOWN_EXPRESSION_T( packets.empty( ) ) );
			packets.clear		( );
			received_order_id	= sequence_number< u16 >( 0xFFFF );
			sent_order_id		= sequence_number< u16 >( 0 );
		}

		inline			~channel( )
		{
		}

	public:
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
	inline	void						set_on_disconnect				( boost::function< void( enum disconnect_event_types_enum ) > const& value ) { m_on_disconnect = value; }

	// STATE[REMOVED] (the `/* no source */` shams below): consumed only by the
	// udp_match_server tick/delete_client path, which is dedicated-server code -
	// zero target symbols, never instantiated (server is absent from the shipped
	// client EXE), and no matched consumer in our scope ODR-uses them. The empty
	// shams are therefore correct (absent from BOTH binaries); reconstruct from a
	// consumer's bytes only if the server path is ever matched.
	inline	bool						is_connected					( ) const { return m_state == connected; }

	inline	bool						has_disconnection_initiated		( ) const { return false; /* no source */ } // STATE[REMOVED]

	inline	bool						is_disconnecting				( ) const { return false; /* no source */ } // STATE[REMOVED]
	inline	bool						is_disconnected					( ) const { return m_state == disconnected; }
	inline	void						set_disconnected				( ) { /* no source */ } // STATE[REMOVED]

	inline	udp_match_packet*			new_packet						( const u8 message_type ) { return NULL; /* no source */ } // STATE[REMOVED]
	inline	void						delete_packet					( udp_match_packet*& packet ) { delete_udp_match_packet( m_packets_allocator, packet ); }

	inline	void						set_max_packet_wait_time_in_ms	( const u32 value ) { /* no source */ } // STATE[REMOVED]

	inline	bool						are_there_any_queued_packets	( ) const { return false; /* no source */ } // STATE[REMOVED]

	inline	u32							unacknowledged_packets_count	( ) const { return m_unacknowledged_packets.size( ); }
			u32							packets_count					( ) const;

	inline	udp_match_stats const&		get_stats						( ) const { return m_stats; }

	inline	u32							last_send_time_in_ms			( ) const { return 0; /* no source */ } // STATE[REMOVED]
	// STATE[INLINED]: body from match_client::last_receive_time_in_ms 0x74c5f0
	// (single volatile load of m_last_receive_time_in_ms @+0xfc)
	inline	u32							last_receive_time_in_ms			( ) const { return m_last_receive_time_in_ms; }
	inline	u32							last_activity_time_in_ms		( ) const { return 0; /* no source */ } // STATE[REMOVED]

	inline	u32							pending_operations_count		( ) const { return 0; /* no source */ } // STATE[REMOVED]

private:
			void						on_error						( client_error_codes_enum client_error_code, boost::system::error_code error_code );

			void						handle_send						(
											udp_match_packet*					packet,
											boost::system::error_code const&	error_code,
											u32									bytes_transferred
										);

			void						fill_packet_header				( udp_match_packet& packet );

			void						send							( udp_match_packet* packet );

			void						send_packets_list				( udp_match_packet* packets_list, u32 packets_count );

			void						update_acknowledgements			( sequence_number< u16 > remote_sequence_id, sequence_number< u16 > local_sequence_id, u16 local_acknowledgement_bits );

			// claude@MATCH: pcstr const - the target mangles QBDI (top-level pointer
			// const lives in the member declaration, the silent-join variant).
			void						dump							( pcstr const caption, const u32 current_time_in_ms );

			void						process_low_level_message		( packet_reader& reader, const u32 time_in_ms );

public:
	template < typename Predicate >
			void						process_incoming_packet			( packet_reader& reader, Predicate const& predicate );

private:
	template < typename Predicate >
			void						call_predicate					( Predicate const& predicate, packet_reader& reader );

			udp_match_packet*			new_low_level_packet			( u8 message_type );

	inline	void						disconnect_impl					( ) { /* no source */ } // STATE[REMOVED]
			void						enqueue_impl					( udp_match_packet* packet );

public:
	static			void				construct_packet				(
											udp_match_packets_orderer&		packets_orderer,
											udp_match_packet&				packet,
											u8								message_type
										);
	static	bool						is_low_level_packet				( base_packet const& packet );

private:
	/* 0x0000 */	udp_match_stats						m_stats;
	/* 0x0080 */	udp_match_packet_list				m_packets_to_send;
	/* 0x0090 */	udp_match_packet_list				m_outgoing_packets;
	/* 0x00a0 */	udp_match_packet_list				m_unacknowledged_packets;
	/* 0x00b0 */	boost::array< channel, 1 >			m_channels;
	/* 0x00c8 */	boost::function< void( enum disconnect_event_types_enum ) >	m_on_disconnect;
	/* 0x00e8 */	boost::asio::ip::udp::socket&		m_socket;
	/* 0x00ec */	boost::asio::ip::udp::endpoint const&	m_remote_endpoint;
	/* 0x00f0 */	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	m_packets_allocator;
	/* 0x00f4 */	udp_match_packets_orderer&			m_packets_orderer;
	/* 0x00f8 */	pcstr								m_logging_id;
	// PDB records volatile long here AND at m_last_send_attempt_time_in_ms
	// (pdb-parser renders cv-qualifiers now) - spelled threading::atomic32_type.
	/* 0x00fc */	threading::atomic32_type			m_last_receive_time_in_ms;
	/* 0x0100 */	const u32							m_disconnection_timeout_in_ms;
	/* 0x0104 */	u32									m_last_send_time_in_ms;
	/* 0x0108 */	threading::atomic32_type			m_last_send_attempt_time_in_ms;
	/* 0x010c */	u32									m_max_packet_wait_time_in_ms;
	/* 0x0110 */	const u32							m_max_idle_time_in_ms;
	/* 0x0114 */	u32									m_disconnection_receive_time_in_ms;
	/* 0x0118 */	u32									m_pending_operations_count;
	/* 0x011c */	state								m_state;
	/* 0x0120 */	u16									m_remote_acknowledgement_bits;
	/* 0x0122 */	u16									m_received_local_acknowledgement_bits;
	/* 0x0124 */	sequence_number< u16 >				m_local_sequence_id;
	/* 0x0126 */	sequence_number< u16 >				m_remote_sequence_id;
	/* 0x0128 */	sequence_number< u16 >				m_received_local_sequence_id;
	/* 0x012a */	sequence_number< u16 >				m_disconnection_local_sequence_id;
	/* 0x0130 */	handler_allocator					m_handler_allocator;
}; // class udp_match_connection

STATIC_SIZE_ASSERT(udp_match_connection, 0x538);
STATIC_SIZE_ASSERT(udp_match_connection::channel, 0x18);
STATIC_SIZE_ASSERT(udp_match_connection::comparer, 0x1);

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/udp_match_connection_inline.h>

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CONNECTION_H_INCLUDED
