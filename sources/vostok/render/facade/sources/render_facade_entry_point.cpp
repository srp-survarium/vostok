////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/api.h>
#include <vostok/render/world.h>
#include <vostok/render/core/memory.h>

namespace vostok {
namespace render {

static uninitialized_reference< world >	s_world;

logic_allocator_type*	logic::g_allocator	= 0;
editor_allocator_type*	editor::g_allocator	= 0;

void set_memory_allocator(
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

world* create_world(
	memory::base_allocator&				logic_allocator,
	memory::base_allocator*				editor_allocator,
	configs::binary_config_ptr const&	in_config,
	bool								is_editor
)
{
	VOSTOK_CONSTRUCT_REFERENCE( s_world, world )(
		logic_allocator,
		editor_allocator,
		in_config,
		is_editor
	);
	return &*s_world;
}

void destroy_world( world*& instance )
{
	R_ASSERT					( &*s_world == instance );
	VOSTOK_DESTROY_REFERENCE	( s_world );
	instance					= 0;
}

} // namespace render
} // namespace vostok
