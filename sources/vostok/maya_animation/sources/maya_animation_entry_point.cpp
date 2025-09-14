////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.01.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "maya_animation_world.h"
#include <vostok/maya_animation/api.h>
#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID(maya_animation_entry_point)

using vostok::maya_animation::maya_animation_world;

static vostok::uninitialized_reference< maya_animation_world >	s_world;

vostok::maya_animation::allocator_type*	vostok::maya_animation::g_allocator = 0;

vostok::maya_animation::world* vostok::maya_animation::create_world( engine& engine )
{
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, maya_animation_world )	( engine );
	return						( &*s_world );
}

void vostok::maya_animation::destroy_world( vostok::maya_animation::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void vostok::maya_animation::set_memory_allocator( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}