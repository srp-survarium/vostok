////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_THREADING_FUNCTIONS_H_INCLUDED
#define VOSTOK_THREADING_FUNCTIONS_H_INCLUDED

#if VOSTOK_PLATFORM_XBOX_360
#	include <PPCIntrinsics.h>
#	ifdef NDEBUG
#		include <vostok/os_include.h>
#	endif // #ifdef NDEBUG
#endif // #if VOSTOK_PLATFORM_XBOX_360 

#if defined(__GNUC__)
#	include <xtls.h>
#endif // #if defined(__GNUC__)

#include <boost/function.hpp>

//#include <intrin.h>

namespace vostok {
namespace threading {

#if VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360
	typedef long							atomic32_value_type;
	typedef atomic32_value_type volatile	atomic32_type;
	typedef s64								atomic64_value_type;
	typedef atomic64_value_type volatile	atomic64_type;
	typedef pvoid							atomic_ptr_value_type;
	typedef atomic_ptr_value_type volatile	atomic_ptr_type;
#elif VOSTOK_PLATFORM_PS3 // #if VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360
	typedef u32								atomic32_value_type;
	typedef atomic32_value_type				atomic32_type;
	typedef u64								atomic64_value_type;
	typedef atomic64_value_type				atomic64_type;
	typedef pvoid							atomic_ptr_value_type;
	typedef atomic_ptr_value_type			atomic_ptr_type;
#else // #elif VOSTOK_PLATFORM_PS3
#	error define atomic type for your platform here
#endif // #if VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360

template < typename T, bool is_volatile >
struct atomic_type_helper {
	typedef T result;
}; // struct atomic_type_helper

template < typename T >
struct atomic_type_helper<T,true> {
	typedef T volatile result;
}; // struct atomic_type_helper

template < typename T >
struct atomic_ptr {
	typedef typename atomic_type_helper< T*, boost::is_volatile<atomic_ptr_type>::value >::result type;
}; // struct atomic_ptr

typedef boost::function< void ( ) >			thread_function_type;

enum tasks_awareness {
	tasks_aware,
	tasks_unaware,
}; // enum tasks_awareness

#if VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360
	typedef u32				thread_id_type;
#elif VOSTOK_PLATFORM_PS3 // #if VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360
	typedef u64				thread_id_type;
#else // #elif VOSTOK_PLATFORM_PS3
#	error define thread_id_type for your platform here
#endif // #if VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360

VOSTOK_CORE_API	thread_id_type	spawn						(
							thread_function_type const& function_to_call,
							pcstr thread_name_for_debugger,
							pcstr thread_name_for_logging,
							u32 stack_size,
							u32 hardware_thread,
							tasks_awareness tasks_awareness = tasks_aware,
							pvoid * out_handle = NULL
						);

VOSTOK_CORE_API	void	yield									( u32 milliseconds = 0 );
VOSTOK_CORE_API	void	set_thread_name							( pcstr debugger_name, pcstr log_string );
VOSTOK_CORE_API	pcstr	current_thread_logging_name				( );
VOSTOK_CORE_API	u32		core_count								( );

VOSTOK_CORE_API	u32		current_thread_id						( );

VOSTOK_CORE_API	void	set_current_thread_affinity				( u32 const hardware_thread );
VOSTOK_CORE_API	u32		current_thread_affinity					( );

VOSTOK_CORE_API	extern	command_line::key	g_debug_single_thread;

// interlocked functions
template < typename T >
inline T* interlocked_exchange_pointer							( typename atomic_ptr<T>::type& target, T* value )
{
	atomic_ptr_value_type const result = interlocked_exchange_pointer( reinterpret_cast<atomic_ptr_type&>(target), static_cast<atomic_ptr_value_type>(value) );
	return	static_cast<T*>( result );
}

VOSTOK_CORE_API	atomic32_value_type	interlocked_increment		( atomic32_type& value );
VOSTOK_CORE_API	atomic32_value_type	interlocked_decrement		( atomic32_type& value );
VOSTOK_CORE_API	atomic32_value_type	interlocked_exchange_add	( atomic32_type& value,  atomic32_value_type  increment );
VOSTOK_CORE_API	atomic32_value_type	interlocked_exchange_sub	( atomic32_type& value,  atomic32_value_type  subtract );
VOSTOK_CORE_API	atomic32_value_type	interlocked_and				( atomic32_type& target, atomic32_value_type  mask );
VOSTOK_CORE_API	atomic32_value_type	interlocked_or				( atomic32_type& target, atomic32_value_type  mask );
//VOSTOK_CORE_API	atomic32_value_type	interlocked_xor				( atomic32_type& target, atomic32_value_type  mask );
VOSTOK_CORE_API	atomic32_value_type	interlocked_exchange		( atomic32_type& target, atomic32_value_type  value );
VOSTOK_CORE_API	pvoid				interlocked_exchange_pointer( atomic_ptr_type& target, pvoid value );
VOSTOK_CORE_API	atomic32_value_type	interlocked_compare_exchange( atomic32_type& target, atomic32_value_type exchange, atomic32_value_type  comparand );
VOSTOK_CORE_API	atomic64_value_type	interlocked_compare_exchange( atomic64_type& target, atomic64_value_type exchange, atomic64_value_type comparand );
VOSTOK_CORE_API	atomic64_value_type interlocked_exchange		( atomic64_type& target, atomic64_value_type exchange );
//VOSTOK_CORE_API	atomic64_value_type interlocked_exchange_add	( atomic64_type& target, atomic64_value_type increment );
VOSTOK_CORE_API	pvoid		interlocked_compare_exchange_pointer( atomic_ptr_type& target, pvoid exchange, pvoid comparand );

// Thread Local Storage functions
#if defined(_MSC_VER)
	typedef u32				tls_key_id_type;
#elif defined(__GNUC__) // #if defined(_MSC_VER)
	typedef _Tlskey_t		tls_key_id_type;
#else // #elif defined(__GNUC__)
#	error unknown platform!
#endif // #if defined(_MSC_VER)

VOSTOK_CORE_API	tls_key_id_type	tls_create_key				( );
VOSTOK_CORE_API	void			tls_delete_key				( tls_key_id_type key );
VOSTOK_CORE_API	void			tls_set_value				( tls_key_id_type key, pvoid value );
VOSTOK_CORE_API	pvoid			tls_get_value				( tls_key_id_type key );
VOSTOK_CORE_API	bool			tls_is_valid_key			( tls_key_id_type key );
VOSTOK_CORE_API	tls_key_id_type	tls_get_invalid_key			( );

} // namespace threading
} // namespace vostok

