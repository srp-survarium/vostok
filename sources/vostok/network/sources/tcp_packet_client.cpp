////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network/tcp_packet_client.h>
#include "network_world.h"
#include <vostok/network_core/tcp_packet_client.h>
#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>
#include "send_order.h"
#include "string_order.h"
#include "receive_response.h"
#include "functor_response.h"
#include "functor_order.h"

using vostok::network::tcp_packet_client;

// STATE[PARTIAL]: legacy client ctor ported (deferred create order); unverified
// FUNCTION BODY[0x75c320]
tcp_packet_client::tcp_packet_client( vostok::network::world& world ) :
	m_world							( static_cast_checked<network_world&>(world) ),
	m_client						( 0 )
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &tcp_packet_client::create_client, this )
		)
	);
}

namespace vostok {
namespace network {

// STATE[PARTIAL]: the legacy client.cpp destroy pattern; unverified vs target
// FUNCTION BODY[0x75b6d0]
void destroy_client( vostok::network_core::tcp_packet_client* client_to_destroy )
{
	VOSTOK_DELETE_IMPL				( g_allocator, client_to_destroy );
}

} // namespace network
} // namespace vostok

// STATE[PARTIAL]: legacy body ported onto the free destroy_client; unverified
// FUNCTION BODY[0x75b6f0]
tcp_packet_client::~tcp_packet_client( )
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &vostok::network::destroy_client, m_client )
		)
	);
}

// STATE[PARTIAL]: legacy body ported onto network_core::tcp_packet_client (the
// carcass shape matches: assert, NEW, four set_on binds); unverified
// FUNCTION BODY[0x75c050]
void tcp_packet_client::create_client( )
{
	ASSERT							( !m_client );
	m_client						= NEW ( vostok::network_core::tcp_packet_client ) ( m_world.io_service() );
	m_client->set_on_packet_received( boost::bind( &tcp_packet_client::on_packet_received, this, _1 ) );
	m_client->set_on_connected		( boost::bind( &tcp_packet_client::on_connected, this ) );
	m_client->set_on_disconnected	( boost::bind( &tcp_packet_client::on_disconnected, this ) );
	m_client->set_on_error			( boost::bind( &tcp_packet_client::on_error, this, _1, _2 ) );
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75c480]
void tcp_packet_client::connect_impl	( pcstr const host, u16 const port )
{
	m_client->connect				( host, port );
}

// STATE[PARTIAL]: the legacy connect_order became the generic string_order (the
// port is bound, the host string is duplicated by the order); unverified
// FUNCTION BODY[0x75c4b0]
void tcp_packet_client::connect		( pcstr const host, u16 const port )
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), string_order ) (
			m_world.orders_allocator( ),
			boost::function< void ( pcstr ) >( boost::bind( &tcp_packet_client::connect_impl, this, _1, port ) ),
			host
		)
	);
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75b580]
void tcp_packet_client::disconnect	( )
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &vostok::network_core::tcp_packet_client::disconnect, m_client )
		)
	);
}

// STATE[PARTIAL]: legacy send took ownership of a pointer; the target clones the
// packet (carcass LOCALS cloned_packet) before deferring; unverified
// FUNCTION BODY[0x75b830]
void tcp_packet_client::send		( vostok::network_core::tcp_packet const& packet )
{
	vostok::network_core::tcp_packet* cloned_packet	= m_world.new_packet( );
	cloned_packet->clone			( packet );
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), send_order ) (
			boost::bind( &vostok::network_core::tcp_packet_client::send, m_client, _1 ),
			boost::cref( *cloned_packet ),
			boost::ref( m_world.orders_allocator() )
		)
	);
}

// STATE[PARTIAL]: the reader now arrives as a parameter (legacy built it here);
// unverified
// FUNCTION BODY[0x75b540]
void tcp_packet_client::on_packet_received_impl( vostok::network_core::packet_reader& reader )
{
	if ( m_on_packet_received )
		m_on_packet_received		( reader );
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75bef0]
void tcp_packet_client::on_packet_received		( vostok::network_core::tcp_packet const& packet )
{
	if ( !m_on_packet_received )
		return;

	vostok::network_core::tcp_packet* cloned_packet	= m_world.new_packet( );
	cloned_packet->clone					( packet );
	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), receive_response ) (
			boost::bind( &tcp_packet_client::on_packet_received_impl, this, _1 ),
			boost::ref( m_world.responses_allocator() ),
			boost::cref( *cloned_packet )
		)
	);
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75b450]
void tcp_packet_client::set_on_packet_received	( boost::function< void ( vostok::network_core::packet_reader& ) > const& on_packet_received )
{
	m_on_packet_received			= on_packet_received;
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75b500]
void tcp_packet_client::on_connected_impl		( )
{
	if ( m_on_connected )
		m_on_connected						( );
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75bdc0]
void tcp_packet_client::on_connected			( )
{
	if ( !m_on_connected )
		return;

	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &tcp_packet_client::on_connected_impl, this )
		)
	);
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75b430]
void tcp_packet_client::set_on_connected	( boost::function< void ( ) > const& on_connected )
{
	m_on_connected					= on_connected;
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75b4c0]
void tcp_packet_client::on_disconnected_impl	( )
{
	if ( m_on_disconnected )
		m_on_disconnected					( );
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75bc90]
void tcp_packet_client::on_disconnected		( )
{
	if ( !m_on_disconnected )
		return;

	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &tcp_packet_client::on_disconnected_impl, this )
		)
	);
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75b410]
void tcp_packet_client::set_on_disconnected	( boost::function< void ( ) > const& on_disconnected )
{
	m_on_disconnected				= on_disconnected;
}

// STATE[PARTIAL]: the on_X_impl pattern applied to the new error channel; unverified
// FUNCTION BODY[0x75b470]
void tcp_packet_client::on_error_impl	(
		const vostok::network_core::client_error_codes_enum	client_error_code,
		const boost::system::error_code	error_code
	)
{
	if ( m_on_error )
		m_on_error						( client_error_code, error_code );
}

// STATE[PARTIAL]: the on_X pattern applied to the new error channel; unverified
// FUNCTION BODY[0x75baf0]
void tcp_packet_client::on_error		(
		const vostok::network_core::client_error_codes_enum	client_error_code,
		const boost::system::error_code	error_code
	)
{
	if ( !m_on_error )
		return;

	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &tcp_packet_client::on_error_impl, this, client_error_code, error_code )
		)
	);
}

// STATE[PARTIAL]: legacy body ported; unverified
// FUNCTION BODY[0x75b3f0]
void tcp_packet_client::set_on_error	(
		boost::function< void ( enum vostok::network_core::client_error_codes_enum, boost::system::error_code ) > const&	on_error
	)
{
	m_on_error						= on_error;
}
