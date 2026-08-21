////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network/match_client.h>
#include "match_client_impl.h"
#include "network_world.h"
#include "connect_order.h"
#include "enqueue_order.h"
#include "send_queued_order.h"
#include "receive_udp_response.h"
#include "functor_order.h"
#include "functor_response.h"
#include "udp_match_fixed_packets_allocator.h"
#include <vostok/network_core/packet_reader.h>

using vostok::network::match_client;
using vostok::network::match_client_impl;

// Global scope and private PDB name are part of the target type identity.
class client_destroyer : public vostok::network::order {
public:
	// Target emits this constructor only inline in match_client::~match_client.
	inline			client_destroyer	(
						vostok::network_core::udp_match_packets_allocator_ptr const&	responses_allocator,
						vostok::memory::base_allocator&			orders_allocator,
						vostok::network::match_client_impl**	client
					) :
		m_responses_allocator	( responses_allocator ),
		m_orders_allocator		( orders_allocator ),
		m_client				( client )
	{
	}

	virtual			~client_destroyer	( );

	virtual	void	execute				( );

private:
	vostok::network_core::udp_match_packets_allocator_ptr	m_responses_allocator;
	vostok::memory::base_allocator&			m_orders_allocator;
	vostok::network::match_client_impl**	m_client;
}; // class client_destroyer

STATIC_SIZE_ASSERT(client_destroyer, 0x14);

void match_client::create_client( vostok::network_core::udp_network_flow_emulator_options const* options )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !*m_client ) );
	match_client_impl* const temp	= VOSTOK_NEW_IMPL( m_world.responses_allocator( ), match_client_impl ) (
		m_world.io_service( ), m_packets_orderer, options );
	vostok::threading::interlocked_exchange_pointer( (pvoid&)*m_client, temp );
	( *m_client )->set_on_packet_received	( boost::bind( &match_client::on_packet_received, this, _1, _2 ) );
	( *m_client )->set_on_disconnect		( boost::bind( &match_client::on_disconnect, this, _1 ) );
}

void match_client::create_responses_packets_allocator( )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !m_response_packets_allocator ) );

	m_response_packets_allocator	= VOSTOK_NEW_IMPL(
		m_world.responses_allocator( ),
		udp_match_fixed_packets_allocator< 8192 >
	) (
		m_world.responses_allocator( )
	);
}

 match_client::match_client(
	vostok::network::world&		world,
	vostok::network_core::udp_match_packets_orderer&	packets_orderer,
	vostok::network_core::udp_network_flow_emulator_options const*	options
) :
	m_order_packets_allocator	(
		VOSTOK_NEW_IMPL( static_cast_checked<network_world&>(world).orders_allocator( ), udp_match_fixed_packets_allocator< 8192 > ) (
			static_cast_checked<network_world&>(world).orders_allocator( )
		)
	),
	m_packets_orderer	( packets_orderer ),
	m_world				( static_cast_checked<network_world&>(world) ),
	m_client			( VOSTOK_NEW_IMPL( static_cast_checked<network_world&>(world).orders_allocator( ), match_client_impl* ) )
{
	*m_client			= 0;
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &match_client::create_responses_packets_allocator, this )
		)
	);
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &match_client::create_client, this, options )
		)
	);
}

client_destroyer::~client_destroyer( )
{
	VOSTOK_DELETE_IMPL		( m_orders_allocator, m_client );
}

void client_destroyer::execute( )
{
	VOSTOK_DELETE_IMPL		( *vostok::network::g_allocator, *m_client );
	m_responses_allocator	= 0;
}

 match_client::~match_client( )
{
	order* const order		= VOSTOK_NEW_IMPL( m_world.orders_allocator( ), client_destroyer ) (
		m_response_packets_allocator,
		m_world.orders_allocator( ),
		m_client
	);
	m_response_packets_allocator	= 0;
	m_world.add_order		( order );
}

void match_client::on_connected(
	vostok::connection_error_types_enum			connection_error,
	vostok::handshaking_error_types_enum		handshaking_error,
	vostok::socket_error_types_enum				socket_error,
	vostok::lobby_server_message_types_enum		lobby_error
)
{
	if ( m_on_connected )
		m_world.add_response	(
			VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
				boost::bind( m_on_connected, connection_error, handshaking_error, socket_error, lobby_error )
			)
		);

}

