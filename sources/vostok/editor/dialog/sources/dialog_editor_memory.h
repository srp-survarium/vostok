////////////////////////////////////////////////////////////////////////////
//	Created		: 26.05.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef DIALOG_EDITOR_MEMORY_H_INCLUDED
#define DIALOG_EDITOR_MEMORY_H_INCLUDED

#include <vostok/editor/dialog/api.h>

namespace vostok {
namespace dialog_editor {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::dialog_editor::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace dialog_editor
} // namespace vostok

#define NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::dialog_editor::g_allocator, type )
#define DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::dialog_editor::g_allocator, pointer )
#define MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::dialog_editor::g_allocator, size, description )
#define REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::dialog_editor::g_allocator, pointer, size, description )
#define FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::dialog_editor::g_allocator, pointer )
#define ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::dialog_editor::g_allocator, type, count )

#endif // #ifndef DIALOG_EDITOR_MEMORY_H_INCLUDED