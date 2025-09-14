////////////////////////////////////////////////////////////////////////////
//	Created		: 18.05.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADER_COMPILER_MEMORY_H_INCLUDED
#define SHADER_COMPILER_MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

namespace vostok {
namespace shader_compiler {

extern	vostok::memory::doug_lea_allocator_type	g_allocator;

#define USER_ALLOCATOR							g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace shader_compiler
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		::vostok::shader_compiler::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	::vostok::shader_compiler::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	::vostok::shader_compiler::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	::vostok::shader_compiler::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		::vostok::shader_compiler::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	::vostok::shader_compiler::g_allocator, type, count )

#endif // #ifndef SHADER_COMPILER_MEMORY_H_INCLUDED