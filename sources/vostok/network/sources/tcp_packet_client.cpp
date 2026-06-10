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

// STATE[95.31%|PARTIAL]: base's compiler-emitted default-ctor of m_on_error
// round-trips `this` through a frame slot (+4 frame) where the target's folded
// member-ctor COMDAT takes it direct - member-init fold shape, not source
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

// STATE[90.83%|PARTIAL]: target inlines the strip_pointer fold (no call before
// delete_helper) where our base keeps the LTCG-promoted call - per-site
// inline-vs-call, byte-identical for the `g_allocator` and `*g_allocator`
// spellings (both tested); kept the legacy client.cpp spelling
// claude@MATCH: GLOBAL-scope static - the target symbol is the unmangled
// PDB-private name `destroy_client` (no namespaces), not a mangled export
static void destroy_client( vostok::network_core::tcp_packet_client* client_to_destroy )
{
	VOSTOK_DELETE_IMPL				( vostok::network::g_allocator, client_to_destroy );
}

// STATE[97.59%|PARTIAL]: the folded function::clear member-dtor COMDATs take
// `this` in esi (target) vs ecx (base) + 8-byte frame slack - the
// receive_response-dtor LTCG-convention residual
tcp_packet_client::~tcp_packet_client( )
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &destroy_client, m_client )
		)
	);
}

// STATE[74.80%|PARTIAL]: target INLINES network_core set_on_packet_received
// (operator= copy-swap-clear) and reg-promotes the other set_on operator= folds;
// base keeps the calls - the boost::function-assign inline-vs-call wall
// (assembly_patterns.md), cascading frame/esi/edi; statements align 6/6
void tcp_packet_client::create_client( )
{
	ASSERT							( !m_client );
	m_client						= NEW ( vostok::network_core::tcp_packet_client ) ( m_world.io_service() );
	m_client->set_on_packet_received( boost::bind( &tcp_packet_client::on_packet_received, this, _1 ) );
	m_client->set_on_connected		( boost::bind( &tcp_packet_client::on_connected, this ) );
	m_client->set_on_disconnected	( boost::bind( &tcp_packet_client::on_disconnected, this ) );
	m_client->set_on_error			( boost::bind( &tcp_packet_client::on_error, this, _1, _2 ) );
}

// STATE[100%|DONE]
void tcp_packet_client::connect_impl	( pcstr const host, u16 const port )
{
	m_client->connect				( host, port );
}

// STATE[100%|DONE]
void tcp_packet_client::connect		( pcstr const host, u16 const port )
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
	);
}

// STATE[100%|DONE]
void tcp_packet_client::disconnect	( )
{
	m_world.add_order				(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &vostok::network_core::tcp_packet_client::disconnect, m_client )
		)
	);
}

// STATE[94.82%|PARTIAL]: base inlines base_packet::buffer()/buffer_size() in
// the clone expansion where target calls the folded COMDATs, and append's
// `this` is edi-promoted in target - the receive_response::execute wall
// claude@MATCH: the clone source is built inline with the ORDERS allocator (both
// the placement NEW and the tcp_packet ctor arg) - NOT via m_world.new_packet()
// (that one is the responses-side g_allocator; see on_packet_received)
void tcp_packet_client::send		( vostok::network_core::tcp_packet const& packet )
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

// STATE[100%|DONE]
void tcp_packet_client::on_packet_received_impl( vostok::network_core::packet_reader& reader )
{
	if ( m_on_packet_received )
		m_on_packet_received		( reader );
}

// STATE[83.84%|PARTIAL]: the clone buffer()/buffer_size() inline-vs-call wall
// (as in send) + the receive_response m_receiver copy lowered via
// assign_to_own in base vs the function-copy fold in target
// claude@MATCH: receive_response takes the allocator and packet as PLAIN
// references (no boost::ref/cref - the target has no addressof calls here,
// unlike send_order's cref/ref pair)
void tcp_packet_client::on_packet_received		( vostok::network_core::tcp_packet const& packet )
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

// STATE[7.25%|PARTIAL]: one-statement `m_x = arg;` - target calls the
// ICF-folded boost::function::operator= (this in edi), base INLINES the
// copy-swap-clear body; the udp_match_client operator= inline-vs-call wall
void tcp_packet_client::set_on_packet_received	( boost::function< void ( vostok::network_core::packet_reader& ) > const& on_packet_received )
{
	m_on_packet_received			= on_packet_received;
}

// STATE[75.26%|PARTIAL]: `if ( m_on_connected )` - target inlines function0's
// safe-bool (operator! fold + neg/sbb/not/and &dummy::nonnull), base calls the
// safe-bool COMDAT; per-instantiation inline-vs-call (function1/2 siblings
// match 100% from the same spelling)
void tcp_packet_client::on_connected_impl		( )
{
	if ( m_on_connected )
		m_on_connected						( );
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
void tcp_packet_client::set_on_connected	( boost::function< void ( ) > const& on_connected )
{
	m_on_connected					= on_connected;
}

// STATE[75.26%|PARTIAL]: same function0 safe-bool inline-vs-call residual as
// on_connected_impl
void tcp_packet_client::on_disconnected_impl	( )
{
	if ( m_on_disconnected )
		m_on_disconnected					( );
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
void tcp_packet_client::set_on_disconnected	( boost::function< void ( ) > const& on_disconnected )
{
	m_on_disconnected				= on_disconnected;
}

// STATE[100%|DONE]
void tcp_packet_client::on_error_impl	(
		const vostok::network_core::client_error_codes_enum	client_error_code,
		const boost::system::error_code	error_code
	)
{
	if ( m_on_error )
		m_on_error						( client_error_code, error_code );
}

// STATE[100%|DONE]
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

// STATE[8.54%|PARTIAL]: same operator= inline-vs-call wall as
// set_on_packet_received (function2 instantiation)
void tcp_packet_client::set_on_error	(
		boost::function< void ( enum vostok::network_core::client_error_codes_enum, boost::system::error_code ) > const&	on_error
	)
{
	m_on_error						= on_error;
}
