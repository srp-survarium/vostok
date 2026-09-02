// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/network/http_client.h>
#include "network_world.h"
#include <vostok/network_core/http_client.h>
#include "functor_order.h"
#include "functor_response.h"
#include "string_order.h"
#include "string_response.h"

namespace vostok {
namespace network {

http_client::http_client( world& world ) :
	m_world		( static_cast_checked<network_world&>(world) ),
	m_client	( 0 ),
	m_busy		( true )
{
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &http_client::create_client_impl, this )
		)
	);
}

// claude@NOTE: STRUCTURE MATCH (1 stmt). Wall: target inlines strip_pointer
// (passes the g_allocator pointer value direct to delete_helper) where our base
// keeps a strip_pointer call before delete_helper - per-site inline-vs-call,
// byte-identical for either allocator spelling (sibling destroy_client documents
// the same wall). Not steerable from this TU.
// claude@MATCH: static - the target symbol is the unmangled PDB-private name
// `vostok::network::destroy_http_client` (internal linkage), not a mangled export
static void destroy_http_client( network_core::http_client* client_to_destroy )
{
	VOSTOK_DELETE_IMPL	( g_allocator, client_to_destroy );
}

http_client::~http_client( )
{
	m_busy	= true;

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &destroy_http_client, m_client )
		)
	);
}

void http_client::create_client_impl( )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !m_client ) );
	m_client				= NEW( network_core::http_client ) ( m_world.io_service( ) );
	m_client->set_on_error	( boost::bind( &http_client::on_error, this, _1 ) );
	m_busy					= false;
}

void http_client::get( pcstr const server, pcstr const path, boost::function< void ( pcstr ) > const& callback )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !m_busy ) );
	m_busy					= true;
	m_on_content_downloaded	= callback;

	// claude@MATCH: bind passed directly (arity picks the 2-string string_order
	// ctor); an explicit function wrap mis-schedules the EH guard `or`
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), string_order ) (
			m_world.orders_allocator( ),
			boost::bind( &http_client::get_impl, this, _1, _2 ),
			server,
			path
		)
	);
}

void http_client::on_content_downloaded_impl( pcstr const content )
{
	if ( m_on_content_downloaded )
		m_on_content_downloaded	( content );

	m_busy	= false;
}

// claude@NOTE: STRUCTURE MATCH (1 stmt). Wall: one dead frame dword (target 0x70
// vs base 0x6C) plus the home-slot rename riding on it - the string_order-ctor
// ghost-dword LTCG class. Not steerable from this TU.
void http_client::on_content_downloaded( )
{
	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), string_response ) (
			m_world.responses_allocator( ),
			boost::bind( &http_client::on_content_downloaded_impl, this, _1 ),
			m_client->result_content( ).c_str( )
		)
	);
}

void http_client::get_impl( pcstr const server, pcstr const path )
{
	m_client->get	( server, path, boost::bind( &http_client::on_content_downloaded, this ) );
}

void http_client::on_error_impl( boost::system::error_code const error_code )
{
	m_busy	= false;

	if ( m_on_error )
		m_on_error	( error_code );
}

void http_client::on_error( boost::system::error_code const error_code )
{
	if ( !m_on_error )
		return;

	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &http_client::on_error_impl, this, error_code )
		)
	);
}

} // namespace network
} // namespace vostok
