////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_MACROS_H_INCLUDED
#define VOSTOK_MEMORY_MACROS_H_INCLUDED

#ifdef DEBUG

#	define VOSTOK_NEW_IMPL( allocator, type )								\
			new ( vostok::memory::new_helper<type>::call( vostok::memory::strip_pointer(allocator), __FUNCTION__, __FILE__, __LINE__ ) ) ( type )

#	define VOSTOK_NEW_ARRAY_IMPL( allocator, type, count )				\
			vostok::memory::new_array_helper<type>::call( vostok::memory::strip_pointer(allocator), count, __FUNCTION__, __FILE__, __LINE__ )

#	define VOSTOK_DELETE_IMPL( allocator, pointer, ... )					\
			vostok::memory::delete_helper			( vostok::memory::strip_pointer(allocator), pointer, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__ )

#	define VOSTOK_DELETE_ARRAY_IMPL( allocator, pointer, ... )			\
			vostok::memory::delete_array_helper	( vostok::memory::strip_pointer(allocator), pointer, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__ )

#	define VOSTOK_MALLOC_IMPL( allocator, size, description )				\
			vostok::memory::malloc_helper			( vostok::memory::strip_pointer(allocator), size, description, __FUNCTION__, __FILE__, __LINE__ )

#	define VOSTOK_REALLOC_IMPL( allocator, pointer, size, description )	\
			vostok::memory::realloc_helper		( vostok::memory::strip_pointer(allocator), pointer, size, description, __FUNCTION__, __FILE__, __LINE__ )

#	define VOSTOK_FREE_IMPL( allocator, pointer )							\
			vostok::memory::free_helper			( vostok::memory::strip_pointer(allocator), pointer, __FUNCTION__, __FILE__, __LINE__ )

#else // #ifdef DEBUG

#	define VOSTOK_NEW_IMPL( allocator, type )								\
			new ( vostok::memory::new_helper<type>::call( vostok::memory::strip_pointer(allocator) ) ) ( type ) 

#	define VOSTOK_NEW_ARRAY_IMPL( allocator, type, count )				\
			vostok::memory::new_array_helper<type>::call( vostok::memory::strip_pointer(allocator), count )

#	define VOSTOK_DELETE_IMPL( allocator, pointer, ... )					\
			vostok::memory::delete_helper			( vostok::memory::strip_pointer(allocator), pointer, ##__VA_ARGS__ )

#	define VOSTOK_DELETE_ARRAY_IMPL( allocator, pointer, ... )			\
			vostok::memory::delete_array_helper	( vostok::memory::strip_pointer(allocator), pointer, ##__VA_ARGS__ )

#	define VOSTOK_MALLOC_IMPL( allocator, size, description )				\
			vostok::memory::malloc_helper			( vostok::memory::strip_pointer(allocator), size )

#	define VOSTOK_REALLOC_IMPL( allocator, pointer, size, description )	\
			vostok::memory::realloc_helper		( vostok::memory::strip_pointer(allocator), pointer, size )

#	define VOSTOK_FREE_IMPL( allocator, pointer )							\
			vostok::memory::free_helper			( vostok::memory::strip_pointer(allocator), pointer )

#endif // #ifdef DEBUG

#if VOSTOK_PLATFORM_PS3
#	define VOSTOK_ALLOC_IMPL( allocator, type, count )						\
				( ( type* )VOSTOK_MALLOC_IMPL( allocator, sizeof( type )*( count ), typeid( type ).name( ) ) )
#else // #if VOSTOK_PLATFORM_PS3
#	define VOSTOK_ALLOC_IMPL( allocator, type, count )						\
				( ( type* )VOSTOK_MALLOC_IMPL( allocator, sizeof( type )*( count ), typeid( type ).raw_name( ) ) )
#endif // #if VOSTOK_PLATFORM_PS3

#define VOSTOK_NEW_WITH_CHECK_IMPL(allocator, out_pointer, type)	(((out_pointer) = (type*)VOSTOK_ALLOC_IMPL(allocator, type, 1)) == 0) ? 0 : new (out_pointer) type

#endif // #ifndef VOSTOK_MEMORY_MACROS_H_INCLUDED