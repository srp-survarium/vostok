////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.11.2008
//	Author		: Lain
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef TEST_MEMORY_H_INCLUDED
#define TEST_MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

#include <map>
#include <vector>
#include <set>

extern	vostok::memory::doug_lea_allocator_type	g_test_allocator;

#define USER_ALLOCATOR							g_test_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

#define TEST_NEW( type )								VOSTOK_NEW_IMPL(		g_test_allocator, type )
#define TEST_DELETE( pointer )							VOSTOK_DELETE_IMPL(	g_test_allocator, pointer )
#define TEST_MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	g_test_allocator, size, description )
#define TEST_REALLOC( pointer, size, description )		VOSTOK_REALLOC_IMPL(	g_test_allocator, pointer, size, description )
#define TEST_FREE( pointer )							VOSTOK_FREE_IMPL(		g_test_allocator, pointer )
#define TEST_ALLOC( type, count )						VOSTOK_ALLOC_IMPL(	g_test_allocator, type, count )

#endif // #ifndef TEST_MEMORY_H_INCLUDED