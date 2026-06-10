////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_world.h"
#include "network_entry_point.h"
#include <vostok/network/api.h>

using vostok::network::network_world;

// STATE[PARTIAL]: legacy static ported
// FUNCTION BODY[0x7d5270] (`dynamic initializer for 's_world'')
static vostok::uninitialized_reference<network_world>	s_world;
vostok::network::allocator_type*	vostok::network::g_allocator = 0;

// STATE[PARTIAL]: legacy body ported; unverified vs target
// FUNCTION BODY[0x65cdc0]
vostok::network::world* vostok::network::create_world	( engine& engine, vostok::memory::base_allocator& orders_allocator )
{
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, network_world )	( engine, orders_allocator );
	return						( &*s_world );
}

// STATE[PARTIAL]: legacy body ported; unverified vs target
// FUNCTION BODY[0x65cd90]
void vostok::network::destroy_world					( vostok::network::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

// STATE[PARTIAL]: legacy body ported; unverified vs target
// FUNCTION BODY[0x65cd60]
void vostok::network::memory_allocator				( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}

// STATE[STUB]
void vostok::network::initialize					( )
{
	// FUNCTION BODY[0x65cd50]: 1
	// <0x65cd53>|0x003|+0x005:'38'
	// ******
}

// STATE[STUB]
void vostok::network::finalize						( )
{
	// FUNCTION BODY[0x65cd40]: 1
	// <0x65cd43>|0x003|+0x005:'43'
	// ******
}
