////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/custom_alloc_handler.h>
#include <vostok/network_core/move_to_list_predicate.h>
#include <vostok/buffer_vector.h>

namespace vostok {
namespace network_core {

// STATE[99.51%|DONE]: full init list matches; residual is ICF-folded call-name noise + handler_allocator stub's trailing byte init (sibling unit)
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

// STATE[91.40%|PARTIAL]: auto-generated member-dtor cleanup matches (STRUCTURE MATCH, 0 stmts
// both sides, 0x50 vs 0x4e bytes); target caches `this` in esi (push esi/pop esi) vs base
// ebp-slot - /Od register-alloc residual + ICF call-name noise on the empty-stub callees.
 udp_match_connection::~udp_match_connection( )
{
}

// STATE[100%|DONE]
void udp_match_connection::on_error( client_error_codes_enum, boost::system::error_code )
{
}

// STATE[0%|PARTIAL]: shape exact (8/8); the single-TU anchor inlines packet_reader::r<> where
// the target keeps the template out-of-line (the documented anchor/inline-vs-call wall);
// report.json serializes the 0.0 fuzzy as null.
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

	// STRUCTURE DIFF: target 8 stmts / base 8 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ----------+--------+--------+----+----+------+------
	// SIZE +0x3 |0x5464d6|0x461e16|0x11|0x14|61    |	packet_reader	reader( packet );
	// BASE_ONLY |--      |0x461e2a|--  |0x10|63    |	reader.r< u16 >( );
	// BASE_ONLY |--      |0x461e3a|--  |0x10|64    |	reader.r< u16 >( );
	// BASE_ONLY |--      |0x461e4a|--  |0x18|65    |	const u16	bits	= reader.r< u16 >( );
	// TRGT_ONLY |0x5464e7|--      |0x14|--  |--    |--
	// TRGT_ONLY |0x5464fb|--      |0x14|--  |--    |--
	// TRGT_ONLY |0x54650f|--      |0xc |--  |--    |--
	// SIZE +0x9 |0x546528|0x461e6f|0x14|0x1d|70    |	reader.advance( reader.r< bool >( ) );
	// SIZE +0x19|0x54653c|0x461e8c|0x8 |0x21|71    |	return reader.eof( );
	// VERDICT: STRUCTURE MATCH (shape ok) - 8/8; the ONLY rows are the SAME three r<u16>
	// statements in the same ordinal slots (#2-#4) the aligner cannot pair because the
	// sizes flip (base inlines r<> at 0x10-0x18, target calls the kept COMDATs at 0xc-0x14
	// with LTCG custom regs); byte overlap is near zero (report 0.0, serialized as null)
	// over a verified-identical skeleton - the documented call-boundary wall, not a shape miss.
}

// STATE[88.83%|PARTIAL]: 19/19 after flipping else-if to the handle_receive house shape
// (braced arm + return, then a separate if); residual is LTCG (log function-ctor
// convention, bool-normalize temp elision, buffer_to_send_size inline-vs-call).
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

	// STRUCTURE DIFF: target 19 stmts / base 19 stmts
	// b.diff    |t.sz|b.sz|b.code
	// SIZE -0x4 |0x6c|0x68|!is_low_level_packet( base_packet( ... ) ) )
	// SIZE -0x11|0x23|0x12|*buffer = ( *reinterpret_cast< u16* >( buffer + 4 ) & 1 ) != 0;
	// SIZE -0x1 |0x15|0x14|if ( error_code ) {
	// SIZE -0x4 |0xc9|0xc5|LOG_ERROR ( "error during writing to socket: %s\r\n", ... );
	// VERDICT: STRUCTURE MATCH (shape ok) - 19/19; rows are LTCG temp-spill elision /
	// log function-ctor convention / inline-vs-call cascade, non-steerable.
}

// STATE[71.59%|PARTIAL]: 5/5; residual is the async_send_to statement (+0x1a: the
// make_custom_alloc_handler named-return result-copy our LTCG fails to elide - the
// start_receiving precedent - plus bind_t copy scheduling) and buffer_to_send_size
// inline-vs-call.
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

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// b.diff    |t.sz |b.sz |b.code
	// SIZE -0x1 |0x2f |0x2e |m_stats.sent.messages.bytes += packet->buffer_to_send_size( );
	// SIZE -0x1 |0x33 |0x32 |m_stats.sent.packets.bytes += packet->buffer_to_send_size( ) + 46;
	// SIZE +0x1a|0x104|0x11e|);
	// VERDICT: STRUCTURE MATCH (shape ok) - 5/5; sole big row is the handler/bind
	// result copies in the async_send_to statement (start_receiving precedent), LTCG.
}

