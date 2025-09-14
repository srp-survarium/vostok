////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef LOGGING_MEMORY_H_INCLUDED
#define LOGGING_MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

#include <map>
#include <vector>
#include <set>

#include <vostok/logging/api.h>

namespace vostok {
namespace logging {

extern memory::doug_lea_allocator *				g_allocator;

#define USER_ALLOCATOR							*::vostok::logging::g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace logging
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::logging::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::logging::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::logging::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::logging::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::logging::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::logging::g_allocator, type, count )

#endif // #ifndef LOGGING_MEMORY_H_INCLUDED