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

#include <vostok/os_preinclude.h>
#define _WIN32_WINNT				0x0500
#include <vostok/os_include.h>

namespace survarium {

extern vostok::memory::doug_lea_allocator_type*	g_allocator;

#define USER_ALLOCATOR							*::survarium::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace survarium

#define NEW( type )								VOSTOK_NEW_IMPL(		::survarium::g_allocator, type )
#define NEW_ARRAY( type, count )				VOSTOK_NEW_ARRAY_IMPL(	::survarium::g_allocator, type, count )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::survarium::g_allocator, pointer )
#define DELETE_ARRAY( pointer )				VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::survarium::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::survarium::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::survarium::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::survarium::g_allocator, type, count )

#endif // #ifndef MEMORY_H_INCLUDED
