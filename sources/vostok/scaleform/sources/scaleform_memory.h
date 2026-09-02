// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SCALEFORM_MEMORY_H_INCLUDED
#define SCALEFORM_MEMORY_H_INCLUDED

// the PDB shows NO module-own allocator global: this module's only dynamic
// initializers are g_file_opener/g_vostok_logger (factory.cpp), and Scaleform
// heap traffic flows through the malloc/free hooks handed to
// scaleform_engine::initialize (xrSysAllocMalloc). The survarium:: glue lives
// in game's allocator world, so reuse game's g_allocator (same namespace,
// storage in game_entry_point.cpp); lean fork of game_memory.h.

namespace survarium {

extern vostok::memory::doug_lea_allocator_type*	g_allocator;

} // namespace survarium

#define NEW( type )								VOSTOK_NEW_IMPL(		*::survarium::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::survarium::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::survarium::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::survarium::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::survarium::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::survarium::g_allocator, type, count )

#endif // #ifndef SCALEFORM_MEMORY_H_INCLUDED
