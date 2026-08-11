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

// The target retains this otherwise-unreferenced /GL survivor; no target object
// contains a code or data xref to it.
#pragma comment( linker, "/include:?sign_up@login_client_impl@network@vostok@@QAEXPBDGABUsign_up_info@3@ABV?$function@$$A6AXW4connection_error_types_enum@vostok@@W4handshaking_error_types_enum@2@W4socket_error_types_enum@2@W4login_server_message_types_enum@2@ABUsign_up_info@2@@Z@boost@@@Z" )

using vostok::network::network_world;

// claude@NOTE: objdiff credits 0 (symbols never pair). The base obj DOES
// contain the initializer (mangled `??__Es_world@@YAXXZ`, same 3 stores as
// target rva 0x7c5270) but the target side carries the demangled name
// (`dynamic initializer for 's_world''), so objdiff never pairs them - a
// delinker/naming gap affecting every dynamic initializer (base rich index has
// zero), not source-steerable from this TU
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
