////////////////////////////////////////////////////////////////////////////
//	Created		: 16.02.2009
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#ifdef DEBUG
#	include <typeinfo.h>
#endif // #ifdef DEBUG

#include <vostok/memory_macros.h>
#include <vostok/editor/world/api.h>

namespace vostok {
namespace editor {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::editor::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace input
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::editor::g_allocator, type )
#define DELETE( pointer, ... )					VOSTOK_DELETE_IMPL(	*::vostok::editor::g_allocator, pointer, __VA_ARGS__ )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::editor::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::editor::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::editor::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::editor::g_allocator, type, count )

#endif // #ifndef MEMORY_H_INCLUDED