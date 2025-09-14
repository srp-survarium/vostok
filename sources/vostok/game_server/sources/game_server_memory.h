////////////////////////////////////////////////////////////////////////////
//	Created		: 27.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_SERVER_MEMORY_H_INCLUDED
#define GAME_SERVER_MEMORY_H_INCLUDED

namespace vostok {
namespace game_server {

extern	vostok::memory::doug_lea_allocator_type	g_allocator;

#define USER_ALLOCATOR							g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace game_server
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::game_server::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::game_server::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::game_server::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::game_server::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::game_server::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::game_server::g_allocator, type, count )

#endif // #ifndef GAME_SERVER_MEMORY_H_INCLUDED