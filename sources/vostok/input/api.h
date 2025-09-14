////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_INPUT_API_H_INCLUDED
#define VOSTOK_INPUT_API_H_INCLUDED

#ifndef VOSTOK_INPUT_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_INPUT_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_INPUT_BUILDING
#			define VOSTOK_INPUT_API			VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_INPUT_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_INPUT_API		VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_INPUT_API		VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_INPUT_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_INPUT_API

#ifndef _MSC_VER
	typedef pvoid HWND;
#endif // #ifndef _MSC_VER

namespace vostok {
namespace input {

struct engine;
struct world;

typedef memory::doug_lea_allocator_type		allocator_type;

VOSTOK_INPUT_API	world*	create_world		( engine& engine, HWND window_handle );
VOSTOK_INPUT_API	void	destroy_world		( world*& world );
VOSTOK_INPUT_API	void	set_memory_allocator( allocator_type& allocator );

} // namespace input
} // namespace vostok

#endif // #ifndef VOSTOK_INPUT_API_H_INCLUDED