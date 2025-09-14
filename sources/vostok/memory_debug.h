////////////////////////////////////////////////////////////////////////////
//	Created		: 13.03.2009
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_DEBUG_H_INCLUDED
#define MEMORY_DEBUG_H_INCLUDED

#include <vostok/platform_extensions.h>

namespace vostok {
namespace memory {

template <typename T>
inline T	uninitialized_value						( )
{
#if VOSTOK_PLATFORM_64_BIT
	return horrible_cast<u64,T>( ::vostok::platform::little_endian() ? 0xfdfdcdcdcdcdcdcd : 0xcdcdcdcdcdcdfdfd ).second;
#else // #if VOSTOK_PLATFORM_64_BIT
	return horrible_cast<u32,T>( ::vostok::platform::little_endian() ? 0xfdfdcdcd : 0xcdcdfdfd ).second;
#endif // #if VOSTOK_PLATFORM_64_BIT
}

} // namespace memory
} // namespace vostok

#endif // #ifndef MEMORY_DEBUG_H_INCLUDED