// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/custom_alloc_handler.h>
#include <vostok/network_core/move_to_list_predicate.h>
#include <vostok/buffer_vector.h>

namespace vostok {
namespace network_core {

 udp_match_connection::udp_match_connection(
	boost::asio::ip::udp::socket&		socket,
	boost::asio::ip::udp::endpoint const&	remote_endpoint,
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
	udp_match_packets_orderer&			packets_orderer,
	const u32							disconnection_timeout_in_ms,
	const u32							max_packet_wait_time_in_ms,
	const u32							max_idle_time_in_ms,
	pcstr const							logging_id
) :
	m_socket			( socket ),
	m_remote_endpoint	( remote_endpoint ),
	m_packets_allocator	( packets_allocator ),
	m_packets_orderer	( packets_orderer ),
	m_logging_id		( logging_id ),
	m_last_receive_time_in_ms		( 0 ),
	m_disconnection_timeout_in_ms	( disconnection_timeout_in_ms ),
	m_last_send_time_in_ms			( 0 ),
	m_last_send_attempt_time_in_ms	( 0 ),
	m_max_packet_wait_time_in_ms	( max_packet_wait_time_in_ms ),
	m_max_idle_time_in_ms			( max_idle_time_in_ms ),
	m_disconnection_receive_time_in_ms	( 0 ),
	m_pending_operations_count		( 0 ),
	m_state				( disconnected ),
	m_remote_acknowledgement_bits		( 0 ),
	m_received_local_acknowledgement_bits	( 0 ),
	m_local_sequence_id				( 0xFFFF ),
	m_remote_sequence_id			( 0xFFFF ),
	m_received_local_sequence_id	( 0xFFFF ),
	m_disconnection_local_sequence_id	( 0xFFFF )
{
}

