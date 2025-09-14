////////////////////////////////////////////////////////////////////////////
//	Created		: 03.08.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_DEBUG_ALLOCATOR_H_INCLUDED
#define VOSTOK_MEMORY_DEBUG_ALLOCATOR_H_INCLUDED

#if VOSTOK_PLATFORM_WINDOWS
#	ifndef MASTER_GOLD
#		define VOSTOK_DEBUG_ALLOCATOR				1
#	else // #ifndef MASTER_GOLD
#		define VOSTOK_DEBUG_ALLOCATOR				0
#	endif // #ifndef MASTER_GOLD
#elif VOSTOK_PLATFORM_XBOX_360 // #if VOSTOK_PLATFORM_WINDOWS
#	ifndef MASTER_GOLD
#		define VOSTOK_DEBUG_ALLOCATOR				1
#	else // #ifndef MASTER_GOLD
#		define VOSTOK_DEBUG_ALLOCATOR				0
#	endif // #ifndef MASTER_GOLD
#elif VOSTOK_PLATFORM_PS3 // #elif VOSTOK_PLATFORM_XBOX_360
#	ifndef MASTER_GOLD
#		define VOSTOK_DEBUG_ALLOCATOR				1
#	else // #ifndef MASTER_GOLD
#		define VOSTOK_DEBUG_ALLOCATOR				0
#	endif // #ifndef MASTER_GOLD
#else // #elif VOSTOK_PLATFORM_PS3
#	error please define your platform!
#endif // #if VOSTOK_PLATFORM_WINDOWS

#if VOSTOK_DEBUG_ALLOCATOR
#	include <vostok/memory_doug_lea_mt_allocator.h>

namespace vostok {

#if VOSTOK_DEBUG_ALLOCATOR

namespace memory {
	typedef doug_lea_mt_allocator					doug_lea_mt_allocator_type;
} // namespace memory

namespace debug {
	extern VOSTOK_CORE_API memory::doug_lea_mt_allocator_type	g_mt_allocator;
} // namespace debug

#endif // #if VOSTOK_DEBUG_ALLOCATOR

} // namespace vostok

#define DEBUG_NEW( type )							VOSTOK_NEW_IMPL(		::vostok::debug::g_mt_allocator, type )
#define DEBUG_DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::debug::g_mt_allocator, pointer )
#define DEBUG_MALLOC( size, description )			VOSTOK_MALLOC_IMPL(	::vostok::debug::g_mt_allocator, size, description )
#define DEBUG_REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::debug::g_mt_allocator, pointer, size, description )
#define DEBUG_FREE( pointer )						VOSTOK_FREE_IMPL(		::vostok::debug::g_mt_allocator, pointer )
#define DEBUG_ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::debug::g_mt_allocator, type, count )

#endif // #if VOSTOK_DEBUG_ALLOCATOR

#endif // #ifndef VOSTOK_MEMORY_DEBUG_ALLOCATOR_H_INCLUDED