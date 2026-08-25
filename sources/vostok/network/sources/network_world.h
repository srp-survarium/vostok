////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_WORLD_H_INCLUDED
#define NETWORK_WORLD_H_INCLUDED

#include <vostok/network/world.h>
#include "two_way_threads_channel.h"
#include <vostok/network/engine.h>

namespace vostok {

namespace network_core {
	class tcp_packet;
} // namespace network_core

namespace network {

class network_world :
	public network::world,
	private boost::noncopyable
{
public:
					network_world		( network::engine& engine, memory::base_allocator& orders_allocator );
	virtual			~network_world		( );
	virtual	void	initialize			( );
	virtual	void	finalize			( );
	virtual	void	tick				( bool single_threaded );
	virtual	void	clear_resources		( );
	virtual	void	dispatch_callbacks	( );

public:
			// claude@MATCH: no pointer-level const - `* const` params mangle QAV
			// where the target has PAV, so the symbols would never pair
			void	add_order			( order* order );
			void	add_response		( response* response );
			network_core::tcp_packet*	new_packet	( );

public:
	inline	memory::base_allocator&		orders_allocator	( ) const	{ return m_channel.orders.owner_allocator(); }
	inline	memory::doug_lea_allocator&	responses_allocator	( ) const	{ return *::vostok::network::g_allocator; }
	inline	boost::asio::io_service&	io_service			( )			{ return *m_io_service; }

private:
			void	process_orders		( );
			void	process_responses	( );

private:
	// the io_service became heap-owned in the shipped code (pointer member)
	boost::asio::io_service*	m_io_service;
	two_way_threads_channel		m_channel;
	engine&						m_engine;
}; // class network_world

STATIC_SIZE_ASSERT(network_world, 0x124);

} // namespace network
} // namespace vostok

#endif // #ifndef NETWORK_WORLD_H_INCLUDED
