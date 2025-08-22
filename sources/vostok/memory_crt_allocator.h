////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_DEFAULT_ALLOCATOR_H_INCLUDED
#define MEMORY_DEFAULT_ALLOCATOR_H_INCLUDED

#include <vostok/memory_base_allocator.h>
#include <vostok/memory_debug_parameters.h>

namespace vostok {
namespace memory {

class VOSTOK_CORE_API crt_allocator : public base_allocator {
public:
					crt_allocator		( );
			pvoid	malloc_impl			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			pvoid	realloc_impl		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			void	free_impl			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	size_t	allocated_size		( ) const;
	virtual	size_t	usable_size_impl	( pvoid pointer ) const;

private:
	typedef	base_allocator				super;

protected:
	virtual	pvoid	call_malloc			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	pvoid	call_realloc		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	void	call_free			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );

private:
	virtual	void	initialize_impl		( pvoid arena, u64 arena_size, pcstr arena_id );
	virtual	void	finalize_impl		( );
	virtual	size_t	total_size			( ) const;

private:
	typedef pvoid (*malloc_ptr_type)( size_t );
	typedef void (*free_ptr_type)( pvoid );
	typedef pvoid (*realloc_ptr_type)( pvoid, size_t );

private:
	malloc_ptr_type		m_malloc_ptr;
	free_ptr_type		m_free_ptr;
	realloc_ptr_type	m_realloc_ptr;
}; // class crt_allocator

} // namespace memory
} // namespace vostok

#define CRT_NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::memory::g_crt_allocator, type )
#define CRT_DELETE( pointer, ... )					VOSTOK_DELETE_IMPL(	*::vostok::memory::g_crt_allocator, pointer, __VA_ARGS__ )
#define CRT_MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::memory::g_crt_allocator, size, description )
#define CRT_REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::memory::g_crt_allocator, pointer, size, description )
#define CRT_FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::memory::g_crt_allocator, pointer )
#define CRT_ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::memory::g_crt_allocator, type, count )

#endif // #ifndef MEMORY_DEFAULT_ALLOCATOR_H_INCLUDED