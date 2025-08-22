////////////////////////////////////////////////////////////////////////////
//	Created		: 06.11.2009
//	Author		: Sergey Chechin, Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_FIXED_SIZE_ALLOCATOR_INLINE_H_INCLUDED
#define VOSTOK_MEMORY_FIXED_SIZE_ALLOCATOR_INLINE_H_INCLUDED

namespace vostok {
namespace memory {

template < class T, class ThreadPolicy >
fixed_size_allocator< T, ThreadPolicy >::fixed_size_allocator		( ) :
	m_arena_id			( 0 )
{
}

template < class T, class ThreadPolicy >
fixed_size_allocator< T, ThreadPolicy >::~fixed_size_allocator		( )
{
	VOSTOK_DESTROY_REFERENCE			( m_allocator );
}

template < class T, class ThreadPolicy >
inline void fixed_size_allocator< T, ThreadPolicy >::initialize_impl(
		pvoid arena,
		u64 size,
		pcstr arena_id
	)
{	
	m_arena_id						= arena_id;
	pbyte const aligned_arena		= (pbyte)math::align_up( (size_t)arena, (size_t)VOSTOK_DEFAULT_ALIGN_SIZE );
	VOSTOK_CONSTRUCT_REFERENCE		( m_allocator, single_size_allocator )( aligned_arena, (size_t)size );
}

template < class T, class ThreadPolicy >
pvoid fixed_size_allocator< T, ThreadPolicy >::allocate ()
{
	return							m_allocator->allocate();
}

template < class T, class ThreadPolicy >
inline pvoid fixed_size_allocator< T, ThreadPolicy >::call_malloc ( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	VOSTOK_CORE_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
	VOSTOK_UNREFERENCED_PARAMETER		( VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_PARAMETER );
	R_ASSERT_U						( size == sizeof( T ) );
	return							allocate();
}

template < class T, class ThreadPolicy >
inline pvoid fixed_size_allocator< T, ThreadPolicy >::call_realloc (
		pvoid pointer,
		size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION
	)
{
	VOSTOK_CORE_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
	VOSTOK_UNREFERENCED_PARAMETER		( VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_PARAMETER );
	VOSTOK_UNREFERENCED_PARAMETERS	( new_size, pointer );
	NOT_IMPLEMENTED					( return 0 );
}

template < class T, class ThreadPolicy >
inline void fixed_size_allocator< T, ThreadPolicy >::call_free ( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	VOSTOK_CORE_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
	deallocate						( pointer );
}

template < class T, class ThreadPolicy >
void fixed_size_allocator< T, ThreadPolicy >::deallocate	( pvoid& data )
{
	m_allocator->deallocate			( data );
}

} // namespace memory
} // namespace vostok

#endif // #ifndef VOSTOK_MEMORY_FIXED_SIZE_ALLOCATOR_INLINE_H_INCLUDED