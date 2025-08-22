////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_ANIMATION_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"animation"
#include <vostok/extensions.h>

#if VOSTOK_PLATFORM_WINDOWS && !defined(MASTER_GOLD)
#	define	VOSTOK_USE_MAYA_ANIMATION	1
#else // #if VOSTOK_PLATFORM_WINDOWS && !defined(MASTER_GOLD)
#	define	VOSTOK_USE_MAYA_ANIMATION	0
#endif // #if VOSTOK_PLATFORM_WINDOWS && !defined(MASTER_GOLD)

#include "animation_memory.h"

namespace vostok {
namespace animation {

template < typename T >
T* get_shift_ptr( T* ptr, u32 byte_shift )
{
	return (T*) ( ( pbyte )( ptr ) + byte_shift );
}

template < typename T >
T const* get_shift_ptr( T const* ptr, u32 byte_shift )
{
	return (T*) ( ( pbyte )( ptr ) + byte_shift );
}

template< typename T1, typename T2 >
size_t bytes_dist( T1 const* from, T2 const* to )
{
	ASSERT( pbyte( from ) > pbyte( to ));
	return pbyte( from ) - pbyte( to );
}

enum {
	integration_interval_length_in_ms	= 10,
};

} // namespace animation
} // namespace vostok

#endif // #ifndef PCH_H_INCLUDED