void match_client::connect(
	pcstr const		host,
	const u16		port,
	const u32		current_time_in_ms,
	vostok::network_core::udp_match_packet* const	packet,
	boost::function< void( enum vostok::connection_error_types_enum, enum vostok::handshaking_error_types_enum, enum vostok::socket_error_types_enum, enum vostok::lobby_server_message_types_enum ) > const&	callback
)
{
	m_on_connected			= callback;
	boost::function< void( enum vostok::connection_error_types_enum, enum vostok::handshaking_error_types_enum, enum vostok::socket_error_types_enum, enum vostok::lobby_server_message_types_enum ) > const&	on_connected	= boost::bind( &match_client::on_connected, this, _1, _2, _3, _4 );

	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), connect_order ) (
			m_world.orders_allocator( ),
			host,
			m_order_packets_allocator,
			packet,
			boost::bind(
				&match_client_impl::connect,
				boost::ref( *m_client ),
				_1,
				port,
				current_time_in_ms,
				_2,
				on_connected )
		)
	);
}

void match_client::disconnect( )
{
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &match_client_impl::disconnect, boost::ref( *m_client ) )
		)
	);
}

// Global scope preserves the target's PDB-private `enqueue_impl` name.
static void enqueue_impl(
		match_client_impl** const					client,
		vostok::network_core::udp_match_packet&		packet
	)
{
	( *client )->enqueue	( ( *client )->clone_packet( packet ) );
}

void match_client::enqueue( vostok::network_core::udp_match_packet* packet )
{
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), enqueue_order ) (
			boost::bind( &enqueue_impl, m_client, _1 ),
			boost::ref( *packet ),
			m_order_packets_allocator,
			( *m_client )->get_stats( ),
			m_stats
		)
	);
}

void match_client::send_queued_packets( const u32 current_time_in_ms )
{
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), send_queued_order ) (
			boost::bind( &match_client_impl::send_queued_packets, boost::ref( *m_client ), current_time_in_ms ),
			boost::ref( *m_client ),
			m_stats
		)
	);
}

void match_client::on_packet_received_impl( const u8 message_type, vostok::network_core::packet_reader& reader )
{
	if ( m_on_packet_received )
		m_on_packet_received( message_type, reader );
}

void match_client::on_packet_received( const u8 message_type, vostok::network_core::packet_reader& reader )
{
	if ( m_on_packet_received ) {
		network_core::udp_match_packet* const packet	= network_core::new_udp_match_packet( *m_response_packets_allocator );
		packet->append		( reader.pointer( ), reader.size_to_eof( ) );
		m_world.add_response	(
			VOSTOK_NEW_IMPL( m_world.responses_allocator( ), receive_udp_response ) (
				boost::bind( &match_client::on_packet_received_impl, this, message_type, _1 ),
				m_response_packets_allocator,
				boost::ref( *packet ),
				( *m_client )->get_stats( ),
				m_stats
			)
		);
	}
}

void match_client::on_disconnect_impl( vostok::network_core::disconnect_event_types_enum type )
{
	if ( m_on_disconnected )
		m_on_disconnected	( type );
}

void match_client::on_disconnect( vostok::network_core::disconnect_event_types_enum type )
{
	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( &match_client::on_disconnect_impl, this, type )
		)
	);
}

vostok::network_core::udp_match_packet* match_client::new_packet( const u8 message_type )
{
	network_core::udp_match_packet* const result	= network_core::new_udp_match_packet( *m_order_packets_allocator );

	match_client_impl::construct_packet	( m_packets_orderer, *result, message_type );
	return					result;
}

bool match_client::is_connected( ) const
{
	return					*m_client && ( *m_client )->is_connected( );
}

bool match_client::is_disconnected( ) const
{
	return					!*m_client || ( *m_client )->is_disconnected( );
}

u32 match_client::last_receive_time_in_ms( ) const
{
	ASSERT					( UNKNOWN_EXPRESSION_T( *m_client ) );
	return					( *m_client )->last_receive_time_in_ms( );
}
