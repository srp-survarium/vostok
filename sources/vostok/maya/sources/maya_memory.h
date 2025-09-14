////////////////////////////////////////////////////////////////////////////
//	Created		: 07.05.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <vostok/memory_crt_allocator.h>

namespace vostok {
namespace maya {

typedef memory::doug_lea_allocator_type			allocator_type;
extern allocator_type							g_allocator;

#define USER_ALLOCATOR							::vostok::maya::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace maya
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::maya::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::maya::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::maya::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::maya::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::maya::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::maya::g_allocator, type, count )

#endif // #ifndef MEMORY_H_INCLUDED