////////////////////////////////////////////////////////////////////////////
//	Created		: 03.08.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#if 1//VOSTOK_DEBUG_ALLOCATOR
#if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR
#include "memory_leak_detector.h"
#include <vostok/memory_doug_lea_mt_allocator.h>

using vostok::memory::doug_lea_mt_allocator;

class mutex_mt_raii : private boost::noncopyable {
public:
	explicit	mutex_mt_raii	( doug_lea_mt_allocator const& instance ) :
		m_instance			( instance ),
		m_is_tasks_aware	( instance.is_tasks_aware() )
	{
		if ( m_is_tasks_aware )
			m_instance.mutex().lock					( );
		else
			m_instance.mutex_tasks_unaware().lock	( );

		m_instance.user_current_thread_id	( );
	}

	inline	~mutex_mt_raii	( )
	{
		if ( m_is_tasks_aware )
			m_instance.mutex().unlock				( );
		else
			m_instance.mutex_tasks_unaware().unlock	( );
	}

private:
	doug_lea_mt_allocator const&	m_instance;
	const bool						m_is_tasks_aware;
}; // class mutex_mt_raii

STATIC_SIZE_ASSERT(mutex_mt_raii, 0x8);

doug_lea_mt_allocator::doug_lea_mt_allocator(
		bool const crash_after_out_of_memory,
		bool const return_null_after_out_of_memory,
		bool use_guards,
		bool is_tasks_aware
	) :
	super	(
		vostok::memory::thread_id_const_false,
		crash_after_out_of_memory,
		return_null_after_out_of_memory,
		use_guards
	),
	m_is_tasks_aware	( is_tasks_aware )
{
}

pvoid doug_lea_mt_allocator::malloc_impl	( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	mutex_mt_raii	guard( *this );
	return		super::malloc_impl( size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS );
}

pvoid doug_lea_mt_allocator::realloc_impl	( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	mutex_mt_raii	guard( *this );
	return		super::realloc_impl( pointer, new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS );
}

void doug_lea_mt_allocator::free_impl		( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	mutex_mt_raii	guard( *this );
	super::free_impl( pointer VOSTOK_CORE_DEBUG_PARAMETERS );
}

pvoid doug_lea_mt_allocator::call_malloc	( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	return		( malloc_impl( size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS ) );
}

pvoid doug_lea_mt_allocator::call_realloc	( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	return		( realloc_impl( pointer, new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS ) );
}

void doug_lea_mt_allocator::call_free		( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	free_impl	( pointer VOSTOK_CORE_DEBUG_PARAMETERS );
}

void doug_lea_mt_allocator::initialize		( pvoid arena, u64 arena_size, pcstr arena_id )
{
	if ( !arena )
		return;

	super::initialize	( arena, arena_size, arena_id );
}

void doug_lea_mt_allocator::initialize_impl	( pvoid arena, u64 arena_size, pcstr arena_id )
{
	if ( !arena )
		return;

	mutex_mt_raii	guard( *this );
	super::initialize_impl	( arena, arena_size, arena_id );
}

void doug_lea_mt_allocator::finalize_impl	( )
{
	mutex_mt_raii	guard( *this );
	super::finalize_impl	( );
}

size_t doug_lea_mt_allocator::total_size	( ) const
{
	mutex_mt_raii	guard( *this );
	return	super::total_size( );
}

size_t doug_lea_mt_allocator::allocated_size( ) const
{
	mutex_mt_raii	guard( *this );
	return	super::allocated_size( );
}

#endif // #if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR
#endif // #if VOSTOK_DEBUG_ALLOCATOR
