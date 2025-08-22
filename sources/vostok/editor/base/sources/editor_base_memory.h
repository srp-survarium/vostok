////////////////////////////////////////////////////////////////////////////
//	Created		: 14.07.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef EDITOR_BASE_MEMORY_H_INCLUDED
#define EDITOR_BASE_MEMORY_H_INCLUDED

#include <vostok/editor/base/api.h>

namespace vostok {
namespace editor_base {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::editor_base::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace editor_base
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::editor_base::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::editor_base::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::editor_base::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::editor_base::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::editor_base::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::editor_base::g_allocator, type, count )

#endif // #ifndef EDITOR_BASE_MEMORY_H_INCLUDED