 udp_match_connection::~udp_match_connection( )
{
}

void udp_match_connection::on_error( client_error_codes_enum, boost::system::error_code )
{
}

// claude@NOTE: PARKED on an inline-vs-call wall. Statement count + the named-local
// set (reader, bits) match the target. The LTCG target keeps packet_reader::r<u16>()
// out-of-line (a `call` returning the value in ax); the single-TU base inlines the
// `inline`-marked template into the r(void*,u32,u32) form (push 2; push 2; lea slot;
// push; call). That byte divergence is why objdiff reports this unpaired. Not
// source-steerable without de-inlining the template at every other call site.
bool udp_match_connection::is_low_level_packet( base_packet const& packet )
{
	packet_reader	reader( packet );

	reader.r< u16 >( );
	reader.r< u16 >( );
	const u16	bits	= reader.r< u16 >( );

	if ( ( bits & 1 ) == 0 )
		return false;

	reader.advance( reader.r< bool >( ) );
	return reader.eof( );
}

void udp_match_connection::handle_send(
	udp_match_packet*					packet,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	--m_pending_operations_count;

	const bool	success	= m_outgoing_packets.erase( packet );
	ASSERT				( UNKNOWN_EXPRESSION_T( success ) );

	if ( !packet->is_reliable )
		delete_udp_match_packet	( m_packets_allocator, packet );

	else if ( m_state != connected &&
			  !is_low_level_packet( base_packet( packet->buffer_to_send( ), packet->buffer_to_send_size( ) ) ) )
		delete_udp_match_packet	( m_packets_allocator, packet );

	else {
		pbyte	buffer	= packet->buffer_to_send( );
		*buffer			= ( *reinterpret_cast< u16* >( buffer + 4 ) & 1 ) != 0;
		m_unacknowledged_packets.push_back( packet );
	}

	if ( error_code ) {
		LOG_ERROR	( "error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
		on_error	( unable_to_write_to_socket, error_code );
		return;
	}

	if ( !bytes_transferred ) {
		LOG_ERROR	( "unable to write to socket\r\n" );
		on_error	( unable_to_write_to_socket, error_code );
	}
}

void udp_match_connection::send( udp_match_packet* const packet )
{
	++m_stats.sent.packets.count;
	m_stats.sent.messages.bytes	+= packet->buffer_to_send_size( );
	m_stats.sent.packets.bytes	+= packet->buffer_to_send_size( ) + 46;

	++m_pending_operations_count;

	m_socket.async_send_to		(
		boost::asio::buffer		(
			packet->buffer_to_send( ),
			packet->buffer_to_send_size( )
		),
		m_remote_endpoint,
		make_custom_alloc_handler	(
			m_handler_allocator,
			boost::bind( &udp_match_connection::handle_send, this, packet, _1, _2 )
		)
	);
}

void udp_match_connection::fill_packet_header( udp_match_packet& packet )
{
	pbyte	buffer	= packet.m_buffer.data( );

	const udp_match_packets_count_enum	packet_type	= udp_match_packets_count_enum( *buffer );
	ASSERT( UNKNOWN_EXPRESSION_T( packet_type < 2 ) );

	reinterpret_cast< sequence_number< u16 >& >( packet.sequence_id ).serialize( buffer );
	m_remote_sequence_id.serialize( buffer );

	*reinterpret_cast< u16* >( buffer )	= u16( ( m_remote_acknowledgement_bits << 1 ) | ( packet_type == udp_match_multiple_packets ) );
	buffer	+= 2;
}

void udp_match_connection::send_packets_list( udp_match_packet* const packets_list, const u32 packets_count )
{
	m_stats.sent.messages.count	+= packets_count;

	if ( !packets_list->next ) {
		ASSERT					( UNKNOWN_EXPRESSION_T( packets_count == 1 ) );
		fill_packet_header		( *packets_list );

		m_stats.sent.data_bytes	+= packets_list->buffer_size( );

		if ( packets_list->send_count > 1 ) {
			++m_stats.resent.packets.count;
			m_stats.resent.packets.bytes	+= packets_list->buffer_to_send_size( ) + 46;

			++m_stats.resent.messages.count;
			m_stats.resent.messages.bytes	+= packets_list->buffer_size( );
			m_stats.resent.data_bytes		+= packets_list->buffer_size( );
		}

		m_outgoing_packets.push_back	( packets_list );

		send					( packets_list );
		return;
	}

	ASSERT( UNKNOWN_EXPRESSION_T( packets_count > 1 ) );
	udp_match_packet* const	packet_to_send	= new_udp_match_packet( m_packets_allocator );
	packet_to_send->is_reliable				= 0;
	reinterpret_cast< sequence_number< u16 >& >( packet_to_send->sequence_id )	= reinterpret_cast< sequence_number< u16 >& >( packets_list->sequence_id );
	*packet_to_send->buffer_to_send( )		= udp_match_multiple_packets;
	fill_packet_header				( *packet_to_send );

	buffer_vector< udp_match_packet* >	packets( ALLOCA( packets_count * sizeof( udp_match_packet* ) ), packets_count );

	for ( udp_match_packet* i = packets_list; i; i = i->next ) {
		ASSERT						( UNKNOWN_EXPRESSION_T( i->buffer_size( ) < 256 ) );

		m_stats.sent.data_bytes		+= i->buffer_size( );

		if ( i->send_count > 1 ) {
			++m_stats.resent.packets.count;
			m_stats.resent.packets.bytes	+= i->buffer_size( ) + 1;

			++m_stats.resent.messages.count;
			m_stats.resent.messages.bytes	+= i->buffer_size( ) + 1;
			m_stats.resent.data_bytes		+= i->buffer_size( );
		}

		packets.push_back			( i );
		packet_to_send->append		( u8( i->buffer_size( ) ) );
		packet_to_send->append		( i->buffer( ), i->buffer_size( ) );
	}

	m_outgoing_packets.push_back	( packet_to_send );

	send							( packet_to_send );

	for ( udp_match_packet** i = packets.begin( ), **e = packets.end( ) ; i != e ; ++i ) {
		if ( ( *i )->is_reliable )
			m_unacknowledged_packets.push_back( *i );

		else
			delete_udp_match_packet	( m_packets_allocator, *i );
	}
}

void udp_match_connection::dump( pcstr const caption, const u32 current_time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETERS( caption, current_time_in_ms );
}

} // namespace network_core
} // namespace vostok

// the sort/erase helpers of send_queued_packets live at GLOBAL scope - the target
// mangling carries no namespace (stlp_std::sort<..,packets_predicate> etc.).
struct packets_predicate {
	inline	bool	operator( )	(
						vostok::network_core::udp_match_packet const* const	left,
						vostok::network_core::udp_match_packet const* const	right
					) const
	{
		return left->buffer_size( ) < right->buffer_size( );
	}
}; // struct packets_predicate

class packets_in_list_predicate {
public:
	inline	explicit	packets_in_list_predicate	( vostok::network_core::sequence_number< u16 > const& sequence_id ) :
		m_sequence_id	( sequence_id )
	{
	}

