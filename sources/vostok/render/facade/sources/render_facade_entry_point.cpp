////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/world.h>
#include <vostok/render/api.h>
#include <vostok/render/core/memory.h>

typedef vostok::render::render_allocator_type	render_allocator_type;
typedef vostok::render::logic_allocator_type	logic_allocator_type;
typedef vostok::render::editor_allocator_type	editor_allocator_type;

using vostok::render::world;

static vostok::uninitialized_reference<world>	s_world;

logic_allocator_type*	vostok::render::logic::g_allocator = 0;
editor_allocator_type*	vostok::render::editor::g_allocator = 0;

void vostok::render::set_memory_allocator	(
		render_allocator_type&	render_allocator,
		logic_allocator_type&	logic_allocator,
		editor_allocator_type&	editor_allocator
	)
{
	R_ASSERT			( !vostok::render::g_allocator );
	g_allocator			= &render_allocator;

	R_ASSERT			( !logic::g_allocator );
	logic::g_allocator	= &logic_allocator;

	R_ASSERT			( !editor::g_allocator );
	editor::g_allocator	= &editor_allocator;
}

vostok::render::world* vostok::render::create_world	(
		vostok::memory::base_allocator& logic_allocator,
		vostok::memory::base_allocator* editor_allocator
	)
{
	VOSTOK_CONSTRUCT_REFERENCE( s_world, world )	( logic_allocator, editor_allocator );
	return					( &*s_world );
}

void vostok::render::destroy_world					( vostok::render::world*& world )
{
	R_ASSERT				( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE	( s_world );
	world					= 0;
}