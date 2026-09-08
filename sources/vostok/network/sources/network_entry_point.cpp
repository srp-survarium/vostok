////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_world.h"
#include "network_entry_point.h"
#include <vostok/network/api.h>
#include <vostok/network_core/sources/network_core_entry_point.h>
#include <vostok/linkage_helper.h>

#ifndef MASTER_GOLD
VOSTOK_DECLARE_LINKAGE_ID(network_entry_point)
#endif // #ifndef MASTER_GOLD

// Temporary /include anchor: target /GL retains this without a visible caller.
// Remove it when real callers keep the symbol alive.
#pragma comment( linker, "/include:?sign_up@login_client_impl@network@vostok@@QAEXPBDGABUsign_up_info@3@ABV?$function@$$A6AXW4connection_error_types_enum@vostok@@W4handshaking_error_types_enum@2@W4socket_error_types_enum@2@W4login_server_message_types_enum@2@ABUsign_up_info@2@@Z@boost@@@Z" )

using vostok::network::network_world;

// The PDB and object index use different names for this initializer thunk.
static vostok::uninitialized_reference<network_world>	s_world;
vostok::network::allocator_type*	vostok::network::g_allocator = 0;

vostok::network::world* vostok::network::create_world	( engine& engine, vostok::memory::base_allocator& orders_allocator )
{
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, network_world )	( engine, orders_allocator );
	return						( &*s_world );
}

void vostok::network::destroy_world					( vostok::network::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void vostok::network::memory_allocator				( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
	network_core::memory_allocator	( allocator );
}

void vostok::network::initialize					( )
{
	network_core::initialize	( );
}

void vostok::network::finalize						( )
{
	network_core::finalize		( );
}
