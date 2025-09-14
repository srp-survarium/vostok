////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "input_world.h"
#include <vostok/input/api.h>
#include <vostok/linkage_helper.h>

using vostok::input::input_world;

VOSTOK_DECLARE_LINKAGE_ID(input_entry_point)

static vostok::uninitialized_reference<input_world>	s_world;
vostok::input::allocator_type*	vostok::input::g_allocator = 0;

vostok::input::world* vostok::input::create_world	( engine& engine, HWND const window_handle )
{
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, input_world )	( engine, window_handle );
	return						( &*s_world );
}

void vostok::input::destroy_world					( vostok::input::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void vostok::input::set_memory_allocator			( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}