	inline	bool		operator( )					( vostok::network_core::udp_match_packet const* const packet ) const
	{
		return reinterpret_cast< vostok::network_core::sequence_number< u16 > const& >( packet->sequence_id ) == m_sequence_id;
	}

private:
	/* 0x0000 */	vostok::network_core::sequence_number< u16 >	m_sequence_id;
}; // class packets_in_list_predicate

namespace vostok {
namespace network_core {

udp_match_packet* udp_match_connection::new_low_level_packet( const u8 message_type )
{
	udp_match_packet* const	packet	= new_udp_match_packet( m_packets_allocator );
	*packet->buffer_to_send( )		= udp_match_multiple_packets;

	packet->append					( u8( 1 ) );

	packet->append					( message_type );

	pcstr	message_id_string;
	switch ( message_type )
	{
		case initiate_disconnection :	message_id_string	= "initiatie disconnection";
										break;

		case confirm_disconnection :	message_id_string	= "confirm disconnection";
										break;

		case continuous_flow :			message_id_string	= "continuous flow";
										break;

		default :						message_id_string	= "<unknown low level message type>";
	}

	++m_stats.sent_low_level.packets.count;
	m_stats.sent_low_level.packets.bytes	+= packet->buffer_to_send_size( ) + 46;
	++m_stats.sent_low_level.messages.count;
	m_stats.sent_low_level.messages.bytes	+= packet->buffer_to_send_size( );
	++m_stats.sent_low_level.data_bytes;

	return							packet;
}

void udp_match_connection::send_queued_packets( const u32 current_time_in_ms )
{
	threading::interlocked_exchange	( m_last_send_attempt_time_in_ms, current_time_in_ms );

	switch ( m_state ) {
		case connected :
			if ( m_last_receive_time_in_ms && m_last_receive_time_in_ms + m_disconnection_timeout_in_ms <= current_time_in_ms ) {
				instant_disconnect	( disconnected_by_timeout );
				return;
			}
			break;

		case initiating_disconnection :
			ASSERT					( UNKNOWN_EXPRESSION );
			ASSERT					( UNKNOWN_EXPRESSION );

			m_packets_to_send.push_back( new_low_level_packet( initiate_disconnection ) );
			break;

		case confirming_disconnection :
			if ( m_disconnection_receive_time_in_ms + m_max_packet_wait_time_in_ms <= current_time_in_ms ) {
				instant_disconnect	( disconnected_by_initiator );
				return;
			}

			m_packets_to_send.push_back( new_low_level_packet( confirm_disconnection ) );
			break;

		case disconnected :
			ASSERT					( UNKNOWN_EXPRESSION );
			break;

		default :
			NODEFAULT				( );
	}

	m_unacknowledged_packets.remove_if	( move_to_list_predicate( m_packets_to_send, m_logging_id, current_time_in_ms, m_max_packet_wait_time_in_ms ) );

	u32	packets_count	= m_packets_to_send.size( );
	if ( !packets_count ) {
		if ( current_time_in_ms < m_last_send_time_in_ms + m_max_idle_time_in_ms )
		{
			return;
		}

		m_packets_to_send.push_back	( new_low_level_packet( continuous_flow ) );
		packets_count	= 1;
	}

	dump							( "before send_queued_packets", current_time_in_ms );

	buffer_vector< udp_match_packet* >	packets( ALLOCA( packets_count * sizeof( udp_match_packet* ) ), packets_count );

	while ( !m_packets_to_send.empty( ) ) {
		udp_match_packet* const	packet	= m_packets_to_send.pop_front( );
		packet->next				= NULL;

		reinterpret_cast< sequence_number< u16 >& >( packet->sequence_id )	= m_local_sequence_id;
		packets.push_back			( packet );
	}

	std::sort						( packets.begin( ), packets.end( ), packets_predicate( ) );

	while ( !packets.empty( ) ) {
		sequence_number< u16 >	test( m_local_sequence_id );
		if ( ++test <= m_received_local_sequence_id ) {
			for ( udp_match_packet** i = packets.begin( ), **e = packets.end( ) ; i != e ; ++i ) {
				if ( ( *i )->is_reliable )
					m_packets_to_send.push_back( *i );

				else
					delete_udp_match_packet( m_packets_allocator, *i );
			}

			break;
		}

		udp_match_packet* const	packet	= packets.back( );
		packets.pop_back			( );
		udp_match_packet*	packet_list_tail	= packet;

		packet->last_send_time_in_ms	= current_time_in_ms;
		ASSERT						( UNKNOWN_EXPRESSION );
		reinterpret_cast< sequence_number< u16 >& >( packet->sequence_id )	= ++m_local_sequence_id;
		ASSERT						( UNKNOWN_EXPRESSION );
		++packet->send_count;
		u32	packets_count			= 1;

		ASSERT						( UNKNOWN_EXPRESSION );
		u32	size_left				= packet->allocated_size( ) - packet->buffer_size( ) - 1;
		if ( size_left > 1 && !*packet->buffer_to_send( ) ) {
			for ( std::reverse_iterator< udp_match_packet** > i = packets.rbegin( ), e = packets.rend( ) ; i != e ; ++i )
				if ( size_left > ( *i )->buffer_size( ) && !*( *i )->buffer_to_send( ) ) {
					size_left				-= ( *i )->buffer_size( ) + 1;
					packet_list_tail->next	= *i;
					packet_list_tail		= *i;
					( *i )->next			= NULL;
					( *i )->last_send_time_in_ms	= current_time_in_ms;
					reinterpret_cast< sequence_number< u16 >& >( ( *i )->sequence_id )	= reinterpret_cast< sequence_number< u16 >& >( packet->sequence_id );
					++( *i )->send_count;
					++packets_count;
				}

			packets.erase			( std::remove_if( packets.begin( ), packets.end( ), packets_in_list_predicate( reinterpret_cast< sequence_number< u16 >& >( packet->sequence_id ) ) ), packets.end( ) );
		}

		m_last_send_time_in_ms		= current_time_in_ms;
		send_packets_list			( packet, packets_count );
	}

	dump							( "after  send_queued_packets", current_time_in_ms );
}

void udp_match_connection::connect( udp_match_packet* const packet )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_state == disconnected ) );

