////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/survarium/game_module_proxy.h>
#include <vostok/game/api.h>

using survarium::game_module_proxy;
using vostok::engine_user::world;

// sushi@TODO: Re-measure create/destroy after the coordinated engine_user::world
// vtable migration tracked in docs/binary_matching/review_todos.md restores the callees.
world* game_module_proxy::create_world						(
		vostok::engine_user::engine& engine,
		vostok::render::world& render_world,
		vostok::sound::world& sound,
		vostok::network::world& network
	)
{
	return
		survarium::game_module::create_world(
			engine,
			render_world,
			sound,
			network
		);
}

void game_module_proxy::destroy_world						( vostok::engine_user::world*& world )
{
	survarium::game_module::destroy_world				( world );
}

void game_module_proxy::register_memory_allocators			( )
{
	survarium::game_module::register_memory_allocators	( );
}

vostok::memory::doug_lea_allocator_type& game_module_proxy::allocator	( )
{
	return	survarium::game_module::allocator( );
}
