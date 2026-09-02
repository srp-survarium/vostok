// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_MEMORY_H_INCLUDED
#define NETWORK_CORE_MEMORY_H_INCLUDED

namespace vostok {
namespace network_core {

extern	memory::base_allocator*	g_allocator;

} // namespace network_core
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::network_core::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::network_core::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::network_core::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::network_core::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::network_core::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::network_core::g_allocator, type, count )

#endif // #ifndef NETWORK_CORE_MEMORY_H_INCLUDED
