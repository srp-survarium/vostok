////////////////////////////////////////////////////////////////////////////
//	Created		: 17.01.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_SINGLE_THREADING_SINGLE_SIZE_ALLOCATOR_POLICY_H_INCLUDED
#define VOSTOK_MEMORY_SINGLE_THREADING_SINGLE_SIZE_ALLOCATOR_POLICY_H_INCLUDED

namespace vostok {
namespace memory {

template < typename T >
struct single_threading_single_size_allocator_policy
{	
	class free_list_type {
	public:
		inline void	set_pointer ( T* p )	{ pointer = p; }
		inline T*	get_pointer	( ) const	{ return pointer; }

	private:
		T*			pointer;
	}; // class free_list_type

	typedef u32		counter_type;

	static inline void	initialize	( free_list_type& free_list_head );
	static inline T*	allocate	( free_list_type& free_list_head );
	static inline void	deallocate	( free_list_type& free_list_head, free_list_type& freeing_node );

	static inline void	increment	( counter_type& operand );
	static inline void	decrement	( counter_type& operand );
}; // struct single_threading_single_size_allocator_policy

} // namespace memory
} // namespace vostok

#include <vostok/memory_single_threading_single_size_allocator_policy_inline.h>

#endif // #ifndef VOSTOK_MEMORY_SINGLE_THREADING_SINGLE_SIZE_ALLOCATOR_POLICY_H_INCLUDED
