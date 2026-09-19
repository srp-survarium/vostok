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

// Callback default construction still differs in its temporary spills.
tcp_packet_client::tcp_packet_client( vostok::network::world& world ) :
	m_world							( static_cast_checked<network_world&>(world) ),
	m_client						( 0 )
#line 26
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &tcp_packet_client::create_client, this )
		)
	);
}

// Retail forwards the allocator directly; this build retains strip_pointer.
// claude@MATCH: GLOBAL-scope static - the target symbol is the unmangled
// PDB-private name `destroy_client` (no namespaces), not a mangled export
static void destroy_client( vostok::network_core::tcp_packet_client* client_to_destroy )
#line 35
{
	VOSTOK_DELETE_IMPL				( vostok::network::g_allocator, client_to_destroy );
}

tcp_packet_client::~tcp_packet_client( )
#line 40
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &destroy_client, m_client )
		)
	);
}

void tcp_packet_client::create_client( )
#line 49
{
	ASSERT							( !m_client );
	m_client						= NEW ( vostok::network_core::tcp_packet_client ) ( m_world.io_service() );
	m_client->set_on_packet_received( boost::bind( &tcp_packet_client::on_packet_received, this, _1 ) );
	m_client->set_on_connected		( boost::bind( &tcp_packet_client::on_connected, this ) );
	m_client->set_on_disconnected	( boost::bind( &tcp_packet_client::on_disconnected, this ) );
	m_client->set_on_error			( boost::bind( &tcp_packet_client::on_error, this, _1, _2 ) );
}

void tcp_packet_client::connect_impl	( pcstr const host, u16 const port )
#line 59
{
	m_client->connect				( host, port );
}

void tcp_packet_client::connect		( pcstr const host, u16 const port )
#line 64
{
	// claude@MATCH: the bind converts implicitly (ctor arity disambiguates the
	// string_order overloads); an explicit boost::function<...>( ... ) wrap makes
	// the base schedule the temp's EH guard `or` early (before the bind) where
	// the target sets it after assign_to
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), string_order ) (
			m_world.orders_allocator( ),
			boost::bind( &tcp_packet_client::connect_impl, this, _1, port ),
			host
		)
#line 71
	);
}
#line 91

void tcp_packet_client::disconnect	( )
#line 75
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &vostok::network_core::tcp_packet_client::disconnect, m_client )
		)
	);
}

// claude@MATCH: the clone source is built inline with the ORDERS allocator (both
// the placement NEW and the tcp_packet ctor arg) - NOT via m_world.new_packet()
// (that one is the responses-side g_allocator; see on_packet_received)
void tcp_packet_client::send		( vostok::network_core::tcp_packet const& packet )
#line 84
{
	vostok::network_core::tcp_packet* cloned_packet	= VOSTOK_NEW_IMPL( m_world.orders_allocator(), vostok::network_core::tcp_packet ) ( m_world.orders_allocator( ) );
	cloned_packet->clone			( packet );
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), send_order ) (
			boost::bind( &vostok::network_core::tcp_packet_client::send, m_client, _1 ),
			boost::cref( *cloned_packet ),
			boost::ref( m_world.orders_allocator() )
		)
	);
}

void tcp_packet_client::on_packet_received_impl( vostok::network_core::packet_reader& reader )
#line 97
{
	if ( m_on_packet_received )
		m_on_packet_received		( reader );
}

// claude@MATCH: receive_response takes the allocator and packet as PLAIN
// references (no boost::ref/cref - the target has no addressof calls here,
// unlike send_order's cref/ref pair)
void tcp_packet_client::on_packet_received		( vostok::network_core::tcp_packet const& packet )
#line 103
{
	if ( !m_on_packet_received )
		return;

	vostok::network_core::tcp_packet* cloned_packet	= m_world.new_packet( );
	cloned_packet->clone					( packet );
	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), receive_response ) (
			boost::bind( &tcp_packet_client::on_packet_received_impl, this, _1 ),
			m_world.responses_allocator( ),
			*cloned_packet
		)
	);
}

void tcp_packet_client::set_on_packet_received	( boost::function< void ( vostok::network_core::packet_reader& ) > const& on_packet_received )
#line 119
{
	m_on_packet_received			= on_packet_received;
}

void tcp_packet_client::on_connected_impl		( )
#line 124
{
	if ( m_on_connected )
		m_on_connected						( );
}

void tcp_packet_client::on_connected			( )
#line 130
{
	if ( !m_on_connected )
		return;

	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &tcp_packet_client::on_connected_impl, this )
		)
	);
}

void tcp_packet_client::set_on_connected	( boost::function< void ( ) > const& on_connected )
#line 142
{
	m_on_connected					= on_connected;
}

void tcp_packet_client::on_disconnected_impl	( )
#line 147
{
	if ( m_on_disconnected )
		m_on_disconnected					( );
}

void tcp_packet_client::on_disconnected		( )
#line 153
{
	if ( !m_on_disconnected )
		return;

	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &tcp_packet_client::on_disconnected_impl, this )
		)
	);
}

void tcp_packet_client::set_on_disconnected	( boost::function< void ( ) > const& on_disconnected )
#line 165
{
	m_on_disconnected				= on_disconnected;
}

void tcp_packet_client::on_error_impl	(
		const vostok::network_core::client_error_codes_enum	client_error_code,
		const boost::system::error_code	error_code
	)
#line 170
{
	if ( m_on_error )
		m_on_error						( client_error_code, error_code );
}

void tcp_packet_client::on_error		(
		const vostok::network_core::client_error_codes_enum	client_error_code,
		const boost::system::error_code	error_code
	)
#line 176
{
	if ( !m_on_error )
		return;

	m_world.add_response					(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &tcp_packet_client::on_error_impl, this, client_error_code, error_code )
		)
	);
}

// Retail retains the callback assignment call; this build expands copy/swap/clear.
void tcp_packet_client::set_on_error	(
		boost::function< void ( enum vostok::network_core::client_error_codes_enum, boost::system::error_code ) > const&	on_error
	)
#line 188
{
	m_on_error						= on_error;
}
