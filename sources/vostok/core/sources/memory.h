////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#if defined(_MSC_VER)
#	define VOSTOK_PRINTF_SPEC_LONG_LONG( digits )			"%" VOSTOK_MAKE_STRING( digits ) "I64d"
#	define VOSTOK_PRINTF_SPEC_LONG_LONG_HEX( digits )		"%" VOSTOK_MAKE_STRING( digits ) "I64x"
#else // #if defined(_MSC_VER)
#	define VOSTOK_PRINTF_SPEC_LONG_LONG( digits )			"%" VOSTOK_MAKE_STRING( digits ) "lld"
#	define VOSTOK_PRINTF_SPEC_LONG_LONG_HEX( digits )		"%" VOSTOK_MAKE_STRING( digits ) "llx"
#endif // #if defined(_MSC_VER)

namespace vostok {
namespace memory {

void preinitialize				( );
void initialize					( );
void finalize					( );

void register_debug_allocator	( u64 const max_memory_size );
void on_after_memory_initialized( );

} // namespace memory
} // namespace vostok

#endif // #ifndef MEMORY_H_INCLUDED