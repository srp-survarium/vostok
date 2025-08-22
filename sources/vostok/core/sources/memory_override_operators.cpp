////////////////////////////////////////////////////////////////////////////
//	Created		: 07.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include <vostok/macro_platform.h>

#include <string.h>	// for memcmp in fastdelegate

#define VOSTOK_MEMORY_OVERRIDE_OPERATORS_H_INCLUDED
#include "pch.h"
#include <vostok/construction.h>
#include <vostok/memory_doug_lea_mt_allocator.h>

#ifdef VOSTOK_STATIC_LIBRARIES

pvoid __cdecl operator new		( size_t buffer_size )
{
#if !VOSTOK_PLATFORM_PS3
#	if VOSTOK_ENABLE_CRT_ALLOCATOR
		R_ASSERT					( ::vostok::memory::g_crt_allocator, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
#		ifdef DEBUG
			::vostok::memory::g_crt_allocator.malloc_impl( buffer_size, "from operator new[]", __FUNCTION__, __FILE__, __LINE__ );
#		else // #ifdef DEBUG
			::vostok::memory::g_crt_allocator.malloc_impl( buffer_size );
#		endif // #ifdef DEBUG
#	else // #if VOSTOK_DISABLE_CRT_ALLOCATOR
		VOSTOK_UNREFERENCED_PARAMETER	( buffer_size );
		UNREACHABLE_CODE			( return 0 );
#	endif // #if VOSTOK_DISABLE_CRT_ALLOCATOR
#else // #if !VOSTOK_PLATFORM_PS3
	// for boost::function functionality only!
	// if you don't see boost::function stuff in the call stack,
	// please report Dmitriy Iassenev ASAP
#	ifdef DEBUG
		return					::vostok::memory::g_crt_allocator->malloc_impl( buffer_size, "new for boost::function", __FUNCTION__, __FILE__, __LINE__ );
#	else // #ifdef DEBUG
		return					::vostok::memory::g_crt_allocator->malloc_impl( buffer_size );
#	endif // #ifdef DEBUG
#endif // #if !VOSTOK_PLATFORM_PS3
}

pvoid __cdecl operator new[]		( size_t buffer_size )
{
#if VOSTOK_ENABLE_CRT_ALLOCATOR
	R_ASSERT					( ::vostok::memory::g_crt_allocator, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
#	ifdef DEBUG
		::vostok::memory::g_crt_allocator->malloc_impl( buffer_size, "from operator new[]", __FUNCTION__, __FILE__, __LINE__ );
#	else // #ifdef DEBUG
		::vostok::memory::g_crt_allocator->malloc_impl( buffer_size );
#	endif // #ifdef DEBUG
#else // #if VOSTOK_DISABLE_CRT_ALLOCATOR
	VOSTOK_UNREFERENCED_PARAMETER	( buffer_size );
	UNREACHABLE_CODE			( return 0 );
#endif // #if VOSTOK_DISABLE_CRT_ALLOCATOR
}

void __cdecl operator delete		( pvoid pointer )
{
#if !VOSTOK_PLATFORM_PS3
#	if VOSTOK_ENABLE_CRT_ALLOCATOR
		R_ASSERT					( ::vostok::memory::g_crt_allocator, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
		::vostok::memory::g_crt_allocator->free_impl( pointer
#		ifdef DEBUG
			, __FUNCTION__, __FILE__, __LINE__
#		endif // #ifdef DEBUG
		);
#	else // #if VOSTOK_DISABLE_CRT_ALLOCATOR
		VOSTOK_UNREFERENCED_PARAMETER	( pointer );
		UNREACHABLE_CODE			( );
#	endif // #if VOSTOK_DISABLE_CRT_ALLOCATOR
#else // #if !VOSTOK_PLATFORM_PS3
	// for boost::function functionality only!
	// if you don't see boost::function stuff in the call stack,
	// please report Dmitriy Iassenev ASAP
#	ifdef DEBUG
		::vostok::memory::g_crt_allocator->free_impl( pointer, __FUNCTION__, __FILE__, __LINE__ );
#	else // #ifdef DEBUG
		::vostok::memory::g_crt_allocator->free_impl( pointer );
#	endif // #ifdef DEBUG
#endif // #if !VOSTOK_PLATFORM_PS3
}

void __cdecl operator delete[]	( pvoid pointer ) throw ( )
{
#if VOSTOK_ENABLE_CRT_ALLOCATOR
	R_ASSERT					( ::vostok::memory::g_crt_allocator, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
#	ifdef DEBUG
		::vostok::memory::g_crt_allocator->free_impl( pointer, __FUNCTION__, __FILE__, __LINE__ );
#	else // #ifdef DEBUG
		::vostok::memory::g_crt_allocator->free_impl( pointer );
#	endif // #ifdef DEBUG
#else // #if VOSTOK_DISABLE_CRT_ALLOCATOR
	VOSTOK_UNREFERENCED_PARAMETER	( pointer );
	UNREACHABLE_CODE			( );
#endif // #if VOSTOK_DISABLE_CRT_ALLOCATOR
}

namespace vostok {
namespace memory {

bool g_crt_allocations_are_enabled	= true;

static char s_crt_allocator_buffer	[ sizeof(vostok::memory::doug_lea_mt_allocator) ];
static vostok::threading::atomic32_type	s_crt_allocator_creation	=	0;

#if VOSTOK_PLATFORM_WINDOWS_32
	static char s_CRT_arena[ 2*1024*1024 ];
#elif VOSTOK_PLATFORM_WINDOWS_64 // #if VOSTOK_PLATFORM_WINDOWS_32
	static char s_CRT_arena[ 128*1024 ];
#elif VOSTOK_PLATFORM_XBOX_360 // #elif VOSTOK_PLATFORM_WINDOWS_64
	static char s_CRT_arena[ 4*1024 ];
#elif VOSTOK_PLATFORM_PS3 // #elif VOSTOK_PLATFORM_XBOX_360
	static char s_CRT_arena[ 4*1024 ];
#else // #elif VOSTOK_PLATFORM_PS3
#	error please define your platform!
#endif // #elif VOSTOK_PLATFORM_WINDOWS_64

struct inplace_constructor {
	void   operator () (vostok::memory::base_allocator* const pointer)
	{
		new (pointer) vostok::memory::doug_lea_mt_allocator( true, false, false );
		vostok::static_cast_checked<vostok::memory::doug_lea_mt_allocator*>(pointer)->initialize	(
			s_CRT_arena,
			sizeof(s_CRT_arena),
			"CRT allocator"
		);
	}
}; // struct inplace_constructor

void initialize_crt_allocator			( )
{
	R_ASSERT					( g_crt_allocations_are_enabled );
	if ( !g_crt_allocator ) {
		vostok::debug::preinitialize	( );
		vostok::logging::preinitialize	( );
		vostok::bind_pointer_to_buffer_mt_safe	(
			(vostok::memory::doug_lea_mt_allocator*&)g_crt_allocator, 
			s_crt_allocator_buffer, 
			s_crt_allocator_creation,
			inplace_constructor( )
		);
	}
}

inline void check_if_CRT_allocations_enabled	( )
{
	R_ASSERT					( vostok::memory::g_crt_allocations_are_enabled, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
	if ( vostok::memory::g_crt_allocations_are_enabled && !vostok::memory::g_crt_allocator ) {
		vostok::memory::initialize_crt_allocator	( );
		R_ASSERT				( vostok::memory::g_crt_allocator );
	}
}

#ifdef _MSC_VER
extern "C" {

pvoid malloc					( size_t size )
{
	check_if_CRT_allocations_enabled( );
	R_ASSERT					( g_crt_allocator, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
#ifdef DEBUG
	return						g_crt_allocator->malloc_impl( size, "from malloc", __FUNCTION__, __FILE__, __LINE__ );
#else // #ifdef DEBUG
	return						g_crt_allocator->malloc_impl( size );
#endif // #ifdef DEBUG
}

pvoid calloc					( size_t count, size_t element_size )
{
	pvoid const result			= malloc( count*element_size );
	memset						( result, 0, count*element_size );
	return						result;
}

pvoid realloc					( pvoid pointer, size_t size )
{
	check_if_CRT_allocations_enabled( );
	R_ASSERT					( g_crt_allocator, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
#ifdef DEBUG
	return						g_crt_allocator->realloc_impl( pointer, size, "from realloc", __FUNCTION__, __FILE__, __LINE__ );
#else // #ifdef DEBUG
	return						g_crt_allocator->realloc_impl( pointer, size );
#endif // #ifdef DEBUG
}

void free						( pvoid pointer )
{
	check_if_CRT_allocations_enabled( );
	R_ASSERT					( g_crt_allocator, "CRT allocation detected. do not use CRT allocations or setup crt allocator (the last doesn't work in MASTER_GOLD configuration)" );
#ifdef DEBUG
	g_crt_allocator->free_impl	( pointer, __FUNCTION__, __FILE__, __LINE__ );
#else // #ifdef DEBUG
	g_crt_allocator->free_impl	( pointer );
#endif // #ifdef DEBUG
}

size_t _msize (pvoid pblock)
{
	R_ASSERT					( vostok::memory::g_crt_allocator );
	return						g_crt_allocator->usable_size( pblock );
}

pvoid _malloc_crt				( size_t size )
{
	return						malloc( size );
}

pvoid _calloc_crt				( size_t count, size_t size )
{
	return						calloc( count, size );
}

pvoid _realloc_crt				( pvoid ptr, size_t size )
{
	return						realloc( ptr, size );
}

pvoid _recalloc_crt				( pvoid ptr, size_t count, size_t size )
{
	VOSTOK_UNREFERENCED_PARAMETERS( ptr, count, size );
	NOT_IMPLEMENTED				( return 0 );
}

pvoid _aligned_malloc			( size_t size, size_t alignment )
{
	VOSTOK_UNREFERENCED_PARAMETERS( size, alignment );
	NOT_IMPLEMENTED				( return 0 );
}

pvoid _aligned_realloc			( pvoid ptr, size_t size, size_t alignment )
{
	VOSTOK_UNREFERENCED_PARAMETERS( ptr, size, alignment );
	NOT_IMPLEMENTED				( return 0 );
}

void _aligned_free				( pvoid ptr )
{
	VOSTOK_UNREFERENCED_PARAMETER	( ptr );
	NOT_IMPLEMENTED				( );
}

pvoid _aligned_offset_malloc	( size_t size, size_t alignment, size_t offset )
{
	VOSTOK_UNREFERENCED_PARAMETERS( size, alignment, offset );
	NOT_IMPLEMENTED				( return 0 );
}

pvoid _aligned_offset_realloc	( pvoid ptr, size_t size, size_t alignment, size_t offset )
{
	VOSTOK_UNREFERENCED_PARAMETERS( ptr, size, alignment, offset );
	NOT_IMPLEMENTED				( return 0 );
}

} // extern "C"

#endif // #ifdef _MSC_VER

} // namespace memory
} // namespace vostok

#endif // #ifdef VOSTOK_STATIC_LIBRARIES


extern "C" {

pvoid vostok_ogg_malloc			( size_t size );

pvoid vostok_ogg_calloc			( size_t count, size_t element_size )
{
	pvoid const result			= vostok_ogg_malloc( count*element_size );
	memset						( result, 0, count*element_size );
	return						result;
}

#ifndef MASTER_GOLD
pvoid vostok_ogg_malloc			( size_t size )
{
	return						VOSTOK_MALLOC_IMPL( vostok::debug::g_mt_allocator, size, "ogg malloc" );
}

pvoid vostok_ogg_realloc			( pvoid pointer, size_t size )
{
	return						VOSTOK_REALLOC_IMPL( vostok::debug::g_mt_allocator, pointer, size, "ogg realloc" );
}

void vostok_ogg_free				( pvoid pointer )
{
	VOSTOK_FREE_IMPL				( vostok::debug::g_mt_allocator, pointer );
}
#else // #ifndef MASTER_GOLD
pvoid vostok_ogg_malloc			( size_t size )
{
	return						malloc( size );
}

pvoid vostok_ogg_realloc			( pvoid pointer, size_t size )
{
	return						realloc( pointer, size );
}

void vostok_ogg_free				( pvoid pointer )
{
	free						( pointer );
}
#endif // #ifndef MASTER_GOLD

} // extern "C"