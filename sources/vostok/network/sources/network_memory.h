////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_MEMORY_H_INCLUDED
#define NETWORK_MEMORY_H_INCLUDED

#include <vostok/network/api.h>

namespace vostok {
namespace network {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::network::g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace network
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::network::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::network::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::network::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::network::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::network::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::network::g_allocator, type, count )

#endif // #ifndef NETWORK_MEMORY_H_INCLUDED