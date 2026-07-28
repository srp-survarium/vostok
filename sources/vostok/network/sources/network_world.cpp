////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_world.h"
#include <vostok/network_core/tcp_packet.h>
#include "functor_response.h"
#include "functor_order.h"

using vostok::network::network_world;

static void empty_function ( ) { }

network_world::network_world					( vostok::network::engine& engine, vostok::memory::base_allocator& orders_allocator ) :
	m_io_service						( NEW( boost::asio::io_service ) ( ) ),
	m_channel							( *vostok::network::g_allocator, orders_allocator ),
	m_engine							( engine )
{
	m_channel.orders.user_initialize	( );
	m_channel.responses.owner_initialize( NEW(functor_response)( &empty_function ), NEW(functor_response)( &empty_function ) );
}

// claude@NOTE: the target never deletes m_io_service - a faithful leak, do not "fix"
network_world::~network_world					( )
{
	m_channel.responses.owner_finalize	( );
}

void network_world::initialize					( )
{
	m_channel.orders.owner_initialize	( VOSTOK_NEW_IMPL(orders_allocator(), functor_order)( &empty_function ), VOSTOK_NEW_IMPL(orders_allocator(), functor_order)( &empty_function ) );
	m_channel.responses.user_initialize	( );
}

void network_world::finalize					( )
{
	m_channel.orders.owner_finalize		( );
}

void network_world::tick						( bool single_threaded )
{
	process_orders						( );

	if ( single_threaded )
		m_io_service->run_one			( );
	else
		m_io_service->poll				( );
}

void network_world::add_order					( vostok::network::order* order )
{
	m_channel.orders.owner_push_back	( order );
}

void network_world::add_response				( vostok::network::response* response )
{
	m_channel.responses.owner_push_back	( response );
}

void network_world::process_orders				( )
{
	m_channel.responses.owner_delete_processed_items	( );
	while ( order* const order = m_channel.orders.user_pop_front( ) )
		order->execute					( );
}

void network_world::process_responses			( )
{
	m_channel.orders.owner_delete_processed_items	( );
	u32 count							= 0;
	while ( response* const response = m_channel.responses.user_pop_front( ) ) {
		response->execute				( );
		if ( ++count >= 10 )
			break;
	}
}

void network_world::clear_resources				( )
{
	m_channel.orders.owner_delete_processed_items	( );
	while ( response* const response = m_channel.responses.user_pop_front( ) ) {
		response->execute				( );
	}
}

void network_world::dispatch_callbacks			( )
{
	process_responses					( );
}

vostok::network_core::tcp_packet* network_world::new_packet( )
{
	return								NEW( network_core::tcp_packet ) ( *g_allocator );
}
