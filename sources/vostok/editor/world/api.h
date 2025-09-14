////////////////////////////////////////////////////////////////////////////
//	Created 	: 31.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_EDITOR_API_H_INCLUDED
#define VOSTOK_EDITOR_API_H_INCLUDED

#include <vostok/memory_pthreads3_allocator.h>
#include <vostok/memory_allocator_helper.h>

#ifndef VOSTOK_EDITOR_API

#	ifndef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_EDITOR_BUILDING
#			define VOSTOK_EDITOR_API	VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_EDITOR_BUILDING
#			define VOSTOK_EDITOR_API	VOSTOK_DLL_IMPORT
#		endif // #ifdef VOSTOK_EDITOR_BUILDING
#	else // #ifndef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_EDITOR_API
#	endif // #ifndef VOSTOK_STATIC_LIBRARIES

#	define VOSTOK_EDITOR_API_C		extern "C" VOSTOK_EDITOR_API
#endif // #ifndef VOSTOK_EDITOR_API

namespace vostok {
namespace editor {

	struct engine;
	struct world;

	typedef memory::doug_lea_allocator				allocator_type;

	typedef world* ( *create_world_ptr )			( engine& engine );
	VOSTOK_EDITOR_API_C	world*	create_world		( engine& engine );

	typedef void ( *destroy_world_ptr )				( world*& world );
	VOSTOK_EDITOR_API_C	void	destroy_world		( world*& world );

	typedef void ( *memory_allocator_ptr )			( allocator_type& allocator );
	VOSTOK_EDITOR_API_C	void	set_memory_allocator( allocator_type& allocator );

} // namespace editor
} // namespace vostok

#endif // #ifndef VOSTOK_EDITOR_API_H_INCLUDED