	m_state	= connected;

	if ( packet )
		enqueue_impl( packet );
}

void udp_match_connection::enqueue_impl( udp_match_packet* packet )
{
	if ( packet->is_ordered )
	{
		sequence_number< u16 >&	sent_order_id	= m_channels[ packet->channel_id ].sent_order_id;
		packet->order_id	= sent_order_id;
		pbyte	stream		= packet->buffer( ) + 1;
		sent_order_id.serialize( stream );
		++sent_order_id;
	}

	if ( packet->is_reliable )
		++m_stats.unacknowledged_packets;

	m_packets_to_send.push_back( packet );
}

void udp_match_connection::enqueue( udp_match_packet* packet )
{
	if ( m_state == connected )
		enqueue_impl( packet );
	else
	{
		ASSERT( UNKNOWN_EXPRESSION_T( m_state != connected ) );
		delete_udp_match_packet( m_packets_allocator, packet );
	}
}

} // namespace network_core
} // namespace vostok

// the acknowledgement purge helper of update_acknowledgements - GLOBAL scope per the
// target mangling (intrusive_list<..>::remove_if<sequence_id_predicate>).
class sequence_id_predicate : private boost::noncopyable {
public:
	inline			sequence_id_predicate	(
						vostok::memory::single_size_buffer_allocator< 300, vostok::threading::single_threading_policy >&	packets_allocator,
						const vostok::network_core::sequence_number< u16 >	sequence_id,
						pcstr const		logging_id
					) :
		m_packets_allocator	( packets_allocator ),
		m_logging_id		( logging_id ),
		m_sequence_id		( sequence_id )
	{
	}

	inline	bool	operator( )				( vostok::network_core::udp_match_packet* packet ) const
	{
		if ( reinterpret_cast< vostok::network_core::sequence_number< u16 >& >( packet->sequence_id ) != m_sequence_id )
			return false;

		delete_udp_match_packet( m_packets_allocator, packet );
		return true;
	}

private:
	/* 0x0000 */	vostok::memory::single_size_buffer_allocator< 300, vostok::threading::single_threading_policy >&	m_packets_allocator;
	/* 0x0004 */	pcstr const		m_logging_id;
	/* 0x0008 */	const vostok::network_core::sequence_number< u16 >	m_sequence_id;
}; // class sequence_id_predicate

STATIC_SIZE_ASSERT(sequence_id_predicate, 0xC);

