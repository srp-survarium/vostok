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

using vostok::network::network_world;

// STATE[PARTIAL]: legacy static ported; the base obj DOES contain the
// initializer (mangled `??__Es_world@@YAXXZ`, same 3 stores as target rva
// 0x7c5270) but the target side carries the demangled name
// (`dynamic initializer for 's_world''), so objdiff never pairs them - a
// delinker/naming gap affecting every dynamic initializer (base rich index has
// zero), not source-steerable from this TU
static vostok::uninitialized_reference<network_world>	s_world;
vostok::network::allocator_type*	vostok::network::g_allocator = 0;

// STATE[100%|DONE]
vostok::network::world* vostok::network::create_world	( engine& engine, vostok::memory::base_allocator& orders_allocator )
{
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, network_world )	( engine, orders_allocator );
	return						( &*s_world );
}

// STATE[100%|DONE]
void vostok::network::destroy_world					( vostok::network::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

// STATE[100%|DONE]
void vostok::network::memory_allocator				( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
	network_core::memory_allocator	( allocator );
}

// STATE[100%|DONE]
void vostok::network::initialize					( )
{
	network_core::initialize	( );
}

// STATE[100%|DONE]
void vostok::network::finalize						( )
{
	network_core::finalize		( );
}
