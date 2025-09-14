////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef RESOURCES_MACROS_H_INCLUDED
#define RESOURCES_MACROS_H_INCLUDED

#include <vostok/memory_doug_lea_allocator.h>

namespace vostok {
namespace memory {

extern  doug_lea_allocator_type						g_resources_helper_allocator;

} // namespace memory
} // namespace vostok

#define RES_NEW( type )			VOSTOK_NEW_IMPL(		::vostok::memory::g_resources_helper_allocator, type )
#define RES_DELETE( pointer )	VOSTOK_DELETE_IMPL(	::vostok::memory::g_resources_helper_allocator, pointer )
#define RES_ALLOC( type, count )VOSTOK_ALLOC_IMPL(	::vostok::memory::g_resources_helper_allocator, type, count )
#define RES_FREE( pointer )		VOSTOK_FREE_IMPL(		::vostok::memory::g_resources_helper_allocator, pointer )

#endif // #ifndef FS_MACROS_H_INCLUDED