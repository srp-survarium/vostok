////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_EDITOR_ALLOCATOR_H_INCLUDED
#define VOSTOK_RENDER_BASE_EDITOR_ALLOCATOR_H_INCLUDED

#include <vostok/render/api.h>

namespace vostok {
namespace render {
namespace editor {

VOSTOK_RENDER_API extern editor_allocator_type*	g_allocator;

#define USER_ALLOCATOR							*::vostok::render::editor::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace editor
} // namespace render
} // namespace vostok

#define E_NEW( type )							VOSTOK_NEW_IMPL(		*::vostok::render::editor::g_allocator, type )
#define E_DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::render::editor::g_allocator, pointer )
#define E_MALLOC( size, description )			VOSTOK_MALLOC_IMPL(	*::vostok::render::editor::g_allocator, size, description )
#define E_REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::render::editor::g_allocator, pointer, size, description )
#define E_FREE( pointer )						VOSTOK_FREE_IMPL(		*::vostok::render::editor::g_allocator, pointer )
#define E_ALLOC( type, count )					VOSTOK_ALLOC_IMPL(	*::vostok::render::editor::g_allocator, type, count )

#endif // #ifndef VOSTOK_RENDER_BASE_EDITOR_ALLOCATOR_H_INCLUDED