namespace vostok {
namespace network_core {

// claude@NOTE: residual is an inline-vs-call wall. Statement count + the 8 named
// locals match the target. The target keeps the free template operator-< u16 >( ) and
// the implicit sequence_number assignment out-of-line (single `call`); the single-TU
// base inlines the operator-'s first level (the operator<= branch) and the address
// temps for the reference-arg serialize/operator= sites. Same root cause as
// is_low_level_packet - not source-steerable here.
void udp_match_connection::update_acknowledgements(
	sequence_number< u16 >		remote_sequence_id,
	sequence_number< u16 >		local_sequence_id,
	const u16					local_acknowledgement_bits
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_remote_sequence_id < remote_sequence_id ) );
	const u32	remote_sequence_difference	= remote_sequence_id - m_remote_sequence_id;
	m_remote_acknowledgement_bits	= u16( remote_sequence_difference < 16 ? m_remote_acknowledgement_bits >> remote_sequence_difference : 0 );
	m_remote_acknowledgement_bits	|= 0x8000;
	m_remote_sequence_id			= remote_sequence_id;

	if ( m_local_sequence_id < local_sequence_id )
	{
		return;
	}

	ASSERT( UNKNOWN_EXPRESSION_T( local_sequence_id <= m_local_sequence_id ) );
	if ( local_sequence_id <= m_received_local_sequence_id ) {
		const u32	difference	= m_received_local_sequence_id - local_sequence_id;
		if ( difference ) {
			if ( difference <= 15 )
				m_received_local_acknowledgement_bits	|= 1 << ( 15 - difference );

			const u32	unacknowledged_packets_size	= m_unacknowledged_packets.size( );
			m_unacknowledged_packets.remove_if	( sequence_id_predicate( m_packets_allocator, local_sequence_id, m_logging_id ) );
			m_stats.unacknowledged_packets		-= unacknowledged_packets_size - m_unacknowledged_packets.size( );
		}

		return;
	}

	const u32	local_sequence_difference	= local_sequence_id - m_received_local_sequence_id;
	const u16	last_local_acknowledgement_bits	= u16( local_sequence_difference < 16 ? m_received_local_acknowledgement_bits >> local_sequence_difference : 0 );
	if ( ( last_local_acknowledgement_bits & local_acknowledgement_bits ) != last_local_acknowledgement_bits )
	{
		return;
	}

	m_stats.max_local_sequence_difference	= math::max( m_stats.max_local_sequence_difference, local_sequence_difference );
	m_received_local_acknowledgement_bits	= local_acknowledgement_bits;

	ASSERT( UNKNOWN_EXPRESSION_T( m_received_local_sequence_id < local_sequence_id ) );
	m_received_local_sequence_id	= local_sequence_id;
	ASSERT( UNKNOWN_EXPRESSION_T( m_received_local_sequence_id == local_sequence_id ) );

	u16	acknowledgement_bits	= u16( m_received_local_acknowledgement_bits ^ last_local_acknowledgement_bits );
	for ( sequence_number< u16 > sequence_id( local_sequence_id ); acknowledgement_bits; --sequence_id, acknowledgement_bits <<= 1 )
		if ( acknowledgement_bits & 0x8000 ) {
			const u32	unacknowledged_packets_size	= m_unacknowledged_packets.size( );
			m_unacknowledged_packets.remove_if	( sequence_id_predicate( m_packets_allocator, sequence_id, m_logging_id ) );
			m_stats.unacknowledged_packets		-= unacknowledged_packets_size - m_unacknowledged_packets.size( );
		}
}

void udp_match_connection::process_low_level_message( packet_reader& reader, const u32 time_in_ms )
{
	switch ( low_level_message_type_enum message_type = low_level_message_type_enum( reader.r< bool >( ) ) ) {
		case initiate_disconnection :
		default :
			if ( m_state != connected )
			{
				break;
			}

			m_state	= confirming_disconnection;
			m_disconnection_receive_time_in_ms	= time_in_ms;
			break;

		case confirm_disconnection :
			if ( m_state == connected ) {
				LOG_ERROR( "processing low level packet: skip confirming disconnection - we didn't initiated it" );
				break;
			}

			if ( m_state == initiating_disconnection ) {
				ASSERT( UNKNOWN_EXPRESSION_T( m_state == initiating_disconnection ) );

				instant_disconnect( disconnected_by_initiator );
			}
			break;

		case continuous_flow :
			break;
	}
}

} // namespace network_core
} // namespace vostok

