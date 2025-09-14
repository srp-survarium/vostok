////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_LOGIC_ALLOCATOR_H_INCLUDED
#define VOSTOK_RENDER_BASE_LOGIC_ALLOCATOR_H_INCLUDED

#include <vostok/render/api.h>

namespace vostok {
namespace render {
namespace logic {

VOSTOK_RENDER_API extern logic_allocator_type*	g_allocator;

#define USER_ALLOCATOR							*::vostok::render::logic::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace logic
} // namespace render
} // namespace vostok

#define L_NEW( type )							VOSTOK_NEW_IMPL(		*::vostok::render::logic::g_allocator, type )
#define L_DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::render::logic::g_allocator, pointer )
#define L_MALLOC( size, description )			VOSTOK_MALLOC_IMPL(	*::vostok::render::logic::g_allocator, size, description )
#define L_REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::render::logic::g_allocator, pointer, size, description )
#define L_FREE( pointer )						VOSTOK_FREE_IMPL(		*::vostok::render::logic::g_allocator, pointer )
#define L_ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::render::logic::g_allocator, type, count )

#endif // #ifndef VOSTOK_RENDER_BASE_LOGIC_ALLOCATOR_H_INCLUDED