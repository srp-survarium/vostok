////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

#include <map>
#include <vector>
#include <set>

namespace vostok {
namespace animation {

extern vostok::memory::doug_lea_allocator_type	g_allocator;

#define USER_ALLOCATOR							::vostok::animation::g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace animation
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::animation::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::animation::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::animation::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::animation::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::animation::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::animation::g_allocator, type, count )

#endif // #ifndef MEMORY_H_INCLUDED