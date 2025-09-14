////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef FS_MACROS_H_INCLUDED
#define FS_MACROS_H_INCLUDED

#include <vostok/memory_synchronized_allocator.h>

namespace vostok {
namespace memory {

typedef memory::memory_synchronized_allocator<memory::doug_lea_allocator_type>	fs_allocator;
//typedef memory::doug_lea_allocator_type	fs_allocator;
extern	fs_allocator				g_fs_allocator;

} // namespace memory
} // namespace vostok

#define FS_NEW( type )			VOSTOK_NEW_IMPL(		::vostok::memory::g_fs_allocator, type )
#define FS_DELETE( pointer )	VOSTOK_DELETE_IMPL(	::vostok::memory::g_fs_allocator, pointer )
#define FS_ALLOC( type, count )	VOSTOK_ALLOC_IMPL(	::vostok::memory::g_fs_allocator, type, count )
#define FS_FREE( pointer )		VOSTOK_FREE_IMPL(		::vostok::memory::g_fs_allocator, pointer )

#endif // #ifndef FS_MACROS_H_INCLUDED