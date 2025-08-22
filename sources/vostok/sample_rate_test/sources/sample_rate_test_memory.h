////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2011
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SAMPLE_RATE_TEST_MEMORY_H_INCLUDED
#define SAMPLE_RATE_TEST_MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

#include <map>
#include <vector>
#include <set>

namespace vostok {
namespace sample_rate_test {

extern	vostok::memory::doug_lea_allocator_type	g_allocator;

#define USER_ALLOCATOR							g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace sample_rate_test
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::sample_rate_test::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::sample_rate_test::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::sample_rate_test::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::sample_rate_test::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::sample_rate_test::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::sample_rate_test::g_allocator, type, count )

#endif // #ifndef SAMPLE_RATE_TEST_MEMORY_H_INCLUDED