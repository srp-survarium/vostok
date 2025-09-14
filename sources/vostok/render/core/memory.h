////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CORE_MEMORY_H_INCLUDED
#define VOSTOK_RENDER_CORE_MEMORY_H_INCLUDED

#include <vostok/render/api.h>

namespace vostok {
namespace render {

VOSTOK_RENDER_API extern render_allocator_type*	g_allocator;

#define USER_ALLOCATOR							*::vostok::render::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace render
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::render::g_allocator, type )
#define NEW_ARRAY(type, count)					VOSTOK_NEW_ARRAY_IMPL(*::vostok::render::g_allocator, type, count)
#define DELETE( pointer, ... )					VOSTOK_DELETE_IMPL(	*::vostok::render::g_allocator, pointer, __VA_ARGS__ )
#define DELETE_ARRAY(pointer)					VOSTOK_DELETE_ARRAY_IMPL(*::vostok::render::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::render::g_allocator, size, description )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::render::g_allocator, type, count )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::render::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::render::g_allocator, pointer )

#endif // #ifndef VOSTOK_RENDER_CORE_MEMORY_H_INCLUDED