////////////////////////////////////////////////////////////////////////////
//	Created		: 23.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "rtp_world.h"
#include <vostok/rtp/api.h>
#include <vostok/linkage_helper.h>

using vostok::rtp::rtp_world;

VOSTOK_DECLARE_LINKAGE_ID(rtp_entry_point)

static vostok::uninitialized_reference<rtp_world>	s_world;
vostok::rtp::allocator_type*	vostok::rtp::g_allocator = 0;

vostok::rtp::world* vostok::rtp::create_world	(
		engine& engine,
		animation::world& animation,
		vostok::render::scene_ptr const& scene,
		vostok::render::game::renderer* renderer,
		pcstr const resources_vostok_path
	)
{
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, rtp_world )	( engine, animation, scene, renderer, resources_vostok_path );
	return						( &*s_world );
}

void vostok::rtp::destroy_world					( vostok::rtp::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void vostok::rtp::set_memory_allocator			( memory::base_allocator& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}