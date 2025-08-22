////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "navigation_world.h"
#include <vostok/ai_navigation/api.h>
#include <vostok/linkage_helper.h>

using vostok::ai::navigation::navigation_world;

//static vostok::uninitialized_reference<navigation_world>	s_world;
vostok::ai::navigation::allocator_type*	vostok::ai::navigation::g_allocator = 0;

VOSTOK_DECLARE_LINKAGE_ID(ai_navigation_entry_point)

vostok::ai::navigation::world* vostok::ai::navigation::create_world	( engine& engine, vostok::render::scene_ptr const& scene, vostok::render::debug::renderer& renderer )
{
	return 				NEW( navigation_world ) ( engine, scene, renderer );
}

void vostok::ai::navigation::destroy_world						( vostok::ai::navigation::world*& world )
{
	DELETE					(world);
	world						= 0;
}

void vostok::ai::navigation::set_memory_allocator					( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}