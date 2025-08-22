////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_world.h"
#include <vostok/network/api.h>

using vostok::network::network_world;

static vostok::uninitialized_reference<network_world>	s_world;
vostok::network::allocator_type*	vostok::network::g_allocator = 0;

vostok::network::world* vostok::network::create_world	( engine& engine )
{
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, network_world )	( engine );
	return						( &*s_world );
}

void vostok::network::destroy_world					( vostok::network::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void vostok::network::set_memory_allocator			( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}

void vostok::network::dispatch_callbacks				( )
{
}

// here is _STLP_DECLSPEC
_STLP_DECLSPEC