// the disconnect purge helper of instant_disconnect/disconnect - GLOBAL scope per the
// target mangling (intrusive_list<..>::remove_if<remove_all_predicate>).
class remove_all_predicate : private boost::noncopyable {
public:
	inline			remove_all_predicate	(
						vostok::memory::single_size_buffer_allocator< 300, vostok::threading::single_threading_policy >&	packets_allocator,
						vostok::network_core::udp_match_stats&	stats,
						pcstr const		logging_id
					) :
		m_packets_allocator	( packets_allocator ),
		m_stats				( stats ),
		m_logging_id		( logging_id )
	{
	}

	inline	bool	operator( )				( vostok::network_core::udp_match_packet* packet ) const
	{
		if ( packet->is_reliable )
			--m_stats.unacknowledged_packets;

		delete_udp_match_packet( m_packets_allocator, packet );
		return true;
	}

private:
	/* 0x0000 */	vostok::memory::single_size_buffer_allocator< 300, vostok::threading::single_threading_policy >&	m_packets_allocator;
	/* 0x0004 */	vostok::network_core::udp_match_stats&	m_stats;
	/* 0x0008 */	pcstr const		m_logging_id;
}; // class remove_all_predicate

STATIC_SIZE_ASSERT(remove_all_predicate, 0xC);

namespace vostok {
namespace network_core {

void udp_match_connection::instant_disconnect( disconnect_event_types_enum type )
{
	m_state								= disconnected;

	m_last_send_time_in_ms				= 0;
	m_last_send_attempt_time_in_ms		= 0;
	m_last_receive_time_in_ms			= 0;
	m_disconnection_receive_time_in_ms	= 0;
	m_remote_acknowledgement_bits		= 0;
	m_received_local_acknowledgement_bits	= 0;
	m_pending_operations_count			= 0;
	m_local_sequence_id					= sequence_number< u16 >( 0xFFFF );
	m_remote_sequence_id				= sequence_number< u16 >( 0xFFFF );
	m_received_local_sequence_id		= sequence_number< u16 >( 0xFFFF );
	m_disconnection_local_sequence_id	= sequence_number< u16 >( 0xFFFF );

	m_unacknowledged_packets.remove_if	( remove_all_predicate( m_packets_allocator, m_stats, m_logging_id ) );
	m_packets_to_send.remove_if			( remove_all_predicate( m_packets_allocator, m_stats, m_logging_id ) );

	for ( u32 i = 0, n = m_channels.size( ) ; i < n ; ++i ) {
		channel&	channel				= m_channels[ i ];
		while ( !channel.packets.empty( ) ) {
			udp_match_packet*	packet	= &*channel.packets.begin( );
			channel.packets.erase		( *packet );
			delete_udp_match_packet		( m_packets_allocator, packet );
		}

		channel.reset					( );
	}

	if ( m_on_disconnect )
		m_on_disconnect( type );
}

void udp_match_connection::disconnect( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_state == connected ) );

	m_state							= initiating_disconnection;

	sequence_number< u16 >	test	( m_local_sequence_id );
	if ( ++test <= m_received_local_sequence_id )
	{
		instant_disconnect			( disconnected_by_initiator );
		return;
	}

	m_unacknowledged_packets.remove_if	( remove_all_predicate( m_packets_allocator, m_stats, m_logging_id ) );
	m_packets_to_send.remove_if			( remove_all_predicate( m_packets_allocator, m_stats, m_logging_id ) );

	for ( u32 i = 0, n = m_channels.size( ) ; i < n ; ++i ) {
		channel&	channel				= m_channels[ i ];
		while ( !channel.packets.empty( ) ) {
			udp_match_packet*	packet	= &*channel.packets.begin( );
			channel.packets.erase		( *packet );
			delete_udp_match_packet		( m_packets_allocator, packet );
		}
	}

	m_disconnection_local_sequence_id	= m_local_sequence_id;
	++m_disconnection_local_sequence_id;
}

u32 udp_match_connection::packets_count( ) const
{
	u32 result	= m_packets_to_send.size( ) + m_outgoing_packets.size( ) + m_unacknowledged_packets.size( );

	for ( u32 i = 0, n = m_channels.size( ) ; i < n ; ++i )
		result	+= m_channels[ i ].packets.size( );

	return result;
}


} // namespace network_core
} // namespace vostok
