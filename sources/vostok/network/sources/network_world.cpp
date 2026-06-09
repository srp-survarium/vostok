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

// STATE[STUB]
static void empty_function ( ) { }
	// FUNCTION BODY[0x65c6e0]
	// ******

// STATE[PARTIAL]: legacy body ported; m_io_service heap-init is a buildability
// guess (the target init-list runs 0xb9 bytes before line 23)
network_world::network_world					( vostok::network::engine& engine, vostok::memory::base_allocator& orders_allocator ) :
	m_io_service						( NEW( boost::asio::io_service ) ( ) ),
	m_channel							( *vostok::network::g_allocator, orders_allocator ),
	m_engine							( engine )
{
	m_channel.orders.user_initialize	( );
	m_channel.responses.owner_initialize( NEW(functor_response)( &empty_function ), NEW(functor_response)( &empty_function ) );

	// FUNCTION BODY[0x65cb30]: 2
	// <0x65cbe9>|0x0b9|+0x00e:'23'
	// <0x65cbf7>|0x0c7|+0x131:'24'
	// ******
}

// STATE[PARTIAL]: legacy body ported; the second target record (<0>) is likely
// the m_io_service delete - unreconstructed
network_world::~network_world					( )
{
	m_channel.responses.owner_finalize	( );

	// FUNCTION BODY[0x65c9f0]: 2
	// <0x65ca02>|0x012|+0x00b:'29'
	// <0>
	// ******
}

// STATE[PARTIAL]: legacy body ported
void network_world::initialize					( )
{
	m_channel.orders.owner_initialize	( VOSTOK_NEW_IMPL(orders_allocator(), functor_order)( &empty_function ), VOSTOK_NEW_IMPL(orders_allocator(), functor_order)( &empty_function ) );
	m_channel.responses.user_initialize	( );

	// FUNCTION BODY[0x65c860]: 2
	// <0x65c873>|0x013|+0x16d:'35'
	// <0x65c9e0>|0x180|+0x00b:'36'
	// ******
}

// STATE[PARTIAL]: legacy body ported
void network_world::finalize					( )
{
	m_channel.orders.owner_finalize		( );

	// FUNCTION BODY[0x65c820]: 1
	// <0x65c82c>|0x00c|+0x00e:'41'
	// ******
}

// STATE[PARTIAL]: legacy tick() ported; the single_threaded branch (carcass
// lines 48-51) is unreconstructed
void network_world::tick						( bool single_threaded )
{
	process_orders						( );
	m_io_service->poll					( );

	// FUNCTION BODY[0x65ca50]: 6
	// <0x65ca5f>|0x00f|+0x00b:'46'
	// <0>
	// <0x65ca6a>|0x01a|+0x008:'48'
	// <0x65ca72>|0x022|+0x00e:'49'
	// <0x65ca80>|0x030|+0x002:'50'
	// <0x65ca82>|0x032|+0x00e:'51'
	// ******
}

// STATE[PARTIAL]: legacy body ported
void network_world::add_order					( vostok::network::order* const order )
{
	m_channel.orders.owner_push_back	( order );

	// FUNCTION BODY[0x65c710]: 1
	// <0x65c719>|0x009|+0x012:'56'
	// ******
}

// STATE[PARTIAL]: legacy body ported
void network_world::add_response				( vostok::network::response* const response )
{
	m_channel.responses.owner_push_back	( response );

	// FUNCTION BODY[0x65c6f0]: 1
	// <0x65c6f9>|0x009|+0x00f:'61'
	// ******
}

// STATE[PARTIAL]: legacy body ported; statement count matches (3)
void network_world::process_orders				( )
{
	m_channel.responses.owner_delete_processed_items	( );
	while ( order* const order = m_channel.orders.user_pop_front( ) )
		order->execute					( );

	// LOCALS
	// order* const 					order<1>
	// ******

	// FUNCTION BODY[0x65c7e0]: 3
	// <0x65c7e9>|0x009|+0x00b:'66'
	// <0x65c7f4>|0x014|+0x017|[1]:'67'
	// <0x65c80b>|0x02b|+0x00f:'68'
	// ******
}

// STATE[PARTIAL]: legacy body ported; the target adds a u32 count local
// (carcass lines 74/77) - unreconstructed
void network_world::process_responses			( )
{
	m_channel.orders.owner_delete_processed_items	( );
	while ( response* const response = m_channel.responses.user_pop_front() )
		response->execute				( );

	// LOCALS
	// u32 								count
	// response* const 					response<1>
	// ******

	// FUNCTION BODY[0x65c780]: 7
	// <0x65c789>|0x009|+0x00e:'73'
	// <0x65c797>|0x017|+0x007:'74'
	// <0x65c79e>|0x01e|+0x014|[1]:'75'
	// <0x65c7b2>|0x032|+0x00d:'76'
	// <0x65c7bf>|0x03f|+0x00f:'77'
	// <0x65c7ce>|0x04e|+0x002:'78'
	// <0x65c7d0>|0x050|+0x002:'79'
	// ******
}

// STATE[STUB]: legacy body was empty; the target drains a response queue here
void network_world::clear_resources				( )
{
	// LOCALS
	// response* const 					response<1>
	// ******

	// CALL SITE INFO
	// <0x65c776> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x65c740]: 4
	// <0x65c749>|0x009|+0x00e:'84'
	// <0x65c757>|0x017|+0x014|[1]:'85'
	// <0x65c76b>|0x02b|+0x00d:'86'
	// <0x65c778>|0x038|+0x002:'87'
	// ******
}

// STATE[PARTIAL]: legacy body ported
void network_world::dispatch_callbacks			( )
{
	process_responses					( );

	// FUNCTION BODY[0x65c840]: 1
	// <0x65c847>|0x007|+0x008:'92'
	// ******
}

// STATE[PARTIAL]: legacy body ported onto network_core::tcp_packet
vostok::network_core::tcp_packet* network_world::new_packet( )
{
	return								NEW( network_core::tcp_packet ) ( *g_allocator );

	// FUNCTION BODY[0x65caa0]: 1
	// <0x65caa9>|0x009|+0x07d:'97'
	// ******
}
