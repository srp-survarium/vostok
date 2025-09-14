////////////////////////////////////////////////////////////////////////////
//	Created		: 14.01.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_SINGLE_SIZE_FIXED_ALLOCATOR_INLINE_H_INCLUDED
#define VOSTOK_MEMORY_SINGLE_SIZE_FIXED_ALLOCATOR_INLINE_H_INCLUDED

namespace vostok {
namespace memory {

template < int DataSize, int Count, typename ThreadingPolicy >
inline single_size_fixed_allocator< DataSize, Count, ThreadingPolicy >::single_size_fixed_allocator() :
	m_single_size_allocator		( m_buffer, DataSize * Count )
{
}

template < int DataSize, int Count, typename ThreadingPolicy >
inline pvoid single_size_fixed_allocator< DataSize, Count, ThreadingPolicy >::malloc_impl( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	return m_single_size_allocator.malloc_impl( size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS );
}

template < int DataSize, int Count, typename ThreadingPolicy >
inline void single_size_fixed_allocator< DataSize, Count, ThreadingPolicy >::free_impl( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	m_single_size_allocator.free_impl( pointer VOSTOK_CORE_DEBUG_PARAMETERS );
}

} // namespace memory
} // namespace vostok

#endif // #ifndef VOSTOK_MEMORY_SINGLE_SIZE_FIXED_ALLOCATOR_INLINE_H_INCLUDED