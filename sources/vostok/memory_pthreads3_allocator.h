////////////////////////////////////////////////////////////////////////////
//	Created 	: 30.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_PTHREADS3_ALLOCATOR_H_INCLUDED
#define VOSTOK_MEMORY_PTHREADS3_ALLOCATOR_H_INCLUDED

#include <vostok/memory_base_allocator.h>
#include <vostok/memory_debug_parameters.h>

namespace vostok {
namespace memory {

#if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR
class VOSTOK_CORE_API pthreads3_allocator : public base_allocator {
public:
			pvoid	malloc_impl			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			pvoid	realloc_impl		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			void	free_impl			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );

private:
	typedef	base_allocator				super;

private:
	virtual	pvoid	call_malloc			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	pvoid	call_realloc		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	void	call_free			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	void	initialize_impl		( pvoid arena, u64 arena_size, pcstr arena_id );
	virtual	void	finalize_impl		( );
	virtual	size_t	total_size			( ) const;
	virtual	size_t	allocated_size		( ) const;
	virtual	size_t	usable_size_impl	( pvoid pointer ) const;
}; // class pthreads3_allocator

#else // #if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR

typedef crt_allocator pthreads3_allocator;

#endif // #if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR

} // namespace memory
} // namespace vostok

#endif // #ifndef VOSTOK_MEMORY_PTHREADS3_ALLOCATOR_H_INCLUDED