// CPU+compiler reorder preventing macro:
#if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_MEMORY_BARRIER_EXCEPT_READS_BEFORE_WRITES	MemoryBarrier
#	define VOSTOK_MEMORY_BARRIER_FULL							MemoryBarrier
#elif VOSTOK_PLATFORM_XBOX_360 // #if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_MEMORY_BARRIER_EXCEPT_READS_BEFORE_WRITES	__lwsync
#	define VOSTOK_MEMORY_BARRIER_FULL							__sync
#elif VOSTOK_PLATFORM_PS3 // #elif VOSTOK_PLATFORM_XBOX_360
#	include <ppu_intrinsics.h>
#	define VOSTOK_MEMORY_BARRIER_EXCEPT_READS_BEFORE_WRITES	__lwsync
#	define VOSTOK_MEMORY_BARRIER_FULL							__sync
#else
#	error please define VOSTOK_MEMORY_BARRIER_FULL for your platform
#endif // #if VOSTOK_PLATFORM_WINDOWS

#include <vostok/threading_functions_guard.h>

#ifdef NDEBUG
#	define	VOSTOK_THREADING_INLINE	inline
#	if VOSTOK_PLATFORM_WINDOWS
#		include <vostok/threading_functions_win_inline.h>
#	elif VOSTOK_PLATFORM_XBOX_360 // #if VOSTOK_PLATFORM_WINDOWS
#		include <vostok/threading_functions_xbox_inline.h>
#	elif VOSTOK_PLATFORM_PS3 // #elif VOSTOK_PLATFORM_XBOX_360
#		include <vostok/threading_functions_ps3_inline.h>
#	else // #elif VOSTOK_PLATFORM_PS3
#		error "create an implementation of threading functions for your platform"
#	endif // #ifdef VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360
#	undef	VOSTOK_THREADING_INLINE
#endif // #ifdef NDEBUG

#endif // #ifndef VOSTOK_THREADING_FUNCTIONS_H_INCLUDED