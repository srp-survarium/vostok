////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Description : memory
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <vostok/physics/api.h>
//
namespace vostok {
namespace physics {
//
	extern memory::base_allocator*					g_ph_allocator;
//
//#define USER_ALLOCATOR							*::vostok::physics::g_allocator
//#include <vostok/std_containers.h>
//#include <vostok/unique_ptr.h>
//#undef USER_ALLOCATOR
//
} // namespace physics
} // namespace vostok

//
//#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::physics::g_allocator, type )
//#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::physics::g_allocator, pointer )
//#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::physics::g_allocator, size, description )
//#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::physics::g_allocator, pointer, size, description )
//#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::physics::g_allocator, pointer )
//#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::physics::g_allocator, type, count )

#endif // #ifndef MEMORY_H_INCLUDED