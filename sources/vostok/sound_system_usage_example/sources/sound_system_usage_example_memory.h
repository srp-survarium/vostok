////////////////////////////////////////////////////////////////////////////
//	Created		: 29.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_SYSTEM_USAGE_EXAMPLE_MEMORY_H_INCLUDED
#define SOUND_SYSTEM_USAGE_EXAMPLE_MEMORY_H_INCLUDED

namespace vostok {
namespace sound_system_usage_example {

extern	vostok::memory::doug_lea_allocator_type	g_allocator;

#define USER_ALLOCATOR							g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace sound_system_usage_example
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::sound_system_usage_example::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::sound_system_usage_example::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::sound_system_usage_example::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::sound_system_usage_example::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::sound_system_usage_example::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::sound_system_usage_example::g_allocator, type, count )

#endif // #ifndef SOUND_SYSTEM_USAGE_EXAMPLE_MEMORY_H_INCLUDED