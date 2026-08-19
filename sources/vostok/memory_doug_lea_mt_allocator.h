////////////////////////////////////////////////////////////////////////////
//	Created		: 03.08.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_DOUG_LEA_MT_ALLOCATOR_H_INCLUDED
#define VOSTOK_MEMORY_DOUG_LEA_MT_ALLOCATOR_H_INCLUDED

#if 1//VOSTOK_DEBUG_ALLOCATOR
#include <vostok/memory_doug_lea_allocator.h>
#include <vostok/threading_mutex.h>

namespace vostok {
namespace memory {

#if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR
class VOSTOK_CORE_API doug_lea_mt_allocator : public doug_lea_allocator {
public:
				doug_lea_mt_allocator	(
						bool const crash_after_out_of_memory = true,
						bool const return_null_after_out_of_memory = false,
						bool use_guards = true,
						bool is_tasks_aware = false
					);
			pvoid	malloc_impl			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			pvoid	realloc_impl		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
			void	free_impl			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	inline	bool	is_tasks_aware		( ) const { return m_is_tasks_aware; }
	inline	threading::mutex& mutex		( ) const { return m_mutex; }
	inline	threading::mutex_tasks_unaware&	mutex_tasks_unaware	( ) const { return m_mutex_tasks_unaware; }
	virtual	void	initialize			( pvoid arena, u64 arena_size, pcstr arena_id );

private:
	typedef	doug_lea_allocator			super;

protected:
	virtual	pvoid	call_malloc			( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	pvoid	call_realloc		( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );
	virtual	void	call_free			( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION );

private:
	virtual	void	initialize_impl		( pvoid arena, u64 arena_size, pcstr arena_id );
	virtual	void	finalize_impl		( );
	virtual	size_t	total_size			( ) const;
	virtual	size_t	allocated_size		( ) const;

public:
	u64							dummy;

private:
	mutable threading::mutex				m_mutex;
	mutable threading::mutex_tasks_unaware	m_mutex_tasks_unaware;
	bool									m_is_tasks_aware;
}; // class doug_lea_allocator

STATIC_SIZE_ASSERT(doug_lea_mt_allocator, 0x70);

#else // #if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR

typedef crt_allocator doug_lea_mt_allocator;

#endif // #if !VOSTOK_USE_CRT_MEMORY_ALLOCATOR

} // namespace memory
} // namespace vostok
#endif // #if VOSTOK_DEBUG_ALLOCATOR

#endif // #ifndef VOSTOK_MEMORY_DOUG_LEA_MT_ALLOCATOR_H_INCLUDED
