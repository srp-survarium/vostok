////////////////////////////////////////////////////////////////////////////
//	Created		: 15.04.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_STACK_ALLOCATOR_H_INCLUDED
#define VOSTOK_MEMORY_STACK_ALLOCATOR_H_INCLUDED

#include <vostok/memory_base_allocator.h>

namespace vostok {
namespace memory {

class VOSTOK_CORE_API stack_allocator : public base_allocator
{
public:
					stack_allocator		( );

	inline	void	reset				( ) { m_arena_current_position	= m_arena_start;}

	virtual	size_t	total_size			( ) const;
	virtual	size_t	allocated_size		( ) const;
			
			pvoid	malloc_impl			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			pvoid	realloc_impl		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			void	free_impl			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );

protected:
	virtual	pvoid	call_malloc			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	pvoid	call_realloc		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	void	call_free			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	
	virtual	void	initialize_impl		( pvoid arena, u64 size, pcstr arena_id );
	virtual	void	finalize_impl		( );
	virtual	size_t	usable_size_impl	( pvoid pointer ) const;

private:
	pvoid	m_arena_current_position;
}; // class stack_allocator

} // namespace memory
} // namespace vostok

#endif // #ifndef VOSTOK_MEMORY_STACK_ALLOCATOR_H_INCLUDED