// STATE[71.06%|PARTIAL]: 7/7 (now emitted - callers landed); residual is LTCG
// temp-spill elision in the inlined serialize/store statements.
void udp_match_connection::fill_packet_header( udp_match_packet& packet )
{
	pbyte	buffer	= packet.m_buffer.data( );

	const udp_match_packets_count_enum	packet_type	= udp_match_packets_count_enum( *buffer );
	ASSERT( UNKNOWN_EXPRESSION_T( packet_type < 2 ) );

	reinterpret_cast< sequence_number< u16 >& >( packet.sequence_id ).serialize( buffer );
	m_remote_sequence_id.serialize( buffer );

	*reinterpret_cast< u16* >( buffer )	= u16( ( m_remote_acknowledgement_bits << 1 ) | ( packet_type == udp_match_low_level_packet ) );
	buffer	+= 2;

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// b.diff   |t.sz|b.sz|b.code
	// SIZE -0xc|0x22|0x16|reinterpret_cast< sequence_number< u16 >& >( packet.sequence_id ).serialize( buffer );
	// SIZE -0xc|0x25|0x19|m_remote_sequence_id.serialize( buffer );
	// SIZE -0x6|0x23|0x1d|*reinterpret_cast< u16* >( buffer ) = u16( ... );
	// VERDICT: STRUCTURE MATCH (shape ok) - 7/7; rows are the target's /Od ref/cast
	// temp spills our LTCG copy-props away in the inlined serialize, non-steerable.
}

// STATE[76.47%|PARTIAL]: 42/42; the base frame outgrows disp8 (inlined buffer_size/
// header_size temps), so every this-access pays +3 - a frame cascade of the
// inline-vs-call wall; plus the seq op= lowering rows.
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
	*packet_to_send->buffer_to_send( )		= udp_match_low_level_packet;
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

	// STRUCTURE DIFF: target 42 stmts / base 42 stmts (condensed - systematic rows)
	// +0x3..+0xa on every m_stats/this access (base this-slot beyond disp8 -> +3/access);
	// SIZE +0xc |0x11|0x1d|packet_to_send->append( u8( i->buffer_size( ) ) ); (inline-vs-call)
	// BASE_ONLY/TRGT_ONLY x3: the is_reliable=0 / seq-copy / tag-byte trio mis-paired by
	// the aligner over the seq-op= -0x8 size flip - same 3 statements both sides.
	// VERDICT: STRUCTURE MATCH (shape ok) - 42/42; all rows are the frame-cascade of the
	// buffer_size/header_size/append inline-vs-call wall + seq op= lowering, non-steerable.
}

// STATE[100%|DONE]: paired after the QBDI top-level-const declaration fix.
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

// STATE[70.07%|PARTIAL]: 18/18; residual is the append(u8) forward dissolved by base
// LTCG into append(pcvoid,u32)+temp where the target calls the folded u8 COMDAT
// directly (+0x8/+0x7), plus reg-form noise. "initiatie disconnection" reproduces the
// target string typo.
udp_match_packet* udp_match_connection::new_low_level_packet( const u8 message_type )
{
	udp_match_packet* const	packet	= new_udp_match_packet( m_packets_allocator );
	*packet->buffer_to_send( )		= udp_match_low_level_packet;

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

	// STRUCTURE DIFF: target 18 stmts / base 18 stmts
	// b.diff   |t.sz|b.sz|b.code
	// SIZE +0x8|0xa |0x12|packet->append					( u8( 1 ) );
	// SIZE +0x7|0xd |0x14|packet->append					( message_type );
	// SIZE -0x1|0x2d|0x2c|m_stats.sent_low_level.packets.bytes += ... + 46;
	// SIZE -0x1|0x29|0x28|m_stats.sent_low_level.messages.bytes += ...;
	// VERDICT: STRUCTURE MATCH (shape ok) - 18/18; append rows are base LTCG dissolving
	// the append(u8) forward into append(pcvoid,u32) + value temp (target calls the
	// folded u8 COMDAT: push imm/movzx + call), the documented COMDAT-forward wall.
}

// STATE[89.69%|PARTIAL]: 71/71 (switch jump-table with NODEFAULT, no bounds check,
// matches); residual is seq op= lowering + buffer_size/allocated_size inline-vs-call
// + a short-vs-near backedge cascade.
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

	// STRUCTURE DIFF: target 71 stmts / base 71 stmts
	// b.diff    |t.sz|b.sz|b.code
	// SIZE -0xe |0x22|0x14|reinterpret_cast<...>( packet->sequence_id ) = m_local_sequence_id;
	// SIZE -0x3 |0x5 |0x2 |} (while backedge near-vs-short)
	// SIZE -0x1 |0x2c|0x2b|if ( ++test <= m_received_local_sequence_id ) {
	// SIZE +0x1 |0x14|0x15|if ( ( *i )->is_reliable )
	// SIZE -0xd |0x45|0x38|reinterpret_cast<...>( packet->sequence_id ) = ++m_local_sequence_id;
	// SIZE +0x19|0x22|0x3b|u32 size_left = packet->allocated_size( ) - packet->buffer_size( ) - 1;
	// SIZE +0xa |0x5a|0x64|if ( size_left > ( *i )->buffer_size( ) && !*( *i )->buffer_to_send( ) ) {
	// SIZE +0xa |0x30|0x3a|size_left -= ( *i )->buffer_size( ) + 1;
	// SIZE -0xe |0x39|0x2b|reinterpret_cast<...>( ( *i )->sequence_id ) = reinterpret_cast<...>( packet->sequence_id );
	// SIZE -0x13|0x70|0x5d|packets.erase( std::remove_if( ... ) ... );
	// VERDICT: STRUCTURE MATCH (shape ok) - 71/71; all rows are seq op= lowering and
	// buffer_size/allocated_size/header_size inline-vs-call (+ its jump-width cascade),
	// non-steerable.
}

// STATE[100%|DONE]
void udp_match_connection::connect( udp_match_packet* const packet )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_state == disconnected ) );

	m_state	= connected;

	if ( packet )
		enqueue_impl( packet );
}

// STATE[79.94%|PARTIAL]: is_ordered binds the channel's running sent_order_id by reference,
// stamps the packet's order_id from it, serializes it into the wire buffer at +1 through a
// raw cursor, then pre-increments it; then the is_reliable / push_back tail.
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

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ---------+--------+--------+----+----+------+------
	// BASE_ONLY|--      |0x461a18|--  |0xd |519   |		packet->order_id	= sent_order_id;
	// BASE_ONLY|--      |0x461a25|--  |0x11|520   |		pbyte	stream		= packet->buffer( ) + 1;
	// TRGT_ONLY|0x545cad|--      |0x11|--  |--    |--
	// TRGT_ONLY|0x545cbe|--      |0x21|--  |--    |--
	// SIZE +0x1|0x545cef|0x461a5b|0x12|0x13|525   |	if ( packet->is_reliable )
	// VERDICT: STRUCTURE MATCH (shape ok) - 9/9 after reconstructing the real body (the target locals expose sequence_number<u16>& sent_order_id + u8* stream); the ONLY rows are the SAME order_id-stamp / stream-init statements drift-mis-paired (target's inlined sequence_number op= materializes a this-temp, 0x15 vs our 0xd); residual is that op= lowering, non-steerable from this caller.
}

// STATE[99.80%|PARTIAL]: connected packets go to enqueue_impl; otherwise an assert eater and
// delete_udp_match_packet. The old inline-vs-call residual closed when the packet leaves
// landed - sizes now equal (0x4b/0x4b).
void udp_match_connection::enqueue( udp_match_packet* packet )
{
	if ( m_state == connected )
		enqueue_impl( packet );
	else
	{
		ASSERT( UNKNOWN_EXPRESSION_T( m_state != connected ) );
		delete_udp_match_packet( m_packets_allocator, packet );
	}

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts - STRUCTURE MATCH, no diverging rows
	// (0x4b bytes both sides).
	// VERDICT: STRUCTURE MATCH - 5/5 aligned, equal sizes; the 0.2% byte residual is ICF
	// call-name noise on the folded delete/assert callees.
}

} // namespace network_core
} // namespace vostok

// the acknowledgement purge helper of update_acknowledgements - GLOBAL scope per the
// target mangling (intrusive_list<..>::remove_if<sequence_id_predicate>).
class sequence_id_predicate : public boost::noncopyable {
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

// STATE[59.47%|PARTIAL]: 33/33; the % floor is the three operator-<u16> statements the
// base inlines (0x51 each) where the target calls the kept COMDAT (0x1c) - the
// is_low_level_packet r<u16> wall - plus math::max the OTHER direction (base calls,
// target inlines branchless - the udp_match_client ctor precedent) and predicate-ctor
// fold scheduling.
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

// STATE[86.24%|PARTIAL]: 12/12; default shares the initiate_disconnection arm (the
// dispatch falls through into it). Residual: the target dispatch emits a redundant
// cmp-0/je for case 0 that our compiler dedups against the adjacent default (label-order
// flip changes nothing), on top of the r<bool> COMDAT call-form wall.
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

