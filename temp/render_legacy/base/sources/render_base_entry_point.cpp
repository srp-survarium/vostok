////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <vostok/render/base/world.h>
#include <vostok/render/api.h>

typedef vostok::render::render_allocator_type	render_allocator_type;
typedef vostok::render::logic_allocator_type	logic_allocator_type;
typedef vostok::render::editor_allocator_type	editor_allocator_type;

using vostok::render::base_world;

static vostok::uninitialized_reference<base_world>	s_world;

render_allocator_type*	vostok::render::g_allocator = 0;
logic_allocator_type*	vostok::render::logic::g_allocator = 0;
editor_allocator_type*	vostok::render::editor::g_allocator = 0;

void vostok::render::memory_allocator		(
		render_allocator_type&	render_allocator,
		logic_allocator_type&	logic_allocator,
		editor_allocator_type&	editor_allocator
	)
{
	R_ASSERT			( !g_allocator );
	g_allocator			= &render_allocator;

	R_ASSERT			( !logic::g_allocator );
	logic::g_allocator	= &logic_allocator;

	R_ASSERT			( !editor::g_allocator );
	editor::g_allocator	= &editor_allocator;
}

vostok::render::world* vostok::render::create_world	( vostok::render::engine::wrapper& wrapper, HWND window_handle )
{
	VOSTOK_CONSTRUCT_REFERENCE( s_world, base_world)	( wrapper, window_handle );
	return					( &*s_world );
}

void vostok::render::destroy_world					( vostok::render::world*& world )
{
	R_ASSERT				( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE	( s_world );
	world					= 0;
}