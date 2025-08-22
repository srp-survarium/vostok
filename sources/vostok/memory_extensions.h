////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_EXTENSIONS_H_INCLUDED
#define VOSTOK_MEMORY_EXTENSIONS_H_INCLUDED

#include <vostok/macro_extensions.h>
#include <vostok/type_extensions.h>
#include <vostok/debug/extensions.h>
#include <vostok/memory_pthreads3_allocator.h>

#if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_MAX_CACHE_LINE_SIZE				64
#elif VOSTOK_PLATFORM_XBOX_360 // #if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_MAX_CACHE_LINE_SIZE				128
#elif VOSTOK_PLATFORM_PS3 // #elif VOSTOK_PLATFORM_XBOX_360
#	define VOSTOK_MAX_CACHE_LINE_SIZE				128
#else // #elif VOSTOK_PLATFORM_PS3
#	error please define your platform!
#endif // #if VOSTOK_PLATFORM_WINDOWS

#define VOSTOK_MAX_CACHE_LINE_PAD					char VOSTOK_STRING_CONCAT(m_cache_line_pad_$, __LINE__) [VOSTOK_MAX_CACHE_LINE_SIZE]

#if !VOSTOK_DISABLE_CRT_ALLOCATOR
#	include <vostok/memory_crt_allocator.h>
#endif // #if !VOSTOK_DISABLE_CRT_ALLOCATOR

namespace vostok {
namespace memory {

#ifdef VOSTOK_STATIC_LIBRARIES
	class doug_lea_mt_allocator;
	typedef memory::doug_lea_mt_allocator		crt_allocator_type;
#else // #ifdef VOSTOK_STATIC_LIBRARIES
	typedef memory::crt_allocator				crt_allocator_type;
#endif // #ifdef VOSTOK_STATIC_LIBRARIES

	extern VOSTOK_CORE_API crt_allocator_type*	g_crt_allocator;
} // namespace memory
} // namespace vostok

#define VOSTOK_DEFAULT_ALIGN_SIZE					8
#define VOSTOK_DEFAULT_ALIGN						VOSTOK_ALIGN(VOSTOK_DEFAULT_ALIGN_SIZE)

namespace vostok {
namespace memory {

typedef pthreads3_allocator						pthreads_allocator_type;
extern VOSTOK_CORE_API pthreads_allocator_type	g_mt_allocator;

} // namespace memory
} // namespace vostok

#include <vostok/memory_doug_lea_allocator.h>
#include <vostok/memory_allocator_helper.h>
#include <vostok/memory_macros.h>
#include <vostok/memory_buffer.h>
#include <fastdelegate/fastdelegate.h>

namespace vostok {

u64 const Kb					= u64(1024);
u64 const Mb					= u64(1024)*Kb;
u64 const Gb					= u64(1024)*Mb;
u64 const Tb					= u64(1024)*Gb;

namespace memory {

namespace debug {
	enum {
		underrun_guard			= 0xffbdFDFD,
		overrun_guard			= 0xffbdFDFD,
		uninitialized_memory	= 0xffbdCDCD,
		freed_memory			= 0xffbdDDDD,
	};
} // namespace debug

VOSTOK_CORE_API	void allocate_region				( u64 const additional_memory_size = 0 );
VOSTOK_CORE_API	void allocate_region				( u64 const additional_memory_size, u64 const additional_address_space );

typedef doug_lea_allocator							doug_lea_allocator_type;

typedef fastdelegate::FastDelegate<void ( base_allocator const&, u32, pvoid, u64, pcstr )>	allocator_predicate_type;
VOSTOK_CORE_API	void iterate_allocators				( allocator_predicate_type const& predicate );
VOSTOK_CORE_API	void register_allocator				( base_allocator& allocator, u64 arena_size, pcstr description );

VOSTOK_CORE_API	void dump_statistics				( bool const dump_stats_for_empty_arenas_as_well = false );
VOSTOK_CORE_API	void copy							( mutable_buffer const & destination, const_buffer const & source );
VOSTOK_CORE_API	void copy							( pvoid destination, size_t destination_size, pcvoid source, size_t source_size );

VOSTOK_CORE_API	void zero							( mutable_buffer const & destination );
VOSTOK_CORE_API	void zero							( pvoid destination, size_t destination_size );

				template <class T, size_t count>
				void zero							( T (& destination)[count] ) { zero(destination, count * sizeof(T)); }

VOSTOK_CORE_API	void fill32							( mutable_buffer const & destination, u32 value, u32 count );
VOSTOK_CORE_API	void fill32							( pvoid destination, size_t destination_size_in_bytes, u32 value, size_t count );
VOSTOK_CORE_API	void fill8							( mutable_buffer const & destination, u8  value, u32 count );
VOSTOK_CORE_API	void fill8							( pvoid destination, size_t destination_size_in_bytes, u8  value, size_t count );

VOSTOK_CORE_API	int  compare						( const_buffer const & buffer1, const_buffer const & buffer2 );
VOSTOK_CORE_API	bool equal							( const_buffer const & buffer1, const_buffer const & buffer2 );

VOSTOK_CORE_API	void lock_process_heap				( );
VOSTOK_CORE_API	bool try_lock_process_heap			( );
VOSTOK_CORE_API	void unlock_process_heap			( );

inline pvoid fill_uninitialized( pvoid buffer, u32 const buffer_size )
{
#ifndef MASTER_GOLD
	u32 const u32_count	= buffer_size/4;
	if ( u32_count )
		memory::fill32	( buffer, buffer_size, (u32)debug::uninitialized_memory, u32_count );

	u32 const u8_count	= buffer_size - u32_count*sizeof(u32);
	if ( u8_count )
		memory::fill8	( static_cast<u32*>(buffer) + u32_count, u8_count, 0xcd, u8_count );
#endif // #ifndef MASTER_GOLD
	VOSTOK_UNREFERENCED_PARAMETER( buffer_size );
	return				buffer;
}

} // namespace memory
} // namespace vostok

#define ALLOCA( size )								::vostok::memory::fill_uninitialized( VOSTOK_ALLOCA_IMPL(size), (size) )

// multithreaded general purpose memory allocation macros
// use ONLY in case when you need multi threaded functionality
// otherwise use specific single threaded memory allocator
#define MT_NEW( type )								VOSTOK_NEW_IMPL(		::vostok::memory::g_mt_allocator, type )
#define MT_DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::memory::g_mt_allocator, pointer )
#define MT_MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::memory::g_mt_allocator, size, description )
#define MT_REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::memory::g_mt_allocator, pointer, size, description )
#define MT_FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::memory::g_mt_allocator, pointer )
#define MT_ALLOC( type, count )						VOSTOK_ALLOC_IMPL(	::vostok::memory::g_mt_allocator, type, count )

#endif // #ifndef VOSTOK_MEMORY_EXTENSIONS_H_INCLUDED
