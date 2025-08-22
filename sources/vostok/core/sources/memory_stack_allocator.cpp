////////////////////////////////////////////////////////////////////////////
//	Created		: 15.04.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <vostok/memory_stack_allocator.h>

namespace vostok {
namespace memory {

stack_allocator::stack_allocator	( ) :
	m_arena_current_position		( 0 )
{
}

size_t stack_allocator::total_size	( ) const
{
	pbyte const arena_end			= static_cast< pbyte >( m_arena_end );
	pbyte const arena_start			= static_cast< pbyte >( m_arena_start );
	return							arena_end - arena_start;
}

size_t stack_allocator::allocated_size	( ) const
{
	pbyte const arena_position		= static_cast< pbyte >( m_arena_current_position );
	pbyte const arena_start			= static_cast< pbyte >( m_arena_start );
	return							arena_position - arena_start;
}

pvoid stack_allocator::call_malloc	( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	return							( malloc_impl( size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS ) );
}

pvoid stack_allocator::call_realloc	( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	return							( realloc_impl( pointer, new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS ) );
}

void stack_allocator::call_free		( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	free_impl						( pointer VOSTOK_CORE_DEBUG_PARAMETERS );
}

void stack_allocator::initialize_impl	( pvoid arena, u64 size, pcstr arena_id )
{
	m_arena_current_position			= arena;
	VOSTOK_UNREFERENCED_PARAMETERS		( size, arena_id );
}

void stack_allocator::finalize_impl		( )
{
}

pvoid stack_allocator::malloc_impl	( size_t size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	VOSTOK_CORE_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
	VOSTOK_UNREFERENCED_PARAMETER		( VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_PARAMETER );
	
	pbyte const arena_end			= static_cast< pbyte >( m_arena_end );
	pbyte const arena_position		= static_cast< pbyte >( m_arena_current_position );
	pbyte const new_position		= arena_position + size;
	pvoid const current_position	= m_arena_current_position;
	R_ASSERT_U						( new_position <= arena_end, "stack_allocator - out of memory!");
	m_arena_current_position		= new_position;
	return							current_position;
}

pvoid stack_allocator::realloc_impl	( pvoid pointer, size_t new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	pvoid result					= malloc_impl( new_size VOSTOK_CORE_DEBUG_PARAMETERS_DESCRIPTION VOSTOK_CORE_DEBUG_PARAMETERS );
	if ( pointer )
		memory::copy				( result, new_size, pointer, new_size );
	free_impl						( pointer VOSTOK_CORE_DEBUG_PARAMETERS );
	LOG_WARNING						( "realloc has been called on stack allocator, is it intended behavior?" );
	return							result;
}

void stack_allocator::free_impl		( pvoid pointer VOSTOK_CORE_DEBUG_PARAMETERS_DECLARATION )
{
	VOSTOK_CORE_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
	VOSTOK_UNREFERENCED_PARAMETERS	( pointer );
}

size_t stack_allocator::usable_size_impl( pvoid ) const
{
	NOT_IMPLEMENTED					( return 0 );
}

} // namespace memory
} // namespace vostok