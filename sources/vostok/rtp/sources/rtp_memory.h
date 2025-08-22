////////////////////////////////////////////////////////////////////////////
//	Created		: 23.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef RTP_MEMORY_H_INCLUDED
#define RTP_MEMORY_H_INCLUDED

#include <vostok/rtp/api.h>

namespace vostok {
namespace rtp {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::rtp::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace rtp
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::rtp::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::rtp::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::rtp::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::rtp::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::rtp::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::rtp::g_allocator, type, count )

#endif // #ifndef RTP_MEMORY_H_INCLUDED