////////////////////////////////////////////////////////////////////////////
//	Created		: 07.11.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef RESOURCES_PERFORMANCE_TEST_MEMORY_H_INCLUDED
#define RESOURCES_PERFORMANCE_TEST_MEMORY_H_INCLUDED

namespace vostok {
namespace resources_performance_test {

extern	vostok::memory::doug_lea_allocator_type	g_allocator;

#define USER_ALLOCATOR							g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace resources_performance_test
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::resources_performance_test::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::resources_performance_test::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::resources_performance_test::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::resources_performance_test::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::resources_performance_test::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::resources_performance_test::g_allocator, type, count )

#endif // #ifndef RESOURCES_PERFORMANCE_TEST_MEMORY_H_INCLUDED