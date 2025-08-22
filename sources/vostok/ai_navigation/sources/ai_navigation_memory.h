////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef AI_NAVIGATION_MEMORY_H_INCLUDED
#define AI_NAVIGATION_MEMORY_H_INCLUDED

#include <vostok/ai_navigation/api.h>

namespace vostok {
namespace ai {
namespace navigation {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::ai::navigation::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace navigation
} // namespace ai
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::ai::navigation::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::ai::navigation::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::ai::navigation::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::ai::navigation::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::ai::navigation::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::ai::navigation::g_allocator, type, count )

#endif // #ifndef AI_NAVIGATION_MEMORY_H_INCLUDED