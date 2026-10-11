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
#line 27
{
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &http_client::create_client_impl, this )
		)
	);
}







// static: retail names it the internal `vostok::network::destroy_http_client`
static void destroy_http_client( network_core::http_client* client_to_destroy )
#line 36
{
	VOSTOK_DELETE_IMPL	( g_allocator, client_to_destroy );
}

http_client::~http_client( )
#line 41
{
	m_busy	= true;

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &destroy_http_client, m_client )
		)
#line 47
	);
}

void http_client::create_client_impl( )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !m_client ) );
	m_client				= NEW( network_core::http_client ) ( m_world.io_service( ) );
	m_client->set_on_error	( boost::bind( &http_client::on_error, this, _1 ) );
	m_busy					= false;
}

void http_client::get( pcstr server, pcstr path, boost::function< void ( pcstr ) > const& callback )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !m_busy ) );
	m_busy					= true;
	m_on_content_downloaded	= callback;


	// the bind converts implicitly: an explicit boost::function wrap reorders the EH state store
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator(), string_order ) (
			m_world.orders_allocator( ),
			boost::bind( &http_client::get_impl, this, _1, _2 ),
			server,
			path
		)
#line 68
	);
}
#line 75

void http_client::on_content_downloaded_impl( pcstr content )
#line 72
{
	if ( m_on_content_downloaded )
		m_on_content_downloaded	( content );

	m_busy	= false;
}




void http_client::on_content_downloaded( )
#line 80
{
	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator(), string_response ) (
			m_world.responses_allocator( ),
			boost::bind( &http_client::on_content_downloaded_impl, this, _1 ),
			m_client->result_content( ).c_str( )
		)
#line 86
	);
#line 88
}
#line 97

void http_client::get_impl( pcstr server, pcstr path )
#line 91
{
	m_client->get	( server, path, boost::bind( &http_client::on_content_downloaded, this ) );
}

void http_client::on_error_impl( boost::system::error_code const error_code )
#line 96
{
	m_busy	= false;

	if ( m_on_error )
		m_on_error	( error_code );
}

void http_client::on_error( boost::system::error_code const error_code )
#line 104
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