	// STRUCTURE DIFF: target 12 stmts / base 12 stmts
	// SIZE +0x3|0  | switch ( low_level_message_type_enum message_type = low_level_message_type_enum( reader.r< bool >( ) ) ) {
	// SIZE -0x2|+14| LOG_ERROR( "processing low level packet: skip confirming disconnection - we didn't initiated it" );
	// VERDICT: STRUCTURE MATCH (shape ok) - 12/12; the switch row is the r<bool> COMDAT
	// call-form wall + the target's redundant case-0 compare our compiler dedups against
	// the adjacent default; the LOG row is __LINE__/lowering byte noise. Non-steerable.
}

} // namespace network_core
} // namespace vostok

// the disconnect purge helper of instant_disconnect/disconnect - GLOBAL scope per the
// target mangling (intrusive_list<..>::remove_if<remove_all_predicate>).
class remove_all_predicate : public boost::noncopyable {
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

// STATE[73.23%|PARTIAL]: 25/25; the seq-reset rows are the op= this-temp lowering, the
// channel-purge rows the intrusive rbtree begin/iterator inline-vs-call wall.
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

	// STRUCTURE DIFF: target 25 stmts / base 25 stmts
	// SIZE -0x8 |+9 | m_local_sequence_id					= sequence_number< u16 >( 0xFFFF );
	// SIZE -0x7 |+10| m_remote_sequence_id				= sequence_number< u16 >( 0xFFFF );
	// SIZE -0x8 |+11| m_received_local_sequence_id		= sequence_number< u16 >( 0xFFFF );
	// SIZE -0x8 |+12| m_disconnection_local_sequence_id	= sequence_number< u16 >( 0xFFFF );
	// SIZE +0x4 |+19| while ( !channel.packets.empty( ) ) {
	// SIZE +0x1d|+20| udp_match_packet*	packet	= &*channel.packets.begin( );
	// SIZE -0x1 |+28| if ( m_on_disconnect )
	// VERDICT: STRUCTURE MATCH (shape ok) - 25/25; the four seq-reset rows are the op=
	// this-temp lowering (target spills &member to a slot, our LTCG folds it into the
	// store), the while/packet rows are the intrusive rbtree begin/iterator-ctor/
	// operator-> kept out-of-line by target vs inlined by base; non-steerable.
}

// STATE[79.07%|PARTIAL]: 18/18; channel-purge rows are the intrusive begin inline-vs-call
// wall (+ jump-width cascade), the m_disconnection_local_sequence_id rows the op= lowering.
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

	// STRUCTURE DIFF: target 18 stmts / base 18 stmts
	// SIZE +0x4 |+16| while ( !channel.packets.empty( ) ) {
	// SIZE +0x23|+17| udp_match_packet*	packet	= &*channel.packets.begin( );
	// SIZE +0x3 |+20| }
	// SIZE -0xe |+23| m_disconnection_local_sequence_id	= m_local_sequence_id;
	// SIZE -0x1 |+24| ++m_disconnection_local_sequence_id;
	// VERDICT: STRUCTURE MATCH (shape ok) - 18/18; while/packet/} rows are the intrusive
	// rbtree begin/iterator inline-vs-call wall + its jump-width cascade; the two seq rows
	// are the op= this-temp lowering; non-steerable.
}

// STATE[99.79%|PARTIAL]: control structure + ops match; the residual is the intrusive set
// size() temp + ICF call-name noise.
u32 udp_match_connection::packets_count( ) const
{
	u32 result	= m_packets_to_send.size( ) + m_outgoing_packets.size( ) + m_unacknowledged_packets.size( );

	for ( u32 i = 0, n = m_channels.size( ) ; i < n ; ++i )
		result	+= m_channels[ i ].packets.size( );

	return result;

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts - STRUCTURE MATCH, no diverging rows
	// (0xae bytes both sides).
	// VERDICT: STRUCTURE MATCH - 4/4 aligned, equal sizes; 99.79% residual is the intrusive
	// set size() temp + ICF call-name noise.
}


} // namespace network_core
} // namespace vostok
