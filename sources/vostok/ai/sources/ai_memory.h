////////////////////////////////////////////////////////////////////////////
//	Created		: 22.11.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef AI_MEMORY_H_INCLUDED
#define AI_MEMORY_H_INCLUDED

#include <vostok/ai/api.h>

namespace vostok {
namespace ai {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::ai::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace ai
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::ai::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::ai::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::ai::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::ai::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::ai::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::ai::g_allocator, type, count )

#endif // #ifndef AI_MEMORY_H_INCLUDED