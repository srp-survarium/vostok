////////////////////////////////////////////////////////////////////////////
//	Created		: 21.01.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef MODEL_EDITOR_MEMORY_H_INCLUDED
#define MODEL_EDITOR_MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>


namespace vostok {
namespace model_editor {

extern memory::doug_lea_allocator_type*				g_allocator;

#define USER_ALLOCATOR				*::vostok::model_editor::g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace model_editor
} // namespace vostok

#define NEW( type )                              VOSTOK_NEW_IMPL(*::vostok::model_editor::g_allocator, type )
#define DELETE( pointer )                        VOSTOK_DELETE_IMPL(*::vostok::model_editor::g_allocator, pointer )
#define MALLOC( size, description )              VOSTOK_MALLOC_IMPL(*::vostok::model_editor::g_allocator, size, description )
#define REALLOC( pointer, size, description )    VOSTOK_REALLOC_IMPL(*::vostok::model_editor::g_allocator, pointer, size, description )
#define FREE( pointer )                          VOSTOK_FREE_IMPL(*::vostok::model_editor::g_allocator, pointer )
#define ALLOC( type, count )                     VOSTOK_ALLOC_IMPL(*::vostok::model_editor::g_allocator, type, count )

#endif // #ifndef MODEL_EDITOR_MEMORY_H_INCLUDED