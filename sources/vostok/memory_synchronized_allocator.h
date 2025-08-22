////////////////////////////////////////////////////////////////////////////
//	Created		: 11.01.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_SYNCHRONIZED_ALLOCATOR_H_INCLUDED
#define VOSTOK_MEMORY_SYNCHRONIZED_ALLOCATOR_H_INCLUDED

#include <vostok/memory_debug_parameters.h>

namespace vostok {
namespace memory {

template <class Allocator>
class memory_synchronized_allocator : public Allocator
{
public:	
	typedef	Allocator	super;
public:	
					memory_synchronized_allocator () 
						: m_synchronization_enabled(true), Allocator(thread_id_const_false) {}

			void	enable_synchronization	() { m_synchronization_enabled	=	true; }
			void	disable_synchronization	() { m_synchronization_enabled	=	false; }
			bool    synchronization_enabled () const { return m_synchronization_enabled; }

	virtual pvoid	malloc_impl		( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION  )
	{
		if ( synchronization_enabled() )
			m_mutex.lock				();
		
		super::user_current_thread_id	();
		pvoid out_result = super::malloc_impl(size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS);
		
		if ( synchronization_enabled() )
			m_mutex.unlock				();
		return							out_result;
	}

	virtual pvoid	realloc_impl	( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
	{
		if ( synchronization_enabled() )
			m_mutex.lock				();

		super::user_current_thread_id	();
		pvoid out_result = super::realloc_impl(pointer, new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS);

		if ( synchronization_enabled() )
			m_mutex.unlock				();
		return							out_result;
	}

	virtual void	free_impl		( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
	{
		if ( synchronization_enabled() )
			m_mutex.lock				();

		super::user_current_thread_id	();
		super::free_impl		(pointer VOSTOK_CORE_DEBUG_PARAMETERS);

		if ( synchronization_enabled() )
			m_mutex.unlock				();
	}

	pvoid call_malloc	( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
	{
		return							malloc_impl(size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS);
	}

	pvoid call_realloc	( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
	{
		return							realloc_impl(pointer, new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS);
	}

	void call_free		( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
	{
		free_impl						(pointer VOSTOK_CORE_DEBUG_PARAMETERS);
	}

private:
	threading::mutex			m_mutex;
	bool						m_synchronization_enabled;
}; // class memory_synchronized_allocator

} // namespace memory
} // namespace vostok

#endif // #ifndef VOSTOK_MEMORY_SYNCHRONIZED_ALLOCATOR_